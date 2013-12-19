// File name: Triangle.h
// Des: This class is for working with triangle math
// Date: 20/7/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_TRIANGLE_H
#define KGE_TRIANGLE_H

#include "math.h"
#include "Vector.h"
#include "Ray.h"
#include "../gfx/Structs.h"

namespace kge
{
namespace math
{
struct Triangle
{
	gfx::Vertex3  Edge1,
				  Edge2;
	gfx::Vertex3* Point1,
				* Point2,
				* Point3;
	Vector		  Normal;

	void Set(gfx::Vertex3* p1, gfx::Vertex3* p2, gfx::Vertex3* p3)
	{
		Point1 = p1;
		Point2 = p2;
		Point3 = p3;
		Edge1  = (*p2) - (*p1);
		Edge2  = (*p3) - (*p1);
		Normal.Cross(Edge1, Edge2);

	} // Set

	//! Checks if the ray and triangle intersects
	CollisionType IntersectRay(Ray* pRay)
	{
		float fGama = -(pRay->Direction * Normal);

		if (fGama < fEpsilon)
			return ECT_NotIntersect;

		Vector b = pRay->Position - (*Point1);
		float fLanda = (b * Normal) / fGama;

		if (fLanda >= 0.0f && fLanda <= 1.0f)
		{
			Vector u;
			u.Cross(b, pRay->Direction);
			float u1, u2;
			u1 = (Edge2.ToVector() * u) / fGama;
			u2 = (-(Edge1.ToVector() * u)) / fGama;
			if (u1 + u2 <= 1.0f && u1 >= 0.0f && u2 >= 0.0f)
				return ECT_Intersect;
		}

		return ECT_NotIntersect;

	} // IntersectRay

	//! Checks if the ray and triangle intersects and store the intersection point in the Out parameter.
	CollisionType IntersectRay(Ray* pRay, kge::math::Vector& Out)
	{
		Edge1  = (*Point2) - (*Point1);
		Edge2  = (*Point3) - (*Point1);
		Normal.Cross(Edge1, Edge2);
		float fGama = -(pRay->Direction * Normal);

		if (fGama < fEpsilon)
			return ECT_NotIntersect;

		Vector b = pRay->Position - (*Point1);
		float fLanda = (b * Normal) / fGama;

		if (fLanda >= 0.0f && fLanda <= 1.0f)
		{
			Vector u;
			u.Cross(b, pRay->Direction);
			float u1, u2;
			u1 = (Edge2.ToVector() * u) / fGama;
			u2 = (-(Edge1.ToVector() * u)) / fGama;
			if (u1 + u2 <= 1.0f && u1 >= 0.0f && u2 >= 0.0f)
			{
				Out = pRay->Position + (pRay->Direction * fLanda);
				return ECT_Intersect;
			}
		}

		return ECT_NotIntersect;

	} // IntersectRay

}; // Triangle

} // math

} // kge

#endif // KGE_TRIANGLE_H
