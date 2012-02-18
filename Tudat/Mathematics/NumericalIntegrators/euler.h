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
 *      100907    K. Kumar          File header and footer added.
 *      100929    D. Dirkx          File checked.
 *      100929    K. Kumar          Minor comment modifications.
 *      110201    K. Kumar          Updated code to make use of State class.
 *      110203    J. Melman         File checked.
 *      110207    K. Kumar          Path changed; moved integrat( ) function
 *                                  to SingleStepIntegrationMethods.
 *      110810    J. Leloux         Corrected doxygen documentation.
 *      110905    S. Billemont      Reorganized includes.
 *                                  Moved (con/de)structors and getter/setters to header.
 *      120207    B. Tong Minh      Updated to TudatCore compatibility.
 *      120213    K. Kumar          Modified getCurrentInterval() to getIndependentVariable().
 *
 *    References
 *
 */

#ifndef TUDAT_EULER_H
#define TUDAT_EULER_H

#include <TudatCore/Mathematics/NumericalIntegrators/numericalIntegrator.h>

namespace tudat
{
namespace mathematics
{
namespace numerical_integrators
{

//! Class that implements the Euler integrator.
/*!
 * Class that implements the Euler, fixed order, fixed step size integrator.
 * \tparam StateType The type of the state. This type should support addition with
 *          StateDerivativeType
 * \tparam StateDerivativeType The type of the state derivative. This type should support
 *          multiplication with IndependentVariableType and doubles.
 * \tparam IndependentVariableType The type of the independent variable.
 * \sa NumericalIntegrator.
 */
template < typename IndependentVariableType = double, typename StateType = Eigen::VectorXd,
           typename StateDerivativeType = Eigen::VectorXd >
class EulerIntegrator :
        public NumericalIntegrator< IndependentVariableType, StateType, StateDerivativeType >
{
public:

    //! Typedef of the base class.
    /*!
     * Typedef of the base class with all template parameters filled in.
     */
    typedef NumericalIntegrator< IndependentVariableType, StateType, StateDerivativeType > Base;

    //! Typedef to the state derivative function.
    /*!
     * Typedef to the state derivative function inherited from the base class.
     * \sa NumericalIntegrator::StateDerivativeFunction.
     */
    typedef typename Base::StateDerivativeFunction StateDerivativeFunction;

    //! Default constructor.
    /*!
     * Default constructor, taking a state derivative function as argument.
     * \param stateDerivativeFunction State derivative function.
     * \param intervalStart The start of the integration interval.
     * \param initialState The initial state.
     * \sa NumericalIntegrator::NumericalIntegrator.
     */
    EulerIntegrator( const StateDerivativeFunction& stateDerivativeFunction,
                     const IndependentVariableType intervalStart,
                     const StateType& initialState ) :
        Base( stateDerivativeFunction ), currentIndependentVariable_( intervalStart ),
        currentState_( initialState ), lastIndependentVariable_( intervalStart ) { }

    //! Get step size of the next step.
    /*!
     * Returns the step size of the next step.
     * \return Step size to be used for the next step.
     */
    virtual IndependentVariableType getNextStepSize( ) const { return stepSize_; }

    //! Get current state.
    /*!
     * Returns the current state of the Euler integrator.
     * \return Current integrated state.
     */
    virtual StateType getCurrentState( ) const { return currentState_; }

    //! Returns the current independent variable.
    /*!
     * Returns the current value of the independent variable of the integrator.
     * \return Current independent variable.
     */
    virtual IndependentVariableType getCurrentIndependentVariable( ) const
    {
        return currentIndependentVariable_;
    }

    //! Perform a single Euler integration step.
    /*!
     * Performs a single Euler integration step using a step of size specified by stepSize. The
     * initial state for the step is internally set to the final state of the previous step. In
     * case this is the first step, the initial state is set to the initial state provided by the
     * user.
     * \param stepSize The size of the step to take.
     * \return The state at the end of the interval.
     */
    virtual StateType performIntegrationStep( const IndependentVariableType stepSize )
    {
        lastIndependentVariable_ = currentIndependentVariable_;
        lastState_ = currentState_;

        stepSize_ = stepSize;
        currentIndependentVariable_ += stepSize_;

        currentState_ += stepSize * stateDerivativeFunction_(
                    currentIndependentVariable_, currentState_ );

        // Return the integration result.
        return currentState_;
    }

    //! Rollback the internal state to the last state.
    /*!
     * Performs rollback of the internal state to the last state. This function can only be called
     * once after calling integrateTo() or performIntegrationStep() unless specified otherwise by
     * implementations, and can not be called before any of these functions have been called. Will
     * return true if the rollback was successful, and false otherwise.
     * \return True if the rollback was successful.
     */
    virtual bool rollbackToPreviousState( )
    {
        if ( currentIndependentVariable_ == lastIndependentVariable_ )
        {
            return false;
        }

        currentIndependentVariable_ = lastIndependentVariable_;
        currentState_ = lastState_;
        return true;
    }

protected:

    //! Last used step size.
    /*!
     * Last used step size, passed to either integrateTo() or performIntegrationStep().
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
};

//! Typedef of Euler integrator (state/state derivative = VectorXd, independent variable = double).
/*!
 * Typedef of an Euler integrator with VectorXds as state and state derivative and double as
 * independent variable.
 */
typedef EulerIntegrator< > EulerIntegratorXd;

//! Typedef of a scalar Euler integrator.
/*!
 * Typedef of a Euler integrator with doubles as state and state derivative and independent variable.
 */
typedef EulerIntegrator< double, double, double > EulerIntegratord;

} // namespace integrators
} // namespace mathematics
} // namespace tudat

#endif // TUDAT_EULER_H
