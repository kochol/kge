#ifndef KGE_KGETHREAD_H
#define KGE_KGETHREAD_H

#include "../kgedef.h"

#include <SDL_thread.h>

namespace kge
{
	//! KGE core classes
	namespace core
	{
		//! KGE thread interface
		class KGE_API KgeThread
		{
		public:

			//! Constructor
			KgeThread();

			//! Destructor
			virtual ~KgeThread();

			//! Create a thread and run it
			void Begin();

			//! Ends the thread and destroy it. Wait for the thread to end.
			void End();

			//! Is thread running
			bool IsRunning() {return m_bIsRunning;}

			//! Thread function for internal use
			virtual int ThreadProc() = 0;

		protected:

			bool                m_bIsRunning;                //!< Is thread running
			SDL_Thread		*	m_pSdlThread;

		}; // KgeThread

	} // core

} // kge

#endif // KGE_KGETHREAD_H