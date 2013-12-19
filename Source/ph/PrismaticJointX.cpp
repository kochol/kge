// File name: PrismaticJointX.cpp
// Des: This class is for creating PhysX Pristamic Joint
// Date: 30/03/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/PrismaticJointX.h"
#include "../../Headers/ph/ActorDynamicX.h"
#include "../../Headers/ph/ActorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		PrismaticJointX::PrismaticJointX(PhysXManager *pMan , kge::ph::Actor *a1, kge::ph::Actor *a2, kge::math::Vector *Anchor, kge::math::Vector *axis)
		{
			NxActor				*nxA1,*nxA2;
			NxPrismaticJointDesc nxPrismaticJointDesc;

			if( !a1 )
				return;

			nxPrismaticJointDesc.setToDefault();

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				nxA1 = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && ( a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC ) )
				nxA2 = ((ActorDynamicX*)(a1))->getPhActor();

			nxPrismaticJointDesc.actor[0] = nxA1;

			if( a2 )
				nxPrismaticJointDesc.actor[1] = nxA2;

			nxPrismaticJointDesc.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			if( axis )
				nxPrismaticJointDesc.setGlobalAxis( NxVec3( axis->x , axis->y , axis->z ) );

			m_pPrismaticJoint = ((NxPrismaticJoint*)(pMan->getPhysxScene()->createJoint( nxPrismaticJointDesc )));
		}
	} // ph
} // kge

#endif // KGE_USE_PHYSX