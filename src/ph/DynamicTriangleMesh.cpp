// File name: DynamicTriangleMesh.h
// Des:
// Date: 2/8/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include <stdio.h>
#include "../../Include/ph/DynamicTriangleMesh.h"
#include "../../Include/sn/TileTerrain.h"

namespace kge
{
namespace ph
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	DynamicTriangleMesh::DynamicTriangleMesh(): m_pTris(NULL), m_iTriCount(0), m_iAabbCount(0),
		m_pAABB(NULL)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	DynamicTriangleMesh::~DynamicTriangleMesh()
	{
		if (m_pTris)
		{
			delete[] m_pTris;
			m_pTris = NULL;
		}

	} // Destructor

	//------------------------------------------------------------------------------------
	// Init the class with a tile terrain class
	//------------------------------------------------------------------------------------
	void DynamicTriangleMesh::Init( sn::TileTerrain* pTerrain )
	{
		// Calculate the number of triangles
		m_iTriCount				 = pTerrain->GetIndexesCount() / 3;
		u32* indices			 = pTerrain->GetIndices();
		gfx::Vertex3* vertices   = pTerrain->GetPositions();

		if (m_pTris)
			delete[] m_pTris;
		m_pTris = new math::Triangle[m_iTriCount];

		for (int i = 0; i < m_iTriCount; i++)
		{
			m_pTris[i].Set(&vertices[indices[i*3]],
						   &vertices[indices[i*3+1]],
						   &vertices[indices[i*3+2]]);
		}

		m_iAabbCount  = pTerrain->GetAABBcount();
		m_pAABB		  = pTerrain->GetAABBs();

	} // Init (TileTerrain)

	//------------------------------------------------------------------------------------
	// Checks the ray intersection with triangle mesh and returns the face ID.
	//------------------------------------------------------------------------------------
	int DynamicTriangleMesh::IntersectRay( math::Ray* pRay )
	{
		for (int k = 0; k < m_iAabbCount; k++)
		{
			if (m_pAABB[k].IntersectRay(pRay))
			{
				printf("AABB = %d\nTC = %d\n", k, m_iTriCount);
				for (int i = k * 512; i < (k + 1) * 512; i++)
				{
					if (m_pTris[i].IntersectRay(pRay) == math::ECT_Intersect)
						return i;
				}
			}
		}

		return -1;

	} // IntersectRay

	//------------------------------------------------------------------------------------
	// Checks the ray intersection with triangle mesh and returns the face ID and Closest point index.
	//------------------------------------------------------------------------------------
	int DynamicTriangleMesh::IntersectRay( math::Ray* pRay, int& OutClosestPointIndex )
	{
		kge::math::Vector v, t;
		float f1, f2, f3;
		for (int k = 0; k < m_iAabbCount; k++)
		{
			if (m_pAABB[k].IntersectRay(pRay))
			{
				for (int i = k * 512; i < (k + 1) * 512; i++)
				{
					if (m_pTris[i].IntersectRay(pRay, v) == math::ECT_Intersect)
					{
						t  = v - *m_pTris[i].Point1;
						f1 = t.GetSqrLength();
						t  = v - *m_pTris[i].Point2;
						f2 = t.GetSqrLength();
						t  = v - *m_pTris[i].Point3;
						f3 = t.GetSqrLength();
						OutClosestPointIndex = i * 3;
						if (f1 < f2 && f1 < f3)
							OutClosestPointIndex = i * 3;
						else if (f2 < f1 && f2 < f3)
							OutClosestPointIndex = i * 3 + 1;
						else if (f3 < f2 && f3 < f1)
							OutClosestPointIndex = i * 3 + 2;

						return i;
					}
				}
			}
		}

		return -1;

	} // IntersectRay (closest point)

	int DynamicTriangleMesh::IntersectRayClosestFace( math::Ray* pRay, int& OutClosestPointIndex, math::Vector &vp )
	{
		int FaceID = -1;
		kge::math::Vector v, t, lastv;
		float f1, f2, f3, lastDist = 100000.0f, dist = 0.0f;
		int lastIndex = -1;
		for (int k = 0; k < m_iAabbCount; k++)
		{
			if (m_pAABB[k].IntersectRay(pRay))
			{
				for (int i = k * 512; i < (k + 1) * 512; i++)
				{
					if (m_pTris[i].IntersectRay(pRay, v) == math::ECT_Intersect)
					{
						t  = v - *m_pTris[i].Point1;
						f1 = t.GetSqrLength();
						t  = v - *m_pTris[i].Point2;
						f2 = t.GetSqrLength();
						t  = v - *m_pTris[i].Point3;
						f3 = t.GetSqrLength();
						OutClosestPointIndex = i * 3;
						if (f1 < f2 && f1 < f3)
							OutClosestPointIndex = i * 3;
						else if (f2 < f1 && f2 < f3)
							OutClosestPointIndex = i * 3 + 1;
						else if (f3 < f2 && f3 < f1)
							OutClosestPointIndex = i * 3 + 2;
						t = v - pRay->Position;
						dist = t.GetSqrLength();
						if (FaceID == -1 || dist < lastDist)
						{
							FaceID = i;
							lastDist = dist;
							lastIndex = OutClosestPointIndex;
							vp = v;
						}
					}
				}
			}
		}

		OutClosestPointIndex = lastIndex;

		return FaceID;

	} // IntersectRayClosestPoint

} // ph

} // kge
