// File name: FixedJointX.h
// Des: This class is for creating PhysX Fixed Joint
// Date: 04/03/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_FIXED_JOINT_X_H
#define KGE_PH_FIXED_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class FixedJointX : public FixedJoint
		{
		public:
			FixedJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , kge::math::Vector* Anchor , math::Vector *axis );

			~FixedJointX(){ /*Nothing To Do*/ }
		protected:
			NxFixedJoint* m_pFixedJoint;
		}; // class FixedJointX
	} // ph
} // kge

#endif // KGE_PH_FIXED_JOINT_X_H

#endif // KGE_USE_PHYSX