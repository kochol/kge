// File name: Win32.cpp
// Des: in class vazife keshidan panjare ro to win32 bar ohde darad.
// Date: 10/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef WIN32

#include "../../headers/io/win32.h"
#include "../../include/io/logger.h"
#include "../../include/kgedef.h"
#include "../../include/device.h"



#pragma comment(lib, "Winmm.lib")

// * * * * * * * *
// G O L O B A L S
// * * * * * * * *
MSG msg;
kge::gfx::RendererAPI g_ra;

// Here is our WndProc (Window Procedure).														
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT    ps;
	
	// This checks what the message is.
	switch(iMsg)											
	{	
// 		case WM_CREATE: // This message is sent when the window is created.									
// 			// We would want to put our initialization code here...
// 			break;

// 		case WM_SIZE: // This message happens when we resize the window.
// 			// We would put code to resize text or a picture here...
// 			break;										

// 		case WM_PAINT: // This message is sent to the WndProc when the window needs to be repainted.  
// 			if (g_ra == kge::gfx::ERA_OpenGL)
// 			{
// 				BeginPaint(hwnd, &ps);
// 				EndPaint(hwnd, &ps);
// 			}
// 			break;										

		case WM_DESTROY: // This message is sent when it's time to destroy the window.
			// Here you would handle deinitialization, freeing memory, etc..								
			//SendMessage(hwnd, WM_QUIT, 0, 0);
			PostQuitMessage(0);										
			break;

		case WM_SYSCOMMAND:
			if (SC_KEYMENU == (wParam & 0xFFF0)) { return 0; }
			return DefWindowProc(hwnd, iMsg, wParam, lParam);
		
		default:
			return DefWindowProc(hwnd, iMsg, wParam, lParam);											
	}													

	return 0;  
				
}		
// - - - - - - - -
// G O L O B A L S
// - - - - - - - -

namespace kge
{
namespace io
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Win32::Win32()
	{
		m_pHwnd = NULL;
		m_phInst = NULL;

		timeBeginPeriod(1);
		
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Win32::~Win32()
	{
		// Return to desktop
		if (m_bFullscreen && (g_ra == gfx::ERA_OpenGL))
			ChangeDisplaySettings(NULL, 0);

		UnregisterClass("KGE003",m_phInst);		

		timeEndPeriod(1);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Initialize the window
	//------------------------------------------------------------------------------------
	bool Win32::Init(int Width, int Height, int Bits, bool Fullscreen,
					 int ra)
	{
		m_iWidth = Width;
		m_iHeight = Height;
		m_iBits = Bits;
		m_bFullscreen = Fullscreen;
		g_ra = (gfx::RendererAPI)ra;

		m_phInst = GetModuleHandle(0);
		WNDCLASSEX  wndclass   = {0};
		wndclass.cbSize        = sizeof(WNDCLASSEX);
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;		
		wndclass.lpfnWndProc   = WndProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = m_phInst;						
		wndclass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = "KGE003";
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW); // default cursor.
		RegisterClassEx(&wndclass);								

		// Age fullscreen va opengl bod 
		if (Fullscreen && (ra==gfx::ERA_OpenGL))
			ToFullScreen();

		// size client panjare ro daghighan ba size darkhsti set mikonim.
		RECT rect = {0, 0, m_iWidth, m_iHeight};
	
		DWORD style = Fullscreen ?
			WS_POPUP | WS_SYSMENU :
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX;

		AdjustWindowRect(&rect, style, FALSE);

		// Hesab kardane makane panjare dar vasate safhe.
		//kge::s32 nLeft = (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2;
		//kge::s32 nTop = (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom  - rect.top)) / 2;

		//if (Fullscreen)
		//{
		//	nLeft = 0;
		//	nTop  = 0;
		//}

		const int upperLeft = Fullscreen ? 0 : CW_USEDEFAULT;

		m_pHwnd = CreateWindowEx(NULL,"KGE003", KGE_VERSION_NAME, style,
					upperLeft, upperLeft, rect.right  - rect.left,
					rect.bottom - rect.top, NULL, NULL, m_phInst, NULL);

		// hwnd created
		ShowWindow(m_pHwnd, SW_SHOW);	
		UpdateWindow(m_pHwnd);			

		//SetActiveWindow(m_pHwnd);
		//SetForegroundWindow(m_pHwnd);

		io::Logger::Log("Creating Win32 Window.");

		return true;
		
	} // Init(int Width, int Height, int Bits, bool Fullscreen)

	//------------------------------------------------------------------------------------
	// Receive messages from OS and handle them
	//------------------------------------------------------------------------------------
	bool Win32::Run()
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				return false;
			if(msg.message == WM_DESTROY)
				return false;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return true;
		
	} // Run

	//------------------------------------------------------------------------------------
	// Sets window caption
	//------------------------------------------------------------------------------------
	void Win32::SetCaption(const char* name)
	{
		SetWindowText(m_pHwnd, name);
		
	} // SetCaption

	//------------------------------------------------------------------------------------
	// Make the window fullscreen mode
	//------------------------------------------------------------------------------------
	void Win32::ToFullScreen()
	{
		if (m_bFullscreen)
		{	// From windowed to fullscreen
			DEVMODE dmSettings;									
			memset(&dmSettings,0,sizeof(dmSettings));			

			if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
			{
				io::Logger::Log("Could Not Enum Display Settings.", ELM_Error);
				return;
			}

			dmSettings.dmPelsWidth	= m_iWidth;	
			dmSettings.dmPelsHeight	= m_iHeight;
	
			int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

			if(result != DISP_CHANGE_SUCCESSFUL)
			{
				io::Logger::Log( kge::io::ELM_Error , 
					"The Display Mode(%ix%i) Not Compatible." ,
					m_iWidth , m_iHeight );
				return;
			}
		}
	} // ToFullScreen

	//------------------------------------------------------------------------------------
	// Show the window
	//------------------------------------------------------------------------------------
	void Win32::Show()
	{
		ShowWindow(m_pHwnd, SW_SHOW);	
		SetActiveWindow(m_pHwnd);
		SetForegroundWindow(m_pHwnd);

	} // Show

	//------------------------------------------------------------------------------------
	// Hide the window
	//------------------------------------------------------------------------------------
	void Win32::Hide()
	{
		ShowWindow(m_pHwnd, SW_HIDE);	

	} // Hide

	//------------------------------------------------------------------------------------
	// Returns the window Handler so you can work with the window.
	//------------------------------------------------------------------------------------
	void* Win32::GetHandler()
	{
		return m_pHwnd;
		
	} // GetHandler

	//-----------------------------
	// 
	//-----------------------------
	void Win32::SendQuitEvent()
	{
		PostQuitMessage(0);

	} // SendQuitEvent
	
	//-----------------------------
	// Resize window
	//-----------------------------
	void Win32::Resize(unsigned int width, unsigned int height, bool fullscreen)
	{
		m_iWidth = width;
		m_iHeight = height;
		m_bFullscreen = fullscreen;

		DWORD style = fullscreen ?
			WS_POPUP | WS_SYSMENU :
			WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX;

		RECT rc = {0, 0, width, height};
		AdjustWindowRect(&rc, style, FALSE);

		SetWindowPos(m_pHwnd, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);

		SetWindowLongPtr(m_pHwnd, GWL_STYLE, style);
		SetWindowPos(m_pHwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowWindow(m_pHwnd, SW_SHOW);
	}

} // io

} // kge

#endif // WIN32