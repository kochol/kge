// File name: VertexBufferDX.h
// Des: This class is interface for working with vertex buffers.
// Date: 02/08/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "VertexBufferDX.h"
#include "RendererDX.h"


KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{
namespace gfx
{

	//----------------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------------
	VertexBufferDX::VertexBufferDX(u32 vcount, u32 stride, u32 format) :
		HardwareBuffer(vcount, stride, HardwareBufferID++), m_pVB(NULL), m_format(format)
	{

	} // Constructor

	//----------------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------------
	VertexBufferDX::~VertexBufferDX()
	{
		if (m_pVB) m_pVB->Release();

		if (m_bDynamic)
			reinterpret_cast<RendererDX*>(Renderer::GetSingletonPtr())->RemoveVertexBuffer(this);

	} // Destructor

	bool VertexBufferDX::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
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

	bool VertexBufferDX::Lock(u32 offset, u32 size, void** ppData, ul32 flags)
	{
		if (!m_bDynamic || !m_pVB)
			return false;

		if (SUCCEEDED(m_pVB->Lock(offset, size, ppData, flags)))
		{
			return true;
		}
		return false;
	} // Lock

	bool VertexBufferDX::Unlock()
	{
		if (SUCCEEDED(m_pVB->Unlock()))
		{
			return true;
		}
		return false;
	} // Unlock

} // gfx

} // kge
