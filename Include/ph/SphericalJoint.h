#ifndef KGE_PH_SPHERICAL_JOINT_H
#define KGE_PH_SPHERICAL_JOINT_H

namespace kge
{
	namespace ph
	{
		class SphericalJoint : public Joint
		{
		public:
			//! Constructor
			SphericalJoint() { /*Nothing To Do*/ }//( Actor *a1 , Actor *a2 , math::Vector &anchor );

			//! Distructor
			virtual ~SphericalJoint() { /*Nothing To Do*/ }

			JointType getType(){ return EJTP_SPHERICAL; }

			virtual void			setSwingAxis( math::Vector* swingAxis ) = 0;

			virtual math::Vector*	getSwingAxis( void ) = 0;

			virtual void			setSwingLimit( float value , float restitution ) = 0;

			virtual void			setTwistLimit( float highValue , float lowValue , float highRestitution , float lowRestitution ) = 0;
		protected:
		};
	} // ph
} // kge
#endif // KGE_PH_SPHERICAL_JOINT_H