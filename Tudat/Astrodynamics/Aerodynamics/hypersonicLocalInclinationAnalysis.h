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
 *      102511    D. Dirkx          First version of file.
 *      110501    D. Dirkx          Added more comments.
 *      112701    D. Dirkx          Finalized for code check.
 *      110131    B. Romgens        Minor modifications during code check.
 *      110204    D. Dirkx          Finalized code.
 *      120912    D. Dirkx          Adjusted to meet RAII idiom. Implemented use of Boost
 *                                  (multi-)arrays where convenient.
 *      121009    A. Ronse          Adjusted inclination-determination to surface-outward normals.
 *                                  Limited inclination computations to 1 per aoa and aos pair.
 *                                  Streamlined initialization of isCoefficientGenerated_.
 *
 *    References
 *      Gentry, A., Smyth, D., and Oliver, W. The Mark IV Supersonic-Hypersonic Arbitrary Body
 *        Program, Volume II - Program Formulation, Douglas Aircraft Company, 1973.
 *      Anderson Jr. , J.D, Hypersonic and High-Temperature Gas Dynamics, 2nd edition,
 *        AIAA Education Series, 2006
 *
 *    Notes
 *
 */

#ifndef TUDAT_HYPERSONIC_LOCAL_INCLINATION_ANALYSIS_H
#define TUDAT_HYPERSONIC_LOCAL_INCLINATION_ANALYSIS_H

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <Eigen/Core>

#include <boost/array.hpp>
#include <boost/multi_array.hpp>

#include "Tudat/Astrodynamics/Aerodynamics/aerodynamicCoefficientGenerator.h"
#include "Tudat/Mathematics/BasicMathematics/linearAlgebraTypes.h"
#include "Tudat/Mathematics/GeometricShapes/lawgsPartGeometry.h"

namespace tudat
{
namespace aerodynamics
{

//! Returns default values of mach number for use in HypersonicLocalInclinationAnalysis.
/*!
 *  Returns default values of mach number for use in HypersonicLocalInclinationAnalysis.
 */
std::vector< double > getDefaultHypersonicLocalInclinationMachPoints(
        const std::string& machRegime );

//! Returns default values of angle of attack for use in HypersonicLocalInclinationAnalysis.
/*!
 *  Returns default values of angle of attack for use in HypersonicLocalInclinationAnalysis.
 */
std::vector< double > getDefaultHypersonicLocalInclinationAngleOfAttackPoints( );

//! Returns default values of angle of sideslip for use in HypersonicLocalInclinationAnalysis.
/*!
 *  Returns default values of angle of sideslip for use in HypersonicLocalInclinationAnalysis.
 */
std::vector< double > getDefaultHypersonicLocalInclinationAngleOfSideslipPoints( );

//! Class for inviscid hypersonic aerodynamic analysis using local inclination methods.
/*!
 * Class for inviscid hypersonic aerodynamic analysis using local inclination
 * methods. These methods assume that the local pressure on the vehicle is only
 * dependent on the local inclination angle w.r.t. the freestream flow and
 * freestream conditions, such as Mach number and ratio of specific heats.
 * All aerodynamic coefficients can be calculated using the generateCoefficients function, or on an
 * as needed basis by using the getAerodynamicCoefficients function. Note that during the
 * panel inclination determination process, a geometry with outward surface-normals is assumed.
 * The resulting coefficients are expressed in the same reference frame as that of the input
 * geometry.
 */
class HypersonicLocalInclinationAnalysis: public AerodynamicCoefficientGenerator< 3, 6 >
{
public:

    enum HypersonicLocalInclinationAnalysisIndependentVariables
    {
        mach_index = 0,
        angle_of_attack_index = 1,
        angle_of_sideslip_index = 2
    };

    //! Default constructor.
    /*!
     * Default constructor.
     */
    HypersonicLocalInclinationAnalysis(
            const std::vector< std::vector< double > >& dataPointsOfIndependentVariables,
            const boost::shared_ptr< SurfaceGeometry > inputVehicleSurface,
            const std::vector< int >& numberOfLines,
            const std::vector< int >& numberOfPoints,
            const std::vector< bool >& invertOrders,
            const std::vector< std::vector< int > >& selectedMethods,
            const double referenceArea,
            const double referenceLength,
            const Eigen::Vector3d& momentReferencePoint,
            const std::string& machRegime = "Full" );

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~HypersonicLocalInclinationAnalysis( ) { }

    //! Get aerodynamic coefficients.
    /*!
     * Returns aerodynamic coefficients.
     * \param independentVariables Array of values of independent variable
     *          indices in dataPointsOfIndependentVariables_.
     * \return vector of coefficients at specified independent variable indices.
     */
    basic_mathematics::Vector6d getAerodynamicCoefficients(
            const boost::array< int, 3 > independentVariables );

    //! Generate aerodynamic database.
    /*!
     * Generates aerodynamic database. Settings of geometry,
     * reference quantities, database point settings and analysis methods
     *  should have been set previously.
     */
    void generateCoefficients( );

    //! Determine inclination angles of panels on a given part.
    /*!
     * Determines panel inclinations for all panels on a given part for given attitude.
     * Outward pointing surface-normals are assumed!
     * \param partNumber Index from vehicleParts_ array for which to determine coefficients.
     * \param angleOfAttack Angle of attack at which to determine inclination angles.
     * \param angleOfSideslip Angle of sideslip at which to determine inclination angles.
     */
    void determineInclination( const int partNumber, const double angleOfAttack,
                               const double angleOfSideslip );

    //! Get the number of vehicle parts.
    /*!
     * Returns the number of vehicle parts.
     */
    int getNumberOfVehicleParts( ) const
    {
        return vehicleParts_.size( );
    }

    //! Get a vehicle part.
    /*!
     * Returns a vehicle part.
     * \param vehicleIndex Index in vehicleParts_ to be retrieved.
     * \return Requested vehicle part.
     */
     boost::shared_ptr< geometric_shapes::LawgsPartGeometry > getVehiclePart(
             const int vehicleIndex ) const
     {
         return vehicleParts_[ vehicleIndex ];
     }

    //! Get Mach regime.
    /*!
     * Returns Mach regime, see machRegime_.
     * \return Mach regime.
     */
    std::string getMachRegime( ) const
    {
        return machRegime_;
    }

    //! Overload ostream to print class information.
    /*!
     * Overloads ostream to print class information, prints the number of lawgs geometry parts and
     * names.
     * \param stream Stream object.
     * \param hypersonicLocalInclinationAnalysis Hypersonic local inclination analysis.
     * \return Stream object.
     */
    friend std::ostream& operator<<( std::ostream& stream,
                                     HypersonicLocalInclinationAnalysis&
                                     hypersonicLocalInclinationAnalysis );

private:

    //! Generate aerodynamic coefficients at a single set of independent variables.
    /*!
     * Generates aerodynamic coefficients at a single set of independent variables.
     * Determines values and sets corresponding entry in vehicleCoefficients_ array.
     * \param independentVariableIndices Array of indices from lists of Mach number,
     *          angle of attack and angle of sideslip points at which to perform analysis.
     */
    void determineVehicleCoefficients( const boost::array< int, 3 > independentVariableIndices );

    //! Determine aerodynamic coefficients for a single LaWGS part.
    /*!
     * Determines aerodynamic coefficients for a single LaWGS part,
     * calls determinepressureCoefficient_ function for given vehicle part.
     * \param partNumber Index from vehicleParts_ array for which to determine coefficients.
     * \param independentVariableIndices Array of indices of independent variables.
     */
    basic_mathematics::Vector6d determinePartCoefficients(
            const int partNumber, const boost::array< int, 3 > independentVariableIndices );

    //! Determine pressure coefficients on a given part.
    /*!
     * Determines pressure coefficients on a single vehicle part.
     * Calls the updateExpansionPressures and updateCompressionPressures for given vehicle part.
     * \param partNumber Index from vehicleParts_ array for which to determine coefficients.
     * \param independentVariableIndices Array of indices of independent variables.
     */
    void determinePressureCoefficients( const int partNumber,
                                        const boost::array< int, 3 > independentVariableIndices );

    //! Determine force coefficients of a part.
    /*!
     * Sums the pressure coefficients of given part and determines force coefficients from it by
     * non-dimensionalization with reference area.
     * \param partNumber Index from vehicleParts_ array for which determine coefficients.
     */
    Eigen::Vector3d calculateForceCoefficients( const int partNumber );

    //! Determine moment coefficients of a part.
    /*!
     * Determines the moment coefficients of a given part by summing the contributions of all
     * panels on the part. Moment arms are taken from panel centroid to momentReferencePoint. Non-
     * dimensionalization is performed by product of referenceLength and referenceArea.
     * \param partNumber Index from vehicleParts_ array for which to determine coefficients.
     */
    Eigen::Vector3d calculateMomentCoefficients( const int partNumber );

    //! Determine the compression pressure coefficients of a given part.
    /*!
     * Sets the values of pressureCoefficient_ on given part and at given Mach number for which
     * inclination > 0.
     * \param machNumber Mach number at which to perform analysis.
     * \param partNumber of part from vehicleParts_ which is to be analyzed.
     */
    void updateCompressionPressures( const double machNumber, const int partNumber );

    //! Determine the expansion pressure coefficients of a given part.
    /*!
     * Determine the values of pressureCoefficient_ on given part and at given Mach number for
     * which inclination <= 0.
     * \param machNumber Mach number at which to perform analysis.
     * \param partNumber of part from vehicleParts_ which is to be analyzed.
     */
    void updateExpansionPressures( const double machNumber, const int partNumber );

    //! Array of vehicle parts.
    /*!
     * Array of vehicle parts.
     */
    std::vector< boost::shared_ptr< geometric_shapes::LawgsPartGeometry > > vehicleParts_;

    //! Multi-array as which indicates which coefficients have been calculated already.
    /*!
     * Multi-array as which indicates which coefficients have been calculated already. Indices of
     * entries coincide with indices of aerodynamicCoefficients_.
     */
    boost::multi_array< bool, 3 > isCoefficientGenerated_;

    //! Three-dimensional array of panel inclination angles.
    /*!
     * Three-dimensional array of panel inclination angles at current values of
     * independent variables. Indices indicate part-line-point.
     */
    std::vector< std::vector< std::vector< double > > > inclination_;

    //! Map of angle of attack and -sideslip pair and associated panel inclinations.
    /*!
     * Map of angle of attack and -sideslip pair and associated panel inclinations.
     */
    std::map< std::pair< double, double >, std::vector< std::vector< std::vector< double > > > >
            previouslyComputedInclinations_;

    //! Three-dimensional array of panel pressure coefficients.
    /*!
     * Three-dimensional array of panel pressure coefficients at current values
     * of independent variables. Indices indicate part-line-point.
     */
    std::vector< std::vector< std::vector< double > > > pressureCoefficient_;

    //! Stagnation pressure coefficient.
    /*!
     * Stagnation pressure coefficient for flow which has passed through a
     * normal shock wave at current Mach number.
     */
    double stagnationPressureCoefficient;

    //! Ratio of specific heats.
    /*!
     * Ratio of specific heat at constant pressure to specific heat at constant pressure.
     */
    double ratioOfSpecificHeats;
\
    //! Array of selected methods.
    /*!
     * Array of selected methods, first index represents compression/expansion,
     * second index represents vehicle part.
     */
    std::vector< std::vector< int > > selectedMethods_;

    //! Mach regime.
    /*!
     * Mach regime, permissible values are "Full", "High" or "Low", default is "Full".
     */
    std::string machRegime_;
};

} // namespace aerodynamics
} // namespace tudat

#endif // TUDAT_HYPERSONIC_LOCAL_INCLINATION_ANALYSIS_H
