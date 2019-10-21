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
 * @file core-computation.hpp
 *
 * Header file.
 */
#ifndef SPACEHUB_CORE_COMPUTATION_HPP
#define SPACEHUB_CORE_COMPUTATION_HPP

#include "macros.hpp"
#include "math.hpp"

/**
 * @namespace space::calc
 * Documentation for space
 */
namespace space::calc {
  template<typename... Args>
  constexpr auto add(Args &&... args) {
    return (... + args);
  }

  template<typename... Args>
  constexpr auto mul(Args &&... args) {
    return (... * args);
  }

  template<typename... Args>
  constexpr auto any(Args... args) {
    return (... || args);
  }

  template<typename... Args>
  constexpr auto all(Args... args) {
    return (... && args);
  }

  template<typename Array>
  void array_set_zero(Array &array) {
    for (auto &a : array) {
      a = 0;
    }
  }

  template<typename... Args>
  void set_arrays_zero(Args &... args) {
    (..., (array_set_zero(args)));
  }

  template<typename Array, typename... Args>
  auto array_dot(Array const &a, Array const &b, Args const &... args) {
    DEBUG_MODE_ASSERT(b.size() == a.size(), "length of the array mismatch!");
    typename Array::value_type product{0};
    size_t const size = a.size();
    for (size_t i = 0; i < size; ++i) {
      product += (args[i] * ... * (a[i] * b[i]));
    }
    return product;
  }

  template<typename Array, typename... Args>
  void array_add(Array &dst, Array const &a, Args const &... args) {
    size_t const size = dst.size();

    for (size_t i = 0; i < size; i++) {
      dst[i] = a[i]  + (args[i] + ...);
    }
  }

  template<typename Array, typename Scalar>
  void array_scale(Array &dst, Array const &a, Scalar scale) {
    DEBUG_MODE_ASSERT(b.size() == a.size() || dst.size() >= a.size(), "length of the array mismatch!");
    size_t const size = dst.size();

    for (size_t i = 0; i < size; i++) {
      dst[i] = a[i] * scale;
    }
  }

  template<typename Array, typename... Args>
  void array_mul(Array &dst, Array const &a, Array const &b, Args const &... args) {
    size_t const size = dst.size();

    for (size_t i = 0; i < size; i++) {
      dst[i] = a[i] * b[i] * (args[i] * ...);
    }
  }

  template<typename Array>
  void array_sub(Array &dst, Array const &a, Array const &b) {
    DEBUG_MODE_ASSERT(b.size() == a.size() || dst.size() >= a.size(), "length of the array mismatch!");
    size_t const size = dst.size();

    for (size_t i = 0; i < size; i++) {
      dst[i] = a[i] - b[i];
    }
  }

  template<typename Array>
  auto array_sum(Array const &array) {
    typename Array::value_type total = 0;
    for (auto const &a : array) {
      total += a;
    }
    return total;
  }

  template<typename Scalar, typename Array>
  void array_advance(Array &var, Array const &increment, Scalar step_size) {
    size_t const size = var.size();

    for (size_t i = 0; i < size; i++) {
      var[i] += increment[i] * step_size;
    }
  }

  template<typename Array, typename Coord>
  void coord_dot(Array &dst, Coord const &a, Coord const &b) {
    size_t const size = dst.size();
    for (size_t i = 0; i < size; ++i) {
      dst[i] = a.x[i] * b.x[i] + a.y[i] * b.y[i] + a.z[i] * b.z[i];
    }
  }

  template<typename Array, typename Coord>
  auto coord_contract_to_scalar(Array &coef, Coord const &a, Coord const &b) {
    size_t const size = coef.size();
    typename Coord::Scalar sum{0};

    for (size_t i = 0; i < size; ++i) {
      sum += (a.x[i] * b.x[i] + a.y[i] * b.y[i] + a.z[i] * b.z[i]) * coef[i];
    }
    return sum;
  }

/*template<typename Scalar, typename Coord>
auto coord_contract_to_scalar(Scalar table_coef, Coord const &a, Coord const &b) {
    size_t size = a.size();
    Scalar sum{0};

    for (size_t i = 0; i < size; ++i) {
        sum += (a.x[i]*b.x[i] + a.y[i]*b.y[i] + a.z[i]*b.z[i])*table_coef;
    }
    return sum;
}*/

  template<typename Coord>
  auto coord_contract_to_scalar(Coord const &a, Coord const &b) {
    size_t const size = a.size();
    typename Coord::Scalar sum{0};

    for (size_t i = 0; i < size; ++i) {
      sum += (a.x[i] * b.x[i] + a.y[i] * b.y[i] + a.z[i] * b.z[i]);
    }
    return sum;
  }

  template<typename Coord, typename... Args>
  inline void coord_add(Coord &dst, Coord const &a, Coord const &b, Args const &... args) {
    array_add(dst.x, a.x, b.x, args.x...);
    array_add(dst.y, a.y, b.y, args.y...);
    array_add(dst.z, a.z, b.z, args.z...);
  }

  template<typename Coord>
  inline void coord_sub(Coord &dst, Coord const &a, Coord const &b) {
    array_sub(dst.x, a.x, b.x);
    array_sub(dst.y, a.y, b.y);
    array_sub(dst.z, a.z, b.z);
  }

  template<typename Coord, typename Scalar>
  inline void coord_scale(Coord &dst, Coord const &a, Scalar scale) {
    array_scale(dst.x, a.x, scale);
    array_scale(dst.y, a.y, scale);
    array_scale(dst.z, a.z, scale);
  }

  template<typename Scalar, typename Coord>
  inline void coord_advance(Coord &var, Coord const &increment, Scalar step_size) {
    array_advance(var.x, increment.x, step_size);
    array_advance(var.y, increment.y, step_size);
    array_advance(var.z, increment.z, step_size);
  }

  template<typename Array>
  inline auto calc_com(Array const &mass, Array const &var) {
    return array_dot(var, mass) / array_sum(mass);
  }

  template<typename Array>
  inline auto calc_com(Array const &mass, Array const &var, typename Array::value_type tot_mass) {
    return array_dot(var, mass) / tot_mass;
  }

  template<typename Array>
  void move_to_com(Array &var, typename Array::value_type const &com_var) {
    for (auto &v : var) v -= com_var;
  }

  template<typename Array1, typename Array2>
  inline void move_to_com(Array1 const &mass, Array2 &var) {
    auto com_var = calc_com(mass, var);
    move_to_com(var, com_var);
  }

  template<typename Coord, typename ScalarArray>
  inline void coord_move_to_com(ScalarArray const &mass, Coord &var) {
    auto tot_mass = array_sum(mass);
    move_to_com(var.x, calc_com(mass, var.x, tot_mass));
    move_to_com(var.y, calc_com(mass, var.y, tot_mass));
    move_to_com(var.z, calc_com(mass, var.z, tot_mass));
  }

  template<typename Particles>
  inline auto calc_kinetic_energy(Particles const &ptc) {
    return 0.5 * coord_contract_to_scalar(ptc.mass(), ptc.vel(), ptc.vel());
  }

  CREATE_METHOD_CHECK(chain_pos);

  CREATE_METHOD_CHECK(index);

  CREATE_METHOD_CHECK(omega);

  CREATE_METHOD_CHECK(bindE);

  CREATE_METHOD_CHECK(mass);

  CREATE_METHOD_CHECK(pos);

  CREATE_METHOD_CHECK(vel);

  template<typename Particle>
  auto calc_potential_energy(Particle const &particle1, Particle const &particle2) {
    if constexpr (HAS_METHOD(Particle, mass) && HAS_METHOD(Particle, pos)) {
      auto potential_eng = -consts::G * particle1.mass() * particle2.mass();

      auto dx = particle1.pos().x - particle2.pos().x;
      auto dy = particle1.pos().y - particle2.pos().y;
      auto dz = particle1.pos().z - particle2.pos().z;

      return potential_eng / sqrt(dx * dx + dy * dy + dz * dz);
    } else {
      auto potential_eng = -consts::G * particle1.mass * particle2.mass;

      auto dx = particle1.pos.x - particle2.pos.x;
      auto dy = particle1.pos.y - particle2.pos.y;
      auto dz = particle1.pos.z - particle2.pos.z;

      return potential_eng / sqrt(dx * dx + dy * dy + dz * dz);
    }
  }

  template<typename Particles>
  auto calc_potential_energy(Particles const &particles) {

    typename Particles::Scalar potential_eng{0};
    size_t const size = particles.number();
    auto const &m = particles.mass();
    auto const &v = particles.vel();
    auto const &p = particles.pos();

    if constexpr (HAS_METHOD(Particles, chain_pos) && HAS_METHOD(Particles, index)) {
      auto const &ch_px = particles.chain_pos().x;
      auto const &ch_py = particles.chain_pos().y;
      auto const &ch_pz = particles.chain_pos().z;
      auto const &idx = particles.index();

      for (size_t i = 0; i < size - 1; ++i) {
        potential_eng -=
                m[idx[i]] * m[idx[i + 1]] / sqrt(ch_px[i] * ch_px[i] + ch_py[i] * ch_py[i] + ch_pz[i] * ch_pz[i]);
      }

      for (size_t i = 0; i < size - 2; ++i) {
        auto dx = ch_px[i] + ch_px[i + 1];
        auto dy = ch_py[i] + ch_py[i + 1];
        auto dz = ch_pz[i] + ch_pz[i + 1];
        potential_eng -= m[idx[i]] * m[idx[i + 2]] / sqrt(dx * dx + dy * dy + dz * dz);
      }

      for (size_t i = 0; i < size; ++i) {
        for (size_t j = i + 3; j < size; ++j) {
          auto dx = p.x[idx[j]] - p.x[idx[i]];
          auto dy = p.y[idx[j]] - p.y[idx[i]];
          auto dz = p.z[idx[j]] - p.z[idx[i]];
          potential_eng -= m[idx[i]] * m[idx[j]] / sqrt(dx * dx + dy * dy + dz * dz);
        }
      }
    } else {
      for (size_t i = 0; i < size; ++i)
        for (size_t j = i + 1; j < size; ++j) {
          auto dx = p.x[i] - p.x[j];
          auto dy = p.y[i] - p.y[j];
          auto dz = p.z[i] - p.z[j];
          potential_eng -= m[i] * m[j] / sqrt(dx * dx + dy * dy + dz * dz);
        }
    }
    return potential_eng * consts::G;
  }

  template<typename Particles>
  inline auto calc_total_energy(Particles const &particles) {
    return calc_potential_energy(particles) + calc_kinetic_energy(particles);
  }

/** @brief Calculate the minimal fall free time of two particles
 *
 *  @param  mass mass array of particle.
 *  @param  position  position array of particle.
 *  @return The minimal fall free time of the two particles
 */
  template<typename ScalarArray, typename Coord>
  inline auto calc_fall_free_time(ScalarArray const &mass, Coord const &position) {
    using Scalar = typename ScalarArray::value_type;
    size_t const size = mass.size();
    Scalar min_fall_free = math::max_value<Scalar>::value;

    for (size_t i = 0; i < size; i++) {
      for (size_t j = i + 1; j < size; j++) {
        Scalar dx = position.x[i] - position.x[j];
        Scalar dy = position.y[i] - position.y[j];
        Scalar dz = position.z[i] - position.z[j];
        Scalar r = sqrt(dx * dx + dy * dy + dz * dz);
        Scalar fall_free = pow(r, 1.5) / sqrt(mass[i] + mass[j]);

        if (fall_free < min_fall_free) min_fall_free = fall_free;
      }
    }
    return min_fall_free * space::consts::pi * 0.5 / sqrt(2 * space::consts::G);
  }


/**
 *
 * @tparam Particles
 * @param particles
 * @return
 */
  template<typename Particles>
  auto calc_step_scale(Particles const &particles) {
    if constexpr (HAS_METHOD(Particles, omega)) {
      return calc_fall_free_time(particles.mass(), particles.pos()) * particles.omega();
    } else {
      return calc_fall_free_time(particles.mass(), particles.pos());
    }
  }

  template<typename Particles>
  auto calc_energy_error(Particles const &particles, typename Particles::Scalar E0) {
    auto U = -calc_potential_energy(particles);
    auto T = calc_kinetic_energy(particles);

    if constexpr (HAS_METHOD(Particles, bindE)) {
      return log(fabs((T + particles.bindE()) / U));
    } else {
      return fabs((T - U - E0) / E0);
    }
  }
}  // namespace space::calc
#endif
