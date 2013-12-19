// File name: DynamicVertexBufferDX.cpp
// Des: 
// Date: April 19, 2011
// Programmer: Nader Golbaz

#include <utility>
#include "DynamicVertexBufferDX.h"

KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{

namespace gfx
{

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicVertexBufferDX::DynamicVertexBufferDX(u32 vcount, u32 stride, u32 format)
        : HardwareBuffer(vcount, stride, HardwareBufferID++),
        m_pVB(0), m_format(format), m_pTempData(KGE_NEW_ARRAY(u8, vcount)),
        m_filledBytes(0), m_verticesSize(vcount * stride),
        m_offsetToLock(0), m_startVertex(0)
    {

    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicVertexBufferDX::DynamicVertexBufferDX(const DynamicVertexBufferDX& other)
        : HardwareBuffer(other.m_iDataCount, other.m_iStride, /*other.m_iID*/HardwareBufferID++),
        m_pVB(/*other.m_pVB*/0), m_format(other.m_format), m_pTempData(KGE_NEW_ARRAY(u8, other.m_iDataCount)),
        m_filledBytes(0), m_verticesSize(other.m_verticesSize),
        m_offsetToLock(other.m_offsetToLock), m_startVertex(other.m_startVertex)
    {
        //if (m_pVB) m_pVB->AddRef();
    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicVertexBufferDX::~DynamicVertexBufferDX()
    {
        if (m_pVB)
            m_pVB->Release();
        KGE_DELETE_ARRAY(m_pTempData);
    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicVertexBufferDX& DynamicVertexBufferDX::operator=(const DynamicVertexBufferDX& other)
    {
        DynamicVertexBufferDX temp(other);
        this->Swap(temp);
        return *this;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    bool DynamicVertexBufferDX::SetData(void* data, u32 offset, u32 iSize, ul32 flags /* = 0 */)
    {
        const u32 len = iSize * m_iStride;

        if (m_filledBytes + len > m_verticesSize)
        {
            // We don't have enough space, vertex buffer should be recreated with more length.
            return false;
        }

        memcpy(m_pTempData + m_filledBytes, data, len);
        m_filledBytes += len;

        return true;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    bool DynamicVertexBufferDX::CommitChanges()
    {
        DWORD flag = D3DLOCK_NOOVERWRITE;

        if (m_filledBytes > m_verticesSize - m_offsetToLock)
        {
            m_offsetToLock = 0;
            flag = D3DLOCK_DISCARD;
        }

        void* buffer = 0;
        if (FAILED(m_pVB->Lock(m_offsetToLock, m_filledBytes, &buffer, flag)))
            return false;

        memcpy(buffer, m_pTempData, m_filledBytes);
        m_pVB->Unlock();

        m_startVertex = m_offsetToLock;
        m_offsetToLock += m_filledBytes;
        m_filledBytes = 0;

        return true;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    void DynamicVertexBufferDX::Swap(DynamicVertexBufferDX& other)
    {
        std::swap(m_bDynamic, other.m_bDynamic);
        std::swap(m_iDataCount, other.m_iDataCount);
        std::swap(m_iStride, other.m_iStride);
        std::swap(m_iID, other.m_iID);
        std::swap(m_pVB, other.m_pVB);
        std::swap(m_format, other.m_format);
        std::swap(m_pTempData, other.m_pTempData);
        std::swap(m_filledBytes, other.m_filledBytes);
        std::swap(m_verticesSize, other.m_verticesSize);
        std::swap(m_offsetToLock, other.m_offsetToLock);
        std::swap(m_startVertex, other.m_startVertex);
    }

} // gfx

} // kge
