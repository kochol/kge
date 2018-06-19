// File name: Renderer.cpp
// Des: This class is the interface for renderers in KGE
// Date: 30/05/1388
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/gfx/Renderer.h"
#include "../../Include/gui/GuiManager.h"
#include "../../Include/sn/Camera.h"

extern kge::gfx::Renderer	*	g_pRenderer;

namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Renderer::Renderer():m_nIndexBufferID(MAXID), m_nVertexDecID(MAXID) , m_clbk_OnLost(0), 
		m_clbk_OnReset(0), m_iBatchCount(1), m_pPosMap(NULL),
		m_eRendererPipeline(ERP_Forward), m_pDifMap(NULL), m_pNorMap(NULL), 
		m_pScreenQuad(NULL)
	{
		m_ppVertexDecs = KGE_NEW_ARRAY(VertexDec*, EVT_Count);
		core::stringc s;
		for (int i = 0; i < EVT_Count; i++)
		{
			s = i;
			m_ppVertexDecs[i] = KGE_NEW(VertexDec)(s);
		}

		g_pRenderer = this;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Renderer::~Renderer()
	{
		for (int i = 0; i < EVT_Count; i++)
			KGE_DELETE(m_ppVertexDecs[i], VertexDec);
		KGE_DELETE_ARRAY(m_ppVertexDecs);
		if (m_pPosMap)
		{
			m_pPosMap->DecRef();
			m_pPosMap = NULL;
		}
		if (m_pDifMap)
		{
			m_pDifMap->DecRef();
			m_pDifMap = NULL;
		}
		if (m_pNorMap)
		{
			m_pNorMap->DecRef();
			m_pNorMap = NULL;
		}
		g_pRenderer = NULL;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Returns the Device class pointer.
	//------------------------------------------------------------------------------------
	Renderer* Renderer::GetSingletonPtr()
	{ 
		return g_pRenderer;

	} // GetSingletonPtr

	void Renderer::SetCallbacks( kgeRenderCallback clbk_OnLost, kgeRenderCallback clbk_OnReset )
	{
		m_clbk_OnLost = clbk_OnLost;
		m_clbk_OnReset = clbk_OnReset;
	}

	void Renderer::SetMaxTextureSize(u32 maxTextureSize)
	{
		m_Params.MaxTextureSize = maxTextureSize;
	}

	u32 Renderer::GetMaxTextureSize() const
	{
		return m_Params.MaxTextureSize;
	}

	//------------------------------------------------------------------------------------
	//  Sets the renderer pipeline
	//------------------------------------------------------------------------------------
	void Renderer::SetRendererPipeline( RendererPipeline rp )
	{
		if (m_eRendererPipeline == rp)
			return;

		m_eRendererPipeline = rp;

		if (rp == ERP_Forward)
		{
			if (m_pPosMap)
			{
				m_pPosMap->DecRef();
				m_pPosMap = NULL;
			}
			if (m_pDifMap)
			{
				m_pDifMap->DecRef();
				m_pDifMap = NULL;
			}
			if (m_pNorMap)
			{
				m_pNorMap->DecRef();
				m_pNorMap = NULL;
			}
		}
		else if (rp == ERP_Deferred)
		{
			CreateRenderableTexture(&m_pPosMap, m_Params.Width, m_Params.Height,
				ETF_R32F, false);
			CreateRenderableTexture(&m_pDifMap, m_Params.Width, m_Params.Height,
				ETF_A8R8G8B8, false);
			CreateRenderableTexture(&m_pNorMap, m_Params.Width, m_Params.Height,
				ETF_R16G16F, false);
		}

	} // SetRendererPipeline

	//------------------------------------------------------------------------------------
	// Draw a screen quad on screen. Note the 2D mode must be enabled before this action
	//------------------------------------------------------------------------------------
	void Renderer::DrawScreenQuad()
	{
		if (!m_pScreenQuad)
		{
			m_pScreenQuad = m_pDevice->GetGuiManager()->AddImage(NULL, core::RectI(0, 0, m_Params.Width, m_Params.Height));
		}

		m_pScreenQuad->Render();

	} // DrawScreenQuad

	//------------------------------------------------------------------------------------
	// Sets the camera far plane as screen quad vectors.
	//------------------------------------------------------------------------------------
	void Renderer::SetFarPlaneAsScreenQuad( sn::Camera* pCam )
	{
		if (!m_pScreenQuad)
		{
			m_pScreenQuad = m_pDevice->GetGuiManager()->AddImage(NULL, core::RectI(0, 0, m_Params.Width, m_Params.Height));
		}
// 		math::Frustum* pFur = (math::Frustum*)pCam->GetFrustum();
// 		math::Vector v		= pFur->GetFarLeftUpPoint();
// 		m_pScreenQuad->m_pVerts[0].pos.X = v.x;
// 		m_pScreenQuad->m_pVerts[0].pos.Y = v.y;
// 		m_pScreenQuad->m_pVerts[0].pos.Z = v.z;
// 		v = pFur->GetFarRightUpPoint();
// 		m_pScreenQuad->m_pVerts[1].pos.X = v.x;
// 		m_pScreenQuad->m_pVerts[1].pos.Y = v.y;
// 		m_pScreenQuad->m_pVerts[1].pos.Z = v.z;
// 		v = pFur->GetFarLeftDownPoint();
// 		m_pScreenQuad->m_pVerts[2].pos.X = v.x;
// 		m_pScreenQuad->m_pVerts[2].pos.Y = v.y;
// 		m_pScreenQuad->m_pVerts[2].pos.Z = v.z;
// 		v = pFur->GetFarRightDownPoint();
// 		m_pScreenQuad->m_pVerts[3].pos.X = v.x;
// 		m_pScreenQuad->m_pVerts[3].pos.Y = v.y;
// 		m_pScreenQuad->m_pVerts[3].pos.Z = v.z;
		m_pScreenQuad->m_pVerts[0].pos.X = -1.0f;
		m_pScreenQuad->m_pVerts[0].pos.Y = 1.0f;
		m_pScreenQuad->m_pVerts[0].pos.Z = 0.5f;

		m_pScreenQuad->m_pVerts[1].pos.X = 1;
		m_pScreenQuad->m_pVerts[1].pos.Y = 1;
		m_pScreenQuad->m_pVerts[1].pos.Z = 0.5;

		m_pScreenQuad->m_pVerts[2].pos.X = -1;
		m_pScreenQuad->m_pVerts[2].pos.Y = -1;
		m_pScreenQuad->m_pVerts[2].pos.Z = 0.5;

		m_pScreenQuad->m_pVerts[3].pos.X = 1;
		m_pScreenQuad->m_pVerts[3].pos.Y = -1;
		m_pScreenQuad->m_pVerts[3].pos.Z = 0.5;

	} // SetFarPlaneAsScreenQuad

	//------------------------------------------------------------------------------------
	// Adds VertexDec pointers
	//------------------------------------------------------------------------------------
	void Renderer::AddVertexDec( VertexDec* pVD )
	{
		m_vVertexDecs.push_back(pVD);

	} // AddVertexDec

	//------------------------------------------------------------------------------------
	// Adds hardware buffer pointers
	//------------------------------------------------------------------------------------
	void Renderer::AddHardwareBuffer( HardwareBuffer* pHB )
	{
		m_vBuffers.push_back(pHB);

	} // AddHardwareBuffer

} // gfx

} // kge
