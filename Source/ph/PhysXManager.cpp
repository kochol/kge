// File name: PhysXManager.cpp
// Des: This class is for managing the physics dynamics with PhysX engine.
// Date: 31/6/1388
// Programmer: Hadi Robati (hadirobati), Ali Akbar Mohammadi(Kochol)

#include "../../Headers/ph/PhysXManager.h"

#ifdef KGE_USE_PHYSX

#include "../../Include/io/Logger.h"
#include "../../Include/sn/TileTerrain.h"
#include "../../Headers/ph/TileTerrainShapeX.h"
#include "../../Headers/ph/actorDynamicX.h"
#include "../../Headers/ph/actorStaticX.h"
#include "../../Headers/ph/PlaneX.h"
#include "../../Headers/ph/MaterialX.h"
#include "../../Headers/ph/UserNotifyX.h"
#include "../../Headers/ph/TriggerReportX.h"
#include "../../Headers/ph/RaycastX.h"
#include "../../Headers/ph/SphericalJointX.h"
#include "../../Headers/ph/RevoluteJointX.h"
#include "../../Headers/ph/PrismaticJointX.h"
#include "../../Headers/ph/CylindricalJointX.h"
#include "../../Headers/ph/FixedJointX.h"
#include "../../Headers/ph/DistanceJointX.h"

#pragma comment(lib, "PhysXLoader.lib")
namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		PhysXManager::PhysXManager(): m_pSDK(NULL), m_pScene(NULL)
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		PhysXManager::~PhysXManager()
		{
			if (m_pSDK)
				m_pSDK->release();

			if (m_pTimer)
				m_pTimer->DecRef();

		} // Destructor

		//------------------------------------------------------------------------------------
		// Init the physics engine
		//------------------------------------------------------------------------------------
		bool PhysXManager::Init( float gravity/*=-98.1f */, float skinWidth /*= 0.025f */, 
			float timing /*= 0.016 */, bool selfTimeCalc/*=true */ ,
			Material *defaultMaterial /* NULL */ ,
			bool isConnectToRemoteDebugger /*=true*/ )
		{
			//Create PhysX SDK.
			m_pSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, NULL, m_xSDKDesc, NULL );
			if( m_pSDK == NULL )
			{
				io::Logger::Log(io::ELM_Error, "Can't Create PhysX SDK." );
				return false;
			}

			//Set Skin Width Parameter
			m_pSDK->setParameter(NX_SKIN_WIDTH, skinWidth);

			if( isConnectToRemoteDebugger == true )
				m_pSDK->getFoundationSDK().getRemoteDebugger()->connect ("localhost", 5425);

			//Set Scene Gravity
			m_xSceneDesc.gravity = NxVec3( 0.0f , gravity , 0.0f );
			m_pScene = m_pSDK->createScene( m_xSceneDesc );
			if( m_pScene == NULL )
			{
				io::Logger::Log(io::ELM_Error, "Can't Create PhysX Scene." );
				return false;
			}

			//Set timing of Scene
			m_pScene->setTiming( timing , 8, NX_TIMESTEP_FIXED );

			if( defaultMaterial == NULL )
			{
				NxMaterialDesc nxDefaultMaterialDesc;

				nxDefaultMaterialDesc.setToDefault();
				//Set Material
				NxMaterial* mat = m_pScene->getMaterialFromIndex(0);
				mat->loadFromDesc( nxDefaultMaterialDesc );
			}
			else
			{
				NxMaterial* a;

				a = m_pScene->getMaterialFromIndex( 0 );
				a->setRestitution( defaultMaterial->getRestitution() );
				a->setDynamicFriction( defaultMaterial->getDynamicFriction() );
				a->setDynamicFrictionV( defaultMaterial->getDynamicFrictionV() );
				a->setStaticFriction( defaultMaterial->getStaticFriction() );
				a->setStaticFrictionV( defaultMaterial->getStaticFrictionV() );
				//a->setDirOfAnisotropy( 
			}

			if( selfTimeCalc==true )
			{
				m_bIsSelfTimeCalc = true;
				m_pTimer = new kge::core::Timer();
			}
			else
				m_bIsSelfTimeCalc = false;

			m_iDynamicActorCounter = 0;

			//User Notify Init
			m_pUserNotifyX = new UserNotifyX();

			m_pScene->setUserNotify( m_pUserNotifyX );

			return true;

		} // Init

		//------------------------------------------------------------------------------------
		// Creates a tile terrain shape for doing dynamics.
		//------------------------------------------------------------------------------------
		TileTerrainShape*	PhysXManager::CreateTileTerrain( sn::TileTerrain* terrain )
		{
			TileTerrainShape* ter = new TileTerrainShapeX(this, terrain);

			return ter;

		} // CreateTileTerrain

		//------------------------------------------------------------------------------------
		// Creates a PhysX Dynamic Actor.
		//------------------------------------------------------------------------------------
		ActorDynamic* PhysXManager::CreateDynamicActor( sn::SceneNode* kgeObj , math::KGE_SHAPES shape , BodyFlag bf , Material* material , float mass )
		{
			ActorDynamic* tempActor = new ActorDynamicX( this , kgeObj , shape , bf , material , mass , &m_iDynamicActorCounter );

			m_pDynamicActors.push_back( tempActor );

			return tempActor;
		} // CreateActor

		//------------------------------------------------------------------------------------
		// Creates a PhysX Dynamic Actor.
		//------------------------------------------------------------------------------------
		ActorDynamic* PhysXManager::CreateDynamicActor( sn::SceneNode* kgeObj , math::Shape* shape , BodyFlag bf , Material* material , float mass )
		{
			ActorDynamic* tempActor = new ActorDynamicX( this , kgeObj , shape , bf , material , mass , &m_iDynamicActorCounter );

			m_pDynamicActors.push_back( tempActor );

			return tempActor;
		} // CreateActor

		//------------------------------------------------------------------------------------
		// Creates a PhysX Static Actor.
		//------------------------------------------------------------------------------------
		ActorStatic* PhysXManager::CreateStaticActor( sn::SceneNode* kgeObj , math::KGE_SHAPES shape , CollitionType ct )
		{
			ActorStatic* tempActor = new ActorStaticX( this , kgeObj , shape , ct , &m_iStaticActorCounter );

			m_pStaticActor.push_back( tempActor );

			return tempActor;
		}

		//------------------------------------------------------------------------------------
		// Creates a PhysX Static Actor.
		//------------------------------------------------------------------------------------
		ActorStatic* PhysXManager::CreateStaticActor( sn::SceneNode* kgeObj , math::Shape* shape , CollitionType ct )
		{
			ActorStatic* tempActor = new ActorStaticX( this , kgeObj , shape , ct , &m_iStaticActorCounter );

			m_pStaticActor.push_back( tempActor );

			return tempActor;
		}

		//------------------------------------------------------------------------------------
		// Release a PhysX Actor.
		//------------------------------------------------------------------------------------
		bool PhysXManager::ReleaseActor( ph::ActorDynamic *actor )
		{

			//((ph::ActorDynamicX*)(actor))->~ActorDynamicX();
			//m_pScene->releaseActor(*((ph::ActorDynamicX*)(actor))->getPhActor());
			for(unsigned int i = 0 ; i < m_pDynamicActors.size(); ++i)
			{
				if( m_pDynamicActors[i]->getActorNumber() == actor->getActorNumber() )
				{
					m_pDynamicActors.erase(m_pDynamicActors.begin() + i );	
					delete actor;
					return true;
				}
			}
			
			return false;
		}

		//------------------------------------------------------------------------------------
		//! Create a Basic PhysX Plane
		//------------------------------------------------------------------------------------
		Plane*		PhysXManager::CreatePlane( sn::SceneNode* kgePlane ,  kge::math::Vector *normal )
		{
			Plane* tempPlane = new PlaneX( this , kgePlane , normal );

			m_pPlane.push_back( tempPlane );

			return tempPlane;
		}
		//------------------------------------------------------------------------------------
		//! Create a Basic PhysX Plane
		//------------------------------------------------------------------------------------ 
		Plane*		PhysXManager::CreatePlane( math::Vector*  pos , math::Vector*  normal , float d )
		{
			Plane* tempPlane = new PlaneX( this , pos , normal , d );

			m_pPlane.push_back( tempPlane );

			return tempPlane;
		}

		//------------------------------------------------------------------------------------
		//! Physic Calculate
		//------------------------------------------------------------------------------------
		bool	PhysXManager::Calculate( float time )
		{
			// Physx Simulate
			if( time == 0.0 )
			{
				if( m_bIsSelfTimeCalc == true )
					m_pScene->simulate( m_pTimer->GetTime() / 500.0f );
				else
					return false;
			}
			else
				m_pScene->simulate( time );

			m_pScene->flushStream();
			m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,true);
			return true;
		}

		//------------------------------------------------------------------------------------
		//! physc Update KGE Scene
		//------------------------------------------------------------------------------------
		bool	PhysXManager::UpdateKgeScene( void )
		{
			for( std::vector<ActorDynamic*>::iterator it = m_pDynamicActors.begin() ;
				it != m_pDynamicActors.end() ; 
				it++ )
			{
				if( *it )
					UpdateKgeSceneForOneActor( (*it) );
			}

			return true;
		}

		//------------------------------------------------------------------------------------
		//! physX Update KGE Scene for One Actor Only
		//------------------------------------------------------------------------------------
		bool	PhysXManager::UpdateKgeSceneForOneActor( ph::ActorDynamic *actor )
		{
			if( actor == NULL )
				return false;

			kge::math::Matrix *kPos = actor->getKgeObj()->GetAbsoluteMatrix();
			kge::math::Matrix pPos = actor->getMatrix();

			memcpy( &kPos->m_fMat , &pPos.m_fMat , 64 );

			return true;
		}

		//------------------------------------------------------------------------------------
		//! Create physX Material
		//------------------------------------------------------------------------------------
		Material* PhysXManager::CreateMaterial( void )
		{
			Material* tempMat = new MaterialX( this );

			return tempMat;
		}

		//------------------------------------------------------------------------------------
		//! Create physX Material
		//------------------------------------------------------------------------------------
		Material* PhysXManager::CreateMaterial(float Restitution, float staticFriction, float dynamicFriction, float dynamicFrictionV, float staticFrictionV, kge::math::Vector *dir)
		{
			Material* tempMat = new MaterialX( this , Restitution , staticFriction , dynamicFriction , dynamicFrictionV , staticFrictionV , dir );

			return tempMat;
		}

		//------------------------------------------------------------------------------------
		//! Set User Notify
		//------------------------------------------------------------------------------------
		void	PhysXManager::SetUserNotify( UserNotify *pUserNotify )
		{
			UserNotifyX* pUserNotifyX = new UserNotifyX();

			//Set Sleep Function Pointer
			pUserNotifyX->setSleepFuncPtr( pUserNotify->getSleepFunc() );

			//Set Wake Up Function Pointer
			pUserNotifyX->setWakeUpFuncPtr( pUserNotify->getWakeUpFunc() );

		m_pScene->setUserNotify( pUserNotifyX );
	}
	
	//------------------------------------------------------------------------------------
	//! Set User Trigger Report
	//------------------------------------------------------------------------------------
	void PhysXManager::SetTriggerReport	( TriggerReport *pTriggerReport )
	{
		TriggerReportX* pTriggerReportX = new TriggerReportX();

		//Set Trigger Function Pointer
		pTriggerReportX->setTriggerFunc( pTriggerReport->getTriggerFunc() );

		m_pScene->setUserTriggerReport( pTriggerReportX );
	}

	//------------------------------------------------------------------------------------
	//! Set Raycast All Shape
	//------------------------------------------------------------------------------------
	void PhysXManager::RaycatAllShapes( math::Vector *dir , math::Vector *orig , core::Functor1<bool,RaycastReport*>* CallBackFunc , ERCP_Raycast shapeType , int group , float maxDis )
	{
		if( dir == NULL || orig == NULL  )
		{
			kge::io::Logger::Log( "RaycastAllShapes : dir or orig Pointer is Null!!!\n" );
			return;
		}

		if( CallBackFunc == NULL  )
		{
			kge::io::Logger::Log( "RaycastAllShapes : CallBackFunc Function Pointer is Null!!!\n" );
			return;
		}

		NxRay ray;
		ray.dir = NxVec3( dir->x , dir->y , dir->z );
		ray.orig= NxVec3( orig->x , orig->y , orig->z );
		ray.dir.normalize();

		RaycastX rayCastX;
		rayCastX.setRaycastReportFunc( CallBackFunc );

		m_pScene->raycastAllShapes( ray , rayCastX , (NxShapesType)(shapeType) , group , maxDis );
	}

	//------------------------------------------------------------------------------------
	//! Set Raycast Any Shape
	//------------------------------------------------------------------------------------
	bool PhysXManager::RaycastAnyShape( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType , int group , float maxDis )
	{
		if( dir == NULL || orig == NULL )
		{
			kge::io::Logger::Log( "RaycastAnyShapes : dir or orig Pointer is Null!!!\n" );
			return false;
		}

		NxRay ray;
		ray.dir = NxVec3( dir->x , dir->y , dir->z );
		ray.orig= NxVec3( orig->x , orig->y , orig->z );
		ray.dir.normalize();

		return (m_pScene->raycastAnyShape( ray , (NxShapesType)(shapeType) , group , maxDis )); 
	}

	//------------------------------------------------------------------------------------
	//! Set Raycast Closest Shape
	//------------------------------------------------------------------------------------
	RaycastReport* PhysXManager::RaycastClosestShape	( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType , int group , float maxDis )
	{
		RaycastReport* report = new RaycastReport();
		NxRaycastHit rayHit;

		NxRay ray;
		ray.dir = NxVec3( dir->x , dir->y , dir->z );
		ray.orig= NxVec3( orig->x , orig->y , orig->z );
		ray.dir.normalize();

		NxShape* shape;
		shape = m_pScene->raycastClosestShape( ray , (NxShapesType)(shapeType) , rayHit , group , maxDis );

		report->m_pActor	= ((ActorDynamic*)(shape->getActor().userData));
		report->m_pDistance = rayHit.distance;
		report->m_pFaceID	= rayHit.faceID;
		report->m_pFlags	= rayHit.flags;
		report->m_pInternalFaceID = rayHit.internalFaceID;
		report->m_pMaterial = (report->m_pActor)->getMaterial();
		report->m_pU		= rayHit.u;
		report->m_pV		= rayHit.v;
		report->m_pWorldImpact	= &kge::math::Vector( rayHit.worldImpact.x , rayHit.worldImpact.y , rayHit.worldImpact.z );
		report->m_pWorldNormal = &kge::math::Vector( rayHit.worldNormal.x , rayHit.worldNormal.y , rayHit.worldNormal.z );

		return report;
	}

	//------------------------------------------------------------------------------------
	//! Create Spherical Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreateSphericalJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor )
	{
		SphericalJoint* sj = new SphericalJointX( this , a1 , a2 , Anchor );
		return ((Joint*)(sj));
	}

	//------------------------------------------------------------------------------------
	//! Create Revolute Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreateRevoluteJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor )
	{
		RevoluteJoint* rj = new RevoluteJointX( this , a1 , a2 , Anchor );
		return ((Joint*)(rj));
	}
	
	//------------------------------------------------------------------------------------
	//! Create Revolute Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreatePrismaticJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis )
	{
		PrismaticJoint* pj = new PrismaticJointX( this , a1 , a2 , Anchor , axis );
		return ((Joint*)(pj));
	}

	//------------------------------------------------------------------------------------
	//! Create Cylindrical Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreateCylindricalJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis )
	{
		CylindricalJoint* cj = new CylindricalJointX( this , a1 , a2 , Anchor , axis );
		return ((Joint*)(cj));
	}

	//------------------------------------------------------------------------------------
	//! Create Cylindrical Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreateFixedJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis )
	{
		FixedJoint* fj = new FixedJointX( this , a1 , a2 , Anchor , axis );
		return ((Joint*)(fj));
	}

	//------------------------------------------------------------------------------------
	//! Create Distance Joint
	//------------------------------------------------------------------------------------
	Joint* PhysXManager::CreateDistanceJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , DistanceJointFlag flags , unsigned int minDis , unsigned int maxDis )
	{
		DistanceJoint *dj = new DistanceJointX( this , a1 , a2 , Anchor , flags , minDis , maxDis );
		return ((Joint*)(dj));
	}
		

} // ph

} // kge

#endif // KGE_USE_PHYSX
