#pragma once

#include "../kgedef.h"

namespace kge
{
	namespace core
	{
		class Task;
		class TaskManagerThread;
		class KgeMutex;			 

		/**
		 * TaskManager class split tasks on different threads to use all CPU power.
		 */
		class KGE_API TaskManager
		{
			//! Constructor
			TaskManager();

		public:
			
			//! Destructor
			~TaskManager();

			//! Returns the TaskManager singleton pointer
			static TaskManager* GetSingletonPointer();

			//! Adds a Task to process
			void AddTask(Task* pTask);
		
		protected:
		
			//! The default threads array
			TaskManagerThread	*	m_pThreads;
			int						m_iThreadCount;
			
		}; // TaskManager
	} // core

} // kge