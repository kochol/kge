// File name: FixedJointX.h
// Des: This class is for creating PhysX Fixed Joint
// Date: 04/03/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/fixedJointX.h"
#include "../../Headers/ph/actorDynamicX.h"
#include "../../Headers/ph/actorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		FixedJointX::FixedJointX( PhysXManager *pMan , kge::ph::Actor *a1, kge::ph::Actor *a2, kge::math::Vector *Anchor , math::Vector *axis )
		{
			if( !a1 )
				return;

			NxFixedJointDesc nxJointDesc;

			nxJointDesc.setToDefault();

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				nxJointDesc.actor[0] = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && ( a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC ) )
				nxJointDesc.actor[1] = ((ActorDynamicX*)(a1))->getPhActor();

			nxJointDesc.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			if( axis )
				nxJointDesc.setGlobalAxis( NxVec3( axis->x , axis->y , axis->z ) );

			m_pFixedJoint = (NxFixedJoint*)(pMan->getPhysxScene()->createJoint( nxJointDesc ) );
		}
	} // ph
} // kge

#endif // KGE_USE_PHYSX
