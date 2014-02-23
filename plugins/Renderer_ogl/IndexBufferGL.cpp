#include "IndexBufferGL.h"
#include "../../Libs/glew/glew.h"
#include "../../Include/gfx/Renderer.h"

KGE_IMPORT extern kge::gfx::Renderer	*	g_pRenderer;
KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	IndexBufferGL::IndexBufferGL( u32 vCount,u32 stride ):
		HardwareBuffer(vCount, stride, HardwareBufferID++)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	IndexBufferGL::~IndexBufferGL()
	{
		glDeleteBuffers(1, &m_uID);

	} // Destructor

	//------------------------------------------------------------------------------------
	// sets the buffer data
	//------------------------------------------------------------------------------------
	bool IndexBufferGL::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
	{
		Bind();
		GLenum staticflag = GL_STATIC_DRAW;
		if (m_bDynamic)
			staticflag = GL_DYNAMIC_DRAW;
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize * m_iStride, Data, staticflag);

		return true;

	} // SetData

	//------------------------------------------------------------------------------------
	// Binds the index buffer
	//------------------------------------------------------------------------------------
	void IndexBufferGL::Bind()
	{
		g_pRenderer->SetIndexBufferID(m_iID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uID);

	} // Bind

} // gfx

} // kge
