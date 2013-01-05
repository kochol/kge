#include "../include/Timer.h"

namespace kge
{
	namespace core
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Timer::Timer()
		{
#ifdef WIN32

			QueryPerformanceFrequency(&freq);

			ims = freq.QuadPart;// / 1000;

		QueryPerformanceCounter(&lastTick);
		base = lastTick;
		Interval = 0;

#else
#ifdef KGE_USE_SDL

			SDL_GetTicks();
			oldTime = SDL_GetTicks();
			currentTime = oldTime;

#endif // KGE_USE_SDL
#endif // WIN32

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Timer::~Timer()
		{

		} // Destructor

		//------------------------------------------------------------------------------------
		// Get the time passed
		//------------------------------------------------------------------------------------
		kge::uint Timer::GetTime( bool Reset )
		{
#ifdef WIN32

			LARGE_INTEGER curTick;
			QueryPerformanceCounter(&curTick);
			int iTime = 0;
			if(lastTick.QuadPart != 0)
			{
				iTime = (int)((__int64)curTick.QuadPart - (__int64)lastTick.QuadPart);
			}
			if (Reset)
				lastTick = curTick;
			return iTime * 1000000LL / ims;

#endif // WIN32

		} // GetTime

		//------------------------------------------------------------------------------------
		// Check if the interval passes or not?
		//------------------------------------------------------------------------------------
		bool Timer::NextFrame()
		{
			if (GetTime(false) > Interval)
			{
				GetTime(true);
				return true;
			}

			return false;

		} // NextFrame

	} // core

} // kge
