// File name: SdlWindow.h
// Des: This class is for creating windows with SDL library.
// Date: 9/9/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef SDLWINDOW
#define SDLWINDOW

#include "../../Include/kgedef.h"

#ifdef KGE_USE_SDL

#include "../../Include/io/Iwindow.h"
#include <SDL.h>

#ifdef WIN32
#include <SDL_syswm.h>
#endif // WIN32

namespace kge
{
namespace io
{
	class SdlWindow : public IWindow
	{
	public:

		// Constructor.
		SdlWindow();

		// Destructor.
		virtual ~SdlWindow();

		// Create a window.
		virtual bool Init(int Width, int Height, int Bits,
			bool Fullscreen, int ra);

		virtual bool InitTimer();

		// aya panjere baste nashode.
		virtual bool Run();

		// esme panjare ra avaz mikone P1= esme panjare
		virtual void SetCaption(const char* name);

		// set mouse cursor
		virtual void SetCursor(CURSOR_TYPES cur);


		// Get the Handler so you can work with the window.
		virtual void* GetHandler();

		// Returns the pressed mouse button
		virtual MouseButton GetMouseButton(Uint8 state);

		//! 
		virtual void SendQuitEvent();

		//! Resize window
		virtual void Resize(unsigned int width, unsigned int height, bool fullscreen);

	protected:

		SDL_Event               sdl_event;              //used to poll for events and handle inputs.
		SDL_Window*            m_sdlSurface;     

#ifdef WIN32
		HWND m_pWindow;
#endif // WIN32

		// this is only for opengl use.
		virtual void ToFullScreen() {}

	}; // Window

} // io

} // kge

#endif // KGE_USE_SDL

#endif // SDLWINDOW
