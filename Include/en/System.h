#pragma once

#include <vector>
#include "../kgedef.h"
#include "Message.h"

namespace kge
{
	namespace en
	{
		class Entity;

		class KGELIB_API System
		{
		public:
		
			//! Constructor
			System();
		
			//! Destructor
			virtual ~System();

			//! Updates the system and it will update the entities.
			virtual void Update(float fElaspedTime) = 0;

			//! Updates the system in asynchronism mode. 
			virtual void AsyncUpdate() = 0;
		
			//! When a new entity created or new component attached to entity 
			//! you must call it on systems to check the entity.
			virtual void NotifySystem(Entity* pEn) = 0;

			//! Receive a message to process
			virtual bool OnMessage(Message* msg) = 0;

		protected:
		
			//! Adds an entity to system
			void AddEntity(Entity* pEn);

			std::vector<Entity*>			m_vEntities;

		}; // System

	} // en

} // kge
