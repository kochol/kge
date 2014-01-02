// File name: Timer.cpp
// Des: Class Timer ke besiar baraye sakhte bazi mofid hast. albate felan faghat ro
//		win32 kar mikone.
// Date: 2/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/core/Timer.h"

#include <SDL.h>

namespace kge
{
namespace core
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Timer::Timer()
	{
		freq = SDL_GetPerformanceFrequency();
		
		ims = freq / 1000;
		//if (ims < 0 ) ims = -ims;

		lastTick = SDL_GetPerformanceCounter();
		base = lastTick;
		fps = 0;
		Interval = 0;

	} // Timer()

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Timer::Timer(int ifps)
	{
		lastTick = SDL_GetPerformanceCounter();
		freq = SDL_GetPerformanceFrequency();
		ims = freq / ifps;
		fps = ifps;
		isFirstGetTimeCall = true;
		base = lastTick;
		Interval = 0;

	} // Timer(int fpsLimit).

	// ******* ****** ** ** ** **** **** ******** ** *** **** **** ***** *****
	// Meghdar zamani ro ke az dafe pish gozashte ro bar hasb mili sanie mide.
	// ******* ****** ** ** ** **** **** ******** ** *** **** **** ***** *****
	int Timer::GetTime()
	{
		Uint64 curTick;
		curTick = SDL_GetPerformanceCounter();

		int iTime = 0;
		if(lastTick != 0)
		{
			iTime = (int)(curTick - lastTick);
		}
		if ((iTime / ims) > 0)
			lastTick = curTick;
		return iTime / ims;

	} // GetTime

	// *********
	// NextFrame
	// *********
	bool Timer::NextFrame()
	{
		Uint64 t;
		t = SDL_GetPerformanceCounter();
		if(lastTick != 0)
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
					int ticks_passed = (int)(t - lastTick);
					int ticks_left = ims - ticks_passed;
					if (t < lastTick)    // time wrap
						done = 1;
					if (ticks_passed >= ims)
						done = 1;

					if (!done)
					{
						if (ticks_left > (int)freq/500)
							SDL_Delay(1);
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

	} // NextFrame

	// ********
	// GetTime2
	// ********
	int Timer::GetTime2()
	{
		Uint64 curTick = SDL_GetPerformanceCounter();
		int iTime = 0;
		if(lastTick != 0)
		{
			iTime = (int)(curTick - lastTick);
		}
		return iTime;

	} // GetTime2

	//--------------------------------------------------------
	// Meghdar sanye ghozashte az dafe ghabl ra barmigardanad.
	//--------------------------------------------------------
	float Timer::GetTimeElapsed()
	{
		Uint64 curTick;
		curTick = SDL_GetPerformanceCounter();

		float elapsedTime = static_cast<float>(curTick - lastTick) / static_cast<float>(freq);

		lastTick = curTick;

		return elapsedTime;

	} // GetTimeElapsed

} // core

} // kge
