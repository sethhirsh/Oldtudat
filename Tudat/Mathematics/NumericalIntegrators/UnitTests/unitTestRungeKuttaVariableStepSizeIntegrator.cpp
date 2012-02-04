/*! \file unitTestRungeKuttaVariableStepSizeIntegrator.cpp
 *    Source file that defines the unit test for the fixed stepsize forward RungeKuttaVariableStepSize integrator.
 *
 *    Path              : /Mathematics/NumericalIntegrators/
 *    Version           : 1
 *    Check status      : Unchecked
 *
 *    Author            : B. Tong Minh
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : b.tongminh@student.tudelft.nl
 *
 *    Date created      : 3 February, 2012
 *    Last modified     : 3 February, 2012
 *
 *    References
 *      Burden, R.L., Faires, J.D. Numerical Analysis, 7th Edition, Books/Cole, 2001.
 *
 *    Notes
 *
 *    Copyright (c) 2010-2012 Delft University of Technology.
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
 *      120203    B. Tong Minh      Copied RungeKutta4Stepsize unit test
 */

// Include statements.
#include <limits>
#include <iostream>

#include <TudatCore/Mathematics/NumericalIntegrators/UnitTests/benchmarkFunctions.h>
#include "Tudat/Mathematics/NumericalIntegrators/rungeKuttaVariableStepsizeIntegrator.h"
#include "Tudat/Mathematics/NumericalIntegrators/rungeKuttaCoefficients.h"

//! Using declaration of the RungeKuttaVariableStepSizeIntegratorXd.
using tudat::mathematics::numerical_integrators::RungeKuttaVariableStepSizeIntegratorXd;
using tudat::mathematics::numerical_integrators::RungeKuttaCoefficients;



//! Test the result of the RungeKuttaVariableStepsize integrator
/*!
 * Test the result of the RungeKuttaVariableStepsize integrator
 * \param stateDerivativeFunction Function pointer to the state derivative function
 * \param intervalStart The start of the integration interval
 * \param intervalEnd The end of the integration interval
 * \param stepSize The step size to take
 * \param initialState The initial state
 * \param expectedState Expected final state
 * \param tolerance Tolerance when comparing
 * \return true if actual final state equals the expected final state within the specified tolerance
 */
bool testRungeKuttaVariableStepsizeIntegrator(
        const RungeKuttaCoefficients& coefficients,
        const RungeKuttaVariableStepSizeIntegratorXd::StateDerivativeFunction& stateDerivativeFunction,
        const double intervalStart, const double intervalEnd, const double stepSize,
        const Eigen::VectorXd& initialState, const Eigen::VectorXd expectedState,
        const double tolerance )
{
    using std::endl;

    // Create forward RungeKuttaVariableStepsize, fixed stepsize integrator.
    {
        RungeKuttaVariableStepSizeIntegratorXd integrator( coefficients, stateDerivativeFunction,
                                                           intervalStart, initialState,
                                                           1.0e-15, tolerance / 10 );

        Eigen::VectorXd finalState = integrator.integrateTo( intervalEnd, stepSize );

        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::abs( integrator.getCurrentInterval() - intervalEnd ) / intervalEnd >
             10 * std::numeric_limits<double>::epsilon() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator end interval mismatch" << endl
                      << "Expected interval end: " << intervalEnd << endl
                      << "Actual interval end: " << integrator.getCurrentInterval() << endl
                      << "Difference: " << integrator.getCurrentInterval() - intervalEnd << endl;
            return false;
        }

        // Compute differences between computed and expected results and generate
        // cerr statement if test fails.
        if ( !expectedState.isApprox( finalState, tolerance ) )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator test failed" << endl
                      << "Expected result: " << expectedState << endl
                      << "Actual result: " << finalState << endl
                      << "Difference: " << ( expectedState - finalState ) << endl;
            return false;
        }
    }

    // Try the same again, but in two steps
    {
        RungeKuttaVariableStepSizeIntegratorXd integrator( coefficients, stateDerivativeFunction,
                                                           intervalStart, initialState,
                                                           1.0e-15, tolerance / 10 );

        const double intermediateInterval = intervalStart + ( intervalEnd - intervalStart ) / 2.0;

        const Eigen::VectorXd intermediateState = integrator.integrateTo( intermediateInterval,
                                                                          stepSize );
        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::abs( integrator.getCurrentInterval() - intermediateInterval ) /
             intermediateInterval >
             std::numeric_limits<double>::epsilon() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator intermediate interval mismatch" << endl
                      << "Expected intermediate interval: " << intermediateInterval << endl
                      << "Actual intermediate interval: " << integrator.getCurrentInterval()
                      << endl;
            return false;
        }

        // Integrate to the end
        Eigen::VectorXd finalState = integrator.integrateTo( intervalEnd, stepSize );

        // Compute differences between computed and expected interval end and generate
        // cerr statement if test fails.
        if ( std::abs( integrator.getCurrentInterval() - intervalEnd ) / intervalEnd >
             10 * std::numeric_limits<double>::epsilon() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator end interval mismatch" << endl
                      << "Expected interval end: " << intervalEnd << endl
                      << "Actual interval end: " << integrator.getCurrentInterval() << endl;
            return false;
        }

        // Compute differences between computed and expected results and generate
        // cerr statement if test fails.
        if ( !expectedState.isApprox( finalState, tolerance ) )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator test failed" << endl
                      << "Expected result: " << expectedState << endl
                      << "Actual result: " << finalState << endl;
            return false;
        }

        integrator.performIntegrationStep( stepSize );
        if ( !integrator.rollbackToPreviousState() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator rollback returned false" << endl;
            return false;
        }
        // No need to check machine precision, because this interval is stored exact
        if ( std::abs( integrator.getCurrentInterval() - intervalEnd ) / intervalEnd >
             10 * std::numeric_limits<double>::epsilon() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator rollback to invalid interval" << endl
                      << "Expected result: " << intervalEnd << endl
                      << "Actual result: " << integrator.getCurrentInterval() << endl
                      << "Difference: " << integrator.getCurrentInterval() - intervalEnd << endl;
            return false;
        }
        // This result should be exactly the same
        if ( integrator.getCurrentState() != finalState )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator rollback to invalid state" << endl
                      << "Expected result: " << finalState << endl
                      << "Actual result: " << integrator.getCurrentState() << endl;
            return false;
        }

        if ( integrator.rollbackToPreviousState() )
        {
            std::cerr << "RungeKuttaVariableStepsizeIntegrator second rollback returned true" << endl;
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
bool testDifferentStateAndStateDerivativeTypes()
{
    using tudat::unit_tests::numerical_integrators::zeroStateDerivative;
    tudat::mathematics::numerical_integrators::RungeKuttaVariableStepSizeIntegrator
            <double, Eigen::Vector3d, Eigen::VectorXd> integrator( RungeKuttaCoefficients(),
                                                                   &zeroStateDerivative,
                                                                   0.0, Eigen::Vector3d::Zero() );
    integrator.integrateTo( 0.0, 0.1 );

    // No need to test anything, this is just to check compile time errors
    return true;
}


//! Test implementation of 4th-order, fixed stepsize, Runge-Kutta integrator.
bool testRungeKuttaVariableStepsizeIntegrator( const RungeKuttaCoefficients& coefficients )
{

    // Test result initialised to false.
    bool testRungeKuttaVariableStepsizeIsOk = true;

    using namespace tudat::unit_tests::numerical_integrators;
    std::map< BenchmarkFunctions, BenchmarkFunction >& benchmarkFunctions =
             getBenchmarkFunctions();


    // Test with x_dot = 0, which results in x_f = x_0
    {
        testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                    coefficients,
                    benchmarkFunctions[Zero].pointerToStateDerivativeFunction,
                    benchmarkFunctions[Zero].initialInterval,
                    benchmarkFunctions[Zero].endInterval,
                    0.2,
                    benchmarkFunctions[Zero].initialState,
                    benchmarkFunctions[Zero].endState,
                    std::numeric_limits<double>::epsilon() );
    }

    // Test with x_dot = 1, which results in x_f = x_0 + t_f
    {
        testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                    coefficients,
                    benchmarkFunctions[Constant].pointerToStateDerivativeFunction,
                    benchmarkFunctions[Constant].initialInterval,
                    benchmarkFunctions[Constant].endInterval,
                    0.2,
                    benchmarkFunctions[Constant].initialState,
                    benchmarkFunctions[Constant].endState,
                    1.0e-14 );
    }

    // Test with x_dot = x, which results in x_f = x0 * exp( t_f )
    {
        testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                    coefficients,
                    benchmarkFunctions[Exponential].pointerToStateDerivativeFunction,
                    benchmarkFunctions[Exponential].initialInterval,
                    benchmarkFunctions[Exponential].endInterval,
                    1.0,
                    benchmarkFunctions[Exponential].initialState,
                    benchmarkFunctions[Exponential].endState,
                    1e-12 );
    }

    // Test with an example from numerical recipes
    {
        testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                    coefficients,
                    benchmarkFunctions[NumericalRecipes].pointerToStateDerivativeFunction,
                    benchmarkFunctions[NumericalRecipes].initialInterval,
                    benchmarkFunctions[NumericalRecipes].endInterval,
                    0.1,
                    benchmarkFunctions[NumericalRecipes].initialState,
                    benchmarkFunctions[NumericalRecipes].endState,
                    1e-4 );
    }

    return !testRungeKuttaVariableStepsizeIsOk;
}

int main()
{
    bool testRungeKuttaVariableStepsizeIsOk = true;

    testRungeKuttaVariableStepsizeIsOk &= testDifferentStateAndStateDerivativeTypes();
    testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg45 ) );
    testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg56 ) );
    testRungeKuttaVariableStepsizeIsOk &= testRungeKuttaVariableStepsizeIntegrator(
                RungeKuttaCoefficients::get( RungeKuttaCoefficients::rungeKuttaFehlberg78 ) );


}


// End of file.

