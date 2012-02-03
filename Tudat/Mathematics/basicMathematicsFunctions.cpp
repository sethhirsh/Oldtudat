/*! \file basicMathematicsFunctions.cpp
 *    Source file that defines the basicMathematicsFunctions namespace,
 *    containing all basic functions contained in Tudat.
 *
 *    Path              : /Mathematics/
 *    Version           : 13
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Author/Checker    : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Author            : J.C.P Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Checker           : L. Abdulkadir
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : L.Abdulkadir@student.tudelft.nl
 *
 *    Date created      : 3 September, 2010
 *    Last modified     : 5 September, 2011
 *
 *    References
 *      Press W.H., et al. Numerical Recipes in C++: The Art of
 *          Scientific Computing. Cambridge University Press, February 2002.
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
 *      100903    K. Kumar          File created.
 *      100916    L. Abdulkadir     File checked.
 *      100929    K. Kumar          Checked code by D. Dirkx added.
 *      101110    K. Kumar          Added raiseToExponentPower() function.
 *      102410    D. Dirkx          Minor comment changes as code check.
 *      101213    K. Kumar          Bugfix raiseToIntegerExponent(); renamed raiseToIntegerPower().
 *                                  Added computeAbsoluteValue() functions.
 *      110202    K. Kumar          Added overload for State* for computeLinearInterpolation().
 *      110111    J. Melman         Added computeModulo() function.
 *      110411    K. Kumar          Added convertCartesianToSpherical() function.
 *      110606    J. Melman         Removed possible singularity from
 *                                  convertCartesianToSpherical.
 *      110707    K. Kumar          Added computeSampleMean(), computeSampleVariance() functions.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120202    K. Kumar          Moved functions to new Statistics and Interpolators
 *                                  sub-directories.
 */

// Include statements.
#include <cmath>
#include <iostream>
#include <limits>
#include "Tudat/Mathematics/basicMathematicsFunctions.h"

//! Tudat library namespace.
namespace tudat
{

//! Mathematics namespace.
namespace mathematics
{

// Using declarations.
using std::map;
using std::pow;

//! Get global random number generator.
globalRandomNumberGeneratorType& getGlobalRandomNumberGenerator( )
{
  static globalRandomNumberGeneratorType globalRandomNumberGenerator(
              static_cast< unsigned int >( std::time( 0 ) ) );
  return globalRandomNumberGenerator;
}

//! Convert spherical to cartesian coordinates.
void convertSphericalToCartesian( double radius, double azimuthAngle, double zenithAngle,
                                  Eigen::VectorXd& cartesianCoordinates )
{
    // Declaring sine and cosine which have multiple usages to save computation time.
    double cosineOfAzimuthAngle = std::cos( azimuthAngle );
    double sineOfZenithAngle = std::sin( zenithAngle );

    // Perform transformation.
    cartesianCoordinates( 0 ) = radius * cosineOfAzimuthAngle * sineOfZenithAngle;
    cartesianCoordinates( 1 ) = radius * std::sin( azimuthAngle ) * sineOfZenithAngle;
    cartesianCoordinates( 2 ) = radius * std::cos( zenithAngle );
}

//! Convert cartesian to spherical coordinates.
void convertCartesianToSpherical( const Eigen::VectorXd& cartesianCoordinates,
                                  Eigen::VectorXd& sphericalCoordinates )
{
    // Compute transformation of Cartesian coordinates to spherical coordinates.
    sphericalCoordinates( 0 ) = cartesianCoordinates.norm( );

    // Check if coordinates are at origin.
    if ( sphericalCoordinates( 0 ) < std::numeric_limits< double >::epsilon( ) )
    {
        sphericalCoordinates( 1 ) = 0.0;
        sphericalCoordinates( 2 ) = 0.0;
    }

    // Else compute coordinates using trigonometric relationships.
    else
    {
        sphericalCoordinates( 1 ) = std::atan2( cartesianCoordinates( 1 ),
                                                cartesianCoordinates( 0 ) );
        sphericalCoordinates( 2 ) = std::acos( cartesianCoordinates( 2 )
                                               / sphericalCoordinates( 0 ) );
    }
}

//! Convert cylindrical to cartesian coordinates, z value left unaffected.
void convertCylindricalToCartesian( double radius, double azimuthAngle,
                                    Eigen::VectorXd& cartesianCoordinates )
{
    // Perform transformation, z value should be set outside function.
    cartesianCoordinates( 0 ) = radius * std::cos( azimuthAngle );
    cartesianCoordinates( 1 ) = radius * std::sin( azimuthAngle );
}

//! Compute modulo of double.
double computeModulo( double dividend, double divisor )
{ return dividend - divisor * floor( dividend / divisor ); }

}

}

// End of file.
