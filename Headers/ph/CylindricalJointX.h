// File name: CylindricalJointX.h
// Des: This class is for creating PhysX Cylindrical Joint
// Date: 03/03/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_CYLINDRICAL_JOINT_X_H
#define KGE_PH_CYLINDRICAL_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class CylindricalJointX : public CylindricalJoint
		{
		public:
			CylindricalJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , math::Vector *Anchor , math::Vector *axis );

			virtual ~CylindricalJointX() { /*Nothing To Do*/ }
		protected:
			NxCylindricalJoint* m_pCylindricalJoint;
		}; //class CylindricalJointX
	} // ph
} // kge
#endif // KGE_PH_CYLINDRICAL_JOINT_X_H

#endif // KGE_USE_PHYSX