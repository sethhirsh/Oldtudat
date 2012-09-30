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
 *      120926    E. Dekens         File created.
 *
 *    References
 *
 *    Notes
 *
 */

#include <cmath>

#include <Eigen/Core>

#include "Tudat/Mathematics/BasicMathematics/sphericalHarmonics.h"

namespace tudat
{
namespace basic_mathematics
{

// Compute the gradient of a single term of a spherical harmonics potential field.
Eigen::Vector3d computePotentialGradient(
        const Eigen::Vector3d& sphericalPosition,
        const double referenceRadius,
        const double preMultiplier,
        const int degree,
        const int order,
        const double cosineHarmonicCoefficient,
        const double sineHarmonicCoefficient,
        const double legendrePolynomial,
        const double legendrePolynomialDerivative )
{
    // Initialize return variable.
    Eigen::Vector3d potentialGradient;

    // Compute radius power term.
    const double radiusPowerTerm = std::pow( referenceRadius / sphericalPosition( radiusIndex ),
                                             static_cast< double >( degree ) + 1.0 );

    // Calculate derivative with respect to radius.
    potentialGradient( radiusIndex ) = - preMultiplier / sphericalPosition( radiusIndex )
            * radiusPowerTerm
            * ( static_cast< double >( degree ) + 1.0 ) * legendrePolynomial
            * ( cosineHarmonicCoefficient
                * std::cos( static_cast< double >( order ) * sphericalPosition( longitudeIndex ) )
                + sineHarmonicCoefficient
                * std::sin( static_cast< double >( order )
                            * sphericalPosition( longitudeIndex ) ) );

    // Calculate derivative with respect to latitude.
    potentialGradient( latitudeIndex ) = preMultiplier * radiusPowerTerm
            * legendrePolynomialDerivative
            * std::cos( sphericalPosition( latitudeIndex ) ) * ( cosineHarmonicCoefficient
            * std::cos( static_cast< double >( order ) * sphericalPosition ( longitudeIndex ) )
            + sineHarmonicCoefficient * std::sin( static_cast< double >( order )
                                                  * sphericalPosition( longitudeIndex ) ) );

    // Calculate derivative with respect to longitude.
    potentialGradient( longitudeIndex ) = preMultiplier * radiusPowerTerm
            * static_cast< double >( order ) * legendrePolynomial
            * ( sineHarmonicCoefficient * std::cos( static_cast< double >( order )
                                                    * sphericalPosition( longitudeIndex ) )
            - cosineHarmonicCoefficient * std::sin( static_cast< double >( order )
                                                    * sphericalPosition( longitudeIndex ) ) );

    // Return result.
    return potentialGradient;
}

} // namespace basic_mathematics
} // namespace tudat
