#pragma once

#include "../kgedef.h"

struct SDL_cond;

namespace kge
{
	namespace core
	{
		class KgeMutex;

		class KGE_API KgeCond
		{
		public:
		
			//! Constructor
			KgeCond();
		
			//! Destructor
			~KgeCond();

			/**
			 *  Wait on the condition variable, unlocking the provided mutex.
			 *
			 *  \warning The mutex must be locked before entering this function!
			 *
			 *  The mutex is re-locked once the condition variable is signaled.
			 *
			 */
			void Wait(KgeMutex* pMutex);

			//! Restart one of the threads that are waiting on the condition variable.
			void Signal();

			//! Restart all threads that are waiting on the condition variable.
			void Broadcast();
		
		protected:
		
			SDL_cond		*	m_pCond;
			
		}; // KgeCond

	} // core

} // kge
