#include "../include/Renderer.h"
#include "../include/HardwareBuffer.h"
#include "../include/VertexDeclaration.h"
#include "../include/Resource.h"

KGE_API kge::u32			HardwareBufferID = 0;

//! Renderer public pointer
extern kge::gfx::Renderer*	g_pRenderer;

namespace kge
{
	namespace gfx
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Renderer::Renderer() : m_bUseStencil(false), m_bUseShaders(false),
			m_bIsSceneRunning(false), m_iFPS(0), m_iFPSTemp(0), m_nIndexBufferID(MAXID),
			m_nVertexDecID(MAXID), m_iBatchCount(1), m_bEnScissor(false)
		{
			// Set texture and vertex IDs to MAXID(NULL)
			for (int i = 0; i < 8; i++)
				m_nTextID[i]		 = MAXID;
			for (int i = 0; i < 16; i++)
				m_nVertexBufferID[i] = MAXID;

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Renderer::~Renderer()
		{
			// Delete buffers
			for (size_t i = 0; i < m_vBuffers.size(); i++)
			{
				KGE_DELETE(m_vBuffers[i], HardwareBuffer);
			}

			// Delete vertex decs
			for (size_t i = 0; i < m_vVertexDecs.size(); i++)
			{
				KGE_DELETE(m_vVertexDecs[i], VertexDec);
			}

		} // Destructor

		//------------------------------------------------------------------------------------
		// Sets the clear color
		//------------------------------------------------------------------------------------
		void Renderer::SetClearColor( const Color& ClearColor )
		{
			m_cClearColor = ClearColor;

		} // SetClearColor

		//------------------------------------------------------------------------------------
		// Adds hardware buffer pointers
		//------------------------------------------------------------------------------------
		void Renderer::AddHardwareBuffer( HardwareBuffer* pHB )
		{
			m_vBuffers.push_back(pHB);

		} // AddHardwareBuffer

		//------------------------------------------------------------------------------------
		// Adds VertexDec pointers
		//------------------------------------------------------------------------------------
		void Renderer::AddVertexDec( VertexDec* pVD )
		{
			m_vVertexDecs.push_back(pVD);

		} // AddVertexDec

		//------------------------------------------------------------------------------------
		// Removes hardware buffer pointer
		//------------------------------------------------------------------------------------
		void Renderer::RemoveHardwareBuffer( HardwareBuffer* pHB )
		{
			int id = pHB->GetID();
			for (int i = 0; i < m_vBuffers.size(); i++)
			{
				if (m_vBuffers[i]->GetID() == id)
				{
					m_vBuffers.erase(i);
					return;
				}
			}

		} // RemoveHardwareBuffer

		//------------------------------------------------------------------------------------
		// Returns the Renderer pointer
		//------------------------------------------------------------------------------------
		Renderer* Renderer::GetPointer()
		{
			return g_pRenderer;

		} // GetPointer

	} // gfx

} // kge
