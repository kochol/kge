#ifndef KGE_ACTOR_DYNAMIC_H
#define KGE_ACTOR_DYNAMIC_H

#include "physicType.h"
#include "Actor.h"

namespace kge
{
	namespace ph
	{
		class Material;

		class ActorDynamic : public Actor
		{	
		public:
			//! Constructor
			ActorDynamic( ) { /* Nothing to do */ }

			//! Destructor
			virtual ~ActorDynamic() { /* Nothing to do */ }

			//! Set Position
			virtual void				setPosition			( kge::math::Vector *pos ) = 0;

			//! Set Actor Number
			virtual void				setActorNumber		( unsigned int i ) = 0;

			//! Get Actor Number
			virtual unsigned int		getActorNumber		( void			 ) = 0;


			//! Get Position
			virtual kge::math::Vector 	getPosition			( void ) = 0;

			//! Get Matrix
			virtual kge::math::Matrix	getMatrix			( void ) = 0;

			//! Set Mass
			virtual void				setMass				( float mass ) = 0;

			//! Get Mass
			virtual float				getMass				( void ) = 0;
		
			//! Recomputes a dynamic actor's mass properties from its shapes
			virtual bool				updateMassFromShapes( float density , float totalMass ) = 0;

			//! Set Angular Damping
			virtual void				setAngularDamping	( float angularDamping ) = 0;
			 
			//! Retrieves the angular damping coefficient.
			virtual float				getAngularDamping	( void ) = 0;

			//! Set linear Damping		
			virtual void				setLinearDamping	( float linearDamping ) = 0;
			
			//! Retrieves the linear damping coefficient.
			virtual float				getLinearDamping	( void ) = 0; 

			//! Sets the linear velocity of the actor. 
			virtual void				setLinearVelocity	( const kge::math::Vector *linVel) = 0; 
			
			//! Sets the angular velocity of the actor. 
			virtual void				setAngularVelocity	( const kge::math::Vector *angVel) = 0; 
			
			//! Retrieves the linear velocity of an actor. 
			virtual kge::math::Vector	getLinearVelocity	( void ) = 0; 
			
			//! Retrieves the angular velocity of the actor.  
			virtual kge::math::Vector	getAngularVelocity	( void ) = 0; 
			
			//! Lets you set the maximum angular velocity permitted for this actor.  
			virtual void				setMaxAngularVelocity(float maxAngVel) = 0; 
			
			//! Retrieves the maximum angular velocity permitted for this actor.   
			virtual float				getMaxAngularVelocity( void ) = 0; 

			//! Sets the linear momentum of the actor. 
			virtual void				setLinearMomentum	( const kge::math::Vector* linMoment) = 0; 
			
			//! Sets the angular momentum of the actor. 
			virtual void				setAngularMomentum	( const kge::math::Vector* angMoment) = 0; 
			  
			//! Retrieves the linear momentum of an actor.  
			virtual kge::math::Vector 	getLinearMomentum	( void ) = 0; 
			  
			//! Retrieves the angular momentum of an actor. 
			virtual kge::math::Vector	getAngularMomentum	( void ) = 0; 
			  
			//! Get Kge Object
			virtual kge::sn::SceneNode* getKgeObj			( void ) = 0;

			//! AddForceAtLocalPos
			virtual void				addForce			( kge::math::Vector *pos ) = 0;

			//! Set Actor Name
			virtual void				setName				( const char *Name ) = 0;

			//! Get Actor Name		
			virtual const char*			getName				( void ) = 0;

			//! Actor that have Body is Dynamic Actor
			virtual bool				isDynamic			( void ) = 0;
			
			//! Caculate Kinematic ( rotational and translational ) Energy of Actor 
			virtual float				computeKineticEnergy( void ) = 0;
			
			//! Sets a dynamic actor's orientation in the world
			virtual void				setGlobalOrientation( const kge::math::Matrix* mat ) = 0;

			//! The moveGlobal* calls serve to move kinematically controlled dynamic actors through the game world. 
			virtual void				moveGlobalPose		( const kge::math::Matrix* mat) = 0; 
   
			//! The moveGlobal* calls serve to move kinematically controlled dynamic actors through the game world. 
			virtual void				moveGlobalPosition  ( const kge::math::Vector* vec) = 0; 

			//! Set the pose of the center of mass relative to the actor.
			virtual void				setCenterOfMass		( const kge::math::Vector* vec) = 0;

			//! Forces the actor to sleep
			virtual void				putToSleep			( void ) = 0;

			//! Set Material
			virtual void				setMaterial			( Material* mat ) = 0;

			//! Get Material
			virtual Material*			getMaterial			( void ) = 0; 

			//! Add another Shape 
			virtual void				addShape			( math::Shape *shape , CollitionType ct = ECTP_TRIGGER_DISABLE , const kge::math::Vector& vec = kge::math::Vector( 0.0f , 0.0f , 0.0f )  ) = 0;

			//! Add another Shape
			virtual void				addShape			(  math::KGE_SHAPES shape , CollitionType ct = ECTP_TRIGGER_DISABLE , const kge::math::Vector& vec = kge::math::Vector( 0.0f , 0.0f , 0.0f ) ) = 0;

			ActorType					getType				( void ) { return EATP_DYNAMIC; }

		protected:
			sn::SceneNode*	localKgeObj;
			Material*		m_pMaterial;

		};
	} // ph
} // kge

#endif // KGE_ACTOR_DYNAMIC_H 