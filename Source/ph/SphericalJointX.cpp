// File name: SphericalJointX.cpp
// Des: This class is for creating PhysX Spherical Joint
// Date: 27/2/1389
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/SphericalJointX.h"
#include "../../Headers/ph/actorDynamicX.h"
#include "../../Headers/ph/actorStaticX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		SphericalJointX::SphericalJointX(kge::ph::PhysXManager *pMan, kge::ph::Actor *a1, kge::ph::Actor *a2, kge::math::Vector *Anchor)
		{
			NxActor					*aX1 , *aX2;
			NxSphericalJointDesc	nxSJD;
			//ActorStaticX	*aS1 , *aS2;

			if( a1->getType() == EATP_DYNAMIC || a1->getType() == EATP_KINEMATIC )
				aX1 = ((ActorDynamicX*)(a1))->getPhActor();

			if( a2 && (a2->getType() == EATP_DYNAMIC || a2->getType() == EATP_KINEMATIC) )
				aX2 = ((ActorDynamicX*)(a2))->getPhActor();

			nxSJD.setToDefault();

			nxSJD.actor[0] = aX1;
			if( a2 )
				nxSJD.actor[1] = aX2;

			nxSJD.setGlobalAnchor( NxVec3( Anchor->x , Anchor->y , Anchor->z ) );

			m_pSphericalJoint = (NxSphericalJoint*)(pMan->getPhysxScene()->createJoint( nxSJD ));
		}
		//----------------------------------------------------------
		//
		//----------------------------------------------------------
		void SphericalJointX::setSwingAxis( math::Vector* swingAxis )
		{
			NxSphericalJointDesc nxSJD;
			m_pSphericalJoint->saveToDesc( nxSJD );
			nxSJD.swingAxis.set( swingAxis->x , swingAxis->y , swingAxis->z );
			nxSJD.flags |= NX_SJF_SWING_LIMIT_ENABLED;
			m_pSphericalJoint->loadFromDesc( nxSJD );
		}
		//----------------------------------------------------------
		//
		//----------------------------------------------------------
		math::Vector* SphericalJointX::getSwingAxis( void )
		{
			math::Vector* v = new math::Vector();
			NxSphericalJointDesc nxSJD;
			m_pSphericalJoint->saveToDesc( nxSJD );
			v->set( nxSJD.swingAxis.x , nxSJD.swingAxis.y , nxSJD.swingAxis.z );
			return v;
		}
		//----------------------------------------------------------
		//
		//----------------------------------------------------------
		void SphericalJointX::setSwingLimit( float value , float restitution )
		{
			NxSphericalJointDesc nxSJD;
			m_pSphericalJoint->saveToDesc( nxSJD );
			nxSJD.swingLimit.value = value;
			nxSJD.swingLimit.restitution = restitution;
			nxSJD.flags |= NX_SJF_SWING_LIMIT_ENABLED;
			m_pSphericalJoint->loadFromDesc( nxSJD );

		}
		//----------------------------------------------------------
		//
		//----------------------------------------------------------
		void SphericalJointX::setTwistLimit( float highValue , float lowValue , float highRestitution , float lowRestitution )
		{
			NxSphericalJointDesc nxSJD;
			m_pSphericalJoint->saveToDesc( nxSJD );

			NxJointLimitPairDesc lim;

			lim.high.value = highValue;
			lim.high.restitution = highRestitution;
			lim.low.value = lowValue;
			lim.low.restitution = lowRestitution;

			nxSJD.twistLimit = lim;
			nxSJD.flags |= NX_SJF_TWIST_LIMIT_ENABLED;
			m_pSphericalJoint->loadFromDesc( nxSJD );

		}
	} // ph
} // kge

#endif // KGE_USE_PHYSX
