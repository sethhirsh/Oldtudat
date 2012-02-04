/*! \file unitTestOrbitalElementConversions.cpp
 *    Source file of unit test for the orbitalElementConversion, from Cartesian to Keplerian and
 *    viceversa. The first part of the code tests the code for elliptical, parabolic, hyperbolic
 *    and circular orbits. SI units are used. The second part of the code tests the code from
 *    Cartesian to Keplerian with the example 3.4 pag. 63 of the book "Fondamenti di Meccanica del
 *    Volo Spaziale" (G. Mengali, A.A. Quarta). In this part of the code, canonical units are used.
 *
 *    Path              : /Astrodynamics/States/
 *    Version           : 13
 *    Check status      : Checked
 *
 *    Author            : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 3 December, 2010
 *    Last modified     : 10 May, 2011
 *
 *    References
 *      http://www.astro.uu.nl/~strous/AA/en/reken/kepler.html, last accessed: 16th February, 2011.
 *      Vallado, D. A., McClain, W. D. Fundamentals of astrodynamics and applications, 2nd Edition,
 *          Kluwer Academic Publishers, The Netherlands, 2004.
 *      Fortescue, P. W., et al. Spacecraft systems engineering, Third Edition,
 *          Wiley, England, 2003.
 *
 *    Notes
 *      Test runs code and verifies result against expected value.
 *      If the tested code is erroneous, the test function returns a boolean
 *      true; if the code is correct, the function returns a boolean false.
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
 *      101203    E. Iorfida        First creation of the code.
 *      101208    E. Iorfida        Fulfillment of the code with the elliptical case.
 *      101208    E. Iorfida        Modified punctuation.
 *      101215    E. Iorfida        Added tolerance, added parabolic, circular and hyperbolic
 *                                  cases.
 *      101217    E. Iorfida        Added computeAbsoluteValue( ) in the errors computation,
 *                                  modified punctuation.
 *      101219    J. Melman         Put gravitational parameters in one place, changed first right
 *                                  ascension to 15.0 * pi / 8.0, thereby exposing a possible
 *                                  error.
 *      110107    E. Iorfida        orbitalConversionBookExampleUnitTest.test added to this file,
 *                                  to have a unique unit test file for the conversion code. Also
 *                                  some punctuation modifications have been made.
 *      110109    J. Melman         Included test for semi-latus rectum of circular case. Gave the
 *                                  orbital angles less trivial values, and not almost exclusively
 *                                  in the first quadrant.
 *      110111    E. Iorfida        Updated to the new format of unitTest file and added hyperbolic
 *                                  equatorial case.
 *      110204    K. Kumar          Removed "vector" from naming.
 *      110216    K. Kumar          Added unit tests for new orbital element conversion functions.
 *      110310    K. Kumar          Changed right ascension of ascending node to longitude of
 *                                  ascending node.
 *      110510    K. Kumar          Updated to use new orbital element conversion functions and
 *                                  removed dynamic memory allocation.
 */

// Include statements.
#include <cmath>
#include <iostream>
#include <limits>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include "Tudat/Astrodynamics/BasicAstrodynamics/convertMeanAnomalyToEccentricAnomaly.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/convertMeanAnomalyToHyperbolicEccentricAnomaly.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h"
#include "Tudat/Astrodynamics/Bodies/celestialBody.h"
#include "Tudat/Astrodynamics/Bodies/planet.h"
#include "Tudat/Astrodynamics/EnvironmentModels/gravityFieldModel.h"
#include "Tudat/Astrodynamics/EnvironmentModels/sphericalHarmonicsGravityField.h"

#include "Tudat/Mathematics/basicMathematicsFunctions.h"
#include "Tudat/Mathematics/RootFindingMethods/newtonRaphson.h"

//! Test orbital element conversion code.
int main( )
{
    // Using declarations.
    using std::cerr;
    using std::endl;
    using std::fabs;
    using std::pow;
    using tudat::orbital_element_conversions::convertCartesianToKeplerianElements;
    using tudat::orbital_element_conversions::convertKeplerianToCartesianElements;
    using tudat::orbital_element_conversions::ConvertMeanAnomalyToEccentricAnomaly;
    using tudat::orbital_element_conversions::ConvertMeanAnomalyToHyperbolicEccentricAnomaly;
    using namespace tudat;

    // Test of orbital element conversion methods imeplemented in Tudat.
    // Test 1: Test of Cartesian-to-Keplerian elements conversion and
    //         Keplerian-to-Cartesian elements conversion.
    // Test 2: Test of true anomaly to eccentric anomaly conversion.
    // Test 3: Test of eccentric anomaly to true anomaly conversion.
    // Test 4: Test of true anomaly to hyperbolic eccentric anomaly conversion.
    // Test 5: Test of hyperbolic eccentric anomaly to true anomaly conversion.
    // Test 6: Test of eccentric anomaly to mean anomaly conversion.
    // Test 7: Test of mean anomaly to eccentric anomaly conversion.
    // Test 8: Test of hyperbolic eccentric anomaly to mean anomaly conversion.
    // Test 9: Test of mean anomaly to hyperbolic eccentric anomaly conversion.
    // Test 10: Test of elapsed time to mean anomaly for elliptical orbits.
    // Test 11: Test of mean anomaly to elapsed time for elliptical orbits.
    // Test 12: Test of elapsed time to mean anomaly for hyperbolic orbits.
    // Test 13: Test of mean anomaly to elapsed time for hyperbolic orbits.

    // Test 1: Test of Cartesian-to-Keplerian elements conversion and
    //         Keplerian-to-Cartesian elements conversion.

    // Initialize unit test result to false.
    bool isOrbitalElementConversionErroneous = false;

//    // Define tolerance.
//    double errorTolerance_ = 1.0e2 * std::numeric_limits< double >::epsilon( );

//    // Test 7: Test of mean anomaly to eccentric anomaly conversion.
//    // Source: ( Vallado, 2004 ).

//    // Set tolerance for conversion.
//    double toleranceOrbitalElementConversion = 1e-8;

//    // Set mean anomaly.
//    double meanAnomaly = unit_conversions::convertDegreesToRadians( 60.0 );

//    // Create object for mean anomaly to eccentric anomaly conversion.
//    orbital_element_conversions::ConvertMeanAnomalyToEccentricAnomaly
//            convertMeanAnomalyToEccentricAnomaly;

//    // Create pointer to Newton-Raphson object.
//    NewtonRaphson* pointerToNewtonRaphson = new NewtonRaphson;

//    // Set eccentricity.
//    convertMeanAnomalyToEccentricAnomaly.setEccentricity( 0.01671 );

//    // Set mean anomaly.
//    convertMeanAnomalyToEccentricAnomaly.setMeanAnomaly( meanAnomaly );

//    // Set Newton-Raphson method.
//    convertMeanAnomalyToEccentricAnomaly.setNewtonRaphson( pointerToNewtonRaphson );

//    // Compute eccentric anomaly.
//    double eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly.convert( );

//    // Check if computed eccentric anomaly is equal to reference value.
//    if ( fabs( eccentricAnomaly - 1.061789204 ) > toleranceOrbitalElementConversion )
//    {
//        isOrbitalElementConversionErroneous = true;

//        cerr << "The conversion of mean anomaly to eccentric anomaly is "
//             << "erroneous as the computed eccentric anomaly after applying the conversion ( "
//             << unit_conversions::convertRadiansToDegrees( eccentricAnomaly )
//             << " ) does not match the expected value of the eccentric anomaly ( "
//             << unit_conversions::convertRadiansToDegrees( 1.061789204 ) << " ) " << endl;
//    }

//    // Test 10: Test of elapsed time to mean anomaly for elliptical orbits.

//    // Set tolerance for conversion.
//    toleranceOrbitalElementConversion = 1.0e-11;

//    // Expected mean anomaly value;
//    double expectedMeanAnomalyForTest10 = 20.203139666972554;

//    // Set elapsed time.
//    double expectedElapsedTime = 4000.0;

//    // Set semi-major axis.
//    double semiMajorAxis = unit_conversions::convertKilometersToMeters( 2500.0 );

//    // Compute mean anomaly.
//    meanAnomaly = orbital_element_conversions::convertElapsedTimeToMeanAnomalyForEllipticalOrbits(
//                expectedElapsedTime, &predefinedEarth, semiMajorAxis );

//    // Declare and compute absolute and relative errors.
//    double absoluteDifference = fabs( meanAnomaly - expectedMeanAnomalyForTest10 );

//    double relativeDifference = absoluteDifference / expectedMeanAnomalyForTest10;

//    // Check if relative error is too large.
//    if ( relativeDifference > std::numeric_limits< double >::epsilon( ) )
//    {
//        isOrbitalElementConversionErroneous = true;

//        cerr << "The conversion of elapsed time to mean anomaly is erroneous "
//             << "as the computed mean anomaly after applying the conversion ( "
//             << unit_conversions::convertRadiansToDegrees( meanAnomaly )
//             << " ) does not match the expected value of the mean anomaly ( "
//             << unit_conversions::convertRadiansToDegrees( expectedMeanAnomalyForTest10 )
//             << " ) " << endl;
//    }

//    // Test 11: Test of mean anomaly to elapsed time for elliptical orbits.
//    //          Reversal of computation for Test 10.

//    // Set tolerance for conversion.
//    toleranceOrbitalElementConversion = 1e-11;

//    // Set mean anomaly.
//    meanAnomaly = expectedMeanAnomalyForTest10;

//    // Declare and compute elapsed time.
//    double elapsedTime = orbital_element_conversions::
//            convertMeanAnomalyToElapsedTimeForEllipticalOrbits(
//                meanAnomaly, &predefinedEarth, semiMajorAxis );

//    // Compute absolute and relative errors.
//    absoluteDifference = fabs( elapsedTime - expectedElapsedTime );

//    relativeDifference = absoluteDifference / expectedElapsedTime;

//    // Check if computed elapsed time is equal to reference value.
//    if ( relativeDifference > toleranceOrbitalElementConversion )
//    {
//        isOrbitalElementConversionErroneous = true;

//        cerr << "The conversion of mean anomaly to elapsed time is erroneous "
//             << "as the computed elapsed time after applying the conversion ( " << elapsedTime
//             << " ) does not match the expected value of the elapsed time ( "
//             << expectedElapsedTime << " ) " << endl;
//    }

//    // Test 12: Test of elapsed time to mean anomaly for hyperbolic orbits.

//    // Set tolerance for conversion.
//    toleranceOrbitalElementConversion = 1e-11;

//    // Set elapsed time.
//    elapsedTime = 1000.0;

//    // Set semi-major axis.
//    semiMajorAxis = unit_conversions::convertKilometersToMeters( -40000.0 );

//    // Compute mean anomaly.
//    meanAnomaly = orbital_element_conversions::
//            convertElapsedTimeToMeanAnomalyForHyperbolicOrbits(
//                elapsedTime, &predefinedEarth, semiMajorAxis );

//    // Check if computed mean anomaly is equal to reference value.
//    if ( fabs( meanAnomaly - 0.078918514324112 ) > toleranceOrbitalElementConversion )
//    {
//        isOrbitalElementConversionErroneous = true;

//        cerr << "The conversion of elapsed time to mean anomaly is erroneous "
//             << "as the computed mean anomaly after applying the conversion ( "
//             << unit_conversions::convertRadiansToDegrees( meanAnomaly )
//             << " ) does not match the expected value of the mean anomaly ( "
//             << unit_conversions::convertRadiansToDegrees( 0.078918514324112 ) << " ) " << endl;
//    }

//    // Test 13: Test of mean anomaly to elapsed time for hyperbolic orbits.

//    // Set tolerance for conversion.
//    toleranceOrbitalElementConversion = 1e-11;

//    // Set mean anomaly.
//    meanAnomaly = 0.078918514324112;

//    // Set semi-major axis.
//    semiMajorAxis = unit_conversions::convertKilometersToMeters( -40000.0 );

//    // Compute elapsed time.
//    elapsedTime = orbital_element_conversions::
//            convertMeanAnomalyToElapsedTimeForHyperbolicOrbits(
//                meanAnomaly, &predefinedEarth, semiMajorAxis );

//    // Check if computed elapsed time is equal to reference value.
//    if ( fabs( elapsedTime - 1000.0 ) > toleranceOrbitalElementConversion )
//    {
//        isOrbitalElementConversionErroneous = true;

//        cerr << "The conversion of mean anomaly to elapsed time is erroneous "
//             << "as the computed elapsed time after applying the conversion ( " << elapsedTime
//             << " ) does not match the expected value of the elapsed time ( "
//             << 1000.0 << " ) " << endl;
//    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    if ( isOrbitalElementConversionErroneous )
    {
        cerr << "testOrbitalElementConversions failed!" << endl;
    }

    return isOrbitalElementConversionErroneous;
}

// End of file.
