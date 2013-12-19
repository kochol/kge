// File name: actorStaticX.cpp
// Des: This class is for creating PhysX Static Actor
// Date: 07/11/1388
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/ActorStaticX.h"
#include "stdio.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		ActorStaticX::ActorStaticX( PhysXManager* phMan , kge::sn::SceneNode *kgeObj , math::KGE_SHAPES shape , CollitionType ct , unsigned int *actorCounter )
		{
			NxActorDesc		nxActorDesc;
			NxActor*		tempActor;
			NxScene*		phScene;
			kge::math::Vector point;

			NxVec3			  pVec;
			kge::math::Vector kVec;

			//Set To Default
			nxActorDesc.setToDefault();

			//Get Size Of Kge Object( Aligned Bounding Box )
			point = (kgeObj->GetAxisAlignedBoundingBox()->m_vMax) - (kgeObj->GetAxisAlignedBoundingBox()->m_vMin);
			point /= 2.0f;

			memcpy( &pVec.x , &point.x , 12 );
			
			if( shape == math::KGE_SHAPE_AABB )
			{
				NxBoxShapeDesc	nxBoxShapeDesc;
			
				//Set Physx Size
				nxBoxShapeDesc.dimensions = pVec;
				
				if( ct != ECTP_TRIGGER_DISABLE )
					nxBoxShapeDesc.shapeFlags |= ct;

				if( !nxBoxShapeDesc.isValid() )
					kge::io::Logger::Log( "ActorDynamicX : Invalid Box Shape Desc!!!\n" );

				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxBoxShapeDesc );
			}
			else if( shape == math::KGE_SHAPE_SPHERE )
			{
				NxSphereShapeDesc nxSphereShapeDesc;

				nxSphereShapeDesc.radius = point.x; 

				if( ct != ECTP_TRIGGER_DISABLE )
					nxSphereShapeDesc.shapeFlags |= ct;

				if( !nxSphereShapeDesc.isValid() )
					kge::io::Logger::Log( "Invalid Sphere Shape Desc!!!\n" );
				
				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxSphereShapeDesc );
			}
			else if( shape == math::KGE_SHAPE_CAPSULE )
			{
				NxCapsuleShapeDesc nxCapsuleShapeDesc;

				nxCapsuleShapeDesc.radius = point.x;
				nxCapsuleShapeDesc.height = point.x;

				if( ct != ECTP_TRIGGER_DISABLE )
					nxCapsuleShapeDesc.shapeFlags |= ct;

				if( !nxCapsuleShapeDesc.isValid() )
					kge::io::Logger::Log( "Invalid Capsule Shape Desc!!!" );

				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxCapsuleShapeDesc );
			}

			//Get Position of Kge Object
			kVec = kgeObj->GetPosition();

			//Set Physx Position
			memcpy( &pVec.x , &kVec.x , 12 );
			nxActorDesc.globalPose.t = pVec;

			//Get Physx Scene from PhysXManager
			phScene = phMan->getPhysxScene();
			localPhysXManager = phMan;

			//Create Actor
			tempActor = phScene->createActor( nxActorDesc );
			if( tempActor  == NULL )
			{
				kge::io::Logger::Log( "Cannot Create Actor!!!\n" );
				return;
			}

			//tempActor->raiseBodyFlag( NX_BF_KINEMATIC );

			tempActor->userData = ((ActorDynamic*)(this));
			

			(*actorCounter)++;
			actorNumber = (*actorCounter);
			localKgeObj = kgeObj;
			localPhysXManager = phMan;
			localActor = tempActor;
		}

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		ActorStaticX::ActorStaticX( PhysXManager* phMan , kge::sn::SceneNode *kgeObj , math::Shape* shape , CollitionType ct , unsigned int *actorCounter )
		{
			NxActorDesc		nxActorDesc;
			NxActor*		tempActor;
			NxScene*		phScene;
			kge::math::Vector point;

			NxVec3			  pVec;
			kge::math::Vector kVec;

			//Set To Default
			nxActorDesc.setToDefault();
			
			if( shape->getType() == math::KGE_SHAPE_AABB )
			{
				NxBoxShapeDesc	nxBoxShapeDesc;

				math::AABB *aa = (math::AABB*)(shape);

				point = (aa->m_vMax - aa->m_vMin);
				point.x = fabs( point.x );
				point.y = fabs( point.y );
				point.z = fabs( point.z );
			
				//Set Physx Box Size
				nxBoxShapeDesc.dimensions.set( point.x , point.y , point.z );

				if( ct != ECTP_TRIGGER_DISABLE )
					nxBoxShapeDesc.shapeFlags |= ct;

				if( !nxBoxShapeDesc.isValid() )
					kge::io::Logger::Log( "Invalid Box Shape Desc!!!\n" );

				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxBoxShapeDesc );
			}
			else if( shape->getType() == math::KGE_SHAPE_SPHERE )
			{
				NxSphereShapeDesc nxSphereShapeDesc;

				math::Sphere *aa = (math::Sphere*)(shape);

				nxSphereShapeDesc.radius = aa->GetRadius(); 

				if( ct != ECTP_TRIGGER_DISABLE )
					nxSphereShapeDesc.shapeFlags |= ct;

				if( !nxSphereShapeDesc.isValid() )
					kge::io::Logger::Log( "Invalid Sphere Shape Desc!!!\n" );
				
				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxSphereShapeDesc );
			}
			else if( shape->getType() == math::KGE_SHAPE_CAPSULE )
			{
				NxCapsuleShapeDesc nxCapsuleShapeDesc;

				nxCapsuleShapeDesc.radius = point.x;
				nxCapsuleShapeDesc.height = point.x;

				if( ct != ECTP_TRIGGER_DISABLE )
					nxCapsuleShapeDesc.shapeFlags |= ct;

				if( !nxCapsuleShapeDesc.isValid() )
					kge::io::Logger::Log( "Invalid Capsule Shape Desc!!!\n" );

				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxCapsuleShapeDesc );
			}

			//Get Position of Kge Object
			kVec = kgeObj->GetPosition();

			//Set Physx Position
			memcpy( &pVec.x , &kVec.x , 12 );
			nxActorDesc.globalPose.t = pVec;

			//Get Physx Scene from PhysXManager
			phScene = phMan->getPhysxScene();
			localPhysXManager = phMan;

			//Create Actor
			tempActor = phScene->createActor( nxActorDesc );
			if( tempActor  == NULL )
			{
				kge::io::Logger::Log( "Cannot Create Actor!!!\n" );
				return;
			}

			tempActor->userData = ((ActorDynamic*)(this));

			(*actorCounter)++;
			actorNumber = (*actorCounter);
			localKgeObj = kgeObj;
			localPhysXManager = phMan;
			localActor = tempActor;
		}

		//----------------------------------------------------------
		// Destructor
		//----------------------------------------------------------
		ActorStaticX::~ActorStaticX()
		{
		}

	} // ph
} // kge

#endif //KGE_USE_PHYSX