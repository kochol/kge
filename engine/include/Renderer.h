#ifndef KGE_RENDERER_H
#define KGE_RENDERER_H

#include "structs.h"
#include "array.h"
#include "String.h"
#include "Matrix.h"
#include "Color.h"
#include "ResourceManager.h"

namespace kge
{
	//! Graphics classes
	namespace gfx
	{
		//   F O R W A R D   D E C L A R A T I O N		
		class HardwareBuffer;
		class VertexDec;
		class Texture;
		class Image;
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
			friend class HardwareBuffer;
		public:

			//! Constructor
			Renderer();

			//! Destructor
			virtual ~Renderer();

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
			 */
			virtual HardwareBuffer* CreateIndexBuffer(	void* Indices, u32 ICount,
				IndexBufferType eIndexBufferType = EIBT_16Bit,
				bool isDynamic = false) = 0;

			//! Sets the vertex buffer for multi streaming and rendering
			/*! Sets a vertex buffer to its stage to start rendering
				\param pBuffer The pointer to vertex buffer
				\param stage The stage number for using in multi streaming
				\sa CreateVertexBuffer, HardwareBuffer
			 */
			virtual void SetVertexBuffer(HardwareBuffer* pBuffer, int stage = 0) = 0;
			
			//! Sets the index buffer for rendering
			/*!
				\param pBuffer The pointer to index buffer
				\sa CreateIndexBuffer, HardwareBuffer
			 */
			virtual void SetIndexBuffer(HardwareBuffer* pBuffer) = 0;

			//! Create a custom vertex declaration and returns its pointer
			/*!
				\param VertexInfoArray An array of CustomVertexElement to declare the vertex type
				\param sName The VertexDec name the name must be unique you need this name to get
				this VertexDec later and use it.
				\return Returns the created VertexDec pointer
				\sa VertexDec, CustomVertexElement, SetVertexDeclaration
			 */
			virtual VertexDec* CreateVertexDeclaration 
				(core::DynamicArray<CustomVertexElement> VertexInfoArray, core::stringc& sName) = 0;

			//! Sets the vertex declaration
			/*! Sets the vertex declaration to prepare renderer to render something
				\param pVD The VertexDec pointer to set
				\sa VertexDec, CustomVertexElement, CreateVertexDeclaration
			 */
			virtual void SetVertexDeclaration(VertexDec* pVD) = 0;

			//! Draw a list of triangles
			/*! Draw a vertex buffer you must set the buffers manually before calling this function
				\param VCount The Vertices count.
				\param ICount The Indices count.
				\param VertexStart The vertex offset in VertexBuffwe
				\param StartIndex The index offset in IndexBuffer
				\sa SetVertexDeclaration, SetIndexBuffer, SetVertexBuffer
			*/
			virtual void DrawTriangleList(u32 VCount, u32 ICount,
										  u32 VertexStart = 0, u32 StartIndex = 0) = 0;

			//! Sets the transformation of World, View or Projection matrices
			/*!
				\param mat The Matrix to set
				\param TM The enum that define the TransformMode. Note you can not use ETM_ViewProjection here
				\sa GetTransform
			 */
			virtual void SetTransForm(math::Matrix *mat, TransformMode TM = ETM_World)=0;

			//! Returns the transformation of World, View, Projection or ViewProjection matrices
			/*!
			  \param TM The enum that define the TransformMode
			  \returns Returns the requested matrix
			  \sa SetTransform
			 */
			virtual math::Matrix GetTransForm(TransformMode TM = ETM_World)=0;

			//! Sets the clear color
			virtual void SetClearColor(const Color& ClearColor);

			//! Creates a texture from an image.
			/*!
			  \param pImg The Image resource pointer that you want to create texture from.
			  \returns Returns the created Texture pointer
			 */
			virtual Texture* CreateTexture(Image* pImg)=0;

			//! Returns the D3D device if the renderer is D3D
			/*!
				\returns Returns LPDIRECT3DDEVICE9 if renderer plugin is d3d9 and 
				returns ID3D11DeviceContext pointer if renderer plugin is d3d11
				otherwise returns NULL.
			 */
			virtual void* GetDirect3dDevice() {return NULL;}

			//! Returns the Renderer pointer
			static Renderer* GetPointer();

			//! Sets the texture
			virtual void SetTexture(Texture* pTex, int Stage = 0) = 0;

			//! Enable/Disable Scissor region
			/*!
				\sa SetScissorRegion
			 */
			virtual void EnableScissorRegion(bool enable) = 0;

			//! Sets the scissor region
			/*!
				\sa EnableScissorRegion
			 */
			virtual void SetScissorRegion(int x, int y, int width, int height) = 0;

		protected:

			bool			m_bUseStencil,			//!< Create and use stencil buffer
							m_bUseShaders,			//!< Dose renderer support shaders?
							m_bIsSceneRunning,		//!< Is scene running
							m_bOnRelease;			//!< Is renderer on release state
			bool			m_bEnScissor;			//!< Scissor region is enabled or not \sa EnableScissorRegion
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
			Color			m_cClearColor;			//!< The clear color
			core::DynamicArray<HardwareBuffer*>
							m_vBuffers;				//!< The created buffer pointers
			core::DynamicArray<VertexDec*>
							m_vVertexDecs;			//!< The created VertexDec pointers

			// Resource managers

			//! Adds hardware buffer pointer
			virtual void AddHardwareBuffer(HardwareBuffer* pHB);

			//! Removes hardware buffer pointer
			virtual void RemoveHardwareBuffer(HardwareBuffer* pHB);

			//! Adds VertexDec pointers
			virtual void AddVertexDec(VertexDec* pVD);

		}; // Renderer

	} // gfx

} // kge

#endif // KGE_RENDERER_H
