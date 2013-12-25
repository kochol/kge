#ifndef KGE_WORLD_H
#define KGE_WORLD_H

#include "kgedef.h"
#include <map>
#include <vector>
#include "Message.h"

namespace kge
{
	namespace en
	{
		class Entity;
		class System;

		//! The world class create EntityManager
		class KGELIB_API World
		{
		public:

			//! Constructor
			World();

			//! Destructor
			~World();

			//! Adds an entity to the World.
			//! Use EntityCreator to create an entity.
			void AddEntity(Entity* pEn);

			//! Remove an entity from World
			void RemoveEntity(Entity* pEn);

			//! Adds a system to world
			void AddSystem(System* pSys);

			//! Updates the World
			void Update(float fElasped);

			//! Sends a message to World systems
			void OnMessage(Message* msg);

		protected:

			std::map<int, Entity*>										m_vEntities;		//!< The list of entities.
			std::vector<System*>										m_vSystems;

		}; // World

	} // en

} // kge

#endif // KGE_WORLD_H
