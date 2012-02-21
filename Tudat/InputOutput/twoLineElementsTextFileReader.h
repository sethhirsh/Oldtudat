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
 *      110221    J. Leloux         Startup of TLE header file and class.
 *      110301    J. Leloux         Adjusting header file to parent classes and Tudat rules.
 *      110803    J. Leloux         First setup for codecheck.
 *      110805    K. Kumar          Layout and comment corrections; added
 *                                  get-function for std::vector container of TLE data.
 *      110810    J. Leloux         Tested new setup and changed descriptions.
 *      110826    J. Leloux         Added functionality for 2-line and 3-line data.
 *      111027    K. Kumar          Modified 2-line and 3-line options using enum.
 *
 *    References
 *      Leloux, J. Filtering Techniques for Orbital Debris Conjunction Analysis
 *          - applied to SSN TLE catalog data and including astrodynamics and
 *          collision probability theory, MSc Literature Research, Delft
 *          University of Technology, 2010.
 *      Celestrak (a). Space Track TLE Retriever Help,
 *          http://celestrak.com/SpaceTrack/TLERetrieverHelp.asp, 2011. Last
 *          accessed: 5 August, 2011.
 *      Space Track. TLE Format, http://www.space-track.org/tle_format.html,
 *          2004. Last accessed: 5 August, 2011.
 *      Celestrak (b). FAQs: Two-Line Element Set Format,
 *          http://celestrak.com/columns/v04n03/, 2006. Last accessed:
 *          5 August, 2011.
 *      Celestrak (c). NORAD Two-Line Element Set Format,
 *          http://celestrak.com/NORAD/documentation/tle-fmt.asp, 2004. Last
 *          accessed: 5 August, 2011.
 *
 */

// Temporary notes (move to class/function doxygen):
// Raw TLE data can be obtained from (Celestrak (a), 2011). Explanations
// of the TLE data format can be viewed in (Space Track, 2004),
// (Celestrak (b), 2006), and (Celestrak (c), 2004).
// 
// 

#ifndef TWOLINEELEMENTSTEXTFILEREADER_H
#define TWOLINEELEMENTSTEXTFILEREADER_H

// Include statements.
#include <fstream>

#include "Tudat/InputOutput/twoLineElementData.h"

//! Tudat library namespace.
/*!
 * The Tudat library namespace.
 */
namespace tudat
{

namespace input_output
{

//! TLE catalog text file reader class.
/*!
 * Definition of TLE catalog text file reader class.
 */
class TwoLineElementsTextFileReader
{
public:

    //! Type definition of map of line-based string data.
    /*!
     * Type definition of map of line-based string data.
     */
    typedef std::map< unsigned int, std::string > LineBasedStringDataMap;

    //! Default constructor.
    /*!
     * Default constructor.
     */
    TwoLineElementsTextFileReader( ) : lineCounter_( 1 ), numberOfHeaderLines_( 0 ), dataFile_( ), fileName_( "" ),
        stringOfData_( "" ), absoluteFilePath_( "" ), absoluteDirectoryPath_( "" ),
        relativeDirectoryPath_( "" ), startingCharacter_( "" ), skipKeyword_( "" ),
        containerOfDataFromFile_( ), currentYear_( -0 ), numberOfObjects_( -0 ),
        numberOfLinesPerTwoLineElementDatum_( 3 ), twoLineElementData_( ) { }

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~TwoLineElementsTextFileReader( ) { if ( dataFile_.is_open( ) ) closeFile( ); }

    //! Set absolute directory path.
    /*!
     * Sets absolute path to directory containing data file. If this is set, the
     * relative path will be cleared.
     * \param absoluteDirectoryPath Absolute path to directory containing data file.
     */
    void setAbsoluteDirectoryPath( std::string absoluteDirectoryPath )
    { relativeDirectoryPath_ = ""; absoluteDirectoryPath_ = absoluteDirectoryPath; }

    //! Set relative directory path.
    /*!
     * Sets relative path to directory containing data file, with respect to
     * root directory of Tudat library. If this is set, the absolute path will be cleared.
     * \param relativeDirectoryPath Relative directory path.
     */
    void setRelativeDirectoryPath( std::string relativeDirectoryPath )
    { absoluteDirectoryPath_ = ""; relativeDirectoryPath_ = relativeDirectoryPath; }

    //! Set file name.
    /*!
     * Sets file name of data file.
     * \param fileName File name.
     */
    void setFileName( std::string fileName ) { fileName_ = fileName; }

    //! Open data file.
    /*!
     * Opens data file.
     */
    void openFile( );

    //! Skip lines.
    /*!
     * Skips a given number of lines of data file. This function cannot be used
     * in combination with the skipLinesStartingWithCharacter( ) function.
     * \param numberOfLines Number of lines.
     */
    void skipLines( unsigned int numberOfLines );

    //! Skip all lines starting with a given character.
    /*!
     * Skips all lines starting with a given character. This function cannot be
     * used in combination with the skipLines( ) function.
     * \param startingCharacter Starting character.
     */
    void skipLinesStartingWithCharacter( const std::string& startingCharacter )
    { startingCharacter_ = startingCharacter; }

    //! Skip all lines containing a given keyword.
    /*!
     * Skips all lines containing a given keyword. This function cannot be
     * used in combination with the skipLines( ) function.
     * \param skipKeyword Keyword to skip line.
     */
    void skipLinesWithKeyword( const std::string& skipKeyword ) { skipKeyword_ = skipKeyword; }

    //! Set number of lines for file header.
    /*!
     * Sets number of lines for file header. This function defines the number of
     * lines of the file header, starting from the beginning of the file.
     * This function cannot be used in combination with the skipLines( ),
     * skipLinesStartingWithCharacter( ), and skipLinesWithKeyword( ) functions.
     * \param numberOfHeaderLines Number of lines for file header.
     */
    void setNumberOfHeaderLines( unsigned int numberOfHeaderLines )
    { numberOfHeaderLines_ = numberOfHeaderLines; }

    //! Close data file.
    /*!
     * Closes data file.
     */
    void closeFile( ) { dataFile_.close( ); }

    //! Get vector container of data from file.
    /*!
     * Returns map container of string data from data file.
     * \return Pointer to map container of data from file.
     */
    LineBasedStringDataMap& getContainerOfData( ) { return containerOfDataFromFile_; }

    //! Get container of header data from file.
    /*!
     * Returns container of header data from file.
     * \return Container of header data from file.
     */
    LineBasedStringDataMap& getContainerOfHeaderData( ) { return containerOfHeaderDataFromFile_; }

    //! Read and store data.
    /*!
     * Reads and stores data from data file.
     */
    void readAndStoreData( );

    //! Read and store data.
    /*!
     * Reads and stores given number of lines of data from data file.
     * \param numberOfLines Number of lines.
     */
    void readAndStoreData( unsigned int numberOfLines );

    //! Strip End-Of-Line characters.
    /*!
     * Strips End-Of-Line (EOL) characters from string data stored in data container.
     * The EOL characters removed are "\r" and "\n".
     * This function should only be used once data input data from the assigned
     * data file has been read and stored.
     * \param containerOfLinesOfData Container of lines of data (can be container of header lines
     *          or of data lines from input file).
     */
    void stripEndOfLineCharacters( LineBasedStringDataMap& containerOfLinesOfData );


    //! Line-number types for TLE input data.
    /*!
     * Line-number types for TLE input data.
     */
    enum LineNumberTypesForTwoLineElementInputData { twoLineType, threeLineType };


    //! Set current year.
    /*!
     * Sets the current year.
     * \param currentYear Current year.
     */
    void setCurrentYear( const unsigned int& currentYear ) { currentYear_ = currentYear; }

    //! Get TLE data.
    /*!
     * Returns container of TwoLineElementData objects, containing all the TLE
     * data retrieved from the catalog file and stored in objects.
     * \return TLE data stored in TwoLineElementData objects.
     */
    std::vector< TwoLineElementData >& getTwoLineElementData( ) { return twoLineElementData_; }

    //! Get number of objects.
    /*!
     * Returns number of objects in TLE data catalog file.
     * \return Number of objects in TLE data catalog file.
     */
    unsigned int& getNumberOfObjects( ) { return numberOfObjects_; }

    //! Convert and store TLE data.
    /*!
     * Converts strings read by TextFileReader to the variables contained in
     * their TLE format and stores their data according to their variable-type
     * in the variables contained in this class.
     */
    void storeTwoLineElementData( );

    //! Checks the integrity of the TLE input file.
    /*!
     * Checks the integrity of the TLE input file. For instance:
     * line number 1 = 1 and 2 = 2, object identification number of
     * line 1 = line 2, U for unclassified data, modulo-10 checksum check, etc.
     * Deletes objects whose TLE data are corrupted.
     * \return 0 for success, integer with number of corrupted objects for
     *          failure (with error output).
     */
    std::multimap< int, std::string > checkTwoLineElementsFileIntegrity( );

    //! Set line number type for TLE input data.
    /*!
      * Sets the line number type for TLE input data. This can be either 2-line or 3-line.
      */
    void setLineNumberTypeForTwoLineElementInputData(
        LineNumberTypesForTwoLineElementInputData lineNumberType )
    {
        if ( lineNumberType == twoLineType ) { numberOfLinesPerTwoLineElementDatum_ = 2; }
        else if ( lineNumberType == threeLineType ) { numberOfLinesPerTwoLineElementDatum_ = 3; }
    }

protected:
    //! Line counter.
    /*!
     * Line counter.
     */
    unsigned int lineCounter_;

    //! Number of header lines.
    /*!
     * Number of lines for file header.
     */
    unsigned int numberOfHeaderLines_;

    //! Data file stream.
    /*!
     * Data file stream.
     */
    std::ifstream dataFile_;

    //! File name.
    /*!
     * File name for data file.
     */
    std::string fileName_;

    //! String of data.
    /*!
     * String of data.
     */
    std::string stringOfData_;

    //! Absolute path to data file.
    /*!
     * Absolute path to data file.
     */
    std::string absoluteFilePath_;

    //! Absolute path to directory.
    /*!
     * Absolute path to directory containing data file.
     */
    std::string absoluteDirectoryPath_;

    //! Relative path to directory.
    /*!
     * Relative path to directory containing data file.
     */
    std::string relativeDirectoryPath_;

    //! Starting character.
    /*!
     * Starting character used by the skipLinesStartingWithCharacter( ) function.
     */
    std::string startingCharacter_;

    //! Skip keyword.
    /*!
     * Keyword used to skip a line.
     */
    std::string skipKeyword_;

    //! Map container of data from file.
    /*!
     * Map container of string data from data file, obtained by reading each
     * line of data from file using the getline( ) function. The key is the
     * line number from the file and the value is line data.
     */
    LineBasedStringDataMap containerOfDataFromFile_;

    //! Container of header data from file.
    /*!
     * Map container of string header data from data file, obtained by reading each
     * line of header data from file using the getline( ) function. The key is the
     * line number from the file and the value is header line data.
     */
    LineBasedStringDataMap containerOfHeaderDataFromFile_;


private:

    //! Current year.
    /*!
     * Current year.
     */
    unsigned int currentYear_;

    //! Number of object in input file.
    /*!
     * Number of objects in input file, linecounter divided by 3.
     */
    unsigned int numberOfObjects_;

    //! Number of lines per TLE datum.
    /*!
     * Number of lines per TLE datum. (Can be 2 or 3).
     */
    unsigned int numberOfLinesPerTwoLineElementDatum_;

    //! Vector of TwoLineElementData objects.
    /*!
     * Vector of TwoLineElementData objects, used to store multiple objects
     * with their TLE data from catalog file.
     */
    std::vector< TwoLineElementData > twoLineElementData_;
};


} // namespace input_output

} // namespace tudat

#endif // TWOLINEELEMENTSTEXTFILEREADER_H

// End of file.
