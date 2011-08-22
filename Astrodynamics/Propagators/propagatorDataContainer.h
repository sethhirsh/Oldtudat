/*! \file propagatorDataContainer.h
 *    Header file that defines the class that contains all the data required by
 *    the Tudat propagators.
 *
 *    Path              : /Astrodynamics/Propagators/
 *    Version           : 8
 *    Check status      : Checked
 *
 *    Author            : K. Kumar
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : K.Kumar@tudelft.nl
 *
 *    Checker           : J.C.P. Melman
 *    Affiliation       : Delft University of Technology
 *    E-mail address    : J.C.P.Melman@tudelft.nl
 *
 *    Date created      : 20 September, 2010
 *    Last modified     : 23 February, 2011
 *
 *    References
 *
 *    Notes
 *      This data is not directly stored in the propagator so as to maximize
 *      the use of object-oriented programming methods. An object of this
 *      container encapsulates all the data relating to the propagation of one
 *      specific body. Creating multiple objects is simple and the container
 *      can be modified in future to append properties without changing any of
 *      the code in the Propagator class.
 *
 *    Copyright (c) 2010 Delft University of Technology.
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
 *      100920    K. Kumar          File created.
 *      100926    K. Kumar          Filename changed, Doxygen comments added.
 *      100927    K. Kumar          Set functions removed, bodyContainer.cpp
 *                                  merged.
 *      100929    J. Melman         Deleted destructor and constructor
 *                                  implementations.
 *                                  stateVectorRangeStart_ ->
 *                                  stateVectorStartIndex_.
 *      100929    K. Kumar          EigenRoutines.h replaced in include
 *                                  statements by linearAlgebra.h.
 *      110201    K. Kumar          Updated code to make use of State class;
 *                                  changed filename.
 *      110205    K. Kumar          Added note about use of container.
 *      110223    K. Kumar          Added missing Doxygen comment.
 */

#ifndef PROPAGATORDATACONTAINER_H
#define PROPAGATORDATACONTAINER_H

// Include statements.
#include <map>
#include <vector>
#include "celestialBody.h"
#include "forceModel.h"
#include "propagator.h"
#include "linearAlgebra.h"
#include "state.h"

// Forward declarations.
class Propagator;
class NumericalPropagator;
class CartesianStateNumericalPropagator;
class KeplerPropagator;
class SeriesPropagator;

// Using declarations.
using std::map;
using std::vector;

//! Propagator data container class.
/*!
 * Class containing properties required by Tudat propagators. It contains all
 * properties that relate to the propagation of one specific body. More than
 * one body can be propagated simultaneously by defining multiple propagator
 * data containers.
 */
class PropagatorDataContainer
{
public:

    // Definition of friendships.
    // Set Propagator class as friend.
    friend class Propagator;

    // Set NumericalPropagator class as friend.
    friend class NumericalPropagator;

    // Set CartesianStateNumericalPropagator class as friend.
    friend class CartesianStateNumericalPropagator;

    // Set KeplerPropagator class as friend.
    friend class KeplerPropagator;

    // Set SeriesPropagator class as friend.
    friend class SeriesPropagator;

    //! Default constructor.
    /*!
     * Default constructor.
     */
    PropagatorDataContainer( );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    ~PropagatorDataContainer( );

    //! Overload ostream to print class information.
    /*!
     * Overloads ostream to print class information.
     * \param stream Stream object.
     * \param propagatorDataContainer Propagator data container.
     * \return Stream object.
     */
    friend std::ostream& operator<<( std::ostream& stream,
                                     PropagatorDataContainer&
                                     propagatorDataContainer );

protected:

private:

    //! Starting index of state.
    /*!
     * Starting index of state in assembled state.
     */
    unsigned int stateStartIndex_;

    //! Size of state.
    /*!
     * Size of state in assembled state.
     */
    unsigned int sizeOfState_;

    //! A map of propagation history.
    /*!
     * A map of propagation history with propagation time taken as key.
     */
    map< double, State > propagationHistory_;

    //! Propagation history iterator.
    /*!
     * Iterator for map of propagation history.
     */
    map< double, State >::iterator iteratorPropagationHistory_;

    //! Vector container of pointers to force models.
    /*!
     * Vector container of pointers to force models.
     */
    vector< ForceModel* > vectorContainerOfPointersToForceModels_;

    //! Final state.
    /*!
     * Final state.
     */
    State finalState_;

    //! Pointer to initial state.
    /*!
     * Initial state given as a pointer to a State object.
     */
    State* pointerToInitialState_;

    //! Pointer to Body class.
    /*!
     * Pointer to Body class.
     */
    Body* pointerToBody_;

    //! Pointer to Propagator class.
    /*!
     * Pointer to Propagator class.
     */
    Propagator* pointerToPropagator_;

    //! Pointer to central body.
    /*!
     * Pointer to central body.
     */
    CelestialBody* pointerToCentralBody_;
};

#endif // PROPAGATORDATACONTAINER_H

// End of file.
