#include "InputMgrJava.h"

kge::io::InputMgrJava*	g_pJavaInputMgr = NULL;

namespace kge
{
	namespace io
	{
		// Constructor
		InputMgrJava::InputMgrJava()
		{
			g_pJavaInputMgr = this;
		}

		void InputMgrJava::sendKeyPressed(int key)
		{
			KeyPressed((KeyCode)key);
		}

		void InputMgrJava::sendKeyReleased(int key)
		{
			KeyReleased((KeyCode)key);
		}

		void InputMgrJava::sendMouseMoved(int x, int y)
		{
			kge::io::MouseState ms;
			ms.X.abs = x;
			ms.X.absOnly = true;
			ms.Y.abs = y;
			ms.Y.absOnly = true;
			MouseMoved(ms);
		}

		void InputMgrJava::sendMouseScrolled(int z)
		{
		}

		void InputMgrJava::sendMousePressed(int id)
		{
			kge::io::MouseState ms;
			//ms.X.abs = x;
			ms.X.absOnly = true;
			//ms.Y.abs = y;
			ms.Y.absOnly = true;
			ms.buttons = id;
			MousePressed(ms, (MouseButtonID)id);
		}

		void InputMgrJava::sendMouseReleased(int id)
		{
			kge::io::MouseState ms;
			//ms.X.abs = x;
			ms.X.absOnly = true;
			//ms.Y.abs = y;
			ms.Y.absOnly = true;
			MouseReleased(ms, (MouseButtonID)id);
		}

	} // io

} // kge
