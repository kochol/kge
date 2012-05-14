#ifndef KGE_INPUTMGROIS_H
#define KGE_INPUTMGROIS_H

#include <OISMouse.h>
#include <OISKeyboard.h>
#include <OISJoyStick.h>
#include <OISInputManager.h>

#include "../include/InputManager.h"

namespace kge
{
	namespace io
	{
		class InputMgrOIS: public InputManager, public OIS::KeyListener, public OIS::MouseListener
		{
		public:

			//! Constructor
			InputMgrOIS();

			//! Destructor
			~InputMgrOIS();

			//! Initialise the InputManager
			void Initialise( void* hwnd );

			//! Capture the inputs
			void Capture();

		protected:

			bool keyPressed( const OIS::KeyEvent &e );
			bool keyReleased( const OIS::KeyEvent &e );
 
			bool mouseMoved( const OIS::MouseEvent &e );
			bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
			bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );
			
			OIS::Mouse        *mMouse;
			OIS::Keyboard     *mKeyboard;
			OIS::InputManager *mInputSystem;

		}; // InputMgrOIS

	} // io

} // kge

#endif // KGE_INPUTMGROIS_H
