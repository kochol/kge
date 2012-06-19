#ifndef KGE_RENDERER_H
#define KGE_RENDERER_H

#include "structs.h"

namespace kge
{
	//! Graphics classes
	namespace gfx
	{
		class HardwareBuffer;

		//! The interface for working with renderers in KGE.
		class KGE_API Renderer
		{
		public:

			//! Constructor
			Renderer();

			//! Destructor
			virtual ~Renderer() {}

			//! Initialize the renderer
			/*! test detail
				\param params The initialize parameters that user requested to
				initialize the renderer.
				\return Returns true on success.s
			 */
			virtual bool Init(InitParameters &params) = 0;

			//! Clear the render target
			/*! 
				\param bColor Clear the color map?
				\param bDepth Clear the depth buffer?
				\param bStencil Clear the stencil buffer?
				\sa BeginRendering
			 */
			virtual bool Clear(bool bColor, bool bDepth, bool bStencil) = 0;
		
			//! Begin the rendering
			/*! 
				\param bColor Clear the color map?
				\param bDepth Clear the depth buffer?
				\param bStencil Clear the stencil buffer?
				\sa Clear, EndRendering
			 */
			virtual bool BeginRendering(bool bColor, bool bDepth, bool bStencil) = 0;

			//! End the rendering and draw to back buffer.
			/*! 
				\return Returns true on success.
			 */
			virtual bool EndRendering() = 0;

			/*! Create a vertex buffer from custom vertex type and store it on video memory.
				\param Vertices A pointer to the Vertices array.
				\param VCount The Vertices count.
				\param Stride The vertex struct size in bytes.
				\param isDynamic Pass true if you want update this buffer data
				\return Returns the created vertex buffer.
				\sa HardwareBuffer
				\todo Add this to ogl renderer plugin
			*/
			virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
													   u32   Stride,
													   bool  isDynamic = false) = 0;

			//! Creates an Index buffer on video memory
			/*!
				\param Indices The pointer to indices data
				\param ICount The index count
				\param eIndexBufferType The index buffer format is 16 bit or 32 bit(for larger meshes)
				\param isDynamic Pass true if you want update this buffer data
				\return Returns the created index buffer
				\sa HardwareBuffer
				\todo Add this to ogl renderer plugin
			 */
			virtual HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
				IndexBufferType eIndexBufferType = EIBT_16Bit,
				bool isDynamic = false) = 0;

			//! Sets the vertex buffer for multi streaming and rendering
			/*! Sets a vertex buffer to its stage to start rendering
				\param pBuffer The pointer to vertex buffer
				\param stage The stage number for using in multi streaming
				\sa CreateVertexBuffer, HardwareBuffer
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetVertexBuffer(HardwareBuffer* pBuffer, int stage = 0) = 0;
			
			//! Sets the index buffer for rendering
			/*!
				\param pBuffer The pointer to index buffer
				\sa CreateIndexBuffer, HardwareBuffer
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetIndexBuffer(HardwareBuffer* pBuffer) = 0;

		protected:

			bool		m_bUseStencil,			//!< Create and use stencil buffer
						m_bUseShaders,			//!< Dose renderer support shaders?
						m_bIsSceneRunning;		//!< Is scene running
			int			m_iFPS,					//!< Frame per second
						m_iFPSTemp,				//!< Temp Frame per second for calculating FPS
						m_iTriCount,			//!< Rendered triangle count
						m_iDrawCount;			//!< Draw call count
			u32			m_nTextID[8],			//!< Activated texture ID.
						m_nVertexBufferID[16],	//!< The current vertex buffer set.
						m_nIndexBufferID,		//!< The current index buffer set.
						m_nVertexDecID;			//!< The current vertex declaration set.

		}; // Renderer

	} // gfx

} // kge

#endif // KGE_RENDERER_H
