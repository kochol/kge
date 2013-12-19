// File name: DistanceJointX.h
// Des: This class is for creating PhysX Distance Joint
// Date: 09/03/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/DistanceJointX.h"
#include "../../Headers/ph/ActorDynamicX.h"
#include "../../Headers/ph/ActorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		DistanceJointX::DistanceJointX(kge::ph::PhysXManager *pMan, kge::ph::Actor *a1, kge::ph::Actor *a2, kge::math::Vector *Anchor , DistanceJointFlag flags ,  unsigned int minDis, unsigned int maxDis)
		{
			if( !a1 )
				return;

			NxDistanceJointDesc nxJointDesc;

			nxJointDesc.setToDefault();

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				nxJointDesc.actor[0] = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && ( a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC ) )
				nxJointDesc.actor[1] = ((ActorDynamicX*)(a1))->getPhActor();

			nxJointDesc.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			if( flags & EDJF_MIN_DISTANCE_ENABLED )
				nxJointDesc.minDistance = minDis;

			if( flags & EDJF_MAX_DISTANCE_ENABLED )
				nxJointDesc.maxDistance = maxDis;

			nxJointDesc.flags = flags;

			m_pDistanceJoint = ((NxDistanceJoint*)(pMan->getPhysxScene()->createJoint( nxJointDesc ) ) );
		}
	} // ph
} // kge
#endif // KGE_USE_PHYSX