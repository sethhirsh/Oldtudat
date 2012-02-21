/*    Copyright (c) 2010-2012 Delft University of Technology.
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
 *      110131    E. Iorfida        First creation of the code.
 *      110202    E. Iorfida        Modified structure of the code, unique
 *                                  base class for launch and capture paths.
 *      110208    E. Iorfida        Deleted execute( ) function. Modified getDeltaV into
 *                                  computeDeltaV.
 *      110214    E. Iorfida        Code updated with the modification made in .h/.cpp files
 *                                  about radius of central body.
 *      110627    K. Kumar          Updated to use new predefined planets code.
 *
 *    References        :
 *      Mengali, G., Quarta, A.A. Fondamenti di Meccanica del volo Spaziale,
 *          Edizioni PLUS.
 *
 */

// Temporary notes (move to class/function doxygen):
// Test runs code and verifies result against expected value.
// If the tested code is erroneous, the test function returns a boolean
// true; if the code is correct, the function returns a boolean false.
// 

// Include statements.
#include <cmath>
#include <iostream>
#include "Tudat/Astrodynamics/Bodies/planet.h"
#include "Tudat/Astrodynamics/MissionSegments/capturePhase.h"
#include "Tudat/Astrodynamics/MissionSegments/escapeAndCapture.h"
#include "Tudat/Astrodynamics/MissionSegments/escapePhase.h"

//! Test patched conics implementation.
int main( )
{
    // Using declarations.
    using std::cerr;
    using std::endl;
    using std::fabs;
    using namespace tudat;

    // Test result initialised to false.
    bool isEscapeAndCaptureErroneous = false;

    // Set tolerance.
    double tolerance = 1.0e-1;

    // Expected test result based on example 11.4 page 334, "Fondamenti di
    // Meccanica del volo Spaziale", G. Mengali, A. A. Quarta.
    double expectedDeltaVEscape = 3.5244e3;
    double expectedDeltaVCapture = 1.9425e3;

    // Set test case.
    EscapePhase escapePhaseTest;
    CapturePhase capturePhaseTest;

    // Central bodies parameters.
    // Central body at launch phase.
    Planet predefinedEarth;
    predefinedEarth.setPredefinedPlanetSettings( Planet::earth );
    
    // Central body at capture phase.
    Planet predefinedMars;
    predefinedMars.setPredefinedPlanetSettings( Planet::mars );
    
    // Set launch conditions.
    escapePhaseTest.setCentralGravityField( predefinedEarth.getGravityFieldModel( ) );
    escapePhaseTest.setParkingOrbitRadius( 6371.0e3 );
    escapePhaseTest.setPeriapsisAltitude( 629.0e3 );
    escapePhaseTest.setEccentricity( 0.0 );
    escapePhaseTest.setHyperbolicExcessSpeed( 2.9444e3 );

    // Set capture conditions.
    capturePhaseTest.setCentralGravityField( predefinedMars.getGravityFieldModel( ) );
    capturePhaseTest.setParkingOrbitRadius( 3389.0e3 );
    capturePhaseTest.setPeriapsisAltitude( 2611.0e3 );
    capturePhaseTest.setEccentricity( 0.0 );
    capturePhaseTest.setHyperbolicExcessSpeed( 2.6486e3 );

    // Execute patched conic implementation.
    escapePhaseTest.computeDeltaV( );
    capturePhaseTest.computeDeltaV( );

    // Define delta-V of escape/capture phase.
    double deltaVEscape_ = escapePhaseTest.computeDeltaV( );
    double deltaVCapture_ = capturePhaseTest.computeDeltaV( );

    // Set test result to true if the test does not match the expected result.
    if ( fabs( deltaVEscape_ - expectedDeltaVEscape ) >= tolerance ||
         fabs( deltaVCapture_ - expectedDeltaVCapture ) >= tolerance )
    {
        isEscapeAndCaptureErroneous = true;

        if ( fabs( deltaVEscape_ - expectedDeltaVEscape ) >= tolerance )
        {
            cerr << "The computed value of the delta-V of the launch phase (" << deltaVEscape_
                 << ") does not match the expected solution ("
                 << expectedDeltaVEscape << ")." << endl;
            cerr << "The difference is: " << fabs( deltaVEscape_ - expectedDeltaVEscape ) << endl;
        }
        else if ( fabs( deltaVCapture_ - expectedDeltaVCapture ) >= tolerance )
        {
            cerr << "The computed value of the delta-V of the capture phase (" << deltaVCapture_
                 << ") does not match the expected solution (" << expectedDeltaVCapture << ")."
                 << endl;
            cerr << "The difference is: " << fabs( deltaVCapture_ - expectedDeltaVCapture )
                 << endl;
        }
    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    if ( isEscapeAndCaptureErroneous )
    {
        cerr << "testEscapeAndCapture failed!" << endl;
    }

    return isEscapeAndCaptureErroneous;
}

// End of file.
