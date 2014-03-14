// File name: Keyboard.h
// Des: In class Keyboard hast.
// Date: 5/10/1386
// Programmer: Pooya Shahinfar (Pswin), Hadi Robati( hadirobati )

#include "../../Include/io/Keyboard.h"

#ifdef KGE_USE_SDL
#include <SDL2/SDL.h>
#include <ctype.h>
#endif // KGE_USE_SDL

#ifdef WIN32
#include <SDL2/SDL_syswm.h>
#endif // WIN32



std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vKeyDownFunction;
std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vKeyUpFunction;

namespace kge
{
namespace io
{
	// ****** ** ***** ***** ***** ******** ****** ******** **** ** *****
	// Vaghti ke klide morde nazar feshorde shavad meghdare true ro mide.
	// ****** ** ***** ***** ***** ******** ****** ******** **** ** *****
	bool Keyboard::KeyDown(KEYS key)
	{
#ifdef KGE_USE_SDL
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		
		if( keystate[ SDL_GetScancodeFromKey(key) ] ) 
            return true;
#endif // KGE_USE_SDL
		return false;

	} // KeyDown(KEYS key)

	// ****** ** ***** ***** ***** ******** ****** ******** **** ** *****
	// Vaghti ke klide morde nazar feshorde shavad meghdare true ro mide.
	// ****** ** ***** ***** ***** ******** ****** ******** **** ** *****
	bool Keyboard::KeyDown(char key)
	{
#ifdef KGE_USE_SDL
		const Uint8 *keystate = SDL_GetKeyboardState(NULL);
		key = tolower(key);
		if ( keystate[ SDL_GetScancodeFromKey(key) ] )
			return true;
#endif // KGE_USE_SDL
		return false;

	} // KeyDown(char key)

	//------------------------------------------------------------------------------------
	// Connect an event 
	//------------------------------------------------------------------------------------
	void Keyboard::Connect(gui::EventType eventtype,
		core::Functor1<void, gui::EventArgs*>* Function)
	{
		switch (eventtype)
		{
		case gui::EET_KeyDown:
			g_vKeyDownFunction.push_back(Function);
			break;
		case gui::EET_KeyUp:
			g_vKeyUpFunction.push_back(Function);
			break;
		}
	} // Connect

} // io

} // kge
