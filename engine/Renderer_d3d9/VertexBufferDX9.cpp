#include "VertexBuferDX9.h"
#include "RendererD3D9.h"

KGE_IMPORT extern kge::u32			HardwareBufferID;
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;

namespace kge
{
	namespace gfx
	{

		//----------------------------------------------------------------------------------------
		// Constructor
		//----------------------------------------------------------------------------------------
		VertexBufferDX9::VertexBufferDX9(u32 vcount, u32 stride, u32 format) :
		HardwareBuffer(vcount, stride, HardwareBufferID++), m_pVB(NULL), m_format(format)
		{

		} // Constructor

		//----------------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------------
		VertexBufferDX9::~VertexBufferDX9()
		{
			if (m_pVB) m_pVB->Release();

			if (m_bDynamic)
				reinterpret_cast<RendererD3D9*>(g_pRenderer)->RemoveVertexBuffer(this);

		} // Destructor

		//------------------------------------------------------------------------------------
		// sets the buffer data if buffer is dynamic.
		//------------------------------------------------------------------------------------
		bool VertexBufferDX9::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
		{
			if (!m_bDynamic || !m_pVB)
				return false;

			void* Buffer = 0;

			if (FAILED(m_pVB->Lock(Offset * m_iStride, iSize  * m_iStride, &Buffer, flags)))
				return false;

			if (iSize == 0)
				iSize = m_iDataCount;

			memcpy(Buffer, Data, iSize * m_iStride);

			m_pVB->Unlock();

			return true;

		} // SetData

		//------------------------------------------------------------------------------------
		// Locks vertex buffer
		//------------------------------------------------------------------------------------
		bool VertexBufferDX9::Lock(u32 offset, u32 size, void** ppData, ul32 flags)
		{
			if (!m_bDynamic || !m_pVB)
				return false;

			if (SUCCEEDED(m_pVB->Lock(offset, size, ppData, flags)))
			{
				return true;
			}
			return false;

		} // Lock

		//------------------------------------------------------------------------------------
		// Unlocks vertex buffer
		//------------------------------------------------------------------------------------
		bool VertexBufferDX9::Unlock()
		{
			if (SUCCEEDED(m_pVB->Unlock()))
			{
				return true;
			}
			return false;

		} // Unlock

	} // gfx

} // kge
