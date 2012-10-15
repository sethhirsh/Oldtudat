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
 *      110405    K. Kumar          File created.
 *      110406    K. Kumar          Added cerr statements.
 *      110411    K. Kumar          Changed test to check errors in position, right ascension, and
 *                                  declination.
 *      110701    K. Kumar          Updated to use new predefined planets architecture; removed
 *                                  dynamic memory allocation.
 *      110714    L. van der Ham    Added circular coplanar case.
 *      111024    K. Kumar          Spotted error in circular coplanar test after changing abs( )
 *                                  to fabs( ). Needs to be fixed.
 *      120322    D. Dirkx          Modified to new Ephemeris interfaces.
 *      120513    P. Musegaas       Boostified unit test.
 *
 *    References
 *      HORIZONS Web-Interface, http://ssd.jpl.nasa.gov/horizons.cgi, last accessed: 5 April, 2011.
 *
 *    It is noted during the 120513 check that this is not a very extensive and/or precise unit
 *    test. Given that the ephemeris class will soon be updated, this is not deemed a big issue.
 *    However the unit test will have to be improved a lot in the next update. Some code was
 *    outcommented when boostifying the unit test. It is attached in commented version at the
 *    bottom of this file.
 *
 *    Also the test of the approximate planet positions (3D) was changed. It used to check the
 *    only the spherical position coordinates. It was changed to check the cartesian elements in
 *    total. The accuracy with which this is possible is very low though.
 */

#define BOOST_TEST_MAIN

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <TudatCore/Basics/testMacros.h>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h>

#include "Tudat/Astrodynamics/Bodies/celestialBody.h"
#include "Tudat/Astrodynamics/Bodies/planet.h"
#include "Tudat/Astrodynamics/Bodies/Ephemeris/approximatePlanetPositionsCircularCoplanar.h"

namespace tudat
{
namespace unit_tests
{

//! Test the functionality of the approximate planet position functions
BOOST_AUTO_TEST_SUITE( test_approximate_planet_positions )

//! Test the orbital elements function against the orbital elements of Mars at JD 2455626.5.
BOOST_AUTO_TEST_CASE( testOrbitalElements )
{
    using tudat::unit_conversions::convertDegreesToRadians;

    // Set tolerance.
    const double tolerance = 2.0e-2;

    // Expected result.
    Eigen::VectorXd expectedKeplerianElements( 6 );
    expectedKeplerianElements[ 0 ] = 2.279361944126564e11;
    expectedKeplerianElements[ 1 ] = 9.338126166083623e-2;
    expectedKeplerianElements[ 2 ] = convertDegreesToRadians( 1.848907897011101 );
    expectedKeplerianElements[ 3 ] = convertDegreesToRadians( 2.866464026954701e2 );
    expectedKeplerianElements[ 4 ] = convertDegreesToRadians( 4.952419052428279e1 );
    expectedKeplerianElements[ 5 ] = convertDegreesToRadians( 3.577219707986779e2 );

    // Create predefined Mars.
    bodies::Planet predefinedMars;
    predefinedMars.setPredefinedPlanetSettings( bodies::Planet::mars );

    // Create predefined Sun.
    bodies::Planet predefinedSun;
    predefinedSun.setPredefinedPlanetSettings( bodies::Planet::sun );

    // Convert the expected Keplerian elements to Cartesian elements.
    Eigen::VectorXd expectedEphemeris;
    expectedEphemeris = orbital_element_conversions::convertKeplerianToCartesianElements(
            expectedKeplerianElements,
            predefinedSun.getGravityFieldModel( )->getGravitationalParameter( ) );

    // Retrieve state of Mars in Cartesian elements at Julian date 2455626.5.
    Eigen::VectorXd marsEphemeris;
    marsEphemeris = predefinedMars.getEphemeris( )->getCartesianStateFromEphemeris( 2455626.5 );

    // Test if the computed ephemeris matches the expected ephemeris within the tolerance set.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedEphemeris, marsEphemeris, tolerance );
}

//! Test the cicular coplanar function against orbital elements of Mars at JD 2455626.5.
BOOST_AUTO_TEST_CASE( testCircularCoplannar )
{
    using namespace tudat::ephemerides;

    // Create predefined Sun.
    bodies::Planet predefinedSun;
    predefinedSun.setPredefinedPlanetSettings( bodies::Planet::sun );

    // Create predefined Mars and set circular coplanar planet positions.
    bodies::CelestialBody predefinedMarsCircularCoplanar;
    predefinedMarsCircularCoplanar.setEphemeris(
                boost::make_shared< ApproximatePlanetPositionsCircularCoplanar >(
                                             ApproximatePlanetPositionsBase::mars ) );

    Eigen::VectorXd marsEphemerisCircularCoplanar;
    marsEphemerisCircularCoplanar = predefinedMarsCircularCoplanar.getEphemeris( )->
            getCartesianStateFromEphemeris( 2455626.5 );

    // Compute the Keplerian elements from this ephemeris.
    Eigen::VectorXd keplerianElementsCircularCoplanar;
    keplerianElementsCircularCoplanar = orbital_element_conversions::
            convertCartesianToKeplerianElements( marsEphemerisCircularCoplanar,
                    predefinedSun.getGravityFieldModel( )->getGravitationalParameter( ) );

    // Check the eccentricity, inclination and z-component of velocity and position are 0.
    BOOST_CHECK_SMALL( keplerianElementsCircularCoplanar( 1 ), 1e-15 );
    BOOST_CHECK_SMALL( keplerianElementsCircularCoplanar( 2 ),
                       std::numeric_limits< double >::min( ) );
    BOOST_CHECK_SMALL( marsEphemerisCircularCoplanar( 2 ), 2.0e-5 );
    BOOST_CHECK_SMALL( marsEphemerisCircularCoplanar( 5 ),
                       std::numeric_limits< double >::min( ) );

}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat

//    // Compute the difference in semi-major axis between Test 2 and
//    // the external EphemerisData "p_elem_t2.txt".
//    double errorSemiMajorAxis = fabs( positionMars.norm( )
//                                      - convertAstronomicalUnitsToMeters( 1.52371243 ) )
//            / convertAstronomicalUnitsToMeters( 1.52371243 );

//    if ( errorSemiMajorAxis > errorTolerance_ )
//    {
//        isApproximatePlanetPositionsErroneous = true;

//        // Generate error statements.
//        cerr << "The computed relative error in position of the  "<< endl;
//        cerr << "coplanar circular position of Mars ( " << errorSemiMajorAxis << " )"<< endl;
//        cerr << "using the ApproximatePlanetPositionsCircularCoplanar class, exceeds "
//             << "the maximum expected error " << endl;
//        cerr << "( " << errorTolerance_ << " )." << endl;
//    }

//    // Check orientation of position vector by comparison of separate components.
//    // Error in position should be smaller than maximum expected offset with respect to
//    // elliptical and inclined orbits.
//    double maximumErrorPosition =   keplerianElementsTest3D.getSemiMajorAxis( ) * (
//                keplerianElementsTest3D.getEccentricity( ) + 1.0
//                - cos( keplerianElementsTest3D.getInclination( ) ) );
//    Vector3d errorPositionVector = positionMars - marsEphemeris.getPosition( );

//    if ( fabs( errorPositionVector( 0 ) ) > maximumErrorPosition
//         || fabs( errorPositionVector( 1 ) ) > maximumErrorPosition )
//    {
//        isApproximatePlanetPositionsErroneous = true;

//        // Generate error statements.
//        cerr << "The computed error in position vector of the  "<< endl;
//        cerr << "coplanar circular position of Mars ( "
//             << errorPositionVector << " meters )" << endl;
//        cerr << "using the ApproximatePlanetPositionsCircularCoplanar class, exceeds "
//             << "the expected error (" << endl;
//        cerr << "( " << maximumErrorPosition << " meters )." << endl;
//    }

    /* FIX THIS TEST!!!
    // Check size of velocity.
    Eigen::Vector3d errorVelocity = velocityMars - marsEphemeris.segment( 3, 3 );

    // Error in scalar velocity should be smaller than maximum expected offset with respect to
    // ellipitical and inclined orbits.
    double expectedErrorVelocity = fabs(
                sqrt( predefinedSun.getGravitationalParameter( )
                      / marsEphemeris.segment( 0, 3 ).norm( ) ) *
                ( ( 1.0 - cos( keplerianElementsTest3D.getInclination( ) )
                    + sqrt( ( 1.0 - keplerianElementsTest3D.getEccentricity( ) ) /
                            ( 1.0 + keplerianElementsTest3D.getEccentricity( ) ) ) - 1.0 ) ) );


    if ( errorVelocity.norm( ) > expectedErrorVelocity )
    {
        isApproximatePlanetPositionsErroneous = true;

        // Generate error statements.
        cerr << "The computed error in velocity of the "<< endl;
        cerr << "coplanar circular position of Mars "
             << "( " << errorVelocity.norm( ) << " meters per second )" << endl;
        cerr << "using the ApproximatePlanetPositionsCircularCoplanar class, exceeds "
             << "the expected error " << endl;
        cerr << "( " << expectedErrorVelocity << " meters per second )." << endl;
        cerr << "The computed error exceeds the expected error by: "
             << fabs( errorVelocity.norm( ) - expectedErrorVelocity )
             << " meters per second." << endl;
    }
    */
