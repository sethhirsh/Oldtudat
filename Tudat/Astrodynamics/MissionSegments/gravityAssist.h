/*!   Copyright (c) 2010-2012 Delft University of Technology.
 *
 *    This software is protected by national and international copyright.
 *    Any unauthorized use, reproduction or modification is unlawful and
 *    will be prosecuted. Commercial and non-private application of the
 *    software in any form is strictly prohibited unless otherwise granted
 *    by the authors.
 *
 *    The code is provided without any warranty; without even the implied
 *    warranty of merchantibility or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110117    E. Iorfida        First creation of code.
 *      110128    E. Iorfida        Added member variables necessary for the unit tests.
 *      110202    J. Melman         Changed several names and suggested minor restructuring of
 *                                  class.
 *      110203    E. Iorfida        Changed some variable names and modified punctuation.
 *      110205    J. Melman         Removed the trailing underscores in some public variables.
 *                                  Changed and added some notes.
 *      110208    E. Iorfida        Added CartesianVelocityElements objects as input. Deleted
 *                                  inheritance from TrajectoryDesignMethod.
 *      110212    J. Melman         Made delta-V private. getDeltaV changed into computeDeltaV.
 *      110214    E. Iorfida        Deleted temporary centralBodyRadius, replaced by an element
 *                                  of GeometricShapes.
 *
 *    References
 *
 */

// Temporary notes (move to class/function doxygen):
// Gravity assist and swing-by are different words for the same thing.
// The delta-V that is computed for a powered swing-by has not been
// proven to be the optimum (lowest) to achieve the desired geometry
// of incoming and outgoing hyperbolic legs.
// For the moment in this code the smallestPeriapsisDistanceFactor
// is given as external input by the user, but in the future it should
// be part of the CelestialBody object.
// Also, the velocity of the central body will need to be computed by
// the ephemeris code.
// At the moment the shape of the central body is a sphere segment,
// and the radius of the planet is set externally by the user.
// In the future it should be possible to get the radius of each planet
// directly from the CelestialBody class, by a link to GeometricShape
// class.
// At the moment, this code uses a Newton-Raphson root finder by default.
// In the future it should be possible to apply, for example, the Halley
// method by using polymorphism.
// 
// 

#ifndef TUDAT_GRAVITYASSIST_H
#define TUDAT_GRAVITYASSIST_H

// Include statements.
#include <Eigen/Core>
#include <iostream>
#include "Tudat/Astrodynamics/Bodies/celestialBody.h"
#include "Tudat/Astrodynamics/States/cartesianVelocityElements.h"
#include "Tudat/Mathematics/GeometricShapes/sphereSegment.h"
#include "Tudat/Mathematics/RootFindingMethods/newtonRaphson.h"
#include "Tudat/Mathematics/RootFindingMethods/newtonRaphsonAdaptor.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Gravity assist method class.
/*!
 * Gravity assist method class.
 */
class GravityAssist
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    GravityAssist( );

    //! Set central gravity field for the swing-by.
    /*!
     * Sets pointer to central body of the swing-by.
     * \param gravityField Central body of the swing-by.
     */
    void setCentralGravityField( GravityFieldModel* gravityField )
    { centralBodyGravityfield_ = gravityField; }

    //! Set smallest periapsis distance.
    /*!
     * Sets the smallest allowable periapsis distance of the swing-by.
     * For maximum swing-by energy this is the central body radius.
     * \param smallestPeriapsisDistance Smallest periapsis distance.
     * \sa GravityAssist::smallestPeriapsisDistance.
     */
    void setSmallestPeriapsisDistance( double smallestPeriapsisDistance )
    { smallestPeriapsisDistance_ = smallestPeriapsisDistance; }

    // Its input will come from the ephemeris class.
    //! Set velocity of the swing-by central body.
    /*!
     * Sets the velocity of the central body involved in the swing-by,
     * which will come from the Ephemeris class.
     * \param centralBodyVelocity Velocity of swing-by central body.
     */
    void setCentralBodyVelocity( Eigen::Vector3d centralBodyVelocity )
    { centralBodyVelocity_ = centralBodyVelocity; }

    //! Set pointer to incoming velocity of the satellite.
    /*!
     * Sets pointer to the incoming velocity vector of the satellite.
     * \param pointerToIncomingVelocity Pointer to incoming velocity of the
     *          satellite.
     */
    void setPointerToIncomingVelocity( CartesianVelocityElements* pointerToIncomingVelocity )
    { pointerToIncomingVelocity_ = pointerToIncomingVelocity; }

    //! Set pointer to outgoing velocity of the satellite.
    /*!
     * Sets pointer to the outgoing velocity of the satellite.
     * \param pointerToOutgoingVelocity Pointer to outgoing velocity of the satellite.
     */
    void setPointerToOutgoingVelocity( CartesianVelocityElements* pointerToOutgoingVelocity )
    { pointerToOutgoingVelocity_ = pointerToOutgoingVelocity; }

    //! Set pointer to Newton-Raphson method for gravity assist algorithm.
    /*!
     * Sets a pointer to the Newton-Raphson method for gravity assist
     * algorithm.
     * \param pointerToNewtonRaphson Pointer to NewtonRaphson object.
     */
    void setNewtonRaphsonMethod( NewtonRaphson *pointerToNewtonRaphson )
    { pointerToNewtonRaphson_ = pointerToNewtonRaphson; }

    //! Compute the delta-V of a powered swing-by.
    /*!
     * Computes the necessary delta-V to perform a powered swing-by.
     * \return deltaV Necessary delta-V of swing-by.
     */
    double computeDeltaV( );

    //! Overload ostream to print class information.
    /*!
     * Overloads ostream to print class information.
     * \param stream Stream object.
     * \param gravityAssist Gravity Assist.
     * \return Stream object.
     */
    friend std::ostream& operator<<( std::ostream& stream, GravityAssist& gravityAssist );

protected:

private:

    //! The gravity field produced by the CelestialBody.
    /*!
     * The gravity field in which the swing-by is performed.
     */
    GravityFieldModel* centralBodyGravityfield_;

    //! Velocity of the swing-by central body.
    /*!
     * Velocity vector of the central body involved in the swing-by.
     */
    Eigen::Vector3d centralBodyVelocity_;

    //! Smallest periapsisDistance.
    /*!
     * The smallest allowable periapsis distance. This is the radius of closest possible approach to
     * the planet. For maximal swing-by energy, this is the central body radius.
     */
    double smallestPeriapsisDistance_;

    //! Pointer to CartesianVelocityElements object.
    /*!
     * Pointer to CartesianVelocityElements object.
     */
    CartesianVelocityElements* pointerToIncomingVelocity_;

    //! Pointer to CartesianVelocityElements object.
    /*!
     * Pointer to CartesianVelocityElements object.
     */
    CartesianVelocityElements* pointerToOutgoingVelocity_;

    //! Hyperbolic excess velocity of the incoming leg.
    /*!
     * Hyperbolic excess velocity of the incoming leg.
     */
    Eigen::Vector3d incomingHyperbolicExcessVelocity_;

    //! Hyperbolic excess velocity of the outgoing leg.
    /*!
     * Hyperbolic excess velocity of the outgoing leg.
     */
    Eigen::Vector3d outgoingHyperbolicExcessVelocity_;

    //! Delta-V of powered gravity assist.
    /*!
     * Necessary delta-V to perform a powered swing-by.
     */
    double deltaV_;

    //! Bending angle between the excess velocities.
    /*!
     * Bending angle between the excess velocities.
     */
    double bendingAngle_;

    //! Eccentricity of the incoming hyperbolic leg.
    /*!
     * Eccentricity of the incoming hyperbolic leg.
     */
    double incomingEccentricity_;

    //! Eccentricity of the outgoing hyperbolic leg.
    /*!
     * Eccentricity of the outgoing hyperbolic leg.
     */
    double outgoingEccentricity_;

    //! Semi-major axis of the incoming hyperbolic leg.
    /*!
     *  Semi-major axis of the incoming hyperbolic leg.
     */
    double incomingSemiMajorAxis_;

    //! Semi-major axis of the outgoing hyperbolic leg.
    /*!
     * Semi-major axis of the outgoing hyperbolic leg.
     */
    double outgoingSemiMajorAxis_;

    //! Delta-V due to the effect of large bending angle.
    /*!
     * Delta-V due to the effect of large bending angle between excess
     * velocities.
     */
    double bendingEffectDeltaV_;

    //! Delta-V due to the effect of different excess speeds.
    /*!
     * Delta-V due to the effect of different excess speeds.
     */
    double velocityEffectDeltaV_;

    //! Pointer to object of NewtonRaphson class.
    /*!
     * Pointer to object of NewtonRaphson class.
     */
    NewtonRaphson* pointerToNewtonRaphson_;

    //! Pointer to adaptor object of NewtonRaphsonAdaptor class.
    /*!
     * Pointer to adaptor object of NewtonRaphsonAdaptor class. The template
     * parameter passed is this class.
     */
    NewtonRaphsonAdaptor< GravityAssist > newtonRaphsonAdaptorForGravityAssist_;

    //! Define root-finder function.
    /*!
     * Defines root-finder function for the velocity-effect delta-V.
     * \param incomingEccentricity Incoming eccentricity.
     * \return Value of root-finder function at defined eccentricity.
     */
    double velocityEffectFunction( double& incomingEccentricity );

    //! Define first derivative of root-finder function.
    /*!
     * Defines first derivative of root-finder function for the velocity-effect
     * delta-V.
     * \param incomingEccentricity_ Incoming eccentricity.
     * \return Value of first derivative of root-finder function at defined
     *          eccentricity.
     */
    double firstDerivativeVelocityEffectFunction( double& incomingEccentricity );
};

}

#endif // TUDAT_GRAVITYASSIST_H

// End of file.
