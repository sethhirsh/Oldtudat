/*    Copyright (c) 2010-2012, Delft University of Technology
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
 *      110117    E. Iorfida        First creation of the code.
 *      110202    J. Melman         Some small changes to layout and
 *                                  added some notes and comments.
 *      110203    E. Iorfida        Changed the code respect to the
 *                                  modification inside the main file.
 *      110208    E. Iorfida        Update file with CartesianVelocityElements
 *                                  pointers as input.
 *      110212    J. Melman         Added comments to clarify test case.
 *      110512    K. Kumar          Updated code to not use dynamic memory
 *                                  allocation and new
 *                                  createPredefinedCelestialBody( ) function.
 *      110627    K. Kumar          Updated to use new predefined planets code.
 *      120326    D. Dirkx          Changed raw pointers to shared pointers.
 *      120416    T. Secretin       Boostified unit test.
 *      120508    P. Musegaas       Update file with gravitational parameter now as double input.
 *
 *    References
 *
 *    A test should be added together with results from the Lambert targeter and the Ephemeris
 *    class. The expected result for the current test should be calculated inside this code, not
 *    outside by a calculator.
 *
 */

#define BOOST_TEST_MAIN

#include <cmath>

#include <boost/make_shared.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <Eigen/Core>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/Bodies/planet.h"
#include "Tudat/Astrodynamics/MissionSegments/gravityAssist.h"

namespace tudat
{
namespace unit_tests
{

//! Test of gravity assist code.
BOOST_AUTO_TEST_SUITE( test_gravity_assist )

//! Test Delta-V computation.
BOOST_AUTO_TEST_CASE( testDeltaV )
{
    // Tolerances.
    // Expected velocity output is defined with an accuracy of 1 m/s.
    const double velocityTolerance = 1.0;

    // In the first test case, the incoming and outgoing inertial
    // velocities are defined such that the hyperbolic excess velocities
    // are equal. In that way, a delta-V is only needed to rotate the
    // velocity vector, which has been calculated by hand.
    // Expected delta-V for a powered swing-by around Mars.
    const double expectedDeltaV = 3.652e3;

    // Define body that is swung by.
    bodies::Planet predefinedMars;
    predefinedMars.setPredefinedPlanetSettings( bodies::Planet::mars );

    // Define Sun gravitational parameter.
    const double gravitationalParameterSun = 1.32712440018e20;

    // Define planet-Sun distance.
    const double distanceMarsToSun = unit_conversions::convertAstronomicalUnitsToMeters( 1.5 );

    // Define smallest periapsis distance factor.
    const double marsSmallestPeriapsisDistanceFactor = 1.076;

    // Define planet heliocentric velocity vector.
    // The orbit is considered to be circular.
    Eigen::Vector3d marsVelocity( 0.0,
                                  sqrt( gravitationalParameterSun / distanceMarsToSun ),
                                  0.0 );

    // Define satellite incoming vector.
    using tudat::mathematics::PI;
    Eigen::Vector3d incomingVelocityTest( -25.0e3 * std::sin( PI / 6.0 ),
                                          25.0e3 * std::cos( PI / 6.0 ),
                                          0.0 );

    // Define satellite outgoing vector.
    Eigen::Vector3d outgoingVelocityTest( incomingVelocityTest( 0 ),
                                          2.0 * marsVelocity( 1 )- incomingVelocityTest( 1 ),
                                          0.0 );

    // Declare GravityAssist object.
    using astrodynamics::mission_segments::GravityAssist;
    GravityAssist myGravityAssist(
                predefinedMars.getGravityFieldModel( )->getGravitationalParameter( ),
                3398.0e3 * marsSmallestPeriapsisDistanceFactor,
                marsVelocity,
                incomingVelocityTest,
                outgoingVelocityTest,
                boost::make_shared< NewtonRaphson >( ) );

    // Compute powered gravity-assist implementation.
    const double deltaV = myGravityAssist.computeDeltaV( );

    // Test if the computed delta-V corresponds to the expected value within the specified
    // tolerance.
    BOOST_CHECK_CLOSE_FRACTION( deltaV, expectedDeltaV, velocityTolerance );
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat

