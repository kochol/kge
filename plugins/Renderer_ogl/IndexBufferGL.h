#pragma once

#include "../../Include/kgedef.h"
#include "../../Include/gfx/HardWareBuffer.h"

namespace kge
{
namespace gfx
{
	class IndexBufferGL : public HardwareBuffer 
	{
	public:

		//! Constructor
		IndexBufferGL(u32 vCount,u32 stride);

		//! Destructor
		~IndexBufferGL();

		//! sets the buffer data
		bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags = 0);

		//! Returns the type
		HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

		//! Binds the index buffer
		void Bind();

		//! Vertex buffer object ID
		u32 m_uID;

	} ; // VertexBufferGL

} // gfx

} // kge
