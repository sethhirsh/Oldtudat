/*! \file momentDueToForceModel.cpp
 *    This header file contains the moment model base class included in Tudat.
 *
 *    Path              : /Astrodynamics/BasicAstrodynamics/
 *    Version           : 1
 *    Check status      : Checked
 *
 *    Author            : D.Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : D.Dirkx@tudelft.nl
 *
 *    Checker           :
 *    Affiliation       :
 *    E-mail address    :
 *
 *    Date created      : 04 February, 2012
 *    Last modified     : 04 February, 2012
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
 *      120204    D. Dirkx          First version of file; Split MomentModel class into
 *                                  PureMomentModel and  this class.
 */

#include "Tudat/Astrodynamics/BasicAstrodynamics/momentDueToForceModel.h"

namespace tudat
{

void MomentDueToForceModel::computeMoment( Eigen::Vector3d force, Eigen::Vector3d arm )
{
    momentDueToForce_ = force.cross( arm );
}

}
