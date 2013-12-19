// File name: Font.h
// Des: Interface class for static text.
// Date: 15/01/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_TEXT_H
#define KGE_TEXT_H

#include "Control.h"
#include "Font.h"

namespace kge
{
namespace gui
{
	class KGE_API Text : public Control
	{
	public:

		//! Constructor
		Text( core::RectI rect,
			  gfx::Renderer* pRenderer,
			  const wchar_t* text,
			  int FontSize,
			  Font* pFont);

		//! Destructor
		~Text();
		
		//! Set Font Color
		void setColor(const gfx::Color& textColor)
		{
			m_Color = textColor;
		}
			
		//! Get Font Color
		gfx::Color getColor()
		{
			return m_Color;
		}

		//! Renders the static text.
		void Render();

	protected:
		Font*		m_pFont;				// Font pointer
		ul32		m_FontSize;				// Font height
		wchar_t*	m_pText;				// Text to write
		gfx::Color	m_Color;				// Text color
		u32			m_iVcount,
					m_iIcount;

		gfx::HardwareBuffer*	m_pVertexBuffer,
							*	m_pIndexBuffer;

		// Updates the control properties.
		virtual void Update();

		// Create text vertex buffer
		virtual void CreateChars();

		// Calc farsi char
		virtual wchar_t CalcChar(int id, bool prev, bool next);

	}; // Text

} // gui

} // kge

#endif // KGE_TEXT_H
