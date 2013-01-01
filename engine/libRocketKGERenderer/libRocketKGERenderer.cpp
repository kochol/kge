#include "../include/libRocketKGERenderer.h"
#include "../include/Renderer.h"
#include "../include/HardwareBuffer.h"
#include "../include/Texture.h"

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

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	libRocketKGERenderer::~libRocketKGERenderer()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to render geometry that it does not wish to optimise.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::RenderGeometry( Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation )
	{

	} // RenderGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
	//------------------------------------------------------------------------------------
	Rocket::Core::CompiledGeometryHandle libRocketKGERenderer::CompileGeometry( Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture )
	{
		RocketKGECompiledGeometry* p = KGE_NEW(RocketKGECompiledGeometry);

		// Create vertex and index buffer
		p->vertices = g_pRenderer->CreateVertexBuffer(vertices, num_vertices, 20);
		p->num_vertices = num_vertices;
		p->indices = g_pRenderer->CreateIndexBuffer(indices, num_indices);
		p->num_indices = num_indices;

		p->texture = texture == NULL ? NULL : (gfx::Texture*) texture;;

		return (Rocket::Core::CompiledGeometryHandle)p;

	} // CompileGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to render application-compiled geometry.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::RenderCompiledGeometry( Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation )
	{

	} // RenderCompiledGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to release application-compiled geometry.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::ReleaseCompiledGeometry( Rocket::Core::CompiledGeometryHandle geometry )
	{
		RocketKGECompiledGeometry* p = (RocketKGECompiledGeometry*)geometry;
		p->vertices->DecRef();
		p->indices->DecRef();
		p->texture->DecRef();

		KGE_DELETE(p, RocketKGECompiledGeometry);

	} // ReleaseCompiledGeometry

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to enable or disable scissoring to clip content.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::EnableScissorRegion( bool enable )
	{

	} // EnableScissorRegion

	//------------------------------------------------------------------------------------
	// Called by Rocket when it wants to change the scissor region.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::SetScissorRegion( int x, int y, int width, int height )
	{

	} // SetScissorRegion

	//------------------------------------------------------------------------------------
	// Called by Rocket when a texture is required by the library.
	//------------------------------------------------------------------------------------
	bool libRocketKGERenderer::LoadTexture( Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source )
	{
		gfx::Texture* p = g_pTextureManager->Load(source.CString(), NULL, NULL);
		if (!p)
			return false;

		texture_handle = (Rocket::Core::TextureHandle)p;
		texture_dimensions.x = p->GetWidth();
		texture_dimensions.y = p->GetHeight();

		return true;

	} // LoadTexture

	//------------------------------------------------------------------------------------
	// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
	//------------------------------------------------------------------------------------
	bool libRocketKGERenderer::GenerateTexture( Rocket::Core::TextureHandle& texture_handle, const u8* source, const Rocket::Core::Vector2i& source_dimensions )
	{
		return false;

	} // GenerateTexture

	//------------------------------------------------------------------------------------
	// Called by Rocket when a loaded texture is no longer required.
	//------------------------------------------------------------------------------------
	void libRocketKGERenderer::ReleaseTexture( Rocket::Core::TextureHandle texture_handle )
	{
		gfx::Texture* p = (gfx::Texture*)texture_handle;

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
