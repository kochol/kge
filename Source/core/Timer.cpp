// File name: Timer.cpp
// Des: Class Timer ke besiar baraye sakhte bazi mofid hast. albate felan faghat ro
//		win32 kar mikone.
// Date: 2/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/core/Timer.h"

#ifndef WIN32
#ifdef KGE_USE_SDL
#include <SDL.h>
#endif // KGE_USE_SDL
#endif // WIN32

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

		SetThreadAffinityMask(GetCurrentThread(), 1);
		QueryPerformanceFrequency(&freq);
		
		ims = freq.QuadPart / 1000;
		//if (ims < 0 ) ims = -ims;

		QueryPerformanceCounter(&lastTick);
		base = lastTick;
		fps = 0;
		Interval = 0;

#else
#ifdef KGE_USE_SDL

		SDL_GetTicks();
		oldTime = SDL_GetTicks();
		currentTime = oldTime;

#endif // KGE_USE_SDL
#endif // WIN32

	} // Timer()

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Timer::Timer(int ifps)
	{
#ifdef WIN32

		SetThreadAffinityMask(GetCurrentThread(), 1);
		QueryPerformanceCounter(&lastTick);
		QueryPerformanceFrequency(&freq);
		ims = freq.QuadPart / ifps;
		fps = ifps;
		isFirstGetTimeCall = true;
		base = lastTick;
		Interval = 0;

#else
#ifdef KGE_USE_SDL

		oldTime = SDL_GetTicks();
		currentTime = oldTime;

#endif // KGE_USE_SDL
#endif // WIN32

	} // Timer(int fpsLimit).

	// ******* ****** ** ** ** **** **** ******** ** *** **** **** ***** *****
	// Meghdar zamani ro ke az dafe pish gozashte ro bar hasb mili sanie mide.
	// ******* ****** ** ** ** **** **** ******** ** *** **** **** ***** *****
	int Timer::GetTime()
	{
#ifdef WIN32

		LARGE_INTEGER curTick;
		QueryPerformanceCounter(&curTick);
		int iTime = 0;
		if(lastTick.QuadPart != 0)
		{
			iTime = (int)((__int64)curTick.QuadPart - (__int64)lastTick.QuadPart);
		}
		if ((iTime / ims) > 0)
			lastTick = curTick;
		return iTime / ims;

#else
#ifdef KGE_USE_SDL

		u32 tempT;
		tempT = SDL_GetTicks();
		currentTime = tempT - oldTime;
		oldTime = tempT;
		return currentTime;

#endif // KGE_USE_SDL
#endif // WIN32

	} // GetTime

	// *********
	// NextFrame
	// *********
	bool Timer::NextFrame()
	{
#ifdef WIN32

		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		if(lastTick.QuadPart != 0)
		{
			if(fps==0)
			{
				if(Interval <= (GetTime2()/ims))
				{
					lastTick = t;
					return true;
				}
				else
					return false;
			}
			else
			{
				int done = 0;
				do
				{
					int ticks_passed = (int)((__int64)t.QuadPart - (__int64)lastTick.QuadPart);
					int ticks_left = ims - ticks_passed;
					if (t.QuadPart < lastTick.QuadPart)    // time wrap
						done = 1;
					if (ticks_passed >= ims)
						done = 1;

					if (!done)
					{
						if (ticks_left > (int)freq.QuadPart/500)
							Sleep(1);
						else
							for (int i=0; i<10; i++)
								Sleep(0);  // causes thread to give up its time slice
					}
				}
				while (!done);
				return true;
			}
		}
		lastTick = t;
		return false;

#else
#ifdef KGE_USE_SDL

		if( GetTime2() >= Interval )
		{
			oldTime = SDL_GetTicks();
			return true;
		}
		else
			return false;

#endif // KGE_USE_SDL
#endif // WIN32

	} // NextFrame

	// ********
	// GetTime2
	// ********
	int Timer::GetTime2()
	{
#ifdef WIN32

	LARGE_INTEGER curTick;
	QueryPerformanceCounter(&curTick);
	int iTime = 0;
	if(lastTick.QuadPart != 0)
	{
		iTime = (int)((__int64)curTick.QuadPart - (__int64)lastTick.QuadPart);
	}
	return iTime;

#else
#ifdef KGE_USE_SDL

		return SDL_GetTicks() - oldTime;

#endif // KGE_USE_SDL
#endif // WIN32

	} // GetTime2

	//--------------------------------------------------------
	// Meghdar sanye ghozashte az dafe ghabl ra barmigardanad.
	//--------------------------------------------------------
	float Timer::GetTimeElapsed()
	{
		//return GetTime() * 0.001f;

		LARGE_INTEGER curTick;
		QueryPerformanceCounter(&curTick);

		float elapsedTime = static_cast<float>(curTick.QuadPart - lastTick.QuadPart) / static_cast<float>(freq.QuadPart);

		lastTick = curTick;

		return elapsedTime;

	} // GetTimeElapsed

} // core

} // kge
