// File name: SdlWindow.cpp
// Des: This class is for creating windows with SDL library.
// Date: 9/9/1386
// Programmer: Ali Akbar Mohamadi(Kochol), Hadi Robati( hadirobati )

#include "../../Headers/io/SdlWindow.h"
#include "../../Include/io/Keyboard.h"
#include "stdio.h"

#ifdef KGE_USE_SDL
#include <SDL2/SDL.h>


#include "../../Include/io/Logger.h"
#include "../../Include/enums.h"

#ifdef WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "sdl2main.lib")
#endif // WIN32

KGE_API SDL_Window* g_pSDLwindow = NULL;

extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vClickFunctions;
extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMotionFunctions;
extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseDownFunctions;
extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseUpFunctions;
extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vMouseWheelFunctions;

extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vKeyDownFunction;
extern std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*> g_vKeyUpFunction;

namespace kge
{
namespace io
{
    SdlWindow::SdlWindow()
        : m_sdlSurface(0)
    {
#ifdef WIN32
        m_pWindow = 0;
#endif
    }

    SdlWindow::~SdlWindow()
    {
		std::vector<kge::core::Functor1<void, kge::gui::EventArgs*>*>::iterator it;
		for (it = g_vClickFunctions.begin(); it != g_vClickFunctions.end(); ++it)
		{
		 	delete (*it);
		}
		g_vClickFunctions.clear();
		 
		for (it = g_vMotionFunctions.begin(); it != g_vMotionFunctions.end(); ++it)
		{
		 	delete (*it);
		}
		g_vMotionFunctions.clear();
		 
		for (it = g_vMouseDownFunctions.begin(); it != g_vMouseDownFunctions.end(); ++it)
		{
		 	delete (*it);
		}
		g_vMouseDownFunctions.clear();
		 
		for (it = g_vMouseUpFunctions.begin(); it != g_vMouseUpFunctions.end(); ++it)
		{
		 	delete (*it);
		}
		g_vMouseUpFunctions.clear();
		 
       //SDL_Quit();
    }

	//-----------------
	// Create a window.
	//-----------------
	bool SdlWindow::Init(int Width, int Height, int Bits,
		bool Fullscreen, int ra)
	{
        m_iWidth = Width;
        m_iHeight = Height;
        m_iBits = Bits;
        m_bFullscreen = Fullscreen;

		if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 )
		{
			io::Logger::Log("Can't Initialize SDL for creating window.", io::ELM_Error);
			return false;
		}

		Uint32 f = 0;
		if (Fullscreen)
			f = SDL_WINDOW_FULLSCREEN ;
		if (ra == gfx::ERA_OpenGL)
		{
			/* Turn on double buffering with a 24bit Z buffer.
			* You may need to change this to 16 or 32 for your system */
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

			f |= SDL_WINDOW_OPENGL;
		}
		m_sdlSurface = SDL_CreateWindow(KGE_VERSION_NAME,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			Width, Height,
			f);
		g_pSDLwindow = m_sdlSurface;

#ifdef WIN32
		SDL_SysWMinfo wmInfo;
	    SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(m_sdlSurface, &wmInfo);
		m_pWindow = (HWND)wmInfo.info.dummy;
#else

#endif // WIN32

#ifdef LINUX
        const SDL_VideoInfo *pSDLVideoInfo = SDL_GetVideoInfo();
		nFlags |= SDL_OPENGL | SDL_DOUBLEBUF | SDL_HWPALETTE | SDL_INIT_TIMER;

		if( pSDLVideoInfo->hw_available )
            nFlags |= SDL_HWSURFACE;
        else
            nFlags |= SDL_SWSURFACE;

        // Hardware supported blitting?
        if( pSDLVideoInfo->blit_hw )
            nFlags |= SDL_HWACCEL;

        // Enable OpenGL Doublebuffering
        SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
#endif //LINUX

		//if Error happen
		if( !m_sdlSurface )
        {
            printf( "Call to SDL_SetVideoMode() failed! - SDL_Error: %s\n", SDL_GetError() );
            SDL_Quit();
            return false;
        }

		return true;
	}

	//---------------------------
	// aya panjere baste nashode.
	//---------------------------
	bool SdlWindow::Run()
	{
		
		while( SDL_PollEvent( &sdl_event ) )
		{
			switch( sdl_event.type )
			{
				case SDL_QUIT:
					SDL_Quit();
					return false;
				
				case SDL_MOUSEBUTTONUP:
					{
						gui::MouseEventArgs meaClick;
						meaClick.m_EventType = gui::EET_MouseUp;
						meaClick.Mousebutton = GetMouseButton(sdl_event.button.button);
						meaClick.X			 = sdl_event.button.x;
						meaClick.Y			 = sdl_event.button.y;
						meaClick.Xrel		 = 0;
						meaClick.Yrel		 = 0;

						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vMouseUpFunctions.begin(); 
							it != g_vMouseUpFunctions.end(); ++it)
						{
							(*it)->Call(&meaClick);
						}

						// Check for clicks.
						if (sdl_event.button.button == SDL_BUTTON_LMASK)
						{
							meaClick.m_EventType = gui::EET_Click;
							for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vClickFunctions.begin(); 
								it != g_vClickFunctions.end(); ++it)
							{
								(*it)->Call(&meaClick);
							}
						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					{
						// Check for clicks.
						gui::MouseEventArgs meaClick;
						meaClick.m_EventType = gui::EET_MouseDown;
						meaClick.Mousebutton = GetMouseButton(sdl_event.button.button);
						meaClick.X			 = sdl_event.button.x;
						meaClick.Y			 = sdl_event.button.y;
						meaClick.Xrel		 = 0;
						meaClick.Yrel		 = 0;
						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vMouseDownFunctions.begin(); 
							it != g_vMouseDownFunctions.end(); ++it)
						{
							(*it)->Call(&meaClick);
						}
					}
					break;

				case SDL_MOUSEMOTION:
					{
						gui::MouseEventArgs meaClick;
						meaClick.m_EventType = gui::EET_MouseMotion;
						meaClick.Mousebutton = GetMouseButton(sdl_event.motion.state);
						meaClick.X			 = sdl_event.motion.x;
						meaClick.Y			 = sdl_event.motion.y;
						meaClick.Xrel		 = sdl_event.motion.xrel;
						meaClick.Yrel		 = sdl_event.motion.yrel;
						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vMotionFunctions.begin(); 
							it != g_vMotionFunctions.end(); ++it)
						{
							(*it)->Call(&meaClick);
						}
					}	
					break;

				case SDL_MOUSEWHEEL:
					{
						gui::MouseEventArgs meaClick;
						meaClick.m_EventType = gui::EET_MouseWheel;
						meaClick.Mousebutton = GetMouseButton(sdl_event.motion.state);
						meaClick.X			 = sdl_event.wheel.x;
						meaClick.Y			 = sdl_event.wheel.y;
						meaClick.Xrel		 = sdl_event.motion.xrel;
						meaClick.Yrel		 = sdl_event.motion.yrel;
						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vMouseWheelFunctions.begin(); 
							it != g_vMouseWheelFunctions.end(); ++it)
						{
							(*it)->Call(&meaClick);
						}
					}

				case SDL_KEYDOWN:
					{
						gui::KeyEventArgs keyEvent;
						keyEvent.currentKeyDown = (io::KEYS)(sdl_event.key.keysym.sym);
						keyEvent.keyState = EKS_Down;
						keyEvent.m_EventType = gui::EET_KeyDown;
						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vKeyDownFunction.begin(); 
							it != g_vKeyDownFunction.end(); ++it)
						{
							(*it)->Call(&keyEvent);
						}
						
					}
					break;
				case SDL_KEYUP:
					{
						gui::KeyEventArgs keyEvent;
						keyEvent.currentKeyDown = (io::KEYS)(sdl_event.key.keysym.sym);
						keyEvent.keyState = EKS_Up;
						keyEvent.m_EventType = gui::EET_KeyUp;
						for(std::vector<core::Functor1<void, gui::EventArgs*>*>::iterator it = g_vKeyUpFunction.begin(); 
							it != g_vKeyUpFunction.end(); ++it)
						{
							(*it)->Call(&keyEvent);
						}
						
					}
					break;
			}
		}

		return true;
	}

	//----------------------------------------------
	// esme panjare ra avaz mikone P1= esme panjare.
	//----------------------------------------------
	void SdlWindow::SetCaption(const char* name)
	{
		SDL_SetWindowTitle(m_sdlSurface, name);
	}

	//----------------------------------------------
	// set mouse cursor, cur(P1) is cursor type
	//----------------------------------------------
	void SdlWindow::SetCursor(CURSOR_TYPES cur)
	{
		switch(cur)
		{
		case kge::io::CUR_SHOW :
		case kge::io::CUR_POINTER :
			SDL_ShowCursor(1);
			break;
		case kge::io::CUR_HIDE :
			SDL_ShowCursor(0);
			break;
		}
	}

	//-------------------------------------------------
	// Get the Handler so you can work with the window.
	//-------------------------------------------------
	void* SdlWindow::GetHandler()
	{
#ifdef WIN32
		return m_pWindow;
#endif // WIN32

		return NULL;
	}

	//------------------------------------------------------------------------------------
	// Returns the pressed mouse button
	//------------------------------------------------------------------------------------
	MouseButton SdlWindow::GetMouseButton(Uint8 state)
	{
		switch (state)
		{
		case SDL_BUTTON_LMASK:
			return EMB_Left;
			break;

		case SDL_BUTTON_RMASK:
			return EMB_Right;
			break;

		case SDL_BUTTON_MMASK:
			return EMB_Middle;
			break;
		}

		return EMB_None;

	} // GetMouseButton

	bool SdlWindow::InitTimer()
	{
		if (SDL_Init( SDL_INIT_TIMER ) < 0)
		{
			io::Logger::Log("Can't Initialize SDL for timers.", io::ELM_Error);
			return false;
		}

		return true;
	}

    //-----------------------------
    // 
    //-----------------------------
    void SdlWindow::SendQuitEvent()
    {
        SDL_Event evnt = { SDL_QUIT };
        SDL_PushEvent(&evnt);

    } // SendQuitEvent

    //-----------------------------
    // Resize window
    //-----------------------------
    void SdlWindow::Resize(unsigned int width, unsigned int height, bool fullscreen)
    {
        m_iWidth = width;
        m_iHeight = height;
        m_bFullscreen = fullscreen;
		SDL_SetWindowSize(m_sdlSurface, width, height);
        if (fullscreen)
            SDL_SetWindowFullscreen(m_sdlSurface, SDL_WINDOW_FULLSCREEN);
        else
            SDL_SetWindowFullscreen(m_sdlSurface, 0);
    }

} // io

} // kge

#endif // KGE_USE_SDL
