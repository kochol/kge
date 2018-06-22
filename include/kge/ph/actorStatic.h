// File name: ActorX.h
// Des: This class is for creating PhysX Static Actor
// Date: 07/11/1388
// Programmer: Hadi Robati (hadirobati)

#ifndef KGE_ACTOR_STATIC_H
#define KGE_ACTOR_STATIC_H

namespace kge
{
	namespace ph
	{
		class ActorStatic : public Actor
		{	
		public:
			//! Constructor
			ActorStatic() { /* Nothing to do */ }
			
			//! Destructor
			~ActorStatic() { /* Nothing to do */ }

			//! Get Kge Object
			kge::sn::SceneNode* getKgeObj	( void ){ return localKgeObj; }

			ActorType getType				( void ) { return EATP_STATIC; }

		protected:
			sn::SceneNode*	localKgeObj;
		};
	}
}

#endif // KGE_ACTOR_STATIC_H