#include "../../Include/en/EntityManager.h"
#include "../../Include/en/Entity.h"

namespace kge
{
	namespace en
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		EntityManager::EntityManager(): m_iNextEntityID(0)
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		EntityManager::~EntityManager()
		{
			std::map<int, Entity*>::iterator it;
			for (it = m_vEntities.begin(); it != m_vEntities.end(); it++)
				KGE_DELETE(it->second, Entity);

		} // Destructor

		//------------------------------------------------------------------------------------
		// Creates an entity
		//------------------------------------------------------------------------------------
		Entity* EntityManager::CreateEntity()
		{
			int enID = 0;
			if (m_vEntityFreeIDs.size() > 0)
			{
				enID = m_vEntityFreeIDs.front();
				m_vEntityFreeIDs.pop();
			}
			else
				enID = m_iNextEntityID++;

			Entity* pEn = KGE_NEW(Entity)(enID);

			m_vEntities[enID] = pEn;

			return pEn;

		} // CreateEntity

		//------------------------------------------------------------------------------------
		// Creates an entity with the given ID if an Entity exist with this ID it will be removed.
		//------------------------------------------------------------------------------------
		Entity* EntityManager::CreateEntity( int enid )
		{
			// Check if we have an entity with the same ID or not
			if (m_vEntities[enid])
			{
				RemoveEntity(enid);
			}

			Entity* pEn = KGE_NEW(Entity)(enid);

			m_vEntities[enid] = pEn;

			return pEn;

		}

		//------------------------------------------------------------------------------------
		// Reserve some entity ID to create Entity outside the EntityManager
		//------------------------------------------------------------------------------------
		int EntityManager::ReserveID( int count )
		{
			int s = m_iNextEntityID++;
			m_iNextEntityID += count;
			return s;

		} // ReserveID

		//------------------------------------------------------------------------------------
		// Removes an entity and delete it
		//------------------------------------------------------------------------------------
		void EntityManager::RemoveEntity( int enid )
		{
			if (m_vEntities[enid])
			{
				Entity* p = m_vEntities[enid];
				KGE_DELETE(p, Entity);
				m_vEntities[enid] = NULL;
			}

		} // RemoveEntity

	} // en

} // kge
