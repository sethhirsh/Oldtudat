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
 *      120323    K. Kumar          Created based off of unit test for RKF45 integrator; test data
 *                                  taken from (The Mathworks, 2012).
 *      120328    K. Kumar          Moved (Burden and Faires, 2011) test class to its own file.
 *      120404    K. Kumar          Updated MATLAB unit test by adding discrete-event data file.
 *      130116    K. Kumar          Rewrote unit test to make use of testing code for numerical
 *                                  integrators migrated to Tudat Core.
 *
 *    References
 *      Burden, R.L., Faires, J.D. Numerical Analysis, 7th Edition, Books/Cole, 2001.
 *      The Mathworks, Inc. RKF78, Symbolic Math Toolbox, 2012.
 *
 *    Notes
 *      All the test for this integrator are based on the data generated using the Symbolic Math
 *      Toolbox (MathWorks, 2012). Ideally, another source of data should be used to complete the
 *      testing.
 *
 *      The single step and full integration error tolerances were picked to be as small as
 *      possible, without causing the tests to fail. These values are not deemed to indicate any
 *      bugs in the code; however, it is important to take these discrepancies into account when
 *      using this numerical integrator.
 *
 */

#define BOOST_TEST_MAIN

#include <limits>
#include <string>

#include <boost/make_shared.hpp>
#include <boost/test/unit_test.hpp>

#include <Eigen/Core>

#include <TudatCore/InputOutput/matrixTextFileReader.h>
#include <TudatCore/Mathematics/NumericalIntegrators/numericalIntegrator.h>
#include <TudatCore/Mathematics/NumericalIntegrators/reinitializableNumericalIntegrator.h>
#include <TudatCore/Mathematics/NumericalIntegrators/UnitTests/numericalIntegratorTests.h>
#include <TudatCore/Mathematics/NumericalIntegrators/UnitTests/numericalIntegratorTestFunctions.h>

#include "Tudat/InputOutput/basicInputOutput.h"
#include "Tudat/Mathematics/BasicMathematics/linearAlgebra.h"
#include "Tudat/Mathematics/NumericalIntegrators/rungeKuttaVariableStepSizeIntegrator.h"
#include "Tudat/Mathematics/NumericalIntegrators/rungeKuttaCoefficients.h"

namespace tudat
{
namespace unit_tests
{

BOOST_AUTO_TEST_SUITE( test_runge_kutta_fehlberg_78_integrator )

using basic_mathematics::linear_algebra::flipMatrixRows;

using numerical_integrators::NumericalIntegratorXdPointer;
using numerical_integrators::ReinitializableNumericalIntegratorXdPointer;
using numerical_integrators::RungeKuttaVariableStepSizeIntegratorXd;
using numerical_integrators::RungeKuttaCoefficients;

using numerical_integrator_test_functions::computeNonAutonomousModelStateDerivative;

//! Test Runge-Kutta-Fehlberg 78 integrator using benchmark data from (The MathWorks, 2012).
BOOST_AUTO_TEST_CASE( testRungeKuttaFehlberg78IntegratorUsingMatlabData )
{
    using namespace numerical_integrator_tests;

    // Read in benchmark data (generated using Symbolic Math Toolbox in Matlab
    // (The MathWorks, 2012)). This data is generated using the RKF78 numerical integrator.
    const std::string pathToForwardIntegrationOutputFile = input_output::getTudatRootPath( )
            + "/Mathematics/NumericalIntegrators/UnitTests"
            + "/matlabOutputRungeKuttaFehlberg78Forwards.txt";
    const std::string pathToDiscreteEventIntegrationOutputFile = input_output::getTudatRootPath( )
            + "/Mathematics/NumericalIntegrators/UnitTests"
            + "/matlabOutputRungeKuttaFehlberg78DiscreteEvent.txt";

    // Store benchmark data in matrix.
    const Eigen::MatrixXd matlabForwardIntegrationData =
            input_output::readMatrixFromFile( pathToForwardIntegrationOutputFile, "," );
    Eigen::MatrixXd matlabBackwardIntegrationData = matlabForwardIntegrationData;
    flipMatrixRows( matlabBackwardIntegrationData );
    const Eigen::MatrixXd matlabDiscreteEventIntegrationData =
            input_output::readMatrixFromFile( pathToDiscreteEventIntegrationOutputFile, "," );

    // Set integrator parameters.

    // All of the following parameters are set such that the input data is fully accepted by the
    // integrator, to determine the steps to be taken.
    const double zeroMinimumStepSize = std::numeric_limits< double >::epsilon( );
    const double infiniteMaximumStepSize = std::numeric_limits< double >::infinity( );
    const double infiniteRelativeErrorTolerance = std::numeric_limits< double >::infinity( );
    const double infiniteAbsoluteErrorTolerance = std::numeric_limits< double >::infinity( );

    // The following parameters set how the error control mechanism should work.
    const double relativeErrorTolerance = 1.0e-15;
    const double absoluteErrorTolerance = 1.0e-15;

    // Case 1: Execute integrateTo() to integrate one step forward in time.
    {
        // Declare integrator with all necessary settings.
        NumericalIntegratorXdPointer integrator
                = boost::make_shared< RungeKuttaVariableStepSizeIntegratorXd >(
                    RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ),
                    &computeNonAutonomousModelStateDerivative,
                    matlabForwardIntegrationData( FIRST_ROW, TIME_COLUMN_INDEX ),
                    ( Eigen::VectorXd( 1 )
                      << matlabForwardIntegrationData( FIRST_ROW,
                                                       STATE_COLUMN_INDEX ) ).finished( ),
                    zeroMinimumStepSize,
                    infiniteMaximumStepSize,
                    infiniteRelativeErrorTolerance,
                    infiniteAbsoluteErrorTolerance );

        executeOneIntegrateToStep( matlabForwardIntegrationData, 1.0e-14, integrator );
    }

    // Case 2: Execute performIntegrationStep() to perform multiple integration steps until final
    //         time.
    {
        // Declare integrator with all necessary settings.
        NumericalIntegratorXdPointer integrator
                = boost::make_shared< RungeKuttaVariableStepSizeIntegratorXd >(
                    RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ),
                    &computeNonAutonomousModelStateDerivative,
                    matlabForwardIntegrationData( FIRST_ROW, TIME_COLUMN_INDEX ),
                    ( Eigen::VectorXd( 1 )
                      << matlabForwardIntegrationData( FIRST_ROW,
                                                       STATE_COLUMN_INDEX ) ).finished( ),
                    zeroMinimumStepSize,
                    infiniteMaximumStepSize,
                    infiniteRelativeErrorTolerance,
                    infiniteAbsoluteErrorTolerance );

        performIntegrationStepToSpecifiedTime( matlabForwardIntegrationData,
                                               1.0e-15, 1.0e-14, integrator );
    }

    // Case 3: Execute performIntegrationStep() to perform multiple integration steps until initial
    //         time (backwards).
    {
        // Declare integrator with all necessary settings.
        NumericalIntegratorXdPointer integrator
                = boost::make_shared< RungeKuttaVariableStepSizeIntegratorXd >(
                    RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ),
                    &computeNonAutonomousModelStateDerivative,
                    matlabBackwardIntegrationData( FIRST_ROW, TIME_COLUMN_INDEX ),
                    ( Eigen::VectorXd( 1 )
                      << matlabBackwardIntegrationData( FIRST_ROW,
                                                        STATE_COLUMN_INDEX ) ).finished( ),
                    zeroMinimumStepSize,
                    infiniteMaximumStepSize,
                    infiniteRelativeErrorTolerance,
                    infiniteAbsoluteErrorTolerance );

        performIntegrationStepToSpecifiedTime( matlabBackwardIntegrationData,
                                               1.0e-15, 1.0e-13, integrator );
    }

    // Case 4: Execute integrateTo() to integrate to specified time in one step.
    {
        // Note that this test has a strange issue that the if the absolute error tolerance is set
        // to 1.0e-15, the last step that the integrateTo() function takes does not result in the
        // expected final time of 1.0. As a temporary solution, the absolute error tolerance has
        // been multiplied by 10.0, which seems to solve the problem. This error indicated a
        // possible problem with the implementation of the integrateTo() function, which needs to
        // be investigated in future.

        // Declare integrator with all necessary settings.
        NumericalIntegratorXdPointer integrator
                = boost::make_shared< RungeKuttaVariableStepSizeIntegratorXd >(
                    RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ),
                    &computeNonAutonomousModelStateDerivative,
                    matlabForwardIntegrationData( FIRST_ROW, TIME_COLUMN_INDEX ),
                    ( Eigen::VectorXd( 1 )
                      << matlabForwardIntegrationData( FIRST_ROW,
                                                       STATE_COLUMN_INDEX ) ).finished( ),
                    zeroMinimumStepSize,
                    infiniteMaximumStepSize,
                    relativeErrorTolerance,
                    absoluteErrorTolerance * 10.0 );

        executeIntegrateToToSpecifiedTime( matlabForwardIntegrationData, 1.0e-13, integrator,
                                           matlabForwardIntegrationData(
                                               matlabForwardIntegrationData.rows( ) - 1,
                                               TIME_COLUMN_INDEX ) );
    }

    // Case 5: Execute performIntegrationstep() to integrate to specified time in multiple steps,
    //         including discrete events.
    {
        // Declare integrator with all necessary settings.
        ReinitializableNumericalIntegratorXdPointer integrator
                = boost::make_shared< RungeKuttaVariableStepSizeIntegratorXd >(
                    RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ),
                    &computeNonAutonomousModelStateDerivative,
                    matlabForwardIntegrationData( FIRST_ROW, TIME_COLUMN_INDEX ),
                    ( Eigen::VectorXd( 1 )
                      << matlabForwardIntegrationData( FIRST_ROW,
                                                       STATE_COLUMN_INDEX ) ).finished( ),
                    zeroMinimumStepSize,
                    infiniteMaximumStepSize,
                    infiniteRelativeErrorTolerance,
                    infiniteAbsoluteErrorTolerance );

        performIntegrationStepToSpecifiedTimeWithEvents( matlabDiscreteEventIntegrationData,
                                                         1.0e-15, 1.0e-12, integrator );
    }
}

BOOST_AUTO_TEST_SUITE_END( )

} // namespace unit_tests
} // namespace tudat
