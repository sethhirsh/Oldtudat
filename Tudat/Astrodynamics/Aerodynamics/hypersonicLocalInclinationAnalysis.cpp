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
 *      Gentry, A., Smyth, D., and Oliver, W. . The Mark IV Supersonic-Hypersonic Arbitrary Body
 *        Program, Volume II - Program Formulation, Douglas Aircraft Aircraft Company, 1973.
 *
 *    Notes
 *
 */

#include <string>

#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/make_shared.hpp>
#include <boost/pointer_cast.hpp>
#include <boost/shared_ptr.hpp>

#include <Eigen/Geometry>

#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/Aerodynamics/aerodynamics.h"
#include "Tudat/Astrodynamics/Aerodynamics/hypersonicLocalInclinationAnalysis.h"
#include "Tudat/Mathematics/GeometricShapes/compositeSurfaceGeometry.h"
#include "Tudat/Mathematics/GeometricShapes/surfaceGeometry.h"

namespace tudat
{
namespace aerodynamics
{

using std::string;
using std::endl;
using tudat::mathematics::PI;

using namespace tudat::mathematics::geometric_shapes;

//! Returns default values of mach number for use in HypersonicLocalInclinationAnalysis.
std::vector< double > getDefaultHypersonicLocalInclinationMachPoints(
        const std::string& machRegime )
{
    std::vector< double > machPoints;

    // Set default points for full hypersonic analysis.
    if ( machRegime == "Full" )
    {
        machPoints.resize( 6 );

        machPoints[ 0 ] = 3.0;
        machPoints[ 1 ] = 4.0;
        machPoints[ 2 ] = 5.0;
        machPoints[ 3 ] = 8.0;
        machPoints[ 4 ] = 10.0;
        machPoints[ 5 ] = 20.0;
    }

    // Set default points for low hypersonic analysis.
    else if ( machRegime == "Low" )
    {
        machPoints.resize( 5 );
        machPoints[ 0 ] = 3.0;
        machPoints[ 1 ] = 4.0;
        machPoints[ 2 ] = 5.0;
        machPoints[ 3 ] = 8.0;
        machPoints[ 4 ] = 10.0;
    }

    // Set default points for high hypersonic analysis.
    else if ( machRegime == "High" )
    {
        machPoints.resize( 4 );
        machPoints[ 0 ] = 5.0;
        machPoints[ 1 ] = 8.0;
        machPoints[ 2 ] = 10.0;
        machPoints[ 3 ] = 20.0;
    }
    return machPoints;
}

//! Returns default values of angle of attack for use in HypersonicLocalInclinationAnalysis.
std::vector< double > getDefaultHypersonicLocalInclinationAngleOfAttackPoints( )
{
    std::vector< double > angleOfAttackPoints;

    // Set number of data points and allocate memory.
    angleOfAttackPoints.resize( 11 );

    // Set default values, 0 to 40 degrees, with steps of 5 degrees.
    for ( int i = 0; i < 11; i++ )
    {
        angleOfAttackPoints[ i ] =
                ( static_cast< double >( i ) * 5.0 * PI / 180.0 );
    }
    return angleOfAttackPoints;
}

//! Returns default values of angle of sideslip for use in HypersonicLocalInclinationAnalysis.
std::vector< double > getDefaultHypersonicLocalInclinationAngleOfSideslipPoints( )
{
    std::vector< double > angleOfSideslipPoints;

    // Set number of data points and allocate memory.
    angleOfSideslipPoints.resize( 2 );

    // Set default values, 0 and 1 degrees.
    angleOfSideslipPoints[ 0 ] = 0.0;
    angleOfSideslipPoints[ 1 ] = 1.0 * PI / 180.0;

    return angleOfSideslipPoints;
}

//! Default constructor.
HypersonicLocalInclinationAnalysis::HypersonicLocalInclinationAnalysis(
        const std::vector< std::vector< double > >& dataPointsOfIndependentVariables,
        const boost::shared_ptr< SurfaceGeometry > inputVehicleSurface,
        const std::vector< int >& numberOfLines,
        const std::vector< int >& numberOfPoints,
        const std::vector< bool >& invertOrders,
        const std::vector< std::vector< int > >& selectedMethods,
        const double referenceArea,
        const double referenceLength,
        const Eigen::Vector3d& momentReferencePoint,
        const std::string& machRegime )
    : AerodynamicCoefficientGenerator< 3, 6 >(
          dataPointsOfIndependentVariables, referenceArea, referenceLength, momentReferencePoint ),
      stagnationPressureCoefficient( 2.0 ),
      ratioOfSpecificHeats( 1.4 ),      // Refer to a constant in "constants file" in the future!
      selectedMethods_( selectedMethods ),
      machRegime_( machRegime )
{
    // Set geometry if it is a single surface.
    if ( boost::dynamic_pointer_cast< SingleSurfaceGeometry > ( inputVehicleSurface ) !=
         boost::shared_ptr< SingleSurfaceGeometry >( ) )
    {
        // Set number of geometries and allocate memory.
        vehicleParts_.resize( 1 );

        vehicleParts_[ 0 ] = boost::make_shared< LawgsPartGeometry >( );
        vehicleParts_[ 0 ]->setReversalOperator( invertOrders[ 0 ] );

        // If part is not already a LaWGS part, convert it.
        if ( boost::dynamic_pointer_cast< LawgsPartGeometry >
             ( inputVehicleSurface ) ==
             boost::shared_ptr< LawgsPartGeometry >( ) )
        {
            // Convert geometry to LaWGS surface mesh and set in vehicleParts_ list.
            vehicleParts_[ 0 ]->setMesh(
                    boost::dynamic_pointer_cast< SingleSurfaceGeometry > ( inputVehicleSurface ),
                    numberOfLines[ 0 ], numberOfPoints[ 0 ] );
        }

        // Else, set geometry directly.
        else
        {
            vehicleParts_[ 0 ] = boost::dynamic_pointer_cast< LawgsPartGeometry >
                                 ( inputVehicleSurface );
        }
    }

    // Set geometry if it is a composite surface.
    else if ( boost::dynamic_pointer_cast< CompositeSurfaceGeometry >( inputVehicleSurface ) !=
              boost::shared_ptr< CompositeSurfaceGeometry >( ) )
    {
        // Dynamic cast to composite surface geometry for further processing.
        boost::shared_ptr< CompositeSurfaceGeometry > compositeSurfaceGeometry_ =
                boost::dynamic_pointer_cast< CompositeSurfaceGeometry >( inputVehicleSurface );

        // Set number of geometries and allocate memory.
        int numberOfVehicleParts =
                compositeSurfaceGeometry_->getNumberOfSingleSurfaceGeometries( );
        vehicleParts_.resize( numberOfVehicleParts );

        // Iterate through all parts and set them in vehicleParts_ list.
        for ( int i = 0; i < numberOfVehicleParts; i++ )
        {
            // If part is not already a LaWGS part, convert it.
            if ( boost::dynamic_pointer_cast< LawgsPartGeometry >
                 ( compositeSurfaceGeometry_->getSingleSurfaceGeometry( i ) ) ==
                 boost::shared_ptr< LawgsPartGeometry >( ) )
            {
                vehicleParts_[ i ] = boost::make_shared< LawgsPartGeometry >( );
                vehicleParts_[ i ]->setReversalOperator( invertOrders[ i ] );

                // Convert geometry to LaWGS and set in list.
                vehicleParts_[ i ]->setMesh(
                            compositeSurfaceGeometry_->getSingleSurfaceGeometry( i ),
                            numberOfLines[ i ], numberOfPoints[ i ] );
            }

            // Else, set geometry directly.
            else
            {
                vehicleParts_[ i ] = boost::dynamic_pointer_cast< LawgsPartGeometry >(
                            compositeSurfaceGeometry_->getSingleSurfaceGeometry( i ) );
            }
        }
    }

    // Allocate memory for panel inclinations and pressureCoefficient_.
    inclination_.resize( vehicleParts_.size( ) );
    pressureCoefficient_.resize( vehicleParts_.size( ) );
    for ( unsigned int i = 0 ; i < vehicleParts_.size( ); i++ )
    {
        inclination_[ i ].resize( vehicleParts_[ i ]->getNumberOfLines( ) );
        pressureCoefficient_[ i ].resize( vehicleParts_[ i ]->getNumberOfLines( ) );
        for ( int j = 0 ; j < vehicleParts_[ i ]->getNumberOfLines( ) ; j++ )
        {
            inclination_[ i ][ j ].resize( vehicleParts_[ i ]->getNumberOfPoints( ) );
            pressureCoefficient_[ i ][ j ].resize( vehicleParts_[ i ]->getNumberOfPoints( ) );
        }
    }

    boost::array< int, 3 > numberOfPointsPerIndependentVariables;
    for( int i = 0; i < 3; i++ )
    {
        numberOfPointsPerIndependentVariables[ i ] =
                dataPointsOfIndependentVariables_[ i ].size( );
    }
    isCoefficientGenerated_.resize( numberOfPointsPerIndependentVariables );

    std::fill( isCoefficientGenerated_.origin( ),
               isCoefficientGenerated_.origin( ) + isCoefficientGenerated_.num_elements( ), 0 );
}

//! Get aerodynamic coefficients.
Eigen::Matrix< double, 6, 1 > HypersonicLocalInclinationAnalysis::getAerodynamicCoefficients(
        const boost::array< int, 3 > independentVariables )
{
    if( isCoefficientGenerated_( independentVariables ) == 0 )
    {
        determineVehicleCoefficients( independentVariables );
    }
    // Return requested coefficients.
    return aerodynamicCoefficients_( independentVariables );

}

//! Generate aerodynamic database.
void HypersonicLocalInclinationAnalysis::generateCoefficients( )
{
    // Allocate variable to pass to coefficient determination for independent
    // variable indices.
    boost::array< int, 3 > independentVariableIndices;

    // Iterate over all combinations of independent variables.
    for ( unsigned int i = 0 ; i < dataPointsOfIndependentVariables_[ mach_index ].size( ) ; i++ )
    {
        independentVariableIndices[ mach_index ] = i;
        for ( unsigned  int j = 0 ; j < dataPointsOfIndependentVariables_[
              angle_of_attack_index ].size( ) ; j++ )
        {
            independentVariableIndices[ angle_of_attack_index ] = j;
            for ( unsigned  int k = 0 ; k < dataPointsOfIndependentVariables_[
                  angle_of_sideslip_index ].size( ) ; k++ )
            {
                independentVariableIndices[ angle_of_sideslip_index ] = k;

                determineVehicleCoefficients( independentVariableIndices );
            }
        }
    }
}

//! Generate aerodynamic coefficients at a single set of independent variables.
void HypersonicLocalInclinationAnalysis::determineVehicleCoefficients(
        const boost::array< int, 3 > independentVariableIndices )
{
    // Declare coefficients vector and initialize to zeros.
    Eigen::Matrix< double, 6, 1 > coefficients = Eigen::Matrix< double, 6, 1 >::Zero( );

    // Loop over all vehicle parts, calculate aerodynamic coefficients and add
    // to aerodynamicCoefficients_.
    for ( unsigned int i = 0 ; i < vehicleParts_.size( ) ; i++ )
    {
        coefficients += determinePartCoefficients( i, independentVariableIndices );
    }

    aerodynamicCoefficients_( independentVariableIndices ) = coefficients;
    isCoefficientGenerated_( independentVariableIndices ) = 1;
}

//! Determine aerodynamic coefficients of a single vehicle part.
Eigen::Matrix< double, 6, 1 > HypersonicLocalInclinationAnalysis::determinePartCoefficients(
        const int partNumber, const boost::array< int, 3 > independentVariableIndices )
{
    // Declare and determine angles of attack and sideslip for analysis.
    double angleOfAttack =  dataPointsOfIndependentVariables_[ angle_of_attack_index ]
            [ independentVariableIndices[ angle_of_attack_index ] ];

    double angleOfSideslip =  dataPointsOfIndependentVariables_[ angle_of_sideslip_index ]
            [ independentVariableIndices[ angle_of_sideslip_index ] ];

    // Declare partCoefficient vector.
    Eigen::Matrix< double, 6, 1 > partCoefficients = Eigen::Matrix< double, 6, 1 >::Zero( );

    // Check whether the inclinations of the vehicle part have already been computed.
    if ( previouslyComputedInclinations_.count(  std::pair< double, double >(
            angleOfAttack, angleOfSideslip ) ) == 0 )
    {
        // Determine panel inclinations for part.
        determineInclination( partNumber, angleOfAttack, angleOfSideslip );

        // Add panel inclinations to container
        previouslyComputedInclinations_[ std::pair< double, double >(
                        angleOfAttack, angleOfSideslip ) ] = inclination_;
    }

    else
    {
        // Fetch inclinations from container
        inclination_ = previouslyComputedInclinations_[ std::pair< double, double >(
                angleOfAttack, angleOfSideslip ) ];
    }

    // Set pressureCoefficient_ array for given independent variables.
    determinePressureCoefficients( partNumber, independentVariableIndices );

    // Calculate force coefficients from pressure coefficients.
    partCoefficients.segment( 0, 3 ) = calculateForceCoefficients( partNumber );

    // Calculate moment coefficients from pressure coefficients.
    partCoefficients.segment( 3, 3 ) = calculateMomentCoefficients( partNumber );

    return partCoefficients;
}

//! Determine the pressure coefficients on a single vehicle part.
void HypersonicLocalInclinationAnalysis::determinePressureCoefficients(
        const int partNumber, const boost::array< int, 3 > independentVariableIndices )
{
    // Retrieve Mach number.
    double machNumber = dataPointsOfIndependentVariables_[ mach_index ]
            [ independentVariableIndices[ mach_index ] ];

    // Determine stagnation point pressure coefficients. Value is computed once
    // here to prevent its calculation in inner loop.
    stagnationPressureCoefficient = computeStagnationPressure(
                machNumber, ratioOfSpecificHeats );

    updateCompressionPressures( machNumber, partNumber );
    updateExpansionPressures( machNumber, partNumber );
}

//! Determine force coefficients from pressure coefficients.
Eigen::Vector3d HypersonicLocalInclinationAnalysis::calculateForceCoefficients(
        const int partNumber )
{
    // Declare force coefficient vector and intialize to zeros.
    Eigen::Vector3d forceCoefficients = Eigen::Vector3d::Zero( );
    
    // Loop over all panels and add pressures, scaled by panel area, to force
    // coefficients.
    for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1 ; i++ )
    {
        for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++)
        {
            forceCoefficients -=
                    pressureCoefficient_[ partNumber ][ i ][ j ] *
                    vehicleParts_[ partNumber ]->getPanelArea( i, j ) *
                    vehicleParts_[ partNumber ]->getPanelSurfaceNormal( i, j );
        }
    }

    // Normalize result by reference area.
    forceCoefficients /= referenceArea_;

    return forceCoefficients;
}

//! Determine moment coefficients from pressure coefficients.
Eigen::Vector3d HypersonicLocalInclinationAnalysis::calculateMomentCoefficients(
        const int partNumber )
{
    // Declare moment coefficient vector and intialize to zeros.
    Eigen::Vector3d momentCoefficients = Eigen::Vector3d::Zero( );

    // Declare moment arm for panel moment determination.
    Eigen::Vector3d referenceDistance ;

    // Loop over all panels and add moments due pressures.
    for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1 ; i++ )
    {
        for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++ )
        {
            // Determine moment arm for given panel centroid.
            referenceDistance = ( vehicleParts_[ partNumber ]->
                                  getPanelCentroid( i, j ) -  momentReferencePoint_ );

            momentCoefficients -=
                    pressureCoefficient_[ partNumber ][ i ][ j ] *
                    vehicleParts_[ partNumber ]->getPanelArea( i, j ) *
                    ( referenceDistance.cross( vehicleParts_[ partNumber ]->
                                               getPanelSurfaceNormal( i, j ) ) );
        }
    }

    // Scale result by reference length and area.
    momentCoefficients /= ( referenceLength_ * referenceArea_ );

    return momentCoefficients;
}

//! Determines the inclination angle of panels on a single part.
void HypersonicLocalInclinationAnalysis::determineInclination( const int partNumber,
                                                               const double angleOfAttack,
                                                               const double angleOfSideslip )
{
    // Declare free-stream velocity vector.
    Eigen::Vector3d freestreamVelocityDirection;

    // Set freestream velocity vector in body frame.
    double freestreamVelocityDirectionX = cos( angleOfAttack )* cos( angleOfSideslip );
    double freestreamVelocityDirectionY = sin( angleOfSideslip );
    double freestreamVelocityDirectionZ = sin( angleOfAttack ) * cos( angleOfSideslip );
    freestreamVelocityDirection( 0 ) = freestreamVelocityDirectionX;
    freestreamVelocityDirection( 1 ) = freestreamVelocityDirectionY;
    freestreamVelocityDirection( 2 ) = freestreamVelocityDirectionZ;

    // Declare cosine of inclination angle.
    double cosineOfInclination;

    // Loop over all panels of given vehicle part and set inclination angles.
    for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1 ; i++ )
    {
        for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++ )
        {

            // Determine cosine of inclination angle from inner product between
            // surface normal and free-stream direction.
            cosineOfInclination = vehicleParts_[ partNumber ]->
                    getPanelSurfaceNormal( i, j ).
                    dot( freestreamVelocityDirection );

            // Set inclination angle.
            inclination_[ partNumber ][ i ][ j ] = PI / 2.0 - acos( cosineOfInclination );
        }
    }
}

//! Determine compression pressure coefficients on all parts.
void HypersonicLocalInclinationAnalysis::updateCompressionPressures( const double machNumber,
                                                                     const int partNumber )
{ 
    int method = selectedMethods_[ 0 ][ partNumber ];

    boost::function< double( double ) > pressureFunction;

    // Switch to analyze part using correct method.
    switch( method )
    {
    case 0:
        pressureFunction =
                boost::bind( aerodynamics::computeNewtonianPressureCoefficient, _1 );
        break;

    case 1:
        pressureFunction =
                boost::bind( aerodynamics::computeModifiedNewtonianPressureCoefficient, _1,
                                        stagnationPressureCoefficient );
        break;

    case 2:
        // Method currently disabled.
        break;

    case 3:
        // Method currently disabled.
        break;

    case 4:
        pressureFunction =
                boost::bind( aerodynamics::computeEmpiricalTangentWedgePressureCoefficient, _1,
                                        machNumber );
        break;

    case 5:
        pressureFunction =
                boost::bind( aerodynamics::computeEmpiricalTangentConePressureCoefficient, _1,
                                        machNumber );
        break;

    case 6:
        pressureFunction =
                boost::bind( aerodynamics::computeModifiedDahlemBuckPressureCoefficient, _1,
                                        machNumber );
        break;

    case 7:
        pressureFunction =
                boost::bind( aerodynamics::computeVanDykeUnifiedPressureCoefficient, _1,
                                        machNumber, ratioOfSpecificHeats, 1 );
        break;

    case 8:
        pressureFunction =
                boost::bind( aerodynamics::computeSmythDeltaWingPressureCoefficient, _1,
                                        machNumber );
        break;

    case 9:
        pressureFunction =
                boost::bind( aerodynamics::computeHankeyFlatSurfacePressureCoefficient, _1,
                                        machNumber );
        break;

    default:
        break;
    }

    for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1; i++ )
    {
        for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++ )
        {
            if ( inclination_[ partNumber ][ i ][ j ] > 0 )
            {
                // If panel inclination is positive, calculate pressure coefficient.
                pressureCoefficient_[ partNumber ][ i ][ j ] =
                        pressureFunction( inclination_[ partNumber ][ i ][ j ] );
            }
        }
    }
}

//! Determines expansion pressure coefficients on all parts.
void HypersonicLocalInclinationAnalysis::updateExpansionPressures( const double machNumber,
                                                                   const int partNumber )
{
    // Get analysis method of part to analyze.
    int method = selectedMethods_[ 1 ][ partNumber ];

    if ( method == 0 || method == 1 || method == 4 )
    {
        boost::function< double( ) > pressureFunction;
        switch( method )
        {
        case 0:
            pressureFunction = boost::bind( &aerodynamics::computeVacuumPressureCoefficient,
                                            machNumber, ratioOfSpecificHeats );
            break;

        case 1:
            pressureFunction = boost::lambda::constant( 0.0 );
            break;

        case 4:
            pressureFunction = boost::bind( &aerodynamics::computeHighMachBasePressure,
                                            machNumber );
            break;

        }

        // Iterate over all panels on part.
        for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1 ; i++ )
        {
            for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++ )
            {
                if ( inclination_[ partNumber ][ i ][ j ] <= 0 )
                {
                    // If panel inclination is negative, calculate pressure using
                    // Van Dyke unified method.
                    pressureCoefficient_[ partNumber ][ i ][ j ] =
                            pressureFunction( );
                }
            }
        }
    }

    else if( method == 3 || method == 5 || method == 6 )
    {

        boost::function< double( double ) > pressureFunction;

        // Declare local variable.
        double freestreamPrandtlMeyerFunction;

        // Switch to analyze part using correct method.
        switch( method )
        {
        case 3:
            // Calculate freestream Prandtl-Meyer function.
            freestreamPrandtlMeyerFunction = aerodynamics::computePrandtlMeyerFunction(
                        machNumber, ratioOfSpecificHeats );
            pressureFunction =
                    boost::bind( &aerodynamics::computePrandtlMeyerFreestreamPressureCoefficient,
                                            _1, machNumber, ratioOfSpecificHeats,
                                            freestreamPrandtlMeyerFunction );
            break;

        case 5:
            pressureFunction =
                    boost::bind( &aerodynamics::computePrandtlMeyerFreestreamPressureCoefficient,
                                            _1, machNumber, ratioOfSpecificHeats, -1 );
            break;

        case 6:
            pressureFunction = boost::bind( &aerodynamics::computeAcmEmpiricalPressureCoefficient,
                                            _1, machNumber );
            break;
        }

        // Iterate over all panels on part.
        for ( int i = 0 ; i < vehicleParts_[ partNumber ]->getNumberOfLines( ) - 1 ; i++ )
        {
            for ( int j = 0 ; j < vehicleParts_[ partNumber ]->getNumberOfPoints( ) - 1 ; j++ )
            {
                if ( inclination_[ partNumber ][ i ][ j ] <= 0 )
                {
                    // If panel inclination is negative, calculate pressure using
                    // Van Dyke unified method.
                    pressureCoefficient_[ partNumber ][ i ][ j ] =
                            pressureFunction( inclination_[ partNumber ][ i ][ j ] );
                }
            }
        }
    }

    else
    {
        std::cerr << "Error, expansion local inclination method number "<< method <<
                " not recognized" << std::endl;
    }
}

//! Overload ostream to print class information.
std::ostream& operator<<( std::ostream& stream,
                          HypersonicLocalInclinationAnalysis& hypersonicLocalInclinationAnalysis )
{
    stream << "This is a hypersonic local inclination analysis object."<< endl;
    stream << "The Mach regime is "
           << hypersonicLocalInclinationAnalysis.getMachRegime( ) << endl;
    stream << "It contains "
           << hypersonicLocalInclinationAnalysis.getNumberOfVehicleParts( )
           << " parts in Lawgs format. " << endl;
    stream << "The names of the vehicle parts are ";

    for ( int i = 0; i < hypersonicLocalInclinationAnalysis.getNumberOfVehicleParts( ); i++ )
    {
        stream << hypersonicLocalInclinationAnalysis.getVehiclePart( i )->getName( ) << ", ";
    }

    stream << endl;

    // Return stream.
    return stream;
}

} // namespace aerodynamics
} // namespace tudat
