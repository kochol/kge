// File name: PrismaticJointX.h
// Des: This class is for creating PhysX Prismatic Joint
// Date: 03/03/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_PRISMATIC_JOINT_X_H
#define KGE_PH_PRISMATIC_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class PrismaticJointX : public PrismaticJoint
		{
		public:
			//! Contructor
			PrismaticJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , math::Vector *Anchor , math::Vector *axis );

			//! Distructor
			~PrismaticJointX() { /* Nothing To Do */ }
		protected:
			NxPrismaticJoint *m_pPrismaticJoint;
		}; // class PrismaticJointX
	} // ph
} // kge

#endif // KGE_PH_PRISMATIC_JOINT_X_H

#endif // KGE_USE_PHYSX