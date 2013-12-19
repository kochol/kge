// File name: JoyStick.cpp
// Des: this class is for using joystick.
// Date: feb-15-2008 , 26/11/1386 (jalali)
// Programmer: Pooya Shainfar

#include "../../Include/io/JoyStick.h"

#include <SDL.h>
#include <ctype.h>

#ifdef WIN32
#include <SDL_syswm.h>
#endif // WIN32



namespace kge
{
namespace io
{
	// ******* ********* 
	// Sazande JoyStick
	// ******* ********* 
	JoyStick::JoyStick()
	{
#ifdef KGE_USE_SDL
		if ( SDL_NumJoysticks == 0)
			return;
		for (int i = 0 ; i<= SDL_NumJoysticks() ; i++)
		{
// 			if(!SDL_JoystickOpened(i))
// 			{
// 				stick = SDL_JoystickOpen( i ); 
// 				return;
// 			}
		}
#endif
	}


	// ********* ********
	// Mokharebe JoyStick
	// ********* ********
	JoyStick::~JoyStick()
	{
#ifdef KGE_USE_SDL
		if ( stick != NULL )
			SDL_JoystickClose( stick );
#endif
	}
	//******** ******* ********** ********
	//greftane mogiate mehvarhaye joystick
	//******** ******* ********** ********
	void JoyStick::GetAXISPosition(JS_AXIS &pos)
	{
#ifdef KGE_USE_SDL
		pos.X1 = (int)( ((SDL_JoystickGetAxis(stick,0) + 32769)* 100) / 65536) ;
		pos.Y1 = pos.Y1 = 100 - (int)(((SDL_JoystickGetAxis(stick,1) + 32769)* 100) / 65536 ) ;

		pos.X2 = (int)( ((SDL_JoystickGetAxis(stick,2) + 32769)* 100) / 65536) ;
		pos.Y2 = 100 -(int)( ((SDL_JoystickGetAxis(stick,3) + 32769)* 100) / 65536) ;
#endif

	} // Get Axis Position

	//******** ******* ********** ********
	//greftane mogiate mehvarhaye joystick
	//******** ******* ********** ********
	JS_AXIS JoyStick::GetAXISPosition()
	{

		JS_AXIS pos;
#ifdef KGE_USE_SDL		
		pos.X1 = (int)( ((SDL_JoystickGetAxis(stick,0) + 32769)* 100) / 65536) ;
		pos.Y1 = 100 - (int)(((SDL_JoystickGetAxis(stick,1) + 32769)* 100) / 65536 ) ;

		pos.X2 = (int)( ((SDL_JoystickGetAxis(stick,2) + 32769)* 100) / 65536) ;
		pos.Y2 = 100 -(int)( ((SDL_JoystickGetAxis(stick,3) + 32769)* 100) / 65536) ;
#endif
		return pos;
	} // Get Axis Position



	// ***** ******* ******** ******* ********* ********
	// check kardane feshorde shodane dokmehaye JoyStick
	// ***** ******* ******** ******* ********* ********
	bool JoyStick::ButtonDown(int btn)
	{
#ifdef KGE_USE_SDL
		if( stick != NULL)
			return SDL_JoystickGetButton(stick,btn - 1 );
#endif
		return false;
		
	} // ButtonDown

	// ************* ******* ***
	// Bargardandane mogiate HAT
	// ************* ******* ***
	JS_HAT JoyStick::GetHATState(int HAT_No)
	{
#ifdef KGE_USE_SDL
		switch(SDL_JoystickGetHat(stick,HAT_No))
		{
		case SDL_HAT_CENTERED:
			return EJ_CENTER ;
		case SDL_HAT_LEFTDOWN :
			return EJ_DOWN_LEFT ;
		case SDL_HAT_LEFTUP :
			return EJ_UP_LEFT ;
		case SDL_HAT_RIGHTDOWN :
			return EJ_DOWN_RIGHT ;
		case SDL_HAT_RIGHTUP  :
			return EJ_UP_RIGHT ;
		case SDL_HAT_UP:
			return EJ_UP ;
		case SDL_HAT_DOWN:
			return EJ_DOWN ;
		case SDL_HAT_LEFT:
			return EJ_LEFT ;
		case SDL_HAT_RIGHT:
			return EJ_RIGHT ;
		default:
			return EJ_CENTER ;
		}
#endif
        return EJ_CENTER;
	}

	// ********* ******* ******** ****
	// gereftane etelate JoyStick feli
	// ********* ******* ******** ****
	JS_INFO JoyStick::GetCurrentJoySickInfo()
	{
		JS_INFO info;
#ifdef KGE_USE_SDL
		info.Number_Of_AXIS = SDL_JoystickNumAxes(stick);
		info.Number_Of_Buttons = SDL_JoystickNumButtons (stick);
		info.Number_Of_POV_HATS = SDL_JoystickNumHats (stick);
		info.Used_JoyStick_Number  = JoyNo + 1;		// shomare ra ba 1 jame mikonim zira shomare ha az sefr shoro mishvand
		info.Number_Of_Balls = SDL_JoystickNumBalls(stick);
#endif
		return info;
	}

	// ***** ******** ***** ***** *********
	// check kardande faale bodan Joysstick
	// ***** ******** ***** ***** *********
	bool JoyStick::JoyStickIsActive()
	{
#ifdef KGE_USE_SDL
		if ( stick != NULL)
            return true;
#endif
		return false;
	}

} // io

} // kge