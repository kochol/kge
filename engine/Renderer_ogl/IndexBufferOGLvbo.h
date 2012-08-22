#ifndef KGE_INDEXBUFFEROGLVBO_H
#define KGE_INDEXBUFFEROGLVBO_H

#include "../include/HardwareBuffer.h"

namespace kge
{
	namespace gfx
	{
		class IndexBufferOGLvbo : public HardwareBuffer
		{
		public:

			//! Constructor
			IndexBufferOGLvbo(u32 vcount, u32 stride, uint bufID);

			//! Destructor
			~IndexBufferOGLvbo();

			//! sets the buffer data if buffer is dynamic.
			bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags);

			//! Returns the type
			HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

			//! Locks vertex buffer
			bool Lock(u32 offset, u32 size, void** ppData, ul32 flags);

			//! Unlocks vertex buffer
			bool Unlock();

		//protected:

			uint        m_iBufID;

		}; // IndexBufferOGLvbo
	}
}

#endif // KGE_INDEXBUFFEROGLVBO_H
