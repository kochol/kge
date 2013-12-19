// File name: IndexBufferDX.cpp
// Des: This class is interface for working with index buffers in Direct3D 9.
// Date: 19/09/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "IndexBufferDX.h"
#include "RendererDX.h"


KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{
namespace gfx
{

	//----------------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------------
	IndexBufferDX::IndexBufferDX(u32 vcount, u32 stride, D3DFORMAT format) :
        HardwareBuffer(vcount, stride, HardwareBufferID++), m_pIB(NULL), m_format(format)
	{

	} // Constructor

	//----------------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------------
	IndexBufferDX::~IndexBufferDX()
	{
		if (m_pIB) m_pIB->Release();

        if (m_bDynamic)
            reinterpret_cast<RendererDX*>(Renderer::GetSingletonPtr())->RemoveIndexBuffer(this);

	} // Destructor

	bool IndexBufferDX::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
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

    bool IndexBufferDX::Lock(u32 offset, u32 size, void** ppData, ul32 flags)
    {
        if (!m_bDynamic || !m_pIB)
            return false;

        if (FAILED(m_pIB->Lock(offset, size, ppData, flags)))
        {
            return false;
        }
        return true;
    } // Lock

    bool IndexBufferDX::Unlock()
    {
        if (FAILED(m_pIB->Unlock()))
        {
            return false;
        }
        return true;
    } // Unlock

} // gfx

} // kge
