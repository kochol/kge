#include "../../include/kge/gfx/DynamicHardwareBuffer.h"
#include "../../include/kge/gfx/Renderer.h"

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	DynamicHardwareBuffer::DynamicHardwareBuffer( u32 vcount, u32 stride, u32 format, 
		u32 IncreaseRate ): m_filledBytes(0), m_verticesSize(vcount), m_offsetToLock(0),
		m_IncreaseRate(IncreaseRate), m_Stride(stride), m_Vcount(0)
	{
		m_pBuffer   = Renderer::GetSingletonPtr()->CreateVertexBuffer(NULL, vcount, stride, true);
		m_pTempData = (u8*) malloc(vcount * stride);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	DynamicHardwareBuffer::~DynamicHardwareBuffer()
	{
		kgedecref(m_pBuffer);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Append data to buffer
	//------------------------------------------------------------------------------------
	bool DynamicHardwareBuffer::AppendData( void* data, u32 count )
	{
		if (m_verticesSize < m_offsetToLock + count)
		{
			m_verticesSize += m_IncreaseRate;
			kgedecref(m_pBuffer);
			m_pBuffer		= Renderer::GetSingletonPtr()->CreateVertexBuffer(NULL, m_verticesSize, m_Stride, true);
			m_pTempData		= (u8*)realloc(m_pTempData, m_verticesSize * m_Stride);
		}

		// copy data
		memcpy(&m_pTempData[m_offsetToLock], data, count * m_Stride);
		m_offsetToLock += count * m_Stride;
		m_Vcount       += count;

		return true;

	} // AppendData

	//------------------------------------------------------------------------------------
	// Commit changes to buffer
	//------------------------------------------------------------------------------------
	bool DynamicHardwareBuffer::Commit()
	{
		m_pBuffer->SetData(m_pTempData, 0, m_Vcount);
		m_offsetToLock = 0;
		m_Vcount       = 0;

		return true;

	} // Commit

	bool DynamicHardwareBuffer::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags /*= 0*/ )
	{
		return m_pBuffer->SetData(Data, Offset, iSize, flags);

	}

	kge::gfx::HardwareBufferType DynamicHardwareBuffer::GetType() const
	{
		return m_pBuffer->GetType();

	}

} // gfx

} // kge
