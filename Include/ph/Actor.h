#ifndef KGE_PH_ACTOR_H
#define KGE_PH_ACTOR_H

#include "physicType.h"

namespace kge
{
	namespace ph
	{
		class Actor
		{
		public:
			/* Constructor */
			Actor() { /* Nothing To Do */ }

			virtual ~Actor(){ /* Nothing To Do*/ };

			virtual ActorType getType( void ) = 0;
		protected:
		}; // class Actor
	} // ph
} // kge

#endif // KGE_PH_ACTOR_H