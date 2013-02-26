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
 *      120717    D. Dirkx          File created.
 *      130226    K. Kumar          Updated return-type for getCartesianStateFromEphemeris().
 *
 *    References
 *
 *    Notes
 *
 */

#ifndef TUDAT_SPICE_EPHEMERIS_H
#define TUDAT_SPICE_EPHEMERIS_H

#include <string>

#include "Tudat/Astrodynamics/Ephemerides/ephemeris.h"

#include "Tudat/External/SpiceInterface/spiceInterface.h"

#include "Tudat/Mathematics/BasicMathematics/linearAlgebraTypes.h"

namespace tudat
{
namespace ephemerides
{

class SpiceEphemeris : public Ephemeris
{
public:

    //! Constructor.
    /*!
     * Constructor, sets the input variables for the calls to the spice function to retrieve state.
     * \param targetBodyName Name of body of which the ephemeris is to be calculated
     * \param observerBodyNameBody relative to which the ephemeris is to be calculated
     * \param correctForStellarAbberation Boolean whether to correct for stellar Abberation in
     *          retrieved values of (observed state).
     * \param correctForLightTimeAbberation Boolean whether to correct for light time in
     *          retrieved values of (observed state).
     * \param convergeLighTimeAbberation Boolean whether to use single iteration or max. 3
     *          iterations for calculating light time.
     * \param referenceFrameName Name of the reference frame in which the epehemeris is to be
     *          calculated.
     */
    SpiceEphemeris( const std::string& targetBodyName, const std::string& observerBodyName,
                    const bool correctForStellarAbberation = true,
                    const bool correctForLightTimeAbberation = true,
                    const bool convergeLighTimeAbberation = false,
                    const std::string& referenceFrameName = "ECLIPJ2000" );

    //! Get Cartesian state from ephemeris.
    /*!
     * Returns Cartesian state from ephemeris at given Julian day.
     * \param ephemerisTime Ephemeris time at which cartesian state is to be determined.
     * \return State from ephemeris.
     */
    basic_mathematics::Vector6d getCartesianStateFromEphemeris( const double julianDay );

private:

    //! Name of body of which ephemeris is to be determined
    /*!
     * Name of body of which ephemeris is to be determined. Name can be either normal name
     * (example Jupiter, not case-sensitive), or NAIF ID number used internally in Spice.
     */
    std::string targetBodyName_;

    //! Name of body w.r.t. which ephemeris is to be determined.
    /*!
     * Name of body w.r.t. which ephemeris is to be determined (i.e. where reference frame
     * is to be centered). Name can be either normal name (example Jupiter, not case-sensitive),
     * or NAIF ID number used internally in Spice.
     */
    std::string observerBodyName_;

    //! Name of the reference frame in which ephemeris is to be expressed.
    /*!
     * Name of the reference frame in which ephemeris is to be expressed. This identifier gives
     * only the orientation of the reference frame, the origin is defined by the observerBodyName_
     * variable.
     */
    std::string referenceFrameName_;

    //! Aberration corrections that are to be applied when retrieving ephemeris.
    /*!
     * Aberration corrections that are to be applied when retrieving ephemeris. Stellar and light-
     * time aberration can be corrected for. Note that only the finite speed of light is included
     * in the light-time aberration, not general relativistic delay or path bending. The variable
     * defines the corrections the same as the required input to the spkezr spice function
     * ( see corresponding spice documentation ).
     */
    std::string abberationCorrections_;
};

} // namespace ephemerides
} // namespace tudat

#endif // TUDAT_SPICE_EPHEMERIS_H
