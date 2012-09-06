#ifndef INDEXBUFFERDX9_H
#define INDEXBUFFERDX9_H

#include "../include/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{
	namespace gfx
	{
		//! This class is interface for working with index buffers.
		class IndexBufferDX9: public HardwareBuffer
		{
		public:

			//! Constructor
			IndexBufferDX9(u32 vcount, u32 stride, D3DFORMAT format);

			//! Destructor
			~IndexBufferDX9();

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

		}; // IndexBufferDX9

	} // gfx

} // kge

#endif // INDEXBUFFERDX9_H
