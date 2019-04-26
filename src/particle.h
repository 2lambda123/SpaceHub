
#ifndef PARTICLES_H
#define PARTICLES_H

#include "dev-tools.h"
#include "vector/vector3.h"
#include "type-class.h"

namespace space {

    /*---------------------------------------------------------------------------*\
        Class PointParticle Declaration
    \*---------------------------------------------------------------------------*/
    template<typename Real>
    struct PointParticle {
    public:
        //type members
        using Scalar = Real;
        using Vector = Vec3<Scalar>;

        //constructors
        PointParticle() = default;

        explicit PointParticle(Scalar m, Vector p, Vector v);

        explicit PointParticle(Scalar m, Scalar px = 0, Scalar py = 0, Scalar pz = 0, Scalar vx = 0, Scalar vy = 0,
                               Scalar vz = 0);

        //friend methods
        friend std::ostream &operator<<(std::ostream &os, PointParticle const &particle);

        friend std::istream &operator>>(std::istream &is, PointParticle &particle);

        //public members
        Vector pos;
        Vector vel;
        Scalar mass;
    };

    /*---------------------------------------------------------------------------*\
        Class PointParticle Implementation
    \*---------------------------------------------------------------------------*/
    template<typename Real>
    PointParticle<Real>::PointParticle(Scalar m, PointParticle::Vector p, PointParticle::Vector v)
            : pos(p), vel(v), mass(m) {}

    template<typename Real>
    PointParticle<Real>::PointParticle(Scalar m, Scalar px, Scalar py, Scalar pz, Scalar vx, Scalar vy, Scalar vz)
            : pos(px, py, pz), vel(vx, vy, vz), mass(m) {}

    template<typename Real>
    std::ostream &operator<<(std::ostream &os, PointParticle<Real> const &particle) {
        space::display(os, particle.mass, particle.pos, particle.vel);
        return os;
    }

    template<typename Real>
    std::istream &operator>>(std::istream &is, PointParticle<Real> &particle) {
        space::input(is, particle.mass, particle.pos, particle.vel);
        return is;
    }

    /*---------------------------------------------------------------------------*\
        Class SoAParticles Declaration
    \*---------------------------------------------------------------------------*/
    template<typename Derived>
    class SoAParticles {
    public:
        //public methods
        DECLARE_CRTP_ACCESSOR(Derived, auto, idn);

        DECLARE_CRTP_ACCESSOR(Derived, auto, mass);

        DECLARE_CRTP_ACCESSOR(Derived, auto, pos);

        DECLARE_CRTP_ACCESSOR(Derived, auto, time);

        DECLARE_CRTP_ACCESSOR(Derived, auto, vel);

        Derived &derived();

        size_t number() const;

        void reserve(size_t new_cap);

        void resize(size_t new_sz);

    private:
        //constructors
        SoAParticles() = default;

        friend Derived;
    };

    /*---------------------------------------------------------------------------*\
        Class SoAParticles Implementation
    \*---------------------------------------------------------------------------*/
    template<typename Derived>
    Derived &SoAParticles<Derived>::derived() {
        return static_cast<Derived &>(*this);
    }

    template<typename Derived>
    size_t SoAParticles<Derived>::number() const {
        return static_cast<Derived const *>(this)->impl_number();
    }

    template<typename Derived>
    void SoAParticles<Derived>::reserve(size_t new_cap) {
        static_cast<Derived *>(this)->impl_reserve(new_cap);
    }

    template<typename Derived>
    void SoAParticles<Derived>::resize(size_t new_sz) {
        static_cast<Derived *>(this)->impl_resize(new_sz);
    }

    /*---------------------------------------------------------------------------*\
        Help functions and tools
    \*---------------------------------------------------------------------------*/
    template<typename T>
    constexpr bool is_soa_particles_v = std::is_base_of_v<SoAParticles<T>, T>;

#define SPACEHUB_PARTICLE_TYPE_CHECK(CTR, VAL) static_assert(std::is_base_of_v<typename CTR::value_type, VAL>,         \
        "Class can only be initialized by containers with its internal 'Particle' type!");

}
#endif

