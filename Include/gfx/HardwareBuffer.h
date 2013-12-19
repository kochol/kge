// File name: HardwareBuffer.h
// Des: This class is interface for working with graphics card hardware buffers.
// Date: 19/09/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_HARDWAREBUFFER_H
#define KGE_HARDWAREBUFFER_H

#include "../KgeUnknown.h"

namespace kge
{
namespace gfx
{
//! Hardware buffers types
enum HardwareBufferType
{
	EHBT_VertexBuffer = 0,
	EHBT_IndexBuffer,

}; // HardwareBufferType

//! Index Buffer type 16 bit or 32 bit
enum IndexBufferType
{
	EIBT_16Bit = 0,
	EIBT_32Bit			// For Large models

}; // IndexBufferType

class HardwareBuffer : public KgeUnknown
{
public:

	//! Default constructor
	HardwareBuffer(): m_bDynamic(false), m_iDataCount(0), m_iStride(0), m_iID(MAXID) {}

	//! Constructor
	HardwareBuffer(u32 count, u32 stride, u32 id) : m_bDynamic(false), m_iDataCount(count), m_iStride(stride), m_iID(id) {}

	//! Destructor
	virtual ~HardwareBuffer() {}

	/*! sets the buffer data if buffer is dynamic.
	\param Data The data pointer
	\param Offset The offset in data count for example if Offset is 2 and it is a vertex buffer the offset means two vertex size not two byte
	\param iSize The size in data count not in bytes
	*/
	virtual bool SetData(void* Data, u32 Offset, u32 iSize, ul32 flags = 0) = 0;

	//! Returns the hardware buffer type
	virtual HardwareBufferType GetType() const = 0;

	//!
	bool Dynamic() const { return m_bDynamic; }

	//!
	void Dynamic(bool val) { m_bDynamic = val; }

	//! Returns this hardware buffer unique ID
	u32 GetID() const { return m_iID; }

	//! Returns this buffer stride.
	u32 GetStride() const { return m_iStride; }

	u32 GetDataCount() const { return m_iDataCount; }

	//! Locks buffer
	virtual bool Lock(u32 offset, u32 size, void** ppData, ul32 flags) { return true;}

	//! Unlocks buffer
	virtual bool Unlock() { return true;}

protected:

	bool	m_bDynamic;

	u32		m_iDataCount,
			m_iStride,
			m_iID;

}; // HardwareBuffer

} // gfx

} // kge

#endif // KGE_HARDWAREBUFFER_H
