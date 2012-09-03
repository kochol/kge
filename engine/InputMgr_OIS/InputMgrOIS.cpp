#include <sstream>

#include "InputMgrOIS.h"
#include "MouseOIS.h"
#include "KeyboardOIS.h"
#include "../include/KgeMemory.h"

namespace kge
{
	namespace io
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		InputMgrOIS::InputMgrOIS(): mMouse(0), mKeyboard(0)
		{
		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		InputMgrOIS::~InputMgrOIS()
		{
			if( mInputSystem ) 
			{
				if( mMouse ) 
				{
					mInputSystem->destroyInputObject( mMouse );
					mMouse = 0;
				}
 
				if( mKeyboard ) 
				{
					mInputSystem->destroyInputObject( mKeyboard );
					mKeyboard = 0;
				}
  
				// If you use OIS1.0RC1 or above, uncomment this line
				// and comment the line below it
				mInputSystem->destroyInputSystem( mInputSystem );
				//mInputSystem->destroyInputSystem();
				mInputSystem = 0;
 
				// Clear Listeners
				mKeyListeners.clear();
				mMouseListeners.clear();
			}
		} // Destructor

		//------------------------------------------------------------------------------------
		// Initialise the InputManager
		//------------------------------------------------------------------------------------
		void InputMgrOIS::Initialise( void* hwnd )
		{
			if( !mInputSystem ) 
			{
				// Setup basic variables
				OIS::ParamList paramList;    
				size_t windowHnd = (size_t)hwnd;
				std::ostringstream windowHndStr;
  
				// Fill parameter list
				windowHndStr << (unsigned int) windowHnd;
				paramList.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );
 
				// Create inputsystem
				mInputSystem = OIS::InputManager::createInputSystem( paramList );
 
				// If possible create a buffered keyboard
				// (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
				//if( mInputSystem->numKeyboards() > 0 ) {
				if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
					mKeyboard = static_cast<OIS::Keyboard*>( mInputSystem->createInputObject( OIS::OISKeyboard, true ) );
					mKeyboard->setEventCallback( this );
					m_pKeyboard = KGE_NEW(KeyboardOIS)();
					((KeyboardOIS*)m_pKeyboard)->m_pKeyboard = mKeyboard;
				}
 
				// If possible create a buffered mouse
				// (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
				//if( mInputSystem->numMice() > 0 ) {
				if (mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0) {
					mMouse = static_cast<OIS::Mouse*>( mInputSystem->createInputObject( OIS::OISMouse, true ) );
					mMouse->setEventCallback( this );
					m_pMouse = KGE_NEW(MouseOIS)();
					((MouseOIS*)m_pMouse)->mMouse = mMouse;
 				}
 
			}
		} // Initialise

		//------------------------------------------------------------------------------------
		// Capture the inputs
		//------------------------------------------------------------------------------------
		void InputMgrOIS::Capture()
		{
			// Need to capture / update each device every frame
			if( mMouse ) 
			{
				mMouse->capture();
				((MouseOIS*)m_pMouse)->CopyState();
			}
 
			if( mKeyboard ) 
			{
				mKeyboard->capture();
			}
		} // Capture

		bool InputMgrOIS::keyPressed( const OIS::KeyEvent &e )
		{
			return this->KeyPressed((KeyCode)e.key); 
		}
		bool InputMgrOIS::keyReleased( const OIS::KeyEvent &e )
		{
			return this->KeyReleased((KeyCode)e.key); 
		}
 
		bool InputMgrOIS::mouseMoved( const OIS::MouseEvent &e )
		{
			return this->MouseMoved(m_pMouse->getMouseState());
		}
		bool InputMgrOIS::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
		{
			return this->MousePressed(m_pMouse->getMouseState(), (MouseButtonID)id);
		}
		bool InputMgrOIS::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
		{
			return this->MouseReleased(m_pMouse->getMouseState(), (MouseButtonID)id);
		}


	} // io

} // kge
