// File name: MeshManager.h
// Des: This class is for working with meshes and changing their vertex.
// Date: 15/5/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_MESHMANAGER_H
#define KGE_MESHMANAGER_H

#include "../kgedef.h"
#include "../Singleton.h"
#include "Structs.h"

namespace kge
{
namespace gfx
{
	//! Recalculate the normals.
	template<class VType>
	void RecalculateNormals_Smooth(VType* verts, u32 numVerts, u16* idx, u32 numIdx)
	{
		for(u32 i = 0; i < numVerts; i++)
		{
			verts[i].Nor.X = 0.0f;
			verts[i].Nor.Y = 0.0f;
			verts[i].Nor.Z = 0.0f;
		}

		Vertex3 e1, e2;
		math::Vector vN;

		for (u32 i = 0; i < numIdx; i += 3)
		{
			e1 = verts[idx[i + 1]].pos - verts[idx[i]].pos;
			e2 = verts[idx[i + 2]].pos - verts[idx[i]].pos;
			vN.Cross(e1, e2);
			vN.Normalize();
			verts[idx[i]].Nor.X += vN.x;
			verts[idx[i]].Nor.Y += vN.y;
			verts[idx[i]].Nor.Z += vN.z;
			verts[idx[i + 1]].Nor.X += vN.x;
			verts[idx[i + 1]].Nor.Y += vN.y;
			verts[idx[i + 1]].Nor.Z += vN.z;
			verts[idx[i + 2]].Nor.X += vN.x;
			verts[idx[i + 2]].Nor.Y += vN.y;
			verts[idx[i + 2]].Nor.Z += vN.z;
		}

		for(u32 i = 0; i < numVerts; i++)
		{
			verts[i].Nor.Normalize();
		}

	} // RecalculateNormals_Smooth

	//! Recalculate the normals.
	template<class VposType, class VnorType>
	void RecalculateNormals_Smooth(VposType* pos, VnorType* normals, u32 numVerts, u16* idx, u32 numIdx)
	{
		for(int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.X = 0.0f;
			normals[i].Nor.Y = 0.0f;
			normals[i].Nor.Z = 0.0f;
		}

		Vertex3 e1, e2;
		math::Vector vN;

		for (int i = 0; i < numIdx; i += 3)
		{
			e1 = pos[idx[i + 1]].pos - pos[idx[i]].pos;
			e2 = pos[idx[i + 2]].pos - pos[idx[i]].pos;
			vN.Cross(e1, e2);
			vN.Normalize();
			normals[idx[i]].Nor.X += vN.x;
			normals[idx[i]].Nor.Y += vN.y;
			normals[idx[i]].Nor.Z += vN.z;
			normals[idx[i + 1]].Nor.X += vN.x;
			normals[idx[i + 1]].Nor.Y += vN.y;
			normals[idx[i + 1]].Nor.Z += vN.z;
			normals[idx[i + 2]].Nor.X += vN.x;
			normals[idx[i + 2]].Nor.Y += vN.y;
			normals[idx[i + 2]].Nor.Z += vN.z;
		}

		for(int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.Normalize();
		}

	} // RecalculateNormals_Smooth

	//! Recalculate the normals.
	template<class VnorType>
	void RecalculateNormals_Smooth(Vertex3* pos, VnorType* normals, u32 numVerts, u16* idx, u32 numIdx)
	{
		for(int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.X = 0.0f;
			normals[i].Nor.Y = 0.0f;
			normals[i].Nor.Z = 0.0f;
		}

		Vertex3 e1, e2;
		math::Vector vN;

		for (int i = 0; i < numIdx; i += 3)
		{
			e1 = pos[idx[i + 1]] - pos[idx[i]];
			e2 = pos[idx[i + 2]] - pos[idx[i]];
			vN.Cross(e1, e2);
			vN.Normalize();
			normals[idx[i]].Nor.X += vN.x;
			normals[idx[i]].Nor.Y += vN.y;
			normals[idx[i]].Nor.Z += vN.z;
			normals[idx[i + 1]].Nor.X += vN.x;
			normals[idx[i + 1]].Nor.Y += vN.y;
			normals[idx[i + 1]].Nor.Z += vN.z;
			normals[idx[i + 2]].Nor.X += vN.x;
			normals[idx[i + 2]].Nor.Y += vN.y;
			normals[idx[i + 2]].Nor.Z += vN.z;
		}

		for(int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.Normalize();
		}

	} // RecalculateNormals_Smooth

	//! Recalculate the normals.
	template<class VnorType>
	void RecalculateNormals_Smooth32(Vertex3* pos, VnorType* normals, u32 numVerts, u32* idx, u32 numIdx)
	{
		for(unsigned int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.X = 0.0f;
			normals[i].Nor.Y = 0.0f;
			normals[i].Nor.Z = 0.0f;
		}

		Vertex3 e1, e2;
		math::Vector vN;

		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			e1 = pos[idx[i + 1]] - pos[idx[i]];
			e2 = pos[idx[i + 2]] - pos[idx[i]];
			vN.Cross(e1, e2);
			vN.Normalize();
			normals[idx[i]].Nor.X += vN.x;
			normals[idx[i]].Nor.Y += vN.y;
			normals[idx[i]].Nor.Z += vN.z;
			normals[idx[i + 1]].Nor.X += vN.x;
			normals[idx[i + 1]].Nor.Y += vN.y;
			normals[idx[i + 1]].Nor.Z += vN.z;
			normals[idx[i + 2]].Nor.X += vN.x;
			normals[idx[i + 2]].Nor.Y += vN.y;
			normals[idx[i + 2]].Nor.Z += vN.z;
		}

		for(unsigned int i = 0; i < numVerts; i++)
		{
			normals[i].Nor.Normalize();
		}

	} // RecalculateNormals_Smooth32

	//! Recalculate the normals.
	inline void RecalculateNormals_Smooth(Vertex3* pos, Vertex3T* NorTex, u32 numVerts, u16* idx, u32 numIdx)
	{
		for(unsigned int i = 0; i < numVerts; i++)
		{
			NorTex[i].pos.X = 0.0f;
			NorTex[i].pos.Y = 0.0f;
			NorTex[i].pos.Z = 0.0f;
		}

		Vertex3 e1, e2;
		math::Vector vN;

		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			e1 = pos[idx[i + 1]] - pos[idx[i]];
			e2 = pos[idx[i + 2]] - pos[idx[i]];
			vN.Cross(e1, e2);
			vN.Normalize();
			NorTex[idx[i]].pos.X += vN.x;
			NorTex[idx[i]].pos.Y += vN.y;
			NorTex[idx[i]].pos.Z += vN.z;
			NorTex[idx[i + 1]].pos.X += vN.x;
			NorTex[idx[i + 1]].pos.Y += vN.y;
			NorTex[idx[i + 1]].pos.Z += vN.z;
			NorTex[idx[i + 2]].pos.X += vN.x;
			NorTex[idx[i + 2]].pos.Y += vN.y;
			NorTex[idx[i + 2]].pos.Z += vN.z;
		}

		for(unsigned int i = 0; i < numVerts; i++)
		{
			NorTex[i].pos.Normalize();
		}

	} // RecalculateNormals_Smooth

	//! Recalculate the normals.
	template<class VType>
	void RecalculateNormals_Flat(VType* verts, u32 numVerts, u16* idx, u32 numIdx)
	{
		Vertex3 e1, e2;
		math::Vector vN;

		for (int i = 0; i < numIdx; i += 3)
		{
			e1 = verts[idx[i + 1]].pos - verts[idx[i]].pos;
			e2 = verts[idx[i + 2]].pos - verts[idx[i]].pos;
			vN.Cross(e1, e2);
			vN.Normalize();
			verts[idx[i]].Nor.X = vN.x;
			verts[idx[i]].Nor.Y = vN.y;
			verts[idx[i]].Nor.Z = vN.z;
			verts[idx[i + 1]].Nor.X = vN.x;
			verts[idx[i + 1]].Nor.Y = vN.y;
			verts[idx[i + 1]].Nor.Z = vN.z;
			verts[idx[i + 2]].Nor.X = vN.x;
			verts[idx[i + 2]].Nor.Y = vN.y;
			verts[idx[i + 2]].Nor.Z = vN.z;
			verts[idx[i]].Nor.Normalize();
			verts[idx[i + 1]].Nor.Normalize();
			verts[idx[i + 2]].Nor.Normalize();
		}
	}

	//! Calculate Binormal & Tangent
	template<class VType>
	void CalcBinormalsTangents(VType* verts, Vertex3* TangentBinormal, u32 numVerts, u16* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				verts[idx[i]].Nor,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				verts[idx[i]].pos,
				verts[idx[i+1]].pos,
				verts[idx[i+2]].pos,
				verts[idx[i]].tex,
				verts[idx[i+1]].tex,
				verts[idx[i+2]].tex
				);
			
			CalcTangent(
				verts[idx[i+1]].Nor,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				verts[idx[i+1]].pos,
				verts[idx[i+2]].pos,
				verts[idx[i]].pos,
				verts[idx[i+1]].tex,
				verts[idx[i+2]].tex,
				verts[idx[i]].tex
				);

			CalcTangent(
				verts[idx[i+2]].Nor,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				verts[idx[i+2]].pos,
				verts[idx[i]].pos,
				verts[idx[i+1]].pos,
				verts[idx[i+2]].tex,
				verts[idx[i]].tex,
				verts[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents

	//! Calculate Binormal & Tangent
	template<class VType>
	void CalcBinormalsTangents32(VType* verts, Vertex3* TangentBinormal, u32 numVerts, u32* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				verts[idx[i]].Nor,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				verts[idx[i]].pos,
				verts[idx[i+1]].pos,
				verts[idx[i+2]].pos,
				verts[idx[i]].tex,
				verts[idx[i+1]].tex,
				verts[idx[i+2]].tex
				);

			CalcTangent(
				verts[idx[i+1]].Nor,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				verts[idx[i+1]].pos,
				verts[idx[i+2]].pos,
				verts[idx[i]].pos,
				verts[idx[i+1]].tex,
				verts[idx[i+2]].tex,
				verts[idx[i]].tex
				);

			CalcTangent(
				verts[idx[i+2]].Nor,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				verts[idx[i+2]].pos,
				verts[idx[i]].pos,
				verts[idx[i+1]].pos,
				verts[idx[i+2]].tex,
				verts[idx[i]].tex,
				verts[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents

	// Calc tangent and binormal
	inline void CalcTangent(Vertex3 &normal, Vertex3 &tangent, Vertex3 &binormal,
					 Vertex3 &vt1, Vertex3 &vt2, Vertex3 &vt3, // vertices
					 Vertex2 &tc1, Vertex2 &tc2, Vertex2 &tc3) // texcoords
	{
		Vertex3 v1 = vt1 - vt2;
		Vertex3 v2 = vt3 - vt1;

		// binormal

		float deltaX1 = tc1.X - tc2.X;
		float deltaX2 = tc3.X - tc1.X;
		binormal = (v1 * deltaX2) - (v2 * deltaX1);
		binormal.Normalize();

		// tangent

		float deltaY1 = tc1.Y - tc2.Y;
		float deltaY2 = tc3.Y - tc1.Y;
		tangent = (v1 * deltaY2) - (v2 * deltaY1);
		tangent.Normalize();

		// adjust

		math::Vector txb;
		txb.Cross(tangent, binormal);
		if (txb * normal < 0.0f)
		{
			tangent  = tangent * -1.0f;
			binormal = binormal * -1.0f;
		}

	}

	//! Calculate Binormal & Tangent
	inline void CalcBinormalsTangents(Vertex3* Pos, Vertex3T* NorTex, Vertex3* TangentBinormal, u32 numVerts, u16* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				NorTex[idx[i]].pos,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				Pos[idx[i]],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex
				);

			CalcTangent(
				NorTex[idx[i+1]].pos,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				Pos[idx[i]],
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex
				);

			CalcTangent(
				NorTex[idx[i+2]].pos,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				Pos[idx[i+2]],
				Pos[idx[i]],
				Pos[idx[i+1]],
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents

	//! Calculate Binormal & Tangent
	inline void CalcBinormalsTangents32(Vertex3* Pos, Vertex3T* NorTex, Vertex3* TangentBinormal, u32 numVerts, u32* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				NorTex[idx[i]].pos,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				Pos[idx[i]],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex
				);

			CalcTangent(
				NorTex[idx[i+1]].pos,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				Pos[idx[i]],
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex
				);

			CalcTangent(
				NorTex[idx[i+2]].pos,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				Pos[idx[i+2]],
				Pos[idx[i]],
				Pos[idx[i+1]],
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents32

	//! Calculate Binormal & Tangent
	inline void CalcBinormalsTangents(Vertex3* Pos, VertexNTC* NorTex, Vertex3* TangentBinormal, u32 numVerts, u16* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				NorTex[idx[i]].Nor,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				Pos[idx[i]],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex
				);

			CalcTangent(
				NorTex[idx[i+1]].Nor,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				Pos[idx[i]],
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex
				);

			CalcTangent(
				NorTex[idx[i+2]].Nor,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				Pos[idx[i+2]],
				Pos[idx[i]],
				Pos[idx[i+1]],
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents

	//! Calculate Binormal & Tangent
	inline void CalcBinormalsTangents32(Vertex3* Pos, VertexNTC* NorTex, Vertex3* TangentBinormal, u32 numVerts, u32* idx, u32 numIdx)
	{
		for (unsigned int i = 0; i < numIdx; i += 3)
		{
			CalcTangent(
				NorTex[idx[i]].Nor,
				TangentBinormal[idx[i] * 2],
				TangentBinormal[idx[i] * 2 + 1],
				Pos[idx[i]],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex
				);

			CalcTangent(
				NorTex[idx[i+1]].Nor,
				TangentBinormal[idx[i+1] * 2],
				TangentBinormal[idx[i+1] * 2 + 1],
				Pos[idx[i+1]],
				Pos[idx[i+2]],
				Pos[idx[i]],
				NorTex[idx[i+1]].tex,
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex
				);

			CalcTangent(
				NorTex[idx[i+2]].Nor,
				TangentBinormal[idx[i+2] * 2],
				TangentBinormal[idx[i+2] * 2 + 1],
				Pos[idx[i+2]],
				Pos[idx[i]],
				Pos[idx[i+1]],
				NorTex[idx[i+2]].tex,
				NorTex[idx[i]].tex,
				NorTex[idx[i+1]].tex
				);

		} // for i

	} // CalcBinormalsTangents

//! This class is for working with meshes and changing their vertex.
class MeshManager : public Singleton<MeshManager>
{
public:

	//! Constructor
	MeshManager();

	//! Destructor
	~MeshManager();

	//! Returns the MeshManager class.
	static MeshManager& GetSingleton() { return *m_pSingleton; }

	//! Returns the MeshManager class pointer.
	static MeshManager* GetSingletonPtr() { return m_pSingleton; }

protected:

}; // MeshManager

} // gfx

} // kge

#endif // KGE_MESHMANAGER_H
