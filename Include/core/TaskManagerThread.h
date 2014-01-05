#pragma once

#include "KgeThread.h"

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
			int GetTaskCount() {return m_iTaskCount;}

			//! Adds a new task to queue
			//! \return Returns the task count in queue
			int AddTask(Task* pTask);
		
		protected:

			//! The mutex that control the start and stop of the thread
			KgeMutex		*	m_pMutex;
			bool				m_bThreadStop;

			//! Task count in queue
			int					m_iTaskCount;
			
		}; // TaskManagerThread

	} // core

} // kge
