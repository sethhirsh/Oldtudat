/*! \file unitTestBasicMathematicsFunctions.cpp
 *    Source file that defines the unitTestBasicMathematicsFunctions unit test,
 *    containing all basic mathematics functions contained in Tudat.
 *
 *    Path              : /Mathematics/
 *    Version           : 7
 *    Check status      : Checked
 *
 *    Author            : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : bart.romgens@gmail.com
 *
 *    Author/Checker    : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Date created      : 7 February, 2011
 *    Last modified     : 5 September, 2011
 *
 *    References
 *
 *    Notes
 *
 *    Copyright (c) 2010-2011 Delft University of Technology.
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
 */

// Include statements.
#include <cmath>
#include <Eigen/Core>
#include <iostream> 
#include <limits>
#include "Tudat/Astrodynamics/States/cartesianPositionElements.h"
#include "Tudat/Mathematics/basicMathematicsFunctions.h"

//! Test implementation of basic mathematics functions.
int main( )
{
    // Using declarations.
    using std::cerr;
    using std::endl;
    using std::fabs;
    using std::acos;
    using std::asin;
    using std::atan2;
    using std::cos;
    using std::sin;
    using tudat::mathematics::computeModulo;
    using tudat::mathematics::convertCylindricalToCartesian;
    using tudat::mathematics::convertSphericalToCartesian;
    using tudat::mathematics::convertCartesianToSpherical;
    using namespace tudat;

    // Declare and initialize test result to false.
    bool isBasicMathematicsFunctionsErroneous = false;

    // Test modulo function.
    // Test 10: Test 0.0 mod 0.0.
    // Test 11: Test 2.0 mod 0.0.
    // Test 12: Test 2.0 mod 2.0.
    // Test 13: Test 3.0 mod 2.5.
    // Test 14: Test 3.0 mod -2.5.
    double resultUsingModuloFunction = computeModulo( 0.0, 0.0 );

    if ( fabs( resultUsingModuloFunction - 0.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeModulo function does not "
             << "function correctly, as the computed value: "
             << resultUsingModuloFunction
             << " does not match the expected value: " << 0.0 << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    resultUsingModuloFunction = computeModulo( 2.0, 0.0 );

    if ( fabs( resultUsingModuloFunction - 2.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeModulo function does not "
             << "function correctly, as the computed value: "
             << resultUsingModuloFunction
             << " does not match the expected value: " << 2.0 << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    resultUsingModuloFunction = computeModulo( 2.0, 2.0 );

    if ( fabs( resultUsingModuloFunction - 0.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeModulo function does not "
             << "function correctly, as the computed value: "
             << resultUsingModuloFunction
             << " does not match the expected value: " << 0.0 << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    resultUsingModuloFunction = computeModulo( 3.0, 2.5 );

    if ( fabs( resultUsingModuloFunction - 0.5 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeModulo function does not "
             << "function correctly, as the computed value: "
             << resultUsingModuloFunction
             << " does not match the expected value: " << -0.5 << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    resultUsingModuloFunction = computeModulo( 3.0, -2.5 );

    if ( fabs( resultUsingModuloFunction + 2.0 ) > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The computeModulo function does not "
             << "function correctly, as the computed value: "
             << resultUsingModuloFunction
             << " does not match the expected value: " << -2.0 << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test conversion from cylindrical to Cartesian coordinates,
    // z value left unaffected.
    // Test 15: Test conversion of ( 2.0, 0.0 ).
    // Test 16: Test conversion of ( 2.0, pi ).
    // Test 17: Test conversion of ( 2.0, -2pi ).
    // Test 18: Test conversion of ( 2.0, 225 deg ).
    // Test 19: Test conversion of ( 2.0, -225 deg ).
    Eigen::VectorXd cartesianCoordinates( 2 );

    convertCylindricalToCartesian( 2.0, 0.0, cartesianCoordinates );

    if ( fabs( cartesianCoordinates( 0 ) - 2.0 ) / 2.0 > 1.0e-15 ||
         fabs( cartesianCoordinates( 1 ) - 0.0 ) > 1.0e-15 )
    {
        cerr << "The convertCylindricalToCartesian, no z, function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates( 0 ) << ", " << cartesianCoordinates( 1 )
             << " ) do not match the expected coordinates: ( 2.0, 0.0 )"
             << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertCylindricalToCartesian( 2.0, M_PI, cartesianCoordinates );

    if ( fabs( cartesianCoordinates( 0 ) + 2.0 ) > 1.0e-15 ||
         fabs( cartesianCoordinates( 1 ) - 0.0 ) > 1.0e-15 )
    {
        cerr << "The convertCylindricalToCartesian, no z, function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates( 0 ) << ", " << cartesianCoordinates( 1 )
             << " ) do not match the expected coordinates: ( -2.0, 0.0 )"
             << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertCylindricalToCartesian( 2.0, -2.0*M_PI, cartesianCoordinates );

    if ( fabs( cartesianCoordinates( 0 ) - 2.0 ) > 1.0e-15 ||
         fabs( cartesianCoordinates( 1 ) - 0.0 ) > 1.0e-15 )
    {
        cerr << "The convertCylindricalToCartesian, no z, function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates( 0 ) << ", " << cartesianCoordinates( 1 )
             << " ) do not match the expected coordinates: ( 2.0, 0.0 )"
             << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertCylindricalToCartesian( 2.0, 225.0 / 180.0 * M_PI, cartesianCoordinates );

    if ( fabs( cartesianCoordinates( 0 ) + sqrt( 2.0 ) )
         > std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates( 1 ) + sqrt( 2.0 ))
         > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCylindricalToCartesian, no z, function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates( 0 ) << ", " << cartesianCoordinates( 1 )
             << " ) do not match the expected coordinates: ( "
             << -sqrt( 2.0 ) << ", " << -sqrt( 2.0 ) << " )" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertCylindricalToCartesian( 2.0, -225.0 / 180.0 * M_PI, cartesianCoordinates );

    if ( fabs( cartesianCoordinates( 0 ) + sqrt( 2.0 ) ) >
         std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates( 1 ) - sqrt( 2.0 )) >
         std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCylindricalToCartesian, no z, function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates( 0 ) << ", " << cartesianCoordinates( 1 )
             << " ) do not match the expected coordinates: ( "
             << -sqrt( 2.0 ) << ", " << sqrt( 2.0 ) << " )" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test conversion from spherical to Cartesian coordinates.
    // Test 20: Test conversion of: ( 0.0, 0.0, 0.0 ).
    // Test 21: Test conversion of: ( 2.0, 225, 225 ).
    // Test 22: Test conversion of: ( 2.0, -225, -225 ).
    // Test 23: Test conversion of: ( 2.0, 180, 180 ).
    Eigen::VectorXd cartesianCoordinates3( 3 );

    convertSphericalToCartesian( 0.0, 0.0, 0.0, cartesianCoordinates3 );

    if ( fabs( cartesianCoordinates3( 0 ) + 0.0 ) >
         std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 1 ) - 0.0 ) >
         std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 2 ) - 0.0 ) >
         std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCylindricalToCartesian, function does not "
             << "function correctly. (test1)" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertSphericalToCartesian( 2.0, 225.0 / 180.0 * M_PI,
                                 225.0 / 180.0 * M_PI, cartesianCoordinates3 );

    if ( fabs( cartesianCoordinates3( 0 ) - 1.0 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 1 ) - 1.0 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 2 ) + sqrt( 2.0 ) ) >
         std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCylindricalToCartesian function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates3( 0 ) << ", " << cartesianCoordinates3( 1 )
             << " , " << cartesianCoordinates3( 2 ) << " ) do not match the "
             << "expected coordinates: ( " << 1.0 << ", " << 1.0 << ", "
             << -sqrt( 2.0 ) << " )" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertSphericalToCartesian( 2.0, -225.0 / 180.0 * M_PI, -225.0 / 180.0 * M_PI,
                                 cartesianCoordinates3 );

    if ( fabs( cartesianCoordinates3( 0 ) + 1.0 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 1 ) - 1.0 ) > std::numeric_limits< double >::epsilon( ) ||
         fabs( cartesianCoordinates3( 2 ) + sqrt( 2.0 ) ) >
         std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCylindricalToCartesian function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates3( 0 ) << ", " << cartesianCoordinates3( 1 )
             << " , " << cartesianCoordinates3( 2 ) << " ) do not match the "
             << "expected coordinates: ( " << -1.0 << ", " << 1.0 << ", "
             << -sqrt( 2.0 ) << " )" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    convertSphericalToCartesian( 2.0, 180.0 / 180.0 * M_PI,
                                 180.0 / 180.0 * M_PI, cartesianCoordinates3 );

    if ( fabs( cartesianCoordinates3( 0 ) - 0.0 ) > 1.0e-15 ||
         fabs( cartesianCoordinates3( 1 ) - 0.0 ) > 1.0e-15 ||
         fabs( cartesianCoordinates3( 2 ) + 2.0 ) / 2.0 > 1.0e-15 )
    {
        cerr << "The convertCylindricalToCartesian function does not "
             << "function correctly, as the computed coordinates: ( "
             << cartesianCoordinates3( 0 ) << ", " << cartesianCoordinates3( 1 )
             << " , " << cartesianCoordinates3( 2 ) << " ) do not match the "
             << "expected coordinates: ( " << 0.0 << ", " << 0.0 << ", "
             << -2.0 << " )" << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test conversion from Cartesian to spherical coordinates.
    // Test 24: Test conversion of: ( 0.0, 0.0, 0.0 ).
    // Test 25: Test conversion of: ( 2.0, 3.5, -4.1 ).
    // Test 26: Test conversion of: ( 5.2, -6.3, 0.0 ).
    // Test 27: Test conversion of: ( 0.0, 12.2, -0.9 ).

    // Cartesian coordinates.
    Eigen::VectorXd cartesianCoordinatesTest24_( 3 );
    cartesianCoordinatesTest24_.setZero( 3 );

    Eigen::VectorXd cartesianCoordinatesTest25_( 3 );
    cartesianCoordinatesTest25_( 0 ) = 2.0;
    cartesianCoordinatesTest25_( 1 ) = 3.5;
    cartesianCoordinatesTest25_( 2 ) = -4.1;

    Eigen::VectorXd cartesianCoordinatesTest26_( 3 );
    cartesianCoordinatesTest26_( 0 ) = 5.2;
    cartesianCoordinatesTest26_( 1 ) = -6.3;
    cartesianCoordinatesTest26_( 2 ) = 0.0;

    Eigen::VectorXd cartesianCoordinatesTest27_( 3 );
    cartesianCoordinatesTest27_( 0 ) = 0.0;
    cartesianCoordinatesTest27_( 1 ) = 12.2;
    cartesianCoordinatesTest27_( 2 ) = -0.9;

    // Expected vectors in spherical coordinates.
    Eigen::VectorXd expectedSphericalCoordinatesTest24_( 3 );
    expectedSphericalCoordinatesTest24_.setZero( 3 );

    Eigen::VectorXd expectedSphericalCoordinatesTest25_( 3 );
    expectedSphericalCoordinatesTest25_( 0 )
            = sqrt( pow( cartesianCoordinatesTest25_( 0 ), 2.0 )
                    + pow( cartesianCoordinatesTest25_( 1 ), 2.0 )
                    + pow( cartesianCoordinatesTest25_( 2 ), 2.0 ) );
    expectedSphericalCoordinatesTest25_( 1 )
            = atan2( cartesianCoordinatesTest25_( 1 ),
                     cartesianCoordinatesTest25_( 0 ) );
    expectedSphericalCoordinatesTest25_( 2 )
            = acos( cartesianCoordinatesTest25_( 2 ) /
                     expectedSphericalCoordinatesTest25_( 0 ) );

    Eigen::VectorXd expectedSphericalCoordinatesTest26_( 3 );
    expectedSphericalCoordinatesTest26_( 0 )
            = sqrt( pow( cartesianCoordinatesTest26_( 0 ), 2.0 )
                    + pow( cartesianCoordinatesTest26_( 1 ), 2.0 )
                    + pow( cartesianCoordinatesTest26_( 2 ), 2.0 ) );
    expectedSphericalCoordinatesTest26_( 1 )
            = atan2( cartesianCoordinatesTest26_( 1 ),
                     cartesianCoordinatesTest26_( 0 ) );
    expectedSphericalCoordinatesTest26_( 2 )
            = acos( cartesianCoordinatesTest26_( 2 ) /
                     expectedSphericalCoordinatesTest26_( 0 ) );

    Eigen::VectorXd expectedSphericalCoordinatesTest27_( 3 );
    expectedSphericalCoordinatesTest27_( 0 )
            = sqrt( pow( cartesianCoordinatesTest27_( 0 ), 2.0 )
                    + pow( cartesianCoordinatesTest27_( 1 ), 2.0 )
                    + pow( cartesianCoordinatesTest27_( 2 ), 2.0 ) );
    expectedSphericalCoordinatesTest27_( 1 )
            = atan2( cartesianCoordinatesTest27_( 1 ),
                     cartesianCoordinatesTest27_( 0 ) );
    expectedSphericalCoordinatesTest27_( 2 )
            = acos( cartesianCoordinatesTest27_( 2 ) /
                     expectedSphericalCoordinatesTest27_( 0 ) );

    // Result vector in spherical coordinates.
    Eigen::VectorXd sphericalCoordinates_( 3 );

    // Test 24: Test conversion of: ( 0.0, 0.0, 0.0 ).

    // Declare absolute and relative differences.
    double absoluteDifference_;
    double relativeDifference_;

    // Compute conversions.
    convertCartesianToSpherical( cartesianCoordinatesTest24_,
                                 sphericalCoordinates_ );

    // Compute absolute and relative differences.
    absoluteDifference_ = fabs( sphericalCoordinates_.norm( )
            - expectedSphericalCoordinatesTest24_.norm( ) );

    relativeDifference_ = absoluteDifference_
            / expectedSphericalCoordinatesTest24_.norm( );

    // Check if relative error is too large.
    if ( relativeDifference_  > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCartesianToSpherical, function does not "
             << "function correctly. ( Test 24 )." << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test 25: Test conversion of: ( 2.0, 3.5, -4.1 ).

    // Compute conversions.
    convertCartesianToSpherical( cartesianCoordinatesTest25_,
                                 sphericalCoordinates_ );

    // Compute absolute and relative differences.
    absoluteDifference_ = fabs( sphericalCoordinates_.norm( )
                               - expectedSphericalCoordinatesTest25_.norm( ) );

    relativeDifference_ = absoluteDifference_
            / expectedSphericalCoordinatesTest25_.norm( );

    if ( relativeDifference_ > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCartesianToSpherical, function does not "
             << "function correctly. ( Test 25 )." << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test 26: Test conversion of: ( 5.2, -6.3, 0.0 ).

    // Compute conversion.
    convertCartesianToSpherical( cartesianCoordinatesTest26_, sphericalCoordinates_ );

    // Compute absolute and relative differences.
    absoluteDifference_ = fabs( sphericalCoordinates_.norm( )
                               - expectedSphericalCoordinatesTest26_.norm( ) );

    relativeDifference_ = absoluteDifference_ / expectedSphericalCoordinatesTest26_.norm( );

    if ( relativeDifference_ > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCartesianToSpherical, function does not "
             << "function correctly. ( Test 26 )." << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Test 27: Test conversion of: ( 0.0, 12.2, -0.9 ).

    // Compute conversion.
    convertCartesianToSpherical( cartesianCoordinatesTest27_, sphericalCoordinates_ );

    // Compute absolute and relative differences.
    absoluteDifference_ = fabs( sphericalCoordinates_.norm( )
                               - expectedSphericalCoordinatesTest27_.norm( ) );

    relativeDifference_ = absoluteDifference_
            / expectedSphericalCoordinatesTest27_.norm( );

    if ( relativeDifference_ > std::numeric_limits< double >::epsilon( ) )
    {
        cerr << "The convertCartesianToSpherical, function does not "
             << "function correctly. ( Test 27 )." << endl;
        isBasicMathematicsFunctionsErroneous = true;
    }

    // Return test result.
    // If test is successful return false; if test fails, return true.
    if ( isBasicMathematicsFunctionsErroneous )
    {
        cerr << "testBasicMathematicsFunctions failed!" << endl;
    }

    return isBasicMathematicsFunctionsErroneous;
}

// End of file.
