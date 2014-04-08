// File name: Control.h
// Des: This file is the base of the gui controls.
// Date: 4/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_CONTROL_H
#define KGE_CONTROL_H

// Include files
#include "../KgeUnknown.h"
#include "Events.h"
#include "../core/Structs.h"
#include "../core/Functor.h"
#include "../kgedef.h"
#include "../gfx/Renderer.h"
#include <vector>

namespace kge
{
//! Graphical User Interface
namespace gui
{
class GuiManager;

//! This class is the base of gui controls.
class KGE_API Control: public KgeUnknown
{
	friend class GuiManager;

public:

	virtual ~Control() {}
	
	//! Checks if control clicked.
	virtual bool CheckClick(int X, int Y);

	//! Connect an event 
	virtual void Connect(EventType eventtype,
						 core::Functor2<void, Control*, EventArgs*>* Function);

	//! Renders the control
	virtual void Render() = 0;

	//! Sets the control position.
	virtual void SetPosistion(int X,
		 					  int Y);

	//! Sets Control size.
	virtual void SetSize(int Width,
						 int Height);

	//! Sets the control rect.
	virtual void SetRect(int X,
						 int Y,
						 int Width,
						 int Height);

	//! Sets the control rect.
	virtual void SetRect(core::RectI rect);

	//! Gets The Focuse Status
	virtual bool GetFocus() { return m_bFocus; }

	//! Set The Focuse Status
	virtual void SetFocus( bool FocusStatus ) { m_bFocus = FocusStatus; }

	// Background image of the control. By changing this value you can modify the control theme.
	kge::gfx::Texture	*	m_BackImage;

protected:


	gfx::HardwareBuffer	*  m_pIndexBuffer;
	
	core::RectI		m_Rect;

	gfx::Renderer*	m_pRenderer;

	bool			m_bMouseOver;

	bool			m_bFocus;

	std::vector
		<EventType>	m_vEventTypes;			// Event types.

	std::vector
		<core::Functor2<void, Control*, EventArgs*>*> m_vFunctors;			// Event functions.

	// Updates the control properties.
	virtual void Update() { }


}; // Control

} // gui

} // kge

#endif // KGE_CONTROL_H
