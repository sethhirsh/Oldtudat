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
 *      110623    K. Kumar          First creation of code.
 *
 *    References
 *
 */

// Include statements.
#include "Tudat/Astrodynamics/Bodies/planet.h"

//! Tudat library namespace.
namespace tudat
{

//! Set predefined planet settings.
void Planet::setPredefinedPlanetSettings( PredefinedPlanets predefinedPlanet )
{
    // Select predefined planet.
    switch( predefinedPlanet )
    {
    case sun:

        // Set predefined Sun central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::sun );

        break;

    case mercury:

        // Set predefined Mercury central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::mercury );

        // Set Mercury as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::mercury );

        break;

    case venus:

        // Set predefined Venus central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::venus );

        // Set Venus as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::venus );

        break;

    case earth:

        // Set predefined Earth central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::earth );

        // Set Earth as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::earthMoonBarycenter );

        break;

    case moon:

        // Set predefined Moon central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::moon );

        // Set Moon as body for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::earthMoonBarycenter );

        break;

    case mars:

        // Set predefined Mars central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::mars );

        // Set Mars as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::mars );

        break;

    case jupiter:

        // Set predefined Jupiter central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::jupiter );

        // Set Jupiter as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::jupiter );

        break;

    case saturn:

        // Set predefined Saturn central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::saturn );

        // Set Saturn as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::saturn );

        break;

    case uranus:

        // Set predefined Uranus central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::uranus );

        // Set Uranus as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::uranus );

        break;

    case neptune:

        // Set predefined Neptune central gravity field.
        predefinedCentralGravityField_.setPredefinedCentralGravityFieldSettings(
                    CentralGravityField::neptune );

        // Set Neptune as planet for ephemeris.
        approximatePlanetPositions_.setPlanet( ApproximatePlanetPositions::neptune );

        break;

    default:

        // Print cerr statement.
        std::cerr << "Desired predefined planet does not exist." << std::endl;
    };

    // Set gravity model to predefined central gravity field.
    pointerToGravityFieldModel_ = &predefinedCentralGravityField_;

    // Set ephemeris.
    pointerToEphemeris_ = &approximatePlanetPositions_;
}

}

// End of file.
