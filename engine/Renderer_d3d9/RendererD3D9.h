#ifndef KGE_RENDERERD3D9_H
#define KGE_RENDERERD3D9_H

#include "../include/Renderer.h"
#include <d3d9.h>
#include <vector>

namespace kge
{
	namespace gfx
	{
		class VertexBufferDX9;
		class IndexBufferDX9;
		class RendererD3D9 : public Renderer
		{
		public:

			//! Constructor
			RendererD3D9();

			//! Destructor
			~RendererD3D9();

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

			//! Sets the vertex buffer for multi streaming and rendering
			/*! Sets a vertex buffer to its stage to start rendering
				\param pBuffer The pointer to vertex buffer
				\param stage The stage number for using in multi streaming
				\sa CreateVertexBuffer, HardwareBuffer
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetVertexBuffer(HardwareBuffer* pBuffer, int stage);

			//! Sets the index buffer for rendering
			/*!
				\param pBuffer The pointer to index buffer
				\sa CreateIndexBuffer, HardwareBuffer
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetIndexBuffer(HardwareBuffer* pBuffer);

			//! Create a custom vertex declaration and returns its pointer
			/*!
				\param VertexInfoArray An array of CustomVertexElement to declare the vertex type
				\return Returns the created VertexDec pointer
				\sa VertexDec, CustomVertexElement
				\todo Add this to ogl renderer plugin
			 */
			virtual VertexDec* CreateVertexDeclaration
				(core::DynamicArray<CustomVertexElement*> VertexInfoArray, core::stringc& sName);

			//! Sets the vertex declaration
			/*! Sets the vertex declaration to prepare renderer to render something
				\pVD The VertexDec pointer to set
				\sa VertexDec, CustomVertexElement, CreateVertexDeclaration
				\todo Add this to ogl renderer plugin
			 */
			virtual void SetVertexDeclaration(VertexDec* pVD);

			//! Removes VertexBufferDX9 from internal list.
			void RemoveVertexBuffer(VertexBufferDX9* vb);
			
			//! Removes IndexBufferDX9 from internal list.
			void RemoveIndexBuffer(IndexBufferDX9* ib);

		protected:

			LPDIRECT3DDEVICE9						m_pD3DDevice;			// D3D9 device
			IDirect3DSwapChain9*					m_pSwapChain;			// D3D9 swap chain
			D3DPRESENT_PARAMETERS					m_d3dpp;				// D3D9 present parameters
			D3DFORMAT								m_DSFMT;				// depth and Stencil Buffer
			D3DMULTISAMPLE_TYPE						m_msType;				// Multi sample type
			ul32									msQuality;				// Multi sample quality
			D3DCAPS9								m_d3dCaps;				// D3D9 Caps
			LPDIRECT3DSURFACE9						m_pBackBufferSurface,	// Main Back buffer surface
													m_pBackBufferZSurface;	// Main depth buffer surface
			kge::ul32								m_Behavior;				// Device Behavior
			D3DCOLOR								m_ClearColor;			// Clear Color
			std::vector<VertexBufferDX9*>	        m_vVBuffers;			// Vertex buffers array.
			std::vector<IndexBufferDX9*>		        m_vIBuffers;	// Index buffers array.


			// Search for the best depth and stencil format.
			void CheckStencil(const InitParameters &params);
			
			// Check stencil format
			bool CheckStencilFMT();
			
			// Check device if it is create able.
			bool CheckDevice(D3DCAPS9 *pCaps, DWORD dwBehavior);

		}; // RendererD3D9

	} // gfx

} // kge

#endif // KGE_RENDERERD3D9_H
