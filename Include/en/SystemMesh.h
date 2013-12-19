#pragma once

#include "../kgedef.h"
#include "System.h"

namespace kge
{
	namespace en
	{
		class KGELIB_API SystemMesh : public System
		{
		public:
		
			//! Constructor
			SystemMesh();
		
			//! Destructor
			~SystemMesh();
		
			//! Updates the system and it will update the entities.
			void Update(float fElaspedTime);

			//! Updates the system in asynchronism mode. 
			void AsyncUpdate();

			//! When a new entity created or new component attached to entity 
			//! you must call it on systems to check the entity.
			void NotifySystem(Entity* pEn);

		protected:

			int								m_iComMeshID,
											m_iComPosID;
			
		}; // SystemMesh

	} // en

} // kge
