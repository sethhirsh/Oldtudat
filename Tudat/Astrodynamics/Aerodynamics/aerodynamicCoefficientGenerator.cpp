/*!   \file aerodynamicCoefficientGenerator.cpp
 *    This file contains the definition of the aerodynamic coefficient generator base class.
 *
 *    Path              : /Astrodynamics/ForceModels/
 *    Version           : 6
 *    Check status      : Checked
 *
 *    Author            : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : B.Romgens@student.tudelft.nl
 *
 *    Date created      : 25 November, 2010
 *    Last modified     : 15 June, 2011
 *
 *    References
 *      Gentry, A., Smyth, D., and Oliver, W. . The Mark IV Supersonic-Hypersonic
 *        Arbitrary Body Program, Volume II - Program Formulation, Douglas Aircraft
 *        Aircraft Company, 1973.
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
 *      102511    D. Dirkx          First version of file.
 *      110501    D. Dirkx          Added more comments.
 *      112701    D. Dirkx          Finalized for code check.
 *      110131    B. Romgens        Minor modifications during code check.
 *      110204    D. Dirkx          Finalized code.
 *      110615    F.M. Engelen      Made a child of Coefficient Database. Moved aerodynamic
 *                                  reference quatities to the parent class.
 */

// Include statements.
#include <string>
#include "Tudat/Astrodynamics/Aerodynamics/aerodynamicCoefficientGenerator.h"

//! Tudat library namespace.
namespace tudat
{

//! Set the number of independent variables.
void AerodynamicCoefficientGenerator::setNumberOfIndependentVariables(
    int numberOfVariables )
{
    // Set number of variables.
    numberOfIndependentVariables_ = numberOfVariables;

    // Allocate memory for data points.
    numberOfPointsPerIndependentVariables_.resize( boost::extents[ numberOfVariables ] );
    dataPointsOfIndependentVariables_.resize( numberOfVariables );
}

//! Set the number of points for Mach number.
void AerodynamicCoefficientGenerator::setNumberOfMachPoints( int numberOfMachPoints )
{
    // Set value of number of mach points.
    numberOfPointsPerIndependentVariables_ [ machIndex_ ] = numberOfMachPoints;

    // Allocate memory for data points.
    dataPointsOfIndependentVariables_[ machIndex_ ].resize( boost::extents[ numberOfMachPoints ] );
}

//! Set the number of points for angle of attack.
void AerodynamicCoefficientGenerator::setNumberOfAngleOfAttackPoints(
        int numberOfAngleOfAttackPoints )
{
    // Set value of number of angle of attack points.
    numberOfPointsPerIndependentVariables_ [ angleOfAttackIndex_ ] = numberOfAngleOfAttackPoints;

    // Allocate memory for data points.
    dataPointsOfIndependentVariables_[ angleOfAttackIndex_ ].resize( boost::extents[ numberOfAngleOfAttackPoints ] );
}

//! Set the number of points for angle of sideslip.
void AerodynamicCoefficientGenerator::setNumberOfAngleOfSideslipPoints(
        int numberOfAngleOfSideslipPoints )
{
    // Set value of number of angle of sideslip points.
    numberOfPointsPerIndependentVariables_ [ angleOfSideslipIndex_ ]
            = numberOfAngleOfSideslipPoints;

    // Allocate memory for data points.
    dataPointsOfIndependentVariables_[ angleOfSideslipIndex_ ].resize( boost::extents[ numberOfAngleOfSideslipPoints ]);
}

//! Set the number of points for the Reynolds Number.
void AerodynamicCoefficientGenerator::setNumberOfReynoldsNumberPoints(
        int numberOfReynoldsNumberPoints )
{
    // Set value of number of Reynolds Number points.
    numberOfPointsPerIndependentVariables_ [ reynoldsNumberIndex_ ] = numberOfReynoldsNumberPoints;

    // Allocate memory for data points.
    dataPointsOfIndependentVariables_[ reynoldsNumberIndex_ ].resize( boost::extents [ numberOfReynoldsNumberPoints ] );
}

//! Convert independent variable indices to list index in vehicleCoefficients_.
int AerodynamicCoefficientGenerator::variableIndicesToListIndex( vector< int > independentVariableIndices )
{
    int i ,j;

    // Declare requested indexs and initialize to zero.
    int coefficientsIndex_ = 0;

    // Declare variable of contribution of single index.
    int singleStepContribution_;

    // Iterate over all indices and add to coefficientsIndex.
    for ( i = 0; i < numberOfIndependentVariables_; i++ )
    {
        // Determine single step contribution.
        singleStepContribution_ = 1;

        for ( j = i + 1; j < numberOfIndependentVariables_; j++ )
        {
            singleStepContribution_ *= numberOfPointsPerIndependentVariables_[ j ];
        }

        // Add contribution to requested index.
        coefficientsIndex_ += singleStepContribution_ * independentVariableIndices[ i ];
    }

    return coefficientsIndex_;
}

}

// End of file.
