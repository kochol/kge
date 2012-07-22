#ifndef KGE_WORLDMANAGER_H
#define KGE_WORLDMANAGER_H

#include "kgedef.h"
#include "array.h"

namespace kge
{
	namespace en
	{
		class World;

		class KGE_API WorldManager
		{
		public:

			//! Constructor
			WorldManager();

			//! Destructor
			~WorldManager();

			//! Creates a world
			World* CreateWorld();

		protected:

			core::DynamicArray<World*>		m_vWorlds;

		}; // WorldManager
	} // en

} // kge

#endif // KGE_WORLDMANAGER_H
