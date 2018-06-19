// File name: Sphere.h
// Des: This class is for working with sphere bounding boxes.
// Date: 25/11/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../include/kge/math/Sphere.h"
#include "../../include/kge/math/AABB.h"
#include "../../include/kge/math/Plane.h"
#include "../../include/kge/math/math.h"
#include <math.h>



namespace kge
{
namespace math
{
	//----------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------
	Sphere::Sphere(): 
		m_fRadius(0.0f), m_bFirstPoint(true)
	{
		m_vCenter.set(0,0,0);

		m_iType = KGE_SHAPE_SPHERE;

	} // Constructor

	//----------------------------------------------------------------------------------
	// Add an internal point to calculate the bounding sphere.
	//----------------------------------------------------------------------------------
	void Sphere::AddPoint(const gfx::Vertex3* vPoint)
	{
		if ( m_bFirstPoint )
		{
			m_vCenter.set( vPoint->X , vPoint->Y , vPoint->Z );
			m_vMax		  = m_vCenter;
			m_vMin		  = m_vCenter;
			m_fRadius	  = 0.0f;
			m_bFirstPoint = false;
		}
		else
		{
			if ( m_vMax.x < vPoint->X ) m_vMax.x = vPoint->X;
			if ( m_vMax.y < vPoint->Y ) m_vMax.y = vPoint->Y;
			if ( m_vMax.z < vPoint->Z ) m_vMax.z = vPoint->Z;

			if ( m_vMin.x > vPoint->X ) m_vMin.x = vPoint->X;
			if ( m_vMin.y > vPoint->Y ) m_vMin.y = vPoint->Y;
			if ( m_vMin.z > vPoint->Z ) m_vMin.z = vPoint->Z;

			m_vCenter.x = ( m_vMax.x + m_vMin.x ) / 2;
			m_vCenter.y = ( m_vMax.y + m_vMin.y ) / 2;
			m_vCenter.z = ( m_vMax.z + m_vMin.z ) / 2;

			float temp;
			m_fRadius = math::abs( m_vMax.x - m_vMin.x ) / 1.5f;
			temp = math::abs ( m_vMax.y - m_vMin.y ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
			temp = math::abs ( m_vMax.z - m_vMin.z ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
		}

	} // AddPoint

	//----------------------------------------------------------------------------------
	// Add an internal point to calculate the bounding sphere.
	//----------------------------------------------------------------------------------
	void Sphere::AddPoint(const Vector& v)
	{
		if ( m_bFirstPoint )
		{
			m_vCenter.set( v.x , v.y , v.z );
			m_vMax		  = m_vCenter;
			m_vMin		  = m_vCenter;
			m_fRadius	  = 0.0f;
			m_bFirstPoint = false;
		}
		else
		{
			if ( m_vMax.x < v.x ) m_vMax.x = v.x;
			if ( m_vMax.y < v.y ) m_vMax.y = v.y;
			if ( m_vMax.z < v.z ) m_vMax.z = v.z;

			if ( m_vMin.x > v.x ) m_vMin.x = v.x;
			if ( m_vMin.y > v.y ) m_vMin.y = v.y;
			if ( m_vMin.z > v.z ) m_vMin.z = v.z;

			m_vCenter.x = ( m_vMax.x + m_vMin.x ) / 2;
			m_vCenter.y = ( m_vMax.y + m_vMin.y ) / 2;
			m_vCenter.z = ( m_vMax.z + m_vMin.z ) / 2;

			float temp;
			m_fRadius = math::abs( m_vMax.x - m_vMin.x ) / 1.5f;
			temp = math::abs ( m_vMax.y - m_vMin.y ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
			temp = math::abs ( m_vMax.z - m_vMin.z ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
		}
	} // AddPoint (Vector)

	//----------------------------------------------------------------------------------
	// Add an internal sphere to calculate the bounding sphere.
	//----------------------------------------------------------------------------------
	void Sphere::AddSphere(const Sphere* sphere)
	{
		if (m_fRadius == 0.0f)
		{
			m_vCenter	  = sphere->GetCenter();
			m_fRadius	  = sphere->GetRadius();
			m_bFirstPoint = false;

			// Set the min and max points.
			m_vMax.set(m_vCenter.x + m_fRadius , m_vCenter.y + m_fRadius , m_vCenter.z + m_fRadius );
			m_vMin.set(m_vCenter.x - m_fRadius , m_vCenter.y - m_fRadius , m_vCenter.z - m_fRadius );
		}
		else
		{
			if ( m_vMax.x < sphere->m_vMax.x ) m_vMax.x = sphere->m_vMax.x;
			if ( m_vMax.y < sphere->m_vMax.y ) m_vMax.y = sphere->m_vMax.y;
			if ( m_vMax.z < sphere->m_vMax.z ) m_vMax.z = sphere->m_vMax.z;

			if ( m_vMin.x < sphere->m_vMin.x ) m_vMin.x = sphere->m_vMin.x;
			if ( m_vMin.y < sphere->m_vMin.y ) m_vMin.y = sphere->m_vMin.y;
			if ( m_vMin.z < sphere->m_vMin.z ) m_vMin.z = sphere->m_vMin.z;

			m_vCenter.x = ( m_vMax.x + m_vMin.x ) / 2;
			m_vCenter.y = ( m_vMax.y + m_vMin.y ) / 2;
			m_vCenter.z = ( m_vMax.z + m_vMin.z ) / 2;

			float temp;
			m_fRadius = math::abs( m_vMax.x - m_vMin.x ) / 1.5f;
			temp = math::abs ( m_vMax.y - m_vMin.y ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
			temp = math::abs ( m_vMax.z - m_vMin.z ) / 1.5f;
			if ( temp > m_fRadius ) m_fRadius = temp;
		}

	} // AddSphere

	//----------------------------------------------------------------------------------------------
	// Adds a AABB to calculate the bounding sphere.
	//----------------------------------------------------------------------------------------------
	void Sphere::AddBox(const AABB& aabb)
	{
		AddPoint(aabb.m_vMin);
		AddPoint(aabb.m_vMax);

	} // AddBox

	//----------------------------------------------------------------------------------------------
	// Checks the sphere collision with a plane and returns ECT_Behind , ECT_Front or ECT_Intersect.
	//----------------------------------------------------------------------------------------------
	CollisionType Sphere::Collision(const Plane* pPlane) const
	{
		float f = GetDistance(pPlane);

		if ( fabs(f) < m_fRadius )
			return ECT_Intersect;
		if ( f > 0 )
			return ECT_Front;

		return ECT_Behind;

	} // Collision(Plane)

	//-------------------------------------------
	// Returns true if point is inside the sphere
	//-------------------------------------------
	bool Sphere::Inside(const math::Vector &point) const
	{
		return (m_vCenter - point).GetSqrLength() <= m_fRadius * m_fRadius;
	}

	//----------------------------------------------------------------------------------
	// Returns the distance of the plane.
	//----------------------------------------------------------------------------------
	float Sphere::GetDistance(const Plane* pPlane) const
	{
		return (pPlane->m_vNormal * m_vCenter + pPlane->m_fDistance);

	} // GetDistance

} // math

} // kge
