/*    Copyright (c) 2010-2013, Delft University of Technology
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without modification, are
 *    permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright notice, this list of
 *        conditions and the following disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *      - Neither the name of the Delft University of Technology nor the names of its contributors
 *        may be used to endorse or promote products derived from this software without specific
 *        prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 *    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *    COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *    OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      120827    K. Kumar          File created.
 *      121105    K. Kumar          Simplified base class definition.
 *      121210    D. Dirkx          Simplified class by removing template parameters.
 *
 *    References
 *
 *    Notes
 *
 */

#ifndef TUDAT_SPHERICAL_HARMONICS_GRAVITATIONAL_ACCELERATION_MODEL_BASE_H
#define TUDAT_SPHERICAL_HARMONICS_GRAVITATIONAL_ACCELERATION_MODEL_BASE_H

#include <boost/function.hpp>

namespace tudat
{
namespace gravitation
{

//! Template base class for spherical harmonics gravitational acceleration models.
/*!
 * This template class serves as the base class for the
 * SphericalHarmonicsGravitationalAccelerationModel, CentralGravitationalAccelerationModel,
 * CentralJ2GravitationalAccelerationModel, CentralJ2J3GravitationalAccelerationModel, and
 * CentralJ2J3J4GravitationalAccelerationModel classes.
 * \tparam StateMatrix Type used to store a state matrix.
 */
template< typename StateMatrix >
class SphericalHarmonicsGravitationalAccelerationModelBase
{
protected:

    //! Typedef for a position-returning function.
    typedef boost::function< StateMatrix( ) > StateFunction;

public:

    //! Default constructor taking position of body subject to acceleration, variable
    //! gravitational parameter, and position of body exerting acceleration.
    /*!
     * Constructor taking a pointer to a function returning the position of the body subject to
     * gravitational acceleration, a pointer to a function returning the gravitational parameter of
     * the body exerting the acceleration, and a pointer to a function returning the position of
     * the body exerting the  gravitational acceleration (typically the central body). The
     * constructor also updates all the internal members.
     * \param positionOfBodySubjectToAccelerationFunction Pointer to function returning position of
     *          body subject to gravitational acceleration.
     * \param gravitationalParameterFunction Pointer to function returning gravitational parameter
     *          of body exerting gravitational acceleration.
     * \param positionOfBodyExertingAccelerationFunction Pointer to function returning position of
     *          body exerting gravitational acceleration.
     */
    SphericalHarmonicsGravitationalAccelerationModelBase(
            const StateFunction positionOfBodySubjectToAccelerationFunction,
            const double aGravitationalParameter,
            const StateFunction positionOfBodyExertingAccelerationFunction )
        : subjectPositionFunction( positionOfBodySubjectToAccelerationFunction ),
          gravitationalParameter( aGravitationalParameter ),
          sourcePositionFunction( positionOfBodyExertingAccelerationFunction )
    { }

    //! Virtual destructor.
    /*!
     * Base class virtual destructor.
     */
    virtual ~SphericalHarmonicsGravitationalAccelerationModelBase( ){ }

    //! Update base class members.
    /*!
     * Updates members of base class. In this case, the current position of the body subject to and
     * exerting the gravitational acceleration, as well as the gravitational parameter are updated
     * by calling the functions provided through the constructor.
     * \return True; this should be modified to return a flag indicating if the update was
     *          successful.
     */
    virtual bool updateMembers( )
    {
        this->positionOfBodySubjectToAcceleration = this->subjectPositionFunction( );
        this->positionOfBodyExertingAcceleration  = this->sourcePositionFunction( );
        return true;
    }

protected:

    //! Position of body subject to acceleration.
    /*!
     * Current position vector of body subject to gravitational acceleration in inertial frame.
     */
    StateMatrix positionOfBodySubjectToAcceleration;

    //! Pointer to function returning position of body subject to acceleration.
    /*!
     * Pointer to function that returns the current position of the body subject to the
     * gravitational acceleration.
     */
    const StateFunction subjectPositionFunction;

    //! Gravitational parameter [m^3 s^-2].
    /*!
     * Current gravitational parameter of body exerting acceleration [m^3 s^-2].
     */
    const double gravitationalParameter;

    //! Position of body exerting acceleration.
    /*!
     * Current position vector of body exerting gravitational acceleration in inertial frame.
     */
    StateMatrix positionOfBodyExertingAcceleration;

    //! Pointer to function returning position of body exerting acceleration.
    /*!
     * Pointer to function that returns the current position of the body exerting the
     * gravitational acceleration.
     */
    const StateFunction sourcePositionFunction;

private:
};

} // namespace gravitation
} // namespace tudat

#endif // TUDAT_SPHERICAL_HARMONICS_GRAVITATIONAL_ACCELERATION_MODEL_BASE_H
