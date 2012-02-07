/*! \file librationPoint.cpp
 *    Source file that defines the computation of the location of a Lagrange libration point in a
 *    Circular Restricted Three-Body Problem (CRTBP).
 *
 *    Path              : /Astrodynamics/MissionSegments/
 *    Version           : 8
 *    Check status      : Checked
 *
 *    Author            : L. van der Ham
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : L.vanderHam@student.tudelft.nl
 *
 *    Checker           : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Date created      : 7 June, 2011
 *    Last modified     : 27 October, 2011
 *
 *    References
 *      van der Ham, L. TBD.
 *      Mireles James, J.D., "Celestial Mechanics Notes Set 4: The Circular Restricted Three Body
 *          Problem", http://www.math.utexas.edu/users/jjames/celestMech, 2006.
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
 *      110527    L. van der Ham    First creation of code.
 *      110602    L. van der Ham    Made LibrationPoints class and added comments.
 *      110707    L. van der Ham    Make code compatible with Tudat revision 114.
 *      110629    L. van der Ham    Modifications according to comments first code check.
 *      110710    K. Kumar          Removed duplicated code; modified libration point
 *                                  compute-functions; changed filename and class; L1 and L2
 *                                  function coefficient discrepancies spotted.
 *      110712    L. van der Ham    Changed L1, L2 and L3 function coefficients.
 *      110927    L. van der Ham    Reverted to full equations of motion for determination location
 *                                  of colinear libration points.
 *      111027    K. Kumar          Moved 1-line functions to header file.
 */

// Include statements.
#include <cmath>
#include "Tudat/Astrodynamics/Gravitation/gravitationalForceModel.h"
#include "Tudat/Astrodynamics/MissionSegments/librationPoint.h"

//! Tudat library namespace.
namespace tudat
{

// Using declarations.
using std::cerr;
using std::endl;
using std::pow;
using std::sqrt;

//! Compute location of Lagrange libration point.
void LibrationPoint::computeLocationOfLibrationPoint(
    LagrangeLibrationPoints lagrangeLibrationPoint )
{
    // Newton-Raphson method implementation.
    // Set the class that contains the functions needed for Newton-Raphson.
    newtonRaphsonAdaptorForLibrationPoint_.setClass( this );

    // Set functions for Newton-Raphson based on collinear libration point passed as input
    // parameter, or computed locations directly of equilateral libration points.
    switch( lagrangeLibrationPoint )
    {
    case L1:

        // Set mathematical functions for determination of libration point L1.
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFunction(
                    &LibrationPoint::computeL1LocationFunction_ );
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFirstDerivativeFunction(
                    &LibrationPoint::computeL1FirstDerivativeLocationFunction_ );

        // Set the adaptor for Newton-Raphson method.
        pointerToNewtonRaphson_->setNewtonRaphsonAdaptor(
                    &newtonRaphsonAdaptorForLibrationPoint_ );

        // Set initial guess for root of equation of motion for L1.
        pointerToNewtonRaphson_->setInitialGuessOfRoot( 1.0 );

        // Compute root of equation for L1 via Newton-Raphson method.
        pointerToNewtonRaphson_->execute( );

        // Set position vector of L1 in Cartesian elements.
        positionOfLibrationPoint_.setCartesianElementX(
                    pointerToNewtonRaphson_->getComputedRootOfFunction( ) );
        positionOfLibrationPoint_.setCartesianElementY( 0.0 );
        positionOfLibrationPoint_.setCartesianElementZ( 0.0 );

        break;

    case L2:

        // Set mathematical functions for determination of libration point L2.
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFunction(
                    &LibrationPoint::computeL2LocationFunction_ );
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFirstDerivativeFunction(
                    &LibrationPoint::computeL2FirstDerivativeLocationFunction_ );

        // Set the adaptor for Newton-Raphson method.
        pointerToNewtonRaphson_->setNewtonRaphsonAdaptor(
                    &newtonRaphsonAdaptorForLibrationPoint_ );

        // Set initial guess for root of equation of motion in L2.
        pointerToNewtonRaphson_->setInitialGuessOfRoot( 1.0 );

        // Compute root of equation for L2 via Newton-Raphson method.
        pointerToNewtonRaphson_->execute( );

        // Set position vector of L2 in Cartesian elements.
        positionOfLibrationPoint_.setCartesianElementX(
                    pointerToNewtonRaphson_->getComputedRootOfFunction( ) );
        positionOfLibrationPoint_.setCartesianElementY( 0.0 );
        positionOfLibrationPoint_.setCartesianElementZ( 0.0 );

        break;

    case L3:

        // Set mathematical functions for determination of libration point L3.
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFunction(
                    &LibrationPoint::computeL3LocationFunction_ );
        newtonRaphsonAdaptorForLibrationPoint_.setPointerToFirstDerivativeFunction(
                    &LibrationPoint::computeL3FirstDerivativeLocationFunction_ );

        // Set the adaptor for Newton-Raphson method.
        pointerToNewtonRaphson_->setNewtonRaphsonAdaptor(
                    &newtonRaphsonAdaptorForLibrationPoint_ );

        // Set initial guess for root of equation of motion in L3.
        pointerToNewtonRaphson_->setInitialGuessOfRoot( -1.0 );

        // Compute root of equation for L3 via Newton-Raphson method.
        pointerToNewtonRaphson_->execute( );

        // Set position vector of L3 in Cartesian elements.
        positionOfLibrationPoint_.setCartesianElementX(
                    pointerToNewtonRaphson_->getComputedRootOfFunction( ) );
        positionOfLibrationPoint_.setCartesianElementY( 0.0 );
        positionOfLibrationPoint_.setCartesianElementZ( 0.0 );

        break;

    case L4:

        // Set position vector of L4 in Cartesian elements.
        positionOfLibrationPoint_.setCartesianElementX( 0.5 - massParameter_ );
        positionOfLibrationPoint_.setCartesianElementY( 0.5 * sqrt( 3.0 ) );
        positionOfLibrationPoint_.setCartesianElementZ( 0.0 );

        break;

    case L5:

        // Set position vector of L5 in Cartesian elements.
        positionOfLibrationPoint_.setCartesianElementX( 0.5 - massParameter_ );
        positionOfLibrationPoint_.setCartesianElementY( -0.5 * sqrt( 3.0 ) );
        positionOfLibrationPoint_.setCartesianElementZ( 0.0 );

        break;

    default:

        cerr << "The Lagrange libration point requested does not exist." << endl;
    };
}

}

// End of file.
