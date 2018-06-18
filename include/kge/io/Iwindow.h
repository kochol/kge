// File name: Iwindow.h
// Des: in file interface kar ba panjereha hast to platform haye mokhtalef.
// Date: 6/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef IWINDOW_H
#define IWINDOW_H

#include "../core/String.h"
#include "Keyboard.h"
#include "Mouse.h"


namespace kge
{
	//! Graphics classes
	namespace gfx
	{
	
#ifdef WIN32
		// E N U M
		enum RendererAPI;
#endif // WIN32
		
	} // gfx

	//! Input/Output classes
	namespace io
	{
		enum CURSOR_TYPES
		{
			CUR_SHOW,		//!< Show mouse cursor in the window
			CUR_HIDE,		//!< Hide mouse cursor in the window
			CUR_POINTER		//!< Set mouse cursor to the default cursor

		}; // mouse cursors

		//! The window interface for creating rendering windows on each OS	
		class IWindow: public KgeUnknown
		{
		public:

			//! Constructor
			IWindow() {}

			//! Destructor
			virtual ~IWindow() {}

			//! Initialize the window
			virtual bool Init(int Width, int Height, int Bits,
				bool Fullscreen, int ra)=0;
						
			//! Recive messages from OS and handle them
			virtual bool Run()=0;

			//! Sets window caption
			virtual void SetCaption(const char* name)=0;

			// set mouse cursor
			virtual void SetCursor(CURSOR_TYPES cur)=0;

			//! Show the window
			virtual void Show() {}

			//! Hide the window
			virtual void Hide() {}

			// Returns the window Handler so you can work with the window.
			virtual void* GetHandler()=0;

			//! 
			virtual void SendQuitEvent() {}

			//! Resize window
			virtual void Resize(unsigned int width, unsigned int height, bool fullscreen) {}

		protected:
			u32 	m_iWidth, 
					m_iHeight, 
					m_iBits;
			bool	m_bFullscreen;

			// this is only for opengl use.
			virtual void ToFullScreen()=0;

		}; // IWindow

	} // io

} // kge

#endif // IWINDOW_H