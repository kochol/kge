#include "../include/KgeThread.h"
#include <stdio.h>

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
DWORD WINAPI cThreadProc( kge::core::KgeThread *pThis )
{
    return pThis->ThreadProc();
}

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
void* thread_function( void *pThis )
{
    ((kge::core::KgeThread*)pThis)->ThreadProc();
    pthread_exit(NULL);
}

#endif

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		KgeThread::KgeThread():m_bIsRunning(false)
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			,m_hThreadHandle(NULL),
			m_dThreadID(0)

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID

#endif
		{
		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		KgeThread::~KgeThread()
		{
			End();
		}

		//------------------------------------------------------------------------------------
		// Create a thread and run it
		//------------------------------------------------------------------------------------
		void KgeThread::Begin()
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			if( m_hThreadHandle )
				End();  // just to be safe.

			// Start the thread.
			m_hThreadHandle = CreateThread( NULL,
											0,
											(LPTHREAD_START_ROUTINE)cThreadProc,
											this,
											0,
											(LPDWORD)&m_dThreadID );
			if( m_hThreadHandle == NULL )
			{
			// Arrooga! Dive, dive!  And deal with the error, too!
			}

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
			if( m_bIsRunning )
				End();  // just to be safe.

            // Start the thread
            int res;
            res = pthread_create(&m_Thread, NULL, thread_function, this);
            if (res != 0)
            {
                printf("Thread creation failed\n");
                return;
            }

#endif

			m_bIsRunning = true;

		} // Begin

		//------------------------------------------------------------------------------------
		// Ends the thread and destroy it
		//------------------------------------------------------------------------------------
		void KgeThread::End()
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			if ( m_hThreadHandle != NULL )
			{
				m_bIsRunning = false;
				WaitForSingleObject( m_hThreadHandle, INFINITE );
				CloseHandle( m_hThreadHandle );
				m_hThreadHandle = NULL;
				m_dThreadID     = 0;
			}

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
            if ( m_bIsRunning )
            {
                m_bIsRunning = false;
                int res;
                void* thread_result;
                // Wait for thread to end it
                res = pthread_join(m_Thread, &thread_result);
            }

#endif

		} // End

	} // core

} // kge
