#ifndef KGE_KGEMUTEX_H
#define KGE_KGEMUTEX_H

#include "../KgeUnknown.h"

struct SDL_mutex;

namespace kge
{
	namespace core
	{
		//! Mutex are used to help threads share their objects.
		class KGE_API KgeMutex: public KgeUnknown
		{
		public:

			//! Constructor
			KgeMutex();

			//! Destructor
			~KgeMutex();

			//! Turn mutex on
			void On() const;

			/**
			 *  Try to turn on the mutex
			 *
			 *  \return true on succeed, false on time out
			 */
			bool TryOn() const;

			/** 
			 * Turn mutex off
			 *  \warning It is an error to unlock a mutex that has not been locked by
			 *           the current thread, and doing so results in undefined behavior.
			 */
			void Off() const;

		protected:

			SDL_mutex		*	m_pSdlMutex;

		}; // KgeMutex

	} // core

} // kge

#endif // KGE_KGEMUTEX_H
