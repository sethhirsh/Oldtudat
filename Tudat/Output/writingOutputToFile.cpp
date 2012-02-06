/*! \file writingOutputToFile.cpp
 *    Header file that defines the class containing all funtionality pertaining
 *    to writing output to file included in Tudat.
 *
 *    Path              : /Astrodynamics/Output/
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
 *    Last modified     : 2 February, 2011
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
 *    warranty of merchantability or fitness for a particular purpose.
 *
 *    Changelog
 *      YYMMDD    Author              Comment
 *      100914    K. Kumar            File created.
 *      100928    K. Kumar            Completed missing comments, changed
 *                                    writeIntegrationHistoryToFile( ) to
 *                                    writePropagationHistoryToFile( ).
 *      100929    B. Romgens          Spelling mistakes corrected and output to
 *                                    file corrected.
 *      100929    K. Kumar            Added checked code written by D. Dirkx.
 *      110202    K. Kumar            Updated writePropagationHistoryToFile()
 *                                    to work with State*.
 */

// Include statements.
#include <Eigen/Core>
#include <map>
#include <fstream>
#include <string>
#include "Tudat/Output/writingOutputToFile.h"

//! Tudat library namespace.
namespace tudat
{

// Declare ofstream object.
std::ofstream WritingOutputToFile::outputFile_;

//! Write propagation history to file.
void WritingOutputToFile::writePropagationHistoryToFile(
    std::map< double, State >& propagationHistory, const std::string& outputFilename )
{
    // Declare local variables.
    // Declare iterator for propagation history.
    std::map< double, State >::iterator iteratorPropagationHistory_;

    // Open output file.
    outputFile_.open( outputFilename.c_str( ) );

    // Loop over map of propagation history.
    for ( iteratorPropagationHistory_ = propagationHistory.begin( );
          iteratorPropagationHistory_ != propagationHistory.end( );
          iteratorPropagationHistory_++ )
    {
        // Print map key to output file.
        outputFile_ << iteratorPropagationHistory_->first;

        // Loop over map data.
        for ( int i = 0;
              i < iteratorPropagationHistory_->second.state.rows( ); i++ )
        {
            // Print map data to file.
            outputFile_.precision( 10 );
            outputFile_ << ", " << iteratorPropagationHistory_->second.state( i );
        }

        // End line of output file.
        outputFile_ << std::endl;
    }

    // Close output file.
    outputFile_.close( );
}

}

// End of file.
