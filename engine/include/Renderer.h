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
			Renderer(): m_bUseStencil(false), m_bUseShaders(false), 
				m_bIsSceneRunning(false), m_iFPS(0), m_iFPSTemp(0) {}

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
				\return Returns the created vertex buffer.
				\sa HardwareBuffer
			*/
			virtual HardwareBuffer* CreateVertexBuffer(void* Vertices, u32 VCount, 
													   u32   Stride,
													   bool  isDynamic = false) = 0;


		protected:

			bool		m_bUseStencil,		//!< Create and use stencil buffer
						m_bUseShaders,		//!< Dose renderer support shaders?
						m_bIsSceneRunning;	//!< Is scene running
			int			m_iFPS,				//!< Frame per second
						m_iFPSTemp,			//!< Temp Frame per second for calculating FPS
						m_iTriCount,		//!< Rendered triangle count
						m_iDrawCount;		//!< Draw call count

		}; // Renderer

	} // gfx

} // kge

#endif // KGE_RENDERER_H
