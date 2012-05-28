/*    Copyright (c) 2010-2012, Delft University of Technology
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
 *      110206    D. Dirkx          First version of file.
 *      110208    D. Dirkx          Finalized for code check.
 *      110212    J. Melman         Fixed many minor formatting issues.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120326    D. Dirkx          Changed raw pointers to shared pointers.
 *
 *    References
 *      Craidon, C.B. A Desription of the Langley Wireframe Geometry Standard
 *          (LaWGS) format, NASA TECHNICAL MEMORANDUM 85767.
 *
 */

#include <cmath>
#include <iostream>

#include <Eigen/Core>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Mathematics/GeometricShapes/lawgsPartGeometry.h"
#include "Tudat/Mathematics/GeometricShapes/sphereSegment.h"

//! Test implementation of Lawgs surface geometry.
int main( )
{
    using namespace tudat;
    using tudat::mathematics::PI;
    using namespace tudat::mathematics::geometric_shapes;

    // Test result initialised to false.
    bool isLawgsSurfaceGeometryBad = false;

    // Create a full sphere as test geometry, with a radius of 2.0.
    const double sphereRadius = 2.0;
    boost::shared_ptr< SphereSegment > sphere = boost::make_shared< SphereSegment >(
                sphereRadius );

    // Create a Lawgs mesh of the sphere.
    LawgsPartGeometry lawgsSurface;
    int numberOfLines = 21;
    int numberOfPoints = 21;
    lawgsSurface.setMesh( sphere, numberOfLines, numberOfPoints );

    // Retrieve the total surface area and check if it is sufficiently close
    // to the expected value.
    double totalArea_ = lawgsSurface.getTotalArea( );

    if ( std::fabs( totalArea_ - 4.0 * PI * ( std::pow( sphereRadius, 2.0 ) ) ) > 0.6 )
    {
        std::cerr << "Total mesh area does not match sphere area sufficiently well." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if number of lines on mesh is correct.
    if ( lawgsSurface.getNumberOfLines( ) != numberOfLines )
    {
        std::cerr << " Number of lines in mesh incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if number of points per line on mesh is correct.
    if ( lawgsSurface.getNumberOfPoints( ) != numberOfPoints )
    {
        std::cerr << " Number of points in mesh is incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Set part name.
    std::string partName_ = "sphere";
    lawgsSurface.setName( partName_ );

    // Test if part name is properly retrieved.
    if ( lawgsSurface.getName ( ) != partName_ )
    {
        std::cerr << " Error in part name of mesh." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Retrieve normal and centroid for panel 0, 0.
    Eigen::Vector3d testNormal_ = lawgsSurface.getPanelSurfaceNormal( 0, 0 );
    Eigen::Vector3d testCentroid_ = lawgsSurface.getPanelCentroid( 0, 0 );

    // Test whether centroid and normal are collinear for panel 0, 0.
    if ( std::fabs( testCentroid_.normalized( ).dot( testNormal_.normalized( ) ) - 1.0 ) > 1.0e-5 )
    {
        std::cerr << "Normal and centroid of sphere segment mesh not collinear." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Test if the position of the x- and y-coordinate of panel 0, 0 is correct.
    if ( std::fabs( std::atan( testCentroid_.y( ) / testCentroid_.x( ) ) - PI / 20.0 ) >
         std::numeric_limits< double >::epsilon( ) )
    {
        std::cerr << "x- and y-coordinate of centroid of panel 0, 0 of "
                  << "sphere mesh is incorrect." << std::endl;
        isLawgsSurfaceGeometryBad = true;
    }

    // Return test result.
    // If test is succesful return 0, if test fails, return 1.
    if ( isLawgsSurfaceGeometryBad )
    {
        std::cerr << "testLawgsSurfaceGeometry failed!" << std::endl;
    }

    return  isLawgsSurfaceGeometryBad;
}
