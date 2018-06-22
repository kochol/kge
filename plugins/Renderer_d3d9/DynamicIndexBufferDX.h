// File name: DynamicIndexBufferDX.h
// Des: 
// Date: April 20, 2011
// Programmer: Nader Golbaz

#ifndef DYNAMICINDEXBUFFERDX_H
#define DYNAMICINDEXBUFFERDX_H

#include "../../include/kge/gfx/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{

namespace gfx
{

    class DynamicIndexBufferDX : public HardwareBuffer
    {
    public:
        //! Constructor
        DynamicIndexBufferDX(u32 length, u32 stride, D3DFORMAT format);
        
        //! Copy Constructor
        DynamicIndexBufferDX(const DynamicIndexBufferDX& other);

        //! Destructor
        ~DynamicIndexBufferDX();

        //! Assignment Operator
        DynamicIndexBufferDX& operator =(const DynamicIndexBufferDX& other);

        //!
        bool SetData(void* data, u32 offset, u32 iSize, ul32 flags = 0);

        //!
        bool CommitChanges();

        //!
        void Swap(DynamicIndexBufferDX& other);

        //!
        HardwareBufferType GetType() const { return EHBT_IndexBuffer; }

        //!
        D3DFORMAT GetFormat() const { return m_format; }

        //!
        u32 GetStartIndex() const { return m_startIndex; }

        //! Index buffer pointer
        IDirect3DIndexBuffer9* m_pIB;

    private:
        D3DFORMAT       m_format;
        u8*             m_pTempData;
        u32             m_filledBytes;
        u32             m_indicesSize;
        u32             m_offsetToLock;
        u32             m_startIndex;
    };

} // gfx

} // kge

#endif // DYNAMICINDEXBUFFERDX_H
