#ifndef KGE_RENDEREROGL_H
#define KGE_RENDEREROGL_H

#include "../include/Renderer.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

namespace kge
{
	namespace gfx
	{
		class RendererOGL : public Renderer
		{
		public:

			//! Constructor
			RendererOGL();

			//! Destructor
			~RendererOGL();

			//! Initialize the renderer
			/*! test detail
				\param params The initialize parameters that user requested to
				initialize the renderer.
				\return Returns true on success.s
			 */
			bool Init(InitParameters &params);

			//! Clear the render target
			/*! 
				\param bColor Clear the color map?
				\param bDepth Clear the depth buffer?
				\param bStencil Clear the stencil buffer?
				\sa BeginRendering
			 */
			bool Clear(bool bColor, bool bDepth, bool bStencil);
		
			//! Begin the rendering
			/*! 
				\param bColor Clear the color map?
				\param bDepth Clear the depth buffer?
				\param bStencil Clear the stencil buffer?
				\sa Clear, EndRendering
			 */
			bool BeginRendering(bool bColor, bool bDepth, bool bStencil);

			//! End the rendering and draw to back buffer.
			/*! 
				\return Returns true on success.
			 */
			bool EndRendering();

		private:
			//--------------------------------
			// functions
			//--------------------------------

			// Check device if it is createable.
			bool CheckDevice( void );

			//--------------------------------
			// Members
			//--------------------------------
		#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			HGLRC m_pGLRC;
			HDC m_pHDC;
		#endif

		}; // RendererOGL
	} // gfx
} // kge
#endif // KGE_RENDEREROGL_H
