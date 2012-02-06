#include "Tudat/Astrodynamics/BasicAstrodynamics/momentDueToForceModel.h"

namespace tudat
{

void MomentDueToForceModel::computeMoment( State* pointerToState, double time )
{
    pointerToForceModel_->computeForce( pointerToState, time );
    Eigen::Vector3d force_ = pointerToForceModel_->getForce( );
    momentDueToForce_ = forceApplicationArm_.cross( force_ );
}

}
