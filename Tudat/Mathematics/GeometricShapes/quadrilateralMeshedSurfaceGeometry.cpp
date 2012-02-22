/*    Copyright (c) 2010-2012 Delft University of Technology.
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
 *      101125    D. Dirkx          First version of file
 *      110127    D. Dirkx          Finalized for code check.
 *      110206    J. Melman         Minor formatting issues.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *
 *    References
 *      An example of a heritage code which uses such a mesh is found in:
 *          The Mark IV Supersonic-Hypersonic Arbitrary Body Program, Volume
 *          II-Program Formulation, Douglas Aircraft Company, AFFDL-TR-73-159,
 *          Volume II.
 *
 */

// Temporary notes (move to class/function doxygen):
// This class uses pointers to pointers to denote two-dimensional arrays
// for heritage reasons, instead of the more modern C++ vector or map
// types.
// 
// The numberOfLines_ and numberOfPoints_ member variables denote the
// number of mesh points. The number of panels in the mesh will be
// numberOfLines_ - 1 by numberOfPoints_ - 1.
// 

#include <Eigen/Geometry>
#include <limits>
#include "Tudat/Mathematics/GeometricShapes/quadrilateralMeshedSurfaceGeometry.h"

namespace tudat
{

//! Calculate panel characteristics.
void QuadrilateralMeshedSurfaceGeometry::performPanelCalculations( )
{
    // Allocate memory for panel properties.
    panelCentroids_.resize(boost::extents[ numberOfLines_ - 1 ][ numberOfPoints_ - 1 ]);
    panelSurfaceNormals_.resize(boost::extents[ numberOfLines_ - 1 ][ numberOfPoints_ - 1 ]);
    panelAreas_.resize(boost::extents[ numberOfLines_ - 1 ][ numberOfPoints_ - 1 ]);

    // Declare local variables for normal and area determination.
    Eigen::Vector3d crossVector1;
    Eigen::Vector3d crossVector2;

    // Reset total area.
    totalArea_ = 0.0;

    // Loop over all panels to determine properties.
    for ( int i = 0; i < numberOfLines_ - 1; i++ )
    {
        for ( int j = 0; j < numberOfPoints_ - 1; j++ )
        {
            // Set panel centroid.
            panelCentroids_[ i ][ j ] = ( meshPoints_[ i ][ j ] +
                                          meshPoints_[ i + 1 ][ j ] +
                                          meshPoints_[ i ][ j + 1 ] +
                                          meshPoints_[ i + 1 ][ j + 1 ] ) / 4;

            // Set panel cross vectors.
            crossVector1 = meshPoints_[ i + 1 ][ j + 1 ] - meshPoints_[ i ][ j ];
            crossVector2 = meshPoints_[ i + 1 ][ j ] - meshPoints_[ i ][ j + 1 ];

            // Set panel normal (not yet normalized).
            panelSurfaceNormals_[ i ][ j ] = crossVector1.cross( crossVector2 );

            // Set panel area (not yet correct size).
            panelAreas_[ i ][ j ] = panelSurfaceNormals_[ i ][ j ].norm( );
            if ( panelAreas_[ i ][ j ] < std::numeric_limits< double >::epsilon( ) )
            {
                std::cerr << "WARNING panel area is zero in part at panel" << i
                          << ", " << j << std::endl;               
            }            

            // Normalize panel normal and, if necessary, invert normal direction.
            panelSurfaceNormals_[ i ][ j ] *= reversalOperator_;
            panelSurfaceNormals_[ i ][ j ].normalize( );

            // Set panel area to correct size.
            panelAreas_[ i ][ j ] *= 0.5;

            // Add panel area to total area.
            totalArea_ += panelAreas_[ i ][ j ];
        }
    }
}

//! Set reversal operator.
void QuadrilateralMeshedSurfaceGeometry::setReversalOperator( bool isMeshInverted )
{
    if ( isMeshInverted == 0 )
    {
        reversalOperator_ = 1;
    }

    else
    {
        reversalOperator_ = -1;
    }
}

//! Get boolean denoting if the mesh is inverted.
bool QuadrilateralMeshedSurfaceGeometry::getReversalOperator( )
{
    bool isMeshInverted;
    if ( reversalOperator_ == 1 )
    {
        isMeshInverted = 0;
    }

    else
    {
        isMeshInverted = 1;
    }

    return isMeshInverted;
}

//! Overload ostream to print class information.
std::ostream& operator<<( std::ostream& stream,
                          QuadrilateralMeshedSurfaceGeometry& quadrilateralMeshedSurfaceGeometry )
{
    stream << "This is a quadrilateral meshed surface geometry"
           << " of a single part." << std::endl;
    stream << "The number of lines ( contours ) is: "
           << quadrilateralMeshedSurfaceGeometry.numberOfLines_ << std::endl;
    stream << "The number of points per line is: "
           << quadrilateralMeshedSurfaceGeometry.numberOfPoints_ << std::endl;

    // Return stream.
    return stream;
}

} // namespace tudat