//
// Created by yihan on 2/24/19.
//

#ifndef SPACEHUB_PARTICLE_SYSTEM_HPP
#define SPACEHUB_PARTICLE_SYSTEM_HPP

#include "dev-tools.hpp"

namespace space {
/*---------------------------------------------------------------------------*\
    Class ParticleSystem Declaration
\*---------------------------------------------------------------------------*/
/**
 * Abstract class of Particle System. A class implements(partly/fully) the interfaces of this
 * class via CRTP idiom can be used cross the system as an implementation of the concept `ParticleSystem`. The particle
 * System integrates the data set of a particle set and provides interface to evolve the internal particles.
 *
 * @tparam Derived The implement class in CRTP idiom.
 */
  template<typename Derived>
  class ParticleSystem {
  public:
    // public methods

    DECLARE_CRTP_ACCESSOR(Derived, auto, mass);

    DECLARE_CRTP_ACCESSOR(Derived, auto, idn);

    DECLARE_CRTP_ACCESSOR(Derived, auto, time);

    DECLARE_CRTP_ACCESSOR(Derived, auto, pos);

    DECLARE_CRTP_ACCESSOR(Derived, auto, vel);

    /**
     * @must_impl
     *
     * Get the (active) particle number of the SoA particle set.
     * @return size_t the number of the particles.
     */
    [[nodiscard]] size_t number() const;

    /**
     * @must_impl
     *
     * Advance the internal time of the particle system with given time interval.
     * @tparam Scalar Floating point like type type.
     * @param[in] dt Avanced time interval.
     */
    template<typename Scalar>
    void advance_time(Scalar dt);

    /**
     * @must_impl
     *
     * Advance the interval position of all particles by providing all corresponding velocity with given step size.
     *
     * @tparam Coord The Coordinates Type.
     * @tparam Scalar Floating point like type type.
     * @param[in] step_size Advance step_size.
     * @param[in] velocity Corresponding velocity of all particles.
     */
    template<typename Coord, typename Scalar>
    void advance_pos(Scalar step_size, Coord const &velocity);

    /**
     * @must_impl
     *
     * Advance the interval velocity of all particles by providing all corresponding acceleration with given step size.
     *
     * @tparam Coord The Coordinates Type.
     * @tparam Scalar Floating point like type type.
     * @param[in] step_size Advance step_size.
     * @param[in] acceleration Corresponding acceleration of all particles.
     */
    template<typename Coord, typename Scalar>
    void advance_vel(Scalar step_size, Coord const &acceleration);

    /**
     * @must_impl
     *
     * Evaluate the acceleration for current state of the particle system.
     *
     * @tparam Coord The Coordinates Type.
     * @param[out] acceleration The calculated acceleration.
     */
    template<typename Coord>
    void evaluate_acc(Coord &acceleration) const;

    /**
     * @opt_impl{Symplectic integration is required.}
     *
     * Drift the particle system with given step size.
     *
     * @tparam Scalar Floating point like type type.
     * @param[in] step_size Drift step size.
     */
    template<typename Scalar>
    void drift(Scalar step_size);

    /**
     * @opt_impl{Symplectic integration is required.}
     *
     * Kick the particle system with given step size.
     *
     * @tparam Scalar Floating point like type type.
     * @param[in] step_size Drift step size.
     */
    template<typename Scalar>
    void kick(Scalar step_size);

    /**
     * @opt_impl{For specific required.}
     *
     * Pre-process of the particle system before each step iteration.
     */
    void pre_iter_process();

    /**
     * @opt_impl{For specific required.}
     *
     * Post-process of the particle system after each step iteration.
     */
    void post_iter_process();

    /**
     * @must_impl
     *
     * Map all evolved data domain to a 1-d iterable Container(i.e `std::vector`, `std::array`, etc,.). Used for general
     * integration modules.
     *
     * @tparam STL 1-d iterable Container.
     * @param[out] stl_ranges Target container to store the evolved data domain.
     * @note The method should keep consistence with load_from_linear_container().
     */
    template<typename STL>
    void to_linear_container(STL &stl_ranges);

    /**
     * @must_impl
     *
     * Reconstruct the particle system from a 1-d iterable Container(i.e `std::vector`, `std::array`, etc,.). Used for general
     * integration modules.
     *
     * @tparam STL 1-d iterable Container.
     * @param[in] stl_ranges Container that store the particle information for reconstruction.
     * @note The method should keep consistence with to_linear_container().
     */
    template<typename STL>
    void load_from_linear_container(STL const &stl_ranges);

  private:
    // constructors
    ParticleSystem() = default;

    friend Derived;

    // private methods
    void impl_pre_iter_process() {}  // default implementation

    void impl_post_iter_process() {}  // default implementation
  };

/*---------------------------------------------------------------------------*\
    Class ParticleSystem Implementation
\*---------------------------------------------------------------------------*/
  template<typename Derived>
  size_t ParticleSystem<Derived>::number() const {
    return static_cast<Derived const *>(this)->impl_number();
  }

  template<typename Derived>
  template<typename Scalar>
  void ParticleSystem<Derived>::advance_time(Scalar dt) {
    static_cast<Derived *>(this)->impl_advance_time(dt);
  }

  template<typename Derived>
  template<typename Coord, typename Scalar>
  void ParticleSystem<Derived>::advance_pos(Scalar step_size, const Coord &velocity) {
    static_cast<Derived *>(this)->impl_advance_pos(step_size, velocity);
  }

  template<typename Derived>
  template<typename Coord, typename Scalar>
  void ParticleSystem<Derived>::advance_vel(Scalar step_size, const Coord &acceleration) {
    static_cast<Derived *>(this)->impl_advance_vel(step_size, acceleration);
  }

  template<typename Derived>
  template<typename Coord>
  void ParticleSystem<Derived>::evaluate_acc(Coord &acceleration) const {
    static_cast<Derived const *>(this)->impl_evaluate_acc(acceleration);
  }

  template<typename Derived>
  template<typename Scalar>
  void ParticleSystem<Derived>::drift(Scalar step_size) {
    static_cast<Derived *>(this)->impl_drift(step_size);
  }

  template<typename Derived>
  template<typename Scalar>
  void ParticleSystem<Derived>::kick(Scalar step_size) {
    static_cast<Derived *>(this)->impl_kick(step_size);
  }

  template<typename Derived>
  void ParticleSystem<Derived>::pre_iter_process() {
    static_cast<Derived *>(this)->impl_pre_iter_process();
  }

  template<typename Derived>
  void ParticleSystem<Derived>::post_iter_process() {
    static_cast<Derived *>(this)->impl_post_iter_process();
  }

  template<typename Derived>
  template<typename STL>
  void ParticleSystem<Derived>::to_linear_container(STL &stl_ranges) {
    static_assert(is_container_v<STL>, "Only STL-like container can be used");
    static_cast<Derived *>(this)->impl_to_linear_container(stl_ranges);
  }

  template<typename Derived>
  template<typename STL>
  void ParticleSystem<Derived>::load_from_linear_container(const STL &stl_ranges) {
    static_assert(is_container_v<STL>, "Only STL-like container can be used");
    static_cast<Derived *>(this)->impl_load_from_linear_container(stl_ranges);
  }

  template<typename Derived>
  std::ostream &operator<<(std::ostream &os, ParticleSystem<Derived> const &ps) {
    os << static_cast<Derived const &>(ps);
    return os;
  }

  template<typename Derived>
  std::istream &operator>>(std::istream &is, ParticleSystem<Derived> &ps) {
    is >> static_cast<Derived &>(ps);
    return is;
  }

/*---------------------------------------------------------------------------*\
    Help functions and tools
\*---------------------------------------------------------------------------*/
  template<typename T>
  constexpr bool is_particle_system_v = std::is_base_of_v<ParticleSystem<T>, T>;
}  // namespace space

#endif  // SPACEHUB_PARTICLE_SYSTEM_HPP
