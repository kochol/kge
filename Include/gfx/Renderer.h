// File name: Renderer.h
// Des: This is the main interface for working with KGE Renderer
// Date: 13/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef RENDERER_H
#define RENDERER_H

#include "Material.h"
#include "../Device.h"
#include "Lighting.h"
#include "Shader.h"
#include "HardwareBuffer.h"
#include "../io/Iwindow.h"
#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../kgedef.h"
#include "../core/Structs.h"
#include "Structs.h"
#include "../core/Timer.h"
#include "../gui/Font.h"
#include "VertexDec.h"
#include "VertexElement.h"

namespace kge
{
namespace gui
{
	class Image;
}

namespace sn
{
	class Camera;
}
namespace gfx
{
	class ImageData;

//! Noe matrixi ke mikhahid taghir bedid.
enum TransformMode
{
	ETM_World,				//! World matrix
	ETM_View,				//! View matrix
	ETM_Projection,			//! Projection matrix
	ETM_ViewProjection
}; // TransformMode

//! Khasiat haye engine ke baraye render estefade mishe.
enum RenderFlags
{
	ERF_2D = 0,					//! Dorbine 2 bodi.
	ERF_Lighting,				//! Nor Pardazi.
	ERF_AlphaBlending,          //! Alpha blending.
	ERF_AdditiveBlending,       //! Additive blending.
	ERF_AlphaTest,
	ERF_StencilBuffer,			//! Stencil buffer for shadows and mirrors.
	ERF_DepthBuffer,			//! Z buffer
	ERF_ZWrite,                 //! Writing to the depth buffer.
	ERF_VSync,					//! VSync
	ERF_WireFrame,				//! Wire frame view
	ERF_Culling,                //! Culling
	ERF_ColorRendering,			//!< Rendering to color buffer
	ERF_Scissor,				//!< Scissor
	ERF_Count					//! This is only for counting.
};

//! Anvae tanzim ha baraye Texture ha.
enum TextureParams
{
	ETP_Point = 0,				//! Kamtarin Keifiyat bishtarin Sorat (Filtering).
	ETP_Linear,					//! Keifiyate monaseb va sorate monesab baraye axar computer haye emroozi (Filtering).
	ETP_Anisotropic,			//! Behtarin keyfiyat kamtarin sorat (Filtering).
	ETP_Wrap,					//! Texture baghale ham namayesh miyaband (Addressing).
	ETP_BorderColor,			//! Border color (Addressing).
	ETP_Clamp,					//! Clamp Mode (Addressing).
	ETP_Mirror					//! Mirror mode (Addressing).
};

//! Ghabeliat haye cart graphic.
enum GraphicCardCaps
{
	EGCC_VertexShader1_1,		/**< Vertex Shader 1.1 */
	EGCC_FragmentShader1_1,		/**< Fragment/Pixel Shader 1.1 */
	EGCC_FragmentShader1_4,		/**< Fragment/Pixel Shader 1.4 */
	EGCC_VertexShader2,			/**< Vertex Shader 2 */
	EGCC_FragmentShader2,		/**< Fragment/Pixel Shader 2 */
	EGCC_VertexShader3,			/**< Vertex Shader 3 */
	EGCC_FragmentShader3,		/**< Fragment/Pixel Shader 3 */
	EGCC_ShaderObject,			/**< Shader object */
	EGCC_GeometryShader4,		/**< Geometry shaders 4 */
	EGCC_ShaderModel4,			/**< Shader model 4 */
	EGCC_ShaderModel3,			/**< Shader model 3 */
	EGCC_ShaderModel2,			/**< Shader model 2 */
	EGCC_OpenGL2,				/**< OpenGL version 2.x */
	EGCC_OpenGL3,				/**< OpenGL version 3.x */
	EGCC_OpenGL4,				/**< OpenGL version 4.x */
	EGCC_DXT1,					/**< Can device support DXT1 compression texture type. */
	EGCC_DXT3,					/**< Can device support DXT3 compression texture type. */
	EGCC_DXT5,					/**< Can device support DXT5 compression texture type. */
	EGCC_INTZ,					/**< INTZ allows a 24-bit depth buffer previously used for rendering to be bound as a texture */

	EGCC_Count					/**< Don't use it */
};

//! Shader versions.
enum ShaderVersion
{
	ESV_VS1_1,	/**< Vertex Shader 1.1 */
	ESV_VS2,	/**< Vertex Shader 2 */
	ESV_VS3,	/**< Vertex Shader 3 */
	ESV_PS1_1,	/**< Pixel Shader 1.1 */
	ESV_PS1_4,	/**< Pixel Shader 1.4 */
	ESV_PS2,	/**< Pixel Shader 2 */
	ESV_PS3		/**< Pixel Shader 3 */

}; // ShaderVersion

enum CullMode
{
	ECM_None,
	ECM_CCW,
	ECM_CW
};

enum RendererPipeline
{
	ERP_Forward,
	ERP_Deferred
};

enum DepthTestMode
{
	EDTM_LessEqual,
	EDTM_Greater
};

//! standard callback function
typedef void (*kgeRenderCallback)(KgeUnknown* Sender);

//! The Renderer interface has functions to work with graphics card
class KGE_API Renderer: public KgeUnknown
{
public:
	//! Constructor
	Renderer();
	
	//! Destructor
	virtual ~Renderer(); 

	//! Returns the Renderer class pointer.
	static Renderer* GetSingletonPtr();

	//! set callback functions for rendering device
	virtual void SetCallbacks(kgeRenderCallback clbk_OnLost, kgeRenderCallback clbk_OnReset);
	
	//! Amade kardane Renderer.
	virtual bool Init(Device* device, InitParameters &params) = 0;

	//! Pak kardane pixel ha va buffer ha.
	virtual bool Clear(bool Pixel, bool Zbuffer, bool Stencil)=0;

	//! Shroe be render gereftan.
	virtual bool BeginRendering(bool ClearPixel, bool ClearZbuffer, bool 
								ClearStencil)=0;

	/*! Creates a renderable texture to use it as a render target
		\param width The texture width
		\param height The texture height
		\param tf Texture color format
	 */
	virtual bool CreateRenderableTexture(Texture** ppOutTexture, int width, int height, TextureFormat tf
		, bool CreateDepthStencilBuffer = true, bool AntiAliasing = false) = 0;

	//! End the Rendering
	virtual void EndRendering()=0;

	//! Set kardane Range pas zamine
	virtual void SetClearColor(const Colorf& ClearColor)=0;

	//! Sets the culling mode
	virtual void SetCullingMode(CullMode cm) = 0;

	//! Sets the vertex buffer for multi streaming
	virtual void SetVertexBuffer(HardwareBuffer* pBuffer, int stage = 0) = 0;

	//! Sets the index buffer for rendering
	virtual void SetIndexBuffer(HardwareBuffer* pBuffer) = 0;

	/*! Set the renderable target where renderer will render there.
		\param RenderNumber The target index you can use Multi Render Targets(MRT) with shaders.
		\param RenderTarget A pointer to the texture that you want to render there pass NULL to render to screen.
	 */
	virtual void SetRenderTarget(int RenderNumber = 0, Texture* RenderTarget = NULL) = 0;

	/*! Sets camera for left-hand
	  \param Position Jaee ke dorbin gharar migirad.
	  \param Lookat Jaee ke dorbin be anja negah mikonad.
	  \param UpDirection Bordari ke jahat bala ra moshakas mikonad.
	  \return Agar movafagh bood meghdare true ra barmigardone.
	 */
	virtual void SetViewLookatLH(math::Vector &Position,
								 math::Vector &Lookat , 
								 math::Vector &UpDirection)=0;

	/*! Sets camera for right hand
	  \param Position Jaee ke dorbin gharar migirad.
	  \param Lookat Jaee ke dorbin be anja negah mikonad.
	  \param UpDirection Bordari ke jahat bala ra moshakas mikonad.
	  \return Agar movafagh bood meghdare true ra barmigardone.
	 */
	virtual void SetViewLookatRH(math::Vector &Position,
								 math::Vector &Lookat, 
								 math::Vector &UpDirection)=0;

	//! Matrixe Projection ro set mikone Be sorate Chap Dast.
	/*!
	  \param Fov Zavie dorbin.
	  \param NearPlane Parde joloyee.
	  \param FarPlane Parde aghabi.
	 */
	virtual void SetPerspectiveLH(float Fov=0.7853f, float NearPlane=1.0f, 
								  float FarPlane=1000.0f)=0;

	//! Matrixe Projection ro set mikone Be sorate Rast Dast.
	/*!
	  \param Fov Zavie dorbin.
	  \param NearPlane Parde joloyee.
	  \param FarPlane Parde aghabi.
	 */
	virtual void SetPerspectiveRH(float Fov=0.8f, float NearPlane=1.0f, 
								  float FarPlane=1000.0f)=0;

	//! Matrix haye asli dorbin va ajsam ra taghir midahad
	/*!
	  \param mat Matrix taghir dahande.
	 */
	virtual void SetTransForm(math::Matrix *mat, TransformMode TM = ETM_World)=0;

	//! Matrix haye asli dorbin va ajsam ra bar migardone.
	/*!
	  \param mat Matrix taghir dahande.
	 */
	virtual math::Matrix GetTransForm(TransformMode TM = ETM_World)=0;

	//! Meghdar Frames Per Second ro bar migardone.
	int GetFPS() { return m_iFPS; }

	//! This function is for Texture Manager for adding textures use SceneManager AddTexture function instead.
	virtual bool AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
							const char* Name, void* ExtraParam) = 0;

	/*! Set a texture
	 */
	virtual void SetTexture(Texture* pTex, int index = 0)=0;

	/*! Enable kardane khsiyat haye render.
		\param RF Parchami ke mikhahid enable shavad.
	*/
	virtual void Enable(RenderFlags RF)=0;

	/*! Disable kardane khsiyat haye render.
		\param RF Parchami ke mikhahid Disable shavad.
	*/
	virtual void Disable ( RenderFlags RF ) = 0;

	//! Tamame kar ba norha dar in pointer gharar darad.
	Lighting *Lights;

	/*! Set kardane kamele ya matrial besorate automatic.
		\param pMaterial Materiali ke bayad set shavad ghabl az render.
	*/
	virtual void SetMaterial(Material *pMaterial) = 0;

	/*! Set kardane keyfiyat texture ha va address mode ha.
		\param Params Noe keyfiyat texture va address mode ha.
	*/
	virtual void SetTextureParams(TextureParams Params, int TextureStage = 0) = 0;

	/*! Check for graphics card capabilities
		\param Item Ghabeliyati ke mikhahid check shavad.
		\return agar poshtibani shavad true barmigardanad.
	*/
	virtual bool CanDo ( GraphicCardCaps Item ) = 0;

	/*! Tanzimate stencil buffer baraye effect haye mokhtalef.
	*/
	virtual void SetStencilBuffer( kge::efx::EffectType ET, u8 State ) = 0;

	//! Create font base on API type.
	virtual void AddFont( kge::gui::Font** ppOut, 
							 const char* strFontName, ul32 FontSize) = 0;

	/*! Create a vertex buffer and store it on video memory.
		\param Vertices A pointer to the Vertices array.
		\param VCount The Vertices count.
		\param eVType The vertex type default value = EVT_V3TN witch is a vertex with texture and normal vector.
		\return Returns the created vertex buffer.
	*/
	virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
											   VertexType eVType = EVT_V3TN, 
											   bool isDynamic = false) = 0;

	/*! Create a vertex buffer from custom vertex type and store it on video memory.
		\param Vertices A pointer to the Vertices array.
		\param VCount The Vertices count.
		\param Stride The vertex struct size in bytes.
		\return Returns the created vertex buffer.
	*/
	virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
											   u32   Stride,
											   bool  isDynamic = false) = 0;

	//! Creates an Index buffer on video memory
	virtual HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
												IndexBufferType eIndexBufferType = EIBT_16Bit,
												bool isDynamic = false) = 0;

	/*! Draw a vertex buffer
		\param VBid Vertex buffer index.
		\param VCount The Vertices count.
		\param ICount The Indices couont.
		\param eVType The vertex type default value = EVT_V3TN witch is a vertex with texture and normal vector.
	*/
	virtual void DrawTriangleList(HardwareBuffer* VB, HardwareBuffer* IB, u32 VCount, u32 ICount,
								  VertexType eVType = EVT_V3TN ) = 0;

	/*! Draw a vertex buffer you must set the buffers manually before calling this function this function is good for multi streaming
		\param VCount The Vertices count.
		\param ICount The Indices count.
		\param CV custom vertex pointer
	*/
	virtual void DrawTriangleList(u32 VCount, u32 ICount,
		VertexDec* CV, u32 VertexStart, u32 StartIndex) = 0;

	/*! Load a vertex shader from a file then compile it and return the
		created vertex shader.
		\param VertexFileName The file name that contain the vertex shader source code.
		\param VertexMain The main vertex shader function name.
		\param eVVersion The vertex shader version that the code must be compiled in that version.
		\return Returns the created vertex shader.
	*/
	virtual Shader* CreateVertexShaderFromFile(const char* VertexFileName, const char* VertexMain = "VSMain", 
										 ShaderVersion eVVersion = ESV_VS1_1) = 0;
								  
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
										   ShaderVersion eVVersion = ESV_VS1_1) = 0;

	/*! Load a pixel shader from a file then compile it and return the
	created pixel shader.
	\param PixelFileName The file name that contain the pixel shader source code.
	\param PixelMain The main pixel shader function name.
	\param ePVersion The pixel shader version that the code must be compiled in that version.
	\return Returns the created pixel shader.
	*/
	virtual Shader* CreatePixelShaderFromFile(const char* PixelFileName, const char* PixelMain = "PSMain",
										   ShaderVersion ePVersion = ESV_PS1_1) = 0;

	/*! Load a pixel shader from a string then compile it and return the
	created pixel shader.
	\param PixelCode The string that contain the pixel shader source code.
	\param PixelMain The main pixel shader function name.
	\param ePVersion The pixel shader version that the code must be compiled in that version.
	\return Returns the created pixel shader.
	*/
	virtual Shader* CreatePixelShaderFromString(const char* PixelCode, const char* PixelMain = "PSMain",
												ShaderVersion ePVersion = ESV_PS1_1) = 0;

	/*! Creates a texture from an ImageData
		\note If you use this function manually you must add your texture to texture manager to avoid bugs.
	*/
	virtual Texture* CreateTexture(ImageData* pImg) = 0;

	//!
	virtual bool AddVertexShader(Shader** ppOutShader, u32 handle, const char* fileName,
		const char* mainName, void* extraParam) = 0;

	//!
	virtual bool AddPixelShader(Shader** ppOutShader, u32 handle, const char* fileName,
		const char* mainName, void* extraParam) = 0;

	//! Returns the last material used to render.
	virtual Material* GetLastMaterial() { return m_pLastMaterial; }

	//! Returns the renderer type (DirectX or OpenGL)
	virtual RendererAPI GetRendererType() = 0;

	// set the fog setings
	virtual bool SetFog(FogType mode,Colorf FogColor ,float expDensity=0,float linearStart= 0,float linearEnd = 1000000) = 0;

	// set video mode
	virtual bool SetVideoMode(int Width, int Height , int Bits, 
			  bool UseStencil = false, bool Fullscreen = false,int MultiSample = 0,bool VSync = false) = 0;

	//! returns the number of triangle rendered.
	virtual u32 GetTriangleCount() { return m_iTriCount; }

	//! returns the number of draw functions called.
	virtual u32 GetDrawCallsCount() { return m_iDrawCount; }

	//! For internal use.
	virtual void SetMainSceneManager(sn::SceneManager* smgr) { m_pSnmgr = smgr; } 

	//! Sets the rendering view port.
	virtual void SetViewPort(const core::RectI& viewport) = 0; 

	virtual bool GetRenderFlag(RenderFlags r) { return m_bRF[(int)r]; }

	//! Returns the direct 3d device.
	virtual void* GetDirect3dDevice() { return NULL; }

	//! Returns the width of the rendering area.
	virtual int GetWindowWidth() { return m_Params.Width; }

	//! Returns the width of the rendering area.
	virtual int GetWindowHeight() { return m_Params.Height; }

	//! Create a custom vertex declaration and returns its pointer
	virtual VertexDec* CreateVertexDeclaration(CustomVertexElement *pVertexInfoArray, core::stringc sName) = 0;

	//! Sets the vertex declaration 
	virtual void SetVertexDec(VertexDec* pVD) = 0;

	//! Returns the default vertex declaration.
	VertexDec* GetVertexDec(VertexType evt) { return m_ppVertexDecs[evt]; }

	//! Set the second rendering window
	virtual void SetRenderingWindow(void* hwnd) = 0;

	//! Resize the view port and rendering
	virtual void OnResize(int width, int height) = 0;

	//! Change resolution, toggle full screen, turn on/off vertical sync
	virtual void OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync) = 0;

	//! Sets the stream source frequency for rendering instanced meshes
	virtual void SetStreamSourceFreq(u32 StreamID, u32 Count) = 0;

	void SetMaxTextureSize(u32 maxTextureSize);
	u32  GetMaxTextureSize() const;

	//! Sets the depth bias
	virtual void SetDepthBias(float fBias) = 0;

	//! Sets the depth bias slope
	virtual void SetDepthBiasSlope(float fSlope) = 0;

	//! Sets the renderer pipeline
	virtual void SetRendererPipeline(RendererPipeline rp);

	//! Gets the current renderer pipeline
	virtual RendererPipeline GetRendererPipeline() { return m_eRendererPipeline; }

	//! Returns the position map
	virtual Texture* GetPositionMap() {return m_pPosMap;}

	//! Returns the normal map
	virtual Texture* GetNormalMap() {return m_pNorMap;}

	//! Returns the Diffuse map
	virtual Texture* GetDiffuseMap() {return m_pDifMap;}

	//! Draw a screen quad on screen. Note the 2D mode must be enabled before this action
	virtual void DrawScreenQuad();

	//! Sets the camera far plane as screen quad vectors.
	virtual void SetFarPlaneAsScreenQuad(sn::Camera* pCam);

	//! Sets the depth test mode.
	virtual void SetDepthMode(DepthTestMode e = EDTM_LessEqual) = 0;

	//! Sets the scissor properties
	virtual void SetScissorRegion(int x, int y, int width, int height) {}

	//! For internal use
	void SetVertexBufferID(u32 id, int index) {m_nVertexBufferID[index] = id;}

	//! For internal use
	void SetIndexBufferID(u32 id) {m_nIndexBufferID = id;}

protected:
	Device			* m_pDevice;
	sn::SceneManager* m_pSnmgr;
	InitParameters	  m_Params;
	Colorf			  m_cClearColor;				// Clear Color
	bool			  m_bIsSceneRunning;			// Vaghty ke barname dar hale render hast.
	bool			  m_bUseShaders;				// aya bayad az shader ha estefade beshe ya na
	core::Timer		* m_pTimer;						// Baraye mohasebeye fps.
	int				  m_iFPS;						// Frames Per Second.
	int				  m_iFPSTemp;
	u32				  m_nTextID[8],					// Activated texture ID.
					  m_nVertexBufferID[16],		// The current vertex buffer set.
					  m_nIndexBufferID,				// The current index buffer set.
					  m_nVertexDecID;				// The current vertex declaration set.
	bool			  m_bRF[ERF_Count];				// Bool baraye moshakhas shodane 
													// RenderFlag ha.
	bool			  m_bGCC[EGCC_Count];			// Bool for precalculate the graphics card capacities.
	Material		* m_pLastMaterial;				// The latest used material.
	u32				  m_iTriCount,					// The count of triangles rendered.
					  m_iDrawCount,					// The count of the draw calls.
					  m_iBatchCount;				// For internal use for counting correct triangles count when using instancing
	core::RectI		  m_rViewPort;

	//! Vertex declarations
	VertexDec		**	m_ppVertexDecs;

	std::vector<VertexDec*>
						m_vVertexDecs;			//!< The created VertexDec pointers
	std::vector<HardwareBuffer*>
						m_vBuffers;				//!< The created buffer pointers

	RendererPipeline	m_eRendererPipeline;

	// Deferred render targets
	Texture			*	m_pPosMap,
					*	m_pDifMap,
					*	m_pNorMap;

	// Screen quad
	gui::Image		*	m_pScreenQuad;
	
	//! rendering callbacks
	kgeRenderCallback   m_clbk_OnLost;              //  callback function called on device lost
	kgeRenderCallback   m_clbk_OnReset;             //  callback function called on device reset


	// bad az Init ba seda zadan in tabe maghadir avalie engine set mishan.
	virtual bool AfterInit() = 0;

	// Precalculate the graphics card capacities.
	virtual void InitGCC() = 0;

	//! Adds VertexDec pointers
	virtual void AddVertexDec(VertexDec* pVD);

	//! Adds hardware buffer pointer
	virtual void AddHardwareBuffer(HardwareBuffer* pHB);

}; // Renderer

} // gfx

} // kge

#endif // RENDERER_H