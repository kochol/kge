#include "../../Libs/glew/glew.h"
#include "VertexBufferGL.h"
#include "../../include/kge/gfx/Renderer.h"

KGE_IMPORT extern kge::gfx::Renderer	*	g_pRenderer;
KGE_IMPORT extern kge::u32 HardwareBufferID;

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	VertexBufferGL::VertexBufferGL( u32 vCount,u32 stride, bool isDynamic ):
		HardwareBuffer(vCount, stride, HardwareBufferID++)
	{
		m_bDynamic = isDynamic;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	VertexBufferGL::~VertexBufferGL()
	{
		glDeleteBuffers(1, &m_uID);

	} // Destructor

	//------------------------------------------------------------------------------------
	// sets the buffer data
	//------------------------------------------------------------------------------------
	bool VertexBufferGL::SetData( void* Data, u32 Offset, u32 iSize, ul32 flags )
	{
		if (!m_bDynamic)
			return false;

		Bind(0);
		GLenum staticflag = GL_STATIC_DRAW;
		if (m_bDynamic)
			staticflag = GL_DYNAMIC_DRAW;
		glBufferData(GL_ARRAY_BUFFER, iSize * m_iStride, Data, staticflag);

		return true;

	} // SetData

	//------------------------------------------------------------------------------------
	// Binds the vertex buffer
	//------------------------------------------------------------------------------------
	void VertexBufferGL::Bind(int stage)
	{
		g_pRenderer->SetVertexBufferID(0, stage);
		glBindBuffer(GL_ARRAY_BUFFER, m_uID);

	} // Bind

} // gfx

} // kge
