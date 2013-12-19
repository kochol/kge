// File name: PhysManager.h
// Des: This class is an interface for managing the physics dynamics
// Date: 31/6/1388
// Programmer: Hadi Robati (hadirobati), Ali Akbar Mohammadi(Kochol)

#ifndef KGE_PHYSMANAGER_H
#define KGE_PHYSMANAGER_H


#include "TileTerrainShape.h"
#include "Actor.h"
#include "ActorDynamic.h"
#include "ActorStatic.h"
#include "Plane.h"
#include "Material.h"
#include "UserNotify.h"
#include "TriggerReport.h"
#include "RaycastReport.h"
#include "../math/Shape.h"
#include "../core/Timer.h"
#include "../core/Functor.h"
#include "physicType.h"
#include "Joint.h"
#include "SphericalJoint.h"
#include "RevoluteJoint.h"
#include "PrismaticJoint.h"
#include "CylindricalJoint.h"
#include "FixedJoint.h"
#include "DistanceJoint.h"

class NxPhysicsSDK;

namespace kge
{
namespace sn
{
	class TileTerrain;
} // sn

namespace ph
{ 
class PhysManager
{
public:

	//! Constructor
	PhysManager(): m_pTimer(NULL) { /* Nothing to do */ }

	//! Destructor
	virtual						~PhysManager() {}

	//! Init the physics engine
	virtual bool				Init				(	float	gravity=-9.8f , float skinWidth = 0.025f , float timing = 0.016 ,
														bool selfTimeCalc=true  , Material *defaultMaterial = NULL , 
														bool isConnectToRemoteDebugger = true ) = 0;

	//! Creates a tile terrain shape for doing dynamics.
	virtual TileTerrainShape*	CreateTileTerrain	( sn::TileTerrain* terrain ) = 0;

	//! Creates a Basic Dynamic Actor
	virtual ActorDynamic*		CreateDynamicActor	( sn::SceneNode* kgeObj , math::KGE_SHAPES shape = math::KGE_SHAPE_AABB , BodyFlag bf = EBFP_NORMAL , Material* material = NULL , float mass = 1.0f ) = 0;
	virtual ActorDynamic*		CreateDynamicActor	( sn::SceneNode* kgeObj , math::Shape* shape , BodyFlag bf = EBFP_NORMAL , Material* material = NULL , float mass = 1.0f ) = 0;

	//! Creates a Basic Static Actor
	virtual ActorStatic*		CreateStaticActor	( sn::SceneNode* kgeObj	, math::KGE_SHAPES shape = math::KGE_SHAPE_AABB ,  CollitionType ct = ECTP_TRIGGER_DISABLE ) = 0;
	virtual ActorStatic*		CreateStaticActor	( sn::SceneNode* kgeObj	, math::Shape* shape , CollitionType ct = ECTP_TRIGGER_DISABLE ) = 0;

	//! Release Actor
	virtual bool				ReleaseActor		( ph::ActorDynamic *actor ) = 0;

	//! Craete a Basic Plane
	virtual Plane*				CreatePlane			( sn::SceneNode* kgePlane ,  math::Vector *normal ) = 0;
	virtual Plane*				CreatePlane			( math::Vector*  pos , math::Vector*  normal , float d ) = 0;

	//! Physc Calculate
	virtual bool				Calculate			( float time = 0.0f ) = 0;

	//! physc Update KGE Scene
	virtual bool				UpdateKgeScene		( void ) = 0;

	virtual bool				UpdateKgeSceneForOneActor( ph::ActorDynamic *actor ) = 0;
	
	//! Physic Create Material
	virtual Material*			CreateMaterial		( float Restitution , float staticFriction , float dynamicFriction , 
														float dynamicFrictionV, float staticFrictionV , kge::math::Vector *dir ) = 0;
	//! Physic Create Material
	virtual Material*			CreateMaterial		( void ) = 0;
	
	//! Set User Notify User Report , You Most set the Pointer of  UserNotify Class then Pass it to below Function
	virtual void				SetUserNotify		( UserNotify *pUserNotify ) = 0;

	//! Set Trigger Report , You Most set the Pointer To Function of TriggerReport Class then pass it to below Function
	virtual void				SetTriggerReport	( TriggerReport *pTriggerReport ) = 0;
	
	//! Raycast All Shapes in Scene
	virtual void				RaycatAllShapes		( math::Vector *dir , math::Vector *orig , core::Functor1<bool,RaycastReport*>* CallBackFunc , ERCP_Raycast shapeType = ERCP_ALL_SHAPES , int group = 0xFFFFFFFF , float maxDis = 3.402823466e+38F ) = 0; 

	//! Return true if Ray any Shape
	virtual bool				RaycastAnyShape		( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType = ERCP_ALL_SHAPES , int group = 0xFFFFFFFF , float maxDis = 3.402823466e+38F ) = 0;

	//! return Closest Shape( Actor ) to ray Point( orig )
	virtual RaycastReport*		RaycastClosestShape	( math::Vector *dir , math::Vector *orig , ERCP_Raycast shapeType = ERCP_ALL_SHAPES , int group = 0xFFFFFFFF , float maxDis = 3.402823466e+38F ) = 0;

	//! Create Spherical Joint ( a2 can set to NULL )
	virtual Joint*				CreateSphericalJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor ) = 0;

	//! Create Revolute Joint ( a2 Can Set To Null )
	virtual Joint*				CreateRevoluteJoint( Actor *a1 , Actor *a2 , math::Vector *Anchor ) = 0;

	//! Create Perismatic Joint
	virtual Joint*				CreatePrismaticJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis = NULL ) = 0; 

	//! Create Cylindrical joint
	virtual Joint*				CreateCylindricalJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis = NULL ) = 0;

	//! Create Fixed joint
	virtual Joint*				CreateFixedJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , math::Vector *axis = NULL ) = 0;

	//! Create Distance Joint
	virtual Joint*				CreateDistanceJoint( Actor *a1 ,  Actor *a2 , math::Vector *Anchor , DistanceJointFlag flags = EDJF_DISABLE ,  unsigned int minDis = 0 , unsigned int maxDis = 0 ) = 0;

protected:

	core::Timer		* m_pTimer;
	bool			  m_bIsSelfTimeCalc;
	unsigned int	  m_iDynamicActorCounter;
	unsigned int	  m_iStaticActorCounter;

}; // PhysManager

} // ph

} // kge

#endif // KGE_PHYSMANAGER_H
