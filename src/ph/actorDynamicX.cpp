// File name: ActorX.cpp
// Des: This class is for creating PhysX Actor
// Date: 20/10/1388
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/actorDynamicX.h"
#include "stdio.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		ActorDynamicX::ActorDynamicX( PhysXManager *pMan , sn::SceneNode* kgeObj , math::KGE_SHAPES shape , BodyFlag bf , Material* material , float mass , unsigned int *actorCounter )
		{
			NxActorDesc		nxActorDesc;
			NxBodyDesc		nxBodyDesc;
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

				//Set Material
				if( material != NULL )
				{
					nxBoxShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

				if( !nxBoxShapeDesc.isValid() )
					kge::io::Logger::Log( "ActorDynamicX : Invalid Box Shape Desc!!!\n" );

				//Set Shape Of Actor
				nxActorDesc.shapes.push_back( &nxBoxShapeDesc );
			}
			else if( shape == math::KGE_SHAPE_SPHERE )
			{
				NxSphereShapeDesc nxSphereShapeDesc;

				nxSphereShapeDesc.radius = point.x;

				//Set Material
				if( material != NULL )
				{
					nxSphereShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

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

				//Set Material
				if( material != NULL )
				{
					nxCapsuleShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

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

			//Set Actor Mass
			nxBodyDesc.mass = mass;

			//Set Body Flags
			if( bf != EBFP_NORMAL )
				nxBodyDesc.flags |= bf;

			//Set Body
			nxActorDesc.body=&nxBodyDesc;

			//Get Physx Scene from PhysXManager
			phScene = pMan->getPhysxScene();
			localPhysXManager = pMan;

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
			localPhysXManager = pMan;
			localActor = tempActor;
		}

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		ActorDynamicX::ActorDynamicX( PhysXManager *pMan , sn::SceneNode* kgeObj , math::Shape* shape , BodyFlag bf , Material* material , float mass , unsigned int *actorCounter )
		{
			NxActorDesc		nxActorDesc;
			NxBodyDesc		nxBodyDesc;
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

				//Set Material
				if( material != NULL )
				{
					nxBoxShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

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

				//Set Material
				if( material != NULL )
				{
					nxSphereShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

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

				//Set Material
				if( material != NULL )
				{
					nxCapsuleShapeDesc.materialIndex = material->getMaterialIndex();
					m_pMaterial = material;
				}

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

			//Set Actor Mass
			nxBodyDesc.mass = mass;

			//Set Body Flags
			if( bf != EBFP_NORMAL )
				nxBodyDesc.flags |= bf;

			//Set Body
			nxActorDesc.body=&nxBodyDesc;

			//Get Physx Scene from PhysXManager
			phScene = pMan->getPhysxScene();
			localPhysXManager = pMan;

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
			localPhysXManager = pMan;
			localActor = tempActor;
		}

		//----------------------------------------------------------
		// Destructor
		//----------------------------------------------------------
		ActorDynamicX::~ActorDynamicX()
		{
			NxScene*		phScene;
			phScene = localPhysXManager->getPhysxScene();
			phScene->releaseActor(*localActor);
		}

		//----------------------------------------------------------
		// Set Position
		//----------------------------------------------------------
		void ActorDynamicX::setPosition( kge::math::Vector *pos )
		{
			NxVec3	pVec( pos->x , pos->y , pos->z );
			localActor->setGlobalPosition( pVec );
		}

		//----------------------------------------------------------
		//! Set Mass
		//----------------------------------------------------------
		void ActorDynamicX::setMass( float mass )
		{
			localActor->setMass( mass );
		}

		//----------------------------------------------------------
		//! Get Mass
		//----------------------------------------------------------
		float ActorDynamicX::getMass()
		{
			return localActor->getMass();
		}

		//----------------------------------------------------------
		//! Recomputes a dynamic actor's mass properties from its shapes
		//----------------------------------------------------------
		bool ActorDynamicX::updateMassFromShapes( float density , float totalMass )
		{
			return localActor->updateMassFromShapes( density , totalMass );
		}

		//----------------------------------------------------------
		//! Set Angular Damping
		//----------------------------------------------------------
		void ActorDynamicX::setAngularDamping( float angularDamping )
		{
			localActor->setAngularDamping( angularDamping );
		}

		//----------------------------------------------------------
		//! Retrieves the angular damping coefficient.
		//----------------------------------------------------------
		float ActorDynamicX::getAngularDamping()
		{
			return localActor->getAngularDamping();
		}

		//----------------------------------------------------------
		//! Set linear Damping
		//----------------------------------------------------------
		void ActorDynamicX::setLinearDamping( float linearDamping )
		{
			localActor->setLinearDamping( linearDamping );
		}

		//----------------------------------------------------------
		//! Retrieves the linear damping coefficient.
		//----------------------------------------------------------
		float ActorDynamicX::getLinearDamping()
		{
			return localActor->getLinearDamping();
		}

		//----------------------------------------------------------
		//! Sets the linear velocity of the actor.
		//----------------------------------------------------------
		void ActorDynamicX::setLinearVelocity( const kge::math::Vector *linVel )
		{
			NxVec3 pVec( linVel->x , linVel->y , linVel->z );
			localActor->setLinearVelocity( pVec );
		}

		//----------------------------------------------------------
		//! Sets the angular velocity of the actor.
		//----------------------------------------------------------
		void ActorDynamicX::setAngularVelocity	( const kge::math::Vector *angVel )
		{
			NxVec3 pVec( angVel->x , angVel->y , angVel->z );
			localActor->setAngularVelocity( pVec );
		}

		//----------------------------------------------------------
		//! Retrieves the linear velocity of an actor.
		//----------------------------------------------------------
		kge::math::Vector ActorDynamicX::getLinearVelocity	()
		{
			NxVec3 pVec = localActor->getLinearVelocity();
			return kge::math::Vector( pVec.x , pVec.y , pVec.z );
		}

		//----------------------------------------------------------
		//! Retrieves the angular velocity of the actor.
		//----------------------------------------------------------
		kge::math::Vector ActorDynamicX::getAngularVelocity()
		{
			NxVec3 pVec = localActor->getAngularVelocity();
			return kge::math::Vector( pVec.x , pVec.y , pVec.z );
		}

		//----------------------------------------------------------
		//! Lets you set the maximum angular velocity permitted for this actor.
		//----------------------------------------------------------
		void ActorDynamicX::setMaxAngularVelocity(float maxAngVel)
		{
			localActor->setMaxAngularVelocity( maxAngVel );
		}

		//----------------------------------------------------------
		//! Retrieves the maximum angular velocity permitted for this actor.
		//----------------------------------------------------------
		float ActorDynamicX::getMaxAngularVelocity()
		{
			return localActor->getMaxAngularVelocity();
		}

		//----------------------------------------------------------
		//! Sets the linear momentum of the actor.
		//----------------------------------------------------------
		void ActorDynamicX::setLinearMomentum( const kge::math::Vector* linMoment )
		{
			NxVec3 pVec;
			kge::math::Vector kVec( linMoment->x , linMoment->y , linMoment->z );

			memcpy( &pVec.x , &kVec.x , 12 );

			localActor->setLinearMomentum( pVec );
		}

		//----------------------------------------------------------
		//! Sets the angular momentum of the actor.
		//----------------------------------------------------------
		void ActorDynamicX::setAngularMomentum( const kge::math::Vector* angMoment )
		{
			NxVec3 pVec( angMoment->x , angMoment->y , angMoment->z );
			localActor->setAngularMomentum( pVec );
		}

		//----------------------------------------------------------
		//! Retrieves the linear momentum of an actor.
		//----------------------------------------------------------
		kge::math::Vector ActorDynamicX::getLinearMomentum()
		{
			NxVec3 pVec = localActor->getLinearMomentum();
			return kge::math::Vector( pVec.x , pVec.y , pVec.z );
		}

		//----------------------------------------------------------
		//! Retrieves the angular momentum of an actor.
		//----------------------------------------------------------
		kge::math::Vector ActorDynamicX::getAngularMomentum()
		{
			NxVec3 pVec = localActor->getAngularMomentum();
			return kge::math::Vector( pVec.x , pVec.y , pVec.z );
		}

		//----------------------------------------------------------
		// Get Matrix
		//----------------------------------------------------------
		kge::math::Matrix	ActorDynamicX::getMatrix()
		{
			kge::math::Matrix mat;
			float glMat[16];

			localActor->getGlobalPose().getColumnMajor44(glMat);

			memcpy( &mat.m_fMat , &glMat[0] , 64  );
			return mat;
		}

		//----------------------------------------------------------
		//Get Position
		//----------------------------------------------------------
		kge::math::Vector 	ActorDynamicX::getPosition()
		{
			NxVec3 pVec = localActor->getGlobalPosition();
			return kge::math::Vector( pVec.x , pVec.y , pVec.z );
		}

		//----------------------------------------------------------
		// Set Actor Number
		//----------------------------------------------------------
		void ActorDynamicX::setActorNumber( unsigned int i )
		{
			actorNumber = i;
		}

		//----------------------------------------------------------
		// Get Actor Number
		//----------------------------------------------------------
		unsigned int ActorDynamicX::getActorNumber()
		{
			return actorNumber;
		}

		//----------------------------------------------------------
		// AddForceAtLocalPos
		//----------------------------------------------------------
		void ActorDynamicX::addForce( kge::math::Vector *pos )
		{
			localActor->addForceAtLocalPos( NxVec3( pos->x , pos->y , pos->z ) , NxVec3( 0.0f , 0.0f , 0.0f ) , NX_IMPULSE );
		}

		//----------------------------------------------------------
		// Set Actor Name
		//----------------------------------------------------------
		void ActorDynamicX::setName( const char *name )
		{
			localActor->setName( name );
		}

		//----------------------------------------------------------
		// Set Actor Name
		//----------------------------------------------------------
		const char* ActorDynamicX::getName()
		{
			return localActor->getName();
		}

		//----------------------------------------------------------
		//! Is Dynamic Actor
		//----------------------------------------------------------
		bool ActorDynamicX::isDynamic()
		{
			return localActor->isDynamic();
		}

		//----------------------------------------------------------
		//! Caculate Kinematic ( rotational and translational ) Energy of Actor
		//----------------------------------------------------------
		float ActorDynamicX::computeKineticEnergy()
		{
			if( localActor->isDynamic() )
				return localActor->computeKineticEnergy();
			else
				return -1;
		}

		//----------------------------------------------------------
		//! Sets a dynamic actor's orientation in the world
		//----------------------------------------------------------
		void ActorDynamicX::setGlobalOrientation( const kge::math::Matrix* mat )
		{
			NxVec3	pVec1(mat->_11 , mat->_12 , mat->_13 );
			NxVec3	pVec2(mat->_21 , mat->_22 , mat->_23 );
			NxVec3	pVec3(mat->_31 , mat->_32 , mat->_33 );

			NxMat33 pMat( pVec1 , pVec2 , pVec3 );

			localActor->setGlobalOrientation( pMat );
		}

		//----------------------------------------------------------
		//! The moveGlobal* calls serve to move kinematically controlled dynamic actors through the game world.
		//----------------------------------------------------------
		void ActorDynamicX::moveGlobalPose( const kge::math::Matrix* mat )
		{
		}

		//----------------------------------------------------------
		//! the moveglobal* calls serve to move kinematically controlled dynamic actors through the game world.
		//----------------------------------------------------------
		void ActorDynamicX::moveGlobalPosition( const kge::math::Vector* vec )
		{
			NxVec3 pVec( vec->x , vec->y , vec->z );
			localActor->moveGlobalPosition( pVec );
		}

		//----------------------------------------------------------
		//! Set the pose of the center of mass relative to the actor.
		//----------------------------------------------------------
		void ActorDynamicX::setCenterOfMass( const kge::math::Vector* vec)
		{
			if( !localActor->isDynamic() )
				return;

			NxVec3 pVec( vec->x , vec->y , vec->z );
			localActor->setCMassGlobalPosition( pVec );
		}

		//----------------------------------------------------------
		//! Forces the actor to sleep
		//----------------------------------------------------------
		void ActorDynamicX::putToSleep()
		{
			localActor->putToSleep();
		}

		//----------------------------------------------------------
		//! Set Material
		//----------------------------------------------------------
		void ActorDynamicX::setMaterial( Material* mat )
		{
			localActor->getShapes()[0]->setMaterial( mat->getMaterialIndex() );
			m_pMaterial = mat;
		}

		//----------------------------------------------------------
		//! Get Material
		//----------------------------------------------------------
		Material* ActorDynamicX::getMaterial( void )
		{
			return m_pMaterial;
		}

		//----------------------------------------------------------
		//! Add Shape to Actor
		//----------------------------------------------------------
		void ActorDynamicX::addShape( math::Shape *shape , CollitionType ct , const kge::math::Vector* vec )
		{
			math::Vector point;

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

				nxBoxShapeDesc.localPose.t.set( vec->x , vec->y , vec->z );

				if( !nxBoxShapeDesc.isValid() )
				{
					io::Logger::Log( "addShape : Invalid Box Shape Desc\n" );
					return;
				}

				localActor->createShape( nxBoxShapeDesc );
			}
			else if( shape->getType() == math::KGE_SHAPE_SPHERE )
			{
				NxSphereShapeDesc nxSphereShapeDesc;

				math::Sphere *aa = (math::Sphere*)(shape);
				nxSphereShapeDesc.radius = aa->GetRadius();

				if( ct != ECTP_TRIGGER_DISABLE )
					nxSphereShapeDesc.shapeFlags |= ct;

				nxSphereShapeDesc.localPose.t.set( vec->x , vec->y , vec->z );

				if( !nxSphereShapeDesc.isValid() )
				{
					io::Logger::Log( "addShape : Invalid Sphere Shape Desc\n" );
					return;
				}

				localActor->createShape( nxSphereShapeDesc );
			}
			else if( shape->getType() == math::KGE_SHAPE_CAPSULE )
			{
			}
		}

		//----------------------------------------------------------
		//! Add Shape to Actor
		//----------------------------------------------------------
		void ActorDynamicX::addShape(kge::math::KGE_SHAPES shape , CollitionType ct , const kge::math::Vector* vec )
		{
			if( shape == math::KGE_SHAPE_AABB )
			{
				NxBoxShapeDesc nxBoxShapeDesc;

				math::Vector point;
				point = (localKgeObj->GetAxisAlignedBoundingBox()->m_vMax - localKgeObj->GetAxisAlignedBoundingBox()->m_vMin);
				point.x = fabs( point.x );
				point.y = fabs( point.y );
				point.z = fabs( point.z );
				point /= 2.0f;

				nxBoxShapeDesc.dimensions.set( point.x , point.y , point.z );
				nxBoxShapeDesc.localPose.t.set( vec->x , vec->y , vec->z );

				if( ct != ECTP_TRIGGER_DISABLE )
					nxBoxShapeDesc.shapeFlags |= ct;

				if( !nxBoxShapeDesc.isValid() )
				{
					io::Logger::Log( "addShape : inValid Box Shape Desc\n" );
					return;
				}

				localActor->createShape( nxBoxShapeDesc );
			}
			else if( shape == math::KGE_SHAPE_SPHERE )
			{
				NxSphereShapeDesc nxSphereShapeDesc;

				math::Vector point;

				point = (localKgeObj->GetAxisAlignedBoundingBox()->m_vMax - localKgeObj->GetAxisAlignedBoundingBox()->m_vMin);
				nxSphereShapeDesc.radius = point.x;

				nxSphereShapeDesc.localPose.t.set( vec->x , vec->y , vec->z );

				if( ct != ECTP_TRIGGER_DISABLE )
					nxSphereShapeDesc.shapeFlags |= ct;

				if( !nxSphereShapeDesc.isValid() )
				{
					io::Logger::Log( "addShape : inValid Sphere Shape Desc\n" );
					return;
				}

				localActor->createShape( nxSphereShapeDesc );
			}

		}

	} // ph
} // kge

#endif // KGE_USE_PHYSX

