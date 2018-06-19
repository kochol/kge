// File name: Control.h
// Des: This file is the base of the gui controls.
// Date: 4/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../include/kge/gui/Control.h"

namespace kge
{
namespace gui
{
	//------------------------------------------------------------------------------------
	// Checks if control clicked.
	//------------------------------------------------------------------------------------
	bool Control::CheckClick(int X, int Y)
	{
		if (X >= m_Rect.X && X <= m_Rect.X + m_Rect.Width &&
			Y >= m_Rect.Y && Y <= m_Rect.Y + m_Rect.Height)
		{
			return true;
		}
		return false;

	} // CheckClick

	//------------------------------------------------------------------------------------
	// Connect an event 
	//------------------------------------------------------------------------------------
	void Control::Connect(EventType eventtype, core::Functor2<void, Control*, EventArgs*>* Function)
	{
		m_vEventTypes.push_back(eventtype);
		m_vFunctors.push_back(Function);

	} // Connect

	//------------------------------------------------------------------------------------
	// Sets the control position.
	//------------------------------------------------------------------------------------
	void Control::SetPosistion(int X, int Y)
	{
		m_Rect.X = X;
		m_Rect.Y = Y;
		Update();

	} // SetPosistion

	//------------------------------------------------------------------------------------
	// Sets Control size.
	//------------------------------------------------------------------------------------
	void Control::SetSize(int Width, int Height)
	{
		m_Rect.Width  = Width;
		m_Rect.Height = Height;
		Update();

	} // SetSize

	//------------------------------------------------------------------------------------
	// Sets the control rect.
	//------------------------------------------------------------------------------------
	void Control::SetRect(int X, int Y, int Width, int Height)
	{
		m_Rect.X = X;
		m_Rect.Y = Y;
		m_Rect.Width  = Width;
		m_Rect.Height = Height;
		Update();
		
	} // SetRect

	//------------------------------------------------------------------------------------
	// Sets the control rect.
	//------------------------------------------------------------------------------------
	void Control::SetRect(core::RectI rect)
	{
		m_Rect = rect;
		Update();

	} // SetRect

} // gui

} // kge
