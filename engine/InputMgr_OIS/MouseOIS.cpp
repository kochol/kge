#include <OISMouse.h>

#include "MouseOIS.h"

namespace kge
{
	namespace io
	{
		// Copy the mouse state
		void MouseOIS::CopyState()
		{
			OIS::MouseState e = mMouse->getMouseState();
			m_State.buttons	= e.buttons;
			m_State.height	= e.height;
			m_State.X.abs	= e.X.abs;
			m_State.X.absOnly= e.X.absOnly;
			m_State.X.rel	= e.X.rel;
			m_State.Y.abs	= e.Y.abs;
			m_State.Y.absOnly= e.Y.absOnly;
			m_State.Y.rel	= e.Y.rel;
			m_State.Z.abs	= e.Z.abs;
			m_State.Z.absOnly= e.Z.absOnly;
			m_State.Z.rel	= e.Z.rel;

		} // CopyState

	} // io

} // kge
