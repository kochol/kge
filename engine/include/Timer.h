#pragma once

#include "kgedef.h"

namespace kge
{
	namespace core
	{
		class KGE_API Timer
		{
		public:

			//! Constructor
			Timer();

			//! Destructor
			~Timer();

			//! Get the time passed
			uint GetTime(bool Reset);

		protected:

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			LARGE_INTEGER lastTick;
			LARGE_INTEGER base;
			LARGE_INTEGER freq; // Frequency of your CPU
			int ims; // Millisecond
#endif

		}; // Timer

	} // core

} // kge
