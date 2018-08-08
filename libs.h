
#ifndef LIBS_H
#define LIBS_H
#include "vector3.h"

namespace SpaceH
{
    
/** @brief Self min()*/
template<typename T1, typename T2>
inline const T2 min(const T1& x, const T2& y)
{
    return x > y ? y : x;
}

/** @brief Self max()*/
template<typename T1, typename T2>
inline const T2 max(const T1& x, const T2& y)
{
    return y > x ? y : x;
}

/** @brief Self abs()*/
template<class T>
inline const T abs(const T& x)
{
    return x > -x ? x : -x;
}

/** @brief Self swap()*/
/*template <class T>
void swap(T& a, T& b)
{
    T tmp(std::move(a));
    a = std::move(b);
    b = std::move(tmp);
}*/


    
template<typename Scalar1, typename Scalar2>
inline void advanceScalar(Scalar1& var, Scalar2 increase)
{
    var += increase;
}

template<typename Scalar, typename VectorArray>
inline void advanceVector(VectorArray& var, const VectorArray& increase, Scalar stepSize )
{
    size_t size = var.size();

    for(size_t i = 0 ; i < size; i++)
    {
        var[i] += increase[i] * stepSize;
    }
}

    /** @brief Move variables to centr
     
     al mass coordinates
     *
     *  @param mass   Array of mass.
     *  @param phyVar Array of variables need to be moved.
     */
    template<typename ScalarArray, typename VectorArray>
    typename VectorArray::value_type calcuCMCoord(const ScalarArray& mass, VectorArray& phyVar)
    {
        typename VectorArray::value_type centralMassVar(0.0, 0.0, 0.0);
        typename ScalarArray::value_type totalMass = 0;
        
        const size_t N = mass.size();
        
        for(size_t i = 0 ; i < N ; ++i)
        {
            totalMass += mass[i];
            centralMassVar += phyVar[i] * mass[i];
        }
        
        centralMassVar /= totalMass;
        
        return centralMassVar;
    }
    
    /** @brief Move variables to central mass coordinates
     *
     *  @param mass      Array of mass.
     *  @param phyVar    Array of variables need to be moved.
     *  @param totalMass The totalMass of the system
     */
    template<typename ScalarArray, typename VectorArray, typename Scalar> 
    typename VectorArray::value_type calcuCMCoord(const ScalarArray& mass, VectorArray& phyVar, const Scalar totalMass)
    {
        typename VectorArray::value_type centralMassVar(0.0, 0.0, 0.0);
        
        const size_t N = mass.size();
        
        for(size_t i = 0 ; i < N ; ++i)
        {
            centralMassVar += phyVar[i] * mass[i];
        }
        
        centralMassVar /= totalMass;
        
        return centralMassVar;
    }
    
/** @brief Move variables to central mass coordinates
 *
 *  @param mass   Array of mass.
 *  @param phyVar Array of variables need to be moved.
 */
template<typename VectorArray>
    void moveToCMCoord(VectorArray& phyVar, const typename VectorArray::value_type& centralMassVar )
{
    const size_t N = phyVar.size();
    
    for(size_t i = 0 ; i < N ; ++i)
    {
        phyVar[i] -= centralMassVar;
    }
}

/** @brief Calculate the kinetic energy of particles
 *
 *  @param  mass          Array of mass.
 *  @param  vel           Array of velocity.
 *  @return The kinetic energy.
 */
template<typename ScalarArray, typename VectorArray>
typename ScalarArray::value_type getKineticEnergy(const ScalarArray& mass, const VectorArray& vel)
{
    typename ScalarArray::value_type kineticEnergy = 0;

    size_t size = mass.size();
    
    for(size_t i = 0 ;  i < size ; ++i)
        kineticEnergy += 0.5 * mass[i] * ( vel[i] * vel[i] );

    return kineticEnergy;
}

    /** @brief Calculate the minimal fall free time of two particles
     *
     *  @param  mass mass array of particle.
     *  @param  pos position array of particle.
     *  @return The minimal fall free time of the two particles
     */
    template<typename ScalarArray, typename VectorArray>
    inline typename ScalarArray::value_type minfallFreeTime(const ScalarArray& mass, const VectorArray& pos)
    {
        size_t size = mass.size();
        typename ScalarArray::value_type r = 0;
        typename ScalarArray::value_type min_fall_free = 0;
        typename ScalarArray::value_type fall_free = 0;
        
        for(size_t i = 0 ; i < size; i++)
        {
            for(size_t j = i + 1 ; j < size; j++)
            {
                r = (pos[i] - pos[j]).norm();
                fall_free  = pow(r, 1.5)/(mass[i] + mass[j]);
                min_fall_free = min_fall_free ==0 ?  fall_free : SpaceH::min(min_fall_free, fall_free);
            }
        }
        
        return 0.1*min_fall_free;
    }
    
    template<typename VectorArray>
    bool isAllZero(const VectorArray& array)
    {
        size_t size = array.size();
        for(size_t i = 0 ; i < size; i++)
        {
            if(array[i].norm() > 0)
                return false;
        }
        return true;
    }
    
    /** @brief Calculate the minimal fall free time of two particles
     *
     *  @param  mass mass array of particle.
     *  @param  pos position array of particle.
     *  @param  vel velocity array of particle.
     *  @return The minimal fall free time of the two particles
     */
    template<typename ScalarArray, typename VectorArray>
    inline typename ScalarArray::value_type minAccdot(const ScalarArray& mass, const VectorArray& pos, const VectorArray& vel)
    {
        size_t size = mass.size();
        typename VectorArray::value_type acc;
        typename VectorArray::value_type adot;
        typename VectorArray::value_type dr;
        typename VectorArray::value_type dv;
        
        typename ScalarArray::value_type r = 0;
        typename ScalarArray::value_type min_ds = 0;
        typename ScalarArray::value_type ds = 0;
        
        for(size_t i = 0 ; i < size; i++)
        {
            adot.setZero();
            acc.setZero();
            for(size_t j = 0 ; j < size; j++)
            {
                if(i != j)
                {
                    dr = pos[i] - pos[j];
                    dv = vel[i] - vel[j];
                    r  = dr.norm();
                    acc += dr/(r*r*r)*mass[j];
                    adot += ( dv/(r*r*r) + dr*(3*(dv*dr)/(r*r*r*r*r)) )*mass[j];
                }
            }
            ds = acc.norm() / adot.norm();
            min_ds = min_ds == 0 ? ds : SpaceH::min(min_ds, ds);
        }
        
        return min_ds;
    }
    
/** @brief Calculate the potential energy of particles
 *
 *  @param  mass            Array of mass.
 *  @param  pos             Array of position.
 *  @return The potential energy.
 */
template<typename ScalarArray, typename VectorArray>
typename ScalarArray::value_type getPotentialEnergy(const ScalarArray& mass, const VectorArray& pos)
{
    typename ScalarArray::value_type potentialEnergy = 0;

    size_t size = mass.size();
    
    for(size_t i = 0 ;  i < size ; ++i)
        for(size_t j = i + 1; j < size; ++j)
            potentialEnergy -= mass[i] * mass[j] / distance(pos[i], pos[j]);

    return potentialEnergy;
}

/** @brief Calculate the total(potential + kinetic) energy of particles
 *
 *  @param  mass            Array of mass.
 *  @param  pos             Array of position.
 *  @param  vel             Array of velocity.
 *  @return The total energy.
 */
template<typename ScalarArray, typename VectorArray>
inline typename ScalarArray::value_type getTotalEnergy(const ScalarArray& mass, const VectorArray& pos,
                             const VectorArray& vel)
{
    typename ScalarArray::value_type potentialEnergy = 0;
    typename ScalarArray::value_type kineticEnergy   = 0;
    size_t size = mass.size();
    
    for(size_t i = 0 ;  i < size ; ++i)
    {
        kineticEnergy += 0.5 * mass[i] * ( vel[i] * vel[i] );

        for(size_t j = i + 1; j < size; ++j)
            potentialEnergy -= mass[i] * mass[j] / distance(pos[i], pos[j]);
    }

    return potentialEnergy + kineticEnergy;
}

    template<typename ScalarArray, typename VectorArray, typename Scalar>
    inline Scalar getEnergyErr(const ScalarArray& mass, const VectorArray& pos, const VectorArray& vel, const Scalar bindE)
    {
        Scalar EK = getKineticEnergy(mass, vel);
        Scalar EP = getPotentialEnergy(mass, pos);
        return log(abs((EK + bindE)/EP));
    }
    
/** @brief print an array. Used for debug*/
template<typename T>
void print(T& var)
{
    const size_t size = var.size();
    for(size_t i = 0 ; i < size; ++i )
        std::cout << var[i] << '\n';

    std::cout << '\n';
}
    
}//end namespace SpaceH
#endif
