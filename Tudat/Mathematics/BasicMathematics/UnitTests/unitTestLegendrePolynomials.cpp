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
 *      Mathworks. Legendre - Associated Legendre functions. Help documentation of MATLAB R2012a,
 *        2012a.
 *      Mathworks. Gravitysphericalharmonic - Implement spherical harmonic representation of
 *        planetary gravity. Help documentation of Aerospace Toolbox of MATLAB R2012a, 2012b.
 *      Eberly, D. Spherical Harmonics. Help documentation of Geometric Tools, 2008. Available at
 *        URL http://www.geometrictools.com/Documentation/SphericalHarmonics.pdf. Last access:
 *        09-09-2012.
 *      Heiskanen, W.A., Moritz, H. Physical geodesy. Freeman, 1967.
 *
 *    Notes
 *
 */

#define BOOST_TEST_MAIN

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/unit_test.hpp>

#include <Eigen/Core>

#include <TudatCore/Basics/testMacros.h>

#include "Tudat/Mathematics/BasicMathematics/legendrePolynomials.h"

namespace tudat
{
namespace unit_tests
{

//! Typedef for Vector10d (vector of 10 doubles).
typedef Eigen::Matrix< double, 10, 1 > Vector10d;

//! Typedef for Vector10i (vector of 10 integers).
typedef Eigen::Matrix< int, 10, 1 > Vector10i;

//! Typedef for Vector12d (vector of 12 doubles).
typedef Eigen::Matrix< double, 12, 1 > Vector12d;

//! Typedef for Vector12i (vector of 12 integers).
typedef Eigen::Matrix< int, 12, 1 > Vector12i;

BOOST_AUTO_TEST_SUITE( test_LegendreFunctions )

BOOST_AUTO_TEST_CASE( test_LegendrePolynomial )
{
    // Declare test values vector.
    Vector12d computedTestValues;

    // Define degree and order vectors.
    const Vector12i degree
            = ( Eigen::VectorXi( 12 ) << 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 150, 150 ).finished( );
    const Vector12i order
            = ( Eigen::VectorXi( 12 ) << 0, 0, 1, 0, 1, 2, 0, 1, 2, 3, 0, 150 ).finished( );

    // Define polynomial parameter.
    const double polynomialParameter = 0.5;

    // Loop through degrees and orders.
    for ( int index = 0; index < degree.size( ); index++ )
    {
        // Compute test value of Legendre polynomial.
        computedTestValues( index ) = basic_mathematics::computeLegendrePolynomial(
                    degree( index ),
                    order( index ),
                    polynomialParameter );
    }

    // Set expected values. These values have been obtained from the MATLAB function "legendre"
    // which is described by Mathworks [2012a]. The definition of the associated Legendre
    // polynomial in Mathworks [2012a] differs slightly from the definition used in "legendre.h":
    // the former includes the Condon-Shortley phase-factor, whereas the latter does not. The
    // Condon-Shortley phase factor consists of "(-1)^order". The results from Mathworks [2012a]
    // have been divided by this factor in order to make them match with the results from
    // "legendre.h".
    const Vector12d expectedValues = ( Eigen::VectorXd( 12 ) <<
                                       1.0,
                                       5.000000000000001e-1,
                                       8.660254037844388e-1,
                                       -1.249999999999999e-1,
                                       1.299038105676658,
                                       2.250000000000000,
                                       -4.375000000000001e-1,
                                       3.247595264191651e-1,
                                       5.625000000000002,
                                       9.742785792574935,
                                       6.749829804674222e-2,
                                       1.599576582902126e297
                                       ).finished( );

    // Check if test values match expected values.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedValues, computedTestValues, 1.0e-14 );
}

BOOST_AUTO_TEST_CASE( test_GeodesyLegendrePolynomial )
{
    // Declare test values vector.
    Vector10d computedTestValues;

    // Define degree and order vectors.
    const Vector10i degree = ( Eigen::VectorXi( 10 ) << 0, 1, 1, 2, 2, 2, 3, 3, 3, 3 ).finished( );
    const Vector10i order = ( Eigen::VectorXi( 10 ) << 0, 0, 1, 0, 1, 2, 0, 1, 2, 3 ).finished( );

    // Define polynomial parameter.
    const double polynomialParameter = 0.5;

    // Loop through degrees and orders.
    for ( int index = 0; index < degree.size( ); index++ )
    {
        // Compute test value of Legendre polynomial.
        computedTestValues( index ) = basic_mathematics::computeGeodesyLegendrePolynomial(
                    degree( index ),
                    order( index ),
                    polynomialParameter );
    }

    // Set expected values. These values have been obtained from the MATLAB subfunction
    // "loc_gravLegendre" which is housed in the main function "gravitysphericalharmonic". The
    // main function is described by Mathworks [2012b].
    const Eigen::VectorXd expectedValues = ( Eigen::VectorXd( 10 ) <<
                                             1.0,
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

    // Check if test values match expected values.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedValues, computedTestValues, 1.0e-15 );
}

BOOST_AUTO_TEST_CASE( test_LegendrePolynomialDerivative )
{
    // Initialize test values vector.
    Vector10d computedTestValues;

    // Define degree and order vectors.
    const Vector10i degree = ( Eigen::VectorXi( 10 ) << 0, 1, 1, 2, 2, 2, 3, 3, 3, 3 ).finished( );
    const Vector10i order = ( Eigen::VectorXi( 10 ) << 0, 0, 1, 0, 1, 2, 0, 1, 2, 3 ).finished( );

    // Define polynomial parameter.
    const double polynomialParameter = 0.5;

    // Define current Legendre polynomials.
    const Vector10d currentLegendrePolynomials =
            ( Eigen::VectorXd( 10 ) << 1.000000000000000,
              5.000000000000001e-1, 8.660254037844388e-1, -1.249999999999999e-1,
              1.299038105676658,    2.250000000000000,    -4.375000000000001e-1,
              3.247595264191651e-1, 5.625000000000002,    9.742785792574935
              ).finished( );

    // Define incremented Legendre polynomials.
    const Vector10d incrementedLegendrePolynomials =
            ( Eigen::VectorXd( 10 ) << 0.000000000000000,
              8.660254037844388e-1, 0.000000000000000, 1.299038105676658,
              2.250000000000000,    0.000000000000000, 3.247595264191651e-1,
              5.625000000000002,    9.742785792574935, 0.000000000000000
              ).finished( );

    // Loop through degrees and orders.
    for ( int index = 0; index < degree.size( ); index++ )
    {
        // Compute test value of Legendre polynomial derivative.
        computedTestValues( index ) = basic_mathematics::computeLegendrePolynomialDerivative(
                    order( index ), polynomialParameter,
                    currentLegendrePolynomials( index ), incrementedLegendrePolynomials( index ) );
    }

    // Define expected values of Legendre polynomial derivatives. These values have been obtained
    // by evaluating the explicit expressions given by Eberly [2008]. This evaluation has been
    // performed using MATLAB R2012a.
    const Vector10d expectedValues = ( Eigen::VectorXd( 10 ) <<
                                       0.0,
                                       1.000000000000000,
                                       -5.773502691896258e-1,
                                       1.500000000000000,
                                       1.732050807568877,
                                       -3.000000000000000,
                                       3.750000000000000e-1,
                                       6.278684177437181,
                                       3.750000000000000,
                                       -1.948557158514987e1
                                       ).finished( );

    // Check if test values match expected values.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedValues, computedTestValues, 1.0e-14 );
}

BOOST_AUTO_TEST_CASE( test_Legendre_GeodesyLegendrePolynomialDerivative )
{
    // Declare test values vector.
    Vector10d computedTestValues;

    // Define degree and order vectors.
    const Vector10i degree = ( Eigen::VectorXi( 10 ) << 0, 1, 1, 2, 2, 2, 3, 3, 3, 3 ).finished( );
    const Vector10i order = ( Eigen::VectorXi( 10 ) << 0, 0, 1, 0, 1, 2, 0, 1, 2, 3 ).finished( );

    // Define polynomial parameter.
    const double polynomialParameter = 0.5;

    // Define current Legendre polynomials.
    const Vector10d currentLegendrePolynomials =
            ( Eigen::VectorXd( 10 ) << 1.000000000000000,
              8.660254037844386e-1, 1.500000000000000, -2.795084971874738e-1,
              1.677050983124842,    1.452368754827781, -1.157516198590759,
              3.507803800100574e-1, 1.921303268617425, 1.358566569955260
              ).finished( );

    // Define incremented Legendre polynomials.
    const Vector10d incrementedLegendrePolynomials =
            ( Eigen::VectorXd( 10 ) << 0.000000000000000,
              1.500000000000000, 0.000000000000000, 1.677050983124842,
              1.452368754827781, 0.000000000000000, 3.507803800100574e-1,
              1.921303268617425, 1.358566569955260, 0.000000000000000
              ).finished( );

    // Loop through degrees and orders.
    for ( int index = 0; index < degree.size( ); index++ )
    {
        // Compute test value of Legendre polynomial derivative.
        computedTestValues( index )
                = basic_mathematics::computeGeodesyLegendrePolynomialDerivative(
                    degree( index ), order( index ), polynomialParameter,
                    currentLegendrePolynomials( index ), incrementedLegendrePolynomials( index ) );
    }

    // Define expected values of Legendre polynomial derivatives. These values have been obtained
    // by evaluating the explicit expressions given by Eberly [2008]. Subsequently these values
    // have been multiplied by the geodesy normalization factor as given by
    // Heiskanen & Moritz [1967]. This evaluation has been performed using MATLAB R2012a.
    const Vector10d expectedValues = ( Eigen::VectorXd( 10 ) <<
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

    // Check if test values match expected values.
    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( expectedValues, computedTestValues, 1.0e-14 );
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
