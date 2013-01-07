#include "../include/InputManager.h"
#include "../include/KgeMemory.h"

namespace kge
{
	namespace io
	{
		InputManager *InputManager::mInputManager = 0;

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		InputManager::InputManager(): m_pMouse(0), m_pKeyboard(0)
		{
			mInputManager = this;

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		InputManager::~InputManager()
		{
			KGE_DELETE(m_pKeyboard, Keyboard);
			KGE_DELETE(m_pMouse, Mouse);

		} // Destructor

		//------------------------------------------------------------------------------------
		// Add a KeyListener for handling the keyboard events
		//------------------------------------------------------------------------------------
		void InputManager::AddKeyListener( KeyListener *keyListener, const std::string& instanceName ) 
		{
			//if( m_pKeyboard ) 
			//{
				// Check for duplicate items
				itKeyListener = mKeyListeners.find( instanceName );
				if( itKeyListener == mKeyListeners.end() ) 
				{
					mKeyListeners[ instanceName ] = keyListener;
				}
				else 
				{
					// Duplicate Item
				}
			//}
		} // AddKeyListener
 
		//------------------------------------------------------------------------------------
		// Add a MouseListener for handling the mouse events
		//------------------------------------------------------------------------------------
		void InputManager::AddMouseListener( MouseListener *mouseListener, const std::string& instanceName ) 
		{
			if( m_pMouse ) 
			{
				// Check for duplicate items
				itMouseListener = mMouseListeners.find( instanceName );
				if( itMouseListener == mMouseListeners.end() ) 
				{
					mMouseListeners[ instanceName ] = mouseListener;
				}
				else 
				{
					// Duplicate Item
				}
			}
		} // AddMouseListener
  
		//------------------------------------------------------------------------------------
		// Removes a KeyListener by its name.
		//------------------------------------------------------------------------------------
		void InputManager::RemoveKeyListener( const std::string& instanceName ) 
		{
			// Check if item exists
			itKeyListener = mKeyListeners.find( instanceName );
			if( itKeyListener != mKeyListeners.end() ) 
			{
				mKeyListeners.erase( itKeyListener );
			}
			else 
			{
				// Doesn't Exist
			}
		} // RemoveKeyListener
 
		//------------------------------------------------------------------------------------
		// Removes a MouseListener by its name.
		//------------------------------------------------------------------------------------
		void InputManager::RemoveMouseListener( const std::string& instanceName ) 
		{
			// Check if item exists
			itMouseListener = mMouseListeners.find( instanceName );
			if( itMouseListener != mMouseListeners.end() ) 
			{
				mMouseListeners.erase( itMouseListener );
			}
			else 
			{
				// Doesn't Exist
			}
		} // RemoveMouseListener
 
		//------------------------------------------------------------------------------------
		// Removes a KeyListener by its pointer.
		//------------------------------------------------------------------------------------
		void InputManager::RemoveKeyListener( KeyListener *keyListener ) 
		{
			itKeyListener    = mKeyListeners.begin();
			itKeyListenerEnd = mKeyListeners.end();
			for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) 
			{
				if( itKeyListener->second == keyListener ) 
				{
					mKeyListeners.erase( itKeyListener );
					break;
				}
			}
		} // RemoveKeyListener
 
		//------------------------------------------------------------------------------------
		// Removes a MouseListener by its pointer.
		//------------------------------------------------------------------------------------
		void InputManager::RemoveMouseListener( MouseListener *mouseListener ) 
		{
			itMouseListener    = mMouseListeners.begin();
			itMouseListenerEnd = mMouseListeners.end();
			for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) 
			{
				if( itMouseListener->second == mouseListener ) 
				{
					mMouseListeners.erase( itMouseListener );
					break;
				}
			}
		} // RemoveMouseListener
  
		//------------------------------------------------------------------------------------
		// Removes all registered listeners
		//------------------------------------------------------------------------------------
		void InputManager::RemoveAllListeners() 
		{
			mKeyListeners.clear();
			mMouseListeners.clear();

		} // RemoveAllListeners
 
		//------------------------------------------------------------------------------------
		// Removes all registered KeyListener
		//------------------------------------------------------------------------------------
		void InputManager::RemoveAllKeyListeners() 
		{
			mKeyListeners.clear();

		} // RemoveAllKeyListeners
 
		//------------------------------------------------------------------------------------
		// Removes all registered MouseListener
		//------------------------------------------------------------------------------------
		void InputManager::RemoveAllMouseListeners() 
		{
			mMouseListeners.clear();

		} // RemoveAllMouseListeners

		//------------------------------------------------------------------------------------
		// Sets the window size
		//------------------------------------------------------------------------------------
		void InputManager::setWindowExtents( int width, int height ) 
		{
			// Set mouse region (if window resizes, we should alter this to reflect as well)
			if (m_pMouse)
			{
				const MouseState &mouseState = m_pMouse->getMouseState();
				mouseState.width  = width;
				mouseState.height = height;
			}

		} // setWindowExtents

		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		bool InputManager::KeyPressed( const KeyCode &e ) 
		{
			itKeyListener    = mKeyListeners.begin();
			itKeyListenerEnd = mKeyListeners.end();
			for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) 
			{
				if(!itKeyListener->second->KeyPressed( e ))
					break;
			}
 
			return true;

		} // KeyPressed
 
		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		bool InputManager::KeyReleased( const KeyCode &e ) 
		{
			itKeyListener    = mKeyListeners.begin();
			itKeyListenerEnd = mKeyListeners.end();
			for(; itKeyListener != itKeyListenerEnd; ++itKeyListener ) 
			{
				if(!itKeyListener->second->KeyReleased( e ))
					break;
			}
 
			return true;

		} // KeyReleased
 
		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		bool InputManager::MouseMoved( const MouseState &e ) 
		{
			itMouseListener    = mMouseListeners.begin();
			itMouseListenerEnd = mMouseListeners.end();
			for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) 
			{
				if(!itMouseListener->second->MouseMoved( e ))
					break;
			}
 
			return true;

		} // MouseMoved
 
		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		bool InputManager::MousePressed( const MouseState &e, MouseButtonID id ) 
		{
			itMouseListener    = mMouseListeners.begin();
			itMouseListenerEnd = mMouseListeners.end();
			for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) 
			{
				if(!itMouseListener->second->MousePressed( e, id ))
					break;
			}
 
			return true;

		} // MousePressed
 
		//------------------------------------------------------------------------------------
		//------------------------------------------------------------------------------------
		bool InputManager::MouseReleased( const MouseState &e, MouseButtonID id ) 
		{
			itMouseListener    = mMouseListeners.begin();
			itMouseListenerEnd = mMouseListeners.end();
			for(; itMouseListener != itMouseListenerEnd; ++itMouseListener ) 
			{
				if(!itMouseListener->second->MouseReleased( e, id ))
					break;
			}
 
			return true;

		} // MouseReleased

		InputManager* InputManager::getSingletonPtr( ) 
		{
			if( !mInputManager ) 
			{
			}
 
			return mInputManager;

		} // getSingletonPtr

	} // io

} // kge
