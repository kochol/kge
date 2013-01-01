#include "RendererD3D11.h"
#include "../include/Logger.h"
#include "VertexBuferDX11.h"
#include "IndexBufferDX11.h"
#include "../include/VertexDeclaration.h"
#include "../include/VertexElement.h"
#include "../include/BlockData.h"

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
	RendererD3D11::RendererD3D11() : m_driverType(D3D_DRIVER_TYPE_NULL), 
		m_featureLevel(D3D_FEATURE_LEVEL_11_0), m_pd3dDevice(NULL), 
		m_pImmediateContext(NULL), m_pSwapChain(NULL), m_pRenderTargetView(NULL),
		m_pBackBuffer(NULL)
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	RendererD3D11::~RendererD3D11()
	{
		if (m_pImmediateContext)
			m_pImmediateContext->ClearState();

		SAFERELEASE(m_pRenderTargetView);
		SAFERELEASE(m_pSwapChain);
		SAFERELEASE(m_pImmediateContext);
		SAFERELEASE(m_pd3dDevice);
		m_pBackBuffer = NULL;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Initialize the renderer
	//------------------------------------------------------------------------------------
	bool RendererD3D11::Init(InitParameters &params)
	{
		HRESULT hr = S_OK;

		UINT createDeviceFlags = 0;
#if KGE_DEBUG_MODE == 1
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		ZeroMemory( &m_SwapChainDesc, sizeof( m_SwapChainDesc ) );
		m_SwapChainDesc.BufferCount = 1;
		m_SwapChainDesc.BufferDesc.Width = params.Width;
		m_SwapChainDesc.BufferDesc.Height = params.Height;
		m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		m_SwapChainDesc.OutputWindow = (HWND)params.hwnd;
		m_SwapChainDesc.SampleDesc.Count = 1;
		m_SwapChainDesc.SampleDesc.Quality = 0;
		m_SwapChainDesc.Windowed = !params.FullScreen;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &m_SwapChainDesc, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext );
			if( SUCCEEDED( hr ) )
				break;
		}
		if( FAILED( hr ) )
		{
			io::Logger::Error("Can't create D3D 11 device.");
			return false;
		}

		// Create a render target view
		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&m_pBackBuffer );
		if( FAILED( hr ) )
		{
			io::Logger::Error("Can't get the back buffer.");
			return false;
		}

		hr = m_pd3dDevice->CreateRenderTargetView( m_pBackBuffer, NULL, &m_pRenderTargetView );
		m_pBackBuffer->Release();
		if( FAILED( hr ) )
		{
			io::Logger::Error("Can't create render target view.");
			return false;
		}

		m_pImmediateContext->OMSetRenderTargets( 1, &m_pRenderTargetView, NULL );

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)params.Width;
		vp.Height = (FLOAT)params.Height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_pImmediateContext->RSSetViewports( 1, &vp );

		io::Logger::Info("Using Direct3D11 for rendering.");

		return true;

	} // Init

	//------------------------------------------------------------------------------------
	// Clear the render target
	//------------------------------------------------------------------------------------
	bool RendererD3D11::Clear(bool bColor, bool bDepth, bool bStencil)
	{
// 		if (!bColor && !bDepth && !bStencil)
// 			return true;
// 
// 		ul32 Flags;
// 
 		if (bColor)
 			m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, m_fClearColor);

// 		if (bDepth && bColor)
// 			Flags |= D3DCLEAR_ZBUFFER;
// 		else if (bDepth)
// 			Flags = D3DCLEAR_ZBUFFER;
// 		if (bStencil && bColor && m_bUseStencil)
// 			Flags |= D3DCLEAR_STENCIL;
// 		else if (bStencil && bDepth && m_bUseStencil)
// 			Flags |= D3DCLEAR_STENCIL;
// 		else if (bStencil && m_bUseStencil)
// 			Flags = D3DCLEAR_STENCIL;
// 
// 		if (FAILED(m_pD3DDevice->Clear(0, NULL, Flags, m_cClearColor.c, 1.0f, 0)))
// 		{
// 			io::Logger::Error("Can't clear the Pixel, ZBuffer or StencilBuffer.");
// 			return false;
// 		}

		return true;

	} // Clear
		
	//------------------------------------------------------------------------------------
	// Begin the rendering
	//------------------------------------------------------------------------------------
	bool RendererD3D11::BeginRendering(bool bColor, bool bDepth, bool bStencil)
	{
		KGEPROFILE;

		Clear(bColor, bDepth, bStencil);

// 		if (!m_bIsSceneRunning)
// 		{
// 
// 			if (FAILED(m_pD3DDevice->BeginScene()))
// 			{
// 				io::Logger::Error("Can't Begin Rendering.");
// 				return false;
// 			}
// 
// 			m_bIsSceneRunning = true;
// 
// 			m_iTriCount  = 0;
// 			m_iDrawCount = 0;
// 		}

		return true;

	} // BeginRendering

	//------------------------------------------------------------------------------------
	// End the rendering and draw to back buffer.
	//------------------------------------------------------------------------------------
	bool RendererD3D11::EndRendering()
	{
		KGEPROFILE;

// 		if (m_bIsSceneRunning)
// 		{
 			HRESULT hr = S_OK;
// 			m_pD3DDevice->EndScene();
// 				
 			hr = m_pSwapChain->Present(NULL, NULL);
// 			m_bIsSceneRunning = false;
// 
// 			if (hr == D3DERR_DEVICELOST)
// 			{
// 				io::Logger::Warning( "Direct3D device lost.");
// 			}
// 		}

		return true;

	} // EndRendering
	
	//------------------------------------------------------------------------------------
	// Create a vertex buffer from custom vertex type and store it on video memory.
	//------------------------------------------------------------------------------------
	HardwareBuffer* RendererD3D11::CreateVertexBuffer
		(	void* Vertices, 
			u32 VCount, 
			u32 Stride, 
			bool isDynamic 
		)
	{
// 		// Create the vertex buffer.
// 		DWORD usage = 0;
// 		D3DPOOL pool = D3DPOOL_MANAGED;
// 
// 		if (isDynamic)
// 		{
// 			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
// 			pool = D3DPOOL_DEFAULT;
// 		}
// 
// 		IDirect3DVertexBuffer9* vb;
// 		HR(m_pD3DDevice->CreateVertexBuffer( Stride * VCount
// 			, usage
// 			, 0
// 			, pool
// 			, &vb
// 			, 0 ));
// 
// 		// Writing to the buffer.
// 		void* Buffer;
// 
// 		if (Vertices)
// 		{
// 			HR(vb->Lock(0, 0, &Buffer, 0));
// 			memcpy(Buffer, Vertices, Stride * VCount);
// 			HR(vb->Unlock());
// 		}
// 
// 		VertexBufferDX9* ppVBout;
// 		ppVBout			= KGE_NEW(VertexBufferDX9)(VCount, Stride, 0);
// 		ppVBout->m_pVB	= vb;
// 		ppVBout->Dynamic(isDynamic);
// 
// 		if (isDynamic) 
// 			m_vVBuffers.push_back(ppVBout);
// 
// 		AddHardwareBuffer(ppVBout);
// 
// 		return ppVBout;
return NULL;
	} // CreateVertexBuffer

	//------------------------------------------------------------------------------------
	// Removes VertexBufferDX9 from internal list.
	//------------------------------------------------------------------------------------
	void RendererD3D11::RemoveVertexBuffer(VertexBufferDX11* vb)
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
	void RendererD3D11::RemoveIndexBuffer(IndexBufferDX11* ib)
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
	HardwareBuffer* RendererD3D11::CreateIndexBuffer( void* Indices, u32 ICount, IndexBufferType eIndexBufferType, bool isDynamic )
	{
// 		// Create the index buffer.
// 		DWORD usage = 0;
// 		D3DPOOL pool = D3DPOOL_MANAGED;
// 
// 		// If the buffer is dynamic set the flags
// 		if (isDynamic)
// 		{
// 			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
// 			pool = D3DPOOL_DEFAULT;
// 		}
// 
// 		// index buffer 16 or 32 bit format
// 		D3DFORMAT fmt = D3DFMT_INDEX16;
// 		u32 size = sizeof( u16 );
// 		if (eIndexBufferType == EIBT_32Bit)
// 		{
// 			fmt = D3DFMT_INDEX32;
// 			size = sizeof( u32 );
// 		}
// 
// 		// Create the index buffer
// 		IDirect3DIndexBuffer9* ib;
// 		void* Buffer;
// 		HR(m_pD3DDevice->CreateIndexBuffer( ICount * size,
// 			usage,
// 			fmt,
// 			pool,
// 			&ib,
// 			0));
// 
// 		// Writing to the buffer.
// 		if (Indices)
// 		{
// 			HR(ib->Lock(0, 0, &Buffer, 0));
// 			memcpy(Buffer, Indices, ICount * size);
// 			HR(ib->Unlock());
// 		}
// 
// 		// Create IndexBufferDX9
// 		IndexBufferDX9* ibOut = KGE_NEW(IndexBufferDX9)(ICount, size, fmt);
// 		ibOut->m_pIB		  = ib;
// 		ibOut->Dynamic(isDynamic);
// 
// 		// Add to list for restoring on device lost
// 		if (isDynamic) 
// 			m_vIBuffers.push_back(ibOut);
// 
// 		AddHardwareBuffer(ibOut);
// 
// 		return ibOut;
return NULL;
	} // CreateIndexBuffer

	//------------------------------------------------------------------------------------
	// Sets the vertex buffer for multi streaming and rendering
	//------------------------------------------------------------------------------------
	void RendererD3D11::SetVertexBuffer( HardwareBuffer* pBuffer, int stage )
	{
// 		if (!pBuffer)
// 		{
// 			return;
// 		}
// 
// 		if (m_nVertexBufferID[stage] == pBuffer->GetID())
// 			return;
// 
// 		m_nVertexBufferID[stage] = pBuffer->GetID();
// 
// 		// Set the vertex buffer
// 		m_pD3DDevice->SetStreamSource(stage, ((VertexBufferDX9*)pBuffer)->m_pVB, 
// 			0, pBuffer->GetStride());

	} // SetVertexBuffer

	//------------------------------------------------------------------------------------
	// Sets the index buffer for rendering
	//------------------------------------------------------------------------------------
	void RendererD3D11::SetIndexBuffer( HardwareBuffer* pBuffer )
	{
// 		if (!pBuffer)
// 		{
// 			return;
// 		}
// 
// 		if (m_nIndexBufferID == pBuffer->GetID())
// 			return;
// 
// 		m_nIndexBufferID = pBuffer->GetID();
// 
// 		m_pD3DDevice->SetIndices(((IndexBufferDX9*)pBuffer)->m_pIB);

	} // SetIndexBuffer

	//------------------------------------------------------------------------------------
	// Create a custom vertex declaration and returns its pointer
	//------------------------------------------------------------------------------------
	VertexDec* RendererD3D11::CreateVertexDeclaration
		( core::DynamicArray<CustomVertexElement> VertexInfoArray, core::stringc& sName )
	{
// 		IDirect3DVertexDeclaration9* vd;
// 		if (FAILED(m_pD3DDevice->CreateVertexDeclaration((D3DVERTEXELEMENT9*)&VertexInfoArray[0], &vd)))
// 		{
// 			io::Logger::Error("Can't create %s vertex declaration", sName.c_str());
// 		}
// 		VertexDec* pOut	  = KGE_NEW(VertexDec)(sName);
// 		pOut->m_VertexDec = vd;
// 
// 		AddVertexDec(pOut);
// 
// 		return pOut;
return NULL;
	} // CreateVertexDeclaration

	//------------------------------------------------------------------------------------
	// Sets the vertex declaration
	//------------------------------------------------------------------------------------
	void RendererD3D11::SetVertexDeclaration( VertexDec* pVD )
	{
// 		if (m_nVertexDecID != pVD->GetID())
// 		{
// 			m_nVertexDecID = pVD->GetID();
// 			m_pD3DDevice->SetVertexDeclaration( (IDirect3DVertexDeclaration9*)pVD->m_VertexDec ); 	
// 		}

	} // SetVertexDeclaration

	//------------------------------------------------------------------------------------
	// Draw a list of triangles
	//------------------------------------------------------------------------------------
	void RendererD3D11::DrawTriangleList( u32 VCount, u32 ICount, 
		u32 VertexStart /*= 0*/, u32 StartIndex /*= 0*/ )
	{
		KGEPROFILE;

// 		if ( ICount > 0 )
// 		{
// 			m_pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, VertexStart, 0, VCount, StartIndex, ICount / 3);
// 			m_iTriCount += ICount / 3 * m_iBatchCount;
// 		}
// 		else
// 		{
// 			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, VertexStart, VCount / 3);
// 			m_iTriCount += VCount / 3 * m_iBatchCount;
// 		}
// 		m_iDrawCount++;

	} // DrawTriangleList

	//------------------------------------------------------------------------------------
	// Sets the transformation of World, View or Projection matrices
	//------------------------------------------------------------------------------------
	void RendererD3D11::SetTransForm( math::Matrix *mat, TransformMode TM /*= ETM_World*/ )
	{
// 		D3DMATRIX mWorld;
// 		if (!mat) 
// 		{
// 			math::Matrix m;
// 			m.LoadIdentity();
// 			memcpy(&mWorld, &m, sizeof(D3DMATRIX)); 
// 		}
// 		else
// 			memcpy(&mWorld, mat, sizeof(D3DMATRIX)); 
// 
// 		switch (TM)
// 		{
// 		case ETM_World:
// 			m_pD3DDevice->SetTransform(D3DTS_WORLD, &mWorld);
// 			m_mWorld = mWorld;
// 			break;
// 		case ETM_View:
// 			m_pD3DDevice->SetTransform(D3DTS_VIEW, &mWorld);
// 			m_mView = mWorld;
// 			if (mat)
// 				m_mViewProj = GetTransForm(ETM_Projection) * (*mat);
// 			else
// 				m_mViewProj = GetTransForm(ETM_Projection);
// 			break;
// 		case ETM_Projection:
// 			m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &mWorld);
// 			m_mProj = mWorld;
// 			if (mat)
// 				m_mViewProj = *mat * GetTransForm(ETM_View);
// 			else
// 				m_mViewProj = GetTransForm(ETM_View);
// 			break;
// 		}

	} // SetTransForm

	//------------------------------------------------------------------------------------
	// Returns the transformation of World, View, Projection or ViewProjection matrices
	//------------------------------------------------------------------------------------
	math::Matrix RendererD3D11::GetTransForm( TransformMode TM /*= ETM_World*/ )
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
	// Sets the clear color
	//------------------------------------------------------------------------------------
	void RendererD3D11::SetClearColor( const Color& ClearColor )
	{
		m_fClearColor[0] = ClearColor.getRed() / 255.0f;
		m_fClearColor[1] = ClearColor.getGreen() / 255.0f;
		m_fClearColor[2] = ClearColor.getBlue() / 255.0f;
		m_fClearColor[3] = ClearColor.getAlpha() / 255.0f;

	} // SetClearColor

	//------------------------------------------------------------------------------------
	// Creates a texture from an image.
	//------------------------------------------------------------------------------------
	Texture* RendererD3D11::CreateTexture( Image* pImg )
	{

		return NULL;

	} // CreateTexture

	//------------------------------------------------------------------------------------
	// Returns the D3D device if the renderer is D3D
	//------------------------------------------------------------------------------------
	void* RendererD3D11::GetDirect3dDevice()
	{
		return (void*)m_pImmediateContext;

	} // GetDirect3dDevice

} // gfx

} // kge
