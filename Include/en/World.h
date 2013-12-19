#ifndef KGE_WORLD_H
#define KGE_WORLD_H

#include "../kgedef.h"
#include <map>
#include <vector>

namespace kge
{
	namespace en
	{
		class Entity;
		class WorldComponent;
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

			//! Adds a component to the world
			void AddComponent(WorldComponent* pCmp);

			//! Returns a component by its ID
			WorldComponent* GetComponent(int iCmpID);

			//! Adds a system to world
			void AddSystem(System* pSys);

			//! Updates the World
			void Update(float fElasped);

		protected:

			std::map<int, Entity*>										m_vEntities;		//!< The list of entities.
			std::vector<System*>										m_vSystems;
			std::map<int, std::vector<WorldComponent*> >				m_vComponents;
			std::map<int, std::vector<WorldComponent*> >::iterator		m_itComponents;

		}; // World

	} // en

} // kge

#endif // KGE_WORLD_H
