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
 *      120926    E. Dekens         File created.
 *
 *    References
 *
 *    Notes
 *
 */

#define BOOST_TEST_MAIN

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <Eigen/Core>

#include <TudatCore/Basics/testMacros.h>

#include "Tudat/Mathematics/BasicMathematics/sphericalHarmonics.h"

namespace tudat
{
namespace unit_tests
{

//! Typedef for Vector10d (vector of 10 doubles).
typedef Eigen::Matrix< double, 10, 1 > Vector10d;

//! Typedef for Vector10i (vector of 10 integers).
typedef Eigen::Matrix< int, 10, 1 > Vector10i;

//! Test suite for spherical harmonics.
BOOST_AUTO_TEST_SUITE( test_SphericalHarmonics )

BOOST_AUTO_TEST_CASE( test_SphericalHarmonics_PotentialGradient )
{
    // Define cosine harmonic coeficients.
    const Vector10d cosineHarmonicCoefficient = ( Eigen::VectorXd( 10 ) <<
                                                  0.0,
                                                  0.0,
                                                  0.0,
                                                  -4.841651437908150e-4,
                                                  -2.066155090741760e-10,
                                                  2.439383573283130e-6,
                                                  9.571612070934730e-7,
                                                  2.030462010478640e-6,
                                                  9.047878948095281e-7,
                                                  7.213217571215680e-7
                                                  ).finished( );

    // Define sine harmonic coeficients.
    const Vector10d sineHarmonicCoefficient = ( Eigen::VectorXd( 10 ) <<
                                                0.0,
                                                0.0,
                                                0.0,
                                                0.0,
                                                1.384413891379790e-9,
                                                -1.400273703859340e-6,
                                                0.0,
                                                2.482004158568720e-7,
                                                -6.190054751776180e-7,
                                                1.414349261929410e-6
                                                ).finished( );

    // Define degree and order vectors.
    const Vector10i degree = ( Eigen::VectorXi( 10 ) << 0, 1, 1, 2, 2, 2, 3, 3, 3, 3 ).finished( );
    const Vector10i order = ( Eigen::VectorXi( 10 ) << 0, 0, 1, 0, 1, 2, 0, 1, 2, 3 ).finished( );

    // Define Earth radius.
    const double referenceRadius = 6378137.0;

    // Define the generic multiplier of potential expression.
    const double preMultiplier = 3.986004418e+14 / referenceRadius;

    // Define an arbitrary position in spherical coordinates.
    const Eigen::Vector3d sphericalPosition = Eigen::Vector3d( 6478137.0, 0.5, 1.7 );

    // Define Legendre polynomials.
    const Vector10d legendrePolynomial = ( Eigen::VectorXd( 10 ) <<
                                           1.000000000000000,
                                           8.660254037844386e-1,
                                           1.500000000000000,
                                           -2.795084971874738e-1,
                                           1.677050983124842,
                                           1.452368754827781,
                                           -1.157516198590759,
                                           3.507803800100574e-1,
                                           1.921303268617425,
                                           1.358566569955260
                                           ).finished( );

    // Define Legendre polynomial derivatives.
    const Vector10d legendrePolynomialDerivative = ( Eigen::VectorXd( 10 ) <<
                                                     0.0,
                                                     1.732050807568877,
                                                     -1.000000000000000,
                                                     3.354101966249685,
                                                     2.236067977499790,
                                                     -1.936491673103709,
                                                     9.921567416492215e-1,
                                                     6.781754013527770,
                                                     1.280868845744950,
                                                     -2.717133139910520
                                                     ).finished( );

    // Compute to be tested potential gradient.
    Eigen::MatrixXd testPotentialGradient( 10, 3 );
    for ( int index = 0; index < degree.size( ); index++ )
    {
        Eigen::Vector3d placeholder = tudat::basic_mathematics::computePotentialGradient(
                    sphericalPosition,
                    referenceRadius,
                    preMultiplier,
                    degree( index ),
                    order( index ),
                    cosineHarmonicCoefficient( index ),
                    sineHarmonicCoefficient( index ),
                    legendrePolynomial( index ),
                    legendrePolynomialDerivative( index ) );

        // Transpose test values matrix.
        testPotentialGradient.row( index ) = placeholder.transpose( );
    }

    // Define expected radius gradient values.
    const Eigen::MatrixXd expectedRadiusGradients = ( Eigen::VectorXd( 10 ) <<
                                                      0.0,
                                                      0.0,
                                                      0.0,
                                                      -3.737960776710316e-3,
                                                      -6.482821630471988e-8,
                                                      8.025564370222122e-5,
                                                      4.017360659202147e-5,
                                                      1.969234962725430e-7,
                                                      4.992070975259301e-5,
                                                      5.107365294374567e-5
                                                      ).finished( );

    // Define expected latitude gradient values.
    const Vector10d expectedLatitudeGradients = ( Eigen::VectorXd( 10 ) <<
                                                  0.0,
                                                  0.0,
                                                  0.0,
                                                  -8.500272421463796e4,
                                                  1.638021763158823e-1,
                                                  2.027828289808856e2,
                                                  4.894092287935980e1,
                                                  -5.411060610134959,
                                                  -4.730070504102903e1,
                                                  1.451794138284170e2
                                                  ).finished( );

    // Define expected longitude gradient values.
    const Vector10d expectedLongitudeGradients = ( Eigen::VectorXd( 10 ) <<
                                                   0.0,
                                                   0.0,
                                                   0.0,
                                                   0.0,
                                                   2.652663251706929e-3,
                                                   3.425472294686391e2,
                                                   0.0,
                                                   -4.213637561313778e1,
                                                   1.872176926758908e2,
                                                   2.877868568505883e2
                                                   ).finished( );

    // Define total matrix of expected gradients.
    Eigen::MatrixXd expectedValues( 10, 3 );
    expectedValues.col( 0 ) = expectedRadiusGradients;
    expectedValues.col( 1 ) = expectedLatitudeGradients;
    expectedValues.col( 2 ) = expectedLongitudeGradients;

    // Check if test values match expected values.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( testPotentialGradient, expectedValues, 1.0e-15 );
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
