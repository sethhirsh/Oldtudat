/*! \file body.h
 *    This file contains a class that describes bodies, both celestial bodies
 *    and vehicles, with all their characteristics.
 *
 *    Path              : /Astrodynamics/Bodies/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Checker           : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : F.M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Date created      : 20 September, 2010
 *    Last modified     : 15 August, 2011
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
 *      100920    J. Melman         First creation of code.
 *      100929    K. Kumar          Minor comment changes.
 *      110115    J. Melman         Added set and get shape model functions.
 *      110815    K. Kumar          Added setMass( ) and getMass( ) functions.
 */

#ifndef TUDAT_BODY_H
#define TUDAT_BODY_H

// Include statements.
#include <iostream>

//! Tudat library namespace.
/*!
 * The Tudat Library namespace.
 */
namespace tudat
{

//! Body base class.
/*!
 * Body base class.
 */
class Body
{
public:

    //! Create a body with a given mass.
    /*!
     * \param Mass of the body, default=0.
     */
    Body( double mass = 0) : mass_( mass ) { }

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~Body( ) { }

    //! Set mass of body.
    /*!
     * Sets the mass of the body.
     * \param mass Mass.
     */
    void setMass( double mass ) { mass_ = mass; }

    //! Get mass of body.
    /*!
     * Returns the mass of the body.
     * \return Mass.
     */
    double& getMass( ) { return mass_; }

protected:

private:

    //! Mass.
    /*!
     * Mass of body.
     */
    double mass_;

};

}

#endif // TUDAT_BODY_H

// End of file.
