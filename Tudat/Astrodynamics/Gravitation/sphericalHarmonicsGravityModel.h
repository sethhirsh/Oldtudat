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
 *      121017    E. Dekens         Code created.
 *      121022    K. Kumar          Added wrapper class for general spherical harmonics.
 *      121105    K. Kumar          Simplified wrapper class for general spherical harmonics,
 *                                  renamed file, and merged content from other files.
 *      121210    D. Dirkx          Simplified class by removing template parameters.
 *      130224    K. Kumar          Updated include guard name; corrected Doxygen errors.
 *
 *    References
 *      Heiskanen, W.A., Moritz, H. Physical geodesy. Freeman, 1967.
 *
 *    Notes
 *      The class implementation currently only wraps the geodesy-normalized free function to
 *      compute the gravitational acceleration. Maybe in future, using an enum, the user can be
 *      given the choice of the free function to wrap.
 *
 */

#ifndef TUDAT_SPHERICAL_HARMONICS_GRAVITY_MODEL_H
#define TUDAT_SPHERICAL_HARMONICS_GRAVITY_MODEL_H

#include <stdexcept>

#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/shared_ptr.hpp>

#include <Eigen/Core>

#include "Tudat/Astrodynamics/BasicAstrodynamics/accelerationModel.h"
#include "Tudat/Astrodynamics/Gravitation/sphericalHarmonicsGravityModelBase.h"

namespace tudat
{
namespace gravitation
{

//! Compute gravitational acceleration due to multiple spherical harmonics terms, defined using
//! geodesy-normalization.
/*!
 * This function computes the acceleration caused by gravitational spherical harmonics, with the
 * coefficients expressed using a geodesy-normalization. This acceleration is the summation of all
 * harmonic terms from degree and order zero, up to a user-specified highest degree and order. The
 * harmonic coefficients for the function must be provided in geodesy-normalized format. This
 * geodesy-normalization is defined as:
 * \f{eqnarray*}
 *  {
 *     \bar{ C }_{ n, m } = \Pi_{ n, m } C_{ n, m } \\
 *     \bar{ S }_{ n, m } = \Pi_{ n, m } S_{ n, m }
 * \f}
 * in which \f$ \bar{ C }_{ n, m } \f$ and \f$ \bar{ S }_{ n, m } \f$ are a geodesy-normalized
 * cosine and sine harmonic coefficient respectively (of degree \f$ n \f$ and order \f$ m \f$). The
 * unnormalized harmonic coefficients are represented by \f$ C_{ n, m } \f$ and \f$ S_{ n, m } \f$.
 * The normalization factor \f$ \Pi_{ n, m } \f$ is given by Heiskanen & Moritz [1967] as:
 * \f[
 *     \Pi_{ n, m } = \sqrt{ \frac{ ( n + m )! }{ ( 2 - \delta_{ 0, m } ) ( 2 n + 1 ) ( n - m )! } }
 * \f]
 * in which \f$ n \f$ is the degree, \f$ m \f$ is the order and \f$ \delta_{ 0, m } \f$ is the
 * Kronecker delta.
 * \param positionOfBodySubjectToAcceleration Cartesian position vector with respect to the
 *          reference frame that is associated with the harmonic coefficients.
 *          The order is important!
 *          position( 0 ) = x coordinate [m],
 *          position( 1 ) = y coordinate [m],
 *          position( 2 ) = z coordinate [m].
 * \param cosineHarmonicCoefficients Matrix with <B>geodesy-normalized</B> cosine harmonic
 *          coefficients. The row index indicates the degree and the column index indicates the order
 *          of coefficients.
 * \param sineHarmonicCoefficients Matrix with <B>geodesy-normalized</B> sine harmonic coefficients.
 *          The row index indicates the degree and the column index indicates the order of
 *          coefficients. The matrix must be equal in size to cosineHarmonicCoefficients.
 * \param gravitationalParameter Gravitational parameter associated with the spherical harmonics
 *          [m^3 s^-2].
 * \param equatorialRadius Reference radius of the spherical harmonics [m].
 * \return Cartesian acceleration vector resulting from the summation of all harmonic terms.
 *           The order is important!
 *           acceleration( 0 ) = x acceleration [m s^-2],
 *           acceleration( 1 ) = y acceleration [m s^-2],
 *           acceleration( 2 ) = z acceleration [m s^-2].
 */
Eigen::Vector3d computeGeodesyNormalizedGravitationalAccelerationSum(
        const Eigen::Vector3d& positionOfBodySubjectToAcceleration,
        const double gravitationalParameter,
        const double equatorialRadius,
        const Eigen::MatrixXd& cosineHarmonicCoefficients,
        const Eigen::MatrixXd& sineHarmonicCoefficients );

//! Compute gravitational acceleration due to single spherical harmonics term.
/*!
 * This function computes the acceleration caused by a single gravitational spherical harmonics
 * term, with the coefficients expressed using a geodesy-normalization. The harmonic coefficients
 * for the function must be provided in geodesy-normalized format. This geodesy-normalization is
 * defined as:
 * \f{eqnarray*}
 *  {
 *     \bar{ C }_{ n, m } = \Pi_{ n, m } C_{ n, m } \\
 *     \bar{ S }_{ n, m } = \Pi_{ n, m } S_{ n, m }
 * \f}
 * in which \f$ \bar{ C }_{ n, m } \f$ and \f$ \bar{ S }_{ n, m } \f$ are a geodesy-normalized
 * cosine and sine harmonic coefficient respectively (of degree \f$ n \f$ and order \f$ m \f$). The
 * unnormalized harmonic coefficients are represented by \f$ C_{ n, m } \f$ and \f$ S_{ n, m } \f$.
 * The normalization factor \f$ \Pi_{ n, m } \f$ is given by Heiskanen & Moritz [1967] as:
 * \f[
 *     \Pi_{ n, m } = \sqrt{ \frac{ ( n + m )! }{ ( 2 - \delta_{ 0, m } ) ( 2 n + 1 ) ( n - m )! } }
 * \f]
 * in which \f$ n \f$ is the degree, \f$ m \f$ is the order and \f$ \delta_{ 0, m } \f$ is the
 * Kronecker delta.
 * \param positionOfBodySubjectToAcceleration Cartesian position vector with respect to the
 *          reference frame that is associated with the harmonic coefficients.
 *          The order is important!
 *          position( 0 ) = x coordinate [m],
 *          position( 1 ) = y coordinate [m],
 *          position( 2 ) = z coordinate [m].
 * \param degree Degree of the harmonic term.
 * \param order Order of the harmonic term.
 * \param cosineHarmonicCoefficient <B>Geodesy-normalized</B> cosine harmonic
 *          coefficient.
 * \param sineHarmonicCoefficient <B>Geodesy-normalized</B> sine harmonic coefficient.
 * \param gravitationalParameter Gravitational parameter associated with the spherical harmonic
 *          [m^3 s^-2].
 * \param equatorialRadius Reference radius of the spherical harmonic [m].
 * \return Cartesian acceleration vector resulting from the spherical harmonic term.
 *           The order is important!
 *           acceleration( 0 ) = x acceleration [m s^-2],
 *           acceleration( 1 ) = y acceleration [m s^-2],
 *           acceleration( 2 ) = z acceleration [m s^-2].
 */
Eigen::Vector3d computeSingleGeodesyNormalizedGravitationalAcceleration(
        const Eigen::Vector3d& positionOfBodySubjectToAcceleration,
        const double gravitationalParameter,
        const double equatorialRadius,
        const int degree,
        const int order,
        const double cosineHarmonicCoefficient,
        const double sineHarmonicCoefficient );

//! Template class for general spherical harmonics gravitational acceleration model.
/*!
 * This templated class implements a general spherical harmonics gravitational acceleration model.
 * The acceleration computed with this class is based on the geodesy-normalization described by
 * (Heiskanen & Moritz, 1967), implemented in the
 * computeGeodesyNormalizedGravitationalAccelerationSum() function. The acceleration computed is a
 * sum, based on the matrix of coefficients of the model provided.
 * \tparam CoefficientMatrixType Data type for cosine and sine coefficients in spherical harmonics
 *         expansion; may be used for compile-time definition of maximum degree and order.
 */
template< typename CoefficientMatrixType = Eigen::MatrixXd >
class SphericalHarmonicsGravitationalAccelerationModel
        : public basic_astrodynamics::AccelerationModel< Eigen::Vector3d >,
        public SphericalHarmonicsGravitationalAccelerationModelBase< Eigen::Vector3d >
{
private:

    //! Typedef for base class.
    typedef SphericalHarmonicsGravitationalAccelerationModelBase< Eigen::Vector3d > Base;

    //! Typedef for coefficient-matrix-returning function.
    typedef boost::function< CoefficientMatrixType( ) > CoefficientMatrixReturningFunction;

public:

    //! Constructor taking position-functions for bodies, and constant parameters of spherical
    //! harmonics expansion.
    /*!
     * Constructor taking a pointer to a function returning the position of the body subject to
     * gravitational acceleration, constant gravitational parameter and equatorial radius of the
     * body exerting the acceleration, constant coefficient matrices for the spherical harmonics
     * expansion, and a pointer to a function returning the position of the body exerting the
     * gravitational acceleration (typically the central body). This constructor uses the
     * Boost::lambda library to create a function on-the-fly that returns the constant
     * gravitational parameter, equatorial radius and coefficient matrices provided. The
     * constructor also updates all the internal members. The position of the body exerting the
     * gravitational acceleration is an optional parameter; the default position is the origin.
     * \param positionOfBodySubjectToAccelerationFunction Pointer to function returning position of
     *          body subject to gravitational acceleration.
     * \param aGravitationalParameter A (constant) gravitational parameter [m^2 s^-3].
     * \param anEquatorialRadius A (constant) equatorial radius [m].
     * \param aCosineHarmonicCoefficientMatrix A (constant) cosine harmonic coefficient matrix.
     * \param aSineHarmonicCoefficientMatrix A (constant) sine harmonic coefficient matrix.
     * \param positionOfBodyExertingAccelerationFunction Pointer to function returning position of
     *          body exerting gravitational acceleration (default = (0,0,0)).
     */
    SphericalHarmonicsGravitationalAccelerationModel(
            const StateFunction positionOfBodySubjectToAccelerationFunction,
            const double aGravitationalParameter,
            const double anEquatorialRadius,
            const CoefficientMatrixType aCosineHarmonicCoefficientMatrix,
            const CoefficientMatrixType aSineHarmonicCoefficientMatrix,
            const StateFunction positionOfBodyExertingAccelerationFunction
            = boost::lambda::constant( Eigen::Vector3d::Zero( ) ) )
        : Base( positionOfBodySubjectToAccelerationFunction,
                aGravitationalParameter,
                positionOfBodyExertingAccelerationFunction ),
          equatorialRadius( anEquatorialRadius ),
          getCosineHarmonicsCoefficients(
              boost::lambda::constant(aCosineHarmonicCoefficientMatrix ) ),
          getSineHarmonicsCoefficients( boost::lambda::constant(aSineHarmonicCoefficientMatrix ) )
    {
        this->updateMembers( );
    }

    //! Constructor taking functions for position of bodies, and parameters of spherical harmonics
    //! expansion.
    /*!
     * Constructor taking pointer to functions returning the position of the body subject to
     * gravitational acceleration, the gravitational parameter of the body exerting the
     * acceleration (central body), the equatorial radius of the central body, the coefficient
     * matrices of the spherical harmonics expansion, and the position of the central body. The
     * constructor also updates all the internal members. The position of the body exerting the
     * gravitational acceleration is an optional parameter; the default position is the origin.
     * \param positionOfBodySubjectToAccelerationFunction Pointer to function returning position of
     *          body subject to gravitational acceleration.
     * \param gravitationalParameterFunction Pointer to function returning gravitational parameter.
     * \param equatorialRadiusFunction Pointer to function returning equatorial radius.
     * \param cosineHarmonicCoefficientsFunction Pointer to function returning matrix of
                cosine-coefficients of spherical harmonics expansion.
     * \param cosineHarmonicCoefficientsFunction Pointer to function returning matrix of
                sine-coefficients of spherical harmonics expansion.
     * \param positionOfBodyExertingAccelerationFunction Pointer to function returning position of
     *          body exerting gravitational acceleration (default = (0,0,0)).
     */
    SphericalHarmonicsGravitationalAccelerationModel(
            const StateFunction positionOfBodySubjectToAccelerationFunction,
            const double aGravitationalParameter,
            const double anEquatorialRadius,
            const CoefficientMatrixReturningFunction cosineHarmonicCoefficientsFunction,
            const CoefficientMatrixReturningFunction sineHarmonicCoefficientsFunction,
            const StateFunction positionOfBodyExertingAccelerationFunction
            = boost::lambda::constant( Eigen::Vector3d::Zero( ) ) )
        : Base( positionOfBodySubjectToAccelerationFunction,
                aGravitationalParameter,
                positionOfBodyExertingAccelerationFunction ),
          equatorialRadius( anEquatorialRadius ),
          getCosineHarmonicsCoefficients( cosineHarmonicCoefficientsFunction ),
          getSineHarmonicsCoefficients( sineHarmonicCoefficientsFunction )
    {
        this->updateMembers( );
    }

    //! Get gravitational acceleration.
    /*!
     * Returns the gravitational acceleration computed using the input parameters provided to the
     * class. This function serves as a wrapper for the
     * computeGeodesyNormalizedGravitationalAccelerationSum() function.
     * \return Computed gravitational acceleration vector.
     */
    Eigen::Vector3d getAcceleration( );

    //! Update class members.
    /*!
     * Updates all the base class members to their current values and also updates the class
     * members of this class.
     */
    void updateMembers( )
    {
        cosineHarmonicCoefficients = getCosineHarmonicsCoefficients( );
        sineHarmonicCoefficients = getSineHarmonicsCoefficients( );
        this->updateBaseMembers( );
    }

protected:

private:

    //! Equatorial radius [m].
    /*!
     * Current value of equatorial (planetary) radius used for spherical harmonics expansion [m].
    */
    const double equatorialRadius;

    //! Matrix of cosine coefficients.
    /*!
     * Matrix containing coefficients of cosine terms for spherical harmonics expansion.
     */
    CoefficientMatrixType cosineHarmonicCoefficients;

    //! Matrix of sine coefficients.
    /*!
     * Matrix containing coefficients of sine terms for spherical harmonics expansion.
     */
    CoefficientMatrixType sineHarmonicCoefficients;

    //! Pointer to function returning cosine harmonics coefficients matrix.
    /*!
     * Pointer to function that returns the current coefficients of the cosine terms of the
     * spherical harmonics expansion.
     */
    const CoefficientMatrixReturningFunction getCosineHarmonicsCoefficients;

    //! Pointer to function returning sine harmonics coefficients matrix.
    /*!
     * Pointer to function that returns the current coefficients of the sine terms of the
     * spherical harmonics expansion.
     */
    const CoefficientMatrixReturningFunction getSineHarmonicsCoefficients;
};

//! Typedef for SphericalHarmonicsGravitationalAccelerationModelXd.
typedef SphericalHarmonicsGravitationalAccelerationModel< >
SphericalHarmonicsGravitationalAccelerationModelXd;

//! Typedef for shared-pointer to SphericalHarmonicsGravitationalAccelerationModelXd.
typedef boost::shared_ptr< SphericalHarmonicsGravitationalAccelerationModelXd >
SphericalHarmonicsGravitationalAccelerationModelXdPointer;

// Template class source.
// The code given below is effectively the ".cpp file" for the template class definition, so you
// only need to look at the code below if you are interested in the source implementation.

//! Get gravitational acceleration.
template< typename CoefficientMatrixType >
Eigen::Vector3d SphericalHarmonicsGravitationalAccelerationModel< CoefficientMatrixType >
::getAcceleration( )
{
    return computeGeodesyNormalizedGravitationalAccelerationSum(
                this->positionOfBodySubjectToAcceleration
                - this->positionOfBodyExertingAcceleration,
                gravitationalParameter,
                equatorialRadius,
                cosineHarmonicCoefficients,
                sineHarmonicCoefficients );
}

} // namespace gravitation
} // namespace tudat

#endif // TUDAT_SPHERICAL_HARMONICS_GRAVITY_MODEL_H
