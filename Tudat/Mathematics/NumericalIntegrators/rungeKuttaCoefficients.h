#ifndef TUDAT_RUNGEKUTTACOEFFICIENTS_H
#define TUDAT_RUNGEKUTTACOEFFICIENTS_H

#include <Eigen/Core>

//! Tudat library namespace.
namespace tudat
{

//! Tudat mathematics namespace
namespace mathematics
{

//! Integrators namespace
namespace numerical_integrators
{

struct RungeKuttaCoefficients
{
    Eigen::MatrixXd aCoefficients;
    Eigen::MatrixXd bCoefficients;
    Eigen::VectorXd cCoefficients;
    unsigned int higherOrder;
    unsigned int lowerOrder;


    RungeKuttaCoefficients() :
        aCoefficients(), bCoefficients(), cCoefficients(),
        higherOrder( 0 ), lowerOrder( 0 )
    { }
    RungeKuttaCoefficients( const Eigen::MatrixXd& aCoefficients_,
                            const Eigen::MatrixXd& bCoefficients_,
                            const Eigen::MatrixXd& cCoefficients_,
                            const unsigned int higherOrder_,
                            const unsigned int lowerOrder_ ) :
        aCoefficients( aCoefficients_ ), bCoefficients( bCoefficients_ ),
        cCoefficients( cCoefficients_ ),
        higherOrder( higherOrder_ ), lowerOrder( lowerOrder_ )
    { }

    enum CoefficientSets
    {
        rungeKuttaFehlberg45,
        rungeKuttaFehlberg56,
        rungeKuttaFehlberg78
    };

    static const RungeKuttaCoefficients& get( CoefficientSets coefficientSet );
};
typedef struct RungeKuttaCoefficients RungeKuttaCoefficients;


} // namespace integrators

} // namespace mathematics

} // namespace tudat

#endif // RUNGEKUTTACOEFFICIENTS_H
