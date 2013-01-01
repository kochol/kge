#ifndef KGE_KGETHREAD_H
#define KGE_KGETHREAD_H

#include "kgedef.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
#include <pthread.h>

#endif

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

			//! Ends the thread and destroy it
			void End();

			//! Is thread running
			bool IsRunning() {return m_bIsRunning;}

			//! Thread function for internal use
			virtual ul32 ThreadProc() = 0;

		protected:

			bool		    m_bIsRunning;		//!< Is thread running

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			HANDLE		    m_hThreadHandle;
			DWORD		    m_dThreadID;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
            pthread_t       m_Thread;

#endif

		}; // KgeThread

	} // core

} // kge

#endif // KGE_KGETHREAD_H
