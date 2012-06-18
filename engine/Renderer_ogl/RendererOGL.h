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

			/*! Create a vertex buffer from custom vertex type and store it on video memory.
				\param Vertices A pointer to the Vertices array.
				\param VCount The Vertices count.
				\param Stride The vertex struct size in bytes.
				\return Returns the created vertex buffer.
				\sa HardwareBuffer
			*/
			HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
											   u32   Stride,
											   bool  isDynamic);

			//! Creates an Index buffer on video memory
			/*!
				\param Indices The pointer to indices data
				\param ICount The index count
				\param eIndexBufferType The index buffer format is 16 bit or 32 bit(for larger meshes)
				\param isDynamic Pass true if you want update this buffer data
				\return Returns the created index buffer
				\sa HardwareBuffer
			 */
			virtual HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
				IndexBufferType eIndexBufferType,
				bool isDynamic);

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
