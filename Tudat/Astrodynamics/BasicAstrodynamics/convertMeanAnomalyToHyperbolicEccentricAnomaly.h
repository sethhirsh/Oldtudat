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
 *      110214    K. Kumar          First creation of code.
 *      110810    J. Leloux         Corrected doxygen documentation.
 *
 *    References
 *      Chobotov, V.A. Orbital Mechanics, Third Edition, AIAA Education Series, VA, 2002.
 *      http://www.cdeagle.com/omnum/pdf/demokep1.pdf, last accessed: 16th February, 2011.
 *
 */

#ifndef TUDAT_CONVERT_MEAN_ANOMALY_TO_HYPERBOLIC_ECCENTRIC_ANOMALY_H
#define TUDAT_CONVERT_MEAN_ANOMALY_TO_HYPERBOLIC_ECCENTRIC_ANOMALY_H

#include "Tudat/Astrodynamics/BasicAstrodynamics/convertMeanAnomalyBase.h"

namespace tudat
{
namespace orbital_element_conversions
{

//! Definition of mean anomaly to hyperbolic eccentric anomaly converter class.
/*!
 * Definition of mean anomaly to hyperbolic eccentric anomaly converter class.
 */
class ConvertMeanAnomalyToHyperbolicEccentricAnomaly : public ConvertMeanAnomalyBase
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    ConvertMeanAnomalyToHyperbolicEccentricAnomaly( ) : hyperbolicEccentricAnomaly_( -1.0 ) { }

    //! Convert mean anomaly to hyperbolic eccentric anomaly.
    /*!
     * Converts mean anomaly to hyperbolic eccentric anomaly for hyperbolic orbits. Currently, the
     * conversion does not work for near-parabolic orbits ( 0.8 < eccentricity < 1.2 ).
     * \return Hyperbolic eccentric anomaly.
     */
    double convert( );

protected:

private:

    //! Hyperbolic eccentric anomaly.
    /*!
     * Hyperbolic eccentric anomaly.
     */
    double hyperbolicEccentricAnomaly_;

    //! Pointer to adaptor NewtonRaphsonAdaptor.
    /*!
     * Pointer to adaptor NewtonRaphsonAdaptor class.
     */
    NewtonRaphsonAdaptor < ConvertMeanAnomalyToHyperbolicEccentricAnomaly > newtonRaphsonAdaptor_;

    //! Compute Kepler's function for hyperbolic orbits.
    /*!
     * Computes Kepler's function, given as:
     * \f[
     *      f( F ) = e * sinh( F ) - F - M
     * \f]
     * for hyperbolic orbits, where \f$ F \f$ is the hyperbolic eccentric anomaly, \f$ e \f$ is the
     * eccentricity, \f$ M \f$ is the mean anomaly. Currently, the case for near-parabolic orbits
     * is not included ( 0.8 < eccentricity < 1.2 ).
     * \param hyperbolicEccentricAnomaly Hyperbolic eccentric anomaly.
     * \return Value of Kepler's function for hyperbolic orbits.
     */
    double computeKeplersFunctionForHyperbolicOrbits_( double& hyperbolicEccentricAnomaly )
    {
        return eccentricity_ * sinh( hyperbolicEccentricAnomaly )
                - hyperbolicEccentricAnomaly - meanAnomaly_;
    }

    //! Compute first-derivative of Kepler's function for hyperbolic orbits.
    /*!
     * Computes the first-derivative of Kepler's function, given as:
     * \f[
     *      \frac{ df( F ) } { dF } = e * cosh( F ) - 1
     * \f]
     * for hyperbolic orbits, where \f$ F \f$ is the hyperbolic eccentric anomaly, and \f$ e \f$ is
     * the eccentricity. Currently, the case for near-parabolic orbits is not included
     * ( 0.8 < eccentricity < 1.2 ).
     * \param hyperbolicEccentricAnomaly Hyperbolic eccentric anomaly
     * \return Value of first-derivative of Kepler's function for hyperbolic orbits.
     */
    double computeFirstDerivativeKeplersFunctionForHyperbolicOrbits_(
        double& hyperbolicEccentricAnomaly )
    { return eccentricity_ * cosh( hyperbolicEccentricAnomaly ) - 1.0; }
};

} // namespace orbital_element_conversions
} // tudat

#endif // TUDAT_CONVERT_MEAN_ANOMALY_TO_HYPERBOLIC_ECCENTRIC_ANOMALY_H
