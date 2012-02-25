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
 *                                  get-function for vector container of TLE data.
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

#include <cmath>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/exception/all.hpp>
#include <boost/throw_exception.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/physicalConstants.h>
#include <TudatCore/InputOutput/basicInputOutput.h>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include <Tudat/InputOutput/basicInputOutput.h>
#include "Tudat/InputOutput/twoLineElementsTextFileReader.h"

namespace tudat
{
namespace input_output
{

// Using declarations.
using tudat::mathematics::PI;
using std::string;
using std::stringstream;
using std::endl;
using std::cerr;
using std::vector;
using std::multimap;
using std::pair;


//! Open data file.
void TwoLineElementsTextFileReader::openFile( )
{
    if ( absoluteDirectoryPath_.compare( "" ) == 0 )
    {
        absoluteFilePath_ = getTudatRootPath( ) + relativeDirectoryPath_ + fileName_;
    }

    else
    {
        absoluteFilePath_ = absoluteDirectoryPath_ + fileName_;
    }

    // Open data file.
    dataFile_.open( absoluteFilePath_.c_str( ), std::ios::binary );

    // Check if file could be opened. Throw exception with error message if file could not be
    // opened.
    if ( !dataFile_ )
    {
        boost::throw_exception(
                    boost::enable_error_info(
                        std::runtime_error(
                            boost::str( boost::format( "Data file '%s' could not be opened." )
                                 % absoluteFilePath_.c_str( ) ) ) )
            << boost::errinfo_file_name( absoluteFilePath_.c_str( ) )
            << boost::errinfo_file_open_mode( "std::ios::binary" )
            << boost::errinfo_api_function( "std::ifstream::open" ) );
    }
}

//! Skip lines.
void TwoLineElementsTextFileReader::skipLines( unsigned int numberOfLines )
{
    // Call getline( ) function for set number of lines to be skipped.
    for ( unsigned int i = 0; i < numberOfLines; i++ )
    {
        // Get next line of data from file and don't do anything.
        std::getline( dataFile_, stringOfData_ );

        // Increment line counter.
        lineCounter_++;
    }
}

//! Read and store data.
void TwoLineElementsTextFileReader::readAndStoreData( )
{
    // Reset the datafile.
    containerOfDataFromFile_.clear( );

    // Whilst the end of the data file has not been reached, continue reading
    // from lines from data file.
    while( !dataFile_.eof( ) )
    {
        // Get next line of data from data file and store in a string.
        getline( dataFile_, stringOfData_ );

        // Check if line of data is header line.
        if ( lineCounter_ <= numberOfHeaderLines_ )
        {
            // Store header line data.
            containerOfHeaderDataFromFile_[ lineCounter_ ] = stringOfData_;
        }

        // Else process non-header data line.
        else
        {
            // Check if string doesn't start with set starting character, if string
            // is not empty, and if the skip keyword is not in the string.
            if ( ( ( !startingCharacter_.empty( ) && stringOfData_.substr( 0, 1 )
                     .compare( startingCharacter_ ) != 0 )
                   || ( !skipKeyword_.empty( ) && stringOfData_.find( skipKeyword_ )
                        == string::npos )
                   || ( startingCharacter_.empty( ) && skipKeyword_.empty( ) ) )
                 && !stringOfData_.empty( ) )
            {
                // Store string in container.
                containerOfDataFromFile_[ lineCounter_ ] = stringOfData_;
            }
        }

        // Increment line counter.
        lineCounter_++;
    }
}

//! Read and store data.
void TwoLineElementsTextFileReader::readAndStoreData( unsigned int numberOfLines )
{
    // Loop over number of lines of data to read and stored from data file.
    for ( unsigned int i = 0; i < numberOfLines; i++ )
    {
        // Get next line of data from data file and store in a string.
        getline( dataFile_, stringOfData_ );

        // Check string is not empty.
        if ( !stringOfData_.empty( ) )
        {
            // Store string in container.
            containerOfDataFromFile_[ lineCounter_ ] = stringOfData_;
        }

        // Increment line counter.
        lineCounter_++;
    }
}

//! Strip End-Of-Line characters.
void TwoLineElementsTextFileReader::stripEndOfLineCharacters( LineBasedStringDataMap& containerOfLinesOfData )
{
    // Declare local variables.
    // Declare string iterator.
    string::iterator iteratorString_;

    // Loop through all the strings stored in the container.
    for ( LineBasedStringDataMap::iterator iteratorContainerOfDataFromFile_
          = containerOfLinesOfData.begin( );
          iteratorContainerOfDataFromFile_ != containerOfLinesOfData.end( );
          iteratorContainerOfDataFromFile_++ )
    {
        // Loop through all the characters in the string.
        for ( iteratorString_ = iteratorContainerOfDataFromFile_->second.begin( );
              iteratorString_ != iteratorContainerOfDataFromFile_->second.end( );
              iteratorString_++ )
        {
            // Check if end-of-line characters are present in string.
            // The end-of-line characters are checked for their integer
            // equivalents. See: http://www.asciitable.com/.
            if ( static_cast< int >( *iteratorString_ ) == 10
                 || static_cast< int >( *iteratorString_ ) == 13 )
            {
                // Strip end-of-line character from string.
                iteratorContainerOfDataFromFile_->second.erase( iteratorString_ );

                // Decrement string iterator since character was erased from string.
                iteratorString_--;
            }
        }
    }
}


//! Convert and store TLE data.
void TwoLineElementsTextFileReader::storeTwoLineElementData( )
{
    using boost::algorithm::trim_copy;

    // Strip End-Of-Line characters from data container.
    stripEndOfLineCharacters( containerOfDataFromFile_ );

    // Calculate number of objects in catalog file.
    numberOfObjects_ = static_cast< int >( lineCounter_ / numberOfLinesPerTwoLineElementDatum_ );

    // Set TLE data vector size.
    twoLineElementData_.resize( numberOfObjects_ );

    // Create vector of the three lines of a single object's TLE data as strings.
    vector< string > twoLineElementString_( 3 );

    // Create the object counter.
    unsigned int objectNumberCounter_ = 0;

    // Declare Keplerian elements variables.
    double inclination_;
    double rightAscensionOfAscendingNode_;
    double eccentricity_;
    double argumentOfPerigee_;
    double meanMotion_;

    // Declare approximate number of revolutions, remainder, and lost part.
    int approximateNumberOfRevolutions_;
    int approximateNumberOfRevolutionsRemainder_;
    int lostNumberOfRevolutions_;

    // Reference: Table 2 in (Vallado, D.A., et al., 2006).
    const double earthWithWorldGeodeticSystem72GravitationalParameter = 398600.8e9;

    // For every 3 lines, read the data from 3 consecutive strings of TLE data
    // and convert them to the TLE data variables
    for ( unsigned int i = 1; i < lineCounter_; i += numberOfLinesPerTwoLineElementDatum_ )
    {
        // General setup for variable storing.
        //---------------------------------------------------------------------

        // Fill the vector with the line strings from the data container.
        if ( numberOfLinesPerTwoLineElementDatum_ == 3 )
        {
            twoLineElementString_.at( 0 ) = containerOfDataFromFile_.at( i );
            twoLineElementString_.at( 1 ) = containerOfDataFromFile_.at( i + 1 );
            twoLineElementString_.at( 2 ) = containerOfDataFromFile_.at( i + 2 );
        }

        else if ( numberOfLinesPerTwoLineElementDatum_ == 2 )
        {
            twoLineElementString_.at( 1 ) = containerOfDataFromFile_.at( i );
            twoLineElementString_.at( 2 ) = containerOfDataFromFile_.at( i + 1 );
        }

        // Push the TLE strings to the TLE data container.
        twoLineElementData_[ objectNumberCounter_ ].twoLineElementStrings = twoLineElementString_;

        // Push the line numbers to the TLE data container.
        twoLineElementData_[ objectNumberCounter_ ].lineNumbers.push_back( i );
        twoLineElementData_[ objectNumberCounter_ ].lineNumbers.push_back( i + 1 );
        if ( numberOfLinesPerTwoLineElementDatum_ == 3 )
        {
            twoLineElementData_[ objectNumberCounter_ ].lineNumbers.push_back( i + 2 );
        }

        // Initiate a stringstream for each line.
        stringstream line0StringStream_( stringstream::in | stringstream::out );
        stringstream line1StringStream_( stringstream::in | stringstream::out );
        stringstream line2StringStream_( stringstream::in | stringstream::out );

        // Insert the strings into the stringstreams
        if ( numberOfLinesPerTwoLineElementDatum_ == 3 )
        {
            line0StringStream_ << twoLineElementString_.at( 0 );
        }
        line1StringStream_ << twoLineElementString_.at( 1 );
        line2StringStream_ << twoLineElementString_.at( 2 );

        // Line-0 variable storing.
        //---------------------------------------------------------------------
        if ( numberOfLinesPerTwoLineElementDatum_ == 3 )
        {
            // Declare string containing part of name of object.
            string namePart_;

            // Loop through the stringstream and read words that constitute name of
            // object. Store name parts in objectName storage container.
            while ( line0StringStream_ >> namePart_ )
            {
                twoLineElementData_[ objectNumberCounter_ ].objectName.push_back( namePart_ );
            }

            // Insert the entire line-0 string in the object name string.
            twoLineElementData_[ objectNumberCounter_ ].objectNameString
                    = twoLineElementString_.at( 0 );
        }

        // Line-1 variable storing.
        //---------------------------------------------------------------------
        // Fill all line-1 variables of the object structure with substrings
        // of the line-1 string using the template function to transform the
        // string to another type.
        // See reference for which columns are assigned to which variable.

        // Get line number integer of line-1 from string.
        twoLineElementData_[ objectNumberCounter_ ].lineNumberLine1  =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 0, 1 ) ) );

        // Get object indentification number integer of line-1 from string
        twoLineElementData_[ objectNumberCounter_ ].objectIdentificationNumber =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 2, 5 ) ) );


        // Get classification character from string.
        twoLineElementData_[ objectNumberCounter_ ].tleClassification =
                twoLineElementString_.at( 1 ) [ 7 ];

        // Get launch year integer from string.
        twoLineElementData_[ objectNumberCounter_ ].launchYear =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 9, 2 ) ) );

        // Calculate four-digit launch year from the above.
        if ( twoLineElementData_[ objectNumberCounter_ ].launchYear > 56 )
        {
            twoLineElementData_[ objectNumberCounter_ ].fourDigitlaunchYear =
                    twoLineElementData_[ objectNumberCounter_ ].launchYear + 1900;
        }

        else
        {
            twoLineElementData_[ objectNumberCounter_ ].fourDigitlaunchYear =
                    twoLineElementData_[ objectNumberCounter_ ].launchYear + 2000;
        }

        // Get launch number integer from string.
        twoLineElementData_[ objectNumberCounter_ ].launchNumber =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 11, 3 ) ) );

        // Get launch part string from string.
        twoLineElementData_[ objectNumberCounter_ ].launchPart =
                twoLineElementString_.at( 1 ).substr( 14, 3 );

        // Get epoch year integer from string.
        twoLineElementData_[ objectNumberCounter_ ].epochYear =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 18, 2 ) ) );

        // Calculate four-digit epoch year from the above.
        if ( twoLineElementData_[ objectNumberCounter_ ].epochYear > 56 )
        {
            twoLineElementData_[ objectNumberCounter_ ].fourDigitEpochYear =
                    twoLineElementData_[ objectNumberCounter_ ].epochYear + 1900;
        }

        else
        {
            twoLineElementData_[ objectNumberCounter_ ].fourDigitEpochYear =
                    twoLineElementData_[ objectNumberCounter_ ].epochYear + 2000;
        }

        // Get epoch day double from string.
        twoLineElementData_[ objectNumberCounter_ ].epochDay =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 20, 12 ) ) );

        // Get "first-derivative of mean motion divided by two" double from string.
        twoLineElementData_[ objectNumberCounter_].firstDerivativeOfMeanMotionDividedByTwo =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 33, 10 ) ) );

        // Get coefficient of scientific notation of "second-derivative of mean motion divided
        // by six" double from string,
        // Apply implied leading decimal point.
        twoLineElementData_[ objectNumberCounter_ ]
                .coefficientOfSecondDerivativeOfMeanMotionDividedBySix =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 44, 6 ) ) ) / 100000.0;

        // Get exponent of scientific notation of "second-derivative of mean motion divided
        // by six" integer from string.
        twoLineElementData_[ objectNumberCounter_]
                .exponentOfSecondDerivativeOfMeanMotionDividedBySix =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 50, 2 ) ) );

        // Calculate "second-derivative of mean motion divided by six" double from the above two.
        twoLineElementData_[ objectNumberCounter_ ].secondDerivativeOfMeanMotionDividedBySix =
                twoLineElementData_[ objectNumberCounter_]
                .coefficientOfSecondDerivativeOfMeanMotionDividedBySix
                * pow( 10, twoLineElementData_[ objectNumberCounter_ ]
                       .exponentOfSecondDerivativeOfMeanMotionDividedBySix );

        // Get coefficient of scientific notation of "B* divided by six" double
        // from string; apply implied leading decimal point.
        twoLineElementData_[ objectNumberCounter_ ].coefficientOfBStar =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 53, 6 ) ) ) /
                100000.0;

        // Get exponent of scientific notation of B* integer from string
        twoLineElementData_[ objectNumberCounter_ ].exponentOfBStar =
                boost::lexical_cast<int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 59, 2 ) ) );

        // Calculate B* double from the above two.
        twoLineElementData_[ objectNumberCounter_ ].bStar =
                twoLineElementData_[ objectNumberCounter_ ].coefficientOfBStar
                * pow( 10.0, twoLineElementData_[ objectNumberCounter_ ].exponentOfBStar );

        // Get orbital model integer from string.
        twoLineElementData_[ objectNumberCounter_ ].orbitalModel =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 62, 1 ) ) );

        // Get TLE number integer from string.
        twoLineElementData_[ objectNumberCounter_ ].tleNumber =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 64, 4 ) ) );

        // Get modulo-10 checksum integer from string.
        twoLineElementData_[ objectNumberCounter_ ].modulo10CheckSumLine1 =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_.at( 1 ).substr( 68, 1 ) ) );

        // Line-2 variable storing.
        //---------------------------------------------------------------------
        // Fill all line-2 variables of the object structure partly with the
        // stringstream and partly with with substrings of the line-2 string
        // using the template function to transform the string to another type.
        // See reference for which columns are assigned to which variable.

        // Get line number integer of line-2 from stringstream.
        line2StringStream_ >> twoLineElementData_[ objectNumberCounter_ ].lineNumberLine2;

        // Get object identification number integer of line-2 from stringstream.
        line2StringStream_ >> twoLineElementData_[ objectNumberCounter_ ]
                              .objectIdentificationNumberLine2;

        // Get inclination double from stringstream.
        line2StringStream_ >> inclination_;
        twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                .setInclination( inclination_ );

        // Get right ascension of ascending node double from stringstream.
        line2StringStream_ >> rightAscensionOfAscendingNode_;
        twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                .setLongitudeOfAscendingNode( rightAscensionOfAscendingNode_ );

        // Get eccetricity double from stringstream.
        line2StringStream_ >> eccentricity_;
        eccentricity_ /= 10000000;
        twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                .setEccentricity( eccentricity_ );

        // Get argument of perigee double from stringstream
        line2StringStream_ >> argumentOfPerigee_;
        twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                .setArgumentOfPeriapsis( argumentOfPerigee_ );

        // Get mean anomaly double from stringstream.
        line2StringStream_ >> twoLineElementData_[ objectNumberCounter_ ].meanAnomaly;

        // Get mean motion double from line-2 string.
        twoLineElementData_[ objectNumberCounter_ ].meanMotionInRevolutionsPerDay =
                boost::lexical_cast<double>(
                    trim_copy( twoLineElementString_[ 2 ].substr( 52, 11 ) ) );

        // Get revolution number integer from line-2 string.
        twoLineElementData_[ objectNumberCounter_ ].revolutionNumber =
                boost::lexical_cast<int>(
                    trim_copy( twoLineElementString_[ 2 ].substr( 63, 5 ) ) );

        // Get modulo-10 checksum integer of line-2 from line-2 string.
        twoLineElementData_[ objectNumberCounter_ ].modulo10CheckSumLine2 =
                boost::lexical_cast<unsigned int>(
                    trim_copy( twoLineElementString_[ 2 ].substr( 68, 1 ) ) );

        // Calculate the approximate total number of revolutions, as the counter resets to 0 after
        // passing by 99999, insert the current year in the following equation.
        approximateNumberOfRevolutions_ = twoLineElementData_[ objectNumberCounter_ ]
                .meanMotionInRevolutionsPerDay
                * ( currentYear_
                    - twoLineElementData_[ objectNumberCounter_ ].fourDigitlaunchYear )
                * tudat::physical_constants::JULIAN_YEAR_IN_DAYS;
        approximateNumberOfRevolutionsRemainder_ = approximateNumberOfRevolutions_ % 100000;
        lostNumberOfRevolutions_ = approximateNumberOfRevolutions_
                - approximateNumberOfRevolutionsRemainder_;

        // Check if the counter has been reset after passing 99999.
        if ( ( twoLineElementData_[ objectNumberCounter_ ].revolutionNumber -
               approximateNumberOfRevolutionsRemainder_ ) <=
             ( approximateNumberOfRevolutionsRemainder_ + 100000 -
               twoLineElementData_[ objectNumberCounter_ ].revolutionNumber ) )
        {
            twoLineElementData_[ objectNumberCounter_ ].totalRevolutionNumber =
                    lostNumberOfRevolutions_ + twoLineElementData_[ objectNumberCounter_ ]
                    .revolutionNumber;
        }

        else
        {
            twoLineElementData_[ objectNumberCounter_ ].totalRevolutionNumber =
                    lostNumberOfRevolutions_ - 100000 + twoLineElementData_[ objectNumberCounter_ ].
                    revolutionNumber;
        }

        // Check if total number of revolutions is negative.
        if ( twoLineElementData_[ objectNumberCounter_ ].totalRevolutionNumber < 0 )
        {
            twoLineElementData_[ objectNumberCounter_ ].totalRevolutionNumber =
                    twoLineElementData_[ objectNumberCounter_ ].revolutionNumber;
        }

        // Semi-major axis of the object is calculated from the other TLE variables.
        meanMotion_ = twoLineElementData_[ objectNumberCounter_ ].meanMotionInRevolutionsPerDay
                * 2.0 * PI / tudat::physical_constants::JULIAN_DAY;
        twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements.setSemiMajorAxis(
                    orbital_element_conversions::convertEllipticalMeanMotionToSemiMajorAxis(
                        meanMotion_, earthWithWorldGeodeticSystem72GravitationalParameter ) );

        // Perigee of the object is calculated from the other TLE variables.
        twoLineElementData_[ objectNumberCounter_ ].perigee =
                twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements.getSemiMajorAxis( )
                * ( 1.0 - twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                    .getEccentricity( ) );

        // Apogee of the object is calculated from the other TLE variables.
        twoLineElementData_[ objectNumberCounter_ ].apogee =
                twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements.getSemiMajorAxis( )
                * ( 1.0 + twoLineElementData_[ objectNumberCounter_ ].TLEKeplerianElements
                    .getEccentricity( ) );

        // Increment object number counter by one.
        objectNumberCounter_++;
    }
}

//! Checks the integrity of the TLE input file.
multimap< int, string > TwoLineElementsTextFileReader::checkTwoLineElementsFileIntegrity( )
{
    // Create vector of the three lines of a TLE as strings
    vector< string > twoLineElementString_( 3 );

    // Boolean which turns to true if one of the tests is not passed.
    bool isObjectErroneous = false;

    // Vector of object numbers of objects with corrupted TLE data.
    vector< int > corruptedTwoLineElementDataPositions_;

    // Multimap of corrupted TLE errors.
    multimap< int, string > corruptedTwoLineElementDataErrors_;

    // Loop is started over the entire structure of objects,
    // multiple checks are performed per TLE, if a check is not passed boolean
    // is set to true for that object, and amount of corrupted TLEs integer is
    // incremented.
    for ( unsigned int i = 0; i < numberOfObjects_; i++ )
    {
        // Fill the vector with the line strings from the data container.
        if ( numberOfLinesPerTwoLineElementDatum_ == 3 )
        {
            twoLineElementString_.at( 1 ) = containerOfDataFromFile_[ i + 1 + ( i * 2 ) + 1 ];
            twoLineElementString_.at( 2 ) = containerOfDataFromFile_[ i + 1 + ( i * 2 ) + 2 ];
        }

        else if ( numberOfLinesPerTwoLineElementDatum_ == 2 )
        {
            twoLineElementString_.at( 1 ) = containerOfDataFromFile_[ 2 * i + 1 ];
            twoLineElementString_.at( 2 ) = containerOfDataFromFile_[ 2 * i + 1 + 1 ];
        }

        // Set boolean to false.
        isObjectErroneous = false;

        // If a line-1 line number is not 1, error message is given and boolean is set to true.
        if ( twoLineElementData_.at( i ).lineNumberLine1 != 1 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Incorrect line-1 leading integer." ) );

            isObjectErroneous = true;
        }

        // If a line-2 line number is not 2, error message is given and boolean is set to true.
        if ( twoLineElementData_.at( i ).lineNumberLine2 != 2 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Incorrect line-2 leading integer." ) );

            isObjectErroneous = true;
        }

        if ( twoLineElementData_.at( i ).tleClassification != 'U'
                  && twoLineElementData_.at( i ).tleClassification != 'C' )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Invalid TLE classification." ) );

            isObjectErroneous = true;
        }

        // Check if orbital model is 0, else error message is given and boolean is set to true.
        if ( twoLineElementData_.at( i ).orbitalModel != 0 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Incorrect orbital model." ) );

            isObjectErroneous = true;
        }

        // Line-1 modulo-10 checker, add all integers of line 1 together,
        // and minus signs count as 1, rest is not counted.

        // Create calculated modulo-10 checksum of line 1 and temporary integer for addition.
        unsigned int line1Modulo10Sum_ = 0;

        // Run loop over the 69-character long, line-2 string, but not the last character,
        // since this is the modulo-10 checksum itself and is not added.
        for ( unsigned int j = 0; j < 68; j++ )
        {
            // Check if the char is not a space or a dot, or a plus or a minus sign or any of the
            // alphabetical positions, because these can not be added.
            if ( twoLineElementString_.at( 1 )[ j ] != ' ' &&
                 twoLineElementString_.at( 1 )[ j ] != '.' &&
                 twoLineElementString_.at( 1 )[ j ] != '+' &&
                 twoLineElementString_.at( 1 )[ j ] != '-' &&
                 j != 7 && j != 14 && j != 15 && j != 16 )
            {
                // Add int to checksum.
                line1Modulo10Sum_ += boost::lexical_cast<int>(
                            twoLineElementString_.at( 1 ).substr( j , 1 ) );
            }

            // Check is the char is a minus sign
            else if ( twoLineElementString_.at( 1 )[ j ] == '-' )
            {
                // Add 1 to checksum.
                line1Modulo10Sum_++;
            }
        }

        // Take modulo 10 of checksum.
        line1Modulo10Sum_ %= 10;

        // Comparison of calculated modulo-10 checksum with modulo-10 checksum from TLE for line 1,
        // if they do not compare, error message is given and boolean is set to true.
        if ( line1Modulo10Sum_ != twoLineElementData_.at( i ).modulo10CheckSumLine1 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Incorrect line-1 modulo-10 checksum." ) );

            isObjectErroneous = true;
        }

        // Line-2 modulo-10 checker, add all integers of line 2 together.
        unsigned int line2Modulo10Sum_ = 0;

        // Run loop over the 69-character long, line-2 string, but not the last character,
        // since this is the modulo-10 checksum itself and is not added.
        for ( unsigned int k = 0; k < 68; k++ )
        {
            // Check if the char is not a space or a dot, because these can not be added.
            if ( twoLineElementString_.at( 2 )[ k ] != ' ' &&
                 twoLineElementString_.at( 2 )[ k ] != '.' )
            {
                // Add int to checksum.
                line2Modulo10Sum_ += boost::lexical_cast<int>(
                            twoLineElementString_[ 2 ].substr( k , 1 ) );
            }
        }

        // Take modulo 10 of checksum.
        line2Modulo10Sum_ %= 10;

        // Comparison of calculated modulo-10 checksum with modulo-10 checksum from TLE for line 2,
        // if they do not compare error message is given and boolean is set to true.
        if ( line2Modulo10Sum_ != twoLineElementData_.at( i ).modulo10CheckSumLine2 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >( i, "Incorrect line-2 modulo-10 checksum." ) );

            isObjectErroneous = true;
        }

        // When the object's line-1 and line-2 identification numbers do not match an
        // error message is given and boolean is set to true.
        if ( twoLineElementData_.at( i ).objectIdentificationNumber !=
             twoLineElementData_.at( i ).objectIdentificationNumberLine2 )
        {
            corruptedTwoLineElementDataErrors_.insert(
                        pair< int, string >(
                            i, "Line-1 and line-2 object idenfitication number mismatch." ) );

            isObjectErroneous = true;
        }

        // If object is erroneous, add to list.
        if ( isObjectErroneous )
        {
            corruptedTwoLineElementDataPositions_.push_back( i );
        }
    }

    // Erase all the corrupted TLEs from the TLE data vector.

    for ( unsigned int q = 0; q < corruptedTwoLineElementDataPositions_.size( ); q++ )
    {
        twoLineElementData_.erase( twoLineElementData_.begin( ) + (
                                   corruptedTwoLineElementDataPositions_.at( q ) - q ) );
        numberOfObjects_--;
    }

    // Return the amount of corrupted TLEs
    return corruptedTwoLineElementDataErrors_;
}

} // namespace input_output
} // namespace tudat
