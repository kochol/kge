// File name: AABB.h
// Des: This class is for working with axis aligned bounding boxes.
// Date: 11/08/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_AABB_H
#define KGE_AABB_H

#include "Vector.h"
#include "Shape.h"
#include "../gfx/Structs.h"

namespace kge
{
namespace math
{
struct Ray;

class KGE_API AABB : public Shape
{
public:

	//! Constructor
	AABB();

	//! Constructor
	AABB(const gfx::Vertex3& init);

	//! Constructor
	AABB(const Vector& init);

	//! Constructor
	AABB(const AABB &init);

	//! Destructor
	~AABB();

	//! Adds a point to bounding box
	void AddInternalPoint(gfx::Vertex3* point);
	void AddInternalPoint(Vector* point);
	void AddInternalPoint(const Vector &point);

	//! Adds an internal AABB
	void AddAABB(const AABB* bb);

	//! Only checks for Y
	void AddY(float y);

	//! Returns true if the ray and box collide.
	inline bool IntersectRay(Ray* pRay);

	//! Returns true if point is inside the box
	bool Inside(const math::Vector &point) const;

	//! Returns true if two boxes are intersecting
	bool Intersect(const AABB& box) const;

	//! Checks for swap min and max 
	void Repair();

	void DebugRender();

	//! resets the bounding box
	void Reset();

	void ResetY();

	int getType() const { return m_iType; }

	Vector GetCenter();

//protected:

	void UpdateLines();

	void CreateIndices();

	//! Min and max vectors
	Vector m_vMin, m_vMax;

	// First vector
	bool	m_bFirst;

	gfx::Vertex3C v[8];
	u16			  i[24];

protected:
	int m_iType;

}; // AABB

} // math

} // kge

#endif // KGE_AABB_H
