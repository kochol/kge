// File name: JoyStick.h
// Des: this class is for using joystick.
// Date: feb-15-2008 , 26/11/1386 (jalali)
// Programmer: Pooya Shainfar

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "../KgeUnknown.h"
#include "../kgedef.h"

#ifdef KGE_USE_SDL
#include <SDL_joystick.h>
#endif

namespace kge
{
namespace io
{
	// sakhtar mehvarhaye JoyStick 
	// tamamie mehvarha bar asase darsade zakhire mishan
	// mehvare X1,Y1 baraye samte chap va X2,Y2 baraye samte rast
	struct JS_AXIS
	{
		int X1;
		int Y1;
		int X2;
		int Y2;

        JS_AXIS() : X1(0), Y1(0), X2(0), Y2(0) {}
	};

	// yek sakhtar ke etelate marbot be JoyStick dar an negahdari mishavad
	struct JS_INFO
	{
		int Number_Of_Balls;
		int Number_Of_Buttons;
		int Number_Of_AXIS;
		int Number_Of_POV_HATS;
		int Used_JoyStick_Number;

        JS_INFO()
            : Number_Of_Balls(0),
            Number_Of_Buttons(0),
            Number_Of_AXIS(0),
            Number_Of_POV_HATS(0),
            Used_JoyStick_Number(0)
        {}
	};

	// enume HAT JoyStick
	enum JS_HAT
	{
		EJ_LEFT,
		EJ_RIGHT,
		EJ_CENTER,
		EJ_UP,
		EJ_DOWN,
		EJ_UP_LEFT,
		EJ_UP_RIGHT,
		EJ_DOWN_LEFT,
		EJ_DOWN_RIGHT,
	};
class KGE_API JoyStick: public KgeUnknown
{
public:

	//! Sazande.
	JoyStick();

	//! Mokhareb.
	~JoyStick();

	//! Gereftane Mogiate mehvarhaye JoyStick.
	/*!
	 \param pos mogiate mehvaraye Joy ra dar an zakhire mikonad
	 */
	void GetAXISPosition(JS_AXIS &pos);
	
	//! Gereftane mogheiate mehvarhaye JoyStick.
	JS_AXIS GetAXISPosition();

	//! check kardane feshorde shodane dokme haye JoyStick
	/*!
	 \param button name dokmeie ke garar ast check shavad ra moshakhas mikonad
	 */
	bool ButtonDown(int btn);

	//! gereftane etelate joystick dar hale estefade
	JS_INFO GetCurrentJoySickInfo();

	//! baresie faal bodan JoyStick
	bool JoyStickIsActive();

	//! bargardandane vaziate HAT
	JS_HAT GetHATState(int HAT_Number = 0);
protected:
#ifdef KGE_USE_SDL
	SDL_Joystick *stick ;
#endif
	int JoyNo;

}; // JoyStick

} // io

} // kge


#endif // JOYSTICK_H

