#ifndef KGE_KGEMUTEX_H
#define KGE_KGEMUTEX_H

#include "KgeUnknown.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX || KGE_PLATFORM == KGE_PLATFORM_ANDROID
#include <pthread.h>

#endif

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

			//! Trun mutex on
			void On() const;

			//! Trun mutex off
			void Off() const;

		protected:

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			HANDLE	m_hMutex;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
            pthread_mutex_t     m_Mutex;
#endif

		}; // KgeMutex

	} // core

} // kge

#endif // KGE_KGEMUTEX_H
