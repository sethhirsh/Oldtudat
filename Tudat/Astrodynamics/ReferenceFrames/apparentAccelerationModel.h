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
 *      120716    A. Ronse          First creation of code.
 *      120718    D. Dirkx          Splitted up functions into Coriolis, centripetal and Euler.
 *
 *    References
 *      J.S. Torok, Analytical Mechanics, Wiley-Interscience, 2000.
 *
 *    Notes
 *
 */

#ifndef TUDAT_APPARENT_ACCELERATION_MODEL_H
#define TUDAT_APPARENT_ACCELERATION_MODEL_H

#include <boost/function.hpp>

#include <Eigen/Dense>

#include "Tudat/Astrodynamics/BasicAstrodynamics/accelerationModel.h"

namespace tudat
{
namespace acceleration_models
{

//! Compute apparent acceleration due to non-inertiality of reference frame.
/*!
 * Computes apparent acceleration experienced by an object, of which the position and velocity
 * are provided in a non-inertial (i.e. rotating and/or accelerating) reference frame.
 * The apparent acceleration of a particle due to the non-inertiality of the reference frame in
 * which its state is given, is given by following equation:
 * \f[
 *      \boldsymbol{a}_{apparent} =
 *      -\boldsymbol{a}_B - \dot{\boldsymbol{\omega}} \times \boldsymbol{r}_{ni} -
 *      \boldsymbol{\omega} \times (\boldsymbol{\omega} \times \boldsymbol{r}_{ni}) -
 *      2(\boldsymbol{\omega} \times \boldsymbol{v}_{ni})
 * \f]
 * where \f$\boldsymbol{a}_B\f$ is the acceleration of the non-inertial frame with respect to
 * an inertial reference frame, \f$\boldsymbol{\omega}\f$ is the respective rotation rate
 * and \f$\boldsymbol{r}_{ni}\f$ and \f$\boldsymbol{v}_{ni}\f$ are the object's position and
 * velocity vector in the non-inertial frame in which the apparent acceleration is computed.
 * \param accelerationOfNonInertialReferenceFrame Acceleration vector of the non-inertial
 *          frame with respect to an inertial reference frame [m s^-2].
 * \param angularVelocityOfNonInertialReferenceFrame Angular velocity vector of the non-inertial
 *          frame with respect to an inertial reference frame [rad s^-1].
 * \param angularAccelerationOfNonInertialReferenceFrame Angular acceleration vector of the
 *          non-inertial frame with respect to an inertial reference frame [rad s^-1].
 * \param positionOfBodyInNonInertialReferenceFrame Position vector of body in the non-inertial
 *          frame of reference in which the apparent acceleration is computed [m].
 * \param velocityOfBodyInNonInertialReferenceFrame Velocity vector of body in the non-inertial
 *          frame of reference in which the apparent acceleration is computed [m s^-1].
 * \return Apparent acceleration as seen by an observer in the rotating and accelerating frame.
 */
Eigen::Vector3d computeApparentAcceleration(
        const Eigen::Vector3d& accelerationOfNonInertialReferenceFrame,
        const Eigen::Vector3d& angularVelocityOfNonInertialReferenceFrame,
        const Eigen::Vector3d& angularAccelerationOfNonInertialReferenceFrame,
        const Eigen::Vector3d& positionOfBodyInNonInertialReferenceFrame,
        const Eigen::Vector3d& velocityOfBodyInNonInertialReferenceFrame );

//! Compute centripetal acceleration due to non-inertiality of reference frame.
/*!
  * Computes centripetal acceleration experienced by an object, of which the position is
  * provided in a rotating reference frame.
  * The centripetal acceleration of a particle due to the non-inertiality of the reference frame in
  * which its state is given, is given by following equation:
  * \f[
  *      \boldsymbol{a}_{centripetal} =
  *      -\boldsymbol{\omega} \times (\boldsymbol{\omega} \times \boldsymbol{r}_{ni})
  * \f]
  * where \f$\boldsymbol{\omega}\f$ is the rotation rate of the non-inertial frame with respect to
  * an inertial reference frame  and \f$\boldsymbol{r}_{ni}\f$ is the object's position in the
  * non-inertial frame in which the apparent acceleration is computed.
  * \param angularVelocityOfNonInertialReferenceFrame Angular velocity vector of the non-inertial
  *          frame with respect to an inertial reference frame [rad s^-1].
  * \param positionOfBodyInNonInertialReferenceFrame Position vector of body in the non-inertial
  *          frame of reference in which the apparent acceleration is computed [m].
  * \return Centripetal acceleration as seen by an observer in the rotating frame.
  */
Eigen::Vector3d computeCentripetalAcceleration(
        const Eigen::Vector3d& angularVelocityOfNonInertialReferenceFrame,
        const Eigen::Vector3d& positionOfBodyInNonInertialReferenceFrame );

//! Compute Coriolis acceleration due to non-inertiality of reference frame.
/*!
  * Computes Coriolis acceleration experienced by an object, of which the position is
  * provided in a rotating reference frame.
  * The Coriolis acceleration of a particle due to the non-inertiality of the reference frame in
  * which its state is given, is given by following equation:
  * \f[
  *      \boldsymbol{a}_{Coriolis} =
  *      -2(\boldsymbol{\omega} \times \boldsymbol{v}_{ni})
  * \f]
  * where \f$\boldsymbol{\omega}\f$ is the rotation rate of the non-inertial frame with respect to
  * an inertial reference frame and \f$\boldsymbol{v}_{ni}\f$ is the object's velocity in the
  * non-inertial frame in which the apparent acceleration is computed.
  * \param angularVelocityOfNonInertialReferenceFrame Angular velocity vector of the non-inertial
  *          frame with respect to an inertial reference frame [rad s^-1].
  * \param velocityOfBodyInNonInertialReferenceFrame Velocity vector of body in the non-inertial
  *          frame of reference in which the apparent acceleration is computed [m s^-1].
  * \return Coriolis acceleration as seen by an observer in the rotating frame.
  */
Eigen::Vector3d computeCoriolisAcceleration(
        const Eigen::Vector3d& angularVelocityOfNonInertialReferenceFrame,
        const Eigen::Vector3d& velocityOfBodyInNonInertialReferenceFrame );

//! Compute Euler acceleration due to non-inertiality of reference frame.
/*!
  * Computes Euler acceleration experienced by an object, of which the position is
  * provided in a rotationally accelerating reference frame.
  * The Euler acceleration of a particle due to the non-inertiality of the reference frame in
  * which its state is given, is given by following equation:
  * \f[
  *      \boldsymbol{a}_{Euler} =
  *      - \dot{\boldsymbol{\omega}} \times \boldsymbol{r}_{ni}
  * \f]
  * where \f$\dot{\boldsymbol{\omega}}\f$ is the rate of change of the rotation rate of the
  * non-inertial frame with respect to an inertial reference frame and \f$\boldsymbol{r}_{ni}\f$
  * is the object's position in the non-inertial frame in which the apparent acceleration is
  * computed.
  * \param angularAccelerationOfNonInertialReferenceFrame Angular acceleration vector of the
  *          non-inertial frame with respect to an inertial reference frame [rad s^-1].
  * \param positionOfBodyInNonInertialReferenceFrame Position vector of body in the non-inertial
  *          frame of reference in which the apparent acceleration is computed [m].
  * \return Euler acceleration as seen by an observer in the rotationally accelerating frame.
  */
Eigen::Vector3d computeEulerAcceleration(
        const Eigen::Vector3d& angularAccelerationOfNonInertialReferenceFrame,
        const Eigen::Vector3d& positionOfBodyInNonInertialReferenceFrame );

//! Apparent acceleration model class.
/*!
 * Implementation of apparent acceleration due to non-inertiality of a reference system in which
 * the equations of motion are evaluated. It evaluates the total apparent acceleration due to the
 * acceleration of the reference frame, as well as the Coriolis, centripetal and Euler
 * accelerations.
 */
class ApparentAccelerationModel : public basic_astrodynamics::AccelerationModel < Eigen::Vector3d >
{
private:

    //! Typedef for Eigen::Vector3d returning function.
    typedef boost::function< Eigen::Vector3d( ) > Vector3dReturningFunction;

public:

    //! Class constructor.
    /*!
     * Constructor for apparent acceleration model.
     * \param accelerationOfNonInertialReferenceFrame Pointer to a function returning
     *         the acceleration vector of the non-intertial reference frame w.r.t. an
     *         inertial frame of reference.
     * \param angularVelocityOfNonInertialReferenceFrame Pointer to a function returning
     *         the angular velocity vector of the non-intertial reference frame w.r.t. an
     *         inertial frame of reference.
     * \param angularAccelerationOfNonInertialReferenceFrame Pointer to a function returning
     *         the angular acceleration vector of the non-inertial frame with respect to an
     *         inertial reference frame.
     * \param positionOfBodyInNonInertialReferenceFrame Pointer to a function returning
     *         the position vector in the non-inertial frame of reference in which the apparent
     *         acceleration is computed.
     * \param velocityOfBodyInNonInertialReferenceFrame Pointer to a function returning
     *          the velocity vector in the non-inertial frame of reference in which the apparent
     *          acceleration is computed.
     */
    ApparentAccelerationModel(
            Vector3dReturningFunction accelerationOfNonInertialReferenceFrameFunction,
            Vector3dReturningFunction angularVelocityOfNonInertialReferenceFrameFunction,
            Vector3dReturningFunction angularAccelerationOfNonInertialReferenceFrameFunction,
            Vector3dReturningFunction positionOfBodyInNonInertialReferenceFrameFunction,
            Vector3dReturningFunction velocityOfBodyInNonInertialReferenceFrameFunction )
        : accelerationOfNonInertialReferenceFrameFunction_(
              accelerationOfNonInertialReferenceFrameFunction ),
          angularVelocityOfNonInertialReferenceFrameFunction_(
              angularVelocityOfNonInertialReferenceFrameFunction ),
          angularAccelerationOfNonInertialReferenceFrameFunction_(
              angularAccelerationOfNonInertialReferenceFrameFunction ),
          positionOfBodyInNonInertialReferenceFrameFunction_(
              positionOfBodyInNonInertialReferenceFrameFunction ),
          velocityOfBodyInNonInertialReferenceFrameFunction_(
              velocityOfBodyInNonInertialReferenceFrameFunction )
    {
        updateMembers( );
    }

    //! Get apparent acceleration.
    /*!
     * Computes and returns the acceleration.
     * \return Vector of the apparent acceleration in the non-inertial Cartesian frame in which the
     *          object's state is defined.
     */
    Eigen::Vector3d getAcceleration( )
    {
        return computeApparentAcceleration( currentAccelerationOfNonInertialReferenceFrame_,
                                            currentAngularVelocityOfNonInertialReferenceFrame_,
                                            currentAngularAccelerationOfNonInertialReferenceFrame_,
                                            currentPositionOfBodyInNonInertialReferenceFrame_,
                                            currentVelocityOfBodyInNonInertialReferenceFrame_ );
    }

    //! Function to update member variables used by this acceleration model.
    /*!
     * Function to update member variables used by this acceleration model. The variables
     * that are required as input for the evaluation of the accelerations are retrieved from
     * boost::functions that may or may not give constant return values, depending on the
     * user input. This function sets the member variables using these functions.
     * \return True if update is successful.
     */
    bool updateMembers( );

protected:

private:

    //! Function returning translational acceleration vector of non-inertial reference frame.
    /*!
     * Function returning translational acceleration vector of non-inertial reference frame.
     */
    const Vector3dReturningFunction accelerationOfNonInertialReferenceFrameFunction_;

    //! Function returning angular velocity vector of non-inertial reference frame.
    /*!
     * Function returning angular velocity vector of non-inertial reference frame.
     */
    const Vector3dReturningFunction angularVelocityOfNonInertialReferenceFrameFunction_;

    //! Function returning angular acceleration vector of non-inertial reference frame.
    /*!
     * Function returning angular acceleration vector of non-inertial reference frame.
     */
    const Vector3dReturningFunction angularAccelerationOfNonInertialReferenceFrameFunction_;

    //! Function returning position vector in non-inertial reference frame.
    /*!
     * Function returning position vector in non-inertial reference frame.
     */
    const Vector3dReturningFunction positionOfBodyInNonInertialReferenceFrameFunction_;

    //! Function returning velocity vector in non-inertial reference frame.
    /*!
     * Function returning velocity vector in non-inertial reference frame.
     */
    const Vector3dReturningFunction velocityOfBodyInNonInertialReferenceFrameFunction_;

    //! Current translational acceleration vector of non-inertial reference frame.
    /*!
     * Current translational acceleration vector of non-inertial reference frame. This vector
     * is updated by the updateMembers( ) function.
     */
    Eigen::Vector3d currentAccelerationOfNonInertialReferenceFrame_;

    //! Current angular velocity vector of non-inertial reference frame.
    /*!
     * Current angular velocity vector of non-inertial reference frame. This vector
     * is updated by the updateMembers( ) function.
     */
    Eigen::Vector3d currentAngularVelocityOfNonInertialReferenceFrame_;

    //! Current angular acceleration vector of non-inertial reference frame.
    /*!
     * Current angular acceleration vector of non-inertial reference frame. This vector
     * is updated by the updateMembers( ) function.
     */
    Eigen::Vector3d currentAngularAccelerationOfNonInertialReferenceFrame_;

    //! Current position vector in non-inertial reference frame.
    /*!
     * Current position vector in non-inertial reference frame. This vector
     * is updated by the updateMembers( ) function.
     */
    Eigen::Vector3d currentPositionOfBodyInNonInertialReferenceFrame_;

    //! Current velocity vector in non-inertial reference frame.
    /*!
     * Current velocity vector in non-inertial reference frame. This vector
     * is updated by the updateMembers( ) function.
     */
    Eigen::Vector3d currentVelocityOfBodyInNonInertialReferenceFrame_;
};

}   // namespace acceleration_models
}   // namespace tudat

#endif // TUDAT_APPARENT_ACCELERATION_MODEL_H
