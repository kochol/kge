// File name: RevoluteJointX.cpp
// Des: This class is for creating PhysX Revolute Joint
// Date: 30/02/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/RevoluteJointX.h"
#include "../../Headers/ph/actorDynamicX.h"
#include "../../Headers/ph/actorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		RevoluteJointX::RevoluteJointX(kge::ph::PhysXManager *pMan, kge::ph::Actor *a1, kge::ph::Actor *a2, kge::math::Vector *Anchor)
		{
			NxActor				*nxA1,*nxA2;
			NxRevoluteJointDesc nxRevJointDesc;

			if( !a1 )
				return;

			nxRevJointDesc.setToDefault();

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				nxA1 = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && (a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC) )
				nxA2 = ((ActorDynamicX*)(a2))->getPhActor();

			nxRevJointDesc.actor[0] = nxA1;
			if( a2 )
				nxRevJointDesc.actor[1] = nxA2;

			nxRevJointDesc.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			m_pRevoluteJoint = (NxRevoluteJoint*)(pMan->getPhysxScene()->createJoint( nxRevJointDesc ));
		}

		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		void RevoluteJointX::setLimit( math::Vector *axis , float high , float low )
		{
			NxRevoluteJointDesc nxRevJointDesc;

			m_pRevoluteJoint->saveToDesc( nxRevJointDesc );
			nxRevJointDesc.setGlobalAxis( NxVec3( axis->x , axis->y , axis->z ) );
			nxRevJointDesc.limit.high.value = high;
			nxRevJointDesc.limit.low.value = low;
			nxRevJointDesc.flags |= NX_RJF_LIMIT_ENABLED;
			m_pRevoluteJoint->loadFromDesc( nxRevJointDesc );
		}
	} // ph
} // kge

#endif // KGE_USE_PHYSX
