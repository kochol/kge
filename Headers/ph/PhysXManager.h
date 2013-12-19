// File name: PhysXManager.h
// Des: This class is for managing the physics dynamics with PhysX engine.
// Date: 31/6/1388
// Programmer: Hadi Robati (hadirobati), Ali Akbar Mohammadi(Kochol)

#include "../../Include/kgedef.h"
#include "../../Include/kge.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PHYSXMANAGER_H
#define KGE_PHYSXMANAGER_H

#include "../../Include/ph/PhysManager.h"

// To use physx you need add physx SDK search paths.
#include <NxPhysics.h>

//??????????????????????????????????????????
#include "UserNotifyX.h"

namespace kge
{
namespace ph
{	 
	class PhysXManager: public PhysManager
	{
	public:

		//! Constructor
		PhysXManager();

		//! Destructor
		~PhysXManager();

		//! Init the physics engine
		bool Init(float	gravity , float skinWidth  , float timing ,
				  bool selfTimeCalc , Material *defaultMaterial , bool isConnectToRemoteDebugger );

		NxScene*			getPhysxScene(){ return m_pScene;	}
		NxPhysicsSDK*		getPhysxSDK	 (){ return m_pSDK;		}

		//! Creates a tile terrain shape for doing dynamics.
		TileTerrainShape*	CreateTileTerrain	(sn::TileTerrain* terrain);

		//! Creates a Basic Physx Actor 
		ActorDynamic*		CreateDynamicActor	( sn::SceneNode* kgeObj , math::KGE_SHAPES shape, BodyFlag bf , Material* material , float mass );
		//! Creates a Basic Physx Actor 
		ActorDynamic*		CreateDynamicActor	( sn::SceneNode* kgeObj , math::Shape* shape , BodyFlag bf , Material* material , float mass );

		//! Creates a Basic Static Actor
		ActorStatic*		CreateStaticActor	( sn::SceneNode* kgeObj	, math::KGE_SHAPES shape , CollitionType ct );
		//! Creates a Basic Static Actor
		ActorStatic*		CreateStaticActor	( sn::SceneNode* kgeObj	, math::Shape* shape , CollitionType ct );

		//! Release PhysX Actor
		bool				ReleaseActor		( ph::ActorDynamic *actor );

		//! Craete a Basic PhysX Plane
		Plane*				CreatePlane			( sn::SceneNode* kgePlane ,  kge::math::Vector *normal );
		Plane*				CreatePlane			( math::Vector*  pos , math::Vector*  normal , float d );

		//! Physc Calculate
		bool				Calculate			( float time = 0.0f );

		//! physc Update KGE Scene
		bool				UpdateKgeScene		( void );

		//! Physic Update KGE Scene for One Actor
		bool				UpdateKgeSceneForOneActor( ph::ActorDynamic *actor );

		//! PhysX Create Material
		Material*			CreateMaterial		( float Restitution , float staticFriction , float dynamicFriction , 
														float dynamicFrictionV, float staticFrictionV , kge::math::Vector *dir );
		//! PhysX Create Material
		Material*			CreateMaterial		( void );

		//! Set User Notify( Sleep and Wakrup )
		void				SetUserNotify		( UserNotify *pUserNotify );

		//! Set Trigger Report( OnTrigger )
		void				SetTriggerReport	( TriggerReport *pTriggerReport );

		//! Set Raycast All Shape
		void				RaycatAllShapes		( math::Vector *dir , math::Vector *orig , core::Functor1<bool,RaycastReport*>* CallBackFunc , ERCP_Raycast shapeType , int group , float maxDis ); 

		//! Set Raycast Any Shape
		bool				RaycastAnyShape		( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType , int group , float maxDis );

		//! Set Raycast Closest Shape
		RaycastReport*		RaycastClosestShape	( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType , int group , float maxDis );

		//! Create Spherical Joint
		Joint*				CreateSphericalJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor );

		//! Create Revolute Joint
		Joint*				CreateRevoluteJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor );

		//! Create Prismatic Joint
		Joint*				CreatePrismaticJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis );

		//! Create Cylindrical joint
		Joint*				CreateCylindricalJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis );

		//! Create Fixed joint
		Joint*				CreateFixedJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis );

		//! Create Distance Joint
		Joint*				CreateDistanceJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , DistanceJointFlag flags , unsigned int minDis , unsigned int maxDis );

	protected:
		std::vector<Plane*>			m_pPlane;
		std::vector<ActorDynamic*>	m_pDynamicActors;
		std::vector<ActorStatic*>	m_pStaticActor;
		NxPhysicsSDKDesc			m_xSDKDesc;		// PhysX SDK Description
		NxPhysicsSDK*				m_pSDK;			// PhysX SDK
		NxSceneDesc  				m_xSceneDesc;	// PhysX Scene Description
		NxScene*					m_pScene;		// PhysX Scene
		UserNotifyX*				m_pUserNotifyX; // PhysX User Notify( Sleep , Wakeup , Joint )
		
	}; // PhysXManager

} // ph

} // kge

#endif // KGE_PHYSXMANAGER_H

#endif // KGE_USE_PHYSX
