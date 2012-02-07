/*! \file basicFunctions.cpp
 *    Source file that defines the basicFunctions namespace, containing all
 *    basic functions contained in Tudat.
 *
 *    Path              : /Basics/
 *    Version           : 1
 *    Check status      : Unchecked
 *    Date created      : 7 February, 2012
 *    Last modified     : 7 February, 2012
 *
 *    References
 *      Press W.H., et al. Numerical Recipes in C++: The Art of
 *          Scientific Computing. Cambridge University Press, February 2002.
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
 *      YYMMDD    Author            Comment
 *      120207    K. Kumar          File created.
 */

// Required Boost unit test framework define.
#define BOOST_TEST_MAIN

// Include statements.
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <Eigen/Core>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include "Tudat/Astrodynamics/States/state.h"
#include "Tudat/Basics/basicFunctions.h"
#include "Tudat/InputOutput/basicInputOutput.h"

// Define Boost test suite.
BOOST_AUTO_TEST_SUITE( test_basic_functions )

//! Test if search for nearest left neighbor using binary search works correctly.
BOOST_AUTO_TEST_CASE( testNearestLeftNeighborUsingBinarySearch )
{
    // Case 1: test Eigen-interface.
    {
        // Populate vector of 10 sorted elements.
        Eigen::VectorXd vectorOfSortedData( 10 );
        vectorOfSortedData << 1.0, 4.5, 10.6, 14.98, 54.65, 88.9, 101.31, 144.63, 180.01, 201.94;

        // Declare vector of target values.
        Eigen::VectorXd vectorOfTargetValues( 5 );
        vectorOfTargetValues << 1.1, 4.6, 10.5, 54.55, 181.63;

        // Declare vector of expected indices.
        Eigen::VectorXi vectorOfExpectedIndices( 5 );
        vectorOfExpectedIndices << 0, 1, 1, 3, 8;

        // Compute nearest left neighbors and check if they match expectations.
        for ( unsigned int i = 0; i < vectorOfTargetValues.rows( ); i++ )
        {
            BOOST_CHECK_EQUAL(
                        vectorOfExpectedIndices[ i ],
                        tudat::basic_functions::computeNearestLeftNeighborUsingBinarySearch(
                            vectorOfSortedData, vectorOfTargetValues[ i ] ) );
        }
    }

    // Case 2: test map-interface with VectorXd.
    {
        // Populate map of 10 sorted elements.
        std::map< double, Eigen::VectorXd > mapOfSortedData;

        Eigen::VectorXd vectorOfData( 1 );
        vectorOfData << 1.0;

        mapOfSortedData[ 0.3 ] = vectorOfData;
        mapOfSortedData[ 3.65 ] = vectorOfData;
        mapOfSortedData[ 43.12 ] = vectorOfData;
        mapOfSortedData[ 2.23 ] = vectorOfData;
        mapOfSortedData[ 1.233 ] = vectorOfData;
        mapOfSortedData[ 6.78 ] = vectorOfData;
        mapOfSortedData[ 0.21 ] = vectorOfData;
        mapOfSortedData[ -1.23 ] = vectorOfData;
        mapOfSortedData[ -931.12 ] = vectorOfData;
        mapOfSortedData[ 124.52 ] = vectorOfData;

        // Declare vector of target values.
        Eigen::VectorXd vectorOfTargetValues( 5 );
        vectorOfTargetValues << -1.22, 3.66, -931.11, 43.12, 0.4;

        // Declare vector of expected indices.
        Eigen::VectorXi vectorOfExpectedIndices( 5 );
        vectorOfExpectedIndices << 1, 6, 0, 8, 3;

        // Compute nearest left neighbors and check if they match expectations.
        for ( unsigned int i = 0; i < vectorOfTargetValues.rows( ); i++ )
        {
            BOOST_CHECK_EQUAL(
                        vectorOfExpectedIndices[ i ],
                        tudat::basic_functions::computeNearestLeftNeighborUsingBinarySearch(
                            mapOfSortedData, vectorOfTargetValues[ i ] ) );
        }
    }

    // Case 2: test map-interface with State.
    {
        // Declare map of 10 sorted elements.
        std::map< double, tudat::State* > mapOfSortedData;

        Eigen::VectorXd vectorOfData( 1 );
        vectorOfData << 1.0;

        tudat::State testState;
        testState.state = vectorOfData;

        mapOfSortedData[ 0.3 ] = &testState;
        mapOfSortedData[ 3.65 ] = &testState;
        mapOfSortedData[ 43.12 ] = &testState;
        mapOfSortedData[ 2.23 ] = &testState;
        mapOfSortedData[ 1.233 ] = &testState;
        mapOfSortedData[ 6.78 ] = &testState;
        mapOfSortedData[ 0.21 ] = &testState;
        mapOfSortedData[ -1.23 ] = &testState;
        mapOfSortedData[ -931.12 ] = &testState;
        mapOfSortedData[ 124.52 ] = &testState;

        // Declare vector of target values.
        Eigen::VectorXd vectorOfTargetValues( 5 );
        vectorOfTargetValues << -1.22, 3.66, -931.11, 43.12, 0.4;

        // Declare vector of expected indices.
        Eigen::VectorXi vectorOfExpectedIndices( 5 );
        vectorOfExpectedIndices << 1, 6, 0, 8, 3;

        // Compute nearest left neighbors and check if they match expectations.
        for ( unsigned int i = 0; i < vectorOfTargetValues.rows( ); i++ )
        {
            BOOST_CHECK_EQUAL(
                        vectorOfExpectedIndices[ i ],
                        tudat::basic_functions::computeNearestLeftNeighborUsingBinarySearch(
                            mapOfSortedData, vectorOfTargetValues[ i ] ) );
        }
    }
}

//! Test if listing all files in specified directory works correctly.
BOOST_AUTO_TEST_CASE( testListAllFilesInDirectory )
{
    // Set path to new directory.
    boost::filesystem::path pathToNewDirectory(
                tudat::input_output::getPackageRootPath( ) + "Basics/TestDirectory"  );

    // Set number of files in directory.
    unsigned int numberOfFiles = 10;

    // Create new directory.
    boost::filesystem::create_directory( pathToNewDirectory );

    // List all files in directory and check that there are none.
    std::vector< boost::filesystem::path > emptyListOfFilenames =
            tudat::basic_functions::listAllFilesInDirectory( pathToNewDirectory.string( ) );

    BOOST_CHECK_EQUAL( emptyListOfFilenames.size( ), 0 );

    // Create test files.
    for ( unsigned int i = 0; i < numberOfFiles; i++ )
    {
        // Create stream new filename.
        std::stringstream newFile;
        newFile << pathToNewDirectory.string( ) << "/testFile" << i << ".txt" << std::endl;

        // Create test file and fill with random contents.
        std::ofstream testFile( newFile.str( ).c_str( ) );
        testFile << "tastes good!\n";
        testFile.close( );
    }

    // List all files in directory and check that they are as expected.
    std::vector< boost::filesystem::path > listOfFilenames =
            tudat::basic_functions::listAllFilesInDirectory( pathToNewDirectory.string( ) );

    for ( unsigned int i = 0; i < listOfFilenames.size( ); i++ )
    {
        std::stringstream newFile;
        newFile << "testFile" << i << ".txt" << std::endl;
        BOOST_CHECK_EQUAL( newFile.str( ), listOfFilenames.at( i ).string( ) );
    }

    // Remove new directory.
    boost::filesystem::remove_all( pathToNewDirectory );
}

// Close Boost test suite.
BOOST_AUTO_TEST_SUITE_END( )

// End of file.




