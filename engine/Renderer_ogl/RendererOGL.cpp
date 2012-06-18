//#############################################################################
//# file: RendererOGL.cpp
//# Copyright (c) 2011 Pooya Shahinfar (Pswin)
//#############################################################################

#include "RendererOGL.h"
#include "initGL.h"
#include "../include/Logger.h"
#include <stdio.h>
#include <stdio.h>

#if KGE_COMPILER == KGE_COMPILER_MSVC
	#pragma comment(lib,"opengl32.lib")
#endif

//--------------------------------
// global variables
//--------------------------------
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX

#   include <X11/extensions/xf86vmode.h>

extern "C"
{
	Display *g_sDisplay;
	Window g_sRoot;
	Window g_sWin;
	XVisualInfo *g_sVisualInfo;
	XSetWindowAttributes g_sSwa;
	GLXContext g_sGLContext;
}
#endif

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		RendererOGL::RendererOGL()
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		RendererOGL::~RendererOGL()
		{
		} // Destructor

		//------------------------------------------------------------------------------------
		// Initialize the renderer
		//------------------------------------------------------------------------------------
		bool RendererOGL::Init(InitParameters &params)
		{
			//! creating OpenGL 2.0 context
			#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				// descriptor of pixel format
				PIXELFORMATDESCRIPTOR pfd;
				memset(&pfd, 0, sizeof(pfd));
				pfd.nSize        = sizeof(pfd);
				pfd.nVersion     = 1;
				pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
				pfd.iPixelType   = PFD_TYPE_RGBA;
				pfd.cColorBits   = params.bits;
				pfd.cDepthBits   = 24; // TODO:
				pfd.cStencilBits = (params.UseStencil)? 8 : 0;


				// getting device context
				m_pHDC = GetDC( (HWND)params.hwnd );

				int pf = ChoosePixelFormat( m_pHDC, &pfd );
				if (pf == 0)
				{
					io::Logger::Error("Window creation failed, can't choose pixel format");
					return false;
				}

				if (SetPixelFormat (m_pHDC, pf, &pfd ) == FALSE)
				{
					io::Logger::Error("Window creation failed, can't set pixel format");
					return false;
				}

				DescribePixelFormat(m_pHDC , pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

				if( params.FullScreen == true )
				{
					DEVMODE dmScreenSettings;   						// Device Mode
					memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
					dmScreenSettings.dmSize=sizeof(dmScreenSettings);	// Size Of The Devmode Structure
					dmScreenSettings.dmPelsWidth	= params.Width;		// Selected Screen Width
					dmScreenSettings.dmPelsHeight	= params.Height;	// Selected Screen Height
					dmScreenSettings.dmBitsPerPel	= 32;				// Selected Bits Per Pixel
					dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
					// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
					if ( ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL )
					{
						io::Logger::Error("can't to switch full screen mode");
					}

				}

				m_pGLRC = wglCreateContext( m_pHDC );
				wglMakeCurrent( m_pHDC,m_pGLRC );

				if ( params.VSync == false )
				{
					wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

					if( wglSwapIntervalEXT )
						wglSwapIntervalEXT(0);
				}

			#elif KGE_PLATFORM == KGE_PLATFORM_LINUX

				// Open client display
				g_sDisplay = XOpenDisplay(NULL);

				if (g_sDisplay == NULL)
				{
					io::Logger::Error("Can not connect to X-Server");
					return false;
				}

				// get video modes
				XF86VidModeModeInfo **modes;
				int screen = DefaultScreen(g_sDisplay);


				// enumerating available modes

				// get the root window
				g_sRoot = DefaultRootWindow(g_sDisplay);

				GLint att[] =
				{
					GLX_RGBA,
					GLX_RED_SIZE        , 8,
					GLX_GREEN_SIZE      , 8,
					GLX_BLUE_SIZE       , 8,
					GLX_ALPHA_SIZE      , 8,
					GLX_DEPTH_SIZE      , 24,
					GLX_STENCIL_SIZE    , 8,
					GLX_DOUBLEBUFFER    , True,
					GLX_SAMPLE_BUFFERS  , 1,
					GLX_SAMPLES         , 0,
					//GLX_EXT_swap_control, True,
					None,
				};



				// choosing visual attributes
				g_sVisualInfo = glXChooseVisual(g_sDisplay,0,att);

				if (g_sVisualInfo == NULL)
				{
					io::Logger::Error("No appropriate visual found, Trying for default");

					GLint def_att[] =
					{
						GLX_RGBA,
						GLX_RED_SIZE        , 8,
						GLX_GREEN_SIZE      , 8,
						GLX_BLUE_SIZE       , 8,
						GLX_ALPHA_SIZE      , 8,
						GLX_DEPTH_SIZE      , 24,
						GLX_STENCIL_SIZE    , 8,
						GLX_DOUBLEBUFFER    , True,
						GLX_EXT_swap_control, True,
						None,
					};

					g_sVisualInfo =  glXChooseVisual(g_sDisplay,0,def_att);

					if (g_sVisualInfo == NULL)
					{
						io::Logger::Error("No default visual found, check your driver!");
						return false;
					}
					else
					{
						io::Logger::Error("Default visual accepted: \n\tGLX_R8G8B8A8"\
								"\n\tGLX_DEPTH_SIZE 24\n\tGLX_STENCIL_SIZE 8");
					}
				}

				// creating color map
				Colormap cmap = XCreateColormap(g_sDisplay,g_sRoot,
							g_sVisualInfo->visual, AllocNone);


				g_sSwa.colormap = cmap;
				g_sSwa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | StructureNotifyMask;

				if ( params.FullScreen )
				{
					int modeNum = 0;
					int bestMode = -1;
					XF86VidModeGetAllModeLines(g_sDisplay, screen, &modeNum, &modes);
					for (int i = 0 ; i < modeNum; i++)
					{
						if (modes[i]->vdisplay == params.Height && modes[i]->hdisplay == params.Width )
							bestMode = i;
					}

					if (bestMode == -1)
					{
						io::Logger::Warning("doen't found any matchable mode with resolution %dx%d screen size",
									params.Width,params.Height);
						io::Logger::Info("default resolution choosed:\n\t%dx%d",modes[0]->hdisplay,modes[0]->vdisplay);
					}
			        XF86VidModeSwitchToMode(g_sDisplay, screen, modes[bestMode]);
			        XF86VidModeSetViewPort(g_sDisplay, screen, 0, 0);
			        params.Width =  modes[bestMode]->hdisplay;
			        params.Height = modes[bestMode]->vdisplay;

			        XFree(modes);

			        /* set window attributes */
			        g_sSwa.override_redirect = True;
			        g_sSwa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask |
			            StructureNotifyMask;
			        g_sWin = XCreateWindow(g_sDisplay,g_sRoot,  0, 0, params.Width, params.Height,
			        		0, g_sVisualInfo->depth, InputOutput,g_sVisualInfo->visual,
			            CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
			            &g_sSwa);
			        XWarpPointer(g_sDisplay, None, g_sWin, 0, 0, 0, 0, 0, 0);
					XMapRaised(g_sDisplay, g_sWin);
			        XGrabKeyboard(g_sDisplay, g_sWin, True, GrabModeAsync,
			            GrabModeAsync, CurrentTime);
			        XGrabPointer(g_sDisplay, g_sWin, True, ButtonPressMask,
			        		GrabModeAsync, GrabModeAsync, g_sWin, None, CurrentTime);
				}
				else
				{
					g_sWin = XCreateWindow(g_sDisplay,g_sRoot, 0, 0, params.Width, params.Height,0,
							g_sVisualInfo->depth, InputOutput, g_sVisualInfo->visual,
							CWColormap | CWEventMask, &g_sSwa);
					XMapWindow(g_sDisplay, g_sWin);
					XStoreName(g_sDisplay,g_sWin, "Kochol Game Engine");
				}

				// Sets the params.hwnd
				params.hwnd = (void*)g_sWin;

				g_sGLContext = glXCreateContext(g_sDisplay,g_sVisualInfo, NULL, GL_TRUE);

				glXMakeCurrent(g_sDisplay,g_sWin, g_sGLContext);

				if ( params.AAQuality >=0 )
				{
					glEnable(GL_MULTISAMPLE);
				}

				if ( params.VSync == false )
				{
					GLXDrawable drawable = glXGetCurrentDrawable();
					//typedef void ( * PFNGLXSWAPINTERVALEXTPROC) (Display* dpy, GLXDrawable drawable, int interval);
					PFNGLXSWAPINTERVALEXTPROC 	glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddressARB((const GLubyte*)"glXSwapIntervalEXT");

					glXSwapIntervalEXT(g_sDisplay, drawable, 0);

				}
			#endif // KGE_PLATFROM_WINDOWS

			// getting OpenGL version
			int major,minor = 0 ;
			sscanf( (const char*)glGetString( GL_VERSION ), "%d.%d", &major, &minor );
			int ver = (major * 10) + minor;

			// checking OpenGL version
			if (ver < 20)
			{
				io::Logger::Error( "OpenGL 2.x not supported, initializing renderer failed." );
				io::Logger::Error( "for initializing Renderer at last you need OpenGL 2.0" );
				return false;
			}
			else if (ver > 30)
			{
				ver  = 21 ; // only load OpenGL 2.1 extensions
			}

			// initialize OpenGL extensions
			if ( initGL(ver) == false )
			{
				io::Logger::Error("Loading one or more needed extensions failed.");
				return false;

			}

			//! setting clear color to black
			glClearColor( 0.0f, 0.0f, 0.0f , 0.0f );

			//! enabling depth test
			glEnable( GL_DEPTH_TEST );

			io::Logger::Info("OpenGL renderer initialized");
			io::Logger::Info("\tOpenGL version is '%d.%d'",ver / 10,ver%10);
			io::Logger::Info("\tOpenGL core version is '%s'",glGetString(GL_VERSION));
			io::Logger::Info("\tGLSL version is '%s'",glGetString(GL_SHADING_LANGUAGE_VERSION));

			return true;

		} // Init

		//------------------------------------------------------------------------------------
		// Clear the render target
		//------------------------------------------------------------------------------------
		bool RendererOGL::Clear(bool bColor, bool bDepth, bool bStencil)
		{
			GLenum flags = 0;

			if (bColor) flags |= GL_COLOR_BUFFER_BIT;
			if (bDepth) flags |= GL_DEPTH_BUFFER_BIT;
			if (bStencil) flags |= GL_STENCIL_BUFFER_BIT;

			glClear( flags );

			return true;

		} // Clear

		//------------------------------------------------------------------------------------
		// Begin the rendering
		//------------------------------------------------------------------------------------
		bool RendererOGL::BeginRendering(bool bColor, bool bDepth, bool bStencil)
		{
			return Clear(bColor, bDepth, bStencil);
		} // BeginRendering

		//------------------------------------------------------------------------------------
		// End the rendering and draw to back buffer.
		//------------------------------------------------------------------------------------
		bool RendererOGL::EndRendering()
		{
			#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SwapBuffers(m_pHDC);
			#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
				glXSwapBuffers(g_sDisplay,g_sWin);
			#endif
			return true;

		} // EndRendering

		// Check device if it is createable.
		bool RendererOGL::CheckDevice( void )
		{
			// TODO: check min requirements
			return true;
		}

		//------------------------------------------------------------------------------------
		// Create a vertex buffer from custom vertex type and store it on video memory.
		//------------------------------------------------------------------------------------
		HardwareBuffer* RendererOGL::CreateVertexBuffer
			(	void* Vertices, 
			u32 VCount, 
			u32 Stride, 
			bool isDynamic 
			)
		{

			return NULL;

		} // CreateVertexBuffer

		//------------------------------------------------------------------------------------
		// Creates an Index buffer on video memory
		//------------------------------------------------------------------------------------
		HardwareBuffer* RendererOGL::CreateIndexBuffer( void* Indices, u32 ICount, IndexBufferType eIndexBufferType, bool isDynamic )
		{
			return NULL;

		} // CreateIndexBuffer

	} // gfx

} // kge
