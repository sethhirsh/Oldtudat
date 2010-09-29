/*! \file writingOutputToFile.h
 *    Header file that defines the class containing all funtionality pertaining
 *    to writing output to file included in Tudat.
 *
 *    Path              : /Astrodynamics/Output/
 *    Version           : 4
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Author            : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : D.Dirkx@student.tudelft.nl
 *
 *    Checker           : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : bart.romgens@gmail.com
 *
 *    Date created      : 12 august, 2010
 *    Last modified     : 29 september, 2010
 *
 *    References
 *
 *    Notes
 *
 *    Copyright (c) 2010 Delft University of Technology.
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
 *      YYMMDD    author              comment
 *      100914    K. Kumar            File created.
 *      100928    K. Kumar            Completed missing comments, changed
 *                                    writeIntegrationHistoryToFile( ) to
 *                                    writePropagationHistoryToFile( ).
 *      100929    B. Romgens          Spelling mistakes corrected.
 *      100929    K. Kumar            Added checked code written by D. Dirkx.
 */


#ifndef WRITINGOUTPUTTOFILE_H
#define WRITINGOUTPUTTOFILE_H

// Include statements.
#include <map>
#include <fstream>
#include <string>
#include "outputHandling.h"
#include "linearAlgebra.h"
#include "surfaceGeometry.h"

//! Writing output to file class.
/*!
 * Class containing all functionality pertaining to writing output to file.
 */
class WritingOutputToFile : public OutputHandling
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    WritingOutputToFile( );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    ~WritingOutputToFile( );

    //! Write propagation history to file.
    /*!
     * This function writes propagation history stored in a map to file.
     * \param propagationHistory Map of propagation history.
     * \param outputFilename Output filename.
     */
    static void writePropagationHistoryToFile(
            std::map < double, VectorXd >& propagationHistory,
            const std::string& outputFilename );

    //! Function to write a surface geometry to a file.
    /*!
     * Function to write the points on a surfaceGeometry object to a file.
     * Each row contains the x, y and z coordinate of the point, each next row
     * defines a single new point.
     * \param numberOfLines defines how many points are taken over the 1st
     * independant variable.
     * \param numberOfPoints defines how many points are taken over the 2nd
     * independant variable.
     * \param filename name of the file to which the points are written.
     * \param writeType defines whether to append or write to the file given by
     *      filename,  should be "a" for append and "w" for write.
     * \param isInvertIndependentVariable boolean flag which if set to true
     *  inverts which independent variable is treated as 1st and which as 2nd.
     */
    void writeGeometryPointsToFile( SurfaceGeometry* geometry,
                                           int numberOfLines,
                                           int numberOfPoints,
                                           const std::string& filename,
                                           int writeType,
                                           bool isInvertIndependentVariable );

protected:

private:

    //! Output file.
    /*!
     * Output file.
     */
    static std::ofstream outputFile_;
};

#endif // WRITINGOUTPUTTOFILE_H

// End of file.
