// File name: Win32.h
// Des: in class vazife keshidan panjare ro to win32 bar ohde darad.
// Date: 10/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef WIN32

#ifndef WIN32_H
#define WIN32_H

#define NOMINMAX
#include <windows.h>
#include "../../include/io/iwindow.h"

namespace kge
{
namespace io
{
class Win32: public IWindow
{
public:
	// sazande khali
	Win32();

	// mokhareb 
	virtual ~Win32();

			//! Initialize the window
	virtual bool Init(int Width, int Height, int Bits, bool Fullscreen
					  , int ra);

	//! Recive messages from OS and handle them
	virtual bool Run();

	// esme panjare ra avaz mikone P1= esme panjare
	virtual void SetCaption(const char* name);

	// set mouse cursor
	void SetCursor(CURSOR_TYPES cur){}

	//! Show the window
	virtual void Show();

	//! Hide the window
	virtual void Hide();

	// Returns the window Handler so you can work with the window.
	void* GetHandler();

    //! 
    virtual void SendQuitEvent();

    //! Resize window
    virtual void Resize(unsigned int width, unsigned int height, bool fullscreen);

protected:

	HWND m_pHwnd;
	HINSTANCE m_phInst;
	
	void ToFullScreen();

}; // Win32 class

} // io

} // kge

#endif // WIN32_H

#endif // WIN32