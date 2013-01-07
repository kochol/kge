#ifndef KGE_RENDEREROGL_H
#define KGE_RENDEREROGL_H

#include "../include/Renderer.h"
#include <vector>

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
			HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
				IndexBufferType eIndexBufferType,
				bool isDynamic);

			//! Sets the vertex buffer for multi streaming and rendering
			/*! Sets a vertex buffer to its stage to start rendering
				\param pBuffer The pointer to vertex buffer
				\param stage The stage number for using in multi streaming
				\sa CreateVertexBuffer, HardwareBuffer
			 */
			void SetVertexBuffer(HardwareBuffer* pBuffer, int stage);

			//! Sets the index buffer for rendering
			/*!
				\param pBuffer The pointer to index buffer
				\sa CreateIndexBuffer, HardwareBuffer
			 */
			void SetIndexBuffer(HardwareBuffer* pBuffer);

			//! Create a custom vertex declaration and returns its pointer
			/*!
				\param VertexInfoArray An array of CustomVertexElement to declare the vertex type
				\return Returns the created VertexDec pointer
				\sa VertexDec, CustomVertexElement
			 */
			VertexDec* CreateVertexDeclaration
				(core::DynamicArray<CustomVertexElement> VertexInfoArray, core::stringc& sName);

			//! Sets the vertex declaration
			/*! Sets the vertex declaration to prepare renderer to render something
				\pVD The VertexDec pointer to set
				\sa VertexDec, CustomVertexElement, CreateVertexDeclaration
			 */
			void SetVertexDeclaration(VertexDec* pVD);

			//! Draw a list of triangles
			/*! Draw a vertex buffer you must set the buffers manually before calling this function
				\param VCount The Vertices count.
				\param ICount The Indices count.
				\param VertexStart The vertex offset in VertexBuffwe
				\param StartIndex The index offset in IndexBuffer
				\sa SetVertexDeclaration, SetIndexBuffer, SetVertexBuffer
			*/
			void DrawTriangleList(u32 VCount, u32 ICount,
								  u32 VertexStart = 0, u32 StartIndex = 0);

			//! Sets the transformation of World, View or Projection matrices
			/*!
				\param mat The Matrix to set
				\param TM The enum that define the TransformMode. Note you can not use ETM_ViewProjection here
				\sa GetTransform
			 */
			void SetTransForm(math::Matrix *mat, TransformMode TM = ETM_World);

			//! Returns the transformation of World, View, Projection or ViewProjection matrices
			/*!
			  \param TM The enum that define the TransformMode
			  \returns Returns the requested matrix
			  \sa SetTransform
			 */
			math::Matrix GetTransForm(TransformMode TM = ETM_World);

			//! Sets the clear color
			void SetClearColor(const Color& ClearColor);

			//! Creates a texture from an image.
			/*!
			  \param pImg The Image resource pointer that you want to create texture from.
			  \returns Returns the created Texture pointer
			 */
			Texture* CreateTexture(Image* pImg);

			//! Sets the texture
			void SetTexture(Texture* pTex, int Stage = 0);

			/*! Enable render flags.
				\param RF The RenderFlags that you want to be enabled.
			*/
			void Enable(RenderFlags RF);

			/*! Disable render flags.
				\param RF The RenderFlags that you want to be disabled.
			*/
			void Disable ( RenderFlags RF );

			//! Sets the scissor region
			void SetScissorRegion(int x, int y, int width, int height);

		private:

			//! Vertex declaration
			std::vector<core::DynamicArray<CustomVertexElement> >	m_vVertexInfoArray;
			VertexDec													*	m_pCurrentVD;

			//! Matrices
			math::Matrix													m_mWorld,
																			m_mView,
																			m_mProj;

			//--------------------------------
			// functions
			//--------------------------------

			// Check device if it is create able.
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
