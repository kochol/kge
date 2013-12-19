// File name: DynamicIndexBufferDX.cpp
// Des: 
// Date: April 20, 2011
// Programmer: Nader Golbaz

#include <utility>
#include "DynamicIndexBufferDX.h"

KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{

namespace gfx
{

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicIndexBufferDX::DynamicIndexBufferDX(u32 length, u32 stride, D3DFORMAT format)
        : HardwareBuffer(length, stride, HardwareBufferID++),
        m_pIB(0), m_format(format), m_pTempData(KGE_NEW_ARRAY(u8, length)),
        m_filledBytes(0), m_indicesSize(length * stride),
        m_offsetToLock(0), m_startIndex(0)
    {

    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicIndexBufferDX::DynamicIndexBufferDX(const DynamicIndexBufferDX& other)
        : HardwareBuffer(other.m_iDataCount, other.m_iStride, /*other.m_iID*/HardwareBufferID++),
        m_pIB(/*other.m_pIB*/0), m_format(other.m_format), m_pTempData(KGE_NEW_ARRAY(u8, other.m_iDataCount)),
        m_filledBytes(0), m_indicesSize(other.m_indicesSize),
        m_offsetToLock(other.m_offsetToLock), m_startIndex(other.m_startIndex)
    {
        //if (m_pIB) m_pIB->AddRef();
    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicIndexBufferDX::~DynamicIndexBufferDX()
    {
        if (m_pIB)
            m_pIB->Release();
        delete[] m_pTempData;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    DynamicIndexBufferDX& DynamicIndexBufferDX::operator=(const DynamicIndexBufferDX& other)
    {
        DynamicIndexBufferDX temp(other);
        this->Swap(temp);
        return *this;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    bool DynamicIndexBufferDX::SetData(void* data, u32 offset, u32 iSize, ul32 flags /* = 0 */)
    {
        const u32 len = iSize * m_iStride;

        if (m_filledBytes + len > m_indicesSize)
        {
            // We don't have enough space, index buffer should be recreated with more length.
            return false;
        }

        memcpy(m_pTempData + m_filledBytes, data, len);
        m_filledBytes += len;

        return true;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    bool DynamicIndexBufferDX::CommitChanges()
    {
        DWORD flag = D3DLOCK_NOOVERWRITE;

        if (m_filledBytes > m_indicesSize - m_offsetToLock)
        {
            m_offsetToLock = 0;
            flag = D3DLOCK_DISCARD;
        }

        void* buffer = 0;
        if (FAILED(m_pIB->Lock(m_offsetToLock, m_filledBytes, &buffer, flag)))
            return false;

        memcpy(buffer, m_pTempData, m_filledBytes);
        m_pIB->Unlock();

        m_startIndex = m_offsetToLock;
        m_offsetToLock += m_filledBytes;
        m_filledBytes = 0;

        return true;
    }

    //------------------------------------------
    // 
    //------------------------------------------
    void DynamicIndexBufferDX::Swap(DynamicIndexBufferDX& other)
    {
        std::swap(m_bDynamic, other.m_bDynamic);
        std::swap(m_iDataCount, other.m_iDataCount);
        std::swap(m_iStride, other.m_iStride);
        std::swap(m_iID, other.m_iID);
        std::swap(m_pIB, other.m_pIB);
        std::swap(m_format, other.m_format);
        std::swap(m_pTempData, other.m_pTempData);
        std::swap(m_filledBytes, other.m_filledBytes);
        std::swap(m_indicesSize, other.m_indicesSize);
        std::swap(m_offsetToLock, other.m_offsetToLock);
        std::swap(m_startIndex, other.m_startIndex);
    }

} // gfx

} // kge
