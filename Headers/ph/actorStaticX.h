// File name: ActorX.h
// Des: This class is for creating PhysX Actor
// Date: 12/10/1388
// Programmer: Hadi Robati (hadirobati)

#include "PhysXManager.h"
#include "../../include/ph/physicType.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_ACTOR_STATIC_X_H
#define KGE_ACTOR_STATIC_X_H

namespace kge
{
	namespace ph
	{
		class ActorStaticX : public ActorStatic
		{
		public:
			//! Constructor
			ActorStaticX					( PhysXManager *phMan , kge::sn::SceneNode *kgeObj , math::KGE_SHAPES shape ,  CollitionType ct ,  unsigned int *staticActorCounter );
			//! Constructor
			ActorStaticX					( PhysXManager *phMan , kge::sn::SceneNode *kgeObj , math::Shape* shape , CollitionType ct ,  unsigned int *staticActorCounter );

			//! Destructor
			~ActorStaticX					();

			//! Get Kge Object
			//kge::sn::SceneNode* getKgeObj	( void ) { return localKgeObj; }
		protected:
			PhysXManager*	localPhysXManager;
			NxActor*		localActor;
			unsigned int	actorNumber;
		};
	} // ph
} // kge

#endif // KGE_ACTOR_STATIC_X_H
#endif // KGE_USE_PHYSX