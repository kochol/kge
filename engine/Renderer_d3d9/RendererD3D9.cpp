#include "RendererD3D9.h"
#include "../include/Logger.h"
#include "VertexBuferDX9.h"
#include "IndexBufferDX9.h"
#include "../include/VertexDeclaration.h"
#include "../include/VertexElement.h"
#include "../include/BlockData.h"
#include "../include/Image.h"
#include "TextureDX9.h"

#include <dxerr.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "DxErr.lib")

// *** Global variables ***
LPDIRECT3D9				g_pD3D = NULL;

// some good multisample modes, list best ones last
const UINT g_nMS = 8;
const D3DMULTISAMPLE_TYPE g_msType[] = { 
	D3DMULTISAMPLE_NONE,
	D3DMULTISAMPLE_2_SAMPLES,
	D3DMULTISAMPLE_3_SAMPLES,
	D3DMULTISAMPLE_4_SAMPLES,
	D3DMULTISAMPLE_5_SAMPLES,
	D3DMULTISAMPLE_6_SAMPLES,
	D3DMULTISAMPLE_7_SAMPLES,
	D3DMULTISAMPLE_8_SAMPLES,
};

// --- Global variables ---

//   H R ( X ) 
#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
		#define HR(x)                                       \
		{                                                   \
			HRESULT hr = x;                                 \
			if(FAILED(hr))                                  \
			{                                               \
				DXTraceW(__FILE__, __LINE__, hr, L#x, TRUE);\
			}                                               \
		}
	#endif

#else
	#ifndef HR
		#define HR(x) x;
	#endif
#endif 

//   S A F E   R E L E A S E
#define SAFERELEASE(x) if (x) { x->Release(); x = NULL; }

namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	RendererD3D9::RendererD3D9() : m_pD3DDevice(NULL), m_pBackBufferSurface(NULL), 
		m_pBackBufferZSurface(NULL), m_pSwapChain(NULL)
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	RendererD3D9::~RendererD3D9()
	{
		SAFERELEASE(m_pD3DDevice);
		SAFERELEASE(g_pD3D);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Initialize the renderer
	//------------------------------------------------------------------------------------
	bool RendererD3D9::Init(InitParameters &params)
	{
		// Create LPDIRECT3D9
		SAFERELEASE(g_pD3D);
		g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		if (!g_pD3D)
		{
			io::Logger::Error("Can't Create Direct3D9.");
			return false;
		}

		// Search for the best depth and stencil format.
		m_bUseStencil = params.UseStencil;
		CheckStencil(params);

		D3DDISPLAYMODE mode = {0};
		if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			return false;

		D3DFORMAT format = mode.Format;
		if (FAILED(g_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			format, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, format)))
		{
			return false;
		}

		// Fill in m_d3dpp
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
		m_d3dpp.BackBufferFormat		= format;
		m_d3dpp.BackBufferWidth			= params.Width;
		m_d3dpp.BackBufferHeight		= params.Height;
		m_d3dpp.Windowed				= !params.FullScreen;
		m_d3dpp.BackBufferCount			= 1;
		m_d3dpp.EnableAutoDepthStencil	= TRUE;
		m_d3dpp.AutoDepthStencilFormat	= m_DSFMT;
		m_d3dpp.MultiSampleType			= m_msType;
		m_d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		m_d3dpp.hDeviceWindow			= (HWND)params.hwnd;
		m_d3dpp.PresentationInterval	= params.VSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

		// fullscreen mode
		if (params.FullScreen)
		{
			m_d3dpp.FullScreen_RefreshRateInHz = mode.RefreshRate;
		}
		// Windowed mode
		else
		{
			m_d3dpp.FullScreen_RefreshRateInHz = 0;
		}

		// Is stencil buffer active?
		if (m_DSFMT == D3DFMT_D24S8 ||
			m_DSFMT == D3DFMT_D24X4S4 ||
			m_DSFMT == D3DFMT_D15S1)
			m_bUseStencil = true;
		else
			m_bUseStencil = false;

		HRESULT hr;
		// Create the device
		hr = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			m_d3dpp.hDeviceWindow,
			m_Behavior, &m_d3dpp, &m_pD3DDevice);

		if (FAILED(hr))
		{
			io::Logger::Error("Can't create Direct3D9 Device.");
			return false;
		}

		// Get the swap chain
		if (FAILED(m_pD3DDevice->GetSwapChain(0, &m_pSwapChain)))
		{
			io::Logger::Error("Can't get the swap chain");
			return false;
		}

		io::Logger::Info("Using Direct3D9 for rendering.");

		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

		return true;

	} // Init

	//------------------------------------------------------------------------------------
	// Clear the render target
	//------------------------------------------------------------------------------------
	bool RendererD3D9::Clear(bool bColor, bool bDepth, bool bStencil)
	{
		if (!bColor && !bDepth && !bStencil)
			return true;

		ul32 Flags;

		if (bColor)
			Flags = D3DCLEAR_TARGET;
		if (bDepth && bColor)
			Flags |= D3DCLEAR_ZBUFFER;
		else if (bDepth)
			Flags = D3DCLEAR_ZBUFFER;
		if (bStencil && bColor && m_bUseStencil)
			Flags |= D3DCLEAR_STENCIL;
		else if (bStencil && bDepth && m_bUseStencil)
			Flags |= D3DCLEAR_STENCIL;
		else if (bStencil && m_bUseStencil)
			Flags = D3DCLEAR_STENCIL;

		if (FAILED(m_pD3DDevice->Clear(0, NULL, Flags, m_cClearColor.c, 1.0f, 0)))
		{
			io::Logger::Error("Can't clear the Pixel, ZBuffer or StencilBuffer.");
			return false;
		}

		return true;

	} // Clear
		
	//------------------------------------------------------------------------------------
	// Begin the rendering
	//------------------------------------------------------------------------------------
	bool RendererD3D9::BeginRendering(bool bColor, bool bDepth, bool bStencil)
	{
		KGEPROFILE;

		Clear(bColor, bDepth, bStencil);

		if (!m_bIsSceneRunning)
		{

			if (FAILED(m_pD3DDevice->BeginScene()))
			{
				io::Logger::Error("Can't Begin Rendering.");
				return false;
			}

			m_bIsSceneRunning = true;

			m_iTriCount  = 0;
			m_iDrawCount = 0;
		}

		return true;

	} // BeginRendering

	//------------------------------------------------------------------------------------
	// End the rendering and draw to back buffer.
	//------------------------------------------------------------------------------------
	bool RendererD3D9::EndRendering()
	{
		KGEPROFILE;

		if (m_bIsSceneRunning)
		{
			HRESULT hr = S_OK;
			m_pD3DDevice->EndScene();
				
			hr = m_pSwapChain->Present(NULL, NULL, NULL, NULL, D3DPRESENT_DONOTWAIT);
			m_bIsSceneRunning = false;

			if (hr == D3DERR_DEVICELOST)
			{
				io::Logger::Warning( "Direct3D device lost.");
			}
		}

		return true;

	} // EndRendering

	//------------------------------------------------------------------------------------
	// Search for the best depth and stencil format.
	//------------------------------------------------------------------------------------
	void RendererD3D9::CheckStencil(const InitParameters &params)
	{
		bool bCheck = false;
		// use stencil buffer
		if (m_bUseStencil)
		{
			m_DSFMT = D3DFMT_D24S8;
			bCheck = CheckStencilFMT();
			if (!bCheck)
			{
				m_DSFMT = D3DFMT_D24X4S4;
				bCheck = CheckStencilFMT();
			}
			if (!bCheck)
			{
				m_DSFMT = D3DFMT_D15S1;
				bCheck = CheckStencilFMT();
			}
		}
		// or use only depth buffer.
		if (!bCheck)
		{
			m_DSFMT = D3DFMT_D32;
			bCheck = CheckStencilFMT();
			if (!bCheck)
			{
				m_DSFMT = D3DFMT_D24X8;
				bCheck = CheckStencilFMT();
			}
			if (!bCheck)
			{
				m_DSFMT = D3DFMT_D16;
				bCheck = CheckStencilFMT();
			}
		}
		// all of types are not supported with device then
		if (!bCheck)
		{
			io::Logger::Error("No Depth/Stencil buffer format supported.");
			return;
		}

		// SELECT MULTISAMPLE TYPE
		UINT nAdapter = D3DADAPTER_DEFAULT;
		//D3DFORMAT fmtAdapter = D3DFMT_A8R8G8B8;
		D3DFORMAT fmtBack = D3DFMT_A8R8G8B8;

		kge::u32 nMSCount;
		if (params.AAMode == EAAM_MSAA)
			nMSCount = params.AAQuality;
		else
			nMSCount = 1;
		for(kge::u32 nMS=nMSCount-1; nMS>=0; nMS--) 
		{
			if (SUCCEEDED(g_pD3D->CheckDeviceMultiSampleType(
				nAdapter
				,D3DDEVTYPE_HAL
				,fmtBack
				,!params.FullScreen
				,g_msType[nMS], &msQuality))) 
			{
				m_msType = g_msType[nMS];
				break;
			}
		} // for [multisample]

		// SELECT VERTEX PROCESSING
		D3DCAPS9 d3dcaps;
		g_pD3D->GetDeviceCaps(nAdapter, D3DDEVTYPE_HAL, &d3dcaps);
		// We have hardware Transformtion and lighting.
		if ((d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
		{
			// 1.case: pure device
			if ( ((d3dcaps.DevCaps & D3DDEVCAPS_PUREDEVICE)!=0) 
					&& (CheckDevice(&d3dcaps, D3DCREATE_PUREDEVICE)==true) ) 
			{
				m_Behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING 
							 | D3DCREATE_PUREDEVICE;
			}
			// 2.case: hardware device
			else if (CheckDevice(&d3dcaps, D3DCREATE_HARDWARE_VERTEXPROCESSING)==true)
			{
				m_Behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
			}
			// 3.case: mixed sw/hw
			else if (CheckDevice(&d3dcaps, D3DCREATE_MIXED_VERTEXPROCESSING)==true)
			{
				m_Behavior = D3DCREATE_MIXED_VERTEXPROCESSING;
			}
		}
		// 4.case: must be sw processing
		else
		{
			m_Behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
		}

		m_d3dCaps = d3dcaps;

	} // Check Stencil

	//------------------------------------------------------------------------------------
	// Check stencil format
	//------------------------------------------------------------------------------------
	bool RendererD3D9::CheckStencilFMT()
	{
		UINT nAdapter = D3DADAPTER_DEFAULT;
		D3DFORMAT fmtAdapter = D3DFMT_X8R8G8B8;
		D3DFORMAT fmtBack = D3DFMT_X8R8G8B8;

		if (FAILED(g_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,D3DUSAGE_DEPTHSTENCIL
			,D3DRTYPE_SURFACE
			,m_DSFMT)))
			return false;
		
		if (FAILED(g_pD3D->CheckDepthStencilMatch(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,fmtBack
			,m_DSFMT)))
			return false;
		
		return true;

	} // CheckstencilFMT

	//------------------------------------------------------------------------------------
	// Check device if it is create able.
	//------------------------------------------------------------------------------------
	bool RendererD3D9::CheckDevice(D3DCAPS9 *pCaps, DWORD dwBehavior)
	{
		if ( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING) ||
			(dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING   )  ) 
		{
			// alpha blending from texture pixels supported
			if ( !(pCaps->TextureCaps & D3DPTEXTURECAPS_ALPHA) ) 
			{
				return false;
			}

			if (pCaps->VertexShaderVersion < D3DVS_VERSION(1,0) )
			{
				return false;
			}
			else
			{
				m_bUseShaders = true;
			}
		}
		return true;

	} // Check Device

	
	//------------------------------------------------------------------------------------
	// Create a vertex buffer from custom vertex type and store it on video memory.
	//------------------------------------------------------------------------------------
	HardwareBuffer* RendererD3D9::CreateVertexBuffer
		(	void* Vertices, 
			u32 VCount, 
			u32 Stride, 
			bool isDynamic 
		)
	{
		// Create the vertex buffer.
		DWORD usage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if (isDynamic)
		{
			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		IDirect3DVertexBuffer9* vb;
		HR(m_pD3DDevice->CreateVertexBuffer( Stride * VCount
			, usage
			, 0
			, pool
			, &vb
			, 0 ));

		// Writing to the buffer.
		void* Buffer;

		if (Vertices)
		{
			HR(vb->Lock(0, 0, &Buffer, 0));
			memcpy(Buffer, Vertices, Stride * VCount);
			HR(vb->Unlock());
		}

		VertexBufferDX9* ppVBout;
		ppVBout			= KGE_NEW(VertexBufferDX9)(VCount, Stride, 0);
		ppVBout->m_pVB	= vb;
		ppVBout->Dynamic(isDynamic);

		if (isDynamic) 
			m_vVBuffers.push_back(ppVBout);

		AddHardwareBuffer(ppVBout);

		return ppVBout;

	} // CreateVertexBuffer

	//------------------------------------------------------------------------------------
	// Removes VertexBufferDX9 from internal list.
	//------------------------------------------------------------------------------------
	void RendererD3D9::RemoveVertexBuffer(VertexBufferDX9* vb)
	{
		const size_t m_vVBuffersSize = m_vVBuffers.size();
		for (size_t i = 0; i < m_vVBuffersSize; ++i)
		{
			if (m_vVBuffers[i] == vb)
			{
				m_vVBuffers[i] = m_vVBuffers[m_vVBuffersSize - 1];
				m_vVBuffers.pop_back();
				return;
			}
		}
	}

	//-------------------------------------------------------
	// Removes IndexBufferDX9 from internal list.
	//-------------------------------------------------------
	void RendererD3D9::RemoveIndexBuffer(IndexBufferDX9* ib)
	{
		const size_t m_vIBuffersSize = m_vIBuffers.size();
		for (size_t i = 0; i < m_vIBuffersSize; ++i)
		{
			if (m_vIBuffers[i] == ib)
			{
				m_vIBuffers[i] = m_vIBuffers[m_vIBuffersSize - 1];
				m_vIBuffers.pop_back();
				return;
			}
		}
	}

	//------------------------------------------------------------------------------------
	// Creates an Index buffer on video memory
	//------------------------------------------------------------------------------------
	HardwareBuffer* RendererD3D9::CreateIndexBuffer( void* Indices, u32 ICount, IndexBufferType eIndexBufferType, bool isDynamic )
	{
		// Create the index buffer.
		DWORD usage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;

		// If the buffer is dynamic set the flags
		if (isDynamic)
		{
			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		// index buffer 16 or 32 bit format
		D3DFORMAT fmt = D3DFMT_INDEX16;
		u32 size = sizeof( u16 );
		if (eIndexBufferType == EIBT_32Bit)
		{
			fmt = D3DFMT_INDEX32;
			size = sizeof( u32 );
		}

		// Create the index buffer
		IDirect3DIndexBuffer9* ib;
		void* Buffer;
		HR(m_pD3DDevice->CreateIndexBuffer( ICount * size,
			usage,
			fmt,
			pool,
			&ib,
			0));

		// Writing to the buffer.
		if (Indices)
		{
			HR(ib->Lock(0, 0, &Buffer, 0));
			memcpy(Buffer, Indices, ICount * size);
			HR(ib->Unlock());
		}

		// Create IndexBufferDX9
		IndexBufferDX9* ibOut = KGE_NEW(IndexBufferDX9)(ICount, size, fmt);
		ibOut->m_pIB		  = ib;
		ibOut->Dynamic(isDynamic);

		// Add to list for restoring on device lost
		if (isDynamic) 
			m_vIBuffers.push_back(ibOut);

		AddHardwareBuffer(ibOut);

		return ibOut;

	} // CreateIndexBuffer

	//------------------------------------------------------------------------------------
	// Sets the vertex buffer for multi streaming and rendering
	//------------------------------------------------------------------------------------
	void RendererD3D9::SetVertexBuffer( HardwareBuffer* pBuffer, int stage )
	{
		if (!pBuffer)
		{
			return;
		}

		if (m_nVertexBufferID[stage] == pBuffer->GetID())
			return;

		m_nVertexBufferID[stage] = pBuffer->GetID();

		// Set the vertex buffer
		m_pD3DDevice->SetStreamSource(stage, ((VertexBufferDX9*)pBuffer)->m_pVB, 
			0, pBuffer->GetStride());

	} // SetVertexBuffer

	//------------------------------------------------------------------------------------
	// Sets the index buffer for rendering
	//------------------------------------------------------------------------------------
	void RendererD3D9::SetIndexBuffer( HardwareBuffer* pBuffer )
	{
		if (!pBuffer)
		{
			return;
		}

		if (m_nIndexBufferID == pBuffer->GetID())
			return;

		m_nIndexBufferID = pBuffer->GetID();

		m_pD3DDevice->SetIndices(((IndexBufferDX9*)pBuffer)->m_pIB);

	} // SetIndexBuffer

	//------------------------------------------------------------------------------------
	// Create a custom vertex declaration and returns its pointer
	//------------------------------------------------------------------------------------
	VertexDec* RendererD3D9::CreateVertexDeclaration
		( core::DynamicArray<CustomVertexElement> VertexInfoArray, core::stringc& sName )
	{
		IDirect3DVertexDeclaration9* vd;
		if (FAILED(m_pD3DDevice->CreateVertexDeclaration((D3DVERTEXELEMENT9*)&VertexInfoArray[0], &vd)))
		{
			io::Logger::Error("Can't create %s vertex declaration", sName.c_str());
		}
		VertexDec* pOut	  = KGE_NEW(VertexDec)(sName);
		pOut->m_VertexDec = vd;

		AddVertexDec(pOut);

		return pOut;

	} // CreateVertexDeclaration

	//------------------------------------------------------------------------------------
	// Sets the vertex declaration
	//------------------------------------------------------------------------------------
	void RendererD3D9::SetVertexDeclaration( VertexDec* pVD )
	{
		if (m_nVertexDecID != pVD->GetID())
		{
			m_nVertexDecID = pVD->GetID();
			m_pD3DDevice->SetVertexDeclaration( (IDirect3DVertexDeclaration9*)pVD->m_VertexDec ); 	
		}

	} // SetVertexDeclaration

	//------------------------------------------------------------------------------------
	// Draw a list of triangles
	//------------------------------------------------------------------------------------
	void RendererD3D9::DrawTriangleList( u32 VCount, u32 ICount, 
		u32 VertexStart /*= 0*/, u32 StartIndex /*= 0*/ )
	{
		KGEPROFILE;

		if ( ICount > 0 )
		{
			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, VertexStart, 0, VCount, StartIndex, ICount / 3);
			m_iTriCount += ICount / 3 * m_iBatchCount;
		}
		else
		{
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, VertexStart, VCount / 3);
			m_iTriCount += VCount / 3 * m_iBatchCount;
		}
		m_iDrawCount++;

	} // DrawTriangleList

	//------------------------------------------------------------------------------------
	// Sets the transformation of World, View or Projection matrices
	//------------------------------------------------------------------------------------
	void RendererD3D9::SetTransForm( math::Matrix *mat, TransformMode TM /*= ETM_World*/ )
	{
		D3DMATRIX mWorld;
		if (!mat) 
		{
			math::Matrix m;
			m.LoadIdentity();
			memcpy(&mWorld, &m, sizeof(D3DMATRIX)); 
		}
		else
			memcpy(&mWorld, mat, sizeof(D3DMATRIX)); 

		switch (TM)
		{
		case ETM_World:
			m_pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);
			m_mWorld = mWorld;
			break;
		case ETM_View:
			m_pD3DDevice->SetTransform(D3DTS_VIEW, &mWorld);
			m_mView = mWorld;
			if (mat)
				m_mViewProj = GetTransForm(ETM_Projection) * (*mat);
			else
				m_mViewProj = GetTransForm(ETM_Projection);
			break;
		case ETM_Projection:
			m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mWorld);
			m_mProj = mWorld;
			if (mat)
				m_mViewProj = *mat * GetTransForm(ETM_View);
			else
				m_mViewProj = GetTransForm(ETM_View);
			break;
		}

	} // SetTransForm

	//------------------------------------------------------------------------------------
	// Returns the transformation of World, View, Projection or ViewProjection matrices
	//------------------------------------------------------------------------------------
	math::Matrix RendererD3D9::GetTransForm( TransformMode TM /*= ETM_World*/ )
	{
		math::Matrix mat;
		switch (TM)
		{
		case ETM_World:
			memcpy(&mat, &m_mWorld, sizeof(D3DMATRIX)); 
			break;
		case ETM_View:
			memcpy(&mat, &m_mView, sizeof(D3DMATRIX)); 
			break;
		case ETM_Projection:
				memcpy(&mat, &m_mProj, sizeof(D3DMATRIX)); 
			break;
		case ETM_ViewProjection:
			return m_mViewProj;
			break;
		}
		return mat;

	} // GetTransForm

	//------------------------------------------------------------------------------------
	// Creates a texture from an image.
	//------------------------------------------------------------------------------------
	Texture* RendererD3D9::CreateTexture( Image* pImg )
	{
		Texture* pTex = KGE_NEW(TextureDX9)(pImg);

		return pTex;

	} // CreateTexture

	//------------------------------------------------------------------------------------
	// Returns the D3D device if the renderer is D3D
	//------------------------------------------------------------------------------------
	void* RendererD3D9::GetDirect3dDevice()
	{
		return (void*)m_pD3DDevice;

	} // GetDirect3dDevice

	//------------------------------------------------------------------------------------
	// Sets the texture
	//------------------------------------------------------------------------------------
	void RendererD3D9::SetTexture( Texture* pTex, int Stage /*= 0*/ )
	{
		if (pTex)
		{
			if (m_nTextID[Stage] == pTex->GetHandle())
				return;

			m_nTextID[Stage] = pTex->GetHandle();
			m_pD3DDevice->SetTexture(Stage, ((TextureDX9*)pTex)->m_pTexture);
		}
		else
		{
			if (m_nTextID[Stage] == MAXID)
				return;

			m_nTextID[Stage] = MAXID;
			m_pD3DDevice->SetTexture(Stage, NULL);
		}

	} // SetTexture

} // gfx

} // kge
