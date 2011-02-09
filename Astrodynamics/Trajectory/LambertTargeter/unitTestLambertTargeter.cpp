/*! \file unitTestLambertTargeter.cpp
 *    Source file of unit test file of Lambert targeting algorithm code.
 *    This unit test file will test the Lambert targeting algorithm code for
 *    both the hyperbolic case and the elliptical case.
 *
 *    Path              : /Astrodynamics/Trajectory/LambertTargeter/
 *    Version           : 6
 *    Check status      : Checked
 *
 *    Author            : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 13 January, 2011
 *    Last modified     : 08 February, 2011
 *
 *    References        : - Hyperbolic case: [1] (tudat.tudelf.nl\external_files ->
 *                        this is just a guess of where the file could be stored).
 *                        TEMPORARY!!! Ron's Excel file!!!
 *                        - Elliptical case: [2] "Fondamenti di Meccanica del volo
 *                        Spaziale", G. Mengali, A. A. Quarta, example 6.1
 *                        page 159-162.
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
 *      YYMMDD    author          comment
 *      110113    E. Iorfida      First creation of the code.
 *      110126    E. Iorfida      Added test for velocities.
 *      110130    J. Melman       Simplified variable names, e.g.,
 *                                'normOfVelocityVector' became 'speed'.
 *                                Also corrected 'tangential' to 'transverse'.
 *                                Suggested less trivial test case.
 *      110201    E. Iorfida      Added pointerToCelestialBody and modified
 *                                variable names (from heliocentric, to inertial).
 *                                Added elliptical case and check for the
 *                                anti-clockwise direction of motio.
 *      110207    E. Iorfida      Added a more stric value for tolerance of
 *                                semi-major axis for hyperbolic case.
 *      110208    E. Iorfida      Added CartesianPositionElements objects as input
 *                                and CartesianVelocityElements objects as output.
 */

// Include statements.
# include "unitTestLambertTargeter.h"
# include "unitConversions.h"
# include "predefinedPlanets.h"

// Using directives.
using mathematics::computeAbsoluteValue;
using predefined_planets::createPredefinedPlanet;
using unit_conversions::convertAstronomicalUnitsToMeters;
using std::endl;
using std::cerr;

//! Namespace for all unit tests.
namespace unit_tests
{

//! Test of Lambert targeting algorithm code.
bool testLambertTargeter( )
{
    // Test result initialised to false.
    bool isLambertTargeterErroneous = false;

    // Expected test result in meters.
    // Hyperbolic test case (results from excel file [1]).
    double expectedValueOfSemiMajorAxisHyperbola = -1270129.3602e3;
    double expectedValueOfRadialSpeedAtDepartureHyperbola = -0.74546e3;
    double expectedValueOfRadialSpeedAtArrivalHyperbola = 0.69321e3;
    double expectedValueOfTransverseSpeedAtDepartureHyperbola = 0.15674e3;
    double expectedValueOfTransverseSpeedAtArrivalHyperbola = 0.10450e3;

    // Elliptical test case (results from example 6.1 page 159-162 [2]).
    // Set canonical units for Earth (see page 29 [2]).
    double distanceUnit_ = 6.378136e6;
    double timeUnit_ = 806.78;

    double expectedValueOfSemiMajorAxisEllipse = 5.4214 * distanceUnit_;
    double expectedValueOfRadialSpeedAtDepartureEllipse = 2.73580e3;
    double expectedValueOfRadialSpeedAtArrivalEllipse = 2.97503e3;
    double expectedValueOfTransverseSpeedAtDepartureEllipse = 6.59430e3;
    double expectedValueOfTransverseSpeedAtArrivalEllipse = 3.29715e3;

    // Tolerance in absolute units.
    double toleranceSemiMajorAxisHyperbola_ = 1.0e2;
    double toleranceSemiMajorAxisEllipse_ = 1.0e4;
    double toleranceVelocity_ = 1.0e-02;

    // Time conversions.
    double timeOfFlightInDaysHyperbola = 100.0;
    double timeOfFlightHyperbola = 24.0 * 60.0 * 60.0 *
                                   timeOfFlightInDaysHyperbola;
    double timeOfFlightEllipse = 5.0 * timeUnit_;

    // Central bodies parameters.
    CelestialBody* pointerToEarth = new CelestialBody;
    pointerToEarth = createPredefinedPlanet( predefined_planets::earth );

    // Compute Lambert targeting algorithm.

    // Hyperbolic orbit case.
    LambertTargeter lambertTargeterHyperbola_;

    // The starting point is twice as far as L1 and L2, which is not really
    // realistic, but it is not about the case, but about the verification.
    CartesianPositionElements* pointerToPositionAtDepartureHyperbola =
            new CartesianPositionElements;
    pointerToPositionAtDepartureHyperbola->setCartesianElementX(
            convertAstronomicalUnitsToMeters( 0.02 ) );
    pointerToPositionAtDepartureHyperbola->setCartesianElementY( 0.0 );
    pointerToPositionAtDepartureHyperbola->setCartesianElementZ( 0.0 );

    CartesianPositionElements* pointerToPositionAtArrivalHyperbola =
            new CartesianPositionElements;
    pointerToPositionAtArrivalHyperbola->setCartesianElementX( 0.0 );
    pointerToPositionAtArrivalHyperbola->setCartesianElementY(
            convertAstronomicalUnitsToMeters( -0.03 ) );
    pointerToPositionAtArrivalHyperbola->setCartesianElementZ( 0.0 );

    lambertTargeterHyperbola_.setPositionAtDeparture(
            pointerToPositionAtDepartureHyperbola );
    lambertTargeterHyperbola_.setPositionAtArrival(
            pointerToPositionAtArrivalHyperbola );
    lambertTargeterHyperbola_.setNumberOfRevolutions( 0 );
    lambertTargeterHyperbola_.setTimeOfFlight(
            timeOfFlightHyperbola );
    lambertTargeterHyperbola_.setCentralBody( pointerToEarth );

    // Create pointers to new NewtonRaphson object.
    NewtonRaphson* pointerToNewtonRaphsonLambertHyperbola = new NewtonRaphson;
    lambertTargeterHyperbola_.setNewtonRaphsonMethod(
            pointerToNewtonRaphsonLambertHyperbola );

    // Elliptical orbit case.
    LambertTargeter lambertTargeterEllipse_;

    CartesianPositionElements* pointerToPositionAtDepartureEllipse =
            new CartesianPositionElements;
    pointerToPositionAtDepartureEllipse->setCartesianElementX(
            2.0 * distanceUnit_ );
    pointerToPositionAtDepartureEllipse->setCartesianElementY( 0.0 );
    pointerToPositionAtDepartureEllipse->setCartesianElementZ( 0.0 );

    CartesianPositionElements* pointerToPositionAtArrivalEllipse =
            new CartesianPositionElements;
    pointerToPositionAtArrivalEllipse->setCartesianElementX(
            2.0 * distanceUnit_ );
    pointerToPositionAtArrivalEllipse->setCartesianElementY(
            2.0 * sqrt( 3.0 ) * distanceUnit_ );
    pointerToPositionAtArrivalEllipse->setCartesianElementZ( 0.0 );

    lambertTargeterEllipse_.setPositionAtDeparture(
            pointerToPositionAtDepartureEllipse );
    lambertTargeterEllipse_.setPositionAtArrival(
            pointerToPositionAtArrivalEllipse );
    lambertTargeterEllipse_.setNumberOfRevolutions( 0 );
    lambertTargeterEllipse_.setTimeOfFlight( timeOfFlightEllipse );
    lambertTargeterEllipse_.setCentralBody( pointerToEarth );

    // Create pointers to new NewtonRaphson object.
    NewtonRaphson* pointerToNewtonRaphsonLambertEllipse = new NewtonRaphson;
    lambertTargeterEllipse_.setNewtonRaphsonMethod(
            pointerToNewtonRaphsonLambertEllipse );

    // Compute Lambert targeting algorithms.
    lambertTargeterHyperbola_.execute( );
    lambertTargeterEllipse_.execute( );

    // Create local vectors for position and velocity.
    Vector3d positionDepartureHyperbola_ =
            pointerToPositionAtDepartureHyperbola->state;

    Vector3d velocityDepartureHyperbola_ =
            lambertTargeterHyperbola_.getInertialVelocityAtDeparture( )->state;

    Vector3d positionDepartureEllipse_ =
            pointerToPositionAtDepartureEllipse->state;

    Vector3d velocityDepartureEllipse_ =
            lambertTargeterEllipse_.getInertialVelocityAtDeparture( )->state;

    // Set test result to true if the test does not match the expected result.
    if ( ( computeAbsoluteValue(
            lambertTargeterHyperbola_.getLambertSemiMajorAxis( ) -
            expectedValueOfSemiMajorAxisHyperbola ) >=
           toleranceSemiMajorAxisHyperbola_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterHyperbola_.getRadialSpeedAtDeparture( ) -
            expectedValueOfRadialSpeedAtDepartureHyperbola ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterHyperbola_.getRadialSpeedAtArrival( )-
            expectedValueOfRadialSpeedAtArrivalHyperbola ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterHyperbola_.getTransverseSpeedAtDeparture( ) -
            expectedValueOfTransverseSpeedAtDepartureHyperbola ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterHyperbola_.getTransverseSpeedAtArrival( ) -
            expectedValueOfTransverseSpeedAtArrivalHyperbola ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterEllipse_.getLambertSemiMajorAxis( ) -
            expectedValueOfSemiMajorAxisEllipse ) >=
           toleranceSemiMajorAxisEllipse_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterEllipse_.getRadialSpeedAtDeparture( ) -
            expectedValueOfRadialSpeedAtDepartureEllipse ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterEllipse_.getRadialSpeedAtArrival( ) -
            expectedValueOfRadialSpeedAtArrivalEllipse ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterEllipse_.getTransverseSpeedAtDeparture( ) -
            expectedValueOfTransverseSpeedAtDepartureEllipse ) >=
           toleranceVelocity_ ) ||
         ( computeAbsoluteValue(
            lambertTargeterEllipse_.getTransverseSpeedAtArrival( ) -
            expectedValueOfTransverseSpeedAtArrivalEllipse ) >=
           toleranceVelocity_ ) ||

         // Check anti-clockwise direction of the computed orbit.
         ( positionDepartureHyperbola_.cross(
                 velocityDepartureHyperbola_ ).z( ) < 0.0 ) ||
         ( positionDepartureEllipse_.cross(
                 velocityDepartureEllipse_).z( ) < 0.0 ) )
    {
        // Set error flag to true.
        isLambertTargeterErroneous = true;

        if ( computeAbsoluteValue(
                lambertTargeterHyperbola_.getLambertSemiMajorAxis( ) -
                expectedValueOfSemiMajorAxisHyperbola ) >=
                toleranceSemiMajorAxisHyperbola_ )
        {
            // Generate error statements.
            cerr << "The computed value of the semi-major axis ( "
                 << lambertTargeterHyperbola_.
                    getLambertSemiMajorAxis( )
                 << " ) using the Lambert targeting algorithm "
                 << "does not match the expected solution ( "
                 << expectedValueOfSemiMajorAxisHyperbola << " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue( lambertTargeterHyperbola_.
                    getLambertSemiMajorAxis( ) -
                    expectedValueOfSemiMajorAxisHyperbola ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterHyperbola_.
             getRadialSpeedAtDeparture( ) -
             expectedValueOfRadialSpeedAtDepartureHyperbola ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << " The computed value of the radial speed at departure ( "
                 << lambertTargeterHyperbola_.
                    getRadialSpeedAtDeparture( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfRadialSpeedAtDepartureHyperbola <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterHyperbola_.
                    getRadialSpeedAtDeparture( ) -
                    expectedValueOfRadialSpeedAtDepartureHyperbola ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterHyperbola_.
             getRadialSpeedAtArrival( ) -
             expectedValueOfRadialSpeedAtArrivalHyperbola ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the radial speed at arrival ( "
                 << lambertTargeterHyperbola_.
                    getRadialSpeedAtArrival( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfRadialSpeedAtArrivalHyperbola <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterHyperbola_.
                    getRadialSpeedAtArrival( ) -
                    expectedValueOfRadialSpeedAtArrivalHyperbola ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterHyperbola_.
             getTransverseSpeedAtDeparture( ) -
             expectedValueOfTransverseSpeedAtDepartureHyperbola ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the tangential speed at departure ( "
                 << lambertTargeterHyperbola_.
                    getTransverseSpeedAtDeparture( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfTransverseSpeedAtDepartureHyperbola <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterHyperbola_.
                    getTransverseSpeedAtDeparture( ) -
                    expectedValueOfTransverseSpeedAtDepartureHyperbola ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterHyperbola_.
             getTransverseSpeedAtArrival( ) -
             expectedValueOfTransverseSpeedAtArrivalHyperbola ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the radial speed at arrival ( "
                 << lambertTargeterHyperbola_.
                    getTransverseSpeedAtArrival( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfTransverseSpeedAtArrivalHyperbola <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterHyperbola_.
                    getTransverseSpeedAtArrival( ) -
                    expectedValueOfTransverseSpeedAtArrivalHyperbola ) << endl;
        }

        if ( computeAbsoluteValue(
                lambertTargeterEllipse_.getLambertSemiMajorAxis( ) -
                expectedValueOfSemiMajorAxisEllipse ) >=
                toleranceSemiMajorAxisEllipse_ )
        {
            // Generate error statements.
            cerr << "The computed value of the semi-major axis ( "
                 << lambertTargeterEllipse_.
                    getLambertSemiMajorAxis( )
                 << " ) using the Lambert targeting algorithm "
                 << "does not match the expected solution ( "
                 << expectedValueOfSemiMajorAxisEllipse << " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue( lambertTargeterEllipse_.
                    getLambertSemiMajorAxis( ) -
                    expectedValueOfSemiMajorAxisEllipse ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterEllipse_.
             getRadialSpeedAtDeparture( ) -
             expectedValueOfRadialSpeedAtDepartureEllipse ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << " The computed value of the radial speed at departure ( "
                 << lambertTargeterEllipse_.
                    getRadialSpeedAtDeparture( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfRadialSpeedAtDepartureEllipse <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterEllipse_.
                    getRadialSpeedAtDeparture( ) -
                    expectedValueOfRadialSpeedAtDepartureEllipse ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterEllipse_.
             getRadialSpeedAtArrival( ) -
             expectedValueOfRadialSpeedAtArrivalEllipse ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the radial speed at arrival ( "
                 << lambertTargeterEllipse_.
                    getRadialSpeedAtArrival( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfRadialSpeedAtArrivalEllipse <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterEllipse_.
                    getRadialSpeedAtArrival( ) -
                    expectedValueOfRadialSpeedAtArrivalEllipse ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterEllipse_.
             getTransverseSpeedAtDeparture( ) -
             expectedValueOfTransverseSpeedAtDepartureEllipse ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the tangential speed at departure ( "
                 << lambertTargeterEllipse_.
                    getTransverseSpeedAtDeparture( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfTransverseSpeedAtDepartureEllipse <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterEllipse_.
                    getTransverseSpeedAtDeparture( ) -
                    expectedValueOfTransverseSpeedAtDepartureEllipse ) << endl;
        }

        if ( computeAbsoluteValue( lambertTargeterEllipse_.
             getTransverseSpeedAtArrival( ) -
             expectedValueOfTransverseSpeedAtArrivalEllipse ) >=
             toleranceVelocity_ )
        {
            // Generate error statements.
            cerr << "The computed value of the radial speed at arrival ( "
                 << lambertTargeterEllipse_.
                    getTransverseSpeedAtArrival( ) << " ) using "
                    " the Lambert targeting algorithm does not match the"
                    " expected solution (" <<
                    expectedValueOfTransverseSpeedAtArrivalEllipse <<
                    " )." << endl;
            cerr << "The error is: "
                 << computeAbsoluteValue(
                    lambertTargeterEllipse_.
                    getTransverseSpeedAtArrival( ) -
                    expectedValueOfTransverseSpeedAtArrivalEllipse ) << endl;
        }

        if ( positionDepartureHyperbola_.cross(
                velocityDepartureHyperbola_ ).z( ) < 0.0 )

        {
            // Generate error statements.
            cerr << "The computed hyperbolic orbit path does not follow "
                    "the standard anti-clockwise direction." << endl;
        }

        if ( positionDepartureEllipse_.cross(
                velocityDepartureEllipse_ ).z( ) < 0.0 )
        {
            // Generate error statements.
            cerr << "The computed elliptical orbit path does not follow "
                    "the standard anti-clockwise direction." << endl;
        }

    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    return isLambertTargeterErroneous;

}
}

// End of file.
