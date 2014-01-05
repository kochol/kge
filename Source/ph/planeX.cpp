// File name: PlaneX.cpp
// Des: This class is for creating PhysX Actor
// Date: 20/10/1388
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/planeX.h"
#include "stdio.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		PlaneX::PlaneX( PhysXManager *pPhysMan , kge::sn::SceneNode *kgePlane , kge::math::Vector *normal )
		{
			NxPlaneShapeDesc nxPlaneShapeDesc;
			NxActorDesc		 nxActorDesc;
			NxActor*		 tempActor;
			NxVec3			 pVec;
			kge::math::Vector kVec;


			nxPlaneShapeDesc.normal=NxVec3( normal->x , normal->y , normal->z );

			if( kgePlane )
				kVec=kgePlane->GetPosition();

			nxPlaneShapeDesc.d = ( (normal->x*kVec.x) + (normal->y*kVec.y) + (normal->z*kVec.z) );
			nxActorDesc.shapes.push_back( &nxPlaneShapeDesc );

			tempActor = pPhysMan->getPhysxScene()->createActor( nxActorDesc );
			localPlane = tempActor;
		} // PlaneX

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		PlaneX::PlaneX( PhysXManager *pPhysMan , kge::math::Vector* pos , kge::math::Vector *normal , float d )
		{
			NxPlaneShapeDesc nxPlaneShapeDesc;
			NxActorDesc		 nxActorDesc;
			NxActor*		 tempActor;

			nxPlaneShapeDesc.normal=NxVec3( normal->x , normal->y , normal->z );
			nxPlaneShapeDesc.d = d;

			nxActorDesc.shapes.push_back( &nxPlaneShapeDesc );

			nxActorDesc.globalPose.t = NxVec3( pos->x , pos->y , pos->z );

			tempActor = pPhysMan->getPhysxScene()->createActor( nxActorDesc );
			localPlane = tempActor;
		} // PlaneX

		//----------------------------------------------------------
		// Destructor
		//----------------------------------------------------------
		PlaneX::~PlaneX()
		{
		}

	} // ph
} // kge

#endif // KGE_USE_PHYSX
