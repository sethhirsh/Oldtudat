#ifndef MOMENTDUETOFORCEMODEL_H
#define MOMENTDUETOFORCEMODEL_H

#include "Tudat/Astrodynamics/BasicAstrodynamics/generalizedForceModel.h"
#include "Tudat/Astrodynamics/BasicAstrodynamics/forceModel.h"

namespace tudat
{

//! Base class for all generalized forces.
/*!
 *  Class for moment due to an resultant force.
 */
class MomentDueToForceModel : public GeneralizedForceModel< Eigen::Vector3d, 3 >
{
public:

    //! Default constructor.
    /*!
     * Default constructor.
     */
    MomentDueToForceModel( ForceModel* pointerToForceModel ) : pointerToForceModel_( pointerToForceModel_ ) { forceApplicationArm_.setZero( ); }

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~MomentDueToForceModel( ) { }

    //! Get moment.
    /*!
     * Returns the moment.
     * \return Moment.
     */
    Eigen::Vector3d getMomentDueToForce( ) { return momentDueToForce_; }

    //! Get moment.
    /*!
     * Returns the moment.
     * \return Moment.
     */
    virtual Eigen::Vector3d  getGeneralizedForce(  ) { return getMomentDueToForce( ); }

    //! Set force application point.
    /*!
     * Sets force application arm, i.e., the vector from the origin of the reference frame in
     * which the moment is calculated, to the application point of the input force.
     * \param forceApplicationArm Vector arm to application point of force.
     */
    void setForceApplicationArm( Eigen::Vector3d& forceApplicationArm )
    { forceApplicationArm_ = forceApplicationArm; }

    //! Get force application arm.
    /*!
     * Returns force application arm, i.e., returns the vector from the origin of the reference
     * frame in which the moment is calculated, to the application point of the input force.
     * \return Vector arm to application point of force.
     */
    Eigen::Vector3d getForceApplicationArm( ) { return forceApplicationArm_; }

    //! Get force model.
    /*!
     * Returns force model. This force is calculated in a reference frame that has the same
     * orientation as the frame the moment is calculated in (normally the body frame).
     * \return Pointer to force model.
     */
    ForceModel* getForceModel( ) { return pointerToForceModel_; }

     //! Compute moment.
     /*!
      * Computes the moment.
      */
    virtual void computeMoment( State* pointerToState, double time );

protected:

private:

    //! Pointer to force model.
    /*!
     * Pointer to force model.
     */
    ForceModel* pointerToForceModel_;

    //! Force application point.
    /*!
     * Force application point, which can also be considered the force arm.
     */
    Eigen::Vector3d forceApplicationArm_;

    //! Moment.
    /*!
     * Moment.
     */
    Eigen::Vector3d momentDueToForce_;
};

}

#endif // MOMENTDUETOFORCEMODEL_H
