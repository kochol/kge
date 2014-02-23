// File name: VertexBufferGL.h
// Des: This class is interface for working with vertex buffer objects in OpenGL.
// Date: 16/05/1389 (jalali)
// Programmer: Pooya Shahinfar (Pswin)

#ifndef _VERTEXBUFFERGL_H
#define _VERTEXBUFFERGL_H

#include "../../Include/kgedef.h"

#include "../../Include/gfx/HardwareBuffer.h"

namespace kge
{
	namespace gfx
	{
		class VertexBufferGL : public HardwareBuffer 
		{
		public:
			//! Constructor
			VertexBufferGL(u32 vCount,u32 stride, bool isDynamic);

			//! Destructor
			~VertexBufferGL();

			//! sets the buffer data
			bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags = 0);

			//! Returns the type
			HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

			//! Binds the vertex buffer
			void Bind(int stage);

			//! OpenGL Vertex buffer object ID
			u32 m_uID;

		} ; // VertexBufferGL
	}
} // kge

#endif // _VERTEXBUFFERGL_H
