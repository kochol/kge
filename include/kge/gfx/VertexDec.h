// File name: VertexDec.h
// Des: This class has information about vertex declaration.
// Date: 13/10/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_VERTEXDEC_H
#define KGE_VERTEXDEC_H

#include "../kgedef.h"
#include "../core/String.h"

namespace kge
{
namespace gfx
{
//! Custom vertex pointer
typedef void* CustomVertex;

//! Custom vertex declaration
class KGE_API VertexDec
{
public:

	//! Constructor
	VertexDec(core::stringc& sName);

	//! Destructor
	~VertexDec();

	//! Returns the vertex declaration ID
	u32 GetID() { return m_iUID; }

	//! Returns the vertex declaration name
	const core::stringc& GetName() const { return m_sName; }

	CustomVertex			m_VertexDec;	//!< For internal use

protected:

	u32						m_iUID;			//!< Unique ID
	core::stringc			m_sName;		//!< The vertex declaration name

}; // VertexDec

} // gfx

} // kge

#endif // KGE_VERTEXDEC_H
