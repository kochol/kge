#include "../../Include/gui/libRocketKGERenderer.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/gfx/HardwareBuffer.h"
#include "../../Include/gfx/Texture.h"
#include "../../Include/gfx/VertexDec.h"
#include "../../Include/gfx/Material.h"
#include "../../Include/math/Matrix.h"
#include "../../Include/gfx/ImageData.h"
#include "../../Include/core/Timer.h"
#include "../../Include/io/Logger.h"

//! Renderer public pointer
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;
KGE_IMPORT extern kge::ResourceManager<kge::gfx::Texture>	*	g_pTextureManager;

#if KGE_COMPILER == KGE_COMPILER_MSVC
#	pragma comment(lib, "../../bin/Debug/kge.lib")
#	pragma comment(lib, "RocketCore.lib")
#endif // KGE_COMPILER == KGE_COMPILER_MSVC

namespace kge
{
	// This structure is created for each set of geometry that Rocket compiles. It stores the vertex and index buffers and
	// the texture associated with the geometry, if one was specified.
	struct RocketKGECompiledGeometry
	{
		gfx::HardwareBuffer	*	vertices;
		int num_vertices;

		gfx::HardwareBuffer	*	indices;
		int num_indices;

		gfx::Texture		*	texture;
	};

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	libRocketKGERenderer::libRocketKGERenderer()
	{
		// Create vertex declaration
		kge::gfx::CustomVertexElement cve2[] =
		{
			{	
				0,
				0,
				kge::gfx::EVET_Float2,
				kge::gfx::EVEM_Default,
				kge::gfx::EVEU_Position,
				0
			},
			{
				0,
				8,
				kge::gfx::EVET_Color,
				kge::gfx::EVEM_Default,
				kge::gfx::EVEU_Color,
				0
			},
			{
				0,
				12,
				kge::gfx::EVET_Float2,
				kge::gfx::EVEM_Default,
				kge::gfx::EVEU_TexCoord,
				0
			},
			CusVertexEND()
		};
		m_pVD = g_pRenderer->CreateVertexDeclaration(cve2, "librocket");

		// Create material
		m_pMat = KGE_NEW(gfx::Material);
		m_pMat->Culling = false;
		m_pMat->Alpha = true;

		const char strVS[] =
			"float4x4 matWVP;\n"\
			"struct VS_INPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float4 Color : COLOR0;\n"\
			"	float2 Texcoord : TEXCOORD0;\n"\
			"};\n"\
			"\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Texcoord : TEXCOORD0;\n"\
			"	float4 color : TEXCOORD1;\n"\
			"};\n"\
			"\n"\
			"VS_OUTPUT main(VS_INPUT input)\n"\
			"{\n"\
			"	VS_OUTPUT o;\n"\
			"	o.Position = mul(input.Position, matWVP);\n"\
			"	o.Position.z = 0;\n "\
			"	o.Texcoord = input.Texcoord;\n"\
			"	o.color    = input.Color;\n"\
			"\n"\
			"	return o;\n"\
			"\n"\
			"} // main\n";
 		m_pMat->shader->m_pVertexShader = g_pRenderer->CreateVertexShaderFromString(strVS, "main", gfx::ESV_VS2);
 		if (m_pMat->shader->m_pVertexShader)
 			m_VshWVP = m_pMat->shader->m_pVertexShader->GetConstatnt("matWVP");

		const char strPS[] =
			"sampler2D DifMap;\n"\
			"\n"\
			"struct PS_INPUT\n"\
			"{\n"\
			"   float2 Texcoord : TEXCOORD0;\n"\
			"   float4 color : TEXCOORD1;\n"\
			"};\n"\
			"\n"\
			"float4 main(PS_INPUT input): COLOR0\n"\
			"{\n"\
			"	float4 r = tex2D(DifMap, input.Texcoord) * input.color;\n"\
			"\n"\
			"	return r;\n"\
			"\n"\
			"} // main\n";
		m_pMat->shader->m_pPixelShader = g_pRenderer->CreatePixelShaderFromString(strPS, "main", gfx::ESV_PS2);

		// Create a white texture
		u8 wc[] = {255,255,255,255};
		gfx::ImageData* pimg = KGE_NEW(gfx::ImageData)(0, "libRocketTextureWhite", wc, 4, 1, 1, 4, 32, gfx::ETF_A8R8G8B8, 1);
		m_pWhiteTexture = g_pRenderer->CreateTexture(pimg);
		KGE_DELETE(pimg, ImageData);
		kge::Device::GetSingletonPtr()->GetTextureManager()->Add(m_pWhiteTexture);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	libRocketKGERenderer::~libRocketKGERenderer()
	{
		KGE_DELETE(m_pMat, Material);
		KGE_DELETE(m_pWhiteTexture, Texture);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::RenderGeometry( Rocket::Core::Vertex* vertices, int num_vertices, 
		int* indices, int num_indices, Rocket::Core::TextureHandle texture, 
		const Rocket::Core::Vector2f& translation )
	{
		int i = 0;
		i++;
	} // RenderGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	//------------------------------------------------------------------------------------
	Rocket::Core::CompiledGeometryHandle libRocketKGERenderer::CompileGeometry
		( Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, 
		Rocket::Core::TextureHandle texture )
	{
		RocketKGECompiledGeometry* p = KGE_NEW(RocketKGECompiledGeometry);

		// Create vertex buffer
		p->vertices = g_pRenderer->CreateVertexBuffer(vertices, num_vertices, 20);
		p->num_vertices = num_vertices;

		// Create index buffer
		u16* pIndices = KGE_NEW_ARRAY(u16, num_indices);
		for (int i = 0; i < num_indices; i++)
		{
			pIndices[i] = indices[i];
		}
		p->indices = g_pRenderer->CreateIndexBuffer(pIndices, num_indices);
		p->num_indices = num_indices;
		KGE_DELETE_ARRAY(pIndices);

		p->texture = texture == NULL ? NULL : (gfx::Texture*) texture;
		if (p->texture)
			p->texture->AddRef();

		return (Rocket::Core::CompiledGeometryHandle)p;

	} // CompileGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to render application-compiled geometry.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::RenderCompiledGeometry
		( Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation )
	{
		RocketKGECompiledGeometry* p = (RocketKGECompiledGeometry*)geometry;
		g_pRenderer->SetVertexDec(m_pVD);
		g_pRenderer->SetVertexBuffer(p->vertices);
		g_pRenderer->SetIndexBuffer(p->indices);
		g_pRenderer->SetMaterial(m_pMat);
		if (p->texture)
			g_pRenderer->SetTexture(p->texture);
		else
			g_pRenderer->SetTexture(m_pWhiteTexture);

		kge::math::Matrix mvp = g_pRenderer->GetTransForm(gfx::ETM_ViewProjection);
		kge::math::Matrix m;
		m._41 = translation.x;
		m._42 = translation.y;
		g_pRenderer->SetTransForm(&m);
		mvp *= m;
		if (m_pMat->shader->m_pVertexShader)
			m_pMat->shader->m_pVertexShader->SetConstant(m_VshWVP, mvp.m_fMat, 16);
		g_pRenderer->DrawTriangleList(p->num_vertices, p->num_indices, m_pVD, 0, 0);

	} // RenderCompiledGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to release application-compiled geometry.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::ReleaseCompiledGeometry
		( Rocket::Core::CompiledGeometryHandle geometry )
	{
		RocketKGECompiledGeometry* p = (RocketKGECompiledGeometry*)geometry;
		p->vertices->DecRef();
		p->indices->DecRef();
		if (p->texture)
			p->texture->DecRef();

		KGE_DELETE(p, RocketKGECompiledGeometry);

	} // ReleaseCompiledGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to enable or disable scissoring to clip content.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::EnableScissorRegion( bool enable )
	{
		if (enable)
			g_pRenderer->Enable(gfx::ERF_Scissor);
		else
			g_pRenderer->Disable(gfx::ERF_Scissor);

	} // EnableScissorRegion

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to change the scissor region.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::SetScissorRegion( int x, int y, int width, int height )
	{
		g_pRenderer->SetScissorRegion(x, y, width, height);

	} // SetScissorRegion

	//------------------------------------------------------------------------------------
	// Called by Rocket when a texture is required by the library.
	//------------------------------------------------------------------------------------
	bool libRocketKGERenderer::LoadTexture( Rocket::Core::TextureHandle& texture_handle, 
		Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source )
	{
		gfx::Texture* p = Device::GetSingletonPtr()->GetSceneManager()->AddTexture(source.CString());
		if (!p)
			return false;

		texture_handle = (Rocket::Core::TextureHandle)p;
		texture_dimensions.x = p->GetWidth();
		texture_dimensions.y = p->GetHeight();
		p->AddRef();

		return true;

	} // LoadTexture

	//------------------------------------------------------------------------------------
	// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	//------------------------------------------------------------------------------------
	bool libRocketKGERenderer::GenerateTexture( Rocket::Core::TextureHandle& texture_handle, 
		const u8* source, const Rocket::Core::Vector2i& source_dimensions )
	{
		// Create image resource
 		int datasize = source_dimensions.x * source_dimensions.y * 4;
 		gfx::ImageData* pimg = KGE_NEW(gfx::ImageData)(0, "libRocketTexture", (u8*)source, 
			datasize, source_dimensions.x, source_dimensions.y, 4, 32, gfx::ETF_A8R8G8B8, 1);
 
 		// Create texture from an Image
 		gfx::Texture* pTex = g_pRenderer->CreateTexture(pimg);
		kge::Device::GetSingletonPtr()->GetTextureManager()->Add(pTex);
 
 		KGE_DELETE(pimg, ImageData);
 
 		texture_handle = (Rocket::Core::TextureHandle)pTex;
 
 		return true;

	} // GenerateTexture

	//------------------------------------------------------------------------------------
	// Called by Rocket when a loaded texture is no longer required.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::ReleaseTexture( Rocket::Core::TextureHandle texture_handle )
	{
		gfx::Texture* p = (gfx::Texture*)texture_handle;

		if (p)
			p->DecRef();

	} // ReleaseTexture
 
 	//------------------------------------------------------------------------------------
 	// Returns the native horizontal texel offset for the renderer.
 	//------------------------------------------------------------------------------------
 	float libRocketKGERenderer::GetHorizontalTexelOffset()
 	{
 		return -0.5f;
 
 	} // GetHorizontalTexelOffset
 
 	//------------------------------------------------------------------------------------
 	// Returns the native vertical texel offset for the renderer.
 	//------------------------------------------------------------------------------------
 	float libRocketKGERenderer::GetVerticalTexelOffset()
 	{
 		return -0.5f;
 
 	} // GetVerticalTexelOffset

} // kge
