#include "../include/Renderer.h"

namespace kge
{
	namespace gfx
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Renderer::Renderer() : m_bUseStencil(false), m_bUseShaders(false), 
			m_bIsSceneRunning(false), m_iFPS(0), m_iFPSTemp(0), m_nIndexBufferID(MAXID),
			m_nVertexDecID(MAXID)
		{
			// Set texture and vertex IDs to MAXID(NULL)
			for (int i = 0; i < 8; i++)
			{
				m_nTextID[i]		 = MAXID;
				m_nVertexBufferID[i] = MAXID;
			}
			for (int i = 8; i < 16; i++)
			{
				m_nVertexBufferID[i] = MAXID;
			}

		} // Constructor

	} // gfx

} // kge
