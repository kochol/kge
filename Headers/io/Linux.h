// File name: Linux.h
// Des: in class vazife keshidan panjare ro to Linux bar ohde darad. omidvaram betonam
//		besazamesh.
// Date: 23/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef LINUX

#ifndef LINUX_H
#define LINUX_H

#include "../../include/kge/io/Iwindow.h"
#include <GL/glx.h>

struct LinuxHandler
{
	Display *m_pDisplay;
	Window   m_Window;
	bool	 m_bDoubleBuffer;

}; // LinuxHandler

namespace kge
{
namespace io
{
class Linux: public IWindow
{
public:
	// sazande khali
	Linux();

	// mokhareb
	virtual ~Linux();

	// Create a new window for rendering in linux.
	virtual bool Init(int Width, int Height, int Bits, bool Fullscreen, int ra);

	// Run the program.
	virtual bool Run();

	// esme panjare ra avaz mikone P1= esme panjare
	virtual void SetCaption(const char* name);

	virtual void* GetHandler();

	// set mouse cursor
	void SetCursor(CURSOR_TYPES cur) {}

	bool bUseStencile;

protected:
	Display *m_pDisplay;
	Window   m_Window;
    XEvent   event;
	bool     m_bDoubleBuffered;

	virtual void ToFullScreen();

}; // Linux

} // io

} // kge

#endif // LINUX_H

#endif // LINUX
