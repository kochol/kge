// File name: IndexBufferDX.h
// Des: This class is interface for working with index buffers in Direct3D 9.
// Date: 19/09/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_INDEXBUFFERDX_H
#define KGE_INDEXBUFFERDX_H

#include "../../Include/gfx/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{
namespace gfx
{
//! This class is interface for working with index buffers.
class IndexBufferDX: public HardwareBuffer
{
public:

	//! Constructor
	IndexBufferDX(u32 vcount, u32 stride, D3DFORMAT format);

	//! Destructor
	~IndexBufferDX();

	//! sets the buffer data if buffer is dynamic.
	bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags);

	//! Returns the type
	HardwareBufferType GetType() const { return EHBT_IndexBuffer; }

    //! Locks index buffer
    bool Lock(u32 offset, u32 size, void** ppData, ul32 flags);

    //! Unlocks index buffer
    bool Unlock();

    //!
    D3DFORMAT GetFormat() const { return m_format; }

	//! Index buffer pointer
	IDirect3DIndexBuffer9*		m_pIB;

private:
    D3DFORMAT                   m_format;

}; // IndexBufferDX

} // gfx

} // kge

#endif // KGE_INDEXBUFFERDX_H
