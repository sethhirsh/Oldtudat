/*! \file unitTestUnitConversions.cpp
 *    This unit test will test the unit conversions that are
 *    defined in unitConversions.h.
 *
 *    Path              : /Astrodynamics/
 *    Version           : 3
 *    Check status      : Checked
 *
 *    Author            : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Author            : F. M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : D.Dirkx@student.tudelft.nl
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 10 September, 2010
 *    Last modified     : 15 June, 2011
 *
 *    References
 *
 *    Notes
 *      Test runs code and verifies result against expected value.
 *      If the tested code is erroneous, the test function returns a boolean
 *      true; if the code is correct, the function returns a boolean false.
 *
 *    Copyright (c) 2010 Delft University of Technology.
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
 *      110111    J. Melman         First creation of code.
 *      110124    J. Melman         Adapted to the offical Tudat standards.
 *      110411    K. Kumar          Added unit tests for
 *                                  convertDegreesToArcminutes() and
 *                                  convertArcminutesToArcseconds().
 *      110615    F.M. Engelen      Added Rankine, feet, and pound/m^2 conversion. Solved bug
 *                                  with respect to absolute precision and relative precision.
 */

// Include statements.
#include "unitTestUnitConversions.h"

// Using directives.
using std::cerr;
using std::endl;
using mathematics::computeAbsoluteValue;
using mathematics::MACHINE_PRECISION_DOUBLES;

//! Namespace for all unit tests.
namespace unit_tests
{

//! Test unit conversions.
bool testUnitConversions( )
{
    // Test result initialised to false.
    bool isUnitConversionsErroneous = false;

    // Test conversion from kilometers to meters.
    if ( computeAbsoluteValue(
            unit_conversions::convertKilometersToMeters( 1.0e6 ) -
            1.0e6 * 1.0e3 ) / 1.0e9  > MACHINE_PRECISION_DOUBLES )
    {
        cerr << "The conversion from kilometers to meters does not "
             << "function correctly." << endl;
        isUnitConversionsErroneous = true;
    }

    // Test conversion from degrees to radians.
    // The tested constants are only given to the 8th significant number.
    if ( computeAbsoluteValue(
            unit_conversions::convertDegreesToRadians(
                    PhysicalConstants::OBLIQUITY_ECLIPTIC_IN_DEGREES ) -
            PhysicalConstants::OBLIQUITY_ECLIPTIC )
        > 1.0e-8 )
        {
        cerr << "The conversion from degrees to radians does not "
             << "function correctly." << endl;
        isUnitConversionsErroneous = true;
    }

    // Test conversion from degrees to arcminutes.
    if ( computeAbsoluteValue(
            unit_conversions::convertDegreesToArcminutes( 43.2 ) -
            43.2 * 60.0 ) / ( 43.2 * 60.0 )  > MACHINE_PRECISION_DOUBLES )
    {
        cerr << "The conversion from degrees to arcminutes does not "
                << "function correctly." << endl;
        cerr << "The calculated value is: " << unit_conversions::convertDegreesToArcminutes( 43.2 );
        cerr << " with a precision of "
                << unit_conversions::convertDegreesToArcminutes( 43.2 ) - 43.2 * 60.0 << endl;
        isUnitConversionsErroneous = true;
    }

    // Test conversion from arcminutes to arcseconds.
    if ( computeAbsoluteValue(
            unit_conversions::convertArcminutesToArcseconds( 125.9 ) -
            125.9 * 60.0 ) / ( 125.9 * 60.0 )  > MACHINE_PRECISION_DOUBLES )
    {
        cerr << "The conversion from arcminutes to arcseconds does not "
             << "function correctly." << endl;
        isUnitConversionsErroneous = true;
    }

    // Test conversion from astronomical units to meters.
    // Case of Neptune's semi-major axis used (source: Wikipedia).
    if ( computeAbsoluteValue(
            unit_conversions::convertAstronomicalUnitsToMeters(
                    30.10366151 ) - 4.503443661e+12 )
        > 1.0e3 )
        {
        cerr << "The conversion from astronomical units to meters does not "
             << "function correctly." << endl;

        isUnitConversionsErroneous = true;
    }

    // Test conversion from temperature in Rankine to Kelvin.
    // meltingtemperature ice (source wikipedia).
    if ( computeAbsoluteValue(
            unit_conversions::convertRankineToKelvin( 491.67 ) - 273.15 ) / 273.15
         > MACHINE_PRECISION_DOUBLES )
    {
        cerr << "The conversion from temperature in Rankine to Kelvin does not "
             << "function correctly." << endl;

        isUnitConversionsErroneous = true;
    }

    // Test conversion from distance in feet to meters.
    // case: lenght of a statute mile (source wikipedia).
    if ( computeAbsoluteValue(
            unit_conversions::convertFeetToMeter( 5280.0 ) - 1609.344 ) / 1609.344
         > MACHINE_PRECISION_DOUBLES )
    {
        cerr << "The conversion from distance in feet to meters does not "
             << "function correctly." << endl;

        isUnitConversionsErroneous = true;
    }

    // Test conversion from pressure in pound per square feet to Pascal.
    // case: atmospheric pressure at sea level. (source wikipedia).
    if ( computeAbsoluteValue(
            unit_conversions::convertPoundPerSquareFeetToPascal( 2116.21662367394 ) - 101325.0 )
        > 1.0e-4 )
        {
        cerr << "The conversion from pressure in pound per square feet to Pascal does not "
             << "function correctly." << endl;
        isUnitConversionsErroneous = true;
    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    return isUnitConversionsErroneous;
}
}


// End of file.
