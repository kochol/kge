// File name: CylindricalJointX.h
// Des: This class is for creating PhysX Cylindrical Joint
// Date: 03/03/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/CylindricalJointX.h"
#include "../../Headers/ph/actorDynamicX.h"
#include "../../Headers/ph/actorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		CylindricalJointX::CylindricalJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , math::Vector *Anchor , math::Vector *axis )
		{
			NxActor				*nxA1,*nxA2;
			NxCylindricalJointDesc nxCylindericalJointDesc;

			if( !a1 )
				return;

			nxCylindericalJointDesc.setToDefault();

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				nxA1 = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && ( a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC ) )
				nxA2 = ((ActorDynamicX*)(a1))->getPhActor();

			nxCylindericalJointDesc.actor[0] = nxA1;

			if( a2 )
				nxCylindericalJointDesc.actor[1] = nxA2;

			nxCylindericalJointDesc.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			if( axis )
				nxCylindericalJointDesc.setGlobalAxis( NxVec3( axis->x , axis->y , axis->z ) );

			m_pCylindricalJoint = ((NxCylindricalJoint*)(pMan->getPhysxScene()->createJoint( nxCylindericalJointDesc ) ) );
		}
	} // ph
} // kge

#endif // KGE_USE_PHYSX
