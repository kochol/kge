// File name: RevoluteJointX.h
// Des: This class is for creating PhysX Revolute Joint
// Date: 25/02/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_REVOLUTE_JOINT_X_H
#define KGE_PH_REVOLUTE_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class RevoluteJointX : public RevoluteJoint
		{
		public:
			RevoluteJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , math::Vector *Anchor );

			~RevoluteJointX() { /*Nothing To Do */ }

			JointType getType(){ return EJTP_REVOLUTE; }

			void setLimit( math::Vector *axis , float high , float low );
		protected:
			NxRevoluteJoint* m_pRevoluteJoint;
		}; // class RevoluteJoint
	} // ph
} // kge

#endif // KGE_PH_REVOLUTE_JOINT_X_H

#endif // KGE_USE_PHYSX