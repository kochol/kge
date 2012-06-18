#include "WinWindow.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#include"../include/Logger.h"

#pragma comment(lib, "Winmm.lib")

// * * * * * * * *
// G O L O B A L S
// * * * * * * * *
MSG msg;
bool g_bIsOpenGL;

// Here is our WndProc (Window Procedure).														
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT    ps;

	// This checks what the message is.
    switch(iMsg)
    {
		case WM_CREATE: // This message is sent when the window is created.
			break;

		case WM_SIZE: // This message happens when we resize the window.
			break;

		case WM_PAINT: // This message is sent to the WndProc when the window needs to be repainted.
			if (g_bIsOpenGL)
			{
				BeginPaint(hwnd, &ps);
				EndPaint(hwnd, &ps);
			}
			break;

		case WM_DESTROY: // This message is sent when it's time to destroy the window.
			PostQuitMessage(0);
			SendMessage(hwnd, WM_QUIT, 0, 0);

			break;

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
	WinWindow::WinWindow()
	{
		m_pHwnd = NULL;
		m_phInst = NULL;
		
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	WinWindow::~WinWindow()
	{
		// Return to desktop
		if (m_bFullscreen && g_bIsOpenGL)
			ChangeDisplaySettings(NULL, 0);

		UnregisterClass("KGE003",m_phInst);		

	} // Destructor

	//------------------------------------------------------------------------------------
	// Initialize the window
	//------------------------------------------------------------------------------------
	bool WinWindow::Init(InitParameters& params)
	{
		m_iWidth = params.Width;
		m_iHeight = params.Height;
		m_iBits = params.bits;
	    m_bFullscreen = params.FullScreen;
		g_bIsOpenGL = params.RendererName == "ogl";

		m_phInst = GetModuleHandle(0);
		WNDCLASSEX  wndclass   = {0};
		wndclass.cbSize        = sizeof(WNDCLASSEX);
		wndclass.style         = CS_HREDRAW | CS_VREDRAW;		
		wndclass.lpfnWndProc   = (WNDPROC)WndProc;
		wndclass.cbClsExtra    = 0;
		wndclass.cbWndExtra    = 0;
		wndclass.hInstance     = m_phInst;						
		wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		wndclass.lpszMenuName  = NULL;
		wndclass.lpszClassName = "KGE003";
		wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW); // defualt cursor.
		RegisterClassEx(&wndclass);								

		// Age fullscreen va opengl bod 
		if (m_bFullscreen && g_bIsOpenGL)
			ToFullScreen();

		// size client panjare ro daghighan ba size darkhsti set mikonim.
		RECT rWin;
		rWin.left = 0;
		rWin.right = m_iWidth;
		rWin.top = 0;
		rWin.bottom = m_iHeight;
		
		kge::ul32 style = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		if (!m_bFullscreen)
			style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN 
					| WS_CLIPSIBLINGS;
		
		AdjustWindowRect(&rWin, style, FALSE);

		// Hesab kardane makane panjare dar vasate safhe.
		kge::s32 nLeft = (GetSystemMetrics(SM_CXSCREEN) - (rWin.right - rWin.left)) / 2;
		kge::s32 nTop = (GetSystemMetrics(SM_CYSCREEN) - (rWin.bottom  - rWin.top)) / 2;

		if (m_bFullscreen)
		{
			nLeft = 0;
			nTop  = 0;
		}

		m_pHwnd = CreateWindowEx(NULL,"KGE003", "KGE 0.1.0", style,
					nLeft, nTop, rWin.right  - rWin.left,
					rWin.bottom - rWin.top, NULL, NULL, m_phInst, NULL);	
		// hwnd created
		ShowWindow(m_pHwnd, SW_SHOW);	
		UpdateWindow(m_pHwnd);			

		SetActiveWindow(m_pHwnd);
		SetForegroundWindow(m_pHwnd);

		params.hwnd = m_pHwnd;

		io::Logger::Info("Creating Win32 Window.");

		return true;

	} // Init

	//------------------------------------------------------------------------------------
	// Recive messages from OS and handle them
	//------------------------------------------------------------------------------------
	bool WinWindow::Run()
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
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
	void WinWindow::SetCaption(const char* name)
	{
		SetWindowText(m_pHwnd, name);
		
	} // SetCaption

	//------------------------------------------------------------------------------------
	// set mouse cursor
	//------------------------------------------------------------------------------------
	void WinWindow::SetCursor(CURSOR_TYPES cur)
	{

	} // SetCursor

	//------------------------------------------------------------------------------------
	// Make the window fullscreen mode
	//------------------------------------------------------------------------------------
	void WinWindow::ToFullScreen()
	{
		if(m_bFullscreen)
		{ // Az Windowed be Fullscreen
			DEVMODE dmSettings;									
			memset(&dmSettings,0,sizeof(dmSettings));			

			if(!EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&dmSettings))
			{
				io::Logger::Error("Could Not Enum Display Settings.");
				return;
			}

			dmSettings.dmPelsWidth	= m_iWidth;	
			dmSettings.dmPelsHeight	= m_iHeight;
	
			int result = ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN);	

			if(result != DISP_CHANGE_SUCCESSFUL)
			{
				io::Logger::Error("The Display Mode(%ix%i) Not Compatible." ,
					m_iWidth , m_iHeight );
				return;
			}
		}
	} // ToFullScreen

	//------------------------------------------------------------------------------------
	// Show the window
	//------------------------------------------------------------------------------------
	void WinWindow::Show()
	{
		ShowWindow(m_pHwnd, SW_SHOW);	
		SetActiveWindow(m_pHwnd);
		SetForegroundWindow(m_pHwnd);

	} // Show

	//------------------------------------------------------------------------------------
	// Hide the window
	//------------------------------------------------------------------------------------
	void WinWindow::Hide()
	{
		ShowWindow(m_pHwnd, SW_HIDE);	

	} // Hide

	//------------------------------------------------------------------------------------
	// Returns the window Handler so you can work with the window.
	//------------------------------------------------------------------------------------
	void* WinWindow::GetHandler()
	{
		return m_pHwnd;
		
	} // GetHandler
	
	} // io

} // kge

#endif // KGE_PLATFORM == KGE_PLATFORM_WINDOWS

