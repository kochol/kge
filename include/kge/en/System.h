#pragma once

#include <vector>
#include "../kgedef.h"
#include "../core/Task.h"
#include "Message.h"

namespace kge
{
	namespace en
	{
		class Entity;

		class KGELIB_API System: public core::Task
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

			//! TaskManager call this function on a different thread and
			//! this function calls AsyncUpdate update from this function.
			core::Task* Do();

			//! Should this system update trough a thread default = false
			bool							m_bAsyncUpdate;
		
			//! Adds an entity to system
			void AddEntity(Entity* pEn);

			std::vector<Entity*>			m_vEntities;

		}; // System

	} // en

} // kge
