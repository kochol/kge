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

			LARGE_INTEGER lastTick;
			LARGE_INTEGER base;
			LARGE_INTEGER freq; // Frequency of your CPU
			int ims; // Millisecond

		}; // Timer

	} // core

} // kge