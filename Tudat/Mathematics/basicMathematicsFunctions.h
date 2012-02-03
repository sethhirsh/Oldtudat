/*! \file basicMathematicsFunctions.h
 *    Header file that defines the basicMathematicsFunctions namespace,
 *    containing all basic functions contained in Tudat.
 *
 *    Path              : /Mathematics/
 *    Version           : 14
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

 *    Date created      : 3 September, 2010
 *    Last modified     : 2 February, 2012
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
 *      100903    K. Kumar          File header and footer added.
 *      100916    L. Abdulkadir     File checked.
 *      100929    K. Kumar          Checked code by D. Dirkx added.
 *      101110    K. Kumar          Added raiseToIntegerExponent() function.
 *      102410    D. Dirkx          Minor comment changes during code check.
 *      101213    K. Kumar          Modified raiseToIntegerExponent() function;
 *                                  renamed raiseToIntegerPower().
 *                                  Added computeAbsoluteValue() functions.
 *      110111    J. Melman         Added computeModulo() function.
 *      110202    K. Kumar          Added overload for State* for computeLinearInterpolation().
 *      110411    K. Kumar          Added convertCartesianToSpherical() function.
 *      110707    K. Kumar          Added computeSampleMean(), computeSampleVariance() functions.
 *      110810    J. Leloux         Corrected doxygen documentation (equations).
 *      110824    J. Leloux         Corrected doxygen documentation.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120202    K. Kumar          Moved functions to new Statistics and Interpolators
 *                                  sub-directories.
 */

#ifndef BASICMATHEMATICSFUNCTIONS_H
#define BASICMATHEMATICSFUNCTIONS_H

// Include statements.
#include <boost/random/mersenne_twister.hpp>
#include <ctime>
#include <Eigen/Core>
#include <map>
#include "Tudat/Astrodynamics/States/state.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Mathematics namespace.
/*!
 *  Mathematics namespace.
 */
namespace mathematics
{

//! Random number generator typedef.
/*!
 * Random number generator typedef. This can be modified to any other Boost random number
 * generator type
 * (http://www.boost.org/doc/libs/1_47_0/doc/html/boost_random/reference.html.
 */
typedef boost::mt19937 globalRandomNumberGeneratorType;

//! Get global random number generator.
/*!
 * Returns global random number generator. The default seed is set to the current time.
 * \return Global random number generator.
 */
globalRandomNumberGeneratorType& getGlobalRandomNumberGenerator( );

//! Convert spherical to cartesian coordinates.
/*!
* Function to convert spherical to cartesian coordinates.
* Schematic representation can be found on, e.g.,
* http://mathworld.wolfram.com/SphericalCoordinates.html.
* The transformation equations are the following, with \f$ r \f$ the radius,
* \f$ \theta \f$ the azimuth angle and \f$ \phi \f$ the zenith angle:
* \f{eqnarray*}{
*      x &=& r\cos\theta\sin\phi \\
*      y &=& r\sin\theta\sin\phi \\
*      z &=& r\cos\phi \\
* \f}
*/
void convertSphericalToCartesian( double radius, double azimuthAngle, double zenithAngle,
                                  Eigen::VectorXd& cartesianCoordinates );

//! Convert cartesian to spherical coordinates.
/*!
* Converts cartesian to spherical coordinates.
* Schematic representation can be found on, e.g.,
* http://mathworld.wolfram.com/SphericalCoordinates.html.
* The transformation equations are the following, with \f$ r \f$ the radius,
* \f$ \theta \f$ the azimuth angle and \f$ \phi \f$ the azimuth angle:
* \f{eqnarray*}{
*      r &=& \sqrt{ x^{ 2 } + y^{ 2 } + z^{ 2 } } \\
*      \theta &=& \arctan\frac{ y }{ x } \\
*      \phi &=& \arccos\frac{ z }{ r } \\
* \f}
*/
void convertCartesianToSpherical( const Eigen::VectorXd& cartesianCoordinates,
                                  Eigen::VectorXd& sphericalCoordinates );

//! Convert cylindrical to cartesian coordinates, z value unaffected.
/*!
* Function to convert cylindrical to cartesian coordinates.
* Schematic representation can be found on, e.g.,
* http://mathworld.wolfram.com/CylindricalCoordinates.html.
* The transformation equations are the following, with \f$ r \f$  the radius and
* \f$ \theta \f$ the azimuth angle:
* \f{eqnarray*}{
*      x &=& r\cos\theta \\
*      y &=& r\sin\theta \\
*      z &=& z \\
* \f}
* Since the value of z is left unaffected by this transformation,
* it is not set or changed by this function.
*/
void convertCylindricalToCartesian( double radius, double azimuthAngle,
                                    Eigen::VectorXd& cartesianCoordinates );

//! Compute modulo of double.
/*!
 * Computes the remainder of division of one number by another.
 * The remainder is in the range [ 0, divisor ].
 * \param dividend Number to be divided.
 * \param divisor Number that is divided by.
 * \return Remainder of division of dividend by divisor.
 */
double computeModulo( double dividend, double divisor );

}

}

#endif // BASICMATHEMATICSFUNCTIONS_H

// End of file.
