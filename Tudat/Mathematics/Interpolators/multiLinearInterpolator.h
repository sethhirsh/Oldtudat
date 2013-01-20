/*    Copyright (c) 2010-2013, Delft University of Technology
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without modification, are
 *    permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright notice, this list of
 *        conditions and the following disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *      - Neither the name of the Delft University of Technology nor the names of its contributors
 *        may be used to endorse or promote products derived from this software without specific
 *        prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 *    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *    COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *    OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *                D. Dirkx          File created.
 *      121027    A. Ronse          Adapted for addition to Tudat.
 *
 *    References
 *
 *    Notes
 *
 */

#ifndef TUDAT_MULTI_LINEAR_INTERPOLATOR_H
#define TUDAT_MULTI_LINEAR_INTERPOLATOR_H

#include <iostream>

#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include <boost/shared_ptr.hpp>

#include "Tudat/Mathematics/Interpolators/lookupScheme.h"
#include "Tudat/Mathematics/Interpolators/interpolator.h"
#include "Tudat/Mathematics/BasicMathematics/nearestNeighbourSearch.h"

namespace tudat
{
namespace interpolators
{

//! Class for performing multi-linear interpolation for arbitrary number of independent variables.
/*!
 * Class for performing multi-linear interpolation for arbitrary number of independent variables.
 * Interpolation is calculated recursively over all dimensions of independent variables. Note
 * that the types (i.e. double, float) of all independent variables must be the same.
 * \tparam IndependentVariableType Type for independent variables.
 * \tparam DependentVariableType Type for dependent variable.
 * \tparam numberOfDimensions Number of independent variables.
 */
template< typename IndependentVariableType, typename DependentVariableType,
          int numberOfDimensions >
    class MultiLinearInterpolator: public Interpolator< IndependentVariableType,
    DependentVariableType, numberOfDimensions >
{
public:

    //! Constructor taking independent and dependent variable data.
    /*!
     * \param independentValues Vector of vectors containing data points of independent variables.
     * \param dependentData Multi-dimensional array of dependent data at each point of
     *          hyper-rectangular grid formed by independent variable points.
     */
    MultiLinearInterpolator( const std::vector< std::vector< IndependentVariableType > >
                             independentValues,
                             const boost::multi_array< DependentVariableType, numberOfDimensions >
                             dependentData,
                             const AvailableLookupScheme selectedLookupScheme = hunting_algorithm )
        : independentValues_( independentValues ),
          dependentData_( dependentData )
    {
        // Check consistency of template arguments and input variables.
        if ( independentValues.size( ) != numberOfDimensions )
        {
            std::cerr << "Error: dimension of independent value vector provided to constructor";
            std::cerr << "incompatible with tenmplate parameter " << std::endl;
        }

        // Check consistency of input data of dependent and independent data.
        for ( int i = 0; i < numberOfDimensions; i++ )
        {
            if ( independentValues[ i ].size( ) != dependentData.shape( )[ i ] )
            {
                std::cerr << "Warning: number of data points in dimension" << i
                          << " of independent and dependent data incompatible" << std::endl;
            }
        }

        makeLookupSchemes( selectedLookupScheme );
    }

    //! Function to perform interpolation.
    /*!
     *  This function performs the multilinear interpolation.
     *  \param independentVariableValues Vector of values of independent variables at which
     *  the value of the dependent variable is to be determined.
     *  \return Interpolated value of dependent variable in all dimensions.
     */
    DependentVariableType interpolate(
            const std::vector< IndependentVariableType >& independentValuesToInterpolate )
    {
        // Determine the nearest lower neighbours.
        std::vector< int > nearestLowerIndices;
        nearestLowerIndices.resize( numberOfDimensions );
        for ( unsigned int i = 0; i < numberOfDimensions; i++ )
        {
            nearestLowerIndices[ i ] = lookUpSchemes_[ i ]->findNearestLowerNeighbour(
                    independentValuesToInterpolate[ i ] );
        }

        // Initialize function evaluation indices to -1 for debugging purposes.
        boost::array< int, numberOfDimensions > interpolationIndices;
        for ( int i = 0; i < numberOfDimensions; i++ )
        {
            interpolationIndices[ i ] = -1;
        }

        // Call first step of interpolation, this function calls itself at subsequent independent
        // variable dimensions to evaluate and properly scale dependent variable table values at
        // all 2^n grid edges.
        return performRecursiveInterpolationStep( 0, independentValuesToInterpolate,
                                                  interpolationIndices, nearestLowerIndices );
    }


private:

    //! Make the lookup scheme that is to be used.
    /*!
     * This function creates the look up scheme that is to be used in determining the interval of
     * the independent variable grid where the interpolation is to be performed. It takes the type
     * of lookup scheme as an enum and constructs the lookup scheme from the independentValues_
     * that have been set previously.
     *  \param selectedScheme Type of look-up scheme that is to be used
     */
    void makeLookupSchemes( const AvailableLookupScheme selectedScheme )
    {
        lookUpSchemes_.resize( numberOfDimensions );
        // Find which type of scheme is used.
        switch( selectedScheme )
        {
        case binary_search:

            for( int i = 0; i < numberOfDimensions; i++ )
            {
                // Create binary search look up scheme.
                lookUpSchemes_[ i ] = boost::shared_ptr< LookUpScheme< IndependentVariableType > >
                        ( new BinarySearchLookupScheme< IndependentVariableType >(
                              independentValues_[ i ] ) );
            }

            break;

        case hunting_algorithm:

            for( int i = 0; i < numberOfDimensions; i++ )
            {
                // Create hunting scheme, which uses an intial guess from previous look-ups.
                lookUpSchemes_[ i ] = boost::shared_ptr< LookUpScheme< IndependentVariableType > >
                        ( new HuntingAlgorithmLookupScheme< IndependentVariableType >(
                              independentValues_[ i ] ) );
            }

            break;

        default:

            std::cerr << "Warning: lookup scheme not found when making scheme for 1-D interpolator"
                      << std::endl;
        }
    }

    //! Perform the step in a single dimension of the interpolation process.
    /*!
     * Function calculates single dimension of the interpolation process. Function calls itself if
     * final dimension not yet reached. Calling this function with currentVariable = 0 will result
     * in 2^{numberOfDimensions} number of calls to the function at currentVariable =
     * numberOfDimensions -1. As such, the complete series of calls, starting at currentVariable =
     * 0, retrieves the dependent variable values at all edges of the grid hyper-rectangle and
     * properly scales them.
     * \param currentVariable Dimension in which this interpolation step is to be performed.
     * \param independentValuesToInterpolate Vector of values of independent variables at which
     *          interpolation is to be performed.
     * \param currentArrayIndices Array of indices modified at index = currentVariable at each
     *          call of function. Variable is passed to dependentData in highest step to return
     *          data for interpolation.
     *  \return Interpolated value in a single dimension
     */
    DependentVariableType performRecursiveInterpolationStep(
            const unsigned int currentVariable,
            const std::vector< IndependentVariableType >& independentValuesToInterpolate,
            boost::array< int, numberOfDimensions > currentArrayIndices,
            const std::vector< int >& nearestLowerIndices )
    {
        IndependentVariableType upperFraction, lowerFraction;
        DependentVariableType upperContribution, lowerContribution;

        // Calculate fractions of data points above and below independent
        // variable value to be added to interpolated value.
        upperFraction = ( independentValuesToInterpolate[ currentVariable ] -
                          independentValues_[ currentVariable ]
                          [ nearestLowerIndices[ currentVariable ] ] ) /
                ( independentValues_[ currentVariable ]
                  [ nearestLowerIndices[ currentVariable ] + 1 ] -
                  independentValues_[ currentVariable ]
                  [ nearestLowerIndices[ currentVariable ] ] );
        lowerFraction = -( independentValuesToInterpolate[ currentVariable ] -
                           independentValues_[ currentVariable ]
                           [ nearestLowerIndices[ currentVariable ] + 1 ] ) /
                ( independentValues_[ currentVariable ]
                  [ nearestLowerIndices[ currentVariable ] + 1 ] -
                  independentValues_[ currentVariable ]
                  [ nearestLowerIndices[ currentVariable ] ] );

        // If at top dimension, call dependent variable data.
        if ( currentVariable == numberOfDimensions - 1 )
        {
            currentArrayIndices[ numberOfDimensions - 1 ] = nearestLowerIndices[ currentVariable ];
            lowerContribution = dependentData_( currentArrayIndices );
            currentArrayIndices[ numberOfDimensions - 1 ] = nearestLowerIndices[ currentVariable ]
                                                            + 1;
            upperContribution = dependentData_( currentArrayIndices );
        }

        // If at lower dimension, update currentArrayIndices and call function with
        // currentVariable++.
        else
        {
            currentArrayIndices[ currentVariable ] = nearestLowerIndices[ currentVariable ];
            lowerContribution = performRecursiveInterpolationStep(
                        currentVariable + 1, independentValuesToInterpolate,
                        currentArrayIndices, nearestLowerIndices );
            currentArrayIndices[ currentVariable ] = nearestLowerIndices[ currentVariable ] + 1;
            upperContribution = performRecursiveInterpolationStep(
                        currentVariable + 1, independentValuesToInterpolate,
                        currentArrayIndices, nearestLowerIndices );
        }

        // Return interpolated value.
        DependentVariableType returnValue = upperFraction * upperContribution +
                                            lowerFraction * lowerContribution;
        return returnValue;
    }

    //! Vector with pointers to look-up scheme.
    /*!
     * Pointers to the look-up schemes that is used to determine in which interval the requested
     * independent variable value falls.
     */
    std::vector< boost::shared_ptr< LookUpScheme< IndependentVariableType > > > lookUpSchemes_;

    //! Vector of vectors containing independent variables.
    /*!
     * Vector of vectors containing independent variables. The size of the outer vector is equal
     * to the number of dimensions of the interpolator.
     */
    std::vector< std::vector< IndependentVariableType > > independentValues_;

    //! Multi-dimensional array of dependent data.
    /*!
     * Multi-dimensional array of dependent data at each point of hyper-rectangular grid formed by
     * independent variable points.
     */
    boost::multi_array< DependentVariableType, numberOfDimensions > dependentData_;
};

} // namespace interpolators
} // namespace tudat

#endif // TUDAT_MULTI_LINEAR_INTERPOLATOR_H
