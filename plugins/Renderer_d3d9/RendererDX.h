// File name: RendererDX.h
// Des: in file interface asli kar ba ghesmat Renderer ba D3D hast. Bayad tamam saye
//      khodemono bokonim ta az D3DX estefade nakonim.
// Date: 13/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef WIN32

#ifndef RENDERERDX_H
#define RENDERERDX_H

#if defined(DEBUG) || defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif

#include "../../include/gfx/renderer.h"
#include "../../include/kgedef.h"
#include "../../include/enums.h"
#include "LightDX.h"
#include <d3d9.h>
#include <vector>

#define RGB16BIT(r,g,b) ((b) + ((g) << 5) + ((r) << 11)) // change 24 bit to 16 bit
#define RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

namespace kge
{
namespace gfx
{

	class TextureDX9;
	class VertexBufferDX;
	class IndexBufferDX;

	static const char* ShaderVersionName[] = {
		"vs_1_1",
		"vs_2_0",
		"vs_3_0",
		"ps_1_1",
		"ps_1_4",
		"ps_2_0",
		"ps_3_0"
	};

class RendererDX: public Renderer
{
	friend class LightDX;

public:

	// Constructor
	RendererDX();

	// Destructor
	~RendererDX();

	// Init
	bool Init(Device* device, InitParameters &params);

	//! Clears specified buffers
	bool Clear(bool Pixel, bool Zbuffer, bool Stencil);

	// Creates a renderable texture to use it as a render target
	virtual bool CreateRenderableTexture(Texture** ppOutTexture, int width, int height, TextureFormat tf, bool CreateDepthStencilBuffer = true, bool AntiAliasing = false);

	//! Begin rendering
	bool BeginRendering(bool Pixel, bool Zbuffer, bool Stencil);

	// Payane render
	void EndRendering();

	// Set kardane Range pas zamine
	void SetClearColor(const Colorf& ClearColor);

	//! Sets the culling mode
	void SetCullingMode(CullMode cm);

	//! Sets the vertex buffer for multi streaming
	void SetVertexBuffer(HardwareBuffer* pBuffer, int stage);

	//! Sets the index buffer for rendering
	void SetIndexBuffer(HardwareBuffer* pBuffer);

	// Set the render able target where renderer will render there.
	void SetRenderTarget(int RenderNumber, Texture* RenderTarget);

	// Set kardane dorbin.
	void SetViewLookatLH(math::Vector &Pos, math::Vector &Lookat, math::Vector &UpDir);

	// Set kardane dorbin.
	void SetViewLookatRH(math::Vector &Pos, math::Vector &Lookat, math::Vector &UpDir);

	// Matrixe Projection ro set mikone Be sorate Chap Dast.
	void SetPerspectiveLH(float Fov, float Near, float Far);

	//! Create and sets the Perspective matrix for right-hand
	void SetPerspectiveRH(float Fov, float Near, float Far);

	// Matrix haye asli dorbin va ajsam ra taghir midahad
	void SetTransForm(math::Matrix *mat, TransformMode TM);

	//! Returns specified matrix
	math::Matrix GetTransForm(TransformMode TM);

	// This function is for Texture Manager for adding textures use SceneManager AddTexture function instead.
	bool AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
		const char* Name, void* ExtraParam);

	// Set a texture
	void SetTexture(Texture* pTex, int index);

	// Enable kardane khsiyat haye render.
	void Enable(RenderFlags RF);

	// Disable kardane khsiyat haye render.
	void Disable ( RenderFlags RF );


	// Set kardane kamele ya matrial besorate automatic.
	void SetMaterial(Material *pMaterial);

	// Set kardane keyfiyat texture ha va address mode ha.
	void SetTextureParams(TextureParams Params, int TextureStage = 0);

	//! Check for graphics card capabilities
	bool CanDo ( GraphicCardCaps Item );

	// Tanzimate stencil buffer baraye effect haye mokhtalef.
	void SetStencilBuffer(kge::efx::EffectType ET, u8 State);

	// Create font
	void AddFont( kge::gui::Font** ppOut, const char* strFontName, ul32 dwHeight );

	// Create a vertex buffer and store it on video memory.
	HardwareBuffer* CreateVertexBuffer( void* Vertices, u32 VCount, 
										VertexType eVType , 
										bool isDynamic);

	// Create a vertex buffer from custom vertex type and store it on video memory.
	HardwareBuffer* CreateVertexBuffer( void* Vertices, u32 VCount, 
										u32   Stride,
										bool  isDynamic);

	// Creates an Index buffer on video memory
	HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
										IndexBufferType eIndexBufferType ,
										bool isDynamic);

	// Draw a vertex buffer
	void DrawTriangleList(HardwareBuffer* VB, HardwareBuffer* IB, u32 VCount, u32 ICount,
						  VertexType eVType = EVT_V3TN );

	// Draw a vertex buffer
	void DrawTriangleList(u32 VCount, u32 ICount,
		VertexDec* CV, u32 VertexStart, u32 StartIndex);

	// Load a vertex shader from a file then compile it and return the created vertex shader
	virtual Shader* CreateVertexShaderFromFile(const char* VertexFileName, const char* VertexMain, 
		ShaderVersion eVVersion);

	// Load a vertex shader from a string then compile it and return the created vertex shader
	virtual Shader* CreateVertexShaderFromString(const char* VertexCode, const char* VertexMain, 
		ShaderVersion eVVersion);

	// Load a pixel shader from a file then compile it and return the created pixel shader
	virtual Shader* CreatePixelShaderFromFile(const char* PixelFileName, const char* PixelMain,
		ShaderVersion ePVersion);

	// Load a pixel shader from a string then compile it and return the created pixel shader
	virtual Shader* CreatePixelShaderFromString(const char* PixelCode, const char* PixelMain,
		ShaderVersion ePVersion);

	//!
	virtual bool AddVertexShader(Shader** ppOutShader, u32 handle, const char* fileName,
		const char* mainName, void* extraParam);

	//!
	virtual bool AddPixelShader(Shader** ppOutShader, u32 handle, const char* fileName,
		const char* mainName, void* extraParam);

	// Returns the renderer type (DirectX or OpenGL)
	virtual RendererAPI GetRendererType() { return ERA_DirectX9; }

	// set the fog setings
	bool SetFog(FogType mode,Colorf FogColor ,float expDensity=0,float linearStart= 0,float linearEnd = 1000000);

	//set video mode
	bool SetVideoMode( int Width, int Height , int Bits, 
			  bool UseStencil = false, bool Fullscreen = false,int MultiSample = 0,bool VSync = false);

	// Sets the rendering view port.
	virtual void SetViewPort(const core::RectI& viewport); 

	//! Returns the direct 3d device.
	void* GetDirect3dDevice();

	//! Create a custom vertex type and put it in pOut parameter.
	VertexDec* CreateVertexDeclaration(CustomVertexElement *pVertexInfoArray, core::stringc sName);

	//! Sets the vertex declaration 
	void SetVertexDec(VertexDec* pVD);

	//! Set the second rendering window
	void SetRenderingWindow(void* hwnd);

	//! Resize the view port and rendering
	void OnResize(int width, int height);

	//! Change resolution, toggle full screen, turn on/off vertical sync
	void OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync);

	//! Sets the stream source frequency for rendering instanced meshes
	void SetStreamSourceFreq(u32 StreamID, u32 Count);

	//! 
	void AddToTexturesList(TextureDX9* texture);

	//! Removes TextureDX9 from internal list.
	void RemoveFromTexturesList(TextureDX9* texture);

	//! Removes VertexBufferDX from internal list.
	void RemoveVertexBuffer(VertexBufferDX* vb);

	//! Removes IndexBufferDX from internal list.
	void RemoveIndexBuffer(IndexBufferDX* ib);

	//! Sets the depth bias
	void SetDepthBias(float fBias);

	//! Sets the depth bias slope
	void SetDepthBiasSlope(float fSlope);

	//! Sets the depth test mode.
	void SetDepthMode(DepthTestMode e);

	//! Sets the scissor properties
	void SetScissorRegion(int x, int y, int width, int height);

	//! Creates a texture from an Image
	Texture* CreateTexture(ImageData* pImg);

protected:
	LPDIRECT3DDEVICE9						m_pd3dDevice;
	IDirect3DSwapChain9*					m_pSwapChain;
	D3DPRESENT_PARAMETERS					m_d3dpp;
	D3DFORMAT								m_DSFMT;		// depth and Stencil Buffer
	D3DMULTISAMPLE_TYPE						m_msType;		// Multi sample type
	kge::ul32								msQuality;
	D3DCAPS9								m_d3dCaps;		// D3D Caps
	LPDIRECT3DSURFACE9						m_pBackBufferSurface, // Back buffer surface
											m_pBackBufferZSurface;
	IDirect3DVertexDeclaration9     *		m_pV2,		    // Vertex2.
									*		m_pV3,		    // Vertex3.
									*		m_pV3C,		    // Vertex3 + Color.
									*		m_pV3T,	    	// Vertex3 + Texture cord.
									*       m_pV3CT,        // Vertex3 + Color + Texcoord.
									*		m_pV3TN,		// Vertex3 + Texture cord + Normal vector.
									*		m_pV3TTN,		// Vertex3 + two Texture cord + Normal vector.
									*		m_pV3TNC,		// Vertex3 + Texture cord + Normal vector + Color.
									*		m_pP0NT1BT2,	// Vertex3 + Normal vector + Texture cord + Tangent + Binormal in multi stream mode.
									*		m_pP0NTC1BT2,	// Vertex3 + Normal vector + Texture cord + color + Tangent + Binormal in multi stream mode.
									*		m_pPJ0NT1BT2;	// Vertex3 + Joint index for hardware skinning + Normal vector + Texture coord + Tangent + Binormal in multi stream mode.

	kge::ul32								m_Behavior;		// Device Behavior
	D3DCOLOR								m_ClearColor;	// Clear Color
	kge::u32								m_TextCount;	// The count of textures
	std::vector<char*>						m_vTextureNames;// Texture names Array.
	std::vector<TextureDX9*>			    m_vTextures;	// Textures Array.
	std::vector<VertexBufferDX*>	        m_vVBuffers;	// Vertex buffers array.
	std::vector<IndexBufferDX*>		        m_vIBuffers;	// Index buffers array.
	bool									m_bDeviceLost;	// Device Lost.
	int										m_iAASurface;
	bool									m_bRTT;			// Render to texture flag
	Texture*								m_pAATexture;


	// C a m e r a   M a t r i x e s
	D3DMATRIX m_mProj,		// Projection Matrix.
			  m_mView,		// View Matrix.
			  m_mWorld;     // World matrix.
	math::Matrix  m_m2D;		// 2D Projection Matrix.

	math::Matrix	m_mViewProj;	// View Projection.


	void CheckStencil(bool Antialiasing);
	bool CheckStencilFMT();
	bool CheckDevice(D3DCAPS9 *pCaps, DWORD dwBehavior);

	// bad az Init ba seda zadan in tabe maghadir avalie engine set mishan.
	bool AfterInit();

	// Check for devil errors.
	void CheckDevilErrors( const char* TextureName );

	// Precalculate the graphics card capacities.
	virtual void InitGCC();

	// Reset the direct3d device.
	virtual bool ResetDevice();

	//! Release all resources created with D3DPOOL_DEFAULT
	void OnLost();

	//! Recreate resources that are released in OnLost() function
	void OnReset();

}; // RendererDX

} // gfx

} // kge

#endif // RENDERERDX_H

#endif // WIN32