/*! \file matrixTextFileReader.h
 *    This file contains the header file of the matrix text file reader included in Tudat.
 *
 *    Path              : /Input/
 *    Version           : 1
 *    Check status      : Checked
 *
 *    Author            : F. M. Engelen
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : F.M.Engelen@student.tudelft.nl
 *
 *    Checker           : S. Billemont
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : simon@angelcorp.be
 *
 *    Date created      : 30 May, 2011
 *    Last modified     : 20 December, 2011
 *
 *    References
 *
 *    Notes
 *      If tabs are used as spaces, it doesn't work. The seperator should also be tabs then.
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
 *      YYMMDD    Author            Comment
 *      110530    F.M. Engelen      First creation of code.
 */

#ifndef TUDAT_MATRIX_TEXT_FILEREADER_H
#define TUDAT_MATRIX_TEXT_FILEREADER_H

// Include statements.
#include <Eigen/Core>
#include <string>

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

namespace input_output
{


//! Read the file and return the data matrix.
/*!
 * Read a textfile whith seperated (space, tab, comma etc...) numbers.
 * The class returns these numbers as a matrixXd. The first line with numbers is used
 * to define the number of columns.
 * \param filename The filename of the to be read file.
 * \param seperators Separators used, every character in the string will be used
 *         as seperator. (multiple seperators possible).
 * \param skipLinesCharacter Skip lines starting with this character.
 * \param relativePath set the relative path (can also be done via the filename).
 * \return The datamatrix.
 */
Eigen::MatrixXd readMatrixFromFile( const std::string& filename,
                                    const std::string& separators = "\t ;,",
                                    const std::string& skipLinesCharacter = "%",
                                    const std::string& relativePath = "" );


} // namespace input_output.

} // namespace tudat.

#endif // TUDAT_MATRIX_TEXT_FILEREADER_H

// End of file.

