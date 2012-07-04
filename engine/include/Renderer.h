#ifndef KGE_RENDERER_H
#define KGE_RENDERER_H

#include "structs.h"
#include "array.h"
#include "String.h"
#include "Matrix.h"

namespace kge
{
	//! Graphics classes
	namespace gfx
	{
		//   F O R W A R D   D E C L A R A T I O N		
		class HardwareBuffer;
		class VertexDec;
		struct CustomVertexElement;

		//! The transform mode enum
		enum TransformMode
		{
			ETM_World,				//!< World matrix
			ETM_View,				//!< View matrix
			ETM_Projection,			//!< Projection matrix
			ETM_ViewProjection		//!< Projection * View matrix only usable by GetTransform

		}; // TransformMode

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

			//! Create a custom vertex declaration and returns its pointer
			/*!
				\param VertexInfoArray An array of CustomVertexElement to declare the vertex type
				\param sName The VertexDec name the name must be unique you need this name to get
				this VertexDec later and use it.
				\return Returns the created VertexDec pointer
				\sa VertexDec, CustomVertexElement, SetVertexDeclaration
				\todo Add this to ogl renderer plugin
			 */
			virtual VertexDec* CreateVertexDeclaration 
				(core::DynamicArray<CustomVertexElement*> VertexInfoArray, core::stringc& sName) = 0;

			//! Sets the vertex declaration
			/*! Sets the vertex declaration to prepare renderer to render something
				\param pVD The VertexDec pointer to set
				\sa VertexDec, CustomVertexElement, CreateVertexDeclaration
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetVertexDeclaration(VertexDec* pVD) = 0;

			//! Draw a list of triangles
			/*! Draw a vertex buffer you must set the buffers manually before calling this function
				\param VCount The Vertices count.
				\param ICount The Indices count.
				\param VertexStart The vertex offset in VertexBuffwe
				\param StartIndex The index offset in IndexBuffer
				\sa SetVertexDeclaration, SetIndexBuffer, SetVertexBuffer
				\todo Add this to ogl renderer plugin
			*/
			virtual void DrawTriangleList(u32 VCount, u32 ICount,
										  u32 VertexStart = 0, u32 StartIndex = 0) = 0;

			//! Sets the transformation of World, View or Projection matrices
			/*!
				\param mat The Matrix to set
				\param TM The enum that define the TransformMode. Note you can not use ETM_ViewProjection here
				\sa GetTransform
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetTransForm(math::Matrix *mat, TransformMode TM = ETM_World)=0;

			//! Returns the transformation of World, View, Projection or ViewProjection matrices
			/*!
			  \param TM The enum that define the TransformMode
			  \returns Returns the requested matrix
			  \sa SetTransform
			  \todo Add this to ogl renderer plugin
			 */
			virtual math::Matrix GetTransForm(TransformMode TM = ETM_World)=0;

		protected:

			bool			m_bUseStencil,			//!< Create and use stencil buffer
							m_bUseShaders,			//!< Dose renderer support shaders?
							m_bIsSceneRunning;		//!< Is scene running
			int				m_iFPS,					//!< Frame per second
							m_iFPSTemp,				//!< Temp Frame per second for calculating FPS
							m_iTriCount,			//!< Rendered triangle count
							m_iDrawCount,			//!< Draw call count
							m_iBatchCount;			//!< For internal use for counting correct triangles count when using instancing
			u32				m_nTextID[8],			//!< Activated texture ID.
							m_nVertexBufferID[16],	//!< The current vertex buffer set.
							m_nIndexBufferID,		//!< The current index buffer set.
							m_nVertexDecID;			//!< The current vertex declaration set.
			math::Matrix	m_mViewProj;			//!< Projection * View matrix.


		}; // Renderer

	} // gfx

} // kge

#endif // KGE_RENDERER_H
