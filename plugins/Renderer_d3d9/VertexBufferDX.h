// File name: VertexBufferDX.h
// Des: This class is interface for working with vertex buffers.
// Date: 02/08/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef VERTEXBUFFERDX_H
#define VERTEXBUFFERDX_H

#include "../../include/kge/gfx/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{
namespace gfx
{
//! This class is interface for working with vertex buffers.
class VertexBufferDX: public HardwareBuffer
{
public:

	//! Constructor
	VertexBufferDX(u32 vcount, u32 stride, u32 format);

	//! Destructor
	~VertexBufferDX();

	//! sets the buffer data if buffer is dynamic.
	bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags);

	//! Returns the type
	HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

    //! Locks vertex buffer
    bool Lock(u32 offset, u32 size, void** ppData, ul32 flags);

    //! Unlocks vertex buffer
    bool Unlock();

    //!
    u32 GetFormat() const { return m_format; }

	//! Vertex buffer pointer
	IDirect3DVertexBuffer9*		m_pVB;

private:
    unsigned int                m_format;

}; // VertexBufferDX

} // gfx

} // kge

#endif // VERTEXBUFFERDX_H
