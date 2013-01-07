#ifndef KGE_RENDERERD3D9_H
#define KGE_RENDERERD3D9_H

#include "../include/Renderer.h"
#include <D3D11.h>
#include <vector>

namespace kge
{
	namespace gfx
	{
		class VertexBufferDX11;
		class IndexBufferDX11;
		class RendererD3D11 : public Renderer
		{
		public:

			//! Constructor
			RendererD3D11();

			//! Destructor
			~RendererD3D11();

			//! Initialize the renderer
			/*! test detail
				\param params The initialize parameters that user requested to
				initialize the renderer.
				\return Returns true on success.
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

			//! Removes VertexBufferDX9 from internal list.
			void RemoveVertexBuffer(VertexBufferDX11* vb);
			
			//! Removes IndexBufferDX9 from internal list.
			void RemoveIndexBuffer(IndexBufferDX11* ib);

			//! Sets the clear color
			void SetClearColor(const Color& ClearColor);

			//! Creates a texture from an image.
			/*!
			  \param pImg The Image resource pointer that you want to create texture from.
			  \returns Returns the created Texture pointer
			 */
			Texture* CreateTexture(Image* pImg);

			//! Returns the D3D device if the renderer is D3D
			void* GetDirect3dDevice();

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

		protected:

			D3D_DRIVER_TYPE							m_driverType;
			D3D_FEATURE_LEVEL						m_featureLevel;
			ID3D11Device						*   m_pd3dDevice;
			ID3D11DeviceContext					*   m_pImmediateContext;
			IDXGISwapChain						*   m_pSwapChain;
			ID3D11RenderTargetView				*	m_pRenderTargetView;
			ID3D11Texture2D						*	m_pBackBuffer;
			DXGI_SWAP_CHAIN_DESC					m_SwapChainDesc;		// D3D11 swap chain desc
			std::vector<VertexBufferDX11*>	        m_vVBuffers;			// Vertex buffers array.
			std::vector<IndexBufferDX11*>		    m_vIBuffers;			// Index buffers array.
			float									m_fClearColor[4];

			// C a m e r a   M a t r i x e s
			math::Matrix							m_mProj,				// Projection Matrix.
													m_mView,				// View Matrix.
													m_mWorld;				// World matrix.
			
		}; // RendererD3D11

	} // gfx

} // kge

#endif // KGE_RENDERERD3D9_H
