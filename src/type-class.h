#ifndef TYPECLASS_H
#define TYPECLASS_H

#include "vector/vector3.h"
#include "dev-tools.h"
#include <array>
#include <vector>

namespace SpaceH {

    template<typename T>
    struct {
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
        using xxxx = std::tuple<ScalarArray &, ScalarArray &, ScalarArray &>;
        using xxxy = std::tuple<ScalarArray const &, ScalarArray const &, ScalarArray const &>;
    };
}//end namespace SpaceH

#endif