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
 *      101026    K. Kumar          First creation of code.
 *      101110    K. Kumar          Added setState( ) function for matrices.
 *      101202    J. Melman         Changed path and blank line removed.
 *      110110    K. Kumar          Changed matrices to vectors.
 *      110202    K. Kumar          Removed set/get functions for state and made state a public
 *                                  variable for functionality.
 *      110204    K. Kumar          Note added about public state variable.
 *      110207    K. Kumar          Added ostream overload.
 *      111123    B. Tong Minh      Added custom constructors.
 *      120509    K. Kumar          Modified length-initializer to initialize element values to
 *                                  zero.
 *
 *    References
 *
 *    The variable state is public as opposed to being protected and accessed by set and get
 *    functions to simplify the use of Eigen functions for vectors in the rest of the code,
 *    e.g. setZero( ), segment( ).
 *
 */

#ifndef TUDAT_STATE_H
#define TUDAT_STATE_H

#include <Eigen/Core>

#include <iostream>

namespace tudat
{
namespace astrodynamics
{
namespace states
{

//! State class.
/*!
 * Definition of State class. This is a base class for all state classes
 * defined within Tudat.
 */
class State
{
public:

    //! Default constructor.
    /*!
     * Default constructor, leaves the internal state vector uninitialized.
     */
    State( ) { }

    //! Constructor with state vector.
    /*!
     * Custom constructor ,which sets the internal state vector to the specified vector.
     * \param stateToSet State vector to set.
     */
    State( const Eigen::VectorXd& stateToSet ) : state( stateToSet ) { }

    //! Constructor with initial length.
    /*!
     * Custom constructor, which sets the internal state vector to the specified length, with all
     * elements set to zero.
     * \param stateLength Length of the state vector.
     */
    State( int stateLength ) : state( Eigen::VectorXd::Zero( stateLength ) ) { }

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~State( ) { }

    //! State.
    /*!
     * State.
     */
    Eigen::VectorXd state;

    //! Overload ostream to print class information.
    /*!
     * Overloads ostream to print class information.
     * \param stream Stream object.
     * \param stateObject State object.
     * \return Stream object.
     */
    friend std::ostream& operator<<( std::ostream& stream, State& stateObject )
    {
        stream << "The state is set to: " << stateObject.state << std::endl;
        return stream;
    }

protected:

private:
};

} // namespace states
} // namespace astrodynamics
} // namespace tudat

#endif // TUDAT_STATE_H
