#include "../../Include/core/KgeMutex.h"
#include "../../Include/io/Logger.h"

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		KgeMutex::KgeMutex()
		{
			m_pSdlMutex = SDL_CreateMutex();
			if( m_pSdlMutex == NULL )
				io::Logger::Error( "Mutex creation failed." );

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		KgeMutex::~KgeMutex()
		{
			if (m_pSdlMutex)
			{
				SDL_DestroyMutex(m_pSdlMutex);
				m_pSdlMutex = NULL;
			}

		} // Destructor

		//------------------------------------------------------------------------------------
		// Turn mutex on
		//------------------------------------------------------------------------------------
		void KgeMutex::On() const
		{
			SDL_LockMutex(m_pSdlMutex);

		} // On

		//------------------------------------------------------------------------------------
		// Try to turn on the mutex
		//------------------------------------------------------------------------------------
		bool KgeMutex::TryOn() const
		{
			int s = SDL_TryLockMutex(m_pSdlMutex);
			if (s == 0)
				return true;

			return false;

		} // On

		//------------------------------------------------------------------------------------
		// Turn mutex off
		//------------------------------------------------------------------------------------
		void KgeMutex::Off() const
		{
			SDL_UnlockMutex(m_pSdlMutex);

		} // Off

	} // core

} // kge
