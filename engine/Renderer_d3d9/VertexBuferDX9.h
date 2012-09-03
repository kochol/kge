#ifndef VERTEXBUFFERDX9_H
#define VERTEXBUFFERDX9_H

#include "../include/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{
	namespace gfx
	{
		//! This class is interface for working with vertex buffers.
		class VertexBufferDX9: public HardwareBuffer
		{
		public:

			//! Constructor
			VertexBufferDX9(u32 vcount, u32 stride, u32 format);

			//! Destructor
			~VertexBufferDX9();

			//! sets the buffer data if buffer is dynamic.
			bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags);

			//! Returns the type
			HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

			//! Locks vertex buffer
			bool Lock(u32 offset, u32 size, void** ppData, ul32 flags);

			//! Unlocks vertex buffer
			bool Unlock();

			//!
			uint GetFormat() const { return m_format; }

			//! Vertex buffer pointer
			IDirect3DVertexBuffer9*		m_pVB;

		private:

			uint                m_format;

		}; // VertexBufferDX

	} // gfx

} // kge

#endif // VERTEXBUFFERDX9_H
