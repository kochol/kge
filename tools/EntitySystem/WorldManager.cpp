#include "../../include/kge/en/WorldManager.h"
#include "../../include/kge/en/World.h"
#include "../../include/kge/en/Server.h"
#include "../../include/kge/en/EntityManager.h"
#include "../../include/kge/core/KgeMemory.h"

namespace kge
{
	namespace en
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		WorldManager::WorldManager()
		{
			m_pMainServer = KGE_NEW(Server);
			m_pEntityMgr  = KGE_NEW(EntityManager);

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		WorldManager::~WorldManager()
		{
			KGE_DELETE(m_pMainServer, Server);
			KGE_DELETE(m_pEntityMgr, EntityManager);

			for (int i = 0; i < m_vWorlds.size(); i++)
				KGE_DELETE(m_vWorlds[i], World);

		} // Destructor

		//------------------------------------------------------------------------------------
		// Creates a world
		//------------------------------------------------------------------------------------
		World* WorldManager::CreateWorld()
		{
			World* pW = KGE_NEW(World)();

			m_vWorlds.push_back(pW);

			return pW;

		} // CreateWorld

		//------------------------------------------------------------------------------------
		// Updates the Worlds
		//------------------------------------------------------------------------------------
		void WorldManager::Update( float fElasped )
		{
			for (int i = 0; i < m_vWorlds.size(); i++)
				m_vWorlds[i]->Update(fElasped);

		} // Update

	} // en

} // kge
