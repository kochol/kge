#pragma once

#include "KgeThread.h"
#include "TSQueue.h"

namespace kge
{
	namespace core
	{
		class KgeMutex;
		class Task;

		/**
		 * This class is used by TaskManager class for running threads.
		 */
		class KGE_API TaskManagerThread: public KgeThread
		{
		public:
		
			//! Constructor
			TaskManagerThread();
		
			//! Destructor
			~TaskManagerThread();

			//! Thread function for internal use
			int ThreadProc();

			//! Returns the Task count in queue
			int GetTaskCount() {return m_TaskQueue.size();}

			//! Adds a new task to queue
			//! \return Returns the task count in queue
			int AddTask(Task* pTask);
		
		protected:

			//! Task count in queue
			TSQueue<Task*>		m_TaskQueue;
			
		}; // TaskManagerThread

	} // core

} // kge
