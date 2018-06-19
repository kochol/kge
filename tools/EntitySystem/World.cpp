#include "../../include/kge/en/World.h"
#include "../../include/kge/en/System.h"
#include "../../include/kge/en/Entity.h"
#include "../../include/kge/core/TaskManager.h"

namespace kge
{
namespace en
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	World::World()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	World::~World()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Adds an entity to the World.
	//------------------------------------------------------------------------------------
	void World::AddEntity( Entity* pEn )
	{
		if (m_vEntities.find(pEn->GetID()) == m_vEntities.end())
		{
			m_vEntities[pEn->GetID()] = pEn;
			for (int i = 0; i < m_vSystems.size(); i++)
			{
				m_vSystems[i]->NotifySystem(pEn);
			}
		}

	} // AddEntity

	//------------------------------------------------------------------------------------
	// Remove an entity from World
	//------------------------------------------------------------------------------------
	void World::RemoveEntity( Entity* pEn )
	{
		if (m_vEntities.find(pEn->GetID()) != m_vEntities.end())
		{
			m_vEntities.erase(pEn->GetID());
		}

	} // RemoveEntity

	//------------------------------------------------------------------------------------
	// Adds a system to world
	//------------------------------------------------------------------------------------
	void World::AddSystem( System* pSys )
	{
		m_vSystems.push_back(pSys);

		// Update entities
		std::map<int, Entity*>::iterator it;
		for(it = m_vEntities.begin(); it != m_vEntities.end(); it++)
			pSys->NotifySystem(it->second);

		// Add system to task manager
		core::TaskManager::GetSingletonPointer()->AddTask(pSys);

	} // AddSystem

	//------------------------------------------------------------------------------------
	// Updates the World
	//------------------------------------------------------------------------------------
	void World::Update( float fElasped )
	{
		for (int i = 0; i < m_vSystems.size(); i++)
		{
			m_vSystems[i]->Update(fElasped);
		}

	} // Update

	//------------------------------------------------------------------------------------
	// Sends a message to World systems
	//------------------------------------------------------------------------------------
	void World::OnMessage( Message* msg )
	{
		for (int i = 0; i < m_vSystems.size(); i++)
		{
			m_vSystems[i]->OnMessage(msg);
		}

	} // SendMessage

} // en

} // kge
