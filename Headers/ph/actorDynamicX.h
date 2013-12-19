// File name: ActorX.h
// Des: This class is for creating PhysX Actor
// Date: 12/10/1388
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_ACTOR_DYANAMIC_X_H
#define KGE_ACTOR_DYANAMIC_X_H

namespace kge
{
	namespace ph
	{
		class ActorDynamicX : public ActorDynamic
		{
		public:
			//! Constructor
			ActorDynamicX							( PhysXManager *pMan , sn::SceneNode *kgeObj , math::KGE_SHAPES shape , BodyFlag bf ,Material* material , float mass , unsigned int *actorCounter );
			//! Constructor
			ActorDynamicX							( PhysXManager *pMan , sn::SceneNode *kgeObj , math::Shape* shape , BodyFlag bf , Material* material , float mass , unsigned int *actorCounter );

			//! Destructor
			~ActorDynamicX							(		);

			//! Set Position
			void				setPosition			( kge::math::Vector *pos );

			//! Set Actor Number
			void				setActorNumber		( unsigned int i );

			//! Get Actor Number
			unsigned int		getActorNumber		( void			 );

			//! Get Position
			kge::math::Vector 	getPosition			( void	);

			//! Get Matrix
			kge::math::Matrix	getMatrix			( void	);

			//! Set Mass
			void				setMass				( float mass );

			//! Get Mass
			float				getMass				( void );

			//! Recomputes a dynamic actor's mass properties from its shapes
			bool				updateMassFromShapes( float density , float totalMass );

			//! Set Angular Damping
			void				setAngularDamping	( float angularDamping );

			//! Retrieves the angular damping coefficient.
			float				getAngularDamping	( void );

			//! Set linear Damping		
			void				setLinearDamping	( float linearDamping );

			//! Retrieves the linear damping coefficient.
			float				getLinearDamping	( void );

			//! Sets the linear velocity of the actor. 
			void				setLinearVelocity	( const kge::math::Vector *linVel ); 
			
			//! Sets the angular velocity of the actor. 
			void				setAngularVelocity	( const kge::math::Vector *angVel ); 
			
			//! Retrieves the linear velocity of an actor. 
			kge::math::Vector	getLinearVelocity	( void ); 
			
			//! Retrieves the angular velocity of the actor.  
			kge::math::Vector	getAngularVelocity	( void ); 
			
			//! Lets you set the maximum angular velocity permitted for this actor.  
			void				setMaxAngularVelocity(float maxAngVel); 
			
			//! Retrieves the maximum angular velocity permitted for this actor.   
			float				getMaxAngularVelocity( void );

			//! Sets the linear momentum of the actor. 
			void				setLinearMomentum	( const kge::math::Vector* linMoment); 
			
			//! Sets the angular momentum of the actor. 
			void				setAngularMomentum	( const kge::math::Vector* angMoment); 
			  
			//! Retrieves the linear momentum of an actor.  
			kge::math::Vector 	getLinearMomentum	( void ); 
			  
			//! Retrieves the angular momentum of an actor. 
			kge::math::Vector	getAngularMomentum	( void );

			//!Get Kge Object
			kge::sn::SceneNode* getKgeObj			( void ) { return localKgeObj; }

			//! get Physx Actor
			NxActor* 			getPhActor			( void ) { return localActor; }

			//! AddForceAtLocalPos
			void				addForce			( kge::math::Vector *pos );

			//! Set Actor Name
			void				setName				( const char *Name );

			//! Get Actor Name
			const char*			getName				( void );

			//! Is Dynamic Actor
			bool				isDynamic			( void );

			//! Caculate Kinematic ( rotational and translational ) Energy of Actor 
			float				computeKineticEnergy( void );

			//! Sets a dynamic actor's orientation in the world
			void				setGlobalOrientation( const kge::math::Matrix* mat );

			//! The moveGlobal* calls serve to move kinematically controlled dynamic actors through the game world. 
			void				moveGlobalPose		( const kge::math::Matrix* mat); 
   
			//! The moveGlobal* calls serve to move kinematically controlled dynamic actors through the game world. 
			void				moveGlobalPosition	( const kge::math::Vector* vec);

			//! Set the pose of the center of mass relative to the actor.
			void				setCenterOfMass		( const kge::math::Vector* vec);

			//! Forces the actor to sleep
			void				putToSleep			( void );

			//! Set Material
			void				setMaterial			( Material* mat );

			//! Get Material
			Material*			getMaterial			( void );

			//! Add another Shape 
			void				addShape			( math::Shape *shape , CollitionType ct , const kge::math::Vector* vec );

			//! Add another Shape
			void				addShape			(  math::KGE_SHAPES shape , CollitionType ct , const kge::math::Vector* vec );

		protected:
			PhysXManager*	localPhysXManager;
			NxActor*		localActor;
			unsigned int	actorNumber;
		};
	}
}

#endif // KGE_ACTOR_DYANAMIC_X_H
#endif // KGE_USE_PHYSX