#ifndef BSITERATOR_H
#define BSITERATOR_H

#include "../core_computation.h"
#include "../dev_tools.h"
#include "../own_math.h"
#include "../type_class.h"

namespace SpaceH {

    template<typename TypeClass, size_t MaxIter>
    class BStab {
    public:
        /* Typedef */
        using type         = TypeClass;
        using Scalar       = typename type::Scalar;

        template<typename T, size_t S>
        using Container = typename type::template Container<T, S>;
        /* Typedef */

        inline Scalar expon(size_t i) const {
            return err_expon_[i];
        }
        inline Scalar limiter(size_t i) const {
            return limiter_[i];
        }
        inline Scalar work(size_t i) const {
            return work_[i];
        }
        inline size_t step(size_t i) const {
            return sub_steps_[i];
        }
        inline Scalar coef(size_t k) const {
            return extrap_coef_[k];
        }
        inline Scalar coef(size_t i, size_t j) const {
            return extrap_coef_[at(i,j)];
        }

        BStab() {
            for (size_t i = 0; i < MaxIter; ++i) {
                if(i == 0) {
                    sub_steps_[i] = 1;
                    work_[i]      = 2;
                } else {
                    sub_steps_[i] = 2 * i;
                    work_[i]      = work_[i - 1] + sub_steps_[i] + 1;
                }
                err_expon_[i] = static_cast<Scalar>(1.0 / (2 * i + 1));
                limiter_[i]   = pow(0.02, err_expon_[i]);

                for (size_t j = 0; j < i; ++j) {
                    Scalar ratio = static_cast<Scalar>(sub_steps_[i]) / static_cast<Scalar>(sub_steps_[i - j - 1]);
                    extrap_coef_[at(i, j)] = 1.0 / (ratio * ratio - 1);
                }
            }
        }
    private:
        inline size_t at(size_t i, size_t j) const {
            return i * (i + 1) / 2 + j;
        }

        /** @brief Extrapolation coefficient.*/
        Container<Scalar, MaxIter * (MaxIter + 1) / 2> extrap_coef_;

        /** @brief The exponent of error estimate at column k.*/
        Container<Scalar, MaxIter> err_expon_;

        /** @brief The minimal coeffient of integration step estimation.*/
        Container<Scalar, MaxIter> limiter_;

        /** @brief The work(computation resource) per step size of each iteration depth.*/
        Container<size_t, MaxIter> work_;

        /** @brief Steps of integration of each iteration depth.*/
        Container<size_t, MaxIter> sub_steps_;
    };

/** @brief Bulirsch-Stoer extrapolation algorithm*/
    template<typename ParticSys, typename Integrator>
    class BSIterator {
    private:

    public:
        /* Typedef */
        using type   = typename ParticSys::type;
        using Scalar = typename type::Scalar;
        using ScalarBuffer = typename type::ScalarBuffer;

        template<typename T, size_t S>
        using Container = typename type::template Container<T, S>;
        /* Typedef */

        /*Template parameter check*/
        CHECK_TYPE(ParticSys, Integrator)
        /*Template parameter check*/

        /** @brief Interface of ODE iterator
         *  @note  BSiterator will force use the internal mid-point integrator as the basic integrator.
         */
        Scalar iterate(ParticSys &particles, Scalar stepLength);

        /**
         * @brief Set the local relative error
         * @param relError
         */
        void setRelativeError(Scalar relError) {
            relativeError_ = relError;
        }

        /**
         *
         * @return
         */
        Scalar getRejRate() {
            return static_cast<float> (rej_num_) / iter_num_;
        }

    private:
        /** @brief The Maximum iteration depth*/
        static constexpr size_t MaxDepth{8};

        /** @brief The local partical system used to iterate.*/
        ParticSys localSystem;

        /** @brief The constat coef for BS extrapolation*/
        BStab<type, MaxDepth + 1> BS;

        /** @brief Extrapolation table.*/
        Container<ScalarBuffer, (MaxDepth + 1) * (MaxDepth + 2) / 2> extrapTab;

        /** @brief The optimal step size array.*/
        Container<Scalar, MaxDepth + 1> optimal_step_coef_;

        /** @brief The work(computation resource) needed to converge at column k.*/
        Container<Scalar, MaxDepth + 1> work_per_len_;

        /** @brief Local absolute error*/
        Scalar absoluteError_{1 * SpaceH::epsilon<Scalar>::value};

        /** @brief Local relative error*/
        Scalar relativeError_{50 * SpaceH::epsilon<Scalar>::value};

        /** @brief Current iteraation depth.*/
        size_t ideal_iter_{7};

        /** @brief Total volume of extrapolation table(in scalar).*/
        size_t extrapTabVolume_{0};

        /** @brief Rejectin number*/
        size_t rej_num_{0};

        /** @brief Total iteration number*/
        size_t iter_num_{0};

        ScalarBuffer initState_;

        void fillFirstColumn(size_t row) {
            size_t center = at(row, 0);
            localSystem.write(extrapTab[center], IO_flag::EVOLVED);//copyDataToExtrapTab;
            /*size_t size = initState_.size();

            for (size_t i = 0; i < size; ++i)
                extrapTab[center][i] -= initState_[i];*/
        }

        void loadResult(ParticSys &particles, size_t iter) {
            size_t center = at(iter, iter);
            /*size_t size = initState_.size();

            for (size_t i = 0; i < size; ++i)
                initState_[i] += extrapTab[center][i];

            particles.read(initState_, IO_flag::EVOLVED);*/
            particles.read(extrapTab[center], IO_flag::EVOLVED);
        }

    private:
        /** @brief Internal integrator */
        Integrator integrator;

        /** @brief Check the rejection criteria for current iteration.*/
        bool divergedInOrderWindow(Scalar error, size_t iter) const;

        /** @brief Extrapolate the kth row to the right end.*/
        void extrapolate(size_t k);

        /** @brief Check/resize the extrapolation table volume.*/
        void checkExtrapVolume();

        /** @brief Calculate the error of the k row of the extrapolation table.*/
        Scalar calcuError(size_t k) const;

        /** @brief Calculate the new iteration integration step coefficient*/
        Scalar calcuIdealStepCoef(Scalar error, size_t k);

        /** @brief Prepare the next iteration.*/
        Scalar prepareNextIteration(size_t iter);

        /** @brief Evolve the local system*/
        void evolveLocalSys(size_t iter, Scalar macroStepSize);

        /** @brief step size reductio after rejection.*/
        inline Scalar stepSizeReduction(size_t iter) {
            if (iter == ideal_iter_ - 1)
                return optimal_step_coef_[iter] * static_cast<Scalar>(BS.work(iter + 1)) / static_cast<Scalar>(BS.work(iter));
            else
                return optimal_step_coef_[ideal_iter_];
        }

        /** @brief check if current iteration can converge in current order window*/
        inline bool isInConvergenceWindow(size_t iter) {
            return iter == ideal_iter_ - 1 || iter == ideal_iter_ || iter == ideal_iter_ + 1;
        }

        /** @brief return convergence column in allowed range*/
        inline size_t allowedRange(size_t i) {
            return  SpaceH::in_range(static_cast<size_t>(2), i, static_cast<size_t>(MaxDepth - 1));
        }

        inline Scalar stepLimiter(Scalar step_coef) {
            return SpaceH::in_range(static_cast<Scalar>(BS.limiter(ideal_iter_)/4), step_coef, static_cast<Scalar>(1.0/BS.limiter(ideal_iter_)));
        }

        inline size_t at(size_t i, size_t j) const {
            return i * (i + 1) / 2 + j;
        }
    };
/**
 *
 * @tparam ParticSys
 * @tparam Integrator
 * @param iter
 * @param macroStepSize
 */
    template<typename ParticSys, typename Integrator>
    void BSIterator<ParticSys, Integrator>::evolveLocalSys(size_t iter, Scalar macroStepSize) {
        size_t steps = BS.step(iter);
        Scalar h = macroStepSize / steps;

        localSystem.kick(0.5 * h);
        for (size_t i = 1; i < steps; i++) {
            localSystem.drift(h);
            localSystem.kick(h);
        }
        localSystem.drift(h);
        localSystem.kick(0.5 * h);
    }

    /**
     * @brief Interface of ODE iterator
     * @tparam ParticSys
     * @tparam Integrator
     * @param  particles   Particle system need iteration.
     * @param  stepLength  Macro integration step length.
     * @return             The next macro integration step length.
     */
    template<typename ParticSys, typename Integrator>
    typename ParticSys::type::Scalar
    BSIterator<ParticSys, Integrator>::iterate(ParticSys &particles, Scalar stepLength) {
        Scalar error = 0;
        Scalar iter_H = stepLength;

        DEBUG_MSG(false, "init StepLen=", iter_H);

        particles.write(initState_, IO_flag::EVOLVED);

        for (;;) {
            iter_num_++;
            localSystem = particles;
            evolveLocalSys(0, iter_H);
            fillFirstColumn(0);
            checkExtrapVolume();

            for (size_t iter = 1; iter <= ideal_iter_ + 1; ++iter) {
                localSystem = particles;
                evolveLocalSys(iter, iter_H);

                fillFirstColumn(iter);
                extrapolate(iter);
                error = calcuError(iter);
                optimal_step_coef_[iter] = calcuIdealStepCoef(error, iter);
                work_per_len_[iter] = BS.work(iter) / optimal_step_coef_[iter];

                DEBUG_MSG(true,
                          "iter=", iter,
                          "optimal=", ideal_iter_,
                          "err=", error,
                          "work=", work_per_len_[iter],
                          "Step Cof =", optimal_step_coef_[iter],
                          "StepLen =", iter_H);

                if (isInConvergenceWindow(iter)) {
                    if (error < 1.0) {
                        DEBUG_MSG(true, "accept");
                        iter_H *= stepLimiter(prepareNextIteration(iter));
                        loadResult(particles, iter);
                        return iter_H;
                    } else if (divergedInOrderWindow(error, iter)) {
                        DEBUG_MSG(true, "reject");
                        rej_num_++;
                        iter_H *= stepLimiter(stepSizeReduction(iter));
                        break;
                    }
                }
            }
        }
    }

/** @brief Check the last_rejected criteria for current iteration.
 *  @param k The kth iteration depth.
 *  @return If current iteration is rejected.
 */
    template<typename ParticSys, typename Integrator>
    bool BSIterator<ParticSys, Integrator>::divergedInOrderWindow(Scalar error, size_t iter) const {
        Scalar r = 1;

        if (iter == ideal_iter_ - 1)
            r = static_cast<Scalar>(BS.step(iter + 1) * BS.step(iter + 2)) /
                static_cast<Scalar>(BS.step(0) * BS.step(0));
        else if (iter == ideal_iter_)
            r = static_cast<Scalar>(BS.step(iter + 1)) / static_cast<Scalar>(BS.step(0));
        else
            return error > 1.0;//k == iterDepth+1 and error >1 reject directly

        return error > r * r;
    }

/** @brief Extrapolate the k-th row to the right end.
 *  @param k The kth row of extrapolation table.
 */
    template<typename ParticSys, typename Integrator>
    void BSIterator<ParticSys, Integrator>::extrapolate(size_t k) {
        size_t curr_row = at(k, 0);
        size_t last_row = at(k - 1, 0);
        size_t size = extrapTab[curr_row].size();
        for (size_t j = 0; j < k; ++j) {
            size_t current = curr_row + j;//
            size_t right = current + 1;
            size_t up = last_row + j;

            for (size_t i = 0; i < size; ++i)
                extrapTab[right][i] = (extrapTab[current][i] - extrapTab[up][i]) * BS.coef(current)
                                    +  extrapTab[current][i];
        }
    }

/** @brief Calculate the error of the k row of the extrapolation table.
 *  @param k The kth row of extrapolation table.
 */
    template<typename ParticSys, typename Integrator>
    typename ParticSys::type::Scalar BSIterator<ParticSys, Integrator>::calcuError(size_t k) const {
        size_t center = at(k, k);
        size_t left = center - 1;
        size_t size = extrapTab[center].size();
        Scalar max_err = 0;

        for (size_t i = 0; i < size; ++i) {
            Scalar d = SpaceH::abs(extrapTab[center][i] - extrapTab[left][i]);
            Scalar scale = SpaceH::max(SpaceH::abs(extrapTab[left][i]   ),
                                       SpaceH::abs(extrapTab[center][i]));
            max_err = SpaceH::max(1.0 * max_err, d / scale);
            DEBUG_MSG(false, i, max_err, d, scale, extrapTab[center][i], extrapTab[left][i], initState_[i]);
        }
        return max_err / relativeError_;
    }

/** @brief Calculate the new iteration integration step coefficient
 *  @param error The error of the k-th row of extrapolation table.
 *  @param order The order of the error in k-th row of extrapolation table.
 *  @return The new iteration integration step length coefficient.
 */
    template<typename ParticSys, typename Integrator>
    typename ParticSys::type::Scalar BSIterator<ParticSys, Integrator>::calcuIdealStepCoef(Scalar error, size_t k) {
        if (error != 0) {
            return 0.9 * pow(0.90 / error, BS.expon(k));
        } else {
            return 1.0 / BS.limiter(k);
        }
            /*return SpaceH::max(BS.limiter(k) / 4,
                               SpaceH::min(0.9 * pow(0.90 / error, BS.expon(k)), 1.0 / BS.limiter(k)));*/
    }

/** @brief Calculate the new iteration depth for next iteration.
 *  @param iter The current iteration index depth.
 *  @param lastRejection The rejection status of last trail of iteration.
 *  @return The new iteration step length.
 */
    template<typename ParticSys, typename Integrator>
    typename ParticSys::type::Scalar BSIterator<ParticSys, Integrator>::prepareNextIteration(size_t iter) {
        switch (static_cast<int>(iter - ideal_iter_)) {
            case -1:
                //ideal_iter_ <= 2 here to avoid none calculated work_per_len[1-1=0]
                if (work_per_len_[iter] < 0.9 * work_per_len_[iter - 1] || ideal_iter_ <= 2)
                {
                    return optimal_step_coef_[iter] * static_cast<Scalar>(BS.work(iter + 1)) / static_cast<Scalar>(BS.work(iter));
                } else {
                    ideal_iter_ = allowedRange(ideal_iter_ - 1);
                    return optimal_step_coef_[ideal_iter_];//reduce order
                }

            case 0:

                if (work_per_len_[iter - 1] < 0.8 * work_per_len_[iter]) {
                    ideal_iter_ = allowedRange(ideal_iter_ - 1);
                    return optimal_step_coef_[ideal_iter_];
                } else if (work_per_len_[iter] < 0.9 * work_per_len_[iter - 1]) {
                    ideal_iter_ = allowedRange(ideal_iter_ + 1);
                    return optimal_step_coef_[iter] * static_cast<Scalar>(BS.work(iter + 1)) / static_cast<Scalar>(BS.work(iter));
                } else {
                    return optimal_step_coef_[ideal_iter_];//keep order
                }

            case 1:

                if (work_per_len_[iter - 2] < 0.8 * work_per_len_[iter - 1]) {
                    ideal_iter_ = allowedRange(ideal_iter_ - 1);
                }
                if (work_per_len_[iter] < 0.9 * work_per_len_[ideal_iter_]) {
                    ideal_iter_ = allowedRange(ideal_iter_ + 1);
                }
                return optimal_step_coef_[ideal_iter_];

            default:
                ERR_MSG("unexpected iteration index!");
        }
    }

/** @brief Check/resize the extrapolation table.*/
    template<typename ParticSys, typename Integrator>
    void BSIterator<ParticSys, Integrator>::checkExtrapVolume() {
        size_t tab_size = extrapTab.size();
        size_t array_size = extrapTab[0].size();

        if (extrapTabVolume_ != tab_size * array_size) {
            for (size_t i = 1; i < tab_size; i++)
                extrapTab[i].resize(array_size);

            extrapTabVolume_ = tab_size * array_size;
        }
    }
}

#endif
