/*! \file keplerPropagator.h
 *    Header file that defines the kepler propagator class included in Tudat.
 *
 *    Path              : /Astrodynamics/Propagators/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : E. Iorfida
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : elisabetta_iorfida@yahoo.it
 *
 *    Date created      : 3 February, 2011
 *    Last modified     : 20 September, 2011
 *
 *    References
 *
 *    Notes
 *      The code at present does not work for near-parabolic orbits
 *      ( 0.8 < eccentricity < 1.2 ). In future, this neeeds to be included
 *      and perhaps a universal method to solve Kepler's equation needs to be
 *      employed. Presently, the code will output an error if the eccentricity
 *      of the orbit to be propagated lies within this range.
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
 *      110203    K. Kumar          File created.
 *      110207    E. Iorfida        Minor changes.
 *      110214    K. Kumar          Updated code to use orbital element conversion functions.
 *      110920    K. Kumar          Corrected simple errors outlined by M. Persson.
 */

#ifndef GENERALIZEDFORCEMODEL_H
#define GENERALIZEDFORCEMODEL_H

namespace tudat
{

//! Base class for all generalized forces.
/*!
 *  Base class for all generalized forces (i.e. forces, moments, etc.). Base class only has
 *  the interface for retrieving the generlaized force, which can be wrapped by a more
 *  specific function (for instance getMoment) in derived classed. Functions to calculate
 *  generalized forces have different interfaces for different derived classes and therefore
 *  has no base class interface.
 */
template< typename GeneralizedForceType, int sizeOfGeneralizedForce >
class GeneralizedForceModel
{
public:

    //! Virtual destructor.
    /*!
     *  Virtual destructor.
     */
    virtual ~GeneralizedForceModel( ){ }

    //! Function to get generalized force.
    /*!
     *  Function to get generalized force. Needs to be implemented in all derived classes
     *  to return the actual type of generalized force that is modelled.
     */
    virtual GeneralizedForceType getGeneralizedForce( ) = 0;

protected:

private:

};

}

#endif // GENERALIZEDFORCEMODEL_H
