// File name: DynamicVertexBufferDX.h
// Des: 
// Date: April 19, 2011
// Programmer: Nader Golbaz

#ifndef DYNAMICVERTEXBUFFERDX_H
#define DYNAMICVERTEXBUFFERDX_H

#include "../../include/kge/gfx/HardwareBuffer.h"
#include <d3d9.h>

namespace kge
{

namespace gfx
{

    class DynamicVertexBufferDX : public HardwareBuffer
    {
    public:

        //! Constructor
        DynamicVertexBufferDX(u32 vcount, u32 stride, u32 format);
        
        //! Copy Constructor
        DynamicVertexBufferDX(const DynamicVertexBufferDX& other);

        //! Destructor
        ~DynamicVertexBufferDX();

        //! Assignment Operator
        DynamicVertexBufferDX& operator =(const DynamicVertexBufferDX& other);

        //!
        bool SetData(void* data, u32 offset, u32 iSize, ul32 flags = 0);

        //!
        bool CommitChanges();

        //!
        void Swap(DynamicVertexBufferDX& other);

        //!
        HardwareBufferType GetType() const { return EHBT_VertexBuffer; }

        //!
        u32 GetFormat() const { return m_format; }

        //!
        u32 GetStartVeretxIndex() const { return m_startVertex; }

        //! Vertex buffer pointer
        IDirect3DVertexBuffer9* m_pVB;

    private:
        u32             m_format;
        u8*             m_pTempData;
        u32             m_filledBytes;
        u32             m_verticesSize;
        u32             m_offsetToLock;
        u32             m_startVertex;
    };

} // gfx

} // kge

#endif // DYNAMICVERTEXBUFFERDX_H
