// File name: Events.h
// Des: This file contains event classes.
// Date: 8/1/1387
// Programmer: Ali Akbar Mohammadi(Kochol)


#ifndef KGE_EVENTS_H
#define KGE_EVENTS_H

namespace kge
{
namespace io
{
	enum MouseButton;
	enum KEYS;
}
namespace gui
{

class GuiManager;

//! Event types.
enum EventType
{
	EET_Click = 0,		//<* Click event.
	EET_MouseDown,		//<* Mouse button down event.
	EET_MouseUp,		//<* Mouse button up event.
	EET_MouseMotion,	//<* When mouse moves.
	EET_MouseIn,		//<* When the mouse enter the control.
	EET_MouseOut,		//<* When the mouse out the control.
	EET_MouseWheel,
	EET_KeyDown,		//<* When the keyboard key is down.
	EET_KeyUp			//<* When the keyboard key is released.

}; // EventType

//! Base events class
class EventArgs
{

public:

	//! Returns the event type.
	virtual EventType GetType() { return m_EventType; }

	kge::gui::EventType m_EventType;

};// EventArgs

//! Mouse event 
class MouseEventArgs : public EventArgs
{
public:

	//! Mouse X position
	int X;

	//! Mouse Y position 
	int Y;

	//! Mouse X relative position.
	int Xrel;

	//! Mouse Y relative position.
	int Yrel;

	//! Pressed button
	io::MouseButton Mousebutton;

};// MouseEventArgs

//! Keyboard Event
class KeyEventArgs : public EventArgs
{
public:

	//! Current Key Pressed
	io::KEYS currentKeyDown;

	unsigned char keyState;
}; // Keyboard Event

} // gui

} // kge

#endif // KGE_EVENTS_H
