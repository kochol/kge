// File name: GuiManager.cpp
// Des: This class manages guis.
// Date: 8/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol) & Hadi Robati( hadirobati )

#include "../../include/kge/gui/GuiManager.h"
#include "../../include/kge/core/mem_fun.h"



namespace kge
{
namespace gui
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	GuiManager::GuiManager(gfx::Renderer* pRenderer, sn::SceneManager* pSmgr):
		m_pFirstFont(NULL)
	{
		m_bClicked	   = false;
		m_bMouseMotion = false;
		m_bMouseDown   = false;
		m_bMouseUp	   = false;
		m_pSmgr		   = pSmgr;
		m_pRenderer	   = pRenderer;
		m_pMouse	   = KGE_NEW(io::Mouse)();
		m_pMat		   = KGE_NEW(gfx::Material)();
		// Create shaders
		// Create deferred shading shaders
		const char strVs[] =
			"float4x4 matViewProjection;\n"\
			"\n"\
			"void main(in float4 inPos: POSITION0, in float2 inTex: TEXCOORD0, out float4 outPos: POSITION, out float2 outTex: TEXCOORD0)\n"\
			"{\n"\
			"	outPos = mul(inPos, matViewProjection);\n"\
			"	outTex = inTex;\n"\
			"}\n";

		m_pMat->shader->m_pVertexShader	= m_pRenderer->CreateVertexShaderFromString(strVs, "main", gfx::ESV_VS3);
		if (m_pMat->shader->m_pVertexShader)
		{
			m_VsmatViewProj	= m_pMat->shader->m_pVertexShader->GetConstatnt("matViewProjection");
		}

		const char strPs[] =
			"sampler2D DifMap: register(s0);\n"\
			"float4 main(in float2 inTex: TEXCOORD0) : COLOR0\n"\
			"{\n"\
			"	return tex2D(DifMap, inTex);\n"\
			"}\n";

		m_pMat->shader->m_pPixelShader = m_pRenderer->CreatePixelShaderFromString(strPs, "main", gfx::ESV_PS3);

		// Create index buffer		
		m_pIndices	   = KGE_NEW_ARRAY(u16, 6);
		m_pIndices[0]  = 0;
		m_pIndices[1]  = 1;
		m_pIndices[2]  = 2;
		m_pIndices[3]  = 1;
		m_pIndices[4]  = 3;
		m_pIndices[5]  = 2;
		m_pIndexBuffer = m_pRenderer->CreateIndexBuffer(m_pIndices, 6);
		m_pFontManager		    = new ResourceManager<Font>(NULL);
		m_meaClick.m_EventType  = EET_Click;
		m_meaClick.Mousebutton  = io::EMB_Left;
		m_meaClick.Xrel		    = 0;
		m_meaClick.Yrel		    = 0;
		m_meaMotion.m_EventType = EET_MouseMotion;
		m_pMouse->Connect(EET_Click, core::mem_fun(this, &GuiManager::OnClick));
		m_pMouse->Connect(EET_MouseMotion, core::mem_fun(this, &GuiManager::OnMouseMotion));
		m_pMouse->Connect(EET_MouseDown, core::mem_fun(this, &GuiManager::OnMouseDown));
		m_pMouse->Connect(EET_MouseUp, core::mem_fun(this, &GuiManager::OnMouseUp));

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	GuiManager::~GuiManager()
	{
		KGE_DELETE_ARRAY(m_pIndices);
		if (m_pFontManager)
		{
			delete m_pFontManager;
			m_pFontManager = NULL;
		}
		KGE_DELETE(m_pMouse, Mouse);
		KGE_DELETE(m_pMat, Material);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Adds an image to the screen.
	//------------------------------------------------------------------------------------
	Image* GuiManager::AddImage(const char* ImageFile, core::RectI rect)
	{
		Image* img		 = new Image(m_pIndexBuffer, rect, m_pRenderer);
		if (ImageFile)
			img->m_BackImage = m_pSmgr->AddTexture(ImageFile);
		else
			img->m_BackImage = NULL;

		m_vControls.push_back(img);

		return img;

	} // AddImage

	//------------------------------------------------------------------------------------
	// Adds an static text and return its pointer.
	//------------------------------------------------------------------------------------
	Text* GuiManager::AddText(const wchar_t* text, core::RectI rect, int FontSize, const char* FontName)
	{
		Text* t = new Text(rect, m_pRenderer, text, FontSize, m_pFirstFont);

		m_vControls.push_back(t);

		return t;

	} // AddText

	//------------------------------------------------------------------------------------
	// Draws all guis.
	//------------------------------------------------------------------------------------
	void GuiManager::DrawAll()
	{
		// Prepare for 2D drawing.
		m_pRenderer->Enable(gfx::ERF_2D);
		m_pRenderer->Disable(gfx::ERF_DepthBuffer);
		m_pRenderer->SetMaterial(m_pMat);
		// set matrix
		math::Matrix mat = m_pRenderer->GetTransForm(gfx::ETM_Projection);
		m_pMat->shader->m_pVertexShader->SetConstant(m_VsmatViewProj, mat.m_fMat, 16);
		bool bLighting = m_pRenderer->GetRenderFlag(gfx::ERF_Lighting);
		if (bLighting)
			m_pRenderer->Disable(gfx::ERF_Lighting);

		// Check for mouse.
		math::Vector2I mPos  = m_pMouse->GetPosition();
		//bool bLeftClick      = m_pMouse->ButtonDown(io::EMB_Left);
		//bool MouseOut		 = true;
		int i				 = 0;

		// Events
		core::Functor2<void, Control*, EventArgs*>* funClick = NULL; 
		Control									  * ctrClick = NULL;

		// Render Guis.
		for(std::vector<Control*>::iterator it = m_vControls.begin(); 
			it != m_vControls.end(); it++)
		{
			i = 0;
			
			// Click Mouse Anjam Shode bood va oon controll Focuse ghablan nadasht.
			if( m_bClicked && (*it)->GetFocus() == false )
			{
				//Agar click mouse roye oon Control bood
				if( (*it)->CheckClick( mPos.x , mPos.y ) )
				{
					// Focus ro oon Controll gharar mogire
					(*it)->SetFocus( true );
					// agar Controlli ghablan dashtim ke focus rosh boode
					if( m_pFocusedControl )
					{
						// Focus Control ghabli False mishe
						m_pFocusedControl->SetFocus( false );
					}
					m_pFocusedControl = (*it);
				}
			}

			for(std::vector<EventType>::iterator it2 = (*it)->m_vEventTypes.begin(); 
				it2 != (*it)->m_vEventTypes.end(); it2++)
			{
				switch (*it2)  
				{
					//   C L I C K 
				case EET_Click:
					if (m_bClicked)
						if ((*it)->CheckClick(mPos.x, mPos.y))
						{
							funClick = (*it)->m_vFunctors[i];
							ctrClick = *it;
						}
					break;

					//   M O U S E   I N
				case EET_MouseIn:
					if (m_bMouseMotion)
					{
						if (!(*it)->m_bMouseOver)
						{
							if ((*it)->CheckClick(mPos.x, mPos.y))
							{
								(*it)->m_bMouseOver = true;
								(*it)->m_vFunctors[i]->Call(*it, &m_meaMotion);
							}
						}
					}
					break;

					//   M O U S E   O U T 
				case EET_MouseOut:
					if (m_bMouseMotion)
					{
						if ((*it)->m_bMouseOver)
						{
							if (!(*it)->CheckClick(mPos.x, mPos.y))
							{
								(*it)->m_bMouseOver = false;
								(*it)->m_vFunctors[i]->Call(*it, &m_meaMotion);
							}
						}
					}
					break;

					//   M O U S E   M O T I O N
				case EET_MouseMotion:
					if (m_bMouseMotion)
					{
						if ((*it)->CheckClick(mPos.x, mPos.y))
						{
							(*it)->m_vFunctors[i]->Call(*it, &m_meaMotion);
						}
					}
					break;

					//   M O U S E   D O W N
				case EET_MouseDown:
					if (m_bMouseDown)
					{
						if ((*it)->CheckClick(mPos.x, mPos.y))
						{
							(*it)->m_vFunctors[i]->Call(*it, &m_meaMouse);
						}
					}
					break;

					//   M O U S E   U P
				case EET_MouseUp:
					if (m_bMouseUp)
					{
						if ((*it)->CheckClick(mPos.x, mPos.y))
						{
							(*it)->m_vFunctors[i]->Call(*it, &m_meaMouse);
						}
					}
					break;
				case EET_KeyDown:
					if( m_bKeyDown )
					{
						/*if( m_bFocus )
						{
							//(*it)->m_
						}*/
					}
				}
				i++;
			}

			(*it)->Render();
		}

		// Call click function
		if (funClick)
		{
			funClick->Call(ctrClick, &m_meaClick);
		}
		
		// End
		if (bLighting)
			m_pRenderer->Enable(gfx::ERF_Lighting);
		m_pRenderer->Enable(gfx::ERF_DepthBuffer);

		m_bClicked		= false;
		m_bMouseMotion	= false;
		m_bMouseDown	= false;
		m_bMouseUp		= false;

	} // DrawAll

	//------------------------------------------------------------------------------------
	// On Click event.
	//------------------------------------------------------------------------------------
	void GuiManager::OnClick(EventArgs* event)
	{
		m_bClicked   = true;
		m_meaClick.X = ((MouseEventArgs*)event)->X;
		m_meaClick.Y = ((MouseEventArgs*)event)->Y;

	} // OnClick

	//------------------------------------------------------------------------------------
	// On Mouse motion event.
	//------------------------------------------------------------------------------------
	void GuiManager::OnMouseMotion(EventArgs* event)
	{
		m_bMouseMotion			= true;
		m_meaMotion.X			= ((MouseEventArgs*)event)->X;
		m_meaMotion.Y			= ((MouseEventArgs*)event)->Y;
		m_meaMotion.Xrel		= ((MouseEventArgs*)event)->Xrel;
		m_meaMotion.Yrel		= ((MouseEventArgs*)event)->Yrel;
		m_meaMotion.Mousebutton = ((MouseEventArgs*)event)->Mousebutton;

	} // OnMouseMotion

	//------------------------------------------------------------------------------------
	// On Mouse down event.
	//------------------------------------------------------------------------------------
	void GuiManager::OnMouseDown(EventArgs* event)
	{
		m_bMouseDown			= true;
		m_meaMouse.m_EventType	= EET_MouseDown;
		m_meaMouse.X			= ((MouseEventArgs*)event)->X;
		m_meaMouse.Y			= ((MouseEventArgs*)event)->Y;
		m_meaMouse.Xrel			= ((MouseEventArgs*)event)->Xrel;
		m_meaMouse.Yrel			= ((MouseEventArgs*)event)->Yrel;
		m_meaMouse.Mousebutton	= ((MouseEventArgs*)event)->Mousebutton;

	} // OnMouseDown

	//------------------------------------------------------------------------------------
	// On Mouse up event.
	//------------------------------------------------------------------------------------
	void GuiManager::OnMouseUp(EventArgs* event)
	{
		m_bMouseUp				= true;
		m_meaMouse.m_EventType	= EET_MouseUp;
		m_meaMouse.X			= ((MouseEventArgs*)event)->X;
		m_meaMouse.Y			= ((MouseEventArgs*)event)->Y;
		m_meaMouse.Xrel			= ((MouseEventArgs*)event)->Xrel;
		m_meaMouse.Yrel			= ((MouseEventArgs*)event)->Yrel;
		m_meaMouse.Mousebutton	= ((MouseEventArgs*)event)->Mousebutton;

	} // OnMouseUp

	//------------------------------------------------------------------------------------
	// For internal use
	//------------------------------------------------------------------------------------
	bool GuiManager::AddFont( Font** ppOutFont, u32 Handle, const char* FileName, const char* Name, void* ExtraParam )
	{
		*ppOutFont = new Font(Handle, FileName, Name, m_pSmgr);		
		bool b = (*ppOutFont)->Init();
		if (b && !m_pFirstFont)
			m_pFirstFont = * ppOutFont;

		return b;

	} // AddFont

} // gui

} // kge

