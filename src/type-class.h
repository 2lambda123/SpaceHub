#ifndef TYPECLASS_H
#define TYPECLASS_H

#include "vector/vector3.h"
#include "dev-tools.h"
#include <array>
#include <vector>

namespace SpaceH {

    template<typename T>
    struct Coord{
        using Scalar = typename T::value_type;
        using Vector = Vec3<Scalar>;

        Coord() = default;

        explicit Coord(size_t count) : x(count), y(count), z(count) {}

        Coord(Coord const & other) : x(other.x), y(other.y), z(other.z) {}

        Coord(Coord&& other) noexcept: x(std::move(other.x)), y(std::move(other.x)), z(std::move(other.z)) {}

        Coord& operator=(Coord const& other) {
            x = other.x;
            x = other.y;
            x = other.z;
        }

        Coord& operator=(Coord && other) noexcept {
            x = std::move(other.x);
            x = std::move(other.y);
            x = std::move(other.z);
        }

        size_t size() {
            return x.size();
        }

        void reserve(size_t new_cap){
            x.reserve(new_cap);
            y.reserve(new_cap);
            z.reserve(new_cap);
        }

        void resize(size_t new_sz){
            x.resize(new_sz);
            y.resize(new_sz);
            z.resize(new_sz);
        }

        template<typename Vector>
        void emplace_back(Vector&& v){
            x.emplace_back(std::forward<Scalar>(v.x));
            y.emplace_back(std::forward<Scalar>(v.y));
            z.emplace_back(std::forward<Scalar>(v.z));
        }

        void emplace_back(Scalar && xx, Scalar && yy, Scalar && zz){
            x.emplace_back(std::forward<Scalar>(xx));
            y.emplace_back(std::forward<Scalar>(yy));
            z.emplace_back(std::forward<Scalar>(zz));
        }

        void shrink_to_fit() {
            x.shrink_to_fit();
            y.shrink_to_fit();
            z.shrink_to_fit();
        }

        T x;
        T y;
        T z;
    };

    template<typename Real, template<class...> class TContainer = std::vector>
    struct TypeSystem {
        template<typename ...T>
        using Container = TContainer<T...>;

        using Scalar      = Real;
        using ScalarArray = Container<Scalar>;
        using IntArray    = Container<int>;
        using IdxArray    = Container<size_t>;
        using Vector      = Vec3<Scalar>;
        using VArray      = Container<Vector>;
        using Coord       = Coord<ScalarArray>;
    };
}//end namespace SpaceH

#endif
