#ifndef KGE_DYNAMICHARDWAREBUFFER_H
#define KGE_DYNAMICHARDWAREBUFFER_H

#include "HardwareBuffer.h"

namespace kge
{
namespace gfx
{

/// Dynamic buffer for working with buffer witch to need change many times before rendering
class DynamicHardwareBuffer : public HardwareBuffer
{
public:

	//! Constructor
	DynamicHardwareBuffer(u32 vcount, u32 stride, u32 format, u32 IncreaseRate);

	//! Destructor
	~DynamicHardwareBuffer();

	/*! sets the buffer data if buffer is dynamic.
	\param Data The data pointer
	\param Offset The offset in data count for example if Offset is 2 and it is a vertex buffer the offset means two vertex size not two byte
	\param iSize The size in data count not in bytes
	*/
	bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags = 0);

	//! Returns the hardware buffer type
	HardwareBufferType GetType() const;

	//! Append data to buffer
	bool AppendData(void* data, u32 count);

	//! Commit changes to buffer
	bool Commit();

	//! Returns the buffer
	HardwareBuffer* GetBuffer() {return m_pBuffer;}

protected:

	u8*					m_pTempData;
	u32				    m_filledBytes;
	u32			        m_verticesSize;
	u32			        m_offsetToLock;
	u32					m_Vcount;
	u32					m_IncreaseRate;
	u32					m_Stride;

	HardwareBuffer	*	m_pBuffer;

private:

}; // DynamicHardwareBuffer

} // gfx

} // kge

#endif // KGE_DYNAMICHARDWAREBUFFER_H
