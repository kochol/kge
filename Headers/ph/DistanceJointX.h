// File name: DistanceJointX.h
// Des: This class is for creating PhysX Distance Joint
// Date: 09/03/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_DISTANCE_JOINT_X_H
#define KGE_PH_DISTANCE_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class DistanceJointX : public DistanceJoint
		{
		public:

			//Contructor
			DistanceJointX( PhysXManager *pMan , Actor *a1 ,  Actor *a2 , math::Vector *Anchor , DistanceJointFlag flags , unsigned int minDis , unsigned int maxDis );

			//Distructor
			~DistanceJointX() { /* Nothing To Do */ }

		protected:
			NxDistanceJoint*	m_pDistanceJoint;
		}; // Class DistanceJoint
	} // ph
} // kge

#endif // KGE_PH_DISTANCE_JOINT_X_H

#endif // KGE_USE_PHYSX