/*! \file orbitalElementConversions.cpp
 *    This source file contains a namespace with orbital element conversion functions.
 *
 *    Path              : /Astrodynamics/BasicAstrodynamics/
 *    Version           : 19
 *    Check status      : Checked
 *
 *    Author            : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Checker           : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Checker           : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Date created      : 20 October, 2010
 *    Last modified     : 3 February, 2011
 *
 *    References
 *      Wertz, J. R. Mission geometry; orbit and constellation design and management.
 *      Mengali, G., Quarta, A.A. Fondamenti di meccanica del volo spaziale
 *
 *    Notes
 *
 *    Copyright (c) 2010-2011 Delft University of Technology.
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
 *      101020    K. Kumar          First creation of code.
 *      101025    E. Iorfida        First development of the code with conversion equations.
 *      101028    E. Iorfida        Modification of code for updating of the  Keplerian elements
 *                                  and Cartesian elements classes.
 *      101103    E. Iorfida        Additional conversion equations for extra  Keplerian elements.
 *      101104    E. Iorfida        Modification of the code (no pointers, but directly call of
 *                                  variables).
 *      101119    E. Iorfida        Removed computation for extra Keplerian elements.
 *      101130    E. Iorfida        Added different orbital cases with if-else operators.
 *      101202    J. Melman         Compile errors taken out.
 *      101203    E. Iorfida        Added gravitational parameter, and modified punctuation.
 *      101215    E. Iorfida        Added tolerance, modified punctuation, added comments, deleted
 *                                  raiseToIntegerExponent, used pow.
 *      101219    J. Melman         Suggested efficiency improvement of if-statements.
 *      110107    E. Iorfida        Written a better definition of the range in which angles are
 *                                  computed, and made some punctuation modifications.
 *      110109    J. Melman         Incorporated function computeModulo and
 *                                  computeAngleBetweenVectors. Reduced number of if-statements
 *                                  considerably and bundled all eccentricity and inclination
 *                                  checks in convertCartesianTopointerToCartesianElements_
 *      110128    K. Kumar          Changed references to pointers.
 *      110204    K. Kumar          Removed "vector" naming.
 *      110310    K. Kumar          Changed right ascension of ascending node to longitude of
 *                                  ascending node.
 *      110510    K. Kumar          Updated conversion functions to not use dynamic memory
 *                                  allocation.
 *      110805    K. Kumar          Added mean motion to semi-major axis conversion.
 *      120203    K. Kumar          Removed functions available in Tudat Core; changed
 *                                  implementations of remaining functions to use Tudat Core;
 *                                  moved to new BasicAstrodynamics sub-directory.
 */

// Include statements.
#include <boost/math/special_functions/atanh.hpp>
#include <cmath>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <limits>
#include <TudatCore/Astrodynamics/orbitalElementConversions.h>
#include <TudatCore/Mathematics/linearAlgebra.h>
#include "Tudat/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h"

//! Tudat library namespace.
namespace tudat
{

// Using declarations.
using std::acos;
using std::atan2;
using std::cos;
using std::cosh;
using std::sin;
using std::sinh;
using std::sqrt;
using std::pow;
using std::fabs;
using mathematics::linear_algebra::computeAngleBetweenVectors;
using tudat::mathematics::computeModulo;

//! Orbital element conversions namespace.
namespace orbital_element_conversions
{

//! Convert Keplerian to Cartesian orbital elements.
CartesianElements convertKeplerianToCartesianElements(
    KeplerianElements* pointerToKeplerianElements, CelestialBody* pointerToCelestialBody )
{
    // Declare computed Cartesian elements.
    CartesianElements computedCartesianElements_;

    // Compute Cartesian elements.
    computedCartesianElements_.state = tudat::orbital_element_conversions::
            convertKeplerianToCartesianElements(
                pointerToKeplerianElements->state,
                pointerToCelestialBody->getGravitationalParameter( ) );

    // Return computed Cartesian elements.
    return computedCartesianElements_;
}

//! Convert Cartesian to Keplerian orbital elements.
KeplerianElements convertCartesianToKeplerianElements(
    CartesianElements* pointerToCartesianElements, CelestialBody* pointerToCelestialBody )
{
    // Declare computed Keplerian elements.
    KeplerianElements computedKeplerianElements_;

    // Compute Keplerian elements.
    computedKeplerianElements_.state = tudat::orbital_element_conversions::
            convertCartesianToKeplerianElements(
                pointerToCartesianElements->state,
                pointerToCelestialBody->getGravitationalParameter( ) );

    // Return computed Cartesian elements.
    return computedKeplerianElements_;
}

//! Convert elapsed time to mean anomaly for elliptical orbits.
double convertElapsedTimeToMeanAnomalyForEllipticalOrbits(
    double elapsedTime, CelestialBody* pointerToCentralBody, double semiMajorAxis )
{
    return tudat::orbital_element_conversions::
            convertElapsedTimeToMeanAnomalyChangeForEllipticalOrbits(
                elapsedTime, pointerToCentralBody->getGravitationalParameter( ), semiMajorAxis );
}

//! Convert mean anomaly to elapsed time for elliptical orbits.
double convertMeanAnomalyToElapsedTimeForEllipticalOrbits(
    double meanAnomaly, CelestialBody* pointerToCentralBody, double semiMajorAxis )
{
    return tudat::orbital_element_conversions::
            convertMeanAnomalyChangeToElapsedTimeForEllipticalOrbits(
                meanAnomaly, pointerToCentralBody->getGravitationalParameter( ), semiMajorAxis );
}

//! Convert elapsed time to mean anomaly for hyperbolic orbits.
double convertElapsedTimeToMeanAnomalyForHyperbolicOrbits(
    double elapsedTime, CelestialBody* pointerToCentralBody, double semiMajorAxis )
{
    return tudat::orbital_element_conversions::
            convertElapsedTimeToMeanAnomalyChangeForHyperbolicOrbits(
                elapsedTime, pointerToCentralBody->getGravitationalParameter( ), semiMajorAxis );
}

//! Convert mean anomaly to elapsed time for hyperbolic orbits.
double convertMeanAnomalyToElapsedTimeForHyperbolicOrbits(
    double meanAnomaly, CelestialBody* pointerToCentralBody, double semiMajorAxis )
{
    return tudat::orbital_element_conversions::
            convertMeanAnomalyChangeToElapsedTimeForHyperbolicOrbits(
                meanAnomaly, pointerToCentralBody->getGravitationalParameter( ), semiMajorAxis );
}

//! Convert mean motion to semi-major axis.
double convertMeanMotionToSemiMajorAxis( double meanMotion, CelestialBody* pointerToCentralBody )
{
    return tudat::orbital_element_conversions::convertMeanMotionToSemiMajorAxis(
                meanMotion, pointerToCentralBody->getGravitationalParameter( ) );
}

}

}

// End of file.
