// File name: GuiManager.h
// Des: This class manages guis.
// Date: 8/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_GUIMANAGER_H
#define KGE_GUIMANAGER_H

#include "Image.h"
#include "Text.h"
#include "../io/Mouse.h"
#include <vector>
#include "../ResourceManager.h"

namespace kge
{
namespace gui
{
	class KGE_API GuiManager: public KgeUnknown
	{

	public:

		//! Constructor
		GuiManager(gfx::Renderer* pRenderer, sn::SceneManager* pSmgr);

		//! Destructor
		~GuiManager();

		//! For internal use
		bool AddFont(Font** ppOutFont, u32 Handle, const char* FileName,
					 const char* Name, void* ExtraParam);

		//! Adds an image to the screen.
		Image* AddImage(const char* ImageFile, core::RectI rect);

		//! Adds an static text and return its pointer.
		Text* AddText(const wchar_t* text,			// Text to print.
					  core::RectI rect,				// Rect
			  		  int FontSize = 24,			// Font Size
					  const char* FontName = "Arial");// Font Name 

		//! Draws all guis.
		void DrawAll();

	protected:

		bool				m_bClicked,		// Check for user clicks.
							m_bMouseMotion, // Check for mouse moves.
							m_bMouseDown,	// Check for mouse down.
							m_bMouseUp,		// Check for mouse up.
							m_bKeyDown;		// Check for Key Down.
		u16				*	m_pIndices;		// Global Indices.
		io::Mouse		*	m_pMouse;		// Mouse
		sn::SceneManager*	m_pSmgr;		// Scene Manager pointer.
		gfx::Renderer	*	m_pRenderer;	// Renderer.
		std::vector
			<Control	*>	m_vControls;
		MouseEventArgs		m_meaClick,
							m_meaMotion,
							m_meaMouse;

		gui::Control	*	m_pFocusedControl;

		ResourceManager
				 <Font> *   m_pFontManager;

		gfx::Material	*	m_pMat;

		Font			*	m_pFirstFont;

		// On Click event.
		void OnClick(EventArgs* event);

		// On Mouse motion event.
		void OnMouseMotion(EventArgs* event);

		// On Mouse motion event.
		void OnMouseDown(EventArgs* event);

		// On Mouse motion event.
		void OnMouseUp(EventArgs* event);

	};// END CLASS DEFINITION GuiManager

} // gui

} // kge

#endif // KGE_GUIMANAGER_H
