#ifndef KGE_VERTEXELEMENT_H
#define KGE_VERTEXELEMENT_H

#include "../kgedef.h"

namespace kge
{
	namespace gfx
	{
		//! Use an array of CustomVertexElement to create custom vertices.
		struct CustomVertexElement
		{
			u16    Stream;     //*< Stream index
			u16    Offset;     //*< Offset in the stream in bytes
			u8     Type;       //*< Data type
			u8     Method;     //*< Processing method
			u8     Usage;      //*< Semantics
			u8     UsageIndex; //*< Semantic index

		}; // CustomVertexElement

		//! Vertex Element Type enum
		enum VertexElementType
		{
			EVET_Float1    =  0,  //*< 1D float expanded to (value, 0., 0., 1.)
			EVET_Float2    =  1,  //*< 2D float expanded to (value, value, 0., 1.)
			EVET_Float3    =  2,  //*< 3D float expanded to (value, value, value, 1.)
			EVET_Float4    =  3,  //*< 4D float
			EVET_Color     =  4,  //*< 4D packed unsigned bytes mapped to 0. to 1. range

			EVET_UByte4    =  5,  //*< 4D unsigned byte
			EVET_Short2    =  6,  // 2D signed short expanded to (value, value, 0., 1.)
			EVET_Short4    =  7,  // 4D signed short

		// The following types are valid only with vertex shaders >= 2.0

			EVET_UByte4N   =  8,  //*< Each of 4 bytes is normalized by dividing to 255.0
			EVET_Short2N   =  9,  //*< 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
			EVET_Short4N   = 10,  //*< 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
			EVET_UShort2N  = 11,  //*< 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
			EVET_UShort4N  = 12,  //*< 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
			EVET_UDec3     = 13,  //*< 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
			EVET_Dec3N     = 14,  //*< 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
			EVET_Float16_2 = 15,  //*< Two 16-bit floating point values, expanded to (value, value, 0, 1)
			EVET_Float16_4 = 16,  //*< Four 16-bit floating point values
			EVET_Unused    = 17,  //*< When the type field in a decl is unused.

		}; // VertexElementType

		//! Vertex Element Method enum
		enum VertexElementMethod
		{
			EVEM_Default = 0,
			EVEM_PartialU = 1,
			EVEM_PartialV = 2,
			EVEM_CrossUV = 3,
			EVEM_UV = 4,
			EVEM_LookUp = 5,
			EVEM_LookUpPreSampled = 6,

		}; // VertexElementMethod

		//! Vertex Element Usage enum
		enum VertexElementUsage
		{
			EVEU_Position = 0,
			EVEU_BlendWeight,   // 1
			EVEU_BlendIndices,  // 2
			EVEU_Normal,        // 3
			EVEU_PSize,         // 4
			EVEU_TexCoord,      // 5
			EVEU_Tangent,       // 6
			EVEU_Binormal,      // 7
			EVEU_TessFactor,    // 8
			EVEU_PositionT,     // 9
			EVEU_Color,         // 10
			EVEU_Fog,           // 11
			EVEU_Depth,         // 12
			EVEU_Sample,        // 13

		}; // VertexElementUsage

		#define CusVertexEND() {0xFF,0,kge::gfx::EVET_Unused,0,0,0}

	} // gfx

} // kge

#endif // KGE_VERTEXELEMENT_H
