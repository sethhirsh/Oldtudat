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
 *      110207    B. Romgens        File created.
 *      110215    K. Kumar          Minor modifications to layout, comments
 *                                  and variable-naming.
 *      110411    K. Kumar          Added unit test for
 *                                  convertCartesianToSpherical( ) function.
 *      110701    K. Kumar          Updated failing tests with relative errors.
 *      110708    K. Kumar          Added unit tests for computeSampleMean( )
 *                                  and computeSampleVariance( ) functions.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      111111    K. Kumar          Strange error with convertCylindricalToCartesian function;
 *                                  achieved precision of results is less than machine precision,
 *                                  fixed by using slightly larger precision tolerance.
 *      120202    K. Kumar          Separated from unitTestBasicMathematics.cpp into new
 *                                  Interpolators sub-directory.
 *
 *    References
 *
 */


#include <cmath>
#include <Eigen/Core>
#include <iostream>
#include <limits>
#include "Tudat/Astrodynamics/States/cartesianPositionElements.h"
#include "Tudat/Astrodynamics/States/state.h"
#include "Tudat/Mathematics/Interpolators/linearInterpolation.h"

//! Test implementation of linear interpolation functions.
int main( )
{
    // Using declarations.
    using std::cerr;
    using std::endl;
    using std::fabs;

    // Declare and initialize test result to false.
    bool isLinearInterpolationFunctionsErroneous = false;

    // Test linear interpolation from independent and dependent vectors.
    // Test 28: Test vector data with expected result of 0.0.
    // Test 29: Test vector data with expected result of -20.5

    // Declare variables.
    // Vectors of data.
    Eigen::VectorXd sortedIndependentVariables( 3 );
    Eigen::VectorXd associatedDependentVariables( 3 );

    // Target independent value in vector data.
    double targetIndependentVariableValue = 0.0;

    // Interpolated value.
    double interpolatedValue;

    // Initialize variables.
    sortedIndependentVariables( 0 ) = 0.0;
    sortedIndependentVariables( 1 ) = 1.0;
    sortedIndependentVariables( 2 ) = 3.0;
    associatedDependentVariables( 0 ) = -20.0;
    associatedDependentVariables( 1 ) = 20.0;
    associatedDependentVariables( 2 ) = 21.0;
    targetIndependentVariableValue = 0.5;

    // Compute interpolation.
    interpolatedValue = tudat::mathematics::interpolators::computeLinearInterpolation(
            sortedIndependentVariables, associatedDependentVariables,
            targetIndependentVariableValue );

    if ( fabs( interpolatedValue - 0.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
       cerr << "The computeLinearInterpolation function for vector data does "
            << "not function correctly, as the computed value: "
            << interpolatedValue << " does not match the expected value: "
            << 0.0 << endl;
       isLinearInterpolationFunctionsErroneous = true;
    }

    // Reinitialize vectors and target with new data.
    sortedIndependentVariables( 0 ) = 0.0;
    sortedIndependentVariables( 1 ) = 1.0;
    sortedIndependentVariables( 2 ) = 3.0;
    associatedDependentVariables( 0 ) = -20.0;
    associatedDependentVariables( 1 ) = 20.0;
    associatedDependentVariables( 2 ) = 21.0;
    targetIndependentVariableValue = 2.0;

    // Compute interpolation.
    interpolatedValue = tudat::mathematics::interpolators::computeLinearInterpolation(
            sortedIndependentVariables, associatedDependentVariables,
            targetIndependentVariableValue );

    if ( fabs( interpolatedValue - 20.5 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeLinearInterpolation function for vector data does "
             << "not function correctly, as the computed value: "
             << interpolatedValue << " does not match the expected value: "
             << 20.5 << endl;
       isLinearInterpolationFunctionsErroneous = true;
    }

    // Test linear interpolation with map of vectors with keys as
    // independent variable.
    // Test 30: Test map of data.

    // Declare map of data and vectors for map value.
    std::map < double, Eigen::VectorXd > sortedIndepedentAndDependentVariables;
    Eigen::VectorXd vectorOne( 3 );
    Eigen::VectorXd vectorTwo( 3 );
    Eigen::VectorXd vectorThree( 3 );
    Eigen::VectorXd interpolatedVector( 3 );

    // Initialize vectors for map value.
    // Initialize first vector.
    vectorOne( 0 ) = 10.0;
    vectorOne( 1 ) = -10.0;
    vectorOne( 2 ) = 70.0;

    // Initialize second vector.
    vectorTwo( 0 ) = 20.0;
    vectorTwo( 1 ) = -5.0;
    vectorTwo( 2 ) = 80.0;

    // Initialize third vector.
    vectorThree( 0 ) = 30.0;
    vectorThree( 1 ) = 60.0;
    vectorThree( 2 ) = 90.0;

    // Set map values in map using vector data.
    sortedIndepedentAndDependentVariables[ 0.0 ] = vectorOne;
    sortedIndepedentAndDependentVariables[ 1.0 ] = vectorTwo;
    sortedIndepedentAndDependentVariables[ 2.0 ] = vectorThree;

    // Set target independent variable value for interpolation.
    targetIndependentVariableValue = 1.5;

    // Compute interpolation.
    interpolatedVector = tudat::mathematics::interpolators::computeLinearInterpolation(
            sortedIndepedentAndDependentVariables,
            targetIndependentVariableValue );

    if ( fabs( interpolatedVector( 0 ) - 25.0 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( interpolatedVector( 1 ) - 27.5 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( interpolatedVector( 2 ) - 85.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeLinearInterpolation function for a map of "
             << "vectors, does not function correctly, as the compute vector "
             << "data: " <<  interpolatedVector << " does not match the "
             << "expected vector data: ( 25.0, 27.5, 85.0 ). " << endl;
       isLinearInterpolationFunctionsErroneous = true;
    }



    // Return test result.
    // If test is successful return false; if test fails, return true.
    if ( isLinearInterpolationFunctionsErroneous )
    {
        cerr << "testLinearInterpolationFunctions failed!" << endl;
    }

    return isLinearInterpolationFunctionsErroneous;
}


