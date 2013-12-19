// File name: math.h
// Des: Class safhe(Plane)
// Date: 17/10/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef PLANE_H
#define PLANE_H

#include "Vector.h"
#include "Matrix.h"

namespace kge
{
namespace math
{
class Sphere;
class AABB;
struct Ray;

class KGE_API Plane
{
public:
	Vector m_vNormal;					// plane normal vector
    float  m_fDistance;					// distance to origin
	float  m_fA , m_fB , m_fC , m_fD;	// ax + by + cz + d = 0

	//! Sazande e ke hich kari nemikone.
	Plane() { /* nothing to do */ }

	//! Checks the plane collision with a sphere and returns ECT_Behind , ECT_Front or ECT_Intersect.
	CollisionType Collision(const Sphere* pSphere) const;

	//! Checks the plane collision with an AABB and returns ECT_Behind , ECT_Front or ECT_Intersect.
	CollisionType Collision(const AABB* pBox) const;

	/*! Sakhte Plane ba yek noghte va bordare normal.
		\param vNormal Bordare normal.
		\param vPoint Mokhtasate noghte.
	 */
	void Set(const Vector &vNormal, const Vector &vPoint);

	/*! Sakhte Plane ba yek noghte va bordare normal va falseleye safhe ta markaze mokhtasat.
		\param vNormal Bordare normal.
		\param vPoint Mokhtasate noghte.
		\param fDistance Faseleye safhe ta markaze mokhtasat.
	 */
	void Set(const Vector &vNormal, const Vector &vPoint, float fDistance);

	/*! Sakhte safhe ba 3 ta noghte ya ye mosals(Polygon).
		\param v0 Noghteye aval.
		\param v1 Noghteye dovom.
		\param v2 Noghteye Sevvom.
	 */
	void Set(const Vector &v0, const Vector &v1, const Vector &v2);

	//! Sakhtane Plane ba formole ax + bY +cz + d = 0.
	void Set( float a , float b , float c , float d );

	/*! Peyda kardane faseleye yek noghte ta safhe. NOTE: bordare Normal bayad normalize shode bashad.
		\param vPoint Noghte e ke mikhahid faseleash ro ta safhe peyda konid.
		\return Fasele noghte ta safhe.
	 */
	inline float GetDistance(const Vector& vPoint ) const;

	//! Matrix Reflection ra sakhte va bar migardone.
	Matrix* GetReflectMatrix();

	//! Builds a matrix that flattens geometry into a plane.
	Matrix* GetShadowMatrix( Vector &vLightDir );

	//! Normalize the plane.
	void Normalize();

	//! Gets the intersection with another plane and return the line
	bool GetIntersectionWithPlane(Plane *Other, Ray &outRay);

	//! Gets the intersection with a line and return the intersected point
	bool GetIntersectionWithLine(Ray *Other, Vector &outPoint);

	//! Gets the intersection with three planes and return the intersected point
	bool GetIntersectionWithPlanes(Plane *o1, Plane *o2, Vector &outPoint);

	//! Computes the dot product of a plane and a 3D vector. The w parameter of the vector is assumed to be 1.
	float DotCoord(const Vector& v) const;

	//! Computes the dot product of a plane and a 3D vector. The w parameter of the vector is assumed to be 0.
	float DotNormal(const Vector& v) const;

protected:
	Vector m_vPoint;					// point on plane

	//! Mohasebeye ax + by + cz + d = 0 ke bad az har bar estefade az tabe set be sorate automatic seda zade mishe.
	inline void CalcABCD();

}; // Plane

} // math

} // kge

#endif // PLANE_H
