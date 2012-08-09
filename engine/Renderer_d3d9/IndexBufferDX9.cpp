#include "IndexBufferDX9.h"
#include "RendererD3D9.h"

KGE_IMPORT extern kge::u32				HardwareBufferID;
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;

namespace kge
{
	namespace gfx
	{

		//----------------------------------------------------------------------------------------
		// Constructor
		//----------------------------------------------------------------------------------------
		IndexBufferDX9::IndexBufferDX9(u32 vcount, u32 stride, D3DFORMAT format) :
			HardwareBuffer(vcount, stride, HardwareBufferID++), m_pIB(NULL), m_format(format)
		{

		} // Constructor

		//----------------------------------------------------------------------------------------
		// Destructor
		//----------------------------------------------------------------------------------------
		IndexBufferDX9::~IndexBufferDX9()
		{
			if (m_pIB) m_pIB->Release();

			if (m_bDynamic)
				reinterpret_cast<RendererD3D9*>(g_pRenderer)->RemoveIndexBuffer(this);

		} // Destructor

		bool IndexBufferDX9::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
		{
			if (!m_bDynamic || !m_pIB)
				return false;

			void* Buffer = 0;

			if (FAILED(m_pIB->Lock(Offset * m_iStride, iSize * m_iStride, &Buffer, flags)))
				return false;

			if (iSize == 0)
				iSize = m_iDataCount;

			memcpy(Buffer, Data, iSize * m_iStride);

			m_pIB->Unlock();

			return true;
		} // SetData

		bool IndexBufferDX9::Lock(u32 offset, u32 size, void** ppData, ul32 flags)
		{
			if (!m_bDynamic || !m_pIB)
				return false;

			if (FAILED(m_pIB->Lock(offset, size, ppData, flags)))
			{
				return false;
			}
			return true;
		} // Lock

		bool IndexBufferDX9::Unlock()
		{
			if (FAILED(m_pIB->Unlock()))
			{
				return false;
			}
			return true;
		} // Unlock

	} // gfx

} // kge
