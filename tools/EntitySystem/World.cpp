#include "../../Include/en/World.h"
#include "../../Include/en/WorldComponent.h"
#include "../../Include/en/System.h"
#include "../../Include/en/Entity.h"

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
		m_vEntities[pEn->GetID()] = pEn;
		for (int i = 0; i < m_vSystems.size(); i++)
		{
			m_vSystems[i]->NotifySystem(pEn);
		}

	} // AddEntity

	//------------------------------------------------------------------------------------
	// Adds a component to the world
	//------------------------------------------------------------------------------------
	void World::AddComponent( WorldComponent* pCmp )
	{
		m_itComponents = m_vComponents.find(pCmp->GetClassID());
		if (m_itComponents == m_vComponents.end())
		{
			m_vComponents.insert(std::pair<int, std::vector<WorldComponent*> >(pCmp->GetClassID(), std::vector<WorldComponent*>()));
		}

		m_vComponents[pCmp->GetClassID()].push_back(pCmp);

	} // AddComponent

	//------------------------------------------------------------------------------------
	// Returns a component by its ID
	//------------------------------------------------------------------------------------
	WorldComponent* World::GetComponent( int iCmpID )
	{
		m_itComponents = m_vComponents.find(iCmpID);
		if (m_itComponents == m_vComponents.end())
		{
			return NULL;
		}

		if (m_itComponents->second.size() > 0)
			return m_itComponents->second[0];

		return NULL;

	} // GetComponent

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

	}

} // en

} // kge
