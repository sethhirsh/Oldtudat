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
 *      120530    P. Musegaas       Very Big update. Updated old unit test to work with new gravity
 *                                  assist function. Deleted references to old planet functions.
 *                                  Added new unit test to test velocity effect deltaV. Created
 *                                  three tests for new gravity assist propagation methods.
 *
 *    References
 *      Izzo, D. and Vinko, T. ACT - Informatics - GTOP Database, ESA Advanced Concept Team, last
 *          accessed on 2012-01-12. http://www.esa.int/gsp/ACT/inf/op/globopt.htm.
 *
 *    Notes
 *      Three main functions are tested in these unit tests.
 *        Regarding the deltaV calculation gravity assist method:
 *          Two tests are written. One tests the bending effect deltaV calculation, the other the
 *          velocity effect deltaV calculation. It would be nice to have a benchmark with both in
 *          one test. This is however difficult to calculate using other methods. Also the bending
 *          effect deltaV calculation has a low accuracy, as it relies on hand calculator
 *          calculations done in 2011.
 *        Regarding the unpowered gravity assist propagator:
 *          One test was written, based on GTOP. This should be a satisfactory test.
 *        Regarding the powered gravity assist propagator:
 *          Two tests were written. The first one is similar to the unpowered gravity assist
 *          propagator. The second one is reverse engineered from the Cassini-1 test, similar to
 *          the one in the deltaV calculation test.
 *
 */

#define BOOST_TEST_MAIN

#include <cmath>

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include <TudatCore/Basics/testMacros.h>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/MissionSegments/gravityAssist.h"

namespace tudat
{
namespace unit_tests
{

//! Test of gravity assist code.
BOOST_AUTO_TEST_SUITE( test_gravity_assist )

//! Test bending angle Delta-V computation.
BOOST_AUTO_TEST_CASE( testBendingAngleDeltaV )
{
    // Tolerance, determined primarily by the accuracy of the hand calculations for this test case.
    const double velocityTolerance = 0.0002;

    // In the first test case, the incoming and outgoing inertial velocities are defined such that
    // the hyperbolic excess velocities are equal. In that way, a delta-V is only needed to rotate
    // the velocity vector, which has been calculated by hand.
    // Expected delta-V for a powered swing-by around Mars.
    const double expectedDeltaV = 3.652e3;

    // Define swingby body gravitational parameter.
    const double marsGravitationalParameter = 4.2828018915e13;

    // Define Sun gravitational parameter.
    const double gravitationalParameterSun = 1.32712440018e20;

    // Define planet-Sun distance.
    const double distanceMarsToSun = unit_conversions::convertAstronomicalUnitsToMeters( 1.5 );

    // Define smallest periapsis distance factor.
    const double marsSmallestPeriapsisDistance = 3656248.0;

    // Define planet heliocentric velocity vector. The orbit is considered to be circular.
    const Eigen::Vector3d marsVelocity( 0.0,
                                        std::sqrt( gravitationalParameterSun / distanceMarsToSun ),
                                        0.0 );

    // Define satellite incoming vector.
    using tudat::mathematics::PI;
    const Eigen::Vector3d incomingVelocity( -25.0e3 * std::sin( PI / 6.0 ),
                                            25.0e3 * std::cos( PI / 6.0 ),
                                            0.0 );

    // Define satellite outgoing vector.
    const Eigen::Vector3d outgoingVelocity( incomingVelocity( 0 ),
                                            2.0 * marsVelocity( 1 ) - incomingVelocity( 1 ),
                                            0.0 );

    // Perform the gravity assist.
    const double deltaV = mission_segments::gravityAssist( marsGravitationalParameter,
                                                           marsVelocity, incomingVelocity,
                                                           outgoingVelocity,
                                                           marsSmallestPeriapsisDistance );

    // Test if the computed delta-V corresponds to the expected value within the specified
    // tolerance.
    BOOST_CHECK_CLOSE_FRACTION( deltaV, expectedDeltaV, velocityTolerance );
}

//! Test velocity effect Delta-V computation.
BOOST_AUTO_TEST_CASE( testVelocityEffectDeltaV )
{
    // Tolerance. Benchmark obtained directly from the GTOP code, based on the first swing-by for
    // the ideal Cassini-1 trajectory. Values were obtained with a 15-digit accuracy from GTOP,
    // resulting in an accuracy of 6e-14 in the final results.
    const double tolerance = 1.0e-13;

    // Expected deltaV cost, as obtained from GTOP code.
    const double expectedDeltaV = 1090.64622870007;

    // Define swingby body gravitational parameter.
    const double venusGravitationalParameter = 3.24860e14;

    // Define smallest periapsis distance factor.
    const double venusSmallestPeriapsisDistance = 6351800.0;

    // Define heliocentric planet velocity vector.
    const Eigen::Vector3d venusVelocity( 32851.224953746, -11618.7310059974, -2055.04615890989 );

    // Define heliocentric satellite incoming vector.
    const Eigen::Vector3d incomingVelocity( 34216.4827530912, -15170.1440677825,
                                            395.792122152361 );

    // Define heliocentric satellite outgoing vector.
    const Eigen::Vector3d outgoingVelocity( 37954.2431376052, -14093.0467234774,
                                            -5753.53728279429 );

    // Perform the gravity assist.
    const double deltaV = mission_segments::gravityAssist( venusGravitationalParameter,
                                                           venusVelocity,incomingVelocity,
                                                           outgoingVelocity,
                                                           venusSmallestPeriapsisDistance );

    // Test if the computed delta-V corresponds to the expected value within the specified
    // tolerance.
    BOOST_CHECK_CLOSE_FRACTION( deltaV, expectedDeltaV, tolerance );
}

//! Test unpowered gravity assist propagation.
BOOST_AUTO_TEST_CASE( testUnpoweredGravityAssistPropagation )
{
    // Tolerance. Benchmark obtained directly from the GTOP code, based on the first swing-by for
    // the ideal Messenger trajectory. Values were obtained with a 15-digit accuracy from GTOP,
    // resulting in an accuracy of 2e-14 in the final results.
    const double tolerance = 1.0e-13;

    // Expected deltaV cost, as obtained from GTOP code.
    const Eigen::Vector3d expectedOutgoingVelocity( 12868.5248737923, -22821.444560174,
                                                    -775.698475033994 );

    // Define swingby body gravitational parameter.
    const double earthGravitationalParameter = 3.9860119e14;

    // Define heliocentric planet velocity vector.
    const Eigen::Vector3d earthVelocity( 15025.522196446, -25544.3782752036, 0.0 );

    // Define heliocentric satellite incoming vector.
    const Eigen::Vector3d incomingVelocity( 17969.3166254716, -23543.691593914, 6.38384671663496 );

    // Define rotation angle.
    const double rotationAngle = 1.35077257078;

    // Define pericenter radius.
    const double pericenterRadius = 1.80629232251 * 6378000.0;

    // Perform the gravity assist.
    const Eigen::Vector3d outgoingVelocity = mission_segments::gravityAssist(
                                                    earthGravitationalParameter, earthVelocity,
                                                    incomingVelocity, rotationAngle,
                                                    pericenterRadius );

    // Test if the computed outgoing velocity corresponds to the expected velocity within the
    // specified tolerance.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedOutgoingVelocity, outgoingVelocity, tolerance );
}

//! Test powered gravity assist propagation function for unpowered gravity assist.
BOOST_AUTO_TEST_CASE( testPoweredGravityAssistPropagationForUnpoweredGravityAssist )
{
    // Tolerance. Benchmark obtained directly from the GTOP code, based on the first swing-by for
    // the ideal Messenger trajectory. Values were obtained with a 15-digit accuracy from GTOP,
    // resulting in an accuracy of 2e-14 in the final results.
    const double tolerance = 1.0e-13;

    // Expected deltaV cost, as obtained from GTOP code.
    const Eigen::Vector3d expectedOutgoingVelocity( 12868.5248737923, -22821.444560174,
                                                    -775.698475033994 );

    // Define swingby body gravitational parameter.
    const double earthGravitationalParameter = 3.9860119e14;

    // Define heliocentric planet velocity vector.
    const Eigen::Vector3d earthVelocity( 15025.522196446, -25544.3782752036, 0.0 );

    // Define heliocentric satellite incoming vector.
    const Eigen::Vector3d incomingVelocity( 17969.3166254716, -23543.691593914, 6.38384671663496 );

    // Define rotation angle.
    const double rotationAngle = 1.35077257078;

    // Define pericenter radius.
    const double pericenterRadius = 1.80629232251 * 6378000.0;

    // Define deltaV.
    const double deltaV = 0.0;

    // Perform the gravity assist.
    const Eigen::Vector3d outgoingVelocity = mission_segments::gravityAssist(
                                                    earthGravitationalParameter, earthVelocity,
                                                    incomingVelocity, rotationAngle,
                                                    pericenterRadius, deltaV );

    // Test if the computed outgoing velocity corresponds to the expected velocity within the
    // specified tolerance.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedOutgoingVelocity, outgoingVelocity, tolerance );
}

//! Test powered gravity assist propagation function for reverse engineered test case.
BOOST_AUTO_TEST_CASE( testPoweredGravityAssistPropagationReverseEngineered )
{
    // Tolerance. Benchmark obtained by reverse engineering the GTOP code, based on the first
    // swing-by for the ideal Cassini-1 trajectory. Values were obtained with a 15-digit accuracy
    // from GTOP, resulting in an accuracy of 1e-14 in the final results.
    const double tolerance = 1.0e-14;

    // Expected deltaV cost, as obtained from GTOP code.
    const Eigen::Vector3d expectedOutgoingVelocity( 37954.2431376052, -14093.0467234774,
                                                    -5753.53728279429 );

    // Define swingby body gravitational parameter.
    const double venusGravitationalParameter = 3.24860e14;

    // Define heliocentric planet velocity vector.
    const Eigen::Vector3d venusVelocity( 32851.224953746, -11618.7310059974, -2055.04615890989 );

    // Define heliocentric satellite incoming vector.
    const Eigen::Vector3d incomingVelocity( 34216.4827530912, -15170.1440677825,
                                            395.792122152361 );

    // Define rotation angle.
    const double rotationAngle = -2.0291949514117;

    // Define pericenter radius.
    const double pericenterRadius = 6351801.04541467;

    // Define deltaV.
    const double deltaV = 1090.64622870007;

    // Perform the gravity assist.
    const Eigen::Vector3d outgoingVelocity = mission_segments::gravityAssist(
                                                    venusGravitationalParameter, venusVelocity,
                                                    incomingVelocity, rotationAngle,
                                                    pericenterRadius, deltaV );

    // Test if the computed outgoing velocity corresponds to the expected velocity within the
    // specified tolerance.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedOutgoingVelocity, outgoingVelocity, tolerance );
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
