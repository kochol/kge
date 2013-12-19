// File name: DynamicTriangleMesh.h
// Des: 
// Date: 29/7/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_DYNAMICTRIANGLEMESH
#define KGE_DYNAMICTRIANGLEMESH

#include "../math/Triangle.h"

namespace kge
{
namespace sn
{
	class TileTerrain;

} // sn

namespace ph
{
class KGE_API DynamicTriangleMesh: public KgeUnknown
{
public:

	//! Constructor
	DynamicTriangleMesh();

	//! Destructor
	~DynamicTriangleMesh();

	//! Init the class with a tile terrain class
	void Init(sn::TileTerrain* pTerrain);

	//! Checks the ray intersection with triangle mesh and returns the face ID.
	int IntersectRay(math::Ray* pRay);

	//! Checks the ray intersection with triangle mesh and returns the face ID and Closest point index.
	int IntersectRay(math::Ray* pRay, int& OutClosestPointIndex);

	//! Checks the ray intersection with triangle mesh and returns the nearest face ID and Closest point index.
	int IntersectRayClosestFace(math::Ray* pRay, int& OutClosestPointIndex, math::Vector &vp);

protected:

	math::Triangle* m_pTris;

	int				m_iTriCount,
					m_iAabbCount,
					m_iUTriCopunt,
					m_iVTriCount;

	math::AABB*		m_pAABB;

}; // DynamicTriangleMesh

} // ph

} // kge

#endif // KGE_DYNAMICTRIANGLEMESH
