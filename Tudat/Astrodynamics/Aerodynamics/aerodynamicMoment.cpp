/*! \file aerodynamicMoment.cpp
*    This source file contains the aerodynamic moment model included in Tudat.
*
*    Path              : /Astrodynamics/Aerodynamics/
*    Version           : 2
*    Check status      : Checked
*
*    Checker           : F. M. Engelen
*    Affiliation       : Delft University of Technology
*    E-mail address    : F.M.Engelen@student.tudelft.nl
*
*    Checker           : D. Dirkx
*    Affiliation       : Delft University of Technology
*    E-mail address    : D..Dirkx@tudelft.nl
*
*    Date created      : 17 May, 2011
*    Last modified     : 22 August, 2011
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
*      110617    F.M. Engelen      First creation of code.
*      110822    D.Dirkx           Removed pointer to double member, removed location of center
*                                  of mass, minor changes.
*/

// Macros.
#define TUDAT_UNUSED_PARAMETER( unusedParameter ) { ( void ) unusedParameter; }

// Include statements.
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Tudat/Astrodynamics/Aerodynamics/aerodynamicMoment.h"

//! Tudat library namespace.
namespace tudat
{

//! Compute aerodynamic moment.
void AerodynamicMoment::computeMoment( State* pointerToState, double time )
{

    // Calculate moment.
    moment_ = dynamicPressure_ * pointerToAerodynamicCoefficientInterface_->getReferenceArea( ) *
            pointerToAerodynamicCoefficientInterface_->getReferenceLength( ) *
            pointerToAerodynamicCoefficientInterface_->getCurrentMomentCoefficients( );
}

}

// End of file.
