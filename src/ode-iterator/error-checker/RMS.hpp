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
 * @file RMS.hpp
 *
 * Header file.
 */
#pragma once

namespace space::ode_iterator {
    /*---------------------------------------------------------------------------*\
         Class RMS Declaration
    \*---------------------------------------------------------------------------*/
    /**
     *
     * @tparam T
     */
    template <typename TypeSystem>
    class RMS {
       public:
        // Type member

        SPACEHUB_USING_TYPE_SYSTEM_OF(TypeSystem);

        // Constructors
        SPACEHUB_MAKE_CONSTRUCTORS(RMS, default, default, default, default, default);

        RMS(Scalar atol, Scalar rtol);

        SPACEHUB_READ_ACCESSOR(auto, atol, atol_);

        SPACEHUB_READ_ACCESSOR(auto, rtol, rtol_);

        void set_atol(Scalar);

        void set_rtol(Scalar);

        template <typename Array>
        auto error(Array const &y0, Array const &y1, Array const &y1_prime) -> Scalar;

       private:
        Scalar atol_{1e-13};

        Scalar rtol_{1e-13};
    };
    /*---------------------------------------------------------------------------*\
         Class RMS Implementation
    \*---------------------------------------------------------------------------*/
    template <typename TypeSystem>
    void RMS<TypeSystem>::set_atol(Scalar error) {
        atol_ = error;
    }

    template <typename TypeSystem>
    void RMS<TypeSystem>::set_rtol(Scalar error) {
        rtol_ = error;
    }

    template <typename TypeSystem>
    template <typename Array>
    auto RMS<TypeSystem>::error(const Array &y0, const Array &y1, const Array &y1_prime) -> Scalar {
        size_t const size = y0.size();
        Scalar error = 0;

        if constexpr (std::is_same_v<typename Array::value_type, Scalar>) {
            for (size_t i = 0; i < size; ++i) {
                Scalar scale = std::max(fabs(y0[i]), fabs(y1[i]));
                auto r = fabs(y1[i] - y1_prime[i]) / (atol_ + scale * rtol_);
                error += r * r;
            }
        } else if constexpr (std::is_same_v<typename Array::value_type, Vec3<Scalar>>) {
            for (size_t i = 0; i < size; ++i) {
                auto scale = vec_max(vec_abs(y0[i]), vec_abs(y1[i]));
                auto v = vec_abs(y1[i] - y1_prime[i]) / (atol_ + scale * rtol_);
                auto r = norm2(v) / 3;
                error += r;
                // Scalar scale = std::max(max_abs(y0[i]), max_abs(y1[i]));
                // auto r = max_abs(y1[i] - y1_prime[i]) / (atol_ + scale * rtol_);
                // error += r * r;
            }
        } else {
            spacehub_abort("Unsupported array type!");
        }
        return sqrt(error / size);
    }

    template <typename TypeSystem>
    RMS<TypeSystem>::RMS(Scalar atol, Scalar rtol) : atol_{atol}, rtol_{rtol} {}
}  // namespace space::ode_iterator
