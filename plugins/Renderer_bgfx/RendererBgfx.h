#pragma once

#include "../../include/kge/gfx/Renderer.h"

namespace kge
{
	namespace gfx
	{
		class RendererBgfx: public Renderer
		{
		public:
			
			//! Constructor
			RendererBgfx();

			//! Destructor
			~RendererBgfx();

			// Inherited via Renderer
			virtual bool Init(Device * device, InitParameters & params) override;

			virtual bool Clear(bool Pixel, bool Zbuffer, bool Stencil) override;

			virtual bool BeginRendering(bool ClearPixel, bool ClearZbuffer, bool ClearStencil) override;

			virtual bool CreateRenderableTexture(Texture ** ppOutTexture, int width, int height, TextureFormat tf, bool CreateDepthStencilBuffer = true, bool AntiAliasing = false) override;

			virtual void EndRendering() override;

			virtual void SetClearColor(const Colorf & ClearColor) override;

			virtual void SetCullingMode(CullMode cm) override;

			virtual void SetVertexBuffer(HardwareBuffer * pBuffer, int stage = 0) override;

			virtual void SetIndexBuffer(HardwareBuffer * pBuffer) override;

			virtual void SetRenderTarget(int RenderNumber = 0, Texture * RenderTarget = NULL) override;

			virtual void SetViewLookatLH(math::Vector & Position, math::Vector & Lookat, math::Vector & UpDirection) override;

			virtual void SetViewLookatRH(math::Vector & Position, math::Vector & Lookat, math::Vector & UpDirection) override;

			virtual void SetPerspectiveLH(float Fov = 0.7853f, float NearPlane = 1.0f, float FarPlane = 1000.0f) override;

			virtual void SetPerspectiveRH(float Fov = 0.8f, float NearPlane = 1.0f, float FarPlane = 1000.0f) override;

			virtual void SetTransForm(math::Matrix * mat, TransformMode TM = ETM_World) override;

			virtual math::Matrix GetTransForm(TransformMode TM = ETM_World) override;

			virtual bool AddTexture(Texture ** ppOutTexture, u32 Handle, const char * FileName, const char * Name, void * ExtraParam) override;

			virtual void SetTexture(Texture * pTex, int index = 0) override;

			virtual void Enable(RenderFlags RF) override;

			virtual void Disable(RenderFlags RF) override;

			virtual void SetMaterial(Material * pMaterial) override;

			virtual void SetTextureParams(TextureParams Params, int TextureStage = 0) override;

			virtual bool CanDo(GraphicCardCaps Item) override;

			virtual void SetStencilBuffer(kge::efx::EffectType ET, u8 State) override;

			virtual void AddFont(kge::gui::Font ** ppOut, const char * strFontName, ul32 FontSize) override;

			virtual HardwareBuffer * CreateVertexBuffer(void * Vertices, u32 VCount, VertexType eVType = EVT_V3TN, bool isDynamic = false) override;

			virtual HardwareBuffer * CreateVertexBuffer(void * Vertices, u32 VCount, u32 Stride, bool isDynamic = false) override;

			virtual HardwareBuffer * CreateIndexBuffer(void * Indices, u32 ICount, IndexBufferType eIndexBufferType = EIBT_16Bit, bool isDynamic = false) override;

			virtual void DrawTriangleList(HardwareBuffer * VB, HardwareBuffer * IB, u32 VCount, u32 ICount, VertexType eVType = EVT_V3TN) override;

			virtual void DrawTriangleList(u32 VCount, u32 ICount, VertexDec * CV, u32 VertexStart, u32 StartIndex) override;

			virtual Shader * CreateVertexShaderFromFile(const char * VertexFileName, const char * VertexMain = "VSMain", ShaderVersion eVVersion = ESV_VS1_1) override;

			virtual Shader * CreateVertexShaderFromString(const char * VertexCode, const char * VertexMain = "VSMain", ShaderVersion eVVersion = ESV_VS1_1) override;

			virtual Shader * CreatePixelShaderFromFile(const char * PixelFileName, const char * PixelMain = "PSMain", ShaderVersion ePVersion = ESV_PS1_1) override;

			virtual Shader * CreatePixelShaderFromString(const char * PixelCode, const char * PixelMain = "PSMain", ShaderVersion ePVersion = ESV_PS1_1) override;

			virtual Texture * CreateTexture(ImageData * pImg) override;

			virtual bool AddVertexShader(Shader ** ppOutShader, u32 handle, const char * fileName, const char * mainName, void * extraParam) override;

			virtual bool AddPixelShader(Shader ** ppOutShader, u32 handle, const char * fileName, const char * mainName, void * extraParam) override;

			virtual RendererAPI GetRendererType() override;

			virtual bool SetFog(FogType mode, Colorf FogColor, float expDensity = 0, float linearStart = 0, float linearEnd = 1000000) override;

			virtual bool SetVideoMode(int Width, int Height, int Bits, bool UseStencil = false, bool Fullscreen = false, int MultiSample = 0, bool VSync = false) override;

			virtual void SetViewPort(const core::RectI & viewport) override;

			virtual VertexDec * CreateVertexDeclaration(CustomVertexElement * pVertexInfoArray, core::stringc sName) override;

			virtual void SetVertexDec(VertexDec * pVD) override;

			virtual void SetRenderingWindow(void * hwnd) override;

			virtual void OnResize(int width, int height) override;

			virtual void OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync) override;

			virtual void SetStreamSourceFreq(u32 StreamID, u32 Count) override;

			virtual void SetDepthBias(float fBias) override;

			virtual void SetDepthBiasSlope(float fSlope) override;

			virtual void SetDepthMode(DepthTestMode e = EDTM_LessEqual) override;

			virtual bool AfterInit() override;

			virtual void InitGCC() override;

		private:

			uint32_t m_debug; // Debug params

		};
	}
}