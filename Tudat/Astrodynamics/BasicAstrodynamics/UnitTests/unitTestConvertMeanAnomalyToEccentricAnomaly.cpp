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
 *      111215    T. Secretin       First creation of the code.
 *      111221    T. Secretin       Removed memory leaks. Added test for circular and
 *                                  near-parabolic orbits, as well as for negative eccentricities.
 *      120326    D. Dirkx          Changed raw pointers to shared pointers.
 *      120421    K. Kumar          Updated test fixtures and cases to use updated conversion
 *                                  object.
 *      120813    P. Musegaas       Updated unit test to new root finding structure.
 *      120822    P. Musegaas       Added near-parabolic functionality, with accompanying unit
 *                                  tests. Added two big random unit tests, to test behaviour that
 *                                  tends to be chaotic for some values with different implemen-
 *                                  tations of the function. Added error file writing function.
 *                                  Added small test for specifying other initial guess. Updated
 *                                  tests for invalid input.
 *      120903    P. Musegaas       Improved random test (does not fail on mean anomaly of 0.0).
 *                                  Decreased number of random values for random tests.
 *      121205    P. Musegaas       Updated code to final version of rootfinders.
 *
 *    References
 *      http://www.esa.int/gsp/ACT/doc/INF/Code/globopt/GTOPtoolbox.rar
 *      ODTBX tolbox: http://sourceforge.net/projects/odtbx/
 *
 */

#define BOOST_TEST_MAIN

#include <ctime>
#include <fstream>
#include <string>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/test/unit_test.hpp>

#include <TudatCore/Astrodynamics/BasicAstrodynamics/unitConversions.h>
#include <TudatCore/Astrodynamics/BasicAstrodynamics/orbitalElementConversions.h>
#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/BasicAstrodynamics/convertMeanAnomalyToEccentricAnomaly.h"
#include "Tudat/InputOutput/basicInputOutput.h"

namespace tudat
{
namespace unit_tests
{

//! Conversion test fixture.
/*!
 * Conversion test fixture used by the Boost unit test framework. This code is executed before each
 * test.
 */
struct conversion_test_fixture
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    conversion_test_fixture( )
    {
        toleranceOrbitalElementConversion = 1.0e-13;
        toleranceOrbitalElementConversionNearParabolic = 1.0e-9;
    }

    //! Conversion tolerance to test against.
    /*!
     * Conversion tolerance to test against.
     */
    double toleranceOrbitalElementConversion;

    //! Conversion tolerance to test against for near-parabolic cases.
    /*!
     * Conversion tolerance to test against for near-parabolic cases.
     */
    double toleranceOrbitalElementConversionNearParabolic;

    //! Convert mean anomaly to eccentric anomaly.
    /*!
     * Converts mean anomaly to eccentric anomaly by creating a conversion object to test and
     * executing the conversion.
     * \param eccentricity Eccentricity [-].
     * \param meanAnomaly Mean anomaly [rad].
     * \param useDefaultInitialGuess Boolean specifying whether to use default initial guess [-].
     * \param initialGuessForRootFinder Initial guess for rootfinder [rad].
     * \return eccentricAnomaly Eccentric anomaly [rad].
     */
    double convertMeanAnomalyToEccentricAnomaly( const double eccentricity,
                                                 const double meanAnomaly,
                                                 const bool useDefaultInitialGuess = true,
                                                 const double initialGuess = TUDAT_NAN )
    {
        // Conversion object to test; mean anomaly to eccentric anomaly conversion.
        basic_astrodynamics::orbital_element_conversions::ConvertMeanAnomalyToEccentricAnomaly
                meanToEccentricAnomaly( eccentricity, meanAnomaly, useDefaultInitialGuess,
                                        initialGuess );

        // Convert to eccentric anomaly and return.
        return meanToEccentricAnomaly.convert( );
    }

protected:

private:
};

//! Error writing function.
/*!
 * This function writes the input values that led to errors to a unique file, if any errors occured
 * during the random tests. To make the file unique, the date and time of execution is added. An
 * error message also shows the location of the file.
 * \param eccentricities A vector containing the eccentricities that caused a failure.
 * \param meanAnomalies A vector containing the mean anomalies that caused a failure.
 * \param testName A string specifying the name of the test that failed.
 */
void writeErrorsToFile( std::vector< double > eccentricities, std::vector< double > meanAnomalies,
                        std::string testName )
{
    // Obtain the current time.
    const boost::posix_time::ptime now = boost::posix_time::second_clock::local_time( );

    // Make a string containing the output file name. This output file is tagged with the date and
    // time at which the code was executed. The default date format is: YYYYMMDDTHHMMSS, in which T
    // separates date and time.
    const std::string outputFileName = tudat::input_output::getTudatRootPath( ) +
                                       "Astrodynamics/BasicAstrodynamics/UnitTests/" +
                                       "ErrorReportConversionMeanToEccentricAnomaly" +
                                       testName + "RunAt" + boost::posix_time::to_iso_string( now )
                                       + ".txt";

    // Make a stream to a file.
    std::ofstream errorFile( outputFileName.c_str( ) );

    // Write an introduction in the file explaining what happened. 70 lines long.
    errorFile << "This error report was generated because the unit test for the" << std::endl
              << "conversion of mean to eccentric anomaly has failed in one of the" << std::endl
              << "random tests. To ensure the data for which it failed is not lost," << std::endl
              << "the corresponding input variables for these cases are listed below." << std::endl
              << "Please report a bug on the Tudat website (tudat.tudelft.nl), with"<< std::endl
              << "these values, so that someone will look into it and the code can be"<< std::endl
              << "improved." << std::endl << std::endl
              << "Eccentricities:           Mean anomalies:" << std::endl;

    // Set the precision for the output of the variables to 16 digits.
    errorFile.precision( 16 );

    // Add the corresponding eccentricities and mean anomalies at neatly arranged positions.
    for ( unsigned int counter = 0; counter < eccentricities.size( ); counter++ )
    {
        errorFile << std::setw( 25 ) << eccentricities[ counter ]
                  << std::setw( 25 ) << meanAnomalies[ counter ] << std::endl;
    }
    errorFile.close( );

    // Add an error message specifying the file that the values have been written to.
    std::cerr << "One or multiple errors occurred during random sampling. " << std::endl
              << "The values leading to these errors have been written to the following file: "
              << std::endl << outputFileName;
}

// Declare Boost fixture test suite.
BOOST_FIXTURE_TEST_SUITE( testsuite_convertMeanAnomalyToEccentricAnomaly, conversion_test_fixture )

//! Test 1: Test conversion for circular orbits.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_circular )
{
    // Set test value for eccentricity.
    const double testEccentricity = 0.0;

    // Set test value for mean anomaly.
    const double testMeanAnomaly = 1.0472;

    // Set reference value for eccentric anomaly;
    const double referenceEccentricAnomaly = 1.0472;

    // Compute eccentric anomaly.
    const double eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly( testEccentricity,
                                                                          testMeanAnomaly );

    // Check if computed eccentric anomaly is less than error tolerance.
    BOOST_CHECK_CLOSE_FRACTION( eccentricAnomaly, referenceEccentricAnomaly,
                                toleranceOrbitalElementConversion );
}

//! Test 2: Test conversion for valid range of eccentricities.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_range )
{
    // Set test values for eccentricity.
    const double arrayOfTestEccentricities [4] = { 0.01671, 0.43582, 0.78514, 0.91525 };

    // Set test values for mean anomaly.
    const double arrayOfTestMeanAnomalies [4] = {
        tudat::unit_conversions::convertDegreesToRadians( 60.0 ),
        tudat::unit_conversions::convertDegreesToRadians( 90.0 ),
        tudat::unit_conversions::convertDegreesToRadians( 120.0 ),
        tudat::unit_conversions::convertDegreesToRadians( 220.0 ) };

    // Set reference values for eccentric anomaly. These were obtained using GTOP.
    const double arrayOfReferenceEccentricAnomalies [4] = { 1.06178920406832,
                                                            1.97200731113253,
                                                            2.5392410896466,
                                                            3.51006218528448 };

    // Loop over sets of data.
    for ( int i = 0; i < 4; i++ )
    {
        // Compute eccentric anomaly.
        const double eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly(
                    arrayOfTestEccentricities[ i ], arrayOfTestMeanAnomalies[ i ] );

        // Check if computed eccentric anomaly is less than error tolerance.
        BOOST_CHECK_CLOSE_FRACTION( eccentricAnomaly, arrayOfReferenceEccentricAnomalies[ i ],
                                    toleranceOrbitalElementConversion );
    }
}

//! Test 3: Test conversion for negative eccentricities.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_negative )
{
    // Set test value for eccentricity.
    const double testEccentricity = -0.5;

    // Set test value for mean anomaly.
    const double testMeanAnomaly = 1.0472;

    // Check if a runtime error is thrown if the anomaly is converted for this eccentricity.
    BOOST_CHECK_THROW( convertMeanAnomalyToEccentricAnomaly( testEccentricity, testMeanAnomaly ),
                       std::runtime_error );
}

//! Test 4: Test conversion for eccentricity above 1.0.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_TooHigh )
{
    // Set test value for eccentricity.
    const double testEccentricity = 2.0;

    // Set test value for mean anomaly.
    const double testMeanAnomaly = 1.0472;

    // Check if a runtime error is thrown if the anomaly is converted for this eccentricity.
    BOOST_CHECK_THROW( convertMeanAnomalyToEccentricAnomaly( testEccentricity, testMeanAnomaly ),
                       std::runtime_error );
}

//! Test 5: Test conversion for near-parabolic orbits.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_nearParabolic )
{
    // Set test value for eccentricity, which is just below  1.0.
    const double testEccentricity = 1.0 - 1.0e-15;

    // Set test values for mean anomaly, to verify that a wide range can be handled. The set was
    // selected such that it contains all possible limit cases, and a wide enough variety of random
    // values.
   const double arrayOfTestMeanAnomalies [ 17 ] = { 0.0,
                                                    1.0e-10,
                                                    0.5,
                                                    mathematics::PI / 2 - 1.0e-10,
                                                    mathematics::PI / 2,
                                                    mathematics::PI / 2 + 1.0e-10,
                                                    2.5,
                                                    mathematics::PI - 1.0e-10,
                                                    mathematics::PI,
                                                    mathematics::PI + 1.0e-10,
                                                    4.0,
                                                    3.0 / 2.0 * mathematics::PI - 1.0e-10,
                                                    3.0 / 2.0 * mathematics::PI,
                                                    3.0 / 2.0 * mathematics::PI + 1.0e-10,
                                                    5.5,
                                                    2.0 * mathematics::PI - 1.0e-10,
                                                    2.0 * mathematics::PI };

    // Set the expected eccentric anomalies corresponding to the corresponding test mean anomaly
    // array. These values were obtained using the convertMeanAnomalyToEccentricAnomaly method,
    // and subsequently verified using the eccentric -> mean anomaly method, by running GTOP code
    // with the same values and by running ODTBX code with the same values. The iteration scheme
    // converges very rapidly except for values close to zero or 2*n*pi. Still Newton Raphson
    // converges in 20-40 iterations for these cases.
    const double arrayOfExpectedEccentricAnomalies [ 17 ] = { 0.0,
                                                              0.000843432672832182,
                                                              1.49730038909589,
                                                              2.30988145995031,
                                                              2.30988146001006,
                                                              2.30988146006981,
                                                              2.81798706288006,
                                                              3.14159265353979,
                                                              3.14159265358979,
                                                              3.14159265363979,
                                                              3.57764001198758,
                                                              3.97330384710978,
                                                              3.97330384722928,
                                                              3.97330384722972,
                                                              4.51869928040234,
                                                              6.28234187379524,
                                                              0.0 };

    // Test the values which are supposed to be 0.0.
    double eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly(
                testEccentricity, arrayOfTestMeanAnomalies[ 0 ] );
    BOOST_CHECK_SMALL( eccentricAnomaly, toleranceOrbitalElementConversionNearParabolic );
    eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly(
                testEccentricity, arrayOfTestMeanAnomalies[ 16 ] );
    BOOST_CHECK_SMALL( eccentricAnomaly, toleranceOrbitalElementConversionNearParabolic );

    // Test the values that are supposed to be equal to certain other values.
    for ( int counter = 1; counter < 16; counter++ )
    {
        // Compute eccentric anomaly.
        eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly(
                    testEccentricity, arrayOfTestMeanAnomalies[ counter ] );

        // Check whether the expected value is set to the eccentric anomaly.
        BOOST_CHECK_CLOSE_FRACTION( eccentricAnomaly, arrayOfExpectedEccentricAnomalies[ counter ],
                                    toleranceOrbitalElementConversionNearParabolic );
    }
}

//! Test 6: Test large number of anomalies for near-parabolic orbits.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_nearParabolic_random )
{
    // Create vectors that will store the input variables of a test that resulted in an error, such
    // that the error scenario can be reproduced.
    std::vector< double > failedMeanAnomalies, failedEccentricities;

    // Boolean that will be set true if a runtime error occurred.
    bool aRuntimeErrorOccurred = false;

    // Set test value for eccentricity, which is just below  1.0.
    const double testEccentricity = 1.0 - 1.0e-15;

    // Initialize both test and reverse calculated mean anomaly and the eccentric anomaly.
    double testMeanAnomaly, reverseCalculatedMeanAnomaly, eccentricAnomaly = 0.0;

    // Instantiate random number generator.
    boost::mt19937 randomNumbergenerator( time( 0 ) );
    boost::random::uniform_real_distribution< > distribution0To2Pi( 0.0, 2 * mathematics::PI );
    boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > >
            generateRandomNumber0To2Pi( randomNumbergenerator, distribution0To2Pi );

    // Specify the number of random samples should be taken. A test of 100,000,000 was performed
    // by the author before the code was submitted. This test remains included to verify that any
    // future method will not fail. The behaviour of the conversion is namely very sensitive and
    // non-converging cases are highly sensitive to input values and the initial guess that is used.
    const int numberOfSamples = 10000;

    // Perform the conversion for the specified number of samples and test whether the values that
    // are subsequently converted back match the initial values.
    for ( int counter = 0; counter < numberOfSamples; counter++ )
    {
        // Set random value in test mean anomaly.
        testMeanAnomaly = generateRandomNumber0To2Pi( );

        // If the Rootfinder does not converge, it will produce a runtime error. In order to make
        // sure that these values that led to the error will not be lost, they will be stored in
        // the failed input data vectors. To do so, a try-catch sequence is used.
        try
        {
            // Compute eccentric anomaly.
            eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly( testEccentricity,
                                                                     testMeanAnomaly );
        }
        catch( std::runtime_error )
        {
            // Store the fact that a runtime error occurred, such that the values will be stored.
            aRuntimeErrorOccurred = true;
        }

        // Calculate the mean anomaly from this eccentric anomaly.
        reverseCalculatedMeanAnomaly = orbital_element_conversions::
                convertEccentricAnomalyToMeanAnomaly( eccentricAnomaly, testEccentricity );

        // Test whether the computed mean anomaly is equal to the mean anomaly from the input and
        // that no runtime errors occurred. If an error was found, store the values leading to this
        // error in a vector for later use. '!' operator is there to ensure that a NaN value will
        // result in the values being written away. It is also checked that the mean anomaly is
        // not equal to 0.0, because that would result in falsely writing an error.
        if ( ( ( !( std::abs( testMeanAnomaly - reverseCalculatedMeanAnomaly ) / testMeanAnomaly <
                toleranceOrbitalElementConversion ) ) && ( testMeanAnomaly != 0.0 ) )
             || aRuntimeErrorOccurred )
        {
            failedMeanAnomalies.push_back( testMeanAnomaly );
            failedEccentricities.push_back( testEccentricity );
        }

        // Reset boolean.
        aRuntimeErrorOccurred = false;
    }

    // Check that no values have been written to the failedMeanAnomalies vector.  If so, this test
    // is passed. Otherwisely these values will be written away and this test will fail.
    BOOST_CHECK( failedMeanAnomalies.empty( ) );

    // If the vector is not empty, write the failed cases of this test case to a file.
    if ( !( failedMeanAnomalies.empty( ) ) )
    {
        writeErrorsToFile( failedEccentricities, failedMeanAnomalies, "Test6" );
    }
}

//! Test 7: Test large number of anomalies with a large number of eccentricities.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_random )
{
    // Create vectors that will store the input variables of a test that resulted in an error, such
    // that the error scenario can be reproduced.
    std::vector< double > failedMeanAnomalies, failedEccentricities;

    // Boolean that will be set true if a runtime error occurred.
    bool aRuntimeErrorOccurred = false;

    // Initialize both test and reverse calculated mean anomaly, the eccentric anomaly and the
    // eccentricity.
    double testEccentricity, testMeanAnomaly, reverseCalculatedMeanAnomaly, eccentricAnomaly = 0.0;

    // Instantiate random number generators. One for the mean anomaly generation, from 0 to 2pi,
    // another one for the eccentricity generation, from 0 to 1.0 - 1.0e-11. This was selected
    // lower than the limit (1.0 - 1.0e-15, because this could invoke machine precision problems
    // with the sharp tolerance used: 1.0e-13 instead of 1.0e-9).
    boost::mt19937 randomNumbergenerator( time( 0 ) );
    boost::random::uniform_real_distribution< > distribution0To2Pi( 0.0, 2 * mathematics::PI );
    boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > >
            generateRandomNumber0To2Pi( randomNumbergenerator, distribution0To2Pi );
    boost::random::uniform_real_distribution< > distribution0To1( 0.0, 1.0 - 1.0e-11 );
    boost::variate_generator< boost::mt19937&, boost::random::uniform_real_distribution < > >
            generateRandomNumber0To1( randomNumbergenerator, distribution0To1 );

    // Specify the number of random samples should be taken. A test of 100,000,000 was performed
    // by the author before the code was submitted. This test remains included to verify that any
    // future method will not fail. The behaviour of the conversion is namely very sensitive and
    // non-converging cases are highly sensitive to input values and the initial guess that is used.
    const int numberOfSamples = 10000;

    // Perform the conversion for the specified number of samples and test whether the values that
    // are subsequently converted back match the initial values.
    for ( int counter = 0; counter < numberOfSamples; counter++ )
    {
        // Set random value in test mean anomaly and the test eccentricity.
        testMeanAnomaly = generateRandomNumber0To2Pi( );
        testEccentricity = generateRandomNumber0To1( );

        // If the Rootfinder does not converge, it will produce a runtime error. In order to make
        // sure that these values that led to the error will not be lost, they will be stored in
        // the failed input data vectors. To do so, a try-catch sequence is used.
        try
        {
            // Compute eccentric anomaly.
            eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly( testEccentricity,
                                                                     testMeanAnomaly );
        }
        catch( std::runtime_error )
        {
            // Store the fact that a runtime error occurred, such that the values will be stored.
            aRuntimeErrorOccurred = true;
        }

        // Calculate the mean anomaly from this eccentric anomaly.
        reverseCalculatedMeanAnomaly = orbital_element_conversions::
                convertEccentricAnomalyToMeanAnomaly( eccentricAnomaly, testEccentricity );

        // Test whether the computed mean anomaly is equal to the mean anomaly from the input and
        // that no runtime errors occurred. If an error was found, store the values leading to this
        // error in a vector for later use. '!' operator is there to ensure that a NaN value will
        // result in the values being written away. It is also checked that the mean anomaly is
        // not equal to 0.0, because that would result in falsely writing an error.
        if ( ( ( !( std::abs( testMeanAnomaly - reverseCalculatedMeanAnomaly ) / testMeanAnomaly <
                toleranceOrbitalElementConversion ) ) && ( testMeanAnomaly != 0.0 ) )
             || aRuntimeErrorOccurred )
        {
            failedMeanAnomalies.push_back( testMeanAnomaly );
            failedEccentricities.push_back( testEccentricity );
        }

        // Reset boolean.
        aRuntimeErrorOccurred = false;
    }

    // Check that no values have been written to the failedMeanAnomalies vector.  If so, this test
    // is passed. Otherwisely these values will be written away and this test will fail.
    BOOST_CHECK( failedMeanAnomalies.empty( ) );

    // If the vector is not empty, write the failed cases of this test case to a file.
    if ( !( failedMeanAnomalies.empty( ) ) )
    {
        writeErrorsToFile( failedEccentricities, failedMeanAnomalies, "Test7" );
    }
}

//! Test 8: Test functionality of specifying the initial guess. Case contained in Test 2 also.
BOOST_AUTO_TEST_CASE( test_convertMeanAnomalyToEccentricAnomaly_specificInitialGuess )
{
    // Set test value for eccentricity.
    const double testEccentricity = 0.78514;

    // Set test value for mean anomaly.
    const double testMeanAnomaly = tudat::unit_conversions::convertDegreesToRadians( 120.0 );

    // Set reference value for eccentric anomaly;
    const double referenceEccentricAnomaly = 2.5392410896466027;

    // Specify initial guess.
    const bool useDefaultGuess = false;
    const double initialGuess = mathematics::PI;

    // Compute eccentric anomaly.
    const double eccentricAnomaly = convertMeanAnomalyToEccentricAnomaly(
                testEccentricity, testMeanAnomaly, useDefaultGuess, initialGuess );

    // Check if computed eccentric anomaly is less than error tolerance.
    BOOST_CHECK_CLOSE( eccentricAnomaly,
                       referenceEccentricAnomaly,
                       toleranceOrbitalElementConversion );
}

// End Boost test suite.
BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
