/*! \file gravitationalForceModel.h
 *    Header file that defines the gravitational force model included in Tudat.
 *
 *    Path              : /Astrodynamics/ForceModels/
 *    Version           : 9
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : F.M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Date created      : 16 September, 2010
 *    Last modified     : 15 August, 2011
 *
 *    References
 *
 *    Notes
 *      The mass and state of the body on which the force acts should be
 *      transferred to the Body class.
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
 *      100916    K. Kumar          File created.
 *      100916    K. Kumar          Filename modified.
 *      100929    D. Dirkx          File checked.
 *      100929    K. Kumar          Minor corrections to include statements and comments.
 *      110113    K. Kumar          Changed setBody( ) argument to pointer; added pointer to
 *                                  GravityFieldModel.
 *      110119    K. Kumar          Changed computeStateDerivatives( ) to computeForce( ).
 *      110202    K. Kumar          Updated code to make use of the State and
 *                                  CartesianPositionElements classes.
 *      110810    J. Leloux         Corrected doxygen documentation.
 *      110815    K. Kumar          Changed filename and class name; changed computeForce( )
 *                                  function and added setMass( ) function.
 */

// Include statements.
#include <iostream>
#include "Tudat/Astrodynamics/Gravitation/gravitationalForceModel.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Compute force due to gravity field.
void GravitationalForceModel::computeForce( State* pointerToState, double time = 0.0 )
{
    CartesianPositionElements cartesianPositionElements_;
    cartesianPositionElements_.state = pointerToState->state.segment( 0, 3 );
    force_ = pointerToGravityFieldModel_->getGradientOfPotential( &cartesianPositionElements_ )
            * pointerToBodySubjectToForce_->getMass( );
}


}


// End of file.
