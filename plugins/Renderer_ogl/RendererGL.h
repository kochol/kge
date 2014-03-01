// Filename:RendererGL.h
// Des:		OpenGL Renderer
// Date:	aug-03-2010
// author:	Pooya Shahinfar, Ali Akbar Mohammadi

#ifndef KGE_RENDERERGL_H
#define KGE_RENDERERGL_H

#include "../../Include/kgedef.h"

#include "../../Include/gfx/Renderer.h"

#include <Cg/cg.h>    /* Can't include this?  Is Cg Toolkit installed! */
#include <Cg/cgGL.h>

#include <SDL.h>

namespace kge
{
namespace gfx
{
class RendererGL : public  Renderer
{
	friend class LightGL;

public:
	//! Constructor
	RendererGL();

	//! Destructor
	~RendererGL();

	// Init
	bool Init(Device* device, InitParameters &params);

	//! Clears specified buffers
	bool Clear(bool Pixel, bool Depth, bool Stencil);

	//! Start Rendering the scene
	virtual bool BeginRendering(bool ClearPixel, bool ClearZbuffer, bool ClearStencil);

	//! Creates a renderable texture for using it in RTT (Render To Texture)
	virtual bool CreateRenderableTexture(Texture** ppOutTexture, int width, int height, TextureFormat tf
										, bool CreateDepthStencilBuffer = true,bool AntiAliasing = false);

	//! End the Rendering
	virtual void EndRendering();

	//! Sets Clearing color (of Color buffer)
	virtual void SetClearColor(const Colorf& ClearColor);

	//! Sets the culling mode
	void SetCullingMode(CullMode cm);

	//! Sets the vertex buffer for multi streaming
	virtual void SetVertexBuffer(HardwareBuffer* pBuffer, int stage);

	//! Sets the index buffer for rendering
	virtual void SetIndexBuffer(HardwareBuffer* pBuffer);

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

	//! Matrix haye asli dorbin va ajsam ra taghir midahad
	virtual void SetTransForm(math::Matrix *mat, TransformMode TM);

	//! Returns specified matrix
	virtual math::Matrix GetTransForm(TransformMode TM = ETM_World);

	//! This function is for Texture Manager for adding textures use SceneManager AddTexture function instead.
	virtual bool AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
						const char* Name, void* ExtraParam);

	// Set a texture
	void SetTexture(Texture* pTex, int index = 0);

	// Enable kardane khsiyat haye render.
	void Enable(RenderFlags RF);

	//! Disables specified renderer state
	virtual void Disable ( RenderFlags RF);
	
	//! Sets material
	virtual void SetMaterial(Material *pMaterial);

	//! Sets texture params
	virtual void SetTextureParams(TextureParams Params, int TextureStage = 0);



	//! Check for graphics card capabilities
	virtual bool CanDo ( GraphicCardCaps Item );
										

	//! Srtup stencil buffer for diffrent effects
	virtual void SetStencilBuffer( kge::efx::EffectType ET, u8 State );
	
	//! Create font base on API type.
	virtual void AddFont( kge::gui::Font** ppOut, 
					 const char* strFontName, ul32 FontSize);
					 
	//! Create a vertex buffer (VBO) and store it on video memory
	virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
									   VertexType eVType = EVT_V3TN, 
									   bool isDynamic = false);

	// Create a vertex buffer from custom vertex type and store it on video memory.
	virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
									   u32   Stride,
									   bool  isDynamic = false);

	//! Creates an Index buffer (IBO) on video memory
	virtual HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
										IndexBufferType eIndexBufferType = EIBT_16Bit,
										bool isDynamic = false);
	//! Draw a vertex buffer
	virtual void DrawTriangleList(HardwareBuffer* VB, HardwareBuffer* IB, u32 VCount, u32 ICount,
						  VertexType eVType = EVT_V3TN );

	/*! Draw a vertex buffer you must set the buffers manually before calling this function this function is good for multistreaming
		\param VCount The Vertices count.
		\param ICount The Indices count.
		\param CV custom vertex pointer
	*/
	virtual void DrawTriangleList(u32 VCount, u32 ICount,
		VertexDec* CV, u32 VertexStart, u32 StartIndex);

	/*! Load a vertex shader from a file then compile it and return the
		created vertex shader.
		\param VertexFileName The file name that contain the vertex shader source code.
		\param VertexMain The main vertex shader function name.
		\param eVVersion The vertex shader version that the code must be compiled in that version.
		\return Returns the created vertex shader.
	*/
	virtual Shader* CreateVertexShaderFromFile(const char* VertexFileName, const char* VertexMain = "VSMain", 
								 ShaderVersion eVVersion = ESV_VS1_1);


	/*! Load a vertex shader from a string then compile it and return the
		created vertex shader.
		\param VertexCode The string that contain the vertex shader source code.
		\param VertexMain The main vertex shader function name.
		\param eVVersion The vertex shader version that the code must be compiled in that version.
		\param PixelCode The string that contain the pixel shader source code.
		\param PixelMain The main pixel shader function name.
		\param ePVersion The pixel shader version that the code must be compiled in that version.
		\return Returns the created vertex shader.
	*/
	virtual Shader* CreateVertexShaderFromString(const char* VertexCode, const char* VertexMain = "VSMain", 
								   ShaderVersion eVVersion = ESV_VS1_1);

	/*! Load a pixel shader from a file then compile it and return the
		created pixel shader.
		\param PixelFileName The file name that contain the pixel shader source code.
		\param PixelMain The main pixel shader function name.
		\param ePVersion The pixel shader version that the code must be compiled in that version.
		\return Returns the created pixel shader.
	*/
	virtual Shader* CreatePixelShaderFromFile(const char* PixelFileName, const char* PixelMain = "PSMain",
								   ShaderVersion ePVersion = ESV_PS1_1);

	/*! Load a pixel shader from a string then compile it and return the
		created pixel shader.
		\param PixelCode The string that contain the pixel shader source code.
		\param PixelMain The main pixel shader function name.
		\param ePVersion The pixel shader version that the code must be compiled in that version.
		\return Returns the created pixel shader.
	*/
	virtual Shader* CreatePixelShaderFromString(const char* PixelCode, const char* PixelMain = "PSMain",
										ShaderVersion ePVersion = ESV_PS1_1);

	//!
	virtual bool AddVertexShader(Shader** ppOutShader, u32 handle, const char* fileName,
	const char* mainName, void* extraParam);

	//!
	virtual bool AddPixelShader(Shader** ppOutShader, u32 handle, const char* fileName,
	const char* mainName, void* extraParam);

	//! Returns the renderer type (DirectX or OpenGL)
	virtual RendererAPI GetRendererType() { return ERA_OpenGL; }

	// set the fog setings
	virtual bool SetFog(FogType mode,Colorf FogColor ,float expDensity=0,float linearStart= 0,float linearEnd = 1000000);

	//! Sets video mode
	virtual bool SetVideoMode(int Width, int Height , int Bits, 
		  bool UseStencil = false, bool Fullscreen = false,int MultiSample = 0,bool VSync = false);

	//! Sets the rendering view port.
	virtual void SetViewPort(const core::RectI& viewport); 

	//! Returns the direct 3d device.
	void* GetDirect3dDevice() {return NULL;}

	//! Create a custom vertex type and put it in pOut parameter.
	virtual VertexDec* CreateVertexDeclaration(CustomVertexElement *pVertexInfoArray, core::stringc sName);

	//! Sets the vertex declaration 
	virtual void SetVertexDec(VertexDec* pVD);

	//! Set the second rendering window
	virtual void SetRenderingWindow(void* hwnd);

	//! Resize the view port and rendering
	virtual void OnResize(int width, int height);

	//! Change resolution, toggle full screen, turn on/off vertical sync
	void OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync);

	//! Sets the stream source frequency for rendering instanced meshes
	void SetStreamSourceFreq(u32 StreamID, u32 Count) {}

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

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	HDC					m_hdc;
	HGLRC				m_hrc;

#endif // KGE_PLATFORM == KGE_PLATFORM_WINDOWS

	SDL_Window		*	m_pWindow;
	SDL_GLContext		m_pContext;

	// C a m e r a   M a t r i x e s
	math::Matrix		m_mProj,		// Projection Matrix.
						m_mView,		// View Matrix.
						m_mWorld,		// World Matrix.
						m_mViewProj,
						m_m2D;			// 2D Projection Matrix.

	// Index buffer info
	int					m_iIndexBufferType,
						m_iIndexBufferSize;

	// Vertex elements array
	std::vector<std::vector<CustomVertexElement> >
						m_vVertexInfoArray;

	VertexDec		*	m_pCurrentVD;

	// CG
	CGcontext			m_CgContext;
	CGprofile			m_CgVertexProfile,
						m_CgFragmentProfile;

	// bad az Init ba seda zadan in tabe maghadir avalie engine set mishan.
	virtual bool AfterInit();

	// Check for devil errors.
	void CheckDevilErrors( const char* TextureName );

	// Check for CG errors
	void checkForCgError( const char* situtaion, const char* ShaderName = NULL );

	// Precalculate the graphics card capacities.
	virtual void InitGCC();

} ; // RendererGL

} // gfx

} // kge

#endif // KGE_RENDERERGL_H
