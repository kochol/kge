// File name: Mouse.h
// Des: In class baraye kar ba mouse hast.
// Date: 6/10/1386 (jalali)
// Programmer: Ali Akbar Mohamadi(Kochol),Pooya Shahinfar(Pswin)

#ifndef MOUSE_H
#define MOUSE_H

#include "../KgeUnknown.h"
#include "../math/Vector.h"
#include "../core/Functor.h"
#include "../gui/Events.h"
#include <vector>

namespace kge
{
namespace io
{
	enum MouseButton
	{
		EMB_Left,
		EMB_Middle,
		EMB_Right,
		EMB_WheelUp,
		EMB_WheelDown,
		EMB_None

	}; // BUTTONS

class KGE_API Mouse: public KgeUnknown
{
public:

	//! Constructor
	Mouse() {}

	//! Destructor
	~Mouse();

	//! Get the mouse position
	/*!
	 \param pos Fill the pos with mouse position.
	 */
	void GetPosition(math::Vector2I &pos);
	
	//! Returns the mouse position.
	math::Vector2I GetPosition();

	//! Set kardane mogheiate mouse.
	/*!
	 \param pos Mahali ke mouse mikhahid gharar begirad.
	 */
	void SetPosition(const math::Vector2I& pos);

	//! Set kardane mogheiate mouse.
	/*!
	 \param x Mahale ofoghi ke mouse mikhahid gharar begirad.
	 \param y Mahale Amodi ke mouse mikhahid gharar begirad.
	 */
	void SetPosition(int x, int y);

	//! check kardane feshorde shodane dokme haye mouse
	/*!
	 \param button name dokmeie ke garar ast check shavad ra moshakhas mikonad
	 */
	bool ButtonDown(MouseButton btn);

	//! Connect an event 
	virtual void Connect(gui::EventType eventtype,
		core::Functor1<void, gui::EventArgs*>* Function);

}; // Mouse

} // io

} // kge

#endif // MOUSE_H