#ifndef KGE_MOUSE_H
#define KGE_MOUSE_H

#include "kgedef.h"

namespace kge
{
	namespace io
	{
		//! Button ID for mouse devices
		enum MouseButtonID
		{
			MB_Left = 0, 
			MB_Right, 
			MB_Middle,
			MB_Button3, 
			MB_Button4,	
			MB_Button5, 
			MB_Button6,	
			MB_Button7

		}; // MouseButtonID

		//! Axis class
		class KGE_API Axis
		{
		public:

			//! Constructor
			Axis() : abs(0), rel(0), absOnly(false) {};

			//! Absoulte and Relative value components
			int abs, rel;

			//! Indicates if this Axis only supports Absoulte (ie JoyStick)
			bool absOnly;

			//! Used internally by OIS
			void clear()
			{
				abs = rel = 0;
			}

		}; // Axis

		/**
			Represents the state of the mouse
			All members are valid for both buffered and non buffered mode
		*/
		class KGE_API MouseState
		{
		public:
			MouseState() : width(50), height(50), buttons(0) {};

			/** Represents the height/width of your display area.. used if mouse clipping
			or mouse grabbed in case of X11 - defaults to 50.. Make sure to set this
			and change when your size changes.. */
			mutable int width, height;

			//! X Axis component
			Axis X;

			//! Y Axis Component
			Axis Y;

			//! Z Axis Component
			Axis Z;

			//! represents all buttons - bit position indicates button down
			int buttons;

			//! Button down test
			inline bool buttonDown( MouseButtonID button ) const
			{
				return ((buttons & ( 1L << button )) == 0) ? false : true;
			}

			//! Clear all the values
			void clear()
			{
				X.clear();
				Y.clear();
				Z.clear();
				buttons = 0;
			}

		}; // MouseState

		/**
			To recieve buffered mouse input, derive a class from this, and implement the
			methods here.
		*/
		class KGE_API MouseListener
		{
		public:
			virtual ~MouseListener() {}
			virtual bool MouseMoved( const MouseState &arg ) = 0;
			virtual bool MousePressed( const MouseState &arg, MouseButtonID id ) = 0;
			virtual bool MouseReleased( const MouseState &arg, MouseButtonID id ) = 0;

		}; // MouseListener

		//! The mouse interface
		class KGE_API Mouse
		{
		public:

			//! Destructor
			virtual ~Mouse() {}

			/** @remarks Returns the state of the mouse - is valid for both buffered and non buffered mode */
			const MouseState& getMouseState() const { return m_State; }

		protected:

			//! The state of the mouse
			MouseState m_State;

		}; // Mouse
		
	} // io

} // kge

#endif // KGE_MOUSE_H
