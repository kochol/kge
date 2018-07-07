#include "RendererBgfx.h"
#include "bgfx\bgfx.h"

namespace kge
{
	namespace gfx
	{

RendererBgfx::RendererBgfx()
{
}

RendererBgfx::~RendererBgfx()
{
}

bool RendererBgfx::Init(Device * device, InitParameters & params)
{
	m_pDevice = device;
	m_Params = params;
	m_debug = BGFX_DEBUG_TEXT;

	bgfx::Init init;
	init.type = bgfx::RendererType::Count;
	init.vendorId = BGFX_PCI_ID_NONE;
	init.resolution.width = params.Width;
	init.resolution.height = params.Height;
	init.resolution.reset = 0;
	if (params.FullScreen)
		init.resolution.reset = BGFX_RESET_FULLSCREEN;
	if (params.VSync)
		init.resolution.reset |= BGFX_RESET_VSYNC;
	if (params.AAMode == EAAM_MSAA)
	{
		switch (params.AAQuality)
		{
		case 2:
			init.resolution.reset |= BGFX_RESET_MSAA_X2;
			break;

		case 4:
			init.resolution.reset |= BGFX_RESET_MSAA_X4;
			break;

		case 8:
			init.resolution.reset |= BGFX_RESET_MSAA_X8;
			break;

		case 16:
			init.resolution.reset |= BGFX_RESET_MSAA_X16;
			break;

		default:
			io::Logger::Warning("The AAQuality number not supported. The valid values are 2, 4, 8, 16");
			break;
		}
	}
	bool r = bgfx::init(init);

	// Enable debug text.
	bgfx::setDebug(m_debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
	);

	return r;
}

bool RendererBgfx::Clear(bool Pixel, bool Zbuffer, bool Stencil)
{
	return false;
}

bool RendererBgfx::BeginRendering(bool ClearPixel, bool ClearZbuffer, bool ClearStencil)
{
	return false;
}

bool RendererBgfx::CreateRenderableTexture(Texture ** ppOutTexture, int width, int height, TextureFormat tf, bool CreateDepthStencilBuffer = true, bool AntiAliasing = false)
{
	return false;
}

void RendererBgfx::EndRendering()
{
	// Advance to next frame. Rendering thread will be kicked to
	// process submitted rendering primitives.
	bgfx::frame();
}

void RendererBgfx::SetClearColor(const Colorf & ClearColor)
{
}

void RendererBgfx::SetCullingMode(CullMode cm)
{
}

void RendererBgfx::SetVertexBuffer(HardwareBuffer * pBuffer, int stage = 0)
{
}

void RendererBgfx::SetIndexBuffer(HardwareBuffer * pBuffer)
{
}

void RendererBgfx::SetRenderTarget(int RenderNumber = 0, Texture * RenderTarget = NULL)
{
}

void RendererBgfx::SetViewLookatLH(math::Vector & Position, math::Vector & Lookat, math::Vector & UpDirection)
{
}

void RendererBgfx::SetViewLookatRH(math::Vector & Position, math::Vector & Lookat, math::Vector & UpDirection)
{
}

void RendererBgfx::SetPerspectiveLH(float Fov = 0.7853f, float NearPlane = 1.0f, float FarPlane = 1000.0f)
{
}

void RendererBgfx::SetPerspectiveRH(float Fov = 0.8f, float NearPlane = 1.0f, float FarPlane = 1000.0f)
{
}

void RendererBgfx::SetTransForm(math::Matrix * mat, TransformMode TM = ETM_World)
{
}

math::Matrix RendererBgfx::GetTransForm(TransformMode TM = ETM_World)
{
	return math::Matrix();
}

bool RendererBgfx::AddTexture(Texture ** ppOutTexture, u32 Handle, const char * FileName, const char * Name, void * ExtraParam)
{
	return false;
}

void RendererBgfx::SetTexture(Texture * pTex, int index = 0)
{
}

void RendererBgfx::Enable(RenderFlags RF)
{
}

void RendererBgfx::Disable(RenderFlags RF)
{
}

void RendererBgfx::SetMaterial(Material * pMaterial)
{
}

void RendererBgfx::SetTextureParams(TextureParams Params, int TextureStage = 0)
{
}

bool RendererBgfx::CanDo(GraphicCardCaps Item)
{
	return false;
}

void RendererBgfx::SetStencilBuffer(kge::efx::EffectType ET, u8 State)
{
}

void RendererBgfx::AddFont(kge::gui::Font ** ppOut, const char * strFontName, ul32 FontSize)
{
}

HardwareBuffer * RendererBgfx::CreateVertexBuffer(void * Vertices, u32 VCount, VertexType eVType = EVT_V3TN, bool isDynamic = false)
{
	return nullptr;
}

HardwareBuffer * RendererBgfx::CreateVertexBuffer(void * Vertices, u32 VCount, u32 Stride, bool isDynamic = false)
{
	return nullptr;
}

HardwareBuffer * RendererBgfx::CreateIndexBuffer(void * Indices, u32 ICount, IndexBufferType eIndexBufferType = EIBT_16Bit, bool isDynamic = false)
{
	return nullptr;
}

void RendererBgfx::DrawTriangleList(HardwareBuffer * VB, HardwareBuffer * IB, u32 VCount, u32 ICount, VertexType eVType = EVT_V3TN)
{
}

void RendererBgfx::DrawTriangleList(u32 VCount, u32 ICount, VertexDec * CV, u32 VertexStart, u32 StartIndex)
{
}

Shader * RendererBgfx::CreateVertexShaderFromFile(const char * VertexFileName, const char * VertexMain = "VSMain", ShaderVersion eVVersion = ESV_VS1_1)
{
	return nullptr;
}

Shader * RendererBgfx::CreateVertexShaderFromString(const char * VertexCode, const char * VertexMain = "VSMain", ShaderVersion eVVersion = ESV_VS1_1)
{
	return nullptr;
}

Shader * RendererBgfx::CreatePixelShaderFromFile(const char * PixelFileName, const char * PixelMain = "PSMain", ShaderVersion ePVersion = ESV_PS1_1)
{
	return nullptr;
}

Shader * RendererBgfx::CreatePixelShaderFromString(const char * PixelCode, const char * PixelMain = "PSMain", ShaderVersion ePVersion = ESV_PS1_1)
{
	return nullptr;
}

Texture * RendererBgfx::CreateTexture(ImageData * pImg)
{
	return nullptr;
}

bool RendererBgfx::AddVertexShader(Shader ** ppOutShader, u32 handle, const char * fileName, const char * mainName, void * extraParam)
{
	return false;
}

bool RendererBgfx::AddPixelShader(Shader ** ppOutShader, u32 handle, const char * fileName, const char * mainName, void * extraParam)
{
	return false;
}

RendererAPI RendererBgfx::GetRendererType()
{
	return RendererAPI();
}

bool RendererBgfx::SetFog(FogType mode, Colorf FogColor, float expDensity = 0, float linearStart = 0, float linearEnd = 1000000)
{
	return false;
}

bool RendererBgfx::SetVideoMode(int Width, int Height, int Bits, bool UseStencil = false, bool Fullscreen = false, int MultiSample = 0, bool VSync = false)
{
	return false;
}

void RendererBgfx::SetViewPort(const core::RectI & viewport)
{
	bgfx::setViewRect(0, uint16_t(viewport.X), uint16_t(viewport.Y), uint16_t(viewport.Width), uint16_t(viewport.Height));
}

VertexDec * RendererBgfx::CreateVertexDeclaration(CustomVertexElement * pVertexInfoArray, core::stringc sName)
{
	return nullptr;
}

void RendererBgfx::SetVertexDec(VertexDec * pVD)
{
}

void RendererBgfx::SetRenderingWindow(void * hwnd)
{
}

void RendererBgfx::OnResize(int width, int height)
{
}

void RendererBgfx::OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync)
{
}

void RendererBgfx::SetStreamSourceFreq(u32 StreamID, u32 Count)
{
}

void RendererBgfx::SetDepthBias(float fBias)
{
}

void RendererBgfx::SetDepthBiasSlope(float fSlope)
{
}

void RendererBgfx::SetDepthMode(DepthTestMode e = EDTM_LessEqual)
{
}

bool RendererBgfx::AfterInit()
{
	return false;
}

void RendererBgfx::InitGCC()
{
}

} // gfx

} // kge
