#ifndef KGE_INPUTMANAGER_H
#define KGE_INPUTMANAGER_H

#include <string>
#include <map>

#include "kgedef.h"
#include "Keyboard.h"
#include "Mouse.h"

namespace kge
{
	namespace io
	{
		//! The input manager that manages the inputs from devices
		class KGE_API InputManager
		{
		public:

			//! Destructor
			virtual ~InputManager();

			//! Initialise the InputManager
			/**
				\param hwnd The window handle
			 */
			virtual void Initialise( void* hwnd ) = 0;

			//! Capture the inputs
			/** Only call this function one time per frame
			 */
			virtual void Capture() = 0;
 
			//! Add a KeyListener for handling the keyboard events
			void AddKeyListener( KeyListener *keyListener, const std::string& instanceName );

			//! Add a MouseListener for handling the mouse events
			void AddMouseListener( MouseListener *mouseListener, const std::string& instanceName );

			//! Removes a KeyListener by its name.
			void RemoveKeyListener( const std::string& instanceName );

			//! Removes a MouseListener by its name.
			void RemoveMouseListener( const std::string& instanceName );
 
			//! Removes a KeyListener by its pointer.
			void RemoveKeyListener( KeyListener *keyListener );

			//! Removes a MouseListener by its pointer.
			void RemoveMouseListener( MouseListener *mouseListener );
 
			//! Removes all registered listeners
			void RemoveAllListeners( void );

			//! Removes all registered KeyListener
			void RemoveAllKeyListeners( void );

			//! Removes all registered MouseListener
			void RemoveAllMouseListeners( void );

			//! Sets the window size
			virtual void setWindowExtents( int width, int height );
 
			//! Returns the mouse device pointer
			Mouse*    GetMouse() { return m_pMouse; }

			//! Returns the keyboard device pointer
			Keyboard* GetKeyboard() { return m_pKeyboard; }
 
			//! Returns the InputManager pointer
			static InputManager* getSingletonPtr( );

		protected:
			InputManager( void );
			InputManager( const InputManager& ) { }
			InputManager & operator = ( const InputManager& );
 
			bool KeyPressed ( const KeyCode &key );
			bool KeyReleased( const KeyCode &key );
 
			bool MouseMoved( const MouseState &e );
			bool MousePressed( const MouseState &e, MouseButtonID id );
			bool MouseReleased( const MouseState &e, MouseButtonID id );
  
			Mouse        *m_pMouse;
			Keyboard     *m_pKeyboard;
  
			std::map<std::string, KeyListener*> mKeyListeners;
			std::map<std::string, MouseListener*> mMouseListeners;
 
			std::map<std::string, KeyListener*>::iterator itKeyListener;
			std::map<std::string, MouseListener*>::iterator itMouseListener;
 
			std::map<std::string, KeyListener*>::iterator itKeyListenerEnd;
			std::map<std::string, MouseListener*>::iterator itMouseListenerEnd;
 
			static InputManager *mInputManager;		

		}; // InputManager

	} // io

} // kge

#endif // KGE_INPUTMANAGER_H
