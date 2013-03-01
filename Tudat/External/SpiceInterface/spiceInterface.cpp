/*    Copyright (c) 2010-2013, Delft University of Technology
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
 *      120717    D. Dirkx          Creation of file.
 *      120921    M.I. Ganeff       Added the functions getTotalCountOfKernelsLoaded and
 *                                  clearSpiceKernels.
 *
 *    References
 *
 *    Notes
 *
 */

#include <boost/lexical_cast.hpp>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>

#include "Tudat/External/SpiceInterface/spiceInterface.h"

namespace tudat
{
namespace spice_interface
{

using tudat::basic_mathematics::Vector6d;

//! Convert a Julian date to ephemeris time (equivalent to TDB in Spice).
double convertJulianDateToEphemerisTime( const double julianDate )
{
    return ( julianDate - j2000_c( ) ) * spd_c( );
}

//! Convert ephemeris time (equivalent to TDB) to a Julian date.
double convertEphemerisTimeToJulianDate( const double ephemerisTime )
{
    return j2000_c( ) + ( ephemerisTime ) / spd_c( );
}

//! Converts a date string to ephemeris time.
double convertDateStringToEphemerisTime( const std::string& dateString )
{
    double ephemerisTime = 0.0;
    str2et_c( dateString.c_str( ), &ephemerisTime );
    return ephemerisTime;
}

//! Get Cartesian state of a body, as observed from another body.
Vector6d getBodyCartesianStateAtEpoch(
        const std::string& targetBodyName, const std::string& observerBodyName,
        const std::string& referenceFrameName, const std::string& abberationCorrections,
        const double ephemerisTime )
{

    // Declare variables for cartesian state and light-time to be determined by Spice.
    double stateAtEpoch[ 6 ];
    double lightTime;

    // Call Spice function to calculate state and light-time.
    spkezr_c( targetBodyName.c_str( ), ephemerisTime, referenceFrameName.c_str( ),
              abberationCorrections.c_str( ), observerBodyName.c_str( ), stateAtEpoch,
              &lightTime );

    // Put result in Eigen Vector.
    Vector6d cartesianStateVector;
    for ( unsigned int i = 0; i < 6 ; i++ )
    {
        cartesianStateVector[ i ] = stateAtEpoch[ i ];
    }

    // Convert from km(/s) to m(/s).
    return unit_conversions::convertKilometersToMeters< Vector6d >(
                cartesianStateVector );
}

//! Get Cartesian position of a body, as observed from another body.
Eigen::Vector3d getBodyCartesianPositionAtEpoch( const std::string& targetBodyName,
                                                 const std::string& observerBodyName,
                                                 const std::string& referenceFrameName,
                                                 const std::string& abberationCorrections,
                                                 const double ephemerisTime )
{
    // Declare variables for cartesian position and light-time to be determined by Spice.
    double positionAtEpoch[ 3 ];
    double lightTime;

    // Call Spice function to calculate position and light-time.
    spkpos_c( targetBodyName.c_str( ), ephemerisTime, referenceFrameName.c_str( ),
              abberationCorrections.c_str( ), observerBodyName.c_str( ), positionAtEpoch,
              &lightTime );

    // Put result in Eigen Vector.
    Eigen::Vector3d cartesianPositionVector;
    for ( unsigned int i = 0; i < 3 ; i++ )
    {
        cartesianPositionVector[ i ] = positionAtEpoch[ i ];
    }

    // Convert from km to m.
    return unit_conversions::convertKilometersToMeters< Eigen::Vector3d >(
                cartesianPositionVector );
}

//! Compute quaternion of rotation between two frames.
Eigen::Quaterniond computeRotationQuaternionBetweenFrames( const std::string& originalFrame,
                                                           const std::string& newFrame,
                                                           const double ephemerisTime )
{
    // Declare rotation matrix.
    double rotationArray[ 3 ][ 3 ];

    // Calculate rotation matrix.
    pxform_c( originalFrame.c_str( ), newFrame.c_str( ), ephemerisTime, rotationArray );

    // Put rotation matrix in Eigen Matrix3d.
    Eigen::Matrix3d rotationMatrix;
    for ( unsigned int i = 0; i < 3; i++ )
    {
        for ( unsigned int j = 0; j < 3; j++ )
        {
            rotationMatrix( i, j ) = rotationArray[ i ][ j ];
        }
    }

    // Convert matrix3d to Quaternion.
    return Eigen::Quaterniond( rotationMatrix );
}

//! Get property of a body from Spice.
std::vector< double > getBodyProperties( const std::string& body, const std::string& property,
                                         const int maximumNumberOfValues )
{
    // Delcare variable in which raw result is to be put by Spice function.
    double propertyArray[ maximumNumberOfValues ];

    // Call Spice function to retrieve property.
    SpiceInt numberOfReturnedParameters;
    bodvrd_c( body.c_str( ), property.c_str( ), maximumNumberOfValues, &numberOfReturnedParameters,
              propertyArray );

    // Put result in STL vector.
    std::vector< double > bodyProperties;
    bodyProperties.resize( numberOfReturnedParameters );
    for ( int i = 0; i < numberOfReturnedParameters; i++ )
    {
        bodyProperties.at( i ) = propertyArray[ i ];
    }
    return bodyProperties;
}

//! Get gravitational parameter of a body.
double getBodyGravitationalParameter( const std::string& body )
{
    // Delcare variable in which raw result is to be put by Spice function.
    double gravitationalParameter[ 1 ];

    // Call Spice function to retrieve gravitational parameter.
    SpiceInt numberOfReturnedParameters;
    bodvrd_c( body.c_str( ), "GM", 1, &numberOfReturnedParameters, gravitationalParameter );

    // Convert from km^3/s^2 to m^3/s^2
    return unit_conversions::convertKilometersToMeters< double >(
                unit_conversions::convertKilometersToMeters< double >(
                    unit_conversions::convertKilometersToMeters< double >(
                        gravitationalParameter[ 0 ] ) ) );
}

//! Get the (arithmetic) mean of the three principal axes of the tri-axial ellipsoid shape.
double getAverageRadius( const std::string& body )
{
    // Delcare variable in which raw result is to be put by Spice function.
    double radii[ 3 ];

    // Call Spice function to retrieve gravitational parameter.
    SpiceInt numberOfReturnedParameters;
    bodvrd_c( body.c_str( ), "RADII", 3, &numberOfReturnedParameters, radii );

    // Compute average and convert from km to m.
    return unit_conversions::convertKilometersToMeters< double >( radii[ 0 ] + radii[ 1 ] +
                                                                  radii[ 2 ] ) / 3.0;
}

//! Convert a body name to its NAIF identification number.
int convertBodyNameToNaifId( const std::string& bodyName )
{
    // Convert body name to NAIF ID number.
    SpiceInt bodyNaifId;
    SpiceBoolean isIdFound;
    bods2c_c( bodyName.c_str( ), &bodyNaifId, &isIdFound );

    // Convert SpiceInt (typedef for long) to int and return.
    return static_cast< int >( bodyNaifId );
}

//! Check if a certain property of a body is in the kernel pool.
bool checkBodyPropertyInKernelPool( const std::string& bodyName, const std::string& bodyProperty )
{
    // Convert body name to NAIF ID.
    const int naifId = convertBodyNameToNaifId( bodyName );

    // Determine if property is in pool.
    SpiceBoolean isPropertyInPool = bodfnd_c( naifId, bodyProperty.c_str( ) );
    return static_cast< bool >( isPropertyInPool );
}

//! Load a Spice kernel.
void loadSpiceKernelInTudat( const std::string& fileName )
{
    furnsh_c(  fileName.c_str( ) );
}

//! Get the amount of loaded Spice kernels.
int getTotalCountOfKernelsLoaded( )
{
    SpiceInt count;
    ktotal_c( "ALL", &count );
    return count;
}

//! Clear all Spice kernels.
void clearSpiceKernels( ) { kclear_c( ); }

} // namespace spice_interface
} // namespace tudat
