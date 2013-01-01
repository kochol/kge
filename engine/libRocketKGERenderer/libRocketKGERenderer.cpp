#include "../include/libRocketKGERenderer.h"

namespace kge
{

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
		return NULL;

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
		return false;

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
