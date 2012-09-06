#include "../include/KgeMutex.h"
#include "../include/Logger.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#endif

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		KgeMutex::KgeMutex()
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			// This mutex will help the two threads share their toys.
			m_hMutex = CreateMutex( NULL, false, NULL );
			if( m_hMutex == NULL )
				io::Logger::Error( "Mutex creation failed." );

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
            int res = pthread_mutex_init(&m_Mutex, NULL);
            if (res != 0)
                io::Logger::Error( "Mutex creation failed." );

#endif

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		KgeMutex::~KgeMutex()
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			if( m_hMutex != NULL )
			{
				CloseHandle( m_hMutex );
				m_hMutex = NULL;
			}

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
            pthread_mutex_destroy(&m_Mutex);

#endif

		} // Destructor

		//------------------------------------------------------------------------------------
		// Trun mutex on
		//------------------------------------------------------------------------------------
		void KgeMutex::On() const
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			WaitForSingleObject( m_hMutex, INFINITE );  // To be safe...

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
            pthread_mutex_lock((pthread_mutex_t*)&m_Mutex);

#endif

		} // On

		//------------------------------------------------------------------------------------
		// Trun mutex off
		//------------------------------------------------------------------------------------
		void KgeMutex::Off() const
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			ReleaseMutex( m_hMutex );  // To be safe...

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
            pthread_mutex_unlock((pthread_mutex_t*)&m_Mutex);

#endif

		} // Off

	} // core

} // kge
