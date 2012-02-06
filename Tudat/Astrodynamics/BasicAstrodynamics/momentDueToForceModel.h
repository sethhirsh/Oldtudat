/*! \file momentDueToForceModel.h
 *    This header file contains the moment model base class included in Tudat.
 *
 *    Path              : /Astrodynamics/BasicAstrodynamics/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Checker           : F. M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Checker           : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Date created      : 10 May, 2011
 *    Last modified     : 24 August, 2011
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
 *      110510    F.M. Engelen      First creation of code.
 *      110810    K. Kumar          Minor corrections; changed function names and removed redundant
 *                                  functions.
 *      110824    J. Leloux         Corrected doxygen documentation.
 *      120204    D. Dirkx          Split MomentModel class into PureMomentModel and  this class.
 */

#ifndef MOMENTDUETOFORCEMODEL_H
#define MOMENTDUETOFORCEMODEL_H

#include "Tudat/Astrodynamics/BasicAstrodynamics/generalizedForceModel.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/forceModel.h"

namespace tudat
{

//! Base class for all generalized forces.
/*!
 *  Class for moment due to an resultant force. Interface for computing moment takes arm and force,
 *  so that only the cross product is calculated. An additional interface with a ForceModel for
 *  instance can be added in the future.
 */
class MomentDueToForceModel : public GeneralizedForceModel< Eigen::Vector3d, 3 >
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    MomentDueToForceModel( );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~MomentDueToForceModel( ) { }

    //! Get moment.
    /*!
     * Returns the moment.
     * \return Moment.
     */
    Eigen::Vector3d getMomentDueToForce( ) { return momentDueToForce_; }

    //! Get moment.
    /*!
     * Returns the moment.
     * \return Moment.
     */
    virtual Eigen::Vector3d getGeneralizedForce(  ) { return getMomentDueToForce( ); }

     //! Compute moment.
     /*!
      * Computes the moment.
      */
    virtual void computeMoment( Eigen::Vector3d force, Eigen::Vector3d arm );

protected:

private:

    //! Moment.
    /*!
     * Moment.
     */
    Eigen::Vector3d momentDueToForce_;
};

}

#endif // MOMENTDUETOFORCEMODEL_H
