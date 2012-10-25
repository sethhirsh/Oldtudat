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
 *      120717    D. Dirkx          Creation of file.
 *      120921    M.I. Ganeff       Added the functions getTotalCountOfKernelsLoaded and
 *                                  clearSpiceKernels.
 *
 *    References
 *
 *    Notes
 *      In order to use the Spice interface, the C-Spice toolkit must be installed on your machine.
 *      It can be downloaded from http://naif.jpl.nasa.gov/naif/toolkit_C.html for all common
 *      operating systems. By placing the cspice folder in the External directory, or 1, 2 or 3 folder
 *      levels above the project source directory, it will be automatically located by the CMake list.
 *      In order to use Spice with Tudat, please run the makeall file provided with Spice to compile the
 *      static library.
 *      IMPORTANT: Before being able to use it, the cspice.a file in the cspice/lib folder needs to be renamed
 *      to libcspice.a.
 *
 *      In addition, the USE_CSPICE variable needs to be set to 1 in the top-level CMakeLists.txt
 *
 */

#ifndef TUDAT_SPICE_INTERFACE_H
#define TUDAT_SPICE_INTERFACE_H

#include <string>
#include <vector>

#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Tudat/Mathematics/BasicMathematics/linearAlgebraTypes.h"

extern "C"
{
    #include "cspice/include/SpiceUsr.h"
}

namespace tudat
{

namespace spice_interface
{

//! Convert a julian date to ephemeris time (equivalent to TDB in Spice).
/*!
 * Function to convert a julian date to ephemeris time, which is equivalent to barycentric
 * dynamical time. A leap second kernel must have been loaded to use this function.
 * \param julianDate Julian date that is to be converted to ephemeris time.
 * \return Ephemeris time calculated from Julian date.
 */
double convertJulianDateToEphemerisTime( const double julianDate );

//! Convert ephemeris time (equivalent to TDB) to a julian date.
/*!
 * Function to convert ephemeris time, which is nearly equal to barycentric dynamical time, to the
 * julian date. A leap second kernel must have been loaded to use this function.
 * \param ephemerisTime Ephemeris time that is to be converted to Julian date
 * \return Julian date calculated from ephemeris time.
 */
double convertEphemerisTimeToJulianDate( const double ephemerisTime );

//! Converts a date string to ephemeris time.
/*!
 * Function to convert a date string, for instance 1988 June 13, 3:29:48 to ephemeris time,
 * wrapper for str2et_c spice function.
 * \param dateString String representing the date. See documentation of spice function str2et_c
 *          for details on supported formats.
 * \return Ephemeris time corresponding to given dateString
 */
double convertDateStringToEphemerisTime( const std::string& dateString );

//! Get Cartesian state of a body, as observed from another body.
/*!
 * This function returns the state of a body, relative to another body, in a frame specified
 * by the user. Corrections for light-time correction and stellar
 * aberration can be applied to obtain the state of one of the bodies, as observed from the other.
 * Wrapper for spkezr_c spice function.
 * \param targetBodyName Name of the body of which the state is to be obtained. A kernel with the
 *          ephemeris of this body must have been loaded. The string must be a spice-recognized
 *          name or ID.
 * \param observerBodyName Name of the body relative to which the state is to be obtained.
 *          A kernel with theephemeris of this body must have been loaded.  The string must be a
 *          spice-recognized name or ID.
 * \param referenceFrameName The spize-revognized name of the reference frame in which the state
 *          is to be returned. Spice kernel(s) required to perform the neccesary conversion from
 *          the states of the target and observer bodies to this frame need to have been loaded.
 * \param abberationCorrections Setting for correction for setting corrections. See Spice
 *          documentation for extended discussion. Short summary: NONE: none, LT: light time
 *          corrected (one iteration for calculation), CN: light time corrected
 *          (multiple iterations, max 3) for calculation, S: Stellar abberation corrected. XLT and
 *          XCN can be provided to make the ephemerisTime input argument the transmission time,
 *          instead of receptionTime. Arguments can be combined (i.e."LT+S" or "XCN+S").
 * \param epehemerisTime Observation time (or transmission time of observed light, see description
 *          of abberationCorrections)
 * \return Cartesian state vector (x,y,z, position+velocity).
 */
basic_mathematics::Vector6d getBodyCartesianStateAtEpoch(
        const std::string& targetBodyName, const std::string& observerBodyName,
        const std::string& referenceFrameName, const std::string& abberationCorrections,
        const double ephemerisTime );

//! Get Cartesian position of a body, as observed from another body.
/*!
 * This function returns the position of a body, relative to another body, in a frame specified
 * by the user. Corrections for light-time correction and stellar
 * aberration can be applied to obtain the state of one of the bodies, as observed from the other.
 * Wrapper for spkpos_c spice function.
 * \param targetBodyName Name of the body of which the position is to be obtained. A kernel with
 *          the ephemeris of this body must have been loaded. The string must be a spice-recognized
 *          name or ID.
 * \param observerBodyName Name of the body relative to which the position is to be obtained.
 *          A kernel with theephemeris of this body must have been loaded.  The string must be a
 *          spice-recognized name or ID.
 * \param referenceFrameName The spice-revognized name of the reference frame in which the state
 *          is to be returned. Spice kernel(s) required to perform the neccesary conversion from
 *          the states of the target and observer bodies to this frame need to have been loaded.
 * \param abberationCorrections Setting for correction for setting corrections. See Spice
 *          documentation for extended discussion. Short summary: NONE: none, LT: light time
 *          corrected (one iteration for calculation), CN: light time corrected
 *          (multiple iterations, max 3) for calculation, S: Stellar abberation corrected. XLT and
 *          XCN can be provided to make the ephemerisTime input argument the transmission time,
 *          instead of receptionTime. Arguments can be combined (i.e."LT+S" or "XCN+S").
 * \param ephemerisTime Observation time (or transmission time of observed light, see description
 *          of abberationCorrections)
 * \return Cartesian position vector (x,y,z, position).
 */
Eigen::Vector3d getBodyCartesianPositionAtEpoch( const std::string& targetBodyName,
                                                 const std::string& observerBodyName,
                                                 const std::string& referenceFrameName,
                                                 const std::string& abberationCorrections,
                                                 const double ephemerisTime );

//! Compute quaternion of rotation between two frames.
/*!
 * This function computes the quaternion of rotation between two frames at a given time instant.
 * Kernels defining the two frames, as well as any required intermeidate frames, at the requested
 * time must have been loaded. Wrapper for pxform_c spice function.
 * \param originalFrame Reference frame from which the rotation is made.
 * \param newFrame Reference frame to which the rotation is made.
 * \param ephemerisTime Value of ephemeris time at which rotation is to be determined.
 */
Eigen::Quaterniond computeRotationQuaternionBetweenFrames( const std::string& originalFrame,
                                                           const std::string& newFrame,
                                                           const double ephemerisTime );

//! Get property of a body from Spice.
/*!
 * Function to retrieve a property of a body from Spice, wraps the bodvrd_c Spice function.
 * NOTE: Function returns values with distance unit km, not m!
 * \param body Name of the body of which the property is to be retrieved.
 * \param property Name of the property that is to be retrieved. Naming conventions can be found
 *          in the bodvrd_c function documentation.
 * \param maximumNumberOfValues Number of values by which the property is expressed (i.e. 1 for
 *          gravitational parameter, 3 for tri-axial ellipsoid principal axes).
 * \return Property value(s) expressed in an STL vector of doubles.
 */
std::vector< double > getBodyProperties( const std::string& body,
                                         const std::string& property,
                                         const int maximumNumberOfValues = 1 );

//! Get gravitational parameter of a body.
/*!
 * This function retrieves the gravitational parameter of a body.
 * Wraps the bodvrd_c spice function with "GM" as property type.
 * \param body Name of the body of which the parameter is to be retrieved.
 * \return Gravitational parameter of requested body.
 */
double getBodyGravitationalParameter( const std::string& body );

//! Get the (arithmetic) mean of the three principal axes of the tri-axial ellipsoid shape.
/*!
 * Returns the (arithmetic) mean of the three principal axes of the tri-axial ellipsoid shapen of
 * the requested body.
 * Uses the bodvrd_c spice function with "RADII" as property type.
 * \param body Name of the body of which the average radius is to be retrieved.
 * \return Arithmetic mean of principal axes of tri-axial ellipsoid shape model of body.
 */
double getAverageRadius( const std::string& body );

//! Convert a body name to its NAIF identification number.
/*!
 * This function converts a body name to its NAIF identification number.The NAIF id number is
 * required for a number of spice functions, whereas the name is easily interpretable by the user.
 * Wrapper for the bods2c_c function.
 * \param bodyName Name of the body for which NAIF id is to be retrieved.
 * \return NAIF id number for the body with bodyName.
 */
int convertBodyNameToNaifId( const std::string& bodyName );

//! Check if a certain property of a body is in the kernel pool.
/*!
 * This function checks if a certain property of a body is in the kernel pool. These properties
 * are defined in PCK kernels. Their names are given in the kernel file, typical names can be
 * found in the Spice documentation. Wrapper for the bodfnd_c function.
 * \param bodyName Name of the body of which the property is to be checked.
 * \param bodyProperty Name of the property of which the presence is to be checked, not case-
 *          sensitive.
 * \return True if property is in pool, false if not.
 */
bool checkBodyPropertyInKernelPool( const std::string& bodyName, const std::string& bodyProperty );

//! Load a Spice kernel.
/*!
 * This function loads a Spice kernel into the kernel pool, from which it can be used by the
 * various internal spice routines. Matters regarding the manner in which Spice handles different
 * kernels containing the same information can be found in the spice required reading
 * documentation, kernel section. Wrapper for the furnsh_c function.
 * \param fileName The file name of the Kernel to be loaded.
 */
void loadSpiceKernelInTudat( const std::string& fileName );

//! Get the amount of loaded Spice kernels.
/*!
 * This function returns the amount of Spice kernels that are loaded into the kernel pool. The
 * same kernel can be loaded multple times. Wrapper for the ktotal_c function.
 * \return Total amount of loaded Spice kernels.
 */
int getTotalCountOfKernelsLoaded( );

//! Clear all Spice kernels.
/*!
 * This function removes all Spice kernels from the kernel pool. Wrapper for the kclear_c function.
 */
void clearSpiceKernels( );

} // namespace spice_interfaces
} // namespace tudat

#endif // TUDAT_SPICE_INTERFACE_H
