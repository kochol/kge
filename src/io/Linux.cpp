// File name: Linux.cpp
// Des: in class vazife keshidan panjare ro to Linux bar ohde darad. omidvaram betonam
//		besazamesh.
// Date: 23/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef LINUX

#include "../../Headers/io/Linux.h"
#include "../../include/kge/io/Logger.h"



namespace kge
{
namespace io
{
	// *******
	// sazande
	// *******
	Linux::Linux()
	{
		m_pDisplay = NULL;
		
	} // Sazande

	//------------
	// Destructor.
	//------------
	Linux::~Linux()
	{
	} // Destructor

	//--------------------------------------------
	// Create a new window for rendering in linux.
	//--------------------------------------------
	bool Linux::Init(int Width, int Height, int Bits, bool Fullscreen, int ra)
	{
		XSetWindowAttributes windowAttributes;
		XVisualInfo *visualInfo = NULL;
		XEvent event;
		Colormap colorMap;
		GLXContext glxContext;
		int errorBase;
		int eventBase;
		m_bDoubleBuffered = true;

		//
		// Open a connection to the X server
		//
		m_pDisplay = XOpenDisplay( NULL );

		if( m_pDisplay == NULL )
		{
			io::Logger::Log("could not open display", io::ELM_Error);
			return false;
		}

		//
		// Make sure OpenGL's GLX extension supported
		//
		if( !glXQueryExtension( m_pDisplay, &errorBase, &eventBase ) )
		{
			io::Logger::Log("X server has no OpenGL GLX extension", io::ELM_Error);
			return false;
		}

		//
		// Find an appropriate visual
		//
		int doubleBufferVisual[]  =
		{
			GLX_RGBA,           // Needs to support OpenGL
			GLX_DEPTH_SIZE, 16, // Needs to support a 16 bit depth buffer
			GLX_DOUBLEBUFFER,   // Needs to support double-buffering
			None                // end of list
		};

		int singleBufferVisual[] =
		{
			GLX_RGBA,           // Needs to support OpenGL
			GLX_DEPTH_SIZE, 16, // Needs to support a 16 bit depth buffer
			None                // end of list
		};

		//
		// Try for the double-bufferd visual first
		//
		visualInfo = glXChooseVisual( m_pDisplay, DefaultScreen(m_pDisplay), doubleBufferVisual );

		if( visualInfo == NULL )
		{
    		// If we can't find a double-bufferd visual, try for a single-buffered visual...
			visualInfo = glXChooseVisual( m_pDisplay, DefaultScreen(m_pDisplay), singleBufferVisual );
			io::Logger::Log("Can't create double buffered window try single buffered.");

			if( visualInfo == NULL )
			{
				io::Logger::Log("There is no RGB visual with depth buffer", io::ELM_Error);
				return false;
			}

			m_bDoubleBuffered = false;
		}

		//
		// Create an OpenGL rendering context
		//
		glxContext = glXCreateContext( m_pDisplay, 
									   visualInfo, 
									   NULL,      // No sharing of display lists
									   GL_TRUE ); // Direct rendering if possible
	                           
		if( glxContext == NULL )
		{
			io::Logger::Log("Could not create rendering context", io::ELM_Error);
			return false;
		}

		//
		// Create an X colormap since we're probably not using the default visual 
		//
		colorMap = XCreateColormap( m_pDisplay, 
									RootWindow(m_pDisplay, visualInfo->screen), 
									visualInfo->visual, 
									AllocNone );

		windowAttributes.colormap     = colorMap;
		windowAttributes.border_pixel = 0;
		windowAttributes.event_mask   = ExposureMask           |
										VisibilityChangeMask   |
										KeyPressMask           |
										KeyReleaseMask         |
										ButtonPressMask        |
										ButtonReleaseMask      |
										PointerMotionMask      |
										StructureNotifyMask    |
										SubstructureNotifyMask |
										FocusChangeMask;
	    
		//
		// Create an X window with the selected visual
		//
		m_Window = XCreateWindow( m_pDisplay, 
								  RootWindow(m_pDisplay, visualInfo->screen), 
								  0, 0,     // x/y position of top-left outside corner of the window
								  Width, Height, // Width and height of window
								  0,        // Border width
								  visualInfo->depth,
								  InputOutput,
								  visualInfo->visual,
								  CWBorderPixel | CWColormap | CWEventMask,
								  &windowAttributes );

		XSetStandardProperties( m_pDisplay,
								m_Window,
								KGE_VERSION_NAME,
								"KGE",
								None,
								NULL,
								NULL,
								NULL );

		// Bind the rendering context to the window
		glXMakeCurrent( m_pDisplay, m_Window, glxContext );

		// Request the X window to be displayed on the screen
		XMapWindow( m_pDisplay, m_Window );

	} // Init

	//-----------------
	// Run the program.
	//-----------------
	bool Linux::Run()
	{
            XNextEvent( m_pDisplay, &event );

            switch( event.type )
            {
                case ButtonPress:
                {
            	    if( event.xbutton.button == 1 )
            		{
					}
                }
                break;

                case ButtonRelease:
                {
                }
                break;
                
                case KeyPress:
                {
                }
                break;

                case KeyRelease:
                {
                }
                break;

                case MotionNotify:
                {
                }
                break;

                case ConfigureNotify:
                {
                    glViewport( 0, 0, event.xconfigure.width, event.xconfigure.height );
                }
            }

		return true;

	} // Run

	//----------------------------------------------
	// esme panjare ra avaz mikone P1= esme panjare.
	//----------------------------------------------
	void Linux::SetCaption(const char* name)
	{
	} // SetCaption

	void* Linux::GetHandler()
	{
		LinuxHandler* h    = new LinuxHandler();
		h->m_pDisplay      = m_pDisplay;
		h->m_Window        = m_Window;
		h->m_bDoubleBuffer = m_bDoubleBuffered;

		return h;

	} // GetHandler

	//-----------------------------
	// this is only for opengl use.
	//-----------------------------
	void Linux::ToFullScreen()
	{
		
	} // ToFullScreen

} // io

} // kge

#endif // LINUX