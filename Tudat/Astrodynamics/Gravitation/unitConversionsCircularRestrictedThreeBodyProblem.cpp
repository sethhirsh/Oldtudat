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
 *      110716    L. van der Ham    Creation of code.
 *      110804    L. van der Ham    Changed use of approximatePlanetPositionsCircularCoplanar.
 *      111108    K. Kumar          Simplified conversion functions; changed "normalized" to
 *                                  "dimensionless".
 *      120307    K. Kumar          Updated Doxygen comments; made functions const-correct;
 *                                  updated to satisfy Tudat protocols; moved file.
 *
 *    References
 *        Wakker, K.F.,"Astrodynamics I, AE4-874", Delft University of Technology, 2007.
 *
 *    Position dimension-scale is distance between the two massive bodies in the CRTBP.
 *    Time dimension-scale is based on orbital period of 2*pi.
 *
 */

#include <cmath>

#include "Tudat/Astrodynamics/Gravitation/unitConversionsCircularRestrictedThreeBodyProblem.h"

namespace tudat
{
namespace astrodynamics
{
namespace gravitation
{
namespace circular_restricted_three_body_problem
{

//! Convert dimensionless Cartesian state to dimensional units.
Eigen::VectorXd convertDimensionlessCartesianStateToDimensionalUnits(
        const Eigen::VectorXd &dimensionlessCartesianState,
        const double gravitationalParameterOfPrimaryBody, const double gravitationalParameterOfSecondaryBody,
        const double distanceBetweenPrimaries )
{
    // Declare dimensional Cartesian state.
    Eigen::VectorXd dimensionalCartesianState( 6 );

    // Convert position to dimensional units.
    dimensionalCartesianState.segment( 0, 3 )
            = dimensionlessCartesianState.segment( 0, 3 ) * distanceBetweenPrimaries;

    // Convert velocity to dimensional units.
    dimensionalCartesianState.segment( 3, 3 )
            = dimensionlessCartesianState.segment( 3, 3 )
            * std::sqrt( ( gravitationalParameterOfPrimaryBody
                           + gravitationalParameterOfSecondaryBody ) / distanceBetweenPrimaries );

    // Return state in dimensional units.
    return dimensionalCartesianState;
}

//! Convert dimensionless time to dimensional units.
double convertDimensionlessTimeToDimensionalTime(
        const double timeInDimensionlessUnits, const double gravitationalParameterOfPrimaryBody,
        const double gravitationalParameterOfSecondaryBody,
        const double distanceBetweenPrimaries )
{
    return timeInDimensionlessUnits * std::sqrt( std::pow( distanceBetweenPrimaries, 3.0 )
                                                 / ( gravitationalParameterOfPrimaryBody
                                                     + gravitationalParameterOfSecondaryBody ) );
}

} // namespace circular_restricted_three_body_problem
} // namespace gravitation
} // namespace astrodynamics
} // namespace tudat
