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
 *      102511    D. Dirkx          First version of file.
 *      110501    D. Dirkx          Added more comments.
 *      110203    L. Abdulkadir     Code check.
 *      110208    D. Dirkx          Fixed shock temperature ratio.
 *      110210    L. Abdulkadir     Code check.
 *      110211    K. Kumar          Corrected Doxygen errors; corrected layout errors; corrected
 *                                  function-naming; optimized code; corrected double precision.
 *
 *    References
 *      Anderson Jr., J.D. , Fundamentals of Aerodynamics, 3rd edition, McGraw Hill, 2001.
 *      Gentry, A., Smyth, D., and Oliver, W. . The Mark IV Supersonic-Hypersonic Arbitrary Body
 *          Program, Volume II - Program Formulation, Douglas Aircraft Company, 1973.
 *      Anderson Jr., J.D, Hypersonic and High-Temperature Gas Dynamics, 2nd edition, AIAA
 *          Education Series, 2006.
 *
 *    Notes
 *
 */

#include <TudatCore/Mathematics/BasicMathematics/mathematicalConstants.h>

#include "Tudat/Astrodynamics/Aerodynamics/aerodynamics.h"

namespace tudat
{
namespace aerodynamics
{

using tudat::basic_mathematics::mathematical_constants::PI;
using std::atan;
using std::exp;
using std::log;
using std::pow;
using std::sqrt;
using std::tan;

//! Compute local-to-static pressure ratio.
double computeLocalToStaticPressureRatio( double machNumber,
                                          double ratioOfSpecificHeats )
{
    // Return local-to-static pressure ratio.
    return pow( 2.0 / ( 2.0 + ( ratioOfSpecificHeats - 1.0 ) * pow( machNumber, 2.0 ) ),
                ratioOfSpecificHeats / ( ratioOfSpecificHeats - 1.0 ) );
}

//! Compute Prandtl-Meyer function.
double computePrandtlMeyerFunction( double machNumber, double ratioOfSpecificHeats )
{
    // Declare local variables.
    // Declare Mach number squared.
    double machNumberSquared_ = pow( machNumber, 2.0 );

    // Return value of Prandtl-Meyer function.
    return sqrt ( ( ratioOfSpecificHeats + 1.0 ) / ( ratioOfSpecificHeats - 1.0 ) )
            * atan ( sqrt ( ( ratioOfSpecificHeats - 1.0 ) / ( ratioOfSpecificHeats + 1.0 )
                            * ( machNumberSquared_ - 1.0 ) ) )
            - atan( sqrt ( machNumberSquared_ - 1.0 ) );
}

//! Compute stagnation pressure coefficient in supersonic flow.
double computeStagnationPressure( double machNumber,
                                  double ratioOfSpecificHeats )
{
    // Declare local variables.
    // Declare Mach number squared.
    double machNumberSquared_ = pow( machNumber, 2.0 );

    // Return stagnation pressure coefficient.
    return 2.0 / ( ratioOfSpecificHeats * machNumberSquared_ )
            * ( pow ( pow( ( ratioOfSpecificHeats + 1.0 ) * machNumber, 2.0 )
                      / ( 4.0 * ratioOfSpecificHeats * machNumberSquared_
                          - 2.0 * ( ratioOfSpecificHeats - 1.0 ) ),
                      ratioOfSpecificHeats / ( ratioOfSpecificHeats - 1.0 ) )
                * ( ( 1.0 - ratioOfSpecificHeats
                      + 2.0 * ratioOfSpecificHeats * machNumberSquared_ )
                    / ( ratioOfSpecificHeats + 1.0 ) ) - 1.0 );
}

//! Compute pressure coefficient based on Newtonian theory.
double computeNewtonianPressureCoefficient( double inclinationAngle )
{
    // Return pressure coefficient.
    return 2.0 * pow( sin( inclinationAngle ), 2.0 );
}

//! Compute pressure coefficient based on modified Newtonian theory.
double computeModifiedNewtonianPressureCoefficient(
    double inclinationAngle, double stagnationPressureCoefficient )
{
    // Return pressure coefficient.
    return stagnationPressureCoefficient * pow( sin( inclinationAngle ), 2.0 );
}

//! Compute pressure coefficient using empirical tangent wedge method.
double computeEmpiricalTangentWedgePressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variable.
    double machNumberSine_;

    // Set local variable.
    machNumberSine_ = machNumber * sin( inclinationAngle );

    // Return pressure coefficient approximation.
    return ( pow( 1.2 * machNumberSine_ + exp( -0.6 * machNumberSine_ ), 2.0 )
             - 1.0 ) / ( 0.6 * pow( machNumber, 2.0 ) );
}

//! Compute pressure coefficient using empirical tangent cone method.
double computeEmpiricalTangentConePressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variables.
    double machNumberSine_;
    double temporaryValue_;

    // Set local variables.
    machNumberSine_ = machNumber * sin ( inclinationAngle );
    temporaryValue_ = pow( ( 1.090909 * machNumberSine_
                             +  exp( -0.5454545 * machNumberSine_ ) ), 2.0 );

    // Return pressure coefficient approximation.
    return ( 48.0 * temporaryValue_ * pow( sin( inclinationAngle), 2.0 ) )
            / ( 23.0 * temporaryValue_ - 5.0 );
}

//! Compute pressure coefficient using modified Dahlem-Buck method.
double computeModifiedDahlemBuckPressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variables.
    double checkAngle_ = 22.5 * PI / 180.0;
    double factor1_;
    double factor2_;
    double exponent_;
    double pressureCoefficient_;

    // Check if inclination angle is greater than check angle. If so, use
    // Newtonian approximation.
    if ( inclinationAngle > checkAngle_ )
    {
        pressureCoefficient_
                = computeNewtonianPressureCoefficient( inclinationAngle );
    }

    // Else use Dahlem-Buck method.
    else
    {
        pressureCoefficient_
                = ( 1.0 + sin( 4.0 * pow( inclinationAngle , 0.75 ) ) )
                / ( pow( 4.0 * cos( inclinationAngle )
                         * cos( 2.0 * inclinationAngle ), 0.75 ) )
                * pow( sin( inclinationAngle ), 1.25 );
    }

    // For mach < 20, a correction term should be applied.
    if ( machNumber > 20.0 )
    {
        factor2_ = 1.0;
    }
    else
    {
        // Determine correction term.
        factor1_ = ( 6.0 - 0.3 * machNumber ) + sin( PI * ( log( machNumber ) - 0.588 ) / 1.20 );

        exponent_ = 1.15 + 0.5 * sin( PI * ( log( machNumber ) - 0.916 ) / 3.29 );

        factor2_ = 1.0 + factor1_ * pow( inclinationAngle * 180.0 / PI, -1.0 * exponent_ );
    }

    // Return pressure coefficient.
    return pressureCoefficient_ * factor2_;
}

//! Compute pressure coefficient using the Hankey flat surface method.
double computeHankeyFlatSurfacePressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variables.
    double stagnationPressureCoefficient_;

    // Calculate 'effective' stagnation pressure coefficient for low
    // inclination angle.
    if( inclinationAngle < PI / 18.0 )
    {
        stagnationPressureCoefficient_ = ( 0.195 + 0.222594 / pow( machNumber, 0.3 ) - 0.4 )
                * inclinationAngle * 180.0 / PI + 4.0;
    }
    // Calculate 'effective' stagnation pressure coefficient for other
    // inclination angle.
    else
    {
        stagnationPressureCoefficient_ = 1.95 + 0.3925 / ( pow( machNumber, 0.3 )
                                                           * tan( inclinationAngle ) );
    }

    // Return pressure coefficient using 'effective' stagnation pressure
    // coefficient.
    return computeModifiedNewtonianPressureCoefficient(
                inclinationAngle, stagnationPressureCoefficient_ );
}

//! Compute pressure coefficient using the Smyth delta wing method.
double computeSmythDeltaWingPressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variables.
    double machNumberSine_;
    double correctedInclinationAngle_;

    // Calculate inclination angle for use in calculations ( angles lower than
    // 1 degree not allowed ).
    if ( inclinationAngle < PI / 180.0 )
    {
        correctedInclinationAngle_ = PI / 180.0;
    }

    else
    {
        correctedInclinationAngle_ = inclinationAngle;
    }

    // Pre-compute for efficiency.
    machNumberSine_ = machNumber * sin( correctedInclinationAngle_ );

    // Employ empirical correlation to calculate pressure coefficient.
    // Return pressure coefficient.
    return 1.66667 * ( pow( 1.09 * machNumberSine_ + exp( -0.49 * machNumberSine_ ), 2.0 ) - 1.0 )
            / pow( machNumber, 2.0 );
}

//! Compute pressure coefficient using the van Dyke unified method.
double computeVanDykeUnifiedPressureCoefficient(
    double inclinationAngle, double machNumber,
    double ratioOfSpecificHeats, int type )
{
    // Declare and initialize local variables and pre-compute for efficiency.
    double ratioOfSpecificHeatsTerm_ = ( ratioOfSpecificHeats + 1.0 ) / 2.0;
    double machNumberTerm_ = sqrt( pow( machNumber , 2.0 ) - 1.0 );
    double exponent_ = 2.0 * ratioOfSpecificHeats / ( ratioOfSpecificHeats - 1.0 );

    // Declare and initialize value.
    double pressureCoefficient_ = 0.0;

    // Calculate compression pressure coefficient.
    if ( inclinationAngle >= 0.0 && type == 1 )
    {
        pressureCoefficient_ = pow( inclinationAngle, 2.0 )
                * ( ratioOfSpecificHeatsTerm_
                    + sqrt( pow( ratioOfSpecificHeatsTerm_, 2.0 )
                            + 4.0 / ( pow(  inclinationAngle * machNumberTerm_, 2.0 ) ) ) );
    }

    // Calculate expansion pressure coefficient.
    else if ( inclinationAngle < 0.0 && type == -1 )
    {
        // Calculate vacuum pressure coefficient.
        double vacuumPressureCoefficient_ = computeVacuumPressureCoefficient(
                    machNumber, ratioOfSpecificHeats );

        // Check to see if pressure coefficient will be lower than vacuum case,
        // set to vacuum if so.
        if ( -1.0 * inclinationAngle * machNumberTerm_
             > 2.0 / ( ratioOfSpecificHeats - 1.0 ) )
        {
            pressureCoefficient_ = vacuumPressureCoefficient_;
        }
        else
        {
            pressureCoefficient_
                    = 2.0 / ( ratioOfSpecificHeats * pow( machNumberTerm_, 2.0 ) )
                    * ( pow( 1.0 - ( ratioOfSpecificHeats - 1.0 ) / 2.0
                             * - 1.0 * inclinationAngle * machNumberTerm_, exponent_ ) - 1.0 );

            if ( pressureCoefficient_ < vacuumPressureCoefficient_ )
            {
                pressureCoefficient_ = vacuumPressureCoefficient_;
            }
        }
    }

    // Return pressure coefficient.
    return pressureCoefficient_;
}

//! Compute pressure coefficient using Prandtl-Meyer expansion.
double computePrandtlMeyerFreestreamPressureCoefficient(
    double inclinationAngle, double machNumber,
    double ratioOfSpecificHeats, double freestreamPrandtlMeyerFunction )
{
    // Declare local variables.
    double prandtlMeyerFunction_;
    double pressureCoefficient_;

    // Determine Prandtl-Meyer function value.
    prandtlMeyerFunction_ = freestreamPrandtlMeyerFunction - inclinationAngle;

    // If Prandtl-Meyer function is greater than the vacuum value, set vacuum
    // pressure coefficient.
    if ( prandtlMeyerFunction_ > maximumPrandtlMeyerFunctionValue )
    {
        pressureCoefficient_ = computeVacuumPressureCoefficient(
                    machNumber, ratioOfSpecificHeats );
    }

    else
    {
        // Determine local mach number.
        double localMachNumber_
                = computeInversePrandtlMeyerFunction( prandtlMeyerFunction_ );

        // Determine local to freestream pressure ratio.
        double pressureRatio_
                = computeLocalToStaticPressureRatio( localMachNumber_,
                                                     ratioOfSpecificHeats )
                / computeLocalToStaticPressureRatio( machNumber,
                                                     ratioOfSpecificHeats );

        // Form pressure coefficient.
        pressureCoefficient_ = 2.0 / ( ratioOfSpecificHeats * pow( machNumber, 2.0 ) )
                * ( pressureRatio_ - 1.0 );
    }

    // Return pressure coefficient.
    return pressureCoefficient_;
}

//! Compute pressure coefficient at vacuum.
double computeVacuumPressureCoefficient(
    double machNumber, double ratioOfSpecificHeats )
{
    // Return pressure coefficient.
    return -2.0 / ( ratioOfSpecificHeats * pow( machNumber, 2.0 ) );
}

//! Compute high Mach base pressure coefficient.
double computeHighMachBasePressure( double machNumber )
{
    // Calculate pressure coefficient.
    return -1.0 / pow( machNumber, 2.0 );
}

//! Compute pressure coefficient using the ACM empirical method.
double computeAcmEmpiricalPressureCoefficient(
    double inclinationAngle, double machNumber )
{
    // Declare local variables.
    double pressureCoefficient_;
    double minimumPressureCoefficient_;
    double preliminaryPressureCoefficient_;

    // Set minimum pressure coefficient.
    minimumPressureCoefficient_ = -1.0 / pow( machNumber, 2.0 );

    // Calculate preliminary pressure coefficient.
    preliminaryPressureCoefficient_ = 180.0 / PI * inclinationAngle
            / ( 16.0 * pow( machNumber, 2.0 ) );

    // If necessary, correct preliminary pressure coefficient.
    if ( minimumPressureCoefficient_ > preliminaryPressureCoefficient_ )
    {
        pressureCoefficient_ = minimumPressureCoefficient_;
    }

    else
    {
        pressureCoefficient_ = preliminaryPressureCoefficient_;
    }

    // Return pressure coefficient.
    return pressureCoefficient_;
}

//! Compute Mach number from Prandtl-Meyer function.
double computeInversePrandtlMeyerFunction( double prandtlMeyerFunctionValue )
{
    // Declare local variables.
    double inputVariableForCorrelation_;
    double machNumber_;

    // Determine input variable for correlation.
    inputVariableForCorrelation_ = pow( prandtlMeyerFunctionValue
                                        / maximumPrandtlMeyerFunctionValue, 2.0 / 3.0 );

    // Calculate Mach number.
    machNumber_ = ( 1.0 + inputVariableForCorrelation_
                    * ( PrandtlMeyerParameter1 + inputVariableForCorrelation_
                        * ( PrandtlMeyerParameter2 + inputVariableForCorrelation_
                            * PrandtlMeyerParameter3 ) ) )
            / ( 1.0 + inputVariableForCorrelation_ * ( PrandtlMeyerParameter4
                                                       + inputVariableForCorrelation_
                                                       * PrandtlMeyerParameter5 ) );

    // Return Mach number.
    return machNumber_;
}

//! Compute ratio of post- to pre-shock pressure.
double computeShockPressureRatio( double normalMachNumber,
                                  double ratioOfSpecificHeats )
{
    // Return pressure ratio.
    return 1.0 + 2.0 * ratioOfSpecificHeats / ( ratioOfSpecificHeats + 1.0 )
            * ( normalMachNumber * normalMachNumber - 1.0 );
}

//! Compute ratio of post- to pre-shock density.
double computeShockDensityRatio( double normalMachNumber,
                                 double ratioOfSpecificHeats )
{
    // Declare local variables.
    double machNumberSquared_;

    // Calculate mach number squared for efficiency.
    machNumberSquared_ = pow( normalMachNumber, 2.0 );

    // Return density ratio.
    return ( ratioOfSpecificHeats + 1.0 ) * machNumberSquared_
            / ( 2.0 + ( ratioOfSpecificHeats - 1.0 ) * machNumberSquared_ );
}

//! Compute ratio of post- to pre-shock temperature.
double computeShockTemperatureRatio( double normalMachNumber,
                                     double ratioOfSpecificHeats )
{
    // Return temperature ratio from perfect gas law.
    return 1.0 / computeShockDensityRatio( normalMachNumber, ratioOfSpecificHeats )
            * computeShockPressureRatio( normalMachNumber, ratioOfSpecificHeats );
}

//! Compute jump in entropy across a shock wave.
double computeShockEntropyJump( double normalMachNumber,
                                double ratioOfSpecificHeats,
                                double specificGasConstant )
{
    // Declare local variables.
    double specificHeatConstantPressure_;

    // Calculate specific heat at constant pressure.
    specificHeatConstantPressure_ = ratioOfSpecificHeats * specificGasConstant
            / ( ratioOfSpecificHeats - 1.0 );

    // Return entropy jump from temperature and pressure ratio.
    return specificHeatConstantPressure_
            * log( computeShockTemperatureRatio( normalMachNumber, ratioOfSpecificHeats ) )
            - specificGasConstant
            * log( computeShockPressureRatio( normalMachNumber, ratioOfSpecificHeats ) );
}

//! Compute post- to pre-shock total pressure ratio.
double computeShockTotalPressureRatio( double normalMachNumber,
                                       double ratioOfSpecificHeats,
                                       double specificGasConstant )
{
    // Return total pressure ratio from entropy jump.
    return exp( -1.0 * computeShockEntropyJump( normalMachNumber, ratioOfSpecificHeats,
                                                specificGasConstant ) / specificGasConstant );
}

//! Compute shock deflection angle.
double computeShockDeflectionAngle( double shockAngle, double machNumber,
                                    double ratioOfSpecificHeats )
{
    // Declare local variables.
    double tangentOfDeflectionAngle_;

    // Calculate tangent of deflection angle.
    tangentOfDeflectionAngle_ = 2.0 * ( pow( machNumber * sin( shockAngle ), 2.0 ) - 1.0 )
            / ( tan( shockAngle ) * ( pow( machNumber, 2.0 )
                                      * ( ratioOfSpecificHeats
                                          + cos( 2.0 * shockAngle ) ) + 2.0 ) );

    // Return deflection angle.
    return atan( tangentOfDeflectionAngle_ );
}

} // namespace aerodynamics
} // namespace tudat
