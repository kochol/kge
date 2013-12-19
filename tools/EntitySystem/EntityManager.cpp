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
		// Reserve some entity ID to create Entity outside the EntityManager
		//------------------------------------------------------------------------------------
		int EntityManager::ReserveID( int count )
		{
			int s = m_iNextEntityID++;
			m_iNextEntityID += count;
			return s;

		} // ReserveID

	} // en

} // kge
