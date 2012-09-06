#ifndef KGE_WINWINDOW_H
#define KGE_WINWINDOW_H

#include "../include/kgedef.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#define NOMINMAX
#include <windows.h>
#include "../include/KgeWindow.h"

namespace kge
{
	namespace io
	{
		class WinWindow : public KgeWindow
		{
		public:

			//! Constructor
			WinWindow();

			//! Destructor
			~WinWindow();

			//! Initialize the window
			bool Init(InitParameters& params);
						
			//! Recive messages from OS and handle them
			bool Run();

			//! Sets window caption
			void SetCaption(const char* name);

			// set mouse cursor
			void SetCursor(CURSOR_TYPES cur);

			//! Show the window
			void Show();

			//! Hide the window
			void Hide();

			// Returns the window Handler so you can work with the window.
			void* GetHandler();

		protected:
			
			HWND m_pHwnd;
			HINSTANCE m_phInst;

			// Make the window fullscreen mode
			void ToFullScreen();
			
		}; // WinWindow

	} // io

} // kge

#endif // KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#endif // KGE_WINWINDOW_H
