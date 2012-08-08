#include "VertexBufferOGLvbo.h"
#include "initGL.h"

extern kge::u32				HardwareBufferID;

namespace kge
{
    namespace gfx
    {

		//------------------------------------------------------------------------------------
        // Constructor
		//------------------------------------------------------------------------------------
        VertexBufferOGLvbo::VertexBufferOGLvbo(u32 vcount, u32 stride, uint bufID):
            HardwareBuffer(vcount, stride, HardwareBufferID++), m_iBufID(bufID)
        {

        } // Constructor

		//------------------------------------------------------------------------------------
        // Destructor
		//------------------------------------------------------------------------------------
        VertexBufferOGLvbo::~VertexBufferOGLvbo()
        {
            glDeleteBuffers(1, &m_iBufID);

        } // Destructor

		//------------------------------------------------------------------------------------
        // sets the buffer data if buffer is dynamic.
		//------------------------------------------------------------------------------------
        bool VertexBufferOGLvbo::SetData(void* Data, u32 Offset, u32 iSize, ul32 flags)
        {
            if (!m_bDynamic)
                return false;



            return true;

        } // SetData

		//------------------------------------------------------------------------------------
        // Locks vertex buffer
		//------------------------------------------------------------------------------------
        bool VertexBufferOGLvbo::Lock(u32 offset, u32 size, void** ppData, ul32 flags)
        {
            if (!m_bDynamic)
                return false;

            return true;

        } // Lock

		//------------------------------------------------------------------------------------
        // Unlocks vertex buffer
		//------------------------------------------------------------------------------------
        bool VertexBufferOGLvbo::Unlock()
        {
            if (!m_bDynamic)
                return false;

            return true;

        } // Unlock

    } // gfx

} // kge
