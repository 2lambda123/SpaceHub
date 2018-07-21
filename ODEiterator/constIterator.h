////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Filename:constIterator.h                                                                                            //
//Author:Yihan Wang                                                                                                   //
//                                                                                                                    //
//                                                                                                                    //
//Description:                                                                                                        //
//                                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef CONSTITERATOR_H
#define CONSTITERATOR_H
/** @brief Most common iterator
 *
 *  Constant iterator keep the step length constant and integrate the particle system for one step.
 */
template <typename ParticSys, typename Integrator>
class constIterator
{
public:
    /* Typedef */
    using type   = typename ParticSys::type;
    using Scalar = typename type::Scalar;
    /* Typedef */
    
    /*Template parameter check*/
    static_assert(std::is_same< typename ParticSys::type, typename Integrator::type>::value,
                  "Template arg 'ParticSys' and 'Integrator' must have the same type set!");
    CREATE_METHOD_CHECK(integrate)
    static_assert(HAS_METHOD(Integrator, integrate,  ParticSys&, Scalar),
                  "Template arg 'Integrator' must have method 'integrate(ParticSys&, Scalar)' !!!");
    /*Template parameter check*/
    
    /** @brief interface to iterate particle system for one step
     *  @param particles  Particle system needs evolution.
     *  @param integrator Integrator to integrate the particle system.
     *  @param stepLength Macro step length for iteration(Here, the step length of the integrator).
     *  @return step length for next iteration.
     */
    Scalar iterate(ParticSys& particles, Scalar stepLength)
    {
        integrator.integrate(particles, stepLength);
        return stepLength;
    }
private:
    Integrator integrator;
};

#endif
