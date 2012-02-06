/*! \file geometricShapesToFile.cpp
 *    Header file that defines the class containing all funtionality pertaining
 *    to writing output to file included in Tudat.
 *
 *    Path              : /Mathematics/GeometricShapes/
 *    Version           : 2
 *    Check status      : Checked
 *
 *    Author            : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : bart.romgens@gmail.com
 *
 *    Date created      : 05 February, 2012
 *    Last modified     : 05 February, 2012
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
 *      YYMMDD    Author              Comment
 *      120205    D. Dirkx            Moved part of old writingOutputToFile class to this free
 *                                    function
 */

#include "Tudat/Mathematics/GeometricShapes/geometricShapesToFile.h"
#include <fstream>

namespace tudat
{

namespace output
{

//! Write single surface geometry to a file.
void writeSingleSurfaceGeometryPointsToFile(
    SingleSurfaceGeometry* pointerToSingleSurfaceGeometry,
    int numberOfLines, int numberOfPoints,
    const std::string& filename, int writeType, const bool& isIndependentVariableInverted )
{
    std::ofstream outputFile_;

    // Declare local variables.
    // Declare grid sizes.
    double gridSize1_;
    double gridSize2_;

    // Declaration of vector which will be iteratively retrieved from geometry
    // and written to file.
    Eigen::VectorXd point = Eigen::VectorXd( 3 );

    // Open the file to which writing will take place if it is to overwrite any
    // existing content.
    if ( writeType == 0 )
    {
        // Open output file.
        outputFile_.open( filename.c_str( ) );
    }

    // Open the file to which writing will take place if it is to append to
    // existing content.
    else if ( writeType == 1 )
    {
        // Open output file with append option.
        outputFile_.open( filename.c_str( ), std::ios::app );
    }


    // Sets the grid size in both directions.
    if ( isIndependentVariableInverted  == false )
    {
        // Set grid size 1.
        gridSize1_ = ( pointerToSingleSurfaceGeometry->getMaximumIndependentVariable( 1 )
                      - pointerToSingleSurfaceGeometry->getMinimumIndependentVariable( 1 ) )
                / ( numberOfLines - 1 );

        // Set grid size 2.
        gridSize2_ = ( pointerToSingleSurfaceGeometry->getMaximumIndependentVariable( 2 )
                      - pointerToSingleSurfaceGeometry->getMinimumIndependentVariable( 2 ) )
                / ( numberOfPoints - 1 );
    }

    // Sets the grid size in both directions, inverted
    // ( i.e., numberOfPoints corresponds to number of points in independent
    // variable 2 direction and numberOfLines corresponds to number of points
    // in indepedent variable 1 direction )
    else
    {
        // Set grid size 1.
        gridSize1_ = ( pointerToSingleSurfaceGeometry->getMaximumIndependentVariable( 1 )
                      - pointerToSingleSurfaceGeometry->getMinimumIndependentVariable( 1 ) )
                / ( numberOfPoints - 1 );

        // Set grid size 1.
        gridSize2_ = ( pointerToSingleSurfaceGeometry->getMaximumIndependentVariable( 2 )
                      - pointerToSingleSurfaceGeometry->getMinimumIndependentVariable( 2 ) )
                / ( numberOfLines - 1 );
    }

    // Iterate over all points, and retrieve the surface point from the
    // surfaceGeometry object, and write it to the file.
    for ( int i = 0; i < numberOfLines ; i++ )
    {
        for ( int j = 0; j < numberOfPoints ; j++ )
        {
            // If not inverted, all points from a single value of independent
            // variable 2 are written first.
            if ( isIndependentVariableInverted  == false )
            {
                // Set point.
                point = pointerToSingleSurfaceGeometry->getSurfacePoint(
                            pointerToSingleSurfaceGeometry->
                            getMinimumIndependentVariable( 1 ) + i * gridSize1_,
                            pointerToSingleSurfaceGeometry->
                            getMinimumIndependentVariable( 2 ) + j * gridSize2_ );
            }

            // If not inverted, all points from a single value of independent
            // variable 2 are written first.
            else
            {
                // Set point.
                point = pointerToSingleSurfaceGeometry->getSurfacePoint(
                            pointerToSingleSurfaceGeometry->
                            getMinimumIndependentVariable( 1 ) + j * gridSize1_,
                            pointerToSingleSurfaceGeometry->
                            getMinimumIndependentVariable( 2 ) + i * gridSize2_);
            }

            // Write the x-, y- and z-coordinates, followed by a next line command.
            outputFile_ << i+1 << " " << j+1 << " " << point( 0 ) << " " << point( 1 ) << " ";
            outputFile_ << point( 2 ) << " " << std::endl;
        }
    }

    // Closing output file.
    outputFile_.close( );
}

//! Write composite surface geometry to a file.
void writeCompositeSurfaceGeometryPointsToFile(
        CompositeSurfaceGeometry* pointerToCompositeSurfaceGeometry,
        std::vector< int > arrayOfNumberOfLines, std::vector< int > arrayOfNumberOfPoints,
        const std::string& filename, int writeType,
        std::vector< bool > isIndependentVariableInvertedArray )
{
    // Remove file of same name, if writeType is not append.
    if ( writeType == 0 )
    {
        std::remove( filename.c_str( ) );
    }

    // Iterate over all single geometries in composite geometry.
    for ( unsigned int i = 0; i < pointerToCompositeSurfaceGeometry->
          getNumberOfSingleSurfaceGeometries( ); i++ )
    {
        // Append single geometry to file.
        writeSingleSurfaceGeometryPointsToFile(
                    pointerToCompositeSurfaceGeometry->getSingleSurfaceGeometry( i ),
                    arrayOfNumberOfLines[ i ], arrayOfNumberOfPoints[ i ],
                    filename, 1, isIndependentVariableInvertedArray[ i ] );
    }
}

}

}
