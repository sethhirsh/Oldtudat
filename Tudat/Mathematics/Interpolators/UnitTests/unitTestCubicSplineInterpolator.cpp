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
 *      110621    F.M. Engelen      File created.
 *      110707    E.A.G. Heeren     Minor spelling/lay-out corrections.
 *      110714    E.A.G. Heeren     Minor spelling/lay-out corrections.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120529    E.A.G. Heeren     Boostified unit test.
 *      120615    T. Secretin       Added check for exception handling.
 *      120716    D. Dirkx          Updated with interpolator architecture.
 *
 *    References
 *
 */

#define BOOST_TEST_MAIN

#include <boost/test/unit_test.hpp>

#include <vector>

#include <TudatCore/Basics/testMacros.h>
#include <TudatCore/InputOutput/matrixTextFileReader.h>

#include "Tudat/Mathematics/Interpolators/cubicSplineInterpolator.h"
#include "Tudat/InputOutput/basicInputOutput.h"

namespace tudat
{
namespace unit_tests
{

// NOTE: No benchmark data from established software package has been used. Matlab implementation
// is slightly different than that implemented here, yielding error of at worst 10e-7 compared
// when using error function, which is used here as test function.
// However, a Matlab implementation of the same algorithm was found on te Matlab forum,
// which is used here as benchmark.
// In addition, the code has been used rather extensively and has been checked for basic
// characteristics such as continuity.
BOOST_AUTO_TEST_SUITE( test_cubic_spline_interpolator )

// Test implementation of cubic spline class.
BOOST_AUTO_TEST_CASE( testCubicSplineInterpolator )
{
    // Test 1: Compare with analytical function 2 + 3x + 5x^2.
    {
        // Declare and initialize independent variable values.
        std::vector< double > independentVariables;
        independentVariables.resize( 6 );
        independentVariables[ 0 ] = 1.0;
        independentVariables[ 1 ] = 3.0;
        independentVariables[ 2 ] = 5.0;
        independentVariables[ 3 ] = 7.0;
        independentVariables[ 4 ] = 9.0;
        independentVariables[ 5 ] = 11.0;

        // Declare and initialize dependent variable values.
        std::vector< double > dependentVariables;
        dependentVariables.resize( 6 );
        dependentVariables[ 0 ] = 10.0;
        dependentVariables[ 1 ] = 56.0;
        dependentVariables[ 2 ] = 142.0;
        dependentVariables[ 3 ] = 268.0;
        dependentVariables[ 4 ] = 434.0;
        dependentVariables[ 5 ] = 640.0;

        // Declare and initialize target independent variable value.
        const double targetIndependentVariableValue = 6.0;

        // Declare and initialize expected result of interpolation from analytical equation.
        const double analyticalValue = 200.0;

        // Declare cubic spline object and initialize with input data.
        interpolators::CubicSplineInterpolatorDouble cubicSplineInterpolation(
                    independentVariables, dependentVariables );

        // Declare interpolated dependent variable value and execute interpolation.
        const double interpolatedDependentVariableValue = cubicSplineInterpolation.interpolate(
                    targetIndependentVariableValue );

        // Check if test result match analytical result.
        BOOST_CHECK_SMALL(  std::fabs( analyticalValue - interpolatedDependentVariableValue )
                            / analyticalValue,
                            5.0e-3 );
    }
}

// Test exception handling implementation of cubic spline class.
BOOST_AUTO_TEST_CASE( testCubicSplineInterpolation_exception_empty_vectors )
{
    // Test 2: Interpolate with empty vectors.
    // Declare independent and dependent variable vectors.
    std::vector< double > independentVariables, dependentVariables;

    // Declare and initialize flag.
    bool areDependentAndIndependentVariablesInitialized = true;

    // Try to initialize with empty vectors.
    try
    {
        // Declare cubic spline object and initialize with input data.
        interpolators::CubicSplineInterpolatorDouble cubicSplineInterpolation(
                    independentVariables, dependentVariables );
    }

    // Catch the expected runtime error, and set the boolean flag to false.
    catch ( std::runtime_error )
    {
        areDependentAndIndependentVariablesInitialized = false;
    }

    // Check value of flag.
    BOOST_CHECK( !areDependentAndIndependentVariablesInitialized );
}

// Test cubic spline interpolator by comparing to Matlab code posted at
// http://www.mathworks.com/matlabcentral/newsreader/view_thread/173708.
BOOST_AUTO_TEST_CASE( test_cubicSplineInterpolator_matlab_forum_compare )
{
    using namespace tudat::interpolators;

    // Load input data used for generating matlab interpolation.
    Eigen::MatrixXd inputData = input_output::readMatrixFromFile(
                input_output::getTudatRootPath( ) +
                "Mathematics/Interpolators/UnitTests/interpolator_test_input_data.dat","," );

    // Put data in STL vectors.
    std::vector< double > independentVariableValues;
    std::vector< double > dependentVariableValues;
    for ( int i = 0; i < inputData.rows( ); i++ )
    {
        independentVariableValues.push_back( inputData( i, 0 ) );
        dependentVariableValues.push_back( inputData( i, 1 ) );
    }

    // Create cubic spline interpolator using hunting algorithm.
    CubicSplineInterpolatorDouble cubicSplineInterpolator(
                independentVariableValues, dependentVariableValues, hunting_algorithm );

    // Load points at which interpolator is to be evaluated and data generated by Matlab.
    Eigen::MatrixXd benchmarkData = input_output::readMatrixFromFile(
                input_output::getTudatRootPath( ) +
                "Mathematics/Interpolators/UnitTests/"
                + "cubic_spline_interpolator_test_output_data.dat",
                "," );

    // Perform interpolation for required data points.
    Eigen::VectorXd outputData = Eigen::VectorXd( benchmarkData.rows( ) );
    for ( int i = 0; i < outputData.rows( ); i++ )
    {
        outputData[ i ] = cubicSplineInterpolator.interpolate( benchmarkData( i, 0 ) );
    }

    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( benchmarkData.block( 0, 1, benchmarkData.rows( ), 1 ),
                                       outputData, 1.0e-13 );

    // Create cubic spline interpolator, now using binary search algorithm.
    cubicSplineInterpolator = CubicSplineInterpolatorDouble(
                independentVariableValues, dependentVariableValues, hunting_algorithm );

    // Perform interpolation for required data points.
    outputData = Eigen::VectorXd( benchmarkData.rows( ) );
    for ( int i = 0; i < outputData.rows( ); i++ )
    {
        outputData[ i ] = cubicSplineInterpolator.interpolate( benchmarkData( i, 0 ) );
    }

    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( benchmarkData.block( 0, 1, benchmarkData.rows( ), 1 ),
                                       outputData, 1.0e-13 );
}

// Test cubic spline interpolator by comparing to Matlab implementation. Note that the two
// implementations are not identical, since the Matlab implementation imposes zero first
// derivatives whereas the present implementation imposes zero second derivatives at endpoints.
BOOST_AUTO_TEST_CASE( test_cubicSplineInterpolator_matlab_compare )
{
    using namespace interpolators;

    // Load input data used for generating matlab interpolation.
    Eigen::MatrixXd inputData = input_output::readMatrixFromFile(
                input_output::getTudatRootPath( ) +
                "Mathematics/Interpolators/UnitTests/interpolator_test_input_data.dat", "," );

    // Put data in STL vectors.
    std::vector< double > independentVariableValues;
    std::vector< double > dependentVariableValues;
    for ( int i = 0; i < inputData.rows( ); i++ )
    {
        independentVariableValues.push_back( inputData( i, 0 ) );
        dependentVariableValues.push_back( inputData( i, 1 ) );
    }

    // Create cubic spline interpolator.
    CubicSplineInterpolatorDouble linearInterpolator(
                independentVariableValues, dependentVariableValues );

    // Load points at which interpolator is to be evaluated and data generated by Matlab.
    Eigen::MatrixXd benchmarkData = input_output::readMatrixFromFile(
                input_output::getTudatRootPath( ) +
                "Mathematics/Interpolators/UnitTests/"
                + "cubic_spline_interpolator_approximate_test_output_data.dat", "," );

    // Perform interpolation for required data points.
    Eigen::VectorXd outputData = Eigen::VectorXd( benchmarkData.rows( ) );
    for ( int i = 0; i < outputData.rows( ); i++ )
    {
        outputData[ i ] = linearInterpolator.interpolate( benchmarkData( i, 0 ) );
    }

    TUDAT_CHECK_MATRIX_CLOSE_FRACTION( benchmarkData.block( 0, 1, benchmarkData.rows( ), 1 ),
                                       outputData, 1.0e-5 );
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
