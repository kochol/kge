#include "../include/VertexDeclaration.h"

kge::u32 VertexDecID;

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		VertexDec::VertexDec(core::stringc& sName):m_VertexDec(NULL), m_sName(sName)
		{
			m_iUID = VertexDecID++;

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		VertexDec::~VertexDec()
		{

		} // Destructor

	} // gfx

} // kge
