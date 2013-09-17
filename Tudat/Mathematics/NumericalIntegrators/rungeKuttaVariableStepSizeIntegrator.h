/*    Copyright (c) 2010-2013, Delft University of Technology
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
 *      110826    K. Kumar          File created.
 *      110909    E.A.G. Heeren     Modified to be included in Tudat. Added if-statement to ensure
 *                                  a maximum stepsize change.
 *      110912    K. Kumar          Minor changes.
 *      111021    F.M. Engelen      Added interface for RKF45 and RKF56.
 *      111110    E.A.G Heeren      Minor changes.
 *      120204    B. Tong Minh      Adapted for new core integrators interface.
 *      120213    K. Kumar          Modified getCurrentInterval() to getIndependentVariable().
 *      120324    K. Kumar          Removed superfluous functions; added runtime error if minimum
 *                                  step size is exceeded; generalized algorithm to work with any
 *                                  Runge-Kutta-type coefficients; added options to customize
 *                                  new step size computation and error control (added default
 *                                  function). Removed erroneous reference (Eagle, 2011).
 *      120331    B. Tong Minh      Added custom runtime_error class for minimum step size exceeded
 *                                  error.
 *      120404    K. Kumar          Moved custom runtime_error class implementation outside of main
 *                                  integrator class.
 *      120614    A. Ronse          Fixed bug in constructor.
 *      130121    K. Kumar          Added shared-ptr typedef.
 *      130307    D. Dirkx          Added function to retrieve integration stage evaluations.
 *
 *    References
 *      Burden, R.L., Faires, J.D. Numerical Analysis, 7th Edition, Books/Cole, 2001.
 *      Montenbruck, O., Gill, E. Satellite Orbits: Models, Methods, Applications, Springer, 2005.
 *
 *    Notes
 *
 */

#ifndef TUDAT_RUNGE_KUTTA_VARIABLE_STEP_SIZE_INTEGRATOR_H
#define TUDAT_RUNGE_KUTTA_VARIABLE_STEP_SIZE_INTEGRATOR_H

#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

#include <boost/bind.hpp>
#include <boost/exception/all.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <Eigen/Core>

#include <TudatCore/Basics/utilityMacros.h>
#include <TudatCore/Mathematics/NumericalIntegrators/reinitializableNumericalIntegrator.h>

#include "Tudat/Mathematics/NumericalIntegrators/rungeKuttaCoefficients.h"

namespace tudat
{
namespace numerical_integrators
{

//! Class that implements the Runge-Kutta variable stepsize integrator.
/*!
 * Class that implements the Runge-Kutta variable step size integrator.
 * \tparam StateType The type of the state. This type should be an Eigen::Matrix derived type.
 * \tparam StateDerivativeType The type of the state derivative. This type should be an
 *          Eigen::Matrix derived type.
 * \tparam IndependentVariableType The type of the independent variable. This type should be
 *          either a float or double.
 * \sa NumericalIntegrator.
 */
template < typename IndependentVariableType = double, typename StateType = Eigen::VectorXd,
           typename StateDerivativeType = StateType >
class RungeKuttaVariableStepSizeIntegrator :
        public ReinitializableNumericalIntegrator<
        IndependentVariableType, StateType, StateDerivativeType >
{
public:

    //! Typedef to the function used to compute the new step size.
    /*!
     * Typedef to the function used to compute the new step size. This should be a pointer to a
     * function or a boost function.
     */
    typedef boost::function< std::pair< IndependentVariableType, bool >(
            const IndependentVariableType, const IndependentVariableType,
            const IndependentVariableType, const IndependentVariableType,
            const StateType&, const StateType&,
            const StateType&, const StateType& ) > NewStepSizeFunction;

    //! Typedef of the base class.
    /*!
     * Typedef of the base class with all template parameters filled in.
     */
    typedef tudat::numerical_integrators::ReinitializableNumericalIntegrator<
    IndependentVariableType, StateType,
    StateDerivativeType > ReinitializableNumericalIntegratorBase;

    //! Typedef to the state derivative function.
    /*!
     * Typedef to the state derivative function inherited from the base class.
     * \sa NumericalIntegrator::StateDerivativeFunction.
     */
    typedef typename ReinitializableNumericalIntegratorBase::NumericalIntegratorBase::
    StateDerivativeFunction StateDerivativeFunction;

    //! Exception that is thrown if the minimum step size is exceeded.
    /*!
     * Exception thrown by RungeKuttaVariableStepSizeIntegrator<>::
     * computeNextStepSizeAndValidateResult() if the minimum step size is exceeded.
     */
    class MinimumStepSizeExceededError;

    //! Default constructor.
    /*!
     * Default constructor, taking coefficients, a state derivative function, initial conditions,
     * minimum & maximum step size and relative & absolute error tolerance per item in the state
     * vector as argument.
     * \param coefficients Coefficients to use with this integrator.
     * \param stateDerivativeFunction State derivative function.
     * \param intervalStart The start of the integration interval.
     * \param initialState The initial state.
     * \param minimumStepSize The minimum step size to take. If this constraint is violated, an
     *          exception will be thrown and the next state is computed using the minimum value.
     * \param maximumStepSize The maximum step size to take. If this constraint is violated, an
     *          exception will be thrown and the next state is computed using the maximum value.
     * \param relativeErrorTolerance The relative error tolerance, for each individual state
     *          vector element.
     * \param absoluteErrorTolerance The absolute error tolerance, for each individual state
     *          vector element.
     * \sa NumericalIntegrator::NumericalIntegrator.
     */
    RungeKuttaVariableStepSizeIntegrator(
            const RungeKuttaCoefficients& coefficients,
            const StateDerivativeFunction& stateDerivativeFunction,
            const IndependentVariableType intervalStart,
            const StateType& initialState,
            const IndependentVariableType minimumStepSize,
            const IndependentVariableType maximumStepSize,
            const StateType& relativeErrorTolerance,
            const StateType& absoluteErrorTolerance,
            const IndependentVariableType safetyFactorForNextStepSize = 0.8,
            const IndependentVariableType maximumFactorIncreaseForNextStepSize = 4.0,
            const IndependentVariableType minimumFactorDecreaseForNextStepSize = 0.1,
            const NewStepSizeFunction& newStepSizeFunction = 0 ) :
        ReinitializableNumericalIntegratorBase( stateDerivativeFunction ),
        currentIndependentVariable_( intervalStart ),
        currentState_( initialState ),
        lastIndependentVariable_( intervalStart ),
        coefficients_( coefficients ),
        minimumStepSize_( std::fabs( minimumStepSize ) ),
        maximumStepSize_( std::fabs( maximumStepSize ) ),
        relativeErrorTolerance_( relativeErrorTolerance.array().abs( ) ),
        absoluteErrorTolerance_( absoluteErrorTolerance.array().abs( ) ),
        safetyFactorForNextStepSize_( std::fabs( safetyFactorForNextStepSize ) ),
        maximumFactorIncreaseForNextStepSize_( std::fabs( maximumFactorIncreaseForNextStepSize ) ),
        minimumFactorDecreaseForNextStepSize_( std::fabs( minimumFactorDecreaseForNextStepSize ) ),
        newStepSizeFunction_( newStepSizeFunction )
    {
        // Set default newStepSizeFunction_ to the class method.
        if ( this->newStepSizeFunction_ == 0 )
        {
            this->newStepSizeFunction_ = boost::bind(
                        &RungeKuttaVariableStepSizeIntegrator::computeNewStepSize,
                        this, _1, _2, _3, _4, _5, _6, _7, _8 );
        }
    }

    //! Default constructor.
    /*!
     * Default constructor, taking coefficients a state derivative function, initial conditions,
     * minimum & maximum step size and relative & absolute error tolerance for all items in the
     * state vector as argument.
     * \param coefficients Coefficients to use with this integrator.
     * \param stateDerivativeFunction State derivative function.
     * \param intervalStart The start of the integration interval.
     * \param initialState The initial state.
     * \param minimumStepSize The minimum step size to take. If this constraint is violated, an
     *          exception will be thrown and the next state is computed using the minimum value.
     * \param maximumStepSize The maximum step size to take. If this constraint is violated, an
     *          exception will be thrown and the next state is computed using the maximum value.
     * \param relativeErrorTolerance The relative error tolerance, equal for all individual state
     *          vector elements.
     * \param absoluteErrorTolerance The absolute error tolerance, equal for all individual state
     *          vector elements.
     * \sa NumericalIntegrator::NumericalIntegrator.
     */
    RungeKuttaVariableStepSizeIntegrator(
            const RungeKuttaCoefficients& coefficients,
            const StateDerivativeFunction& stateDerivativeFunction,
            const IndependentVariableType intervalStart,
            const StateType& initialState,
            const IndependentVariableType minimumStepSize,
            const IndependentVariableType maximumStepSize,
            const typename StateType::Scalar relativeErrorTolerance,
            const typename StateType::Scalar absoluteErrorTolerance,
            const IndependentVariableType safetyFactorForNextStepSize = 0.8,
            const IndependentVariableType maximumFactorIncreaseForNextStepSize = 4.0,
            const IndependentVariableType minimumFactorDecreaseForNextStepSize = 0.1,
            const NewStepSizeFunction& newStepSizeFunction = 0 ) :
        ReinitializableNumericalIntegratorBase( stateDerivativeFunction ),
        currentIndependentVariable_( intervalStart ),
        currentState_( initialState ),
        lastIndependentVariable_( intervalStart ),
        coefficients_( coefficients ),
        minimumStepSize_( std::fabs( minimumStepSize ) ),
        maximumStepSize_( std::fabs( maximumStepSize ) ),
        relativeErrorTolerance_( StateType::Constant( initialState.rows( ), initialState.cols( ),
                                                      std::fabs( relativeErrorTolerance ) ) ),
        absoluteErrorTolerance_( StateType::Constant( initialState.rows( ), initialState.cols( ),
                                                      std::fabs( absoluteErrorTolerance ) ) ),
        safetyFactorForNextStepSize_( std::fabs( safetyFactorForNextStepSize ) ),
        maximumFactorIncreaseForNextStepSize_( std::fabs( maximumFactorIncreaseForNextStepSize ) ),
        minimumFactorDecreaseForNextStepSize_( std::fabs( minimumFactorDecreaseForNextStepSize ) ),
        newStepSizeFunction_( newStepSizeFunction )
    {
        // Set default newStepSizeFunction_ to the class method.
        if ( newStepSizeFunction_ == 0 )
        {
            this->newStepSizeFunction_ = boost::bind(
                        &RungeKuttaVariableStepSizeIntegrator::computeNewStepSize,
                        this, _1, _2, _3, _4, _5, _6, _7, _8 );
        }
    }

    //! Get step size of the next step.
    /*!
     * Returns the step size of the next step.
     * \return Step size to be used for the next step.
     */
    virtual IndependentVariableType getNextStepSize( ) const { return this->stepSize_; }

    //! Get current state.
    /*!
     * Returns the current state of the integrator.
     * \return Current integrated state.
     */
    virtual StateType getCurrentState( ) const { return this->currentState_; }

    //! Get current independent variable.
    /*!
     * Returns the current value of the independent variable of the integrator.
     * \return Current independent variable.
     */
    virtual IndependentVariableType getCurrentIndependentVariable( ) const
    {
        return this->currentIndependentVariable_;
    }

    //! Get current state derivatives.
    /*!
     * Returns the current state derivatives, i.e., the values of k_{i} (stage evaluations) in
     * Runge-Kutta scheme.
     * \return Current state derivatives evaluated according to stages of Runge-Kutta scheme.
     */
    std::vector< StateDerivativeType > getCurrentStateDerivatives( ) 
    { 
        return currentStateDerivatives_;
    }

    //! Perform a single integration step.
    /*!
     * Perform a single integration step and compute a new step size.
     * \param stepSize The step size to take. If the time step is too large to satisfy the error
     *          constraints, the step is redone until the error constraint is satisfied.
     * \return The state at the end of the interval.
     */
    virtual StateType performIntegrationStep( const IndependentVariableType stepSize );

    //! Rollback internal state to the last state.
    /*!
     * Performs rollback of the internal state to the last state. This function can only be called
     * once after calling integrateTo( ) or performIntegrationStep( ) unless specified otherwise by
     * implementations, and can not be called before any of these functions have been called. Will
     * return true if the rollback was successful, and false otherwise.
     * \return True if the rollback was successful.
     */
    virtual bool rollbackToPreviousState( )
    {
        if ( this->currentIndependentVariable_ == this->lastIndependentVariable_ )
        {
            return false;
        }

        this->currentIndependentVariable_ = this->lastIndependentVariable_;
        this->currentState_ = this->lastState_;
        return true;
    }

    //! Modify the state at the current interval.
    /*!
     * Modify the state at the current interval. This allows for discrete jumps in the state, often
     * used in simulations of discrete events. In astrodynamics, this relates to simulations of
     * rocket staging, impulsive shots, parachuting, attitude normalization, ideal control, etc.
     * The modified state cannot be rolled back; to do this, just simply store the state before
     * calling this function the first time, and call it again with the initial state as parameter
     * to revert to the state before the discrete change.
     * \param newState The state to set the current state to.
     */
    void modifyCurrentState( const StateType& newState )
    {
        this->currentState_ = newState;
        this->lastIndependentVariable_ = currentIndependentVariable_;
    }

protected:

    //! Computes the next step size and validates the result.
    /*!
     * Computes the next step size based on a higher and lower order estimate, determines if the
     * error is within bounds and returns a new step size.
     * \param lowerOrderEstimate The integrated result with the lower order coefficients.
     * \param higherOrderEstimate The integrated result with the higher order coefficients.
     * \param stepSize The step size used to obtain these results.
     * \return True if teh error was within bounds, false otherwise.
     */
    virtual bool computeNextStepSizeAndValidateResult( const StateType& lowerOrderEstimate,
                                                       const StateType& higherOrderEstimate,
                                                       const IndependentVariableType stepSize );

    //! Compute new step size.
    /*!
     * Computes the new step size based on a generic definition of the local truncation error.
     */
    virtual std::pair< IndependentVariableType, bool > computeNewStepSize(
            const IndependentVariableType stepSize, const IndependentVariableType lowerOrder,
            const IndependentVariableType higherOrder,
            const IndependentVariableType safetyFactorForNextStepSize,
            const StateType& relativeErrorTolerance, const StateType& absoluteErrorTolerance,
            const StateType& lowerOrderEstimate, const StateType& higherOrderEstimate );

    //! Last used step size.
    /*!
     * Last used step size, passed to either integrateTo( ) or performIntegrationStep( ).
     */
    IndependentVariableType stepSize_;

    //! Current independent variable.
    /*!
     * Current independent variable as computed by performIntegrationStep().
     */
    IndependentVariableType currentIndependentVariable_;

    //! Current state.
    /*!
     * Current state as computed by performIntegrationStep( ).
     */
    StateType currentState_;

    //! Last independent variable.
    /*!
     * Last independent variable value as computed by performIntegrationStep().
     */
    IndependentVariableType lastIndependentVariable_;

    //! Last state.
    /*!
     * Last state as computed by performIntegrationStep( ).
     */
    StateType lastState_;

    //! Coefficients for the integrator.
    /*!
     * Coefficients for the integrator, as defined by the Butcher tableau.
     */
    RungeKuttaCoefficients coefficients_;

    //! Minimum step size.
    /*!
     * Minimum step size.
     */
    IndependentVariableType minimumStepSize_;

    //! Maximum step size.
    /*!
     * Maximum step size.
     */
    IndependentVariableType maximumStepSize_;

    //! Relative error tolerance.
    /*!
     * Relative error tolerance per element in the state.
     */
    StateType relativeErrorTolerance_;

    //! Absolute error tolerance.
    /*!
     * Absolute error tolerance per element in the state.
     */
    StateType absoluteErrorTolerance_;

    //! Safety factor for next step size.
    /*!
     * Safety factor used to scale prediction of next step size. This is usually picked between
     * 0.8 and 0.9 (Burden and Faires, 2001).
     */
    IndependentVariableType safetyFactorForNextStepSize_;

    //! Maximum factor increase for next step size.
    /*!
     * The maximum factor by which the next step size can increase compared to the current value.
     * The need for this maximum stems from a need to ensure that the step size changes do not
     * alias with the dynamics of the model being integrated. This is typically set at 4.0, based
     * on numerical experiments (Burden and Faires, 2001).
     */
    IndependentVariableType maximumFactorIncreaseForNextStepSize_;

    //! Minimum factor decrease for next step size.
    /*!
     * The minimum factor by which the next step size can decrease compared to the current value.
     * The need for this minimum stems from a need to ensure that the step size changes do not
     * alias with the dynamics of the model being integrated. This is typically set at 0.1, based
     * on numerical experiments (Burden and Faires, 2001).
     */
    IndependentVariableType minimumFactorDecreaseForNextStepSize_;

    //! Function that returns the new step size computed.
    /*!
     * Function that returns the new step size computed, as passed to the constructor.
     */
    NewStepSizeFunction newStepSizeFunction_;

    //! Vector of state derivatives.
    /*!
     * Vector of state derivatives, i.e. values of k_{i} in Runge-Kutta scheme.
     */
    std::vector< StateDerivativeType > currentStateDerivatives_;
};

//! Perform a single integration step.
template < typename IndependentVariableType, typename StateType, typename StateDerivativeType >
StateType
RungeKuttaVariableStepSizeIntegrator< IndependentVariableType, StateType, StateDerivativeType >
::performIntegrationStep( const IndependentVariableType stepSize )
{
    // Define and allocated vector for the number of stages.
    currentStateDerivatives_.clear( );
    currentStateDerivatives_.reserve( this->coefficients_.cCoefficients.rows( ) );

    // Define lower and higher order estimates.
    StateType lowerOrderEstimate( this->currentState_ ),
              higherOrderEstimate( this->currentState_ );

    // Compute the k_i state derivatives per stage.
    for ( int stage = 0; stage < this->coefficients_.cCoefficients.rows( ); stage++ )
    {
        // Compute the intermediate state to pass to the state derivative for this stage.
        StateType intermediateState( this->currentState_ );

        // Compute the intermediate state.
        for ( int column = 0; column < stage; column++ )
        {
            intermediateState += stepSize * this->coefficients_.aCoefficients( stage, column )
                    * currentStateDerivatives_[ column ];
        }

        // Compute the state derivative.
        currentStateDerivatives_.push_back(
                    this->stateDerivativeFunction_(
                        this->currentIndependentVariable_ +
                        this->coefficients_.cCoefficients( stage ) * stepSize,
                        intermediateState ) );

        // Update the estimate.
        lowerOrderEstimate += this->coefficients_.bCoefficients( 0, stage ) * stepSize *
                currentStateDerivatives_[ stage ];
        higherOrderEstimate += this->coefficients_.bCoefficients( 1, stage ) * stepSize *
                currentStateDerivatives_[ stage ];
    }

    // Determine if the error was within bounds and compute a new step size.
    if ( computeNextStepSizeAndValidateResult( lowerOrderEstimate,
                                               higherOrderEstimate, stepSize ) )
    {
        // Accept the current step.
        this->lastIndependentVariable_ = this->currentIndependentVariable_;
        this->lastState_ = this->currentState_;
        this->currentIndependentVariable_ += stepSize;

        switch ( this->coefficients_.orderEstimateToIntegrate )
        {
        case RungeKuttaCoefficients::lower:
            this->currentState_ = lowerOrderEstimate;
            return this->currentState_;

        case RungeKuttaCoefficients::higher:
            this->currentState_ = higherOrderEstimate;
            return currentState_;

        default: // The default case will never occur because OrderEstimateToIntegrate is an enum.
            boost::throw_exception(
                        boost::enable_error_info(
                            std::runtime_error( "Order estimate to integrate is invalid." ) ) );
        }
    }
    else
    {
        // Reject current step.
        return performIntegrationStep( this->stepSize_ );
    }
}

//! Compute the next step size and validate the result.
template< typename IndependentVariableType, typename StateType, typename StateDerivativeType >
bool
RungeKuttaVariableStepSizeIntegrator< IndependentVariableType, StateType, StateDerivativeType >
::computeNextStepSizeAndValidateResult(
        const StateType& lowerOrderEstimate, const StateType& higherOrderEstimate,
        const IndependentVariableType stepSize )
{
    // Compute new step size using new step size function, which also returns whether the
    // relative error is within bounds or not.
    std::pair< IndependentVariableType, bool > newStepSizePair = this->newStepSizeFunction_(
                stepSize, this->coefficients_.lowerOrder, this->coefficients_.higherOrder,
                this->safetyFactorForNextStepSize_, this->relativeErrorTolerance_,
                this->absoluteErrorTolerance_, lowerOrderEstimate,
                higherOrderEstimate );

    // Check whether change in stepsize does not exceed bounds.
    // If the stepsize is reduced to less than the prescibed minimum factor, set to minimum factor.
    // If the stepsize is increased to more than the prescribed maximum factor, set to maximum
    // factor. These bounds are necessary to prevent the stepsize changes from aliasing
    // with the dynamics of the system of ODEs.
    // Also check if maximum step size is exceeded and step next step size to maximum if necessary.
    // Typically used bounds can be found in (Burden and Faires, 2001).
    if ( newStepSizePair.first / stepSize <= this->minimumFactorDecreaseForNextStepSize_ )
    {
        this->stepSize_ = stepSize * this->minimumFactorDecreaseForNextStepSize_;
    }

    else if ( newStepSizePair.first / stepSize >= maximumFactorIncreaseForNextStepSize_ )
    {
        this->stepSize_ = stepSize * this->maximumFactorIncreaseForNextStepSize_;
    }

    else
    {
       this->stepSize_ = newStepSizePair.first;
    }

    // Check if minimum step size is violated and throw exception if necessary.
    if ( std::fabs( this->stepSize_ ) < this->minimumStepSize_ )
    {
        boost::throw_exception(
                    boost::enable_error_info(
                        MinimumStepSizeExceededError( this->minimumStepSize_,
                                                      std::fabs( this->stepSize_ ) ) ) );
    }

    else if ( std::fabs( this->stepSize_ ) > this->maximumStepSize_ )
    {
        this->stepSize_ = this->maximumStepSize_;
    }

    // Check if computed error in state is too large and reject step if true.
    return newStepSizePair.second;
}

//! Compute new step size.
/*!
 * Computes the new step size based on a generic definition of the local truncation error.
 */
template< typename IndependentVariableType, typename StateType, typename StateDerivativeType >
std::pair< IndependentVariableType, bool >
RungeKuttaVariableStepSizeIntegrator< IndependentVariableType, StateType, StateDerivativeType >
::computeNewStepSize(
        const IndependentVariableType stepSize,
        const IndependentVariableType lowerOrder,
        const IndependentVariableType higherOrder,
        const IndependentVariableType safetyFactorForNextStepSize,
        const StateType& relativeErrorTolerance,
        const StateType& absoluteErrorTolerance,
        const StateType& lowerOrderEstimate,
        const StateType& higherOrderEstimate )
{
    TUDAT_UNUSED_PARAMETER( lowerOrder);

    // Compute the truncation error based on the higher and lower order estimates.
    const StateType truncationError_ =
            ( higherOrderEstimate - lowerOrderEstimate ).array( ).abs( );

    // Compute error tolerance based on relative and absolute error tolerances.
    const StateType errorTolerance_ =
            ( higherOrderEstimate.array( ).abs( ) *
              relativeErrorTolerance.array( ) ).matrix( )
            + absoluteErrorTolerance;

    // Compute relative truncation error. This will indicate if the current step satisfies the
    // required tolerances.
    const StateType relativeTruncationError_ = truncationError_.array( ) /
            errorTolerance_.array( );

    // Compute the maximum error based on the largest coefficient in the relative truncation error
    // matrix.
    const typename StateType::Scalar maximumErrorInState_
            = relativeTruncationError_.array( ).abs( ).maxCoeff( );

    // Compute the new step size. This is based off of the equation given in
    // (Montenbruck and Gill, 2005).
    const IndependentVariableType newStepSize = safetyFactorForNextStepSize * stepSize
            * std::pow( 1.0 / maximumErrorInState_, 1.0 / higherOrder );

    // Check if the current state can be accepted.
    const bool isIntegrationStepAccepted = maximumErrorInState_ <= 1.0;

    // Return the computed new step size and whether the current step taken is acceptable. If it
    // isn't, the step will be recomputed with the computed new step size.
    return std::make_pair( newStepSize, isIntegrationStepAccepted );
}

//! Exception that is thrown if the minimum step size is exceeded.
/*!
 * Exception thrown by RungeKuttaVariableStepSizeIntegrator<>::
 * computeNextStepSizeAndValidateResult() if the minimum step size is exceeded.
 */
template < typename IndependentVariableType, typename StateType, typename StateDerivativeType >
class RungeKuttaVariableStepSizeIntegrator< IndependentVariableType, StateType,
        StateDerivativeType >
        ::MinimumStepSizeExceededError : public std::runtime_error
{
public:

    //! Default constructor.
    /*!
     * Default constructor, initializes the parent runtime_error.
     * \param minimumStepSize_ The minimum step size allowed by the integrator.
     * \param requestedStepSize_ The new calculated step size.
     */
    MinimumStepSizeExceededError( IndependentVariableType minimumStepSize_,
                                  IndependentVariableType requestedStepSize_ ) :
        std::runtime_error( "Minimum step size exceeded." ),
        minimumStepSize( minimumStepSize_ ), requestedStepSize( requestedStepSize_ )
    { }

    //! The minimum step size allowed by the integrator.
    /*!
     * The minimum step size allowed by the integrator.
     */
    IndependentVariableType minimumStepSize;

    //! The new calculated step size.
    /*!
     * The new calculated step size.
     */
    IndependentVariableType requestedStepSize;

protected:
private:
};

//! Typedef of variable-step size Runge-Kutta integrator (state/state derivative = VectorXd,
//! independent variable = double).
/*!
 * Typedef of a variable-step size Runge-Kutta integrator with VectorXds as state and state
 * derivative and double as independent variable.
 */
typedef RungeKuttaVariableStepSizeIntegrator< > RungeKuttaVariableStepSizeIntegratorXd;

//! Typedef for shared-pointer to RungeKuttaVariableStepSizeIntegratorXd object.
typedef boost::shared_ptr< RungeKuttaVariableStepSizeIntegratorXd >
RungeKuttaVariableStepSizeIntegratorXdPointer;

} // namespace numerical_integrators
} // namespace tudat

#endif // TUDAT_RUNGE_KUTTA_VARIABLE_STEP_SIZE_INTEGRATOR_H
