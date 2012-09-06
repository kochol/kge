#ifndef KGE_KGEWINDOW_H
#define KGE_KGEWINDOW_H

#include "kgedef.h"
#include "KgeUnknown.h"
#include "structs.h"

namespace kge
{

	//! Input/Output classes
	namespace io
	{
		//! The window interface for creating rendering windows on each OS
		class KgeWindow: public KgeUnknown
		{
		public:

			//! Constructor
			KgeWindow() {}

			//! Destructor
			virtual ~KgeWindow() {}

			//! Initialize the window
			virtual bool Init(InitParameters& params)=0;
						
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

		protected:
			u32 	m_iWidth, 
					m_iHeight, 
					m_iBits;
			bool	m_bFullscreen;

		}; // KgeWindow

	} // io

} // kge

#endif // KGE_KGEWINDOW_H