/*---------------------------------------------------------------------------*\
        .-''''-.         |
       /        \        |
      /_        _\       |  SpaceHub: The Open Source N-body Toolkit
     // \  <>  / \\      |
     |\__\    /__/|      |  Website:  https://yihanwangastro.github.io/SpaceHub/
      \    ||    /       |
        \  __  /         |  Copyright (C) 2019 Yihan Wang
         '.__.'          |
---------------------------------------------------------------------
License
    This file is part of SpaceHub.
    SpaceHub is free software: you can redistribute it and/or modify it under
    the terms of the MIT License. SpaceHub is distributed in the hope that it
    will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the MIT License
    for more details. You should have received a copy of the MIT License along
    with SpaceHub.
\*---------------------------------------------------------------------------*/
/**
 * @file Burlish-Stoer.hpp
 *
 * Header file.
 */
#pragma once

#include <array>
#include <functional>
#include <vector>

#include "../core-computation.hpp"
#include "../integrator/symplectic/symplectic-integrator.hpp"
#include "../spacehub-concepts.hpp"
namespace space::ode_iterator {

    /*---------------------------------------------------------------------------*\
         Class BurlishStoerConsts Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * @tparam T
     * @tparam MaxIter
     */
    template <typename T, size_t MaxIter, bool IsKDK>
    class BurlishStoerConsts {
       public:
        using Scalar = T;

        constexpr static size_t max_iter{MaxIter};

        constexpr static double dec_factor{0.8};

        constexpr static double inc_factor{0.9};

        constexpr inline Scalar cost(size_t i) const { return cost_[i]; };

        [[nodiscard]] constexpr inline size_t step_sequence(size_t i) const { return sub_steps_[i]; };

        constexpr inline Scalar table_coef(size_t i, size_t j) const { return extrap_coef_[at(i, j)]; };

        constexpr explicit BurlishStoerConsts();

       private:
        [[nodiscard]] inline size_t at(size_t i, size_t j) const { return i * MaxIter + j; };

        /** @brief Extrapolation coefficient.*/
        std::array<Scalar, MaxIter *(MaxIter)> extrap_coef_;

        /** @brief The work(computation resource) per step size of each iteration depth.*/
        std::array<size_t, MaxIter> cost_;

        /** @brief Steps of integration of each iteration depth.*/
        std::array<size_t, MaxIter> sub_steps_;
    };

    /*---------------------------------------------------------------------------*\
         Class BurlishStoer Declaration
    \*---------------------------------------------------------------------------*/
    /**
     * @tparam Real
     * @tparam ErrEstimator
     * @tparam StepController
     */
    template <typename Integrator, typename ErrEstimator, typename StepController>
    class BurlishStoer {
       public:
        static constexpr size_t max_depth{7};

        SPACEHUB_USING_TYPE_SYSTEM_OF(Integrator);

        using BSConsts =
            BurlishStoerConsts<Scalar, max_depth + 1, std::is_same_v<Integrator, integrator::LeapFrogKDK<TypeSet>>>;

        using State = ScalarArray;

        static constexpr size_t max_try_num{100};

        template <CONCEPT_PARTICLE_SYSTEM U>
        Scalar iterate(U &particles, typename U::Scalar macro_step_size);

        Scalar iterate(std::function<void(State const &, State &, Scalar)> func, State &data, Scalar &time,
                       Scalar step_size);

        void set_atol(Scalar atol);

        void set_rtol(Scalar rtol);

        Scalar reject_rate() { return static_cast<Scalar>(rej_num_) / static_cast<Scalar>(iter_num_); };

       private:
        void check_variable_size();

        [[nodiscard]] inline size_t at(size_t i, size_t j) const { return parameters_.at(i, j); };

        template <CONCEPT_PARTICLE_SYSTEM U>
        void integrate_by_n_steps(U &particles, Scalar macro_step_size, size_t steps);

        void integrate_by_n_steps(std::function<void(State const &, State &, Scalar)> func, State &data_out,
                                  Scalar &time, Scalar step_size, size_t steps);

        void extrapolate(size_t k);

        inline bool in_converged_window(size_t k);

        inline size_t allowed(size_t i) const;

        Scalar set_next_iteration(size_t k);

        bool is_diverged_anyhow(Scalar error, size_t k) const;

        Scalar get_next_step_len(size_t k_new, size_t k) const;

       private:
        /** @brief The constat coef for BS extrapolation*/
        BSConsts parameters_;

        /** @brief Extrapolation table.*/
        std::array<State, max_depth + 1> extrap_list_;

        ErrEstimator err_checker_;

        StepController step_controller_;

        State input_;

        /** @brief The optimal step size array.*/
        std::array<Scalar, max_depth + 1> ideal_step_size_{0};

        /** @brief The work(computation resource) needed to converge at column k.*/
        std::array<Scalar, max_depth + 1> cost_per_len_{0};

        Scalar last_error_{1.0};

        /** @brief Current iteration depth.*/
        size_t ideal_rank_{4};

        /** @brief Total volume of extrapolation table(in scalar).*/
        size_t var_num_{0};

        /** @brief Rejection number*/
        size_t rej_num_{0};

        /** @brief Total iteration number*/
        size_t iter_num_{0};

        bool step_reject_{false};
    };

    /*---------------------------------------------------------------------------*\
         Class BurlishStoerConsts Implementation
    \*---------------------------------------------------------------------------*/
    template <typename Scalar, size_t MaxIter, bool IsKDK>
    constexpr BurlishStoerConsts<Scalar, MaxIter, IsKDK>::BurlishStoerConsts() {
        for (size_t i = 0; i < MaxIter; ++i) {
            sub_steps_[i] = 2 * (i + 1);

            if (i == 0) {
                cost_[i] = sub_steps_[i] + static_cast<size_t>(IsKDK) * 2;
            } else {
                cost_[i] = cost_[i - 1] + sub_steps_[i] + static_cast<size_t>(IsKDK) * 2;
            }

            for (size_t j = 0; j < MaxIter; ++j) {
                if (j < i) {
                    // Scalar ratio = static_cast<Scalar>(sub_steps_[i]) / static_cast<Scalar>(sub_steps_[i - j - 1]);
                    // extrap_coef_[at(i, j)] = 1.0 / (ratio * ratio - 1.0);
                    double nj2 = sub_steps_[i - j - 1] * sub_steps_[i - j - 1];
                    double ni2 = sub_steps_[i] * sub_steps_[i];
                    extrap_coef_[at(i, j)] = nj2 / (ni2 - nj2);
                } else {
                    extrap_coef_[at(i, j)] = 0;
                }
            }
        }
    }
    /*---------------------------------------------------------------------------*\
         Class BurlishStoer Implementation
    \*---------------------------------------------------------------------------*/
    template <typename Integrator, typename ErrEstimator, typename StepController>
    auto BurlishStoer<Integrator, ErrEstimator, StepController>::iterate(
        std::function<void(State const &, State &, Scalar)> func, State &data, Scalar &time, Scalar step_size)
        -> Scalar {
        Scalar iter_h = step_size;
        input_ = data;
        check_variable_size();
        for (size_t i = 0; i < max_try_num; ++i) {
            iter_num_++;
            integrate_by_n_steps(func, extrap_list_[0], time, iter_h, parameters_.step_sequence(0));

            for (size_t k = 1; k <= ideal_rank_ + 1; ++k) {
                integrate_by_n_steps(func, extrap_list_[k], time, iter_h, parameters_.step_sequence(k));

                extrapolate(k);

                Scalar error = err_checker_.error(input_, extrap_list_[1], extrap_list_[0]);

                ideal_step_size_[k] = step_controller_.next_step_size(2 * k + 1, iter_h, error);

                cost_per_len_[k] = parameters_.cost(k) / ideal_step_size_[k];
                // space::print_csv(std::cout, k, ideal_rank_, error, ideal_step_size_[k], cost_per_len_[k], '\n');
                if (in_converged_window(k)) {
                    if (error <= 1.0) {
                        step_reject_ = false;
                        time += iter_h;
                        data = extrap_list_[0];
                        iter_h = set_next_iteration(k);
                        last_error_ = error;
                        return iter_h;
                    } else if (is_diverged_anyhow(error, k)) {
                        step_reject_ = true;
                        rej_num_++;
                        iter_h = set_next_iteration(k);  // get_next_try_step(k);
                        break;
                    }
                }
            }
            // particles.read_from_scalar_array(input_);
        }
        spacehub_abort("Reach max iteration loop number!");
    }
    template <typename Integrator, typename ErrEstimator, typename StepController>
    template <CONCEPT_PARTICLE_SYSTEM U>
    auto BurlishStoer<Integrator, ErrEstimator, StepController>::iterate(U &particles,
                                                                         typename U::Scalar macro_step_size) -> Scalar {
        Scalar iter_h = macro_step_size;
        particles.write_to_scalar_array(input_);
        check_variable_size();

        for (size_t i = 0; i < max_try_num; ++i) {
            iter_num_++;
            integrate_by_n_steps(particles, iter_h, parameters_.step_sequence(0));
            particles.write_to_scalar_array(extrap_list_[0]);

            for (size_t k = 1; k <= ideal_rank_ + 1; ++k) {
                particles.read_from_scalar_array(input_);
                integrate_by_n_steps(particles, iter_h, parameters_.step_sequence(k));
                particles.write_to_scalar_array(extrap_list_[k]);
                extrapolate(k);

                Scalar error = err_checker_.error(input_, extrap_list_[1], extrap_list_[0]);

                // ideal_step_size_[k] =
                // step_controller_.next_step_size(2 * k + 1, iter_h, std::make_tuple(error, last_error_));

                ideal_step_size_[k] = step_controller_.next_step_size(2 * k + 1, iter_h, error);

                cost_per_len_[k] = parameters_.cost(k) / ideal_step_size_[k];
                // space::print_csv(std::cout, k, ideal_rank_, error, ideal_step_size_[k], cost_per_len_[k], '\n');
                if (in_converged_window(k)) {
                    if (error <= 1.0) {
                        step_reject_ = false;
                        iter_h = set_next_iteration(k);
                        particles.read_from_scalar_array(extrap_list_[0]);
                        last_error_ = error;
                        return iter_h;
                    } else if (is_diverged_anyhow(error, k)) {
                        step_reject_ = true;
                        rej_num_++;
                        iter_h = set_next_iteration(k);  // get_next_try_step(k);
                        break;
                    }
                }
            }
            particles.read_from_scalar_array(input_);
        }
        spacehub_abort("Reach max iteration loop number!");
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::set_atol(Scalar atol) {
        err_checker_.set_atol(atol);
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::set_rtol(Scalar rtol) {
        err_checker_.set_rtol(rtol);
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::check_variable_size() {
        var_num_ = input_.size();
        if (var_num_ > extrap_list_[0].size()) [[unlikely]] {
            for (auto &v : extrap_list_) {
                v.resize(var_num_);
            }
        }
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    template <CONCEPT_PARTICLE_SYSTEM U>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::integrate_by_n_steps(U &particles,
                                                                                      Scalar macro_step_size,
                                                                                      size_t steps) {
        size_t num_drift = steps / 2;
        Scalar h = macro_step_size / num_drift;

        if constexpr (std::is_same_v<Integrator, integrator::LeapFrogDKD<TypeSet>>) {
            particles.drift(0.5 * h);
            for (size_t i = 1; i < num_drift; i++) {
                particles.kick(h);
                particles.drift(h);
            }
            particles.kick(h);
            particles.drift(0.5 * h);
        } else if constexpr (std::is_same_v<Integrator, integrator::LeapFrogKDK<TypeSet>>) {
            particles.kick(0.5 * h);
            for (size_t i = 1; i < num_drift; i++) {
                particles.drift(h);
                particles.kick(h);
            }
            particles.drift(h);
            particles.kick(0.5 * h);
        } else {
            static_assert(true, "Burlish-Stoer Undefined embeded integration method");
        }
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::integrate_by_n_steps(
        std::function<void(State const &, State &, Scalar)> func, State &data_out, Scalar &time, Scalar step_size,
        size_t steps) {
        data_out = input_;
        Scalar h = step_size / steps;
        Scalar t = time;
        State dxdt(input_.size());
        State data_mid(input_);

        func(input_, dxdt, t);
        calc::array_advance(data_out, input_, dxdt, h);
        t += h;
        for (size_t i = 1; i < steps; i++) {
            func(data_out, dxdt, t);
            calc::array_advance(data_mid, dxdt, 2 * h);
            t += h;
            std::swap(data_mid, data_out);
        }
        func(data_out, dxdt, t);
        calc::array_advance(data_mid, dxdt, h);
        calc::array_add(data_out, data_mid, data_out);
        calc::array_scale(data_out, data_out, 0.5);
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    void BurlishStoer<Integrator, ErrEstimator, StepController>::extrapolate(size_t k) {
        for (size_t j = k; j > 0; --j) {
            for (size_t i = 0; i < var_num_; ++i) {
                extrap_list_[j - 1][i] = extrap_list_[j][i] + (extrap_list_[j][i] - extrap_list_[j - 1][i]) *
                                                                  parameters_.table_coef(k, k - j);
            }
        }
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    bool BurlishStoer<Integrator, ErrEstimator, StepController>::in_converged_window(size_t k) {
        return k == ideal_rank_ - 1 || k == ideal_rank_ || k == ideal_rank_ + 1;
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    size_t BurlishStoer<Integrator, ErrEstimator, StepController>::allowed(size_t i) const {
        return math::in_range(static_cast<size_t>(2), i, static_cast<size_t>(max_depth - 1));
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    auto BurlishStoer<Integrator, ErrEstimator, StepController>::get_next_step_len(size_t k_new, size_t k) const
        -> Scalar {
        if (k_new <= k) {
            return ideal_step_size_[k_new];
        } else if (k_new == k + 1) {
            return ideal_step_size_[k] * static_cast<Scalar>(parameters_.cost(k + 1)) /
                   static_cast<Scalar>(parameters_.cost(k));
        } else {
            spacehub_abort("unexcepted order!");
        }
    }

    template <typename Integrator, typename ErrEstimator, typename StepController>
    auto BurlishStoer<Integrator, ErrEstimator, StepController>::set_next_iteration(size_t k) -> Scalar {
        if (k == ideal_rank_ - 1 || k == ideal_rank_) [[likely]] {
            if (cost_per_len_[k - 1] < BSConsts::dec_factor * cost_per_len_[k]) [[unlikely]] {
                ideal_rank_ = allowed(k - 1);
                return get_next_step_len(ideal_rank_, k);
            } else if (cost_per_len_[k] < BSConsts::inc_factor * cost_per_len_[k - 1] && !step_reject_) [[unlikely]] {
                ideal_rank_ = allowed(k + 1);
                return get_next_step_len(ideal_rank_, k);
            } else {
                ideal_rank_ = allowed(k);
                return get_next_step_len(ideal_rank_, k);
            }
        } else if (k == ideal_rank_ + 1) {
            if (cost_per_len_[k - 2] < BSConsts::dec_factor * cost_per_len_[k - 1]) {
                ideal_rank_ = allowed(k - 2);
            }
            if (cost_per_len_[k] < BSConsts::inc_factor * cost_per_len_[ideal_rank_] && !step_reject_) {
                ideal_rank_ = allowed(k);
            }
            return get_next_step_len(ideal_rank_, k);
        } else {
            spacehub_abort("unexpected iteration index!");
        }
    }  // namespace space::ode_iterator

    template <typename Integrator, typename ErrEstimator, typename StepController>
    bool BurlishStoer<Integrator, ErrEstimator, StepController>::is_diverged_anyhow(Scalar error, size_t k) const {
        Scalar r = 1.0;
        if (k == ideal_rank_ - 1) {
            r = static_cast<Scalar>(parameters_.step_sequence(k + 1) * parameters_.step_sequence(k + 2)) /
                static_cast<Scalar>(parameters_.step_sequence(0) * parameters_.step_sequence(0));
        } else if (k == ideal_rank_) {
            r = static_cast<Scalar>(parameters_.step_sequence(k + 1)) /
                static_cast<Scalar>(parameters_.step_sequence(0));
        }  // else k == iterDepth+1 and error >1 reject directly
        return error > r * r;
    }
}  // namespace space::ode_iterator
