#pragma once

#include <Rocket/Core/RenderInterface.h>
#include "kgedef.h"

//=====================================
// DLL export / import
//=====================================
#if defined( _MSC_VER )
#	ifdef LIBROCKETKGERENDERER_EXPORTS
#		define KGEROCKET_API __declspec( dllexport )
#	else
#		define KGEROCKET_API __declspec( dllimport )
#	endif
#endif

namespace kge
{
	class KGEROCKET_API libRocketKGERenderer: public Rocket::Core::RenderInterface
	{
		//! Constructor
		libRocketKGERenderer();

		//! Destructor
		~libRocketKGERenderer();

		/// Called by Rocket when it wants to render geometry that it does not wish to optimise.
		virtual void RenderGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture, const Rocket::Core::Vector2f& translation);

		/// Called by Rocket when it wants to compile geometry it believes will be static for the forseeable future.
		virtual Rocket::Core::CompiledGeometryHandle CompileGeometry(Rocket::Core::Vertex* vertices, int num_vertices, int* indices, int num_indices, Rocket::Core::TextureHandle texture);

		/// Called by Rocket when it wants to render application-compiled geometry.
		virtual void RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry, const Rocket::Core::Vector2f& translation);

		/// Called by Rocket when it wants to release application-compiled geometry.
		virtual void ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle geometry);

		/// Called by Rocket when it wants to enable or disable scissoring to clip content.
		virtual void EnableScissorRegion(bool enable);

		/// Called by Rocket when it wants to change the scissor region.
		virtual void SetScissorRegion(int x, int y, int width, int height);

		/// Called by Rocket when a texture is required by the library.
		virtual bool LoadTexture(Rocket::Core::TextureHandle& texture_handle, Rocket::Core::Vector2i& texture_dimensions, const Rocket::Core::String& source);

		/// Called by Rocket when a texture is required to be built from an internally-generated sequence of pixels.
		virtual bool GenerateTexture(Rocket::Core::TextureHandle& texture_handle, const u8* source, const Rocket::Core::Vector2i& source_dimensions);

		/// Called by Rocket when a loaded texture is no longer required.
		virtual void ReleaseTexture(Rocket::Core::TextureHandle texture_handle);

		/// Returns the native horizontal texel offset for the renderer.
		float GetHorizontalTexelOffset();

		/// Returns the native vertical texel offset for the renderer.
		float GetVerticalTexelOffset();

	}; // libRocketKGERenderer

} // kge