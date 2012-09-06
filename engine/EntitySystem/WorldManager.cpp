#include "../include/WorldManager.h"
#include "../include/World.h"

namespace kge
{
	namespace en
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		WorldManager::WorldManager()
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		WorldManager::~WorldManager()
		{

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

	} // en

} // kge
