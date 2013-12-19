// File name: Sphere.h
// Des: This class is for working with sphere bounding boxes.
// Date: 23/11/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef SPHERE_H
#define SPHERE_H

#include "../gfx/Structs.h"
#include "Shape.h"
#include "Vector.h"

namespace kge
{
namespace math
{
class Plane;
class AABB;

class KGE_API Sphere : public Shape
{
public:

	//! Constructor
	Sphere();

	//! Destructor
	~Sphere() { }

	//! Add an internal point to calculate the bounding sphere.
	void AddPoint(const gfx::Vertex3* vPoint);

	//! Add an internal point to calculate the bounding sphere.
	void AddPoint(const Vector& v);

	//! Add an internal sphere to calculate the bounding sphere.
	void AddSphere(const Sphere* sphere);

	//! Adds a AABB to calculate the bounding sphere.
	void AddBox(const AABB& aabb);

	//! Checks the sphere collision with a plane and returns ECT_Behind , ECT_Front or ECT_Intersect.
	CollisionType Collision(const Plane* pPlane) const;

	//! Returns true if point is inside the sphere
	bool Inside(const math::Vector &point) const;

	//! Returns the sphere center point.
	const Vector& GetCenter() const { return m_vCenter; } 

	//! Returns the sphere radius.
	float GetRadius() const { return m_fRadius; }

	//! Returns the distance of the plane.
	float GetDistance(const Plane* pPlane) const;

	//! Sets the sphere center.
	void SetCenter(const Vector& v) { m_vCenter = v; }

	//! Sets the sphere center.
	void SetRadius(float f) { m_fRadius = f; }

	int getType() const { return m_iType; }

	Vector	m_vCenter,		/**< The center of the sphere. */
			m_vMax,			/**< The max point uses by AddPoint function. */
			m_vMin;			/**< The min point uses by AddPoint function. */
	float	m_fRadius;		/**< The sphere radius. */

protected:
	bool	m_bFirstPoint;	/**< Check for first point recive. */
	int		m_iType;

}; // Sphere

} // math

} // kge

#endif // SPHERE_H
