#ifndef KGE_VERTEXBUFFEROGLVBO_H
#define KGE_VERTEXBUFFEROGLVBO_H

#include "../include/HardwareBuffer.h"

namespace kge
{
    namespace gfx
    {
        class VertexBufferOGLvbo : public HardwareBuffer
        {
        public:

			//! Constructor
			VertexBufferOGLvbo(u32 vcount, u32 stride, uint bufID);

			//! Destructor
			~VertexBufferOGLvbo();

			//! sets the buffer data if buffer is dynamic.
			bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags);

			//! Returns the type
			HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

			//! Locks vertex buffer
			bool Lock(u32 offset, u32 size, void** ppData, ul32 flags);

			//! Unlocks vertex buffer
			bool Unlock();

        protected:

            uint        m_iBufID;

        }; // VertexBufferOGLvbo

    } // gfx

} // kge

#endif // KGE_VERTEXBUFFEROGLVBO_H
