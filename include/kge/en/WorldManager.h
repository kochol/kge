#ifndef KGE_WORLDMANAGER_H
#define KGE_WORLDMANAGER_H

#include "../kgedef.h"
#include <vector>

namespace kge
{
	namespace en
	{
		class World;
		class Server;
		class EntityManager;

		class KGELIB_API WorldManager
		{
		public:

			//! Constructor
			WorldManager();

			//! Destructor
			~WorldManager();

			//! Creates a world
			World* CreateWorld();

			//! Adds another server that World Manager can use
			void AddServer(Server* pServer);

			//! Returns the main server pointer
			Server*	GetMainServer() {return m_pMainServer;}

			//! Returns the entity manager
			EntityManager* GetEntityManager() {return m_pEntityMgr;}

			//! Returns the world by its index
			World* GetWorld(int index){return m_vWorlds[index];}

			//! Updates the Worlds
			void Update(float fElasped);

		protected:

			EntityManager		*	m_pEntityMgr;
			Server				*	m_pMainServer;
			std::vector<World*>		m_vWorlds;
			std::vector<Server*>	m_vServers;

		}; // WorldManager

	} // en

} // kge

#endif // KGE_WORLDMANAGER_H
