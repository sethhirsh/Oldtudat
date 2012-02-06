/*! \file writingOutputToFile.h
 *    Header file that defines the class containing all funtionality pertaining
 *    to writing output to file included in Tudat.
 *
 *    Path              : /Output/
 *    Version           : 5
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Author            : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : B. Romgens
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : bart.romgens@gmail.com
 *
 *    Date created      : 12 August, 2010
 *    Last modified     : 2 February, 2010
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
 *      100914    K. Kumar            File created.
 *      100928    K. Kumar            Completed missing comments, changed
 *                                    writeIntegrationHistoryToFile( ) to
 *                                    writePropagationHistoryToFile( ).
 *      100929    B. Romgens          Spelling mistakes corrected.
 *      100929    K. Kumar            Added checked code written by D. Dirkx.
 *      110202    K. Kumar            Updated writePropagationHistoryToFile( )
 *                                    to work with State*.
 */

#ifndef WRITINGOUTPUTTOFILE_H
#define WRITINGOUTPUTTOFILE_H

// Include statements.
#include <map>
#include "Tudat/Astrodynamics/States/state.h"
#include "Tudat/Mathematics/GeometricShapes/compositeSurfaceGeometry.h"
#include "Tudat/Mathematics/GeometricShapes/singleSurfaceGeometry.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

//! Writing output to file class.
/*!
 * Class containing all functionality pertaining to writing output to file.
 */
class WritingOutputToFile
{
public:

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~WritingOutputToFile( ) { }

    //! Write propagation history to file.
    /*!
     * Writes propagation history stored in a map to file. The data is stored
     * in State objects.
     * \param propagationHistory Map of propagation history.
     * \param outputFilename Output filename.
     */
    void writePropagationHistoryToFile( std::map< double, State >& propagationHistory,
                                        const std::string& outputFilename );

protected:

private:

    //! Output file.
    /*!
     * Output file.
     */
    static std::ofstream outputFile_;
};

}

#endif // WRITINGOUTPUTTOFILE_H

// End of file.
