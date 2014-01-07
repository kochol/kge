#ifndef KGE_ENTITYMANAGER_H
#define KGE_ENTITYMANAGER_H

#include "kgedef.h"
#include <map>
#include <queue>

namespace kge
{
	namespace en
	{
		class Entity;

		class KGELIB_API EntityManager
		{
		public:

			//! Constructor
			EntityManager();

			//! Destructor
			~EntityManager();

			//! Creates an entity
			Entity* CreateEntity();

			//! Creates an entity with the given ID if an Entity exist with this ID it will be removed.
			Entity* CreateEntity(int enid);

			//! Removes an entity and delete it
			void RemoveEntity(int enid);

			//! Reserve some entity ID to create Entity outside the EntityManager
			/*! \param count The count of Entity ID to reserve
				\returns Returns the first Entity ID of reserved IDs
			 */
			int ReserveID(int count);

		protected:

			std::map<int, Entity*>		m_vEntities;		//!< The list of entities.
			std::queue<int>				m_vEntityFreeIDs;	//!< The free entities ID.
			int							m_iNextEntityID;

		}; // EntityManager

	} // en

} // kge

#endif // KGE_ENTITYMANAGER_H
