/*!   Copyright (c) 2010-2012 Delft University of Technology.
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
 *      120203    B. Tong Minh      Copied Euler unit test.
 *      120207    K. Kumar          Adapted to use modified benchmark functions in Tudat Core.
 *
 *    References
 *      Burden, R.L., Faires, J.D. Numerical Analysis, 7th Edition, Books/Cole, 2001.
 *
 */

// Include statements.
#include <iostream>
#include <limits>
#include <TudatCore/Mathematics/NumericalIntegrators/UnitTests/benchmarkFunctions.h>
#include "Tudat/Mathematics/NumericalIntegrators/euler.h"

//! Using declaration of the EulerIntegratorXd.
using tudat::mathematics::numerical_integrators::EulerIntegratorXd;

//! Test the result of the Euler integrator.
/*!
 * Test the result of the Euler integrator.
 * \param stateDerivativeFunction Function pointer to the state derivative function.
 * \param intervalStart The start of the integration interval.
 * \param intervalEnd The end of the integration interval.
 * \param stepSize The step size to take.
 * \param initialState_ The initial state.
 * \param expectedState Expected final state.
 * \param tolerance Tolerance when comparing.
 * \return True if actual final state equals the expected final state within the specified
 *          tolerance.
 */
bool testEulerIntegrator( const EulerIntegratorXd::StateDerivativeFunction& stateDerivativeFunction,
                          const double intervalStart, const double intervalEnd, const double stepSize,
                          const Eigen::VectorXd& initialState_, const Eigen::VectorXd expectedState,
                          const double tolerance )
{
    using std::endl;

    // Create forward Euler, fixed stepsize integrator.
    {
        EulerIntegratorXd integrator( stateDerivativeFunction, intervalStart, initialState_ );

        Eigen::VectorXd finalState = integrator.integrateTo( intervalEnd, stepSize );

        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::fabs( integrator.getCurrentInterval( ) - intervalEnd ) / intervalEnd >
             std::numeric_limits< double >::epsilon( ) )
        {
            std::cerr << "EulerIntegrator end interval mismatch" << endl
                      << "Expected interval end: " << intervalEnd << endl
                      << "Actual interval end: " << integrator.getCurrentInterval( ) << endl;
            return false;
        }

        // Compute differences between computed and expected results and generate
        // cerr statement if test fails.
        if ( !expectedState.isApprox( finalState, tolerance ) )
        {
            std::cerr << "EulerIntegrator test failed" << endl
                      << "Expected result: " << expectedState << endl
                      << "Actual result: " << finalState << endl;
            return false;
        }
    }

    // Try the same again, but in two steps
    {
        EulerIntegratorXd integrator( stateDerivativeFunction, intervalStart, initialState_ );

        const double intermediateInterval = intervalStart + ( intervalEnd - intervalStart ) / 2.0;

        const Eigen::VectorXd intermediateState = integrator.integrateTo( intermediateInterval,
                                                                          stepSize );
        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::fabs( integrator.getCurrentInterval( ) - intermediateInterval ) /
             intermediateInterval >
             std::numeric_limits< double >::epsilon( ) )
        {
            std::cerr << "EulerIntegrator intermediate interval mismatch" << endl
                      << "Expected intermediate interval: " << intermediateInterval << endl
                      << "Actual intermediate interval: " << integrator.getCurrentInterval( )
                      << endl;
            return false;
        }

        // Integrate to the end
        Eigen::VectorXd finalState = integrator.integrateTo( intervalEnd, stepSize );

        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::fabs( integrator.getCurrentInterval( ) - intervalEnd ) / intervalEnd >
             std::numeric_limits< double >::epsilon( ) )
        {
            std::cerr << "EulerIntegrator end interval mismatch" << endl
                      << "Expected interval end: " << intervalEnd << endl
                      << "Actual interval end: " << integrator.getCurrentInterval( ) << endl;
            return false;
        }

        // Compute differences between computed and expected results and generate
        // cerr statement if test fails.
        if ( !expectedState.isApprox( finalState, tolerance ) )
        {
            std::cerr << "EulerIntegrator test failed" << endl
                      << "Expected result: " << expectedState << endl
                      << "Actual result: " << finalState << endl;
            return false;
        }

        integrator.performIntegrationStep( stepSize );
        if ( !integrator.rollbackToPreviousState( ) )
        {
            std::cerr << "EulerIntegrator rollback returned false" << endl;
            return false;
        }
        if ( std::fabs( integrator.getCurrentInterval( ) - intervalEnd ) /
             intervalEnd > std::numeric_limits< double >::epsilon( ) )
        {
            std::cerr << "EulerIntegrator rollback to invalid interval" << endl
                      << "Expected result: " << intervalEnd << endl
                      << "Actual result: " << integrator.getCurrentInterval( ) << endl;
            return false;
        }
        if ( integrator.getCurrentState( ) != finalState )
        {
            std::cerr << "EulerIntegrator rollback to invalid state" << endl
                      << "Expected result: " << finalState << endl
                      << "Actual result: " << integrator.getCurrentState( ) << endl;
            return false;
        }

        if ( integrator.rollbackToPreviousState( ) )
        {
            std::cerr << "EulerIntegrator second rollback returned true" << endl;
            return false;
        }
    }

    return true;
}

//! Test different types of states and state derivatives
/*!
 * Test if different types of states and state derivatives work. If something
 * is broken, then a compile time error will be generated.
 * \return Unconditionally true
 */
bool testDifferentStateAndStateDerivativeTypes( )
{
    using tudat::mathematics::numerical_integrators::computeZeroStateDerivative;
    tudat::mathematics::numerical_integrators::EulerIntegrator
            < double, Eigen::Vector3d, Eigen::VectorXd > integrator( &computeZeroStateDerivative,
                                                                     0.0, Eigen::Vector3d::Zero( ) );
    integrator.integrateTo( 1.0, 0.1 );

    // No need to test anything, this is just to check compile time errors.
    return true;
}


//! Test implementation of 4th-order, fixed stepsize, Runge-Kutta integrator.
int main( )
{

    // Test result initialised to false.
    bool testEulerIsOk = true;

    using namespace tudat::mathematics::numerical_integrators;
    std::map< BenchmarkFunctions, BenchmarkFunction >& benchmarkFunctions =
             getBenchmarkFunctions( );


    // Test with x_dot = 0, which results in x_f = x_0.
    {
        testEulerIsOk &= testEulerIntegrator(
                    benchmarkFunctions[ Zero ].pointerToStateDerivativeFunction_,
                    benchmarkFunctions[ Zero ].initialInterval_,
                    benchmarkFunctions[ Zero ].endInterval_, 0.2,
                    benchmarkFunctions[ Zero ].initialState_,
                    benchmarkFunctions[ Zero ].endState_,
                    std::numeric_limits< double >::epsilon( ) );
    }

    // Test with x_dot = 1, which results in x_f = x_0 + t_f.
    {
        testEulerIsOk &= testEulerIntegrator(
                    benchmarkFunctions[ Constant ].pointerToStateDerivativeFunction_,
                    benchmarkFunctions[ Constant ].initialInterval_,
                    benchmarkFunctions[ Constant ].endInterval_, 0.2,
                    benchmarkFunctions[ Constant ].initialState_,
                    benchmarkFunctions[ Constant ].endState_,
                    std::numeric_limits< double >::epsilon( ) );
    }

    // Test with x_dot = x, which results in x_f = x0 * exp( t_f )
    {
        testEulerIsOk &= testEulerIntegrator(
                    benchmarkFunctions[ Exponential ].pointerToStateDerivativeFunction_,
                    benchmarkFunctions[ Exponential ].initialInterval_,
                    benchmarkFunctions[ Exponential ].endInterval_, 0.0001,
                    benchmarkFunctions[ Exponential ].initialState_,
                    benchmarkFunctions[ Exponential ].endState_,
                    1.0e-2 );
    }

    // Test with x_dot = x, but integrate backwards
    {
        testEulerIsOk &= testEulerIntegrator(
                    benchmarkFunctions[ BackwardsExponential ].pointerToStateDerivativeFunction_,
                    benchmarkFunctions[ BackwardsExponential ].initialInterval_,
                    benchmarkFunctions[ BackwardsExponential ].endInterval_, -0.0001,
                    benchmarkFunctions[ BackwardsExponential ].initialState_,
                    benchmarkFunctions[ BackwardsExponential ].endState_,
                    1.0e-2 );
    }

    // Test with an example from numerical recipes
    {
        testEulerIsOk &= testEulerIntegrator(
                    benchmarkFunctions[ BurdenAndFaires ].pointerToStateDerivativeFunction_,
                    benchmarkFunctions[ BurdenAndFaires ].initialInterval_,
                    benchmarkFunctions[ BurdenAndFaires ].endInterval_, 0.001,
                    benchmarkFunctions[ BurdenAndFaires ].initialState_,
                    benchmarkFunctions[ BurdenAndFaires ].endState_,
                    1.0e-2 );
    }

    testEulerIsOk &= testDifferentStateAndStateDerivativeTypes( );

    return !testEulerIsOk;
}


// End of file.

