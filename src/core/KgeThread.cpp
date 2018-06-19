#include "../../Include/core/KgeThread.h"
#include "../../Include/io/Logger.h"
#include <stdio.h>

int thread_function( void *pThis )
{
	return ((kge::core::KgeThread*)pThis)->ThreadProc();
	
}

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		KgeThread::KgeThread():m_bIsRunning(false), m_pSdlThread(NULL)
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
			if( m_pSdlThread )
				End(); // just to be safe.

			// Start the thread.
			m_pSdlThread = SDL_CreateThread(thread_function, "", this);
			if( m_pSdlThread == NULL )
			{
				io::Logger::Error("Can't create thread.");
				return;
			}

			m_bIsRunning = true;

		} // Begin

		//------------------------------------------------------------------------------------
		// Ends the thread and destroy it
		//------------------------------------------------------------------------------------
		void KgeThread::End()
		{
			if ( m_pSdlThread != NULL )
			{
				m_bIsRunning = false;
				SDL_WaitThread(m_pSdlThread, NULL);
				m_pSdlThread = NULL;
			}

		} // End

	} // core

} // kge
