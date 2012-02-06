/*! \file geometricShapesToFile.h
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

#ifndef GEOMETRICSHAPESTOFILE_H
#define GEOMETRICSHAPESTOFILE_H

#include "Tudat/Mathematics/GeometricShapes/singleSurfaceGeometry.h"
#include "Tudat/Mathematics/GeometricShapes/compositeSurfaceGeometry.h"
#include <vector>

namespace tudat
{

namespace output
{

//! Write single surface geometry to a file.
/*!
 * Writes the points on a SingleSurfaceGeometry object to a file.
 * Each row contains the x, y and z coordinate of the point, each next row
 * defines a single new point.
 * \param pointerToSingleSurfaceGeometry Geometry which is to be written.
 * \param numberOfLines Defines how many points are taken over the 1st
 *          independent variable.
 * \param numberOfPoints Defines how many points are taken over the 2nd
 *          independent variable.
 * \param filename Name of the file to which the points are written.
 * \param writeType Defines whether to append or write to the file given by
 *          filename, should be "a" for append and "w" for write.
 * \param isIndependentVariableInverted Boolean flag which if set to true
 *          inverts which independent variable is treated as 1st and which
 *          as 2nd.
 */
void writeSingleSurfaceGeometryPointsToFile(
        SingleSurfaceGeometry* pointerToSingleSurfaceGeometry,
        int numberOfLines, int numberOfPoints,
        const std::string& filename, int writeType,
        const bool& isIndependentVariableInverted );

//! Write composite surface geometry to a file.
/*!
 *  Writes the single surface geometries in a composite surface geometry to
 *  a file. The writeSingleGeometryPointsToFile() function is called for
 *  each surface geometry.
 *  \param pointerToCompositeSurfaceGeometry Geometry from which there
 *          is to be written.
 *  \param arrayOfNumberOfLines Array of how many points to take over the 1st
 *          independent variables of single surface geometries.
 *  \param arrayOfNumberOfPoints Array of how many points to take over the 2nd
 *          independent variables of single surface geometries.
 *  \param filename Name of the file to which the points are written.
 *  \param writeType Defines whether to append or write to the file given
 *          by filename,  should be "a" for append and "w" for write.
 *  \param isIndependentVariableInvertedArray Array of booleans which if
 *          set to true invert which independent variable is treated as 1st
 *          and which as 2nd for each single surface geometry.
 */
void writeCompositeSurfaceGeometryPointsToFile(
        CompositeSurfaceGeometry* pointerToCompositeSurfaceGeometry,
        std::vector< int > arrayOfNumberOfLines, std::vector< int > arrayOfNumberOfPoints,
        const std::string& filename, int writeType,
        std::vector< bool > isIndependentVariableInvertedArray );

}

}

#endif // GEOMETRICSHAPESTOFILE_H
