// File name: Mouse.h
// Des: In class baraye kar ba mouse hast.
// Date: 6/10/1386
// Programmer: Pooya Shahinfar(Pswin)

#include "../../Include/io/Mouse.h"
#include "../../Include/io/Logger.h"

#ifdef KGE_USE_SDL
#include <SDL.h>
#endif // KGE_USE_SDL

#ifdef WIN32
#include <SDL_syswm.h>
#endif // WIN32

extern SDL_Window* g_pSDLwindow;


std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vClickFunctions;
std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMotionFunctions;
std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseDownFunctions;
std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseUpFunctions;
std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseWheelFunctions;

namespace kge
{
namespace io
{
	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Mouse::~Mouse()
	{
	} // Destructor

	// ********* ********* ******
	// Gereftane mogheiate mouse.
	// ********* ********* ******
	void Mouse::GetPosition(math::Vector2I &pos)
	{
        pos.x = 0;
        pos.y = 0;

#ifdef KGE_USE_SDL
		SDL_GetMouseState(&pos.x, &pos.y);
#endif // KGE_USE_SDL

	} // GetPosition

	math::Vector2I Mouse::GetPosition()
	{
		math::Vector2I pos;

#ifdef KGE_USE_SDL
		SDL_GetMouseState(&pos.x, &pos.y);
#endif // KGE_USE_SDL

		return pos;

	} // GetPosition

	// *** ******* ********* ******
	// Set kardane mogheiate mouse.
	// *** ******* ********* ******
	void Mouse::SetPosition(const math::Vector2I& pos)
	{
#ifdef KGE_USE_SDL
		SDL_WarpMouseInWindow(g_pSDLwindow, pos.x,pos.y);
#endif // KGE_USE_SDL

	} // SetPosition(Vector pos)

	// *** ******* ********* ******
	// Set kardane mogheiate mouse.
	// *** ******* ********* ******
	void Mouse::SetPosition(int x, int y)
	{
#ifdef KGE_USE_SDL
		SDL_WarpMouseInWindow(g_pSDLwindow, x, y);
#endif // KGE_USE_SDL
		 
	} // SetPosition(int x, y)

	// ***** ******* ******** ******* ********* *****
	// check kardane feshorde shodane dokmehaye mouse
	// ***** ******* ******** ******* ********* *****
	bool Mouse::ButtonDown(MouseButton btn)
	{
#ifdef KGE_USE_SDL
		switch (btn)
		{
		case EMB_Left:
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(1))
				return true;
			break;
		case EMB_Right:
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(3))
				return true;
			break;
		case EMB_Middle:
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(2))
				return true;
			break;
		case EMB_WheelUp:
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(4))
				return true;
			break;
		case EMB_WheelDown:
			if(SDL_GetMouseState(NULL, NULL)&SDL_BUTTON(5))
				return true;
			break;
		}
#endif // KGE_USE_SDL
		return false;

	} // ButtonDown(BUTTONS btn)

	//------------------------------------------------------------------------------------
	// Connect an event 
	//------------------------------------------------------------------------------------
	void Mouse::Connect(gui::EventType eventtype,
		core::Functor1<void, gui::EventArgs*>* Function)
	{
		switch (eventtype)
		{
		case gui::EET_Click:
			g_vClickFunctions.push_back(Function);
			break;

		case gui::EET_MouseDown:
			g_vMouseDownFunctions.push_back(Function);
			break;

		case gui::EET_MouseUp:
			g_vMouseUpFunctions.push_back(Function);
			break;

		case gui::EET_MouseMotion:
			g_vMotionFunctions.push_back(Function);
			break;

		case gui::EET_MouseWheel:
			g_vMouseWheelFunctions.push_back(Function);

		}
	} // Connect

} // io

} // kge
