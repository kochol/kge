// File name: Timer.h
// Des: Timer class
// Date: 2/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef TIMER_H
#define TIMER_H

#include <SDL_stdinc.h>
#include "../KgeUnknown.h"
#include "../kgedef.h"

namespace kge
{
//! The core of engine.
namespace core
{
//! In class ham ke maloom hast che kari anjam midahad.
class KGE_API Timer: public KgeUnknown
{
public:

	//! Constructor
	Timer();

	//! Constructor
	//! \param fpsLimit For limiting the FPS.
	Timer(int fpsLimit);

	//! Destructor.
	~Timer() {}

	//! Returns the elapsed time in mili seconds
	int GetTime();

	//! If the Interval time reached returns true
	//! \sa Interval
	bool NextFrame();

	//! For limiting the FPS
	int fps;

	//! Sets the interval in mili seconds
	//! \sa NextFrame
	int Interval;

	//! Returns the elapsed time in seconds
	float GetTimeElapsed();

	int GetTime2();

protected:

	bool isFirstGetTimeCall;

	Uint64 lastTick;
	Uint64 base;
	Uint64 freq; // Frequency of your CPU
	int ims; // Millisecond

}; // Timer

} // core

} // kge

#endif // TIMER_H
