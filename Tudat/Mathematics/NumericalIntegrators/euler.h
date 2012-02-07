/*! \file euler.h
 *    Header file that defines the Euler integrator implemented in Tudat.
 *
 *    Path              : /Mathematics/NumericalIntegrators/
 *    Version           : 9
 *    Check status      : Unchecked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Author            : B. Tong Minh
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : B.TongMinh@student.tudelft.nl
 *
 *    Checker           : D. Dirkx
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : d.dirkx@tudelft.nl
 *
 *    Checker           : J. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 7 February, 2012
 *    Last modified     : 5 September, 2011
 *
 *    References
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
 *      120207    B. Tong Minh      Updated to TudatCore compatibility
 */

#ifndef TUDAT_EULER_H
#define TUDAT_EULER_H

#include <TudatCore/Mathematics/NumericalIntegrators/numericalIntegrator.h>

//! Tudat library namespace.
namespace tudat
{

//! Tudat mathematics namespace
namespace mathematics
{

//! Integrators namespace
namespace numerical_integrators
{

//! Class that implements the Euler integrator
/*!
 * Class that implements the Euler, fixed order, fixed step size integrator
 * \tparam StateType The type of the state. This type should support addition with
 * StateDerivativeType
 * \tparam StateDerivativeType The type of the state derivative. This type should support
 * multiplication with IndependentVariableType and doubles.
 * \tparam IndependentVariableType The type of the independent variable
 * \sa NumericalIntegrator
 */
template < typename IndependentVariableType = double, typename StateType = Eigen::VectorXd,
           typename StateDerivativeType = Eigen::VectorXd >
class EulerIntegrator :
        public NumericalIntegrator<IndependentVariableType, StateType, StateDerivativeType>
{
public:
    //! Typedef of the base class
    /*!
     * Typedef of the base class with all template parameters filled in
     */
    typedef NumericalIntegrator<IndependentVariableType, StateType, StateDerivativeType> Base;

    //! Typedef to the state derivative function
    /*!
     * Typedef to the state derivative function inherited from the base class.
     * \sa NumericalIntegrator::StateDerivativeFunction
     */
    typedef typename Base::StateDerivativeFunction StateDerivativeFunction;

    //! Default constructor
    /*!
     * Default constructor, taking a state derivative function as argument.
     * \param stateDerivativeFunction State derivative function.
     * \param intervalStart The start of the integration interval
     * \param initialState The initial state
     * \sa NumericalIntegrator::NumericalIntegrator
     */
    EulerIntegrator( const StateDerivativeFunction& stateDerivativeFunction,
                           const IndependentVariableType intervalStart,
                           const StateType& initialState ) :
        Base( stateDerivativeFunction ),
        currentInterval_( intervalStart ),
        currentState_( initialState ),
        lastInterval_( intervalStart )
    { }

    //! Returns the step size of the next step
    /*!
     * Returns the step size of the next step.
     * \return Step size to be used for the next step
     */
    virtual IndependentVariableType getNextStepSize( ) const { return stepSize_; }

    //! Returns the current state
    /*!
     * Returns the current state of the integrator. Child classes should override this and provide
     * the computed state by performIntegrationStep( ).
     * \return Current integrated state
     */
    virtual StateType getCurrentState( ) const { return currentState_; }

    //! Returns the current interval
    /*!
     * Returns the current interval of the integrator. Child classes should override this and
     * provide the computed interval by performIntegrationStep( ).
     * \return Current interval
     */
    virtual IndependentVariableType getCurrentInterval( ) const { return currentInterval_; }

    //! Perform a single integration step
    /*!
     * Perform a single integration step from intervalStart with initialState and stepSize and
     * sets the stepSize member variable.
     * \param intervalStart The start of the interval of this step
     * \param initialState The initial state
     * \param initialStepSize The initial step size
     * \return The state at the end of the interval
     */
    virtual StateType performIntegrationStep( const IndependentVariableType stepSize )
    {
        lastInterval_ = currentInterval_;
        lastState_ = currentState_;

        stepSize_ = stepSize;
        currentInterval_ += this->stepSize_;
        currentState_ += stepSize * stateDerivativeFunction_( currentInterval_, currentState_ );

        // Return the integration result
        return currentState_;
    }

    //! Rollback the internal state to the last state;
    /*!
     * Rollback the internal state to the last state. This function can only be called once after
     * calling integrateTo( ) or performIntegrationStep( ) unless specified otherwise by
     * implementations, and can not be called before any of these functions have been called. Will
     * return true if the rollback was succesful, and false otherwise.
     * \return True if the rollback was succesful.
     */
    virtual bool rollbackToPreviousState( )
    {
        if ( currentInterval_ == lastInterval_ )
        {
            return false;
        }

        currentInterval_ = lastInterval_;
        currentState_ = lastState_;
        return true;
    }


protected:
    //! Last used step size
    /*!
     * Last used step size, passed to either integrateTo( ) or performIntegrationStep( )
     */
    IndependentVariableType stepSize_;

    //! Current interval
    /*!
     * Current interval as computed by performIntegrationStep( )
     */
    IndependentVariableType currentInterval_;
    //! Current state
    /*!
     * Current state as computed by performIntegrationStep( )
     */
    StateType currentState_;

    //! Last interval
    /*!
     * Last interval as computed by performIntegrationStep( )
     */
    IndependentVariableType lastInterval_;

    //! Last state
    /*!
     * Last state as computed by performIntegrationStep( )
     */
    StateType lastState_;




}; // class EulerIntegrator

//! Typedef of an Euler integrator with VectorXds as state and state derivative and double as independent variable
/*!
 * Typedef of an Euler integrator with VectorXds as state and state derivative and double as
 * independent variable.
 */
typedef EulerIntegrator<> EulerIntegratorXd;


//! Typedef of a scalar Euler integrator.
/*!
 * Typedef of an Euler integrator with doubles as state and state derivative and independent variable.
 */
typedef EulerIntegrator<double, double, double> EulerIntegratord;

} // namespace integrators

} // namespace mathematics

} // namespace tudat

#endif // TUDAT_EULER_H

// End of file.
