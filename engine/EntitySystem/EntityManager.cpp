#include "../include/EntityManager.h"
#include "../include/Entity.h"

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

	} // en

} // kge
