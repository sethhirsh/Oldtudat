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
 *      130301    E.D. Brandon      Creation of code.
 *      130325    D.Dirkx           Split code into separate files for different functions.
 *
 *    References
 *      Novak, D.M. and M. Vasile. Improved Shaping Approach to the Preliminary Design of Low-
 *          Thrust Trajectories, Journal of Guidance, Control, and Dynamics 34(1), pp. 128�147,
 *          2011.
 *      Wall, B.J. and D. Novak. A 3D Shape-Based Approximation Method for Low-Thrust Trajectory
 *          Design, Advances in the Astronautical Sciences 142(2), pp. 1163-1176, 2012.
 *      Wall, B.J., Pols, B. and B. Lanktree. Shape-Based Approximation Method for Low-Thrust
 *          Interception and Rendezvous Trajectory Design, Advances in the Astronautical Sciences
 *          136(2), pp. 1447-1458, 2010.
 *
 *    Notes
 *      This file contains the improved inverse polynomial function, which is described by Wall et
 *      al. [2010] (See also Wall and Novak [2012]). This function is a mathematical representation
 *      of the radial position, r (in spherical coordinates), of a thrusting spacecraft. The
 *      improved inverse polynomial function can be used to approximate a continuous-thrust
 *      trajectory (also referred to as a low-thrust trajectory), when combined with a function
 *      which represents the (out-of-plane) elevation angle, phi (in spherical coordinates), of a
 *      thrusting spacecraft. The spherical coordinate system that is used for the calculations and
 *      the descriptions is taken from Novak and Vasile [2011].
 */

#ifndef TUDAT_IMPROVED_INVERSE_POLYNOMIAL_WALL_H
#define TUDAT_IMPROVED_INVERSE_POLYNOMIAL_WALL_H

#include <boost/function.hpp>
#include <boost/make_shared.hpp>

#include <Eigen/Core>

#include "Tudat/Mathematics/BasicMathematics/function.h"

namespace tudat
{
namespace mission_segments
{

//! Improved inverse polynomial function approximating the radial position of a thrusting
//! spacecraft.
/*!
 * This class contains an improved inverse polynomial function, and its exact first and second
 * derivative w.r.t. the azimuthal angle \f$ \theta \f$. It is a mathematical function, and can be
 * used to approximate the radial position of a thrusting spacecraft. The function is documented
 * by Wall et al. [2010] as the "improved inverse polynomial method". The radial position of the
 * spacecraft is parameterized in terms of an independent variable, the (in-plane) azimuthal angle
 * \f$ \theta \f$. The first and second derivative are taken with respect to this independent
 * variable.
 *
 * The function is completely described by the independent variable \f$ \theta \f$ and a set of
 * seven parameters. These parameters are related to the boundary conditions of the problem: three
 * initial conditions, three final conditions and the total time-of-flight. One of the parameters
 * is used to solve for the time-of-flight and is therefore a time-dependent parameter. These
 * seven parameters are passed to this class as boost::functions to facilitate the flexible
 * external manipulation of their values.
 *
 */
class ImprovedInversePolynomialWall : public basic_mathematics::Function< >
{
public:

    //! Default constructor with immediate definition of parameters.
    /*!
     * Default constructor with immediate definition of parameters through boost::functions.
     * This setup allows for a flexible external manipulation of the values of the parameters.
     *
     * \param aTimeDependentParameter The parameter that is used to solve for the time-of-flight.
     *      This parameter is equivalent to parameter d from Wall et al. [2010].
     * \param aSetOfBoundaryParameters A set of six parameters, related to the position boundary
     *      conditions.
     *      These parameters are equivalent to the parameters a, b, c, e, f, and g from Wall et al.
     *      [2010]. The order is important!
     *      aSetOfBoundaryParameters.first( 0 ) = a
     *      aSetOfBoundaryParameters.first( 1 ) = b
     *      aSetOfBoundaryParameters.first( 2 ) = c
     *      aSetOfBoundaryParameters.second( 0 ) = e
     *      aSetOfBoundaryParameters.second( 1 ) = f
     *      aSetOfBoundaryParameters.second( 2 ) = g
     */
    ImprovedInversePolynomialWall( const boost::function< double(  ) > aTimeDependentParameter,
                                   const boost::function< std::pair< Eigen::Vector3d ,
                                   Eigen::Vector3d >(  ) > aSetOfBoundaryParameters ) :
        timeDependentParameter_( aTimeDependentParameter ),
        boundaryParameters_( aSetOfBoundaryParameters ){  }

    //! Default destructor.
    /*!
     * Default destructor.
     */
    ~ImprovedInversePolynomialWall(  ){  }

    //! Evaluate the function value for a given (in-plane) azimuthal angle.
    /*!
     * Evaluates the inverse polynomial function for a given (in-plane) azimuthal angle
     * \f$ \theta \f$.
     *
     * The (improved version of the) inverse polynomial function is given by Wall et al. [2010] as:
     *
     * \f[
     *      r ( \theta ) = \frac{ 1 }
     *      { a + b cos( \theta + c ) + d \theta^3 + e \theta^4 + f \theta^5 + g \theta^6 }
     * \f]
     *
     * in which \f$ r \f$ is the radial position of the spacecraft, \f$ \theta \f$ is the
     * (in-plane) azimuthal angle and \f$ ( a, b, c, d, e, f, g ) \f$ are parameters that define
     * the shape of the trajectory. The parameter \f$ d \f$ is the time-dependent parameter which
     * is used to solve for a required time-of-flight.
     *
     * \param anAzimuthalAngle Value of the (in-plane) azimuthal angle [rad].
     * \return The radial position at \f$ \theta \f$ [m].
     */
    double evaluate( const double anAzimuthalAngle );

    //! Compute the derivative of the function.
    /*!
     * Computes the derivative of a given order of the function, for a given (in-plane) azimuthal
     * angle \f$ \theta \f$. It only evaluates the first or second derivative, therefore the order
     * should be either 1 or 2.
     *
     * The first derivative of the function is:
     *
     * \f[
     *      \frac{ dr }{ d\theta } ( \theta ) =
     *      -r^2 \left( -b sin \left( \theta + c \right) + 3 d \theta^2 + 4 e \theta^3 + 5 f
     *      \theta^4 + 6 g \theta^5 \right)
     * \f]
     *
     * The second derivative of the function is:
     *
     * \f[
     *      \frac{ d^2 r }{ d \theta^2 } ( \theta ) =
     *      2 r^3 \left( -b sin \left( \theta + c \right) + 3 d \theta^2 + 4 e \theta^3 + 5 f
     *      \theta^4 + 6 g \theta^5 \right)^2 -
     *      r^2 \left( -b cos \left( \theta + c \right) + 6 d \theta + 12 e \theta^2 + 20 f
     *      \theta^3 + 30 g \theta^4 \right)
     * \f]
     *
     * When substituting for the first derivative, the equation becomes:
     *
     * \f[
     *      \frac{ d^2 r }{ d \theta^2 } ( \theta ) =
     *      \frac{ 2 }{ r } r^{ \prime 2 } - r^2 \left( -b cos \left( \theta + c \right)
     *      + 6 d \theta + 12 e \theta^2 + 20 f \theta^3 + 30 g \theta^4 \right)
     * \f]
     *
     * In these formulas \f$ r \f$ is the radial position of the spacecraft, \f$ \theta \f$ is the
     * (in-plane) azimuthal angle, the prime indicates the derivative with respect to
     *  \f$ \theta \f$, and \f$ ( a, b, c, d, e, f, g ) \f$ are parameters that define the shape of
     * the trajectory. The parameter \f$ d \f$ is the time-dependent parameter which is used to
     * solve for a required time-of-flight.
     *
     * \param order             Order of the derivative to evaluate. Can be either 1 or 2.
     * \param anAzimuthalAngle  Value of the (in-plane) azimuthal angle [rad].
     * \return Value of the derivative of order 1 or 2 of the function [m rad^-1, m rad^-2].
     */
    double computeDerivative( const unsigned int order, const double anAzimuthalAngle );

    //! Compute the definite integral of the function.
    /*!
     * This function throws a runtime error when it is called. The integral of the inverse
     * polynomial function is not part of the shape-based method and has not been implemented
     */
    double computeDefiniteIntegral( const unsigned int order,
                                    const double lowerBound,
                                    const double upperBound );

protected:

private:

    //! The time-dependent parameter.
    /*!
     * The time-dependent parameter, which is used to satisfy a required time-of-flight.
     */
    boost::function< double(  ) > timeDependentParameter_;

    //! The parameters of the function, related to the boundary conditions.
    /*!
     * The six time-independent parameters of the function. These parameters determine the shape of
     * the trajectory and are dependent on the initial and final boundary conditions.
     *
     * The first three parameters are dependent on the initial boundary conditions and the last
     * three parameters are dependent on the final boundary conditions plus the time-dependent
     * parameter 'aTimeDependentParameter'. The parameters correspond to Wall et al. [2010] in the
     * following manner: the first three parameters are a, b and c; the last three parameters are
     * e, f, and g.
     *
     * The order is important!
     * boundaryParameters_.first( 0 ) = a
     * boundaryParameters_.first( 1 ) = b
     * boundaryParameters_.first( 2 ) = c
     * boundaryParameters_.second( 0 ) = e
     * boundaryParameters_.second( 1 ) = f
     * boundaryParameters_.second( 2 ) = g
     */
    boost::function< std::pair< Eigen::Vector3d , Eigen::Vector3d >(  ) > boundaryParameters_;
};

//! Typedef for shared-pointer to ImprovedInversePolynomialWall object.
typedef boost::shared_ptr< ImprovedInversePolynomialWall > ImprovedInversePolynomialWallPointer;

} // namespace mission_segments
} // namespace tudat

#endif // TUDAT_IMPROVED_INVERSE_POLYNOMIAL_WALL_H
