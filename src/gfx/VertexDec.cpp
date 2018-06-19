// File name: VertexDec.cpp
// Des: This class has information about vertex declaration.
// Date: 13/10/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/gfx/VertexDec.h"



extern kge::u32 VertexDecID;

#ifndef NULL
#define NULL 0
#endif

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
