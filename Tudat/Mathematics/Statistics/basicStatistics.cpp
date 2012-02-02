/*! \file linearAlgebra.cpp
 *    This source file contains basic statistics functions.
 *
 *    Path              : /Mathematics/Statistics/
 *    Version           : 5
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
 *      100930    D. Dirkx          Modified to comply with Tudat standards
 *      100930    J. Melman         Implemented namespace, minor comment
 *                                  changes.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120202    K. Kumar          Moved functions from linearAlgebra.cpp.
 */

// Include statements.
#include <cmath>
#include "Mathematics/Statistics/basicStatistics.h"

//! Compute average of the components of a vector.
double tudat::statistics::computeAverageOfVectorComponents( const Eigen::VectorXd& vectorOfData )
{ return vectorOfData.sum( ) / vectorOfData.rows( ); }

//! Compute standard deviation of the components of a vector.
double computeStandardDeviationOfVectorComponents( const Eigen::VectorXd& vectorOfData )
{
    // Compute average of components.
    double averageOfComponents = tudat::statistics::
            computeAverageOfVectorComponents( vectorOfData );

    // Declare variance of components.
    double varianceOfComponents = 0.0;

    // Compute variance of components.
    for ( int i = 0 ; i < vectorOfData.rows( ) ; i++ )
    {
        varianceOfComponents += std::pow( ( vectorOfData( i ) - averageOfComponents ), 2.0 );
    }

    varianceOfComponents /= static_cast< double >( vectorOfData.rows( ) - 1 );

    // Return square root of variance ( = standard deviation ).
    return std::sqrt( varianceOfComponents );
}

// End of file.
