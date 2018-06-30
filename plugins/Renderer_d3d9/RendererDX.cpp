// File name: RendererDX.cpp
// Des: in file interface asli kar ba ghesmat Renderer ba D3D hast. Bayad tamam saye
//      khodemono bokonim ta az D3DX estefade nakonim.
// Date: 14/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#include <iostream>

#include "rendererdx.h"
#include "texturedx9.h"
#include "VertexShaderDX.h"
#include "PixelShaderDX.h"
#include "VertexBufferDX.h"
#include "IndexBufferDX.h"
#include "../../include/kge/gfx/meshbuffer.h"
#include "../../include/kge/io/logger.h"
#include "../../include/kge/efx/EffectManager.h"
#include "../../include/kge/gui/Image.h"
#include "../../include/kge/gfx/ImageData.h"
#include "../../include/kge/math/matrix.h"
#include <math.h>

#pragma comment(lib, "d3d9.lib")

#pragma comment(lib, "d3dx9.lib")

#define FOURCC_INTZ ((D3DFORMAT)(MAKEFOURCC('I','N','T','Z')))

// *** Global variables ***
LPDIRECT3D9				m_pD3D;

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

#include <d3dx9.h>

//===============================================================
// Debug

#if defined(DEBUG) | defined(_DEBUG)

#include <dxerr.h>
#pragma comment(lib, "DxErr.lib")

#ifndef HR
#define HR(x)                                      \
{                                                  \
	HRESULT hr = x;                                \
	if(FAILED(hr))                                 \
{                                              \
	DXTraceW(__FILE__, __LINE__, hr, L#x, TRUE); \
	}                                              \
	}
#endif

#else
#ifndef HR
#define HR(x) x;
#endif
#endif 

KGE_IMPORT extern kge::u32 VertexShaderSet;
KGE_IMPORT extern kge::u32 PixelShaderSet;

namespace kge
{
	namespace gfx
	{
		//-------------
		// Constructor.
		//-------------
		RendererDX::RendererDX():
		m_pd3dDevice(NULL), m_pV2(NULL), m_pV3(NULL), m_pV3C(NULL),
		m_pV3T(NULL), m_pV3CT(NULL), m_pV3TN(NULL), m_bDeviceLost(false), m_iAASurface(-1),
		m_pAATexture(NULL), m_pP0NT1BT2(NULL), m_pPJ0NT1BT2(NULL), m_bRTT(false),
		m_pBackBufferSurface(0), m_pBackBufferZSurface(0), m_pSwapChain(0)
	{
		m_iTriCount  = 0;
		m_iDrawCount = 0;
		for (int i = 0; i < ERF_Count; i++)
			m_bRF[i] = false;
		for (int i = 0; i < 16; i++)
			m_nVertexBufferID[i] = MAXID;

		m_pTimer = KGE_NEW(core::Timer)();
		m_pTimer->Interval = 1000;
		m_iFPS = 0;
		m_iFPSTemp = 0;

	} // Constructor

	// **********
	// Destructor
	// **********
	RendererDX::~RendererDX()
	{
		if (m_pd3dDevice)
		{
			m_pd3dDevice->Release();
			m_pd3dDevice = NULL;
		}
		if (m_pD3D)
		{
			m_pD3D->Release();
			m_pD3D = NULL;
		}
		if ( m_pV2 )
		{
			m_pV2->Release();
			m_pV2 = NULL;
		}
		if ( m_pV3 )
		{
			m_pV3->Release();
			m_pV3 = NULL;
		}
		if ( m_pV3C )
		{
			m_pV3C->Release();
			m_pV3C = NULL;
		}
		if ( m_pV3T )
		{
			m_pV3T->Release();
			m_pV3T = NULL;
		}
		if ( m_pV3CT )
		{
			m_pV3CT->Release();
		}
		if ( m_pV3TN )
		{
			m_pV3TN->Release();
			m_pV3TN = NULL;
		}
		// Deleting the textures.
		//for(std::vector<LPDIRECT3DTEXTURE9>::iterator it = m_vTextures.begin(); 
		//	it != m_vTextures.end(); it++)
		//{
		//	(*it)->Release();
		//	(*it) = NULL;
		//}
		// Deleting the vertex buffers.
// 		for(std::vector<IDirect3DVertexBuffer9*>::iterator it = m_vVBuffers.begin(); 
// 			it != m_vVBuffers.end(); it++)
// 		{
// 			if ((*it))
// 			{
// 				(*it)->Release();
// 				(*it) = NULL;
// 			}
// 		}
		// Deleting the index buffers.
// 		for(std::vector<IDirect3DIndexBuffer9*>::iterator it = m_vIBuffers.begin(); 
// 			it != m_vIBuffers.end(); it++)
// 		{
// 			if ((*it))
// 			{
// 				(*it)->Release();
// 				(*it) = NULL;
// 			}
// 		}

		KGE_DELETE(Lights, Lighting);
		KGE_DELETE(m_pTimer, Timer);

	} // Destructor

	// ***** ******* *********
	// Amade kardane Renderer.
	// ***** ******* *********
	bool RendererDX::Init(Device* device, InitParameters &params)
	{
		m_pDevice		  = device;
		m_Params			= params;
		m_bIsSceneRunning = false;
		m_TextCount		  = 0;

		for (int i = 0; i < 8; i++)
		{
			m_nTextID[i]  = MAXID;
		}

		m_vTextures.clear();
		m_vVBuffers.clear();
		m_vIBuffers.clear();

		// Init the Light class
		Lights = KGE_NEW(LightDX)(this);

		// Create LPDIRECT3D9
		if(m_pD3D)
		{
			m_pD3D->Release();
			m_pD3D = NULL;
		}
		m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

		if (!m_pD3D)
		{
			io::Logger::Log("Can't Create Direct3D9.", io::ELM_Error);
			return false;
		}

		// Find the device name
		D3DADAPTER_IDENTIFIER9 id;
		m_pD3D->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &id);
		io::Logger::Log(io::ELM_Information, "Device name: %s", id.DeviceName);
		io::Logger::Log(io::ELM_Information, "Driver name: %s", id.Driver);
		io::Logger::Log(io::ELM_Information, "Driver Description: %s", id.Description);

		// Behtarin chiz hayi ke baraye sakhte Device D3D lazzem hast ro in tabe 
		// moshakhas mikone.
		CheckStencil(params.AAMode == gfx::EAAM_MSAA);

		D3DDISPLAYMODE mode = {0};
		if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			return false;

		D3DFORMAT format = mode.Format;
		if (FAILED(m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
			format, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, format)))
		{
			return false;
		}

		// Fill in m_d3dpp
		ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
		m_d3dpp.Windowed = !params.FullScreen;
		m_d3dpp.BackBufferCount = 1;
		m_d3dpp.EnableAutoDepthStencil = TRUE;
		m_d3dpp.AutoDepthStencilFormat = m_DSFMT;
		m_d3dpp.MultiSampleType = m_msType;
		m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_d3dpp.hDeviceWindow = (HWND)params.hwnd;
		m_d3dpp.PresentationInterval = params.VSync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;

		// fullscreen mode
		if (params.FullScreen)
		{
			m_d3dpp.BackBufferFormat = format;
			m_d3dpp.BackBufferWidth = params.Width;
			m_d3dpp.BackBufferHeight = params.Height;
			m_d3dpp.FullScreen_RefreshRateInHz = mode.RefreshRate;
		}
		// Windowed mode
		else
		{
			m_d3dpp.BackBufferWidth = params.Width;
			m_d3dpp.BackBufferHeight = params.Height;
			m_d3dpp.BackBufferFormat = format;
			m_d3dpp.FullScreen_RefreshRateInHz = 0;
		}

		// Is stencil buffer active?
		if (m_DSFMT == D3DFMT_D24S8 ||
			m_DSFMT == D3DFMT_D24X4S4 ||
			m_DSFMT == D3DFMT_D15S1)
			m_Params.UseStencil = true;
		else
			m_Params.UseStencil = false;

		HRESULT hr;
		// Create the device
		hr = m_pD3D->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
			m_d3dpp.hDeviceWindow,
			m_Behavior, &m_d3dpp, &m_pd3dDevice);

		if (FAILED(hr))
		{
			io::Logger::Log("Can't create Direct3D9 Device.", io::ELM_Error);
			return false;
		}

		// Make Clear Color to Black.
		m_ClearColor = D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);

		io::Logger::Log("Using Direct3D9 for rendering.");
		return AfterInit();

	} // Init

	//--------------
	// Check Stencil
	//--------------
	void RendererDX::CheckStencil(bool Antialiasing)
	{
		bool bCheck = false;
		// use stencil buffer
		if (m_Params.UseStencil)
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
			io::Logger::Log("No Depth/Stencil buffer format supported.", io::ELM_Error);
			return;
		}

		// SELECT MULTISAMPLE TYPE
		UINT nAdapter = D3DADAPTER_DEFAULT;
		//D3DFORMAT fmtAdapter = D3DFMT_A8R8G8B8;
		D3DFORMAT fmtBack = D3DFMT_A8R8G8B8;

		kge::u32 nMSCount;
		if (Antialiasing)
			nMSCount = 4;
		else
			nMSCount = 1;
		for(int nMS=(int)nMSCount-1; nMS>=0; nMS--) 
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(
				nAdapter
				,D3DDEVTYPE_HAL
				,fmtBack
				,!m_Params.FullScreen
				,g_msType[nMS], &msQuality))) 
			{
				m_msType = g_msType[nMS];
				break;
			}
		} // for [multisample]

		// SELECT VERTEX PROCESSING
		D3DCAPS9 d3dcaps;
		m_pD3D->GetDeviceCaps(nAdapter, D3DDEVTYPE_HAL, &d3dcaps);
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
			else 
			if (CheckDevice(&d3dcaps, D3DCREATE_HARDWARE_VERTEXPROCESSING)==true)
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

	// ***** ******* ******
	// Check Stencil Format
	// ***** ******* ******
	bool RendererDX::CheckStencilFMT()
	{
		UINT nAdapter = D3DADAPTER_DEFAULT;
		D3DFORMAT fmtAdapter = D3DFMT_X8R8G8B8;
		D3DFORMAT fmtBack = D3DFMT_X8R8G8B8;

		if (FAILED(m_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,D3DUSAGE_DEPTHSTENCIL
			,D3DRTYPE_SURFACE
			,m_DSFMT)))
			return false;
		if (FAILED(m_pD3D->CheckDepthStencilMatch(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,fmtBack
			,m_DSFMT)))
			return false;
		return true;
	} // CheckstencilFMT

	// ***** ******
	// Check Device
	// ***** ******
	bool RendererDX::CheckDevice(D3DCAPS9 *pCaps, DWORD dwBehavior)
	{
		if ( (dwBehavior & D3DCREATE_HARDWARE_VERTEXPROCESSING) ||
			(dwBehavior & D3DCREATE_MIXED_VERTEXPROCESSING   )  ) 
		{
			// alphablending from texture pixels supported
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

	// *** ******* ***** ** ** ****** ***
	// Pak kardane pixel ha va buffer ha.
	// *** ******* ***** ** ** ****** ***
	bool RendererDX::Clear(bool Pixel, bool Zbuffer, bool Stencil)
	{
		if (!Pixel && !Zbuffer && !Stencil)
			return true;

		ul32 Flags;

		if (Pixel)
			Flags = D3DCLEAR_TARGET;
		if (Zbuffer && Pixel)
			Flags |= D3DCLEAR_ZBUFFER;
		else if (Zbuffer)
			Flags = D3DCLEAR_ZBUFFER;
		if (Stencil && Pixel && m_Params.UseStencil)
			Flags |= D3DCLEAR_STENCIL;
		else if (Stencil && Zbuffer && m_Params.UseStencil)
			Flags |= D3DCLEAR_STENCIL;
		else if (Stencil && m_Params.UseStencil)
			Flags = D3DCLEAR_STENCIL;

		if (FAILED(m_pd3dDevice->Clear(0, NULL, Flags, m_ClearColor, 1.0f, 0)))
		{
			io::Logger::Log("Can't clear the Pixel, ZBuffer or StencilBuffer.", io::ELM_Error);
			return false;
		}

		return true;
	} // Clear

	//----------------------------------------------------------------------------------------
	// Creates a renderable texture to use it as a render target
	//----------------------------------------------------------------------------------------
	bool RendererDX::CreateRenderableTexture(Texture** ppOutTexture, int width, int height, TextureFormat tf
		, bool CreateDepthStencilBuffer, bool AntiAliasing)
	{
		u32 Handle = m_pDevice->GetTextureManager()->GetNewHandle();
		TextureDX9* pTex = KGE_NEW(TextureDX9)(m_pd3dDevice, this, Handle, NULL, NULL, NULL);
		*ppOutTexture = pTex;
		m_pDevice->GetTextureManager()->Add(*ppOutTexture);
		
		return ((TextureDX9*)*ppOutTexture)->CreateRenderableTexture(width, height, tf, 
			m_DSFMT, CreateDepthStencilBuffer, AntiAliasing, m_msType, msQuality);

	} // CreateRenderableTexture

	//----------------------------------------------------------------------------------------
	// Begin Rendering
	//----------------------------------------------------------------------------------------
	bool RendererDX::BeginRendering(bool Pixel, bool Zbuffer, bool Stencil)
	{
		HRESULT hr;

		Clear(Pixel, Zbuffer, Stencil);
		D3DVIEWPORT9 vp;
		vp.X	  = m_rViewPort.X;
		vp.Y	  = m_rViewPort.Y;
		vp.Width  = m_rViewPort.Width;
		vp.Height = m_rViewPort.Height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;
		m_pd3dDevice->SetViewport(&vp);

		if (!m_bIsSceneRunning)
		{
			if (m_bDeviceLost)
			{

				// free CPU for a while for other process.
				Sleep(100);

				if (FAILED(hr = m_pd3dDevice->TestCooperativeLevel()))
				{
					if (hr == D3DERR_DEVICELOST)
						return false;

					if (hr == D3DERR_DEVICENOTRESET)
					{
						if (!ResetDevice()) return false;
					}
				}
			}

			if (FAILED(m_pd3dDevice->BeginScene()))
			{
				io::Logger::Log("Can't Begin Rendering.", io::ELM_Error);
				return false;
			}

			m_bIsSceneRunning = true;

			// calculate fps.
			m_iFPSTemp++;
			if (m_pTimer->NextFrame())
			{
				m_iFPS = m_iFPSTemp;
				m_iFPSTemp = 0;
			}

			// if camera is 2D
			if (m_bRF[ERF_2D])
			{
				m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_mView);
				m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);
				//	Enable(ERF_Lighting);
				m_bRF[ERF_2D] = false;
			}

			// TODO: what this is for?
			//m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			//   m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			//m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 

			m_iTriCount  = 0;
			m_iDrawCount = 0;
		}

		return true;

	} // Begin Rendering

	//------------------------------------------------------------------------------------
	// End Rendering
	//------------------------------------------------------------------------------------
	void RendererDX::EndRendering()
	{
		RECT re;
		re.left = 0;
		re.top  = 0;
		re.right = m_Params.Width;
		re.bottom = m_Params.Height;
		if (m_bIsSceneRunning)
		{
			HRESULT hr = S_OK;
			if (m_iAASurface > -1)
			{
				// Copy Back buffer to texture
				IDirect3DSurface9 * pBackBuffer = 0;
				m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
				hr = m_pd3dDevice->StretchRect(pBackBuffer, NULL, 
					((TextureDX9*)m_pAATexture)->m_pTextureSurface, NULL, D3DTEXF_LINEAR);
				if (FAILED(hr))
				{
					io::Logger::Log(io::ELM_Error, "Can't copy the back buffer to the texture");
				}
				m_iAASurface = -1;
				m_pAATexture = NULL;
				pBackBuffer->Release();
			}
			else
			{
				m_pd3dDevice->EndScene();

				//if (!m_bRTT)
				//{
				if (m_d3dpp.Windowed)
				{
					hr = m_pSwapChain->Present(&re, &re, m_d3dpp.hDeviceWindow, NULL, D3DPRESENT_DONOTWAIT);
					m_bIsSceneRunning = false;
				}
				else
				{
					if (!m_bRTT)
					{
						hr = m_pSwapChain->Present(NULL, NULL, NULL, NULL, D3DPRESENT_DONOTWAIT);
						m_bIsSceneRunning = false;
					}
				}		
				//}

				if (hr == D3DERR_DEVICELOST)
				{
					io::Logger::Log( "Direct3D device lost." , io::ELM_Warning );
					if (!m_bDeviceLost)
					{
						OnLost();
					}
					m_bDeviceLost = true;
				}
			}
		}
	} // EndRendering

	// *** ******* ***** *** ******
	// Set kardane Range pas zamine
	// *** ******* ***** *** ******
	void RendererDX::SetClearColor(const Colorf& C)
	{
		m_cClearColor = C;
		m_ClearColor = D3DCOLOR_COLORVALUE(C.r, C.g, C.b, 1.0f);
	} // SetClearColor

	// *** ** **** ** **** ***** ** **** ******** ****** ****** *** *******
	// bad az Init ba seda zadan in tabe maghadir avalie engine set mishan.
	// *** ** **** ** **** ***** ** **** ******** ****** ****** *** *******
	bool RendererDX::AfterInit()
	{
		if (FAILED(m_pd3dDevice->GetSwapChain(0, &m_pSwapChain)))
		{
			return false;
		}

		m_nVertexDecID = MAXID;
		m_nIndexBufferID = MAXID;
		for (int i = 0; i < 8; i++)
		{
			m_nVertexBufferID[i] = MAXID;
			m_nVertexBufferID[i+8] = MAXID;
			m_nTextID[i] = MAXID;
		}

		// Calculate the 2D camera matrix.
		m_m2D.SetOrthoOffscreenLH(0.0f, m_Params.Width, m_Params.Height, 0.0f, 0.0f, 1.0f);

		// Create vertex declaration for using with shaders
		if ( m_bUseShaders )
		{
			// Create m_pV2
			D3DVERTEXELEMENT9 decl[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl, &m_pV2);

			m_ppVertexDecs[EVT_V2]->m_VertexDec = m_pV2;

			// Create m_pV3
			D3DVERTEXELEMENT9 decl2[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl2, &m_pV3);

			m_ppVertexDecs[EVT_V3]->m_VertexDec = m_pV3;

			// Create m_pV3C
			D3DVERTEXELEMENT9 decl3[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl3, &m_pV3C);

			m_ppVertexDecs[EVT_V3C]->m_VertexDec = m_pV3C;

			// Create m_pV3T
			D3DVERTEXELEMENT9 decl4[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl4, &m_pV3T);

			m_ppVertexDecs[EVT_V3T]->m_VertexDec = m_pV3T;

			// Create m_pV3CT
			const D3DVERTEXELEMENT9 decl8[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				{0,  28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl8, &m_pV3CT);

			m_ppVertexDecs[EVT_V3CT]->m_VertexDec = m_pV3CT;

			// Create m_pV3TN
			D3DVERTEXELEMENT9 decl5[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
				{0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl5, &m_pV3TN);

			m_ppVertexDecs[EVT_V3TN]->m_VertexDec = m_pV3TN;

			// Create m_pV3TNC
			D3DVERTEXELEMENT9 decl6[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
				{0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				{0,  32, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl6, &m_pV3TNC);

			m_ppVertexDecs[EVT_V3TNC]->m_VertexDec = m_pV3TNC;

			// Create m_pV3TTN
			D3DVERTEXELEMENT9 decl7[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
				{0,  24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				{0,  32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl7, &m_pV3TTN);

			m_ppVertexDecs[EVT_V3TTN]->m_VertexDec = m_pV3TTN;

			// Create m_pP0NT1BT2
			D3DVERTEXELEMENT9 decl9[] =
			{
				{0,  0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{1,  0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0},
				{1,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				{2,  0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0},
				{2,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
				{3,  0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
				{3,  16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
				{3,  32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
				{3,  48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl9, &m_pP0NT1BT2);

			m_ppVertexDecs[EVT_P0NT1BT2I3]->m_VertexDec = m_pP0NT1BT2;

			// Create m_pP0NTC1BT2
			D3DVERTEXELEMENT9 decl11[] =
			{
				{0,  0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{1,  0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
				{1,  12, D3DDECLTYPE_FLOAT2,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				{1,  20, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				{2,  0,  D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
				{2,  12, D3DDECLTYPE_FLOAT3,   D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl11, &m_pP0NTC1BT2);

			m_ppVertexDecs[EVT_P0NTC1BT2]->m_VertexDec = m_pP0NTC1BT2;

			// Create m_pPJ0NT1BT2
			D3DVERTEXELEMENT9 decl10[] =
			{
				{0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
				{0,  12, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
				{1,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
				{1,  12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
				{2,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
				{2,  12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
				{3,  0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
				{3,  16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
				{3,  32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3},
				{3,  48, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4},
				D3DDECL_END()
			};
			m_pd3dDevice->CreateVertexDeclaration(decl10, &m_pPJ0NT1BT2);

			m_ppVertexDecs[EVT_PJ0NT1BT2I3]->m_VertexDec = m_pPJ0NT1BT2;

		} // If the hardware support shaders.

		// activate render states
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE); 
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW); 
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		// Sets the default blending values
		m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// activate texture filtering
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// compute alpha from diffuse colors during shading
		//		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_BLENDDIFFUSEALPHA);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
		m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		// Create timer for calculating the FPS
		m_pTimer->Interval = 1000;
		m_iFPS = 0;
		m_iFPSTemp = 0;

		// Precalculate the graphics card capacities.
		InitGCC();

		// init view port
		m_rViewPort.X		= 0;
		m_rViewPort.Y		= 0;
		m_rViewPort.Width	= m_Params.Width;
		m_rViewPort.Height	= m_Params.Height;

		// Get back buffer surface.
		m_pd3dDevice->GetRenderTarget(0, &m_pBackBufferSurface);
		if (m_pd3dDevice->GetDepthStencilSurface(&m_pBackBufferZSurface) != D3D_OK)
			io::Logger::Log(io::ELM_Error, "Error getting main Z/Stencil buffer");


		Enable(ERF_Culling);
		Enable(ERF_ZWrite);
		Disable(ERF_AdditiveBlending);
		Disable(ERF_AlphaBlending);

		return true;


	} // AfterInit()

	//----------------------------------------------------------------------------------------
	// Set the renderable target where renderer will render there.
	//----------------------------------------------------------------------------------------
	void RendererDX::SetRenderTarget(int RenderNumber, Texture* RenderTarget)
	{
		if (RenderTarget)
		{
			if (!RenderTarget->IsAntiAliased())
			{
				m_pd3dDevice->SetRenderTarget(RenderNumber, ((TextureDX9*)RenderTarget)->m_pTextureSurface);
				if (RenderNumber == 0)
				{
					if (((TextureDX9*)RenderTarget)->m_pDepthSurface)
						m_pd3dDevice->SetDepthStencilSurface(((TextureDX9*)RenderTarget)->m_pDepthSurface);
				}
				m_bRTT = true;
			}
			else
			{
				m_pd3dDevice->SetRenderTarget(0, m_pBackBufferSurface);
				m_pd3dDevice->SetDepthStencilSurface(m_pBackBufferZSurface);
				m_iAASurface = RenderNumber;
				m_pAATexture = RenderTarget;
			}
		}
		else if ( RenderNumber != 0)
		{
			m_pd3dDevice->SetRenderTarget(RenderNumber, NULL);
		}
		else
		{
			m_pd3dDevice->SetRenderTarget(0, m_pBackBufferSurface);
			m_pd3dDevice->SetDepthStencilSurface(m_pBackBufferZSurface);
			m_bRTT = false;
		}

	} // SetRenderTarget

	// *** ******* *******
	// Set kardane dorbin.
	// *** ******* *******
	void RendererDX::SetViewLookatLH(math::Vector &vPos, math::Vector &vPoint, 
		math::Vector &vUp)
	{
		math::Vector Z = vPoint - vPos; // Z
		Z.Normalize();
		math::Vector X;					// X
		X.Cross(vUp, Z);
		X.Normalize();
		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = vUp * Z;
		vcTemp = Z * fDot;
		vcUp = vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis 
		if (fL < 1e-6f) 
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);

				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					io::Logger::Log("Error: Can't calculate the up vector for camera.", io::ELM_Error);
					return;
				}
			}
		}
		vcUp /= fL;

		m_mView._11 = X.x;
		m_mView._12 = vcUp.x;
		m_mView._13 = Z.x;
		m_mView._21 = X.y;
		m_mView._22 = vcUp.y;
		m_mView._23 = Z.y;
		m_mView._31 = X.z;
		m_mView._32 = vcUp.z;
		m_mView._33 = Z.z;
		m_mView._41 = -(X * vPos);
		m_mView._42 = -(vcUp * vPos);
		m_mView._43 = -(Z * vPos);
		m_mView._44 = 1.0f;
		m_mView._14 = m_mView._24 = m_mView._34 = 0.0f;

		m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_mView);

	} // SetViewLookAtLH

	// *** ******* *******
	// Set kardane dorbin.
	// *** ******* *******
	void RendererDX::SetViewLookatRH(math::Vector &vPos, math::Vector &vPoint, 
		math::Vector &vUp)
	{
		math::Vector Z = vPos - vPoint; // Z
		Z.Normalize();
		math::Vector X;					// X
		X.Cross(vUp, Z);
		X.Normalize();
		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = vUp * Z;
		vcTemp = Z * fDot;
		vcUp = vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis 
		if (fL < 1e-6f) 
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);

				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					io::Logger::Log("Error: Can't calcute the up vector for camera.", io::ELM_Error);
					return;
				}
			}
		}
		vcUp /= fL;

		m_mView._11 = X.x;				m_mView._12 = vcUp.x;				m_mView._13 = Z.x;
		m_mView._21 = X.y;				m_mView._22 = vcUp.y;				m_mView._23 = Z.y;
		m_mView._31 = X.z;				m_mView._32 = vcUp.z;				m_mView._33 = Z.z;
		m_mView._41 = -(X * vPos);		m_mView._42 = -(vcUp * vPos);		m_mView._43 = -(Z * vPos);		m_mView._44 = 1.0f;

		m_mView._14 = m_mView._24 = m_mView._34 = 0.0f;

		m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_mView);

	} // SetViewLookAtRH

	// ******* ********** ** *** ****** ** ****** **** *****
	// Matrixe Projection ro set mikone Be sorate Chap dast.
	// ******* ********** ** *** ****** ** ****** **** *****
	void RendererDX::SetPerspectiveLH(float Fov, float Near, float Far)
	{
		float f = (float)m_Params.Width/(float)m_Params.Height;
		float ys = 1.0f / tanf(Fov * 0.5f);
		float xs = ys / f;
		m_mProj._12=m_mProj._13=m_mProj._14=m_mProj._21=m_mProj._23=m_mProj._24=
			m_mProj._31=m_mProj._32=m_mProj._41=m_mProj._42=m_mProj._44=0.0f;
		m_mProj._11 = xs;
		m_mProj._22 = ys;
		m_mProj._33 = Far/(Far-Near);
		m_mProj._34 = 1.0f;
		m_mProj._43 = (-Near)*Far/(Far-Near);

		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);

	} // SetPerspectiveLH

	// ******* ********** ** *** ****** ** ****** **** *****
	// Matrixe Projection ro set mikone Be sorate Rast dast.
	// ******* ********** ** *** ****** ** ****** **** *****
	void RendererDX::SetPerspectiveRH(float Fov, float Near, float Far)
	{
		float ys = 1/tanf(Fov/2.0f);
		float xs = ys/((float)m_Params.Width/(float)m_Params.Height);
		m_mProj._12=m_mProj._13=m_mProj._14=m_mProj._21=m_mProj._23=m_mProj._24=
			m_mProj._31=m_mProj._32=m_mProj._41=m_mProj._42=m_mProj._44=0.0f;
		m_mProj._11 = xs;
		m_mProj._22 = ys;
		m_mProj._33 = Far/(Far-Near);
		m_mProj._34 = -1.0f;
		m_mProj._43 = Near*Far/(Far-Near);

		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);

	} // SetPerspectiveRH

	// ****** ***** ** ****** ********
	// Maknae Ajsam ra taghir midahad.
	// ****** ***** ** ****** ********
	void RendererDX::SetTransForm(math::Matrix *mat, TransformMode TM)
	{
		D3DMATRIX mWorld;
		if (!mat) 
		{
			math::Matrix m;
			memcpy(&mWorld, &m, sizeof(D3DMATRIX)); 
		}
		else
			memcpy(&mWorld, mat, sizeof(D3DMATRIX)); 

		switch (TM)
		{
		case ETM_World:
// 			if (!m_bUseShaders)
// 				m_pd3dDevice->SetTransform(D3DTS_WORLD, &mWorld);
			m_mWorld = mWorld;
			break;
		case ETM_View:
// 			if (!m_bUseShaders)
// 				m_pd3dDevice->SetTransform(D3DTS_VIEW, &mWorld);
			m_mView = mWorld;
			if (mat)
				m_mViewProj = GetTransForm(ETM_Projection) * (*mat);
			else
				m_mViewProj = GetTransForm(ETM_Projection);
			break;
		case ETM_Projection:
// 			if (!m_bUseShaders)
// 				m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &mWorld);
			m_mProj = mWorld;
			if (mat)
				m_mViewProj = *mat * GetTransForm(ETM_View);
			else
				m_mViewProj = GetTransForm(ETM_View);
			break;
		}
	} // SetWorldTransform

	//---------------------------------------------------
	// Matrix haye asli dorbin va ajsam ra bar migardone.
	//---------------------------------------------------
	math::Matrix RendererDX::GetTransForm(TransformMode TM)
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
			if (m_bRF[ERF_2D])
				memcpy(&mat, &m_m2D, sizeof(D3DMATRIX)); 
			else
				memcpy(&mat, &m_mProj, sizeof(D3DMATRIX)); 
			break;
		case ETM_ViewProjection:
			return m_mViewProj;
			break;
		}
		return mat;
	} // GetTransForm

	//-------------------------------------------------------------------------------------------------------
	// This function is for Texture Manager for adding textures use SceneManager AddTexture function instead.
	//-------------------------------------------------------------------------------------------------------
	bool RendererDX::AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
		const char* Name, void* ExtraParam)
	{
		*ppOutTexture = KGE_NEW(TextureDX9)(m_pd3dDevice, this, Handle, FileName, Name, ExtraParam);

		if (FileName)
		{
//			return static_cast<TextureDX9*>(*ppOutTexture)->LoadTexture();
			return false;
		}

		return true;

	} // AddTexture

	//-------------------------------------------------------------------
	// Set a texture
	//-------------------------------------------------------------------
	void RendererDX::SetTexture(Texture* pTex, int index)
	{
		if (!pTex)
		{
			m_pd3dDevice->SetTexture(index, NULL);
			m_nTextID[index] = MAXID;
			return;
		}
		if (m_nTextID[index] != pTex->GetHandle())
		{
			if (pTex->GetFormat() != ETF_NULL)
			{
				m_pd3dDevice->SetTexture(index, ((TextureDX9*)pTex)->m_pTexture);
				if (index < 8)
					m_nTextID[index] = pTex->GetHandle();
			}
			else
			{
				m_pd3dDevice->SetTexture(index, ((TextureDX9*)pTex)->m_pDepth);
				if (index < 8)
					m_nTextID[index] = pTex->GetHandle();
			}
		}

	} // SetTexture

	//------------------------------------
	// Enable kardane khsiyat haye render.
	//------------------------------------
	void RendererDX::Enable(RenderFlags RF)
	{

		switch (RF)
		{
			/////////////////
			// Dorbine 2D //
			///////////////
		case ERF_2D:
			{
				SetTransForm(&m_m2D, ETM_Projection);
				SetTransForm(NULL, ETM_View);
				SetTransForm(NULL, ETM_World);
				m_bRF[ERF_2D] = true;
			}
			break;
			//////////////////////////////
			// Nor Pardazi (Lighting). //
			//////////////////////////// 
		case ERF_Lighting:
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
			m_bRF[ERF_Lighting] = true;
			break;
			/////////////////////////////
			// Shafafiyat (Blending). //
			///////////////////////////
		case ERF_AlphaBlending:
			if (m_bRF[ERF_AlphaBlending])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA); // D3DBLEND_ONE
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//		m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_bRF[ERF_AlphaBlending] = true;
			break;

			//////////////////////////
			// Addition blending   //
			////////////////////////
		case ERF_AdditiveBlending:
			if (m_bRF[ERF_AdditiveBlending])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			m_bRF[ERF_AdditiveBlending] = true;
			break;

			/////////////////
			// Alpha test //
			///////////////
		case ERF_AlphaTest:
			if (m_bRF[ERF_AlphaTest])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
			m_pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			m_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, 100);
			m_bRF[ERF_AlphaTest] = true;
			break;

			//////////////////////
			// Stencil Buffer. //
			////////////////////
		case ERF_StencilBuffer:
			if ( m_Params.UseStencil == false )
				return;
			m_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
			m_bRF[ERF_StencilBuffer] = true;
			break;

			///////////////////////////////
			// Depth Buffer (Z Buffer). //
			/////////////////////////////
		case ERF_DepthBuffer:
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
			m_bRF[ERF_DepthBuffer] = true;
			break;

			/////////////////////////////////
			// Write to the depth buffer. //
			///////////////////////////////
		case ERF_ZWrite:
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_bRF[ERF_ZWrite] = true;
			break;

			////////////////////////////
			// Wire frame rendering. //
			//////////////////////////
		case ERF_WireFrame:
			m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_bRF[ERF_WireFrame] = true;
			break;

		case ERF_Culling:
			if (m_bRF[ERF_Culling])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_bRF[ERF_Culling] = true;
			break;

		case ERF_ColorRendering:
			if (m_bRF[ERF_ColorRendering])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000f);
			m_bRF[ERF_ColorRendering] = true;
			break;

			/////////////////
			// Scissoring //
			///////////////
		case ERF_Scissor:
			m_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			break;

		} // switch (RF)

	} // Enable

	//-------------------------------------
	// Disable kardane khasiyat haye render.
	//-------------------------------------
	void RendererDX::Disable ( RenderFlags RF )
	{
		switch (RF)
		{
			/////////////////
			// Dorbine 2D //
			///////////////
		case ERF_2D:
			SetTransForm((math::Matrix*)&m_mView, ETM_View);
			SetTransForm((math::Matrix*)&m_mProj, ETM_Projection);
			m_bRF[ERF_2D] = false;
			m_pSnmgr->SetActiveCamera(m_pSnmgr->GetActiveCamera());
			break;
			//////////////////////////////
			// Nor Pardazi (Lighting). //
			//////////////////////////// 
		case ERF_Lighting:
			m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			m_bRF[ERF_Lighting] = false;
			break;
			/////////////////////////////
			// Shafafiyat (Blending). //
			///////////////////////////
		case ERF_AlphaBlending:
			if (!m_bRF[ERF_AlphaBlending])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		//	m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_bRF[ERF_AlphaBlending] = false;
			break;

			//////////////////////////
			// Addition blending    //
			//////////////////////////
		case ERF_AdditiveBlending:
			if (!m_bRF[ERF_AdditiveBlending])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_bRF[ERF_AdditiveBlending] = false;
			break;

			/////////////////
			// Alpha test //
			///////////////
		case ERF_AlphaTest:
			if (!m_bRF[ERF_AlphaTest])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			m_bRF[ERF_AlphaTest] = false;
			break;

			//////////////////////
			// Stencil Buffer. //
			////////////////////
		case ERF_StencilBuffer:
			m_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE , FALSE );
			m_bRF[ERF_StencilBuffer] = false;
			break;

			///////////////////////////////
			// Depth Buffer (Z Buffer). //
			/////////////////////////////
		case ERF_DepthBuffer:
			m_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
			m_bRF[ERF_DepthBuffer] = false;
			break;

			/////////////////////////////////
			// Write to the depth buffer. //
			///////////////////////////////
		case ERF_ZWrite:
			m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_bRF[ERF_ZWrite] = false;
			break;

			////////////////////////////
			// Wire frame rendering. //
			//////////////////////////
		case ERF_WireFrame:
			m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			m_bRF[ERF_WireFrame] = false;
			break;

		case ERF_Culling:
			if (!m_bRF[ERF_Culling])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_bRF[ERF_Culling] = false;
			break;

		case ERF_ColorRendering:
			if (!m_bRF[ERF_ColorRendering])
				return;
			m_pd3dDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0);
			m_bRF[ERF_ColorRendering] = false;
			break;

			/////////////////
			// Scissoring //
			///////////////
		case ERF_Scissor:
			m_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			break;

		} // switch (RF)

	}  // Disable

	//--------------------------------------------------
	// Set kardane kamele ya matrial besorate automatic.
	//--------------------------------------------------
	void RendererDX::SetMaterial(Material *mat)
	{
		m_pLastMaterial = mat;

		// Material to set.
		D3DMATERIAL9 mtrl;
		ZeroMemory(&mtrl, sizeof(mtrl));

		// Setting the colors.
		memcpy(&mtrl.Ambient.r, &mat->Ambient.r, sizeof(float) * 4);
		memcpy(&mtrl.Diffuse.r, &mat->Diffuse.r, sizeof(float) * 4);
		memcpy(&mtrl.Specular.r, &mat->Specular.r, sizeof(float) * 4);

		if ( mat->Alpha )
		{
			Enable(gfx::ERF_AlphaBlending);
		}
		else
		{
			Disable(gfx::ERF_AlphaBlending);
		}
		if ( mat->TextureAlphal )
		{
			Enable(gfx::ERF_AlphaTest);
		}
		else
		{
			Disable(gfx::ERF_AlphaTest);

		}
		if ( mat->Culling )
		{
			Enable(gfx::ERF_Culling);
		}
		else
		{
			Disable(gfx::ERF_Culling);
		}

		// Setting the material.
		m_pd3dDevice->SetMaterial(&mtrl);

		// Setting the Texture.
		for (int i = 0; i < 8; i++)
		{
			if (!mat->ppTexture[i])
				break;
			SetTexture(mat->ppTexture[i], i);
		}

		// Setting the shaders.
		if (mat->shader)
		{
			if (mat->shader->m_pFun)
				mat->shader->m_pFun->Call(mat->shader);
			if (mat->shader->m_pVertexShader)
			{
				mat->shader->m_pVertexShader->PreRender();
				mat->shader->m_pVertexShader->Render();
			}
			else if (m_bUseShaders)
			{
				VertexShaderSet = MAXID;
				m_pd3dDevice->SetVertexShader(NULL);
			}
			if (mat->shader->m_pPixelShader)
			{
				mat->shader->m_pPixelShader->PreRender();
				mat->shader->m_pPixelShader->Render();
			}
			else if (m_bUseShaders)
			{
				PixelShaderSet  = MAXID;
				m_pd3dDevice->SetPixelShader(NULL);
			}
		}
		else
		{
			if (m_bUseShaders)
			{
				VertexShaderSet = MAXID;
				m_pd3dDevice->SetVertexShader(NULL);
				PixelShaderSet  = MAXID;
				m_pd3dDevice->SetPixelShader(NULL);
			}
		}

	} // SetMaterial

	//----------------------------------------------------
	// Set kardane keyfiyat texture ha va address mode ha.
	//----------------------------------------------------
	void RendererDX::SetTextureParams(TextureParams Params, int TextureStage)
	{
		DWORD Value = D3DTEXF_LINEAR;
		D3DSAMPLERSTATETYPE Type1, Type2;

		switch (Params)
		{
			//*** F I L T E R I N G S
		case gfx::ETP_Point:
			Value = D3DTEXF_POINT;
			Type1 = D3DSAMP_MINFILTER;
			Type2 = D3DSAMP_MAGFILTER;
			break;

		case gfx::ETP_Linear:
			Value = D3DTEXF_LINEAR;
			Type1 = D3DSAMP_MINFILTER;
			Type2 = D3DSAMP_MAGFILTER;
			break;

		case gfx::ETP_Anisotropic:
			Value = D3DTEXF_ANISOTROPIC;
			Type1 = D3DSAMP_MINFILTER;
			Type2 = D3DSAMP_MAGFILTER;
			break;

			//*** A D D R E S S I N G
		case gfx::ETP_Wrap:
			Value = D3DTADDRESS_WRAP;
			Type1 = D3DSAMP_ADDRESSU;
			Type2 = D3DSAMP_ADDRESSV;
			break;

		case gfx::ETP_BorderColor:
			Value = D3DTADDRESS_BORDER;
			Type1 = D3DSAMP_ADDRESSU;
			Type2 = D3DSAMP_ADDRESSV;
			m_pd3dDevice->SetSamplerState(TextureStage, D3DSAMP_BORDERCOLOR, 0x00ffffff);
			break;

		case gfx::ETP_Clamp:
			Value = D3DTADDRESS_CLAMP;
			Type1 = D3DSAMP_ADDRESSU;
			Type2 = D3DSAMP_ADDRESSV;
			break;

		case gfx::ETP_Mirror:
			Value = D3DTADDRESS_MIRROR;
			Type1 = D3DSAMP_ADDRESSU;
			Type2 = D3DSAMP_ADDRESSV;
			break;
		}

		m_pd3dDevice->SetSamplerState(TextureStage, Type1, Value);
		m_pd3dDevice->SetSamplerState(TextureStage, Type2, Value);

	} // SetTextureParams

	//-------------------------------------------------------------------------------------
	// Baraye fahmidane inke aya cart graphic az on ghabelilat ha poshti bani mikone ya na.
	//-------------------------------------------------------------------------------------
	bool RendererDX::CanDo ( GraphicCardCaps Item )
	{
		return m_bGCC[Item];

	} // CanDo

	//-------------------------------------------------------
	// Tanzimate stencil buffer baraye effect haye mokhtalef.
	//-------------------------------------------------------
	void RendererDX::SetStencilBuffer (efx::EffectType ET, u8 State)
	{
		if ( ET == efx::EET_ShadowPlane )
		{
			switch ( State )
			{
			case 1:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,         D3DCMP_EQUAL);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILREF,          0x1);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILMASK,         0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK,    0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,         D3DSTENCILOP_INCR); 
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL,         D3DSTENCILOP_KEEP);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL,        D3DSTENCILOP_INCR);

				m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,            D3DBLEND_SRCALPHA);
				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,		    D3DBLEND_INVSRCALPHA);

				m_pd3dDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0L);
				m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS,           -1L);
				break;

			case 2:
				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,        TRUE);
				m_pd3dDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0L);
				m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS,           0L);
				break;
			}
		}
		else if ( ET == efx::EET_MirrorPlane )
		{	
			switch ( State )
			{
				// Setting before draw mirror
			case 1:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILREF,       0x1);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILMASK,      0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);

				// disable writes to the depth and back buffers
				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE,     FALSE);
				m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,		 D3DBLEND_ZERO);
				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,        D3DBLEND_ONE);
				break;

				// Setting after draw mirror and before draw reflection into mirror
			case 2:
				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,  D3DCMP_EQUAL);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,  D3DSTENCILOP_KEEP);

				m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,     D3DBLEND_DESTCOLOR);
				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,	 D3DBLEND_ZERO);

				m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,	 D3DCULL_CW);
				break;

			case 3:
				m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,	 D3DCULL_CCW);
				break;

			} // switch ( State )
		}
		else if (ET == efx::EET_Bloom)
		{
			switch (State)
			{
			case 1:
				m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,     D3DBLEND_ONE);
				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,	 D3DBLEND_ONE);

				break;

			case 2:
				m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
//				m_pd3dDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
//				m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND,     D3DBLEND_ONE);
//				m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,	 D3DBLEND_ONE);

				break;
			}
		}
		else if (ET == efx::EET_PointLightInside)
		{
			switch (State)
			{
			case 1:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILREF,       0x1);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_REPLACE);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
				break;
			}
		}
		else if (ET == efx::EET_PointLightOutside)
		{
			switch (State)
			{
			case 1:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_ALWAYS);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILREF,       0x0);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_INCR);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFAIL,      D3DSTENCILOP_KEEP);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILZFAIL,     D3DSTENCILOP_KEEP);
				break;

			case 2:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_DECR);
				break;

			case 3:
				m_pd3dDevice->SetRenderState(D3DRS_STENCILFUNC,      D3DCMP_NOTEQUAL);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILREF,       0x0);
				m_pd3dDevice->SetRenderState(D3DRS_STENCILPASS,      D3DSTENCILOP_KEEP);
				break;
			}
		}

	} // SetStencilBuffer

	//------------
	// Create Font
	//------------
	void RendererDX::AddFont( kge::gui::Font** ppOut, const char* strFontName, ul32 dwHeight )
	{
		*ppOut = NULL;

	} // CreateFont

	//-----------------------------------------------------
	// Create a vertex buffer and store it on video memory.
	//-----------------------------------------------------
	HardwareBuffer* RendererDX::CreateVertexBuffer( void* Vertices, u32 VCount, 
		VertexType eVType , 
		bool isDynamic)
	{
		size_t stride=0;
		DWORD dw = 0;

		switch (eVType)
		{
			// EVT_V3
		case EVT_V3:
			stride = sizeof(Vertex3);
			dw	   = D3DFVF_XYZ;
			break;

			// EVT_V3C
		case EVT_V3C:
			stride = sizeof(Vertex3C);
			dw	   = D3DFVF_XYZ | D3DFVF_DIFFUSE;
			break;

			// EVT_V3T
		case EVT_V3T:
			stride = sizeof(Vertex3T);
			dw	   = D3DFVF_XYZ | D3DFVF_TEX1;
			break;

			// EVT_V3CT
		case EVT_V3CT:
			stride = sizeof(Vertex3CT);
			dw	   = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2;
			break;

			// EVT_V3TN
		case EVT_V3TN:
			stride = sizeof(Vertex3TN);
			dw	   = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
			break;

			// EVT_V3TNC
		case EVT_V3TNC:
			stride = sizeof(Vertex3TNC);
			dw	   = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
			break;

		} // switch (eVType)

		// Create the vertex buffer.
		IDirect3DVertexBuffer9* vb;
		DWORD usage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if (isDynamic)
		{
			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		HR(m_pd3dDevice->CreateVertexBuffer( (UINT)stride * VCount
			, usage
			, dw
			, pool
			, &vb
			, 0 ));

		// Writing to the buffer.
		void* Buffer;

		if (Vertices)
		{
			HR(vb->Lock(0, 0, &Buffer, 0));

			switch (eVType)
			{
				// EVT_V3
			case EVT_V3:
				Vertex3* vBuf, *vV;
				vBuf = (Vertex3*)Buffer; 
				vV   = (Vertex3*)Vertices;
				for (u32 i  = 0; i < VCount; i++)
					vBuf[i] = vV[i];
				break;

				// EVT_V3C
			case EVT_V3C:
				Vertex3C* vBufC, *vVC;
				vBufC = (Vertex3C*)Buffer; 
				vVC   = (Vertex3C*)Vertices;
				for (u32 i  = 0; i < VCount; i++)
					vBufC[i] = vVC[i];
				break;

				// EVT_V3T
			case EVT_V3T:
				Vertex3T* vBufT, *vVT;
				vBufT = (Vertex3T*)Buffer; 
				vVT   = (Vertex3T*)Vertices;
				for (u32 i  = 0; i < VCount; i++)
					vBufT[i] = vVT[i];
				break;

				// EVT_V3CT
			case EVT_V3CT:
				{
					Vertex3CT* vBufCT = (Vertex3CT*)Buffer; 
					Vertex3CT* vVCT = (Vertex3CT*)Vertices;
					for (u32 i = 0; i < VCount; ++i)
					{
						vBufCT[i] = vVCT[i];
					}
				}
				break;

				// EVT_V3TN
			case EVT_V3TN:
				Vertex3TN* vBufTN, *vVTN;
				vBufTN = (Vertex3TN*)Buffer; 
				vVTN   = (Vertex3TN*)Vertices;
				for (u32 i  = 0; i < VCount; i++)
					vBufTN[i] = vVTN[i];
				break;

				// EVT_V3TNC
			case EVT_V3TNC:
				Vertex3TNC* vBufTNC, *vVTNC;
				vBufTNC = (Vertex3TNC*)Buffer; 
				vVTNC   = (Vertex3TNC*)Vertices;
				for (u32 i  = 0; i < VCount; i++)
					vBufTNC[i] = vVTNC[i];
				break;

			} // switch (eVType)

			HR(vb->Unlock());
		}

		VertexBufferDX* ppVBout;
		ppVBout			= KGE_NEW(VertexBufferDX)(VCount, (u32)stride, dw);
		ppVBout->m_pVB	= vb;
		ppVBout->Dynamic(isDynamic);
		
		m_vVBuffers.push_back(ppVBout);

		return ppVBout;

	} // CreateVertexBuffer

	//----------------------------------------------------------------------------------------
	// Create a vertex buffer from custom vertex type and store it on video memory.
	//----------------------------------------------------------------------------------------
	HardwareBuffer* RendererDX::CreateVertexBuffer( void* Vertices, u32 VCount, 
		u32   Stride,
		bool  isDynamic)
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
		HR(m_pd3dDevice->CreateVertexBuffer( Stride * VCount
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

		VertexBufferDX* ppVBout;
		ppVBout			= KGE_NEW(VertexBufferDX)(VCount, Stride, 0);
		ppVBout->m_pVB	= vb;
		ppVBout->Dynamic(isDynamic);
		
		if (isDynamic) m_vVBuffers.push_back(ppVBout);

		return ppVBout;

	} // CreateVertexBuffer

	//----------------------------------------------------------------------------------------
	// Creates an Index buffer on video memory
	//----------------------------------------------------------------------------------------
	HardwareBuffer* RendererDX::CreateIndexBuffer(	void* Indices, u32 ICount,
		IndexBufferType eIndexBufferType ,
		bool isDynamic)
	{
		// Create the index buffer.
		DWORD usage = 0;
		D3DPOOL pool = D3DPOOL_MANAGED;

		if (isDynamic)
		{
			usage = D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY;
			pool = D3DPOOL_DEFAULT;
		}

		D3DFORMAT fmt = D3DFMT_INDEX16;
		u32 size = sizeof( u16 );
		if (eIndexBufferType == EIBT_32Bit)
		{
			fmt = D3DFMT_INDEX32;
			size = sizeof( u32 );
		}
		IDirect3DIndexBuffer9* ib;
		void* Buffer;
		HR(m_pd3dDevice->CreateIndexBuffer( ICount * size,
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

		IndexBufferDX* ibOut = KGE_NEW(IndexBufferDX)(ICount, size, fmt);
		ibOut->m_pIB		 = ib;
		ibOut->Dynamic(isDynamic);
		
		if (isDynamic) m_vIBuffers.push_back(ibOut);

		return ibOut;

	} // CreateIndexBuffer

	//----------------------------------------------------------------------------------------
	// Draw a vertex buffer.
	//----------------------------------------------------------------------------------------
	void RendererDX::DrawTriangleList(HardwareBuffer* VB, HardwareBuffer* IB, u32 VCount, u32 ICount,
		VertexType eVType)
	{
		size_t stride=0;
		DWORD dw = 0;

		if ( m_bUseShaders )
		{
			switch (eVType)
			{
				// EVT_V3
			case EVT_V3:
				stride = sizeof(Vertex3);
				break;

				// EVT_V3C
			case EVT_V3C:
				stride = sizeof(Vertex3C);
				break;

				// EVT_V3T
			case EVT_V3T:
				stride = sizeof(Vertex3T);
				break;

				// EVT_V3CT
			case EVT_V3CT:
				stride = sizeof(Vertex3CT);
				break;

				// EVT_V3TN
			case EVT_V3TN:
				stride = sizeof(Vertex3TN);
				break;

				// EVT_V3TNC
			case EVT_V3TNC:
				stride = sizeof(Vertex3TNC);
				break;

			} // switch (eVType)
		}
		else
		{
			switch (eVType)
			{
				// EVT_V3
			case EVT_V3:
				stride = sizeof(Vertex3);
				dw	   = D3DFVF_XYZ;
				break;

				// EVT_V3C
			case EVT_V3C:
				stride = sizeof(Vertex3C);
				dw	   = D3DFVF_XYZ | D3DFVF_DIFFUSE;
				break;

				// EVT_V3T
			case EVT_V3T:
				stride = sizeof(Vertex3T);
				dw	   = D3DFVF_XYZ | D3DFVF_TEX1;
				break;

				// EVT_V3CT
			case EVT_V3CT:
				stride = sizeof(Vertex3CT);
				dw	   = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEX2;
				break;

				// EVT_V3TN
			case EVT_V3TN:
				stride = sizeof(Vertex3TN);
				dw	   = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
				break;

				// EVT_V3TNC
			case EVT_V3TNC:
				stride = sizeof(Vertex3TNC);
				dw	   = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_DIFFUSE;
				break;

			} // switch (eVType)
		}
		SetVertexBuffer(VB, 0);

		if ( m_bUseShaders )
			SetVertexDec( m_ppVertexDecs[eVType] ); 	
		else
			m_pd3dDevice->SetFVF(dw);

		if ( ICount > 0 )
		{
			SetIndexBuffer(IB);
			m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VCount, 0, ICount / 3);
			m_iTriCount += ICount / 3;
		}
		else
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VCount / 3);
			m_iTriCount += VCount / 3;
		}
		m_iDrawCount++;

	} // DrawTriangleList(Vertex buffer)

	//----------------------------------------------------------------------------------------
	// Draw a vertex buffer
	//----------------------------------------------------------------------------------------
	void RendererDX::DrawTriangleList(u32 VCount, u32 ICount,
		VertexDec* CV, u32 VertexStart, u32 StartIndex)
	{
		SetVertexDec(CV);

		if ( ICount > 0 )
		{
			m_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, VertexStart, 0, VCount, StartIndex, ICount / 3);
			m_iTriCount += ICount / 3 * m_iBatchCount;
		}
		else
		{
			m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, VertexStart, VCount / 3);
			m_iTriCount += VCount / 3 * m_iBatchCount;
		}
		m_iDrawCount++;
	}

	//-------------------------------------------------------------------------
	// Load a shader from a file then compile it and return the created shader.
	//-------------------------------------------------------------------------
	Shader* RendererDX::CreateVertexShaderFromFile(const char* FileName, const char* VertexMain, 
		ShaderVersion eVersion)
	{
		//
		// Compile shader.
		//
		ID3DXBuffer* shader			    = NULL;
		ID3DXBuffer* errorBuffer	    = NULL;
		ID3DXConstantTable* ConstTable  = NULL;
		HRESULT hr = S_OK;

		if (VertexMain)
			hr = D3DXCompileShaderFromFile(FileName, NULL, NULL, VertexMain, 
			ShaderVersionName[eVersion], D3DXSHADER_DEBUG,
			&shader, &errorBuffer, &ConstTable);

		//
		// Check for compile errors.
		//
		if (errorBuffer)
		{
			io::Logger::Log("Can't compile vertex shader code:\n",
				static_cast<const char*>(errorBuffer->GetBufferPointer()),
				io::ELM_Error);
			errorBuffer->Release();
			return NULL;
		}

		//
		// Create shader
		//

		IDirect3DVertexShader9* vShader = NULL;
		if (VertexMain)
		{
			hr = m_pd3dDevice->CreateVertexShader(
				(DWORD*)shader->GetBufferPointer(),
				&vShader);

			if (FAILED(hr))
			{
				io::Logger::Log("Can't create vertex shader from ", FileName, io::ELM_Error);
				return NULL;
			}
			shader->Release();
		}

		Shader *sh = KGE_NEW(VertexShaderDX)(m_pd3dDevice, -1, vShader, ConstTable, 
			FileName, NULL);
		m_pDevice->GetVertexShaderManager()->Add(sh);

		return sh;

	} // CreateVertexShaderFromFile

	//---------------------------------------------------------------------------
	// Load a shader from a string then compile it and return the created shader.
	//---------------------------------------------------------------------------
	Shader* RendererDX::CreateVertexShaderFromString(const char* VertexCode, const char* VertexMain , 
		ShaderVersion eVVersion )
	{
		//
		// Compile shader.
		//
		ID3DXBuffer* shader			    = NULL;
		ID3DXBuffer* errorBuffer	    = NULL;
		ID3DXConstantTable* ConstTable  = NULL;
		HRESULT hr = S_OK;

		if (VertexMain)
			hr = D3DXCompileShader(VertexCode, (UINT)strlen(VertexCode), NULL, NULL, VertexMain, 
			ShaderVersionName[eVVersion], D3DXSHADER_DEBUG | D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY,
			&shader, &errorBuffer, &ConstTable);

		//
		// Check for compile errors.
		//
		if (errorBuffer)
		{
			io::Logger::Log("Can't compile vertex shader code:\n",
				static_cast<const char*>(errorBuffer->GetBufferPointer()),
				io::ELM_Error);
			errorBuffer->Release();
		}

		if (FAILED(hr))
		{
			io::Logger::Log("D3DXCompileShader FAILED.",
				io::ELM_Error);
			return NULL;
		}

		//
		// Create shader
		//
		IDirect3DVertexShader9* vShader = NULL;
		if (VertexMain)
		{
			hr = m_pd3dDevice->CreateVertexShader(
				(DWORD*)shader->GetBufferPointer(),
				&vShader);

			if (FAILED(hr))
			{
				io::Logger::Log("Can't create vertex shader from string.", io::ELM_Error);
				return NULL;
			}
			shader->Release();
		}

		Shader *sh = KGE_NEW(VertexShaderDX)(m_pd3dDevice, -1, vShader, ConstTable 
			, NULL, NULL);
		m_pDevice->GetVertexShaderManager()->Add(sh);

		return sh;

	} // CreateVertexShaderFromString

	//----------------------------------------------------------------------------------------
	// Load a pixel shader from a file then compile it and return the created pixel shader
	//----------------------------------------------------------------------------------------
	Shader* RendererDX::CreatePixelShaderFromFile(const char* PixelFileName, const char* PixelMain, ShaderVersion ePVersion )
	{
		//
		// Compile shader.
		//
		ID3DXBuffer* Pshader		    = NULL;
		ID3DXBuffer* PerrorBuffer	    = NULL;
		ID3DXConstantTable* PConstTable = NULL;
		HRESULT Phr = S_OK;

		// Pixel shader
		if (PixelMain)
			Phr = D3DXCompileShaderFromFile(PixelFileName, NULL, NULL, PixelMain, 
			ShaderVersionName[ePVersion], D3DXSHADER_DEBUG,
			&Pshader, &PerrorBuffer, &PConstTable);

		//
		// Check for compile errors.
		//
		if (PerrorBuffer)
		{
			io::Logger::Log("Can't compile pixel shader code:\n",
				static_cast<const char*>(PerrorBuffer->GetBufferPointer()),
				io::ELM_Error);
			PerrorBuffer->Release();
			return NULL;
		}

		if (FAILED(Phr))
		{
			io::Logger::Log("D3DXCompileShaderFromFileA FAILED.",
				io::ELM_Error);
			return NULL;
		}

		//
		// Create shader
		//

		IDirect3DPixelShader9* pShader = NULL;
		if (PixelMain)
		{
			Phr = m_pd3dDevice->CreatePixelShader(
				(DWORD*)Pshader->GetBufferPointer(),
				&pShader);

			if (FAILED(Phr))
			{
				io::Logger::Log("Can't create pixel shader from ", PixelFileName, io::ELM_Error);
				return NULL;
			}
			Pshader->Release();
		}

		Shader *sh = KGE_NEW(PixelShaderDX)(m_pd3dDevice, -1, pShader, PConstTable, 
			PixelFileName, NULL);
		m_pDevice->GetPixelShaderManager()->Add(sh);

		return sh;

	} // CreatePixelShaderFromFile

	//----------------------------------------------------------------------------------------
	// Load a pixel shader from a string then compile it and return the created pixel shader
	//----------------------------------------------------------------------------------------
	Shader* RendererDX::CreatePixelShaderFromString(const char* PixelCode, const char* PixelMain, ShaderVersion ePVersion )
	{
		//
		// Compile shader.
		//
		ID3DXBuffer* Pshader		    = NULL;
		ID3DXBuffer* PerrorBuffer	    = NULL;
		ID3DXConstantTable* PConstTable = NULL;
		HRESULT Phr = S_OK;

		// Pixel shader
		if (PixelMain)
			Phr = D3DXCompileShader(PixelCode, (UINT)strlen(PixelCode), NULL, NULL, PixelMain, 
			ShaderVersionName[ePVersion], D3DXSHADER_DEBUG,
			&Pshader, &PerrorBuffer, &PConstTable);

		//
		// Check for compile errors.
		//
		if (PerrorBuffer)
		{
			io::Logger::Log("Can't compile pixel shader code:\n",
				static_cast<const char*>(PerrorBuffer->GetBufferPointer()),
				io::ELM_Error);
			PerrorBuffer->Release();
		}

		if (FAILED(Phr))
		{
			io::Logger::Log("D3DXCompileShader FAILED.",
				io::ELM_Error);
			return NULL;
		}

		//
		// Create shader
		//
		IDirect3DPixelShader9* pShader = NULL;
		if (PixelMain)
		{
			Phr = m_pd3dDevice->CreatePixelShader(
				(DWORD*)Pshader->GetBufferPointer(),
				&pShader);

			if (FAILED(Phr))
			{
				io::Logger::Log("Can't create pixel shader from string ", io::ELM_Error);
				return NULL;
			}
			Pshader->Release();
		}

		Shader *sh = KGE_NEW(PixelShaderDX)(m_pd3dDevice, -1, pShader, PConstTable, 
			NULL, NULL);
		m_pDevice->GetPixelShaderManager()->Add(sh);

		return sh;

	} // CreatePixelShaderFromString

	//-------------------------------------------
	//
	//-------------------------------------------
	bool RendererDX::AddVertexShader(Shader** ppOutShader, u32 handle, const char* fileName, const char* mainName, void* extraParam)
	{
		ID3DXBuffer* pshader = 0;
		ID3DXBuffer* pErrorBuffer = 0;
		ID3DXConstantTable* pConstTable = 0;
		HRESULT result = S_OK;
		ShaderVersion version = *((ShaderVersion*)extraParam);
		DWORD flag = 0;

#if defined(DEBUG) | defined(_DEBUG)
		flag = D3DXSHADER_DEBUG;
#endif

		if (mainName)
		{
			result = D3DXCompileShaderFromFile(fileName, 0, 0, mainName, 
				ShaderVersionName[version], flag,
				&pshader, &pErrorBuffer, &pConstTable);
		}

		if (pErrorBuffer)
		{
			io::Logger::Log("Can't compile vertex shader code:\n",
				static_cast<const char*>(pErrorBuffer->GetBufferPointer()),
				io::ELM_Error);
			pErrorBuffer->Release();
			return false;
		}

		if (FAILED(result))
		{
			io::Logger::Log("D3DXCompileShaderFromFileA FAILED.", io::ELM_Error);
			return false;
		}

		IDirect3DVertexShader9* pVShader = 0;

		if (mainName)
		{
			result = m_pd3dDevice->CreateVertexShader((DWORD*)pshader->GetBufferPointer(), &pVShader);

			if (FAILED(result))
			{
				io::Logger::Log("Can't create vertex shader from ", fileName, io::ELM_Error);
				return false;
			}
			pshader->Release();
		}

		Shader *sh = KGE_NEW(VertexShaderDX)(m_pd3dDevice, handle, pVShader, pConstTable, fileName, NULL);

		*ppOutShader = sh;
		return true;
	}

	//-------------------------------------------
	//
	//-------------------------------------------
	bool RendererDX::AddPixelShader(Shader** ppOutShader, u32 handle, const char* fileName, const char* mainName, void* extraParam)
	{
		ID3DXBuffer* pshader = 0;
		ID3DXBuffer* pErrorBuffer = 0;
		ID3DXConstantTable* pConstTable = 0;
		HRESULT result = S_OK;
		ShaderVersion version = *((ShaderVersion*)extraParam);
		DWORD flag = 0;

#if defined(DEBUG) | defined(_DEBUG)
		flag = D3DXSHADER_DEBUG;
#endif

		if (mainName)
		{
			result = D3DXCompileShaderFromFile(fileName, 0, 0, mainName, 
			ShaderVersionName[version], flag,
			&pshader, &pErrorBuffer, &pConstTable);
		}

		if (pErrorBuffer)
		{
			io::Logger::Log("Can't compile pixel shader code:\n",
				static_cast<const char*>(pErrorBuffer->GetBufferPointer()),
				io::ELM_Error);
			pErrorBuffer->Release();
			return false;
		}

		if (FAILED(result))
		{
			io::Logger::Log("D3DXCompileShaderFromFileA FAILED.", io::ELM_Error);
			return false;
		}

		IDirect3DPixelShader9* pPShader = 0;

		if (mainName)
		{
			result = m_pd3dDevice->CreatePixelShader((DWORD*)pshader->GetBufferPointer(), &pPShader);

			if (FAILED(result))
			{
				io::Logger::Log("Can't create pixel shader from ", fileName, io::ELM_Error);
				return false;
			}
			pshader->Release();
		}

		Shader *sh = KGE_NEW(PixelShaderDX)(m_pd3dDevice, handle, pPShader, pConstTable, fileName, NULL);

		*ppOutShader = sh;
		return true;
	}

	//-------------------------------------------
	// Precalculate the graphics card capacities.
	//-------------------------------------------
	void RendererDX::InitGCC()
	{
		// Checking the vertex shader versions support.
		if ( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION ( 3 , 0 ) )
		{
			if ( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION ( 2 , 0 ) )
			{
				if ( m_d3dCaps.VertexShaderVersion < D3DVS_VERSION ( 1 , 1 ) )
				{
					m_bGCC[ gfx::EGCC_VertexShader1_1 ] = false;
					m_bGCC[ gfx::EGCC_VertexShader2 ] = false;
					m_bGCC[ gfx::EGCC_VertexShader3 ] = false;
				}
				else
				{
					m_bGCC[ gfx::EGCC_VertexShader1_1 ] = true;
					m_bGCC[ gfx::EGCC_VertexShader2 ] = false;
					m_bGCC[ gfx::EGCC_VertexShader3 ] = false;
				}
			}
			else
			{
				m_bGCC[ gfx::EGCC_VertexShader1_1 ] = true;
				m_bGCC[ gfx::EGCC_VertexShader2 ] = true;
				m_bGCC[ gfx::EGCC_VertexShader3 ] = false;
			}
		}
		else
		{
			m_bGCC[ gfx::EGCC_VertexShader1_1 ] = true;
			m_bGCC[ gfx::EGCC_VertexShader2 ] = true;
			m_bGCC[ gfx::EGCC_VertexShader3 ] = true;
		}

		// Checking the pixel shader versions support.
		if ( m_d3dCaps.PixelShaderVersion  < D3DPS_VERSION ( 3 , 0 ) )
		{
			if ( m_d3dCaps.PixelShaderVersion  < D3DPS_VERSION ( 2 , 0 ) )
			{
				if ( m_d3dCaps.PixelShaderVersion  < D3DPS_VERSION ( 1 , 4 ) )
				{
					if ( m_d3dCaps.PixelShaderVersion  < D3DPS_VERSION ( 1 , 1 ) )
					{
						m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = false;
						m_bGCC[ gfx::EGCC_FragmentShader1_4 ] = false;
						m_bGCC[ gfx::EGCC_FragmentShader2 ] = false;
						m_bGCC[ gfx::EGCC_FragmentShader3 ] = false;
					}
					else
					{
						m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = true;
						m_bGCC[ gfx::EGCC_FragmentShader1_4 ] = false;
						m_bGCC[ gfx::EGCC_FragmentShader2 ] = false;
						m_bGCC[ gfx::EGCC_FragmentShader3 ] = false;
					}
				}
				else
				{
					m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = true;
					m_bGCC[ gfx::EGCC_FragmentShader1_4 ] = true;
					m_bGCC[ gfx::EGCC_FragmentShader2 ] = false;
					m_bGCC[ gfx::EGCC_FragmentShader3 ] = false;
				}
			}
			else
			{
				m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = true;
				m_bGCC[ gfx::EGCC_FragmentShader1_4 ] = true;
				m_bGCC[ gfx::EGCC_FragmentShader2 ] = true;
				m_bGCC[ gfx::EGCC_FragmentShader3 ] = false;
			}
		}
		else
		{
			m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = true;
			m_bGCC[ gfx::EGCC_FragmentShader1_4 ] = true;
			m_bGCC[ gfx::EGCC_FragmentShader2 ] = true;
			m_bGCC[ gfx::EGCC_FragmentShader3 ] = true;
		}

		// EGCC_OpenGL2
		m_bGCC[ gfx::EGCC_OpenGL2 ] = false;

		// EGCC_ShaderObject
		m_bGCC[ gfx::EGCC_ShaderObject ] = true;

		// EGCC_ShaderModel4
		m_bGCC[ gfx::EGCC_ShaderModel4 ] = false;

		// EGCC_GeometryShader4
		m_bGCC[ gfx::EGCC_GeometryShader4 ] = false;

		// Check texture format supports.
		UINT nAdapter = D3DADAPTER_DEFAULT;
		D3DFORMAT fmtAdapter = D3DFMT_X8R8G8B8;
		//D3DFORMAT fmtBack = D3DFMT_X8R8G8B8;

		m_bGCC[ gfx::EGCC_DXT1 ] = true;
		m_bGCC[ gfx::EGCC_DXT3 ] = true;
		m_bGCC[ gfx::EGCC_DXT5 ] = true;

		if (FAILED(m_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,0
			,D3DRTYPE_TEXTURE
			,D3DFMT_DXT1)))
			m_bGCC[ gfx::EGCC_DXT1 ] = false;
		if (FAILED(m_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,0
			,D3DRTYPE_TEXTURE
			,D3DFMT_DXT3)))
			m_bGCC[ gfx::EGCC_DXT3 ] = false;
		if (FAILED(m_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,0
			,D3DRTYPE_TEXTURE
			,D3DFMT_DXT5)))
			m_bGCC[ gfx::EGCC_DXT5 ] = false;
		if (FAILED(m_pD3D->CheckDeviceFormat(nAdapter
			,D3DDEVTYPE_HAL
			,fmtAdapter
			,0
			,D3DRTYPE_TEXTURE
			,FOURCC_INTZ)))
			m_bGCC[ gfx::EGCC_INTZ ] = false;		

	} // InitGCC

	//---------------------------------------
	// set fog settings 
	//---------------------------------------
	bool RendererDX::SetFog(FogType mode,Colorf FogColor ,float expDensity,float linearStart,float linearEnd )
	{	

		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,true);
		switch (mode)
		{
		case EFT_NONE:
			m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE, false);
			return true;
			break;
		case EFT_LINEAR:
			m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
			m_pd3dDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD*)(&linearStart));
			m_pd3dDevice->SetRenderState(D3DRS_FOGEND,*(DWORD*)(&linearEnd));
			break;
		case EFT_EXP:
			m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_EXP);
			m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD*)(&expDensity));
			break;
		case EFT_EXP2:
			m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_EXP2);
			m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY,*(DWORD*)(&expDensity));
			break;
		}

		m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR,D3DCOLOR_COLORVALUE(FogColor.r ,FogColor.g,FogColor.b,1.0));

		return true;
	}

	//---------------------------------------
	// set video settings 
	//---------------------------------------
	bool RendererDX::SetVideoMode(int Width, int Height , int Bits, 
		bool UseStencil , bool Fullscreen ,int MultiSample,bool VSync)
	{
		UINT	  nAdapter	 = D3DADAPTER_DEFAULT;
		//D3DFORMAT fmtAdapter = D3DFMT_X8R8G8B8;
		D3DFORMAT fmtBack	 = D3DFMT_X8R8G8B8;
		kge::ul32 msQuality;

		for(int nMS = MultiSample-1; nMS >= 0; nMS--) 
		{
			if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(
				nAdapter
				,D3DDEVTYPE_HAL
				,fmtBack
				,!m_Params.FullScreen
				,g_msType[nMS], &msQuality))) 
			{
				m_msType = g_msType[nMS];
				break;
			}
		} // for [multisample]

		m_d3dpp.Windowed		= !Fullscreen;
		m_d3dpp.MultiSampleType = m_msType;

		if (VSync)
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; 
		else
			m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; 

		// fullscreen mode
		if (Fullscreen)
		{
			m_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
			m_d3dpp.BackBufferWidth = Width;
			m_d3dpp.BackBufferHeight = Height;
			m_d3dpp.FullScreen_RefreshRateInHz = 60;
		}
		// Windowed mode
		else
		{
			m_d3dpp.BackBufferWidth = Width;
			m_d3dpp.BackBufferHeight = Height;
			m_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		}

		m_Params.Width  = Width;
		m_Params.Height = Height;

		SetPerspectiveLH(kge::math::PI / 4.0f, 1.0f, 1000.0f);

		return ResetDevice();
	}

	//----------------------------------------------------------------------------------------
	// Reset the direct3d device.
	//----------------------------------------------------------------------------------------
	bool RendererDX::ResetDevice()
	{
		HRESULT hr = m_pd3dDevice->Reset( &m_d3dpp );

		if (FAILED(hr))
		{
			if (hr == D3DERR_DEVICELOST)
			{
				io::Logger::Log("The direct3d device has been lost." , io::ELM_Error);
			}
			else
			{
				io::Logger::Log("Can't reset the Direct3D device." , io::ELM_Error);
				// TODO: The direct3d device can not be reset, We should recreate device.
			}

			return false;
		}

		io::Logger::Log("Direct3D device reseted.");

		// Restore the projection and view matrices.
		m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_mProj);
		m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_mView);

		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		m_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// Calculate the 2D camera matrix.
		m_m2D._11 = (float)2/(m_Params.Width);
		m_m2D._22 = (float)2/(-m_Params.Height);
		m_m2D._12 = m_m2D._13 = m_m2D._14 = m_m2D._21 = m_m2D._23 = m_m2D._24 =
			m_m2D._31 = m_m2D._32 = m_m2D._34 = m_m2D._43 = 0.0f;
		m_m2D._41 = -1.0f;
		m_m2D._44 = m_m2D._33 = m_m2D._42 = 1.0f;

		// Set the render states.
		for(int i = 1 ; i < ERF_Count; i++)
		{
			if (m_bRF[i])
				Enable( (RenderFlags) i);
			else
				Disable( (RenderFlags) i);
		}

		Enable(ERF_DepthBuffer);
		Enable(ERF_Culling);
		Enable(ERF_ZWrite);
		Disable(ERF_AlphaBlending);
		Disable(ERF_AdditiveBlending);

		SetTextureParams(gfx::ETP_Clamp, 6);
		SetTextureParams(kge::gfx::ETP_Anisotropic, 7);

		// Set the ambient light.
		Lights->SetAmbientLight(Lights->GetAmbientLight());

		// TODO: Reload the textures.
		// Delete the textures.
		//for(std::vector<LPDIRECT3DTEXTURE9>::iterator it = m_vTextures.begin(); 
		//	it != m_vTextures.end(); it++)
		//{
		//	(*it)->Release();
		//}
		//m_vTextures.clear();

		//// Getting the texture names.
		//std::vector<char*> temp;
		//for(std::vector<char*>::iterator it = m_vTextureNames.begin(); 
		//	it != m_vTextureNames.end(); it++)
		//{
		//	temp.push_back((*it));
		//}
		//m_vTextureNames.clear();

		//// Reloads the textures.
		//m_TextCount = 0;
		//m_nTextID	= MAXID;
		//for(std::vector<char*>::iterator it = temp.begin(); 
		//	it != temp.end(); it++)
		//{
		//	u32 u = AddTexture((*it));
		//}
		//temp.clear();

		// TODO: If we use D3DPOOL_DEFUALT we need to release buffers and re create them.
		// Recreate the vertex buffers.

		//// Check the buffers.
		//core::ArrayPri<sn::Mesh*> MeshArray;
		//	io::Logger::Log("TEST");
		//	io::Logger::Log(io::ELM_Warning, "%d", m_pSnmgr->m_vMeshBuffer.size());
		//for(std::vector<MeshBuffer*>::iterator it = m_pSnmgr->m_vMeshBuffer.begin(); 
		//	it != m_pSnmgr->m_vMeshBuffer.end(); it++)
		//{
		//	for (u32 i = 0; i < (*it)->m_iMeshCount; i++)
		//	{
		//		if ((*it)->m_pMeshes[i].m_VBufferID > -1)
		//		{
		//			MeshArray.PushBack(&(*it)->m_pMeshes[i], (*it)->m_pMeshes[i].m_VBufferID);
		//		}
		//	}
		//}

		//// Delete current vertex buffers.
		//io::Logger::Log(io::ELM_Warning, "%d", MeshArray.Count());

		OnReset();

		if (m_clbk_OnReset) m_clbk_OnReset(this);
		m_bDeviceLost = false;

		return true;
	}

	//----------------------------------------------------------------------------------------
	// Sets the rendering view port.
	//----------------------------------------------------------------------------------------
	void RendererDX::SetViewPort(const core::RectI& viewport)
	{
		D3DVIEWPORT9 vp;
		vp.X	  = viewport.X;
		vp.Y	  = viewport.Y;
		vp.Width  = viewport.Width;
		vp.Height = viewport.Height;
		vp.MinZ   = 0.0f;
		vp.MaxZ   = 1.0f;
		m_rViewPort = viewport;
		m_pd3dDevice->SetViewport(&vp);

	} // SetViewPort

	//----------------------------------------------------------------------------------------
	// creates a custom vertex type and put it in pOut parameter.
	//----------------------------------------------------------------------------------------
	VertexDec* RendererDX::CreateVertexDeclaration(CustomVertexElement *pVertexInfoArray
		, core::stringc sName)
	{		
		IDirect3DVertexDeclaration9* vd;
		m_pd3dDevice->CreateVertexDeclaration((D3DVERTEXELEMENT9*)pVertexInfoArray, &vd);
		VertexDec* pOut	  = KGE_NEW(VertexDec)(sName);
		pOut->m_VertexDec = vd;

		AddVertexDec(pOut);

		return pOut;

	} // CreateCustomVertexType

	void* RendererDX::GetDirect3dDevice()
	{
		return m_pd3dDevice; 

	} // GetDirect3dDevice

	//----------------------------------------------------------------------------------------
	// Sets the vertex buffer for multi streaming
	//----------------------------------------------------------------------------------------
	void RendererDX::SetVertexBuffer( HardwareBuffer* pBuffer, int stage )
	{
		if (!pBuffer)
		{
			//			if (m_nVertexBufferID[stage] != MAXID)
			//			{
			//				m_nVertexBufferID[stage] = MAXID;
			//				m_pd3dDevice->SetStreamSource(stage, NULL, 0, 0);
			//			}
			return;
		}

		if (m_nVertexBufferID[stage] == pBuffer->GetID())
			return;

		m_nVertexBufferID[stage] = pBuffer->GetID();

		m_pd3dDevice->SetStreamSource(stage, ((VertexBufferDX*)pBuffer)->m_pVB, 0, pBuffer->GetStride());

	} // SetVertexBuffer

	//----------------------------------------------------------------------------------------
	// Sets the index buffer for rendering
	//----------------------------------------------------------------------------------------
	void RendererDX::SetIndexBuffer( HardwareBuffer* pBuffer )
	{
		if (!pBuffer)
		{
			//if (m_nIndexBufferID != MAXID)
			//{
			//	m_nIndexBufferID = MAXID;
			//	m_pd3dDevice->SetIndices(NULL);
			//}
			return;
		}

		if (m_nIndexBufferID == pBuffer->GetID())
			return;

		m_nIndexBufferID = pBuffer->GetID();

		m_pd3dDevice->SetIndices(((IndexBufferDX*)pBuffer)->m_pIB);

	} // SetIndexBuffer

	//----------------------------------------------------------------------------------------
	// Sets the vertex declaration 
	//----------------------------------------------------------------------------------------
	void RendererDX::SetVertexDec( VertexDec* pVD )
	{
		if (m_nVertexDecID != pVD->GetID())
		{
			m_nVertexDecID = pVD->GetID();
			m_pd3dDevice->SetVertexDeclaration( (IDirect3DVertexDeclaration9*)pVD->m_VertexDec ); 	
		}

	} // SetVertexDec

	//----------------------------------------------------------------------------------------
	// Set the second rendering window
	//----------------------------------------------------------------------------------------
	void RendererDX::SetRenderingWindow( void* hwnd )
	{
		m_d3dpp.hDeviceWindow = (HWND)hwnd;

	} // SetRenderingWindow

	//----------------------------------------------------------------------------------------
	// Resize the view port and rendering
	//----------------------------------------------------------------------------------------
	void RendererDX::OnResize( int width, int height )
	{
		m_Params.Width = width;
		m_Params.Height = height;
		efx::EffectManager::GetSingletonPtr()->OnResize();

		if (m_eRendererPipeline == ERP_Deferred)
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
			CreateRenderableTexture(&m_pPosMap, m_Params.Width, m_Params.Height,
				ETF_R32F, false);
			CreateRenderableTexture(&m_pDifMap, m_Params.Width, m_Params.Height,
				ETF_A8R8G8B8, false);
			CreateRenderableTexture(&m_pNorMap, m_Params.Width, m_Params.Height,
				ETF_R16G16F, false);
		}

		if (m_pScreenQuad)
		{
			m_pScreenQuad->SetSize(width, height);
		}

		//OnResize(width, height,
		//    !m_d3dpp.Windowed,
		//    m_d3dpp.PresentationInterval != D3DPRESENT_INTERVAL_IMMEDIATE);

	} // OnResize

	//-------------------------------------------------------
	// Change resolution, toggle full screen, turn on/off vertical sync
	//-------------------------------------------------------
	void RendererDX::OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync)
	{
		m_Params.Width = width;
		m_Params.Height = height;
		efx::EffectManager::GetSingletonPtr()->OnResize();
		if (m_eRendererPipeline == ERP_Deferred)
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
			CreateRenderableTexture(&m_pPosMap, m_Params.Width, m_Params.Height,
				ETF_R32F, false);
			CreateRenderableTexture(&m_pDifMap, m_Params.Width, m_Params.Height,
				ETF_A8R8G8B8, false);
			CreateRenderableTexture(&m_pNorMap, m_Params.Width, m_Params.Height,
				ETF_R16G16F, false);
		}

		if (m_pScreenQuad)
		{
			m_pScreenQuad->SetSize(width, height);
		}

		D3DDISPLAYMODE mode = {0};
		if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
			return;

		m_d3dpp.BackBufferWidth = width;
		m_d3dpp.BackBufferHeight = height;
		m_d3dpp.Windowed = !fullscreen;
		m_d3dpp.PresentationInterval = vsync ? D3DPRESENT_INTERVAL_ONE : D3DPRESENT_INTERVAL_IMMEDIATE;
		m_d3dpp.FullScreen_RefreshRateInHz = fullscreen ? mode.RefreshRate : 0;

		// init view port
		m_rViewPort.X		= 0;
		m_rViewPort.Y		= 0;
		m_rViewPort.Width	= m_Params.Width;
		m_rViewPort.Height	= m_Params.Height;

		if (!m_bDeviceLost)
		{
			OnLost();
		}
		m_bDeviceLost = true;

		ResetDevice();
	}

	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	void RendererDX::RemoveVertexBuffer(VertexBufferDX* vb)
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
	//
	//-------------------------------------------------------
	void RendererDX::RemoveIndexBuffer(IndexBufferDX* ib)
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

	//----------------------------------------------------
	// Release all resources created with D3DPOOL_DEFAULT
	//----------------------------------------------------
	void RendererDX::OnLost()
	{
		SendMessage(m_d3dpp.hDeviceWindow, KGE_DEVICE_LOST, 0, 0);

		if (m_clbk_OnLost)
			m_clbk_OnLost(this);

		if (m_pBackBufferSurface)
		{
			m_pBackBufferSurface->Release();
			m_pBackBufferSurface = 0;
		}

		if (m_pBackBufferZSurface)
		{
			m_pBackBufferZSurface->Release();
			m_pBackBufferZSurface = 0;
		}

		for (std::vector<TextureDX9*>::iterator it = m_vTextures.begin();
			it != m_vTextures.end(); ++it)
		{
			(*it)->OnLost();
		}

		for (std::vector<VertexBufferDX*>::iterator it = m_vVBuffers.begin();
			it != m_vVBuffers.end(); ++it)
		{
			if ((*it)->m_pVB)
			{
				(*it)->m_pVB->Release();
				(*it)->m_pVB = 0;
			}
		}

		for (std::vector<IndexBufferDX*>::iterator it = m_vIBuffers.begin();
			it != m_vIBuffers.end(); ++it)
		{
			if ((*it)->m_pIB)
			{
				(*it)->m_pIB->Release();
				(*it)->m_pIB = 0;
			}
		}

	} // OnLost

	//-----------------------------------------------------------
	// Recreate resources that are released in OnLost() function
	//-----------------------------------------------------------
	void RendererDX::OnReset()
	{
		SendMessage(m_d3dpp.hDeviceWindow, KGE_DEVICE_RESET, 0, 0);

		// Recreate render able textures
		for (std::vector<TextureDX9*>::iterator it = m_vTextures.begin();
			it != m_vTextures.end(); ++it)
		{
			(*it)->OnReset();
		}

		// Call effect manager OnReset function
		efx::EffectManager::GetSingletonPtr()->OnReset();

		HRESULT hr = S_OK;

		for (std::vector<VertexBufferDX*>::iterator it = m_vVBuffers.begin();
			it != m_vVBuffers.end(); ++it)
		{
			hr = m_pd3dDevice->CreateVertexBuffer((*it)->GetStride() * (*it)->GetDataCount(),
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &((*it)->m_pVB), 0);

			if (FAILED(hr))
			{
				io::Logger::Log("ERROR!!!", io::ELM_Error);
			}
		}

		for (std::vector<IndexBufferDX*>::iterator it = m_vIBuffers.begin();
			it != m_vIBuffers.end(); ++it)
		{
			hr = m_pd3dDevice->CreateIndexBuffer((*it)->GetStride() * (*it)->GetDataCount(),
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, (*it)->GetFormat(),
				D3DPOOL_DEFAULT, &((*it)->m_pIB), 0);

			if (FAILED(hr))
			{
				io::Logger::Log("ERROR!!!", io::ELM_Error);
			}
		}

		VertexShaderSet = MAXID;
		PixelShaderSet  = MAXID;
		m_nVertexDecID = MAXID;
		m_nIndexBufferID = MAXID;
		for (int i = 0; i < 8; i++)
		{
			m_nVertexBufferID[i] = MAXID;
			m_nVertexBufferID[i+8] = MAXID;
			m_nTextID[i] = MAXID;
		}

		// Get back buffer surface.
		hr = m_pd3dDevice->GetRenderTarget(0, &m_pBackBufferSurface);
		if (FAILED(hr))
			io::Logger::Log(io::ELM_Error, "Error getting back buffer surface");;

		hr = m_pd3dDevice->GetDepthStencilSurface(&m_pBackBufferZSurface);
		if (FAILED(hr))
			io::Logger::Log(io::ELM_Error, "Error getting main Z/Stencil buffer");

	} // OnReset

	//------------------------------------------------------------------------------------
	// Sets the stream source frequency for rendering instanced meshes
	//------------------------------------------------------------------------------------
	void RendererDX::SetStreamSourceFreq( u32 StreamID, u32 Count )
	{
		if (Count == MAXID)
			m_pd3dDevice->SetStreamSourceFreq(StreamID, D3DSTREAMSOURCE_INSTANCEDATA | 1);
		else if (Count == 1)
			m_pd3dDevice->SetStreamSourceFreq(StreamID, 1);
		else
		{
			m_pd3dDevice->SetStreamSourceFreq(StreamID, D3DSTREAMSOURCE_INDEXEDDATA | Count);
			m_iBatchCount = Count;
		}

	} // SetStreamSourceFreq

	//---------------------------------------------------
	// 
	//---------------------------------------------------
	void RendererDX::AddToTexturesList(TextureDX9* texture)
	{
		m_vTextures.push_back(texture);
	}

	//---------------------------------------------------
	// 
	//---------------------------------------------------
	void RendererDX::RemoveFromTexturesList(TextureDX9* texture)
	{
		const size_t m_vTexturesSize = m_vTextures.size();
		for (size_t i = 0; i < m_vTexturesSize; ++i)
		{
			if (m_vTextures[i] == texture)
			{
				m_vTextures[i] = m_vTextures[m_vTexturesSize - 1];
				m_vTextures.pop_back();
				return;
			}
		}
	}

	//------------------------------------------------------------------------------------
	// Sets the depth bias
	//------------------------------------------------------------------------------------
	void RendererDX::SetDepthBias( float fBias )
	{
		m_pd3dDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fBias);

	} // SetDepthBias

	//------------------------------------------------------------------------------------
	// Sets the depth bias slope
	//------------------------------------------------------------------------------------
	void RendererDX::SetDepthBiasSlope( float fSlope )
	{
		m_pd3dDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&fSlope);

	} // SetDepthBiasSlope	

	//------------------------------------------------------------------------------------
	// Sets the culling mode
	//------------------------------------------------------------------------------------
	void RendererDX::SetCullingMode( CullMode cm )
	{
		switch (cm)
		{
		case ECM_CCW:
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			break;

		case ECM_CW:
			m_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
			break;

		}

	} // SetCullingMode

	//------------------------------------------------------------------------------------
	// Sets the depth test mode.
	//------------------------------------------------------------------------------------
	void RendererDX::SetDepthMode( DepthTestMode e )
	{
		D3DCMPFUNC mode;
		switch (e)
		{
		case EDTM_LessEqual:
			mode = D3DCMP_LESSEQUAL;
			break;

		case EDTM_Greater:
			mode = D3DCMP_GREATER;
			break;
		}

		m_pd3dDevice->SetRenderState(D3DRS_ZFUNC, mode);
	}

	//------------------------------------------------------------------------------------
	// Sets the scissor properties
	//------------------------------------------------------------------------------------
	void RendererDX::SetScissorRegion( int x, int y, int width, int height )
	{
		RECT scissor_rect;
		scissor_rect.left = x;
		scissor_rect.right = x + width;
		scissor_rect.top = y;
		scissor_rect.bottom = y + height;

		m_pd3dDevice->SetScissorRect(&scissor_rect);

	} // SetScissorRegion

	//------------------------------------------------------------------------------------
	// Creates a texture from an Image
	//------------------------------------------------------------------------------------
	Texture* RendererDX::CreateTexture( ImageData* pImg )
	{
		Texture* pTex = KGE_NEW(TextureDX9)(m_pd3dDevice, this, pImg);

		return pTex;

	} // CreateTexture

	} // gfx

} // kge
