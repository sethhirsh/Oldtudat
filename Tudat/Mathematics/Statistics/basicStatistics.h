/*! \file basicStatistics.h
 *    This header file contains basic statistics functions.
 *
 *    Path              : /Mathematics/Statistics/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Date created      : 7 August, 2009
 *    Last modified     : 2 February, 2012
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
 *      090807    J. Melman         First creation of code.
 *      100930    D. Dirkx          Modified to comply with Tudat standards.
 *      100930    J. Melman         Implemented namespace, minor comment changes.
 *      120202    K. Kumar          Moved functions from linearAlgebra.h.
 */

#ifndef TUDAT_BASICSTATISTICS_H
#define TUDAT_BASICSTATISTICS_H

// Include statements.
#include <Eigen/Core>

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Statistics namespace.
/*!
 * Statistics namespace.
 */
namespace statistics
{

//! Compute average of the components of a vector.
/*!
 * Computes the average (arithmetic mean) of the components of a vector.
 * \param vectorOfData Vector containing data to be averaged.
 * \return Average of data in vector.
 */
double computeAverageOfVectorComponents( const Eigen::VectorXd& vectorOfData );

//! Compute standard deviation of the components of a vector.
/*!
 * Computes the standard deviation of the components of a vector.
 * \param vectorOfData Vector containing data to be averaged.
 * \return Standard deviation of data in vector.
 */
double computeStandardDeviationOfVectorComponents( const Eigen::VectorXd& vectorOfData );

} // Namespace statistics

} // Namespace tudat.

#endif // TUDAT_BASICSTATISTICS_H

// End of file.
