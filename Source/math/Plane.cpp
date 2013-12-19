#include "../../Include/math/Plane.h"
#include "../../Include/math/Sphere.h"
#include "../../Include/math/Ray.h"
#include "../../Include/math/AABB.h"
#include "../../Include/io/Logger.h"
#include <math.h>



namespace kge
{
namespace math
{
	//----------------------------------------------------------------------------------------------
	// Checks the plane collision with a sphere and returns ECT_Behind , ECT_Front or ECT_Intersect.
	//----------------------------------------------------------------------------------------------
	CollisionType Plane::Collision(const Sphere* pSphere) const
	{
		float f = GetDistance(pSphere->GetCenter());

		if ( pSphere->GetRadius() > fabs(f) )
			return ECT_Intersect;
		if ( f > 0 )
			return ECT_Front;

		return ECT_Behind;

	} // Collision ( Sphere ) 

	//----------------------------------------------------------------------------------------------
	// Checks the plane collision with an AABB and returns ECT_Behind , ECT_Front or ECT_Intersect.
	//----------------------------------------------------------------------------------------------
	CollisionType Plane::Collision( const AABB* pBox ) const
	{
		Vector nearPoint(pBox->m_vMax);
		Vector farPoint(pBox->m_vMin);

		for (int i = 0; i < 3; i++)
		{
			if (m_vNormal.v[i] > 0)
			{
				nearPoint.v[i] = pBox->m_vMin.v[i];
				farPoint.v[i]  = pBox->m_vMax.v[i];
			}
		}

		if (m_vNormal * nearPoint + m_fD > 0.0)
			return ECT_Front;

		if (m_vNormal * farPoint + m_fD > 0.0)
			return ECT_Intersect;

		return ECT_Behind;

	} // Collision ( AABB )

	//----------------------------------------------
	// Sakhte Plane ba yek noghte va bordare normal.
	//----------------------------------------------
	void Plane::Set(const Vector &vcN, const Vector &vcP)
	{
	    m_fDistance = - ( vcN * vcP);
		m_vNormal   = vcN;
	    m_vPoint    = vcP;

		CalcABCD();

	} // Set(vN , vP)

	//--------------------------------------------------------------------------------------
	// Sakhte Plane ba yek noghte va bordare normal va falseleye safhe ta markaze mokhtasat.
	//--------------------------------------------------------------------------------------
	void Plane::Set(const Vector &vcN, const Vector &vcP, float fD)
	{
	    m_vNormal   = vcN;
		m_fDistance = fD;
	    m_vPoint    = vcP;

		CalcABCD();

	} // Set(vN , vP , fD)

	//---------------------------------------------------
	// Sakhte safhe ba 3 ta noghte ya ye mosals(Polygon).
	//---------------------------------------------------
	void Plane::Set(const Vector &v0, const Vector &v1, const Vector &v2)
	{
	    Vector vcEdge1 = v1 - v0;
		Vector vcEdge2 = v2 - v0;

	    m_vNormal.Cross(vcEdge1, vcEdge2);
		m_vNormal.Normalize();
	    m_fDistance = - (m_vNormal * v0);
		m_vPoint    = v0;

		CalcABCD();

	} // Set( V0 , V1 , V2 )

	//-----------------------------------------------
	// Sakhtane Plane ba formole ax + bY +cz + d = 0.
	//-----------------------------------------------
	void Plane::Set( float a , float b , float c , float d )
	{
		m_fA = a;
		m_fB = b;
		m_fC = c;
		m_fD = d;

	} // Set ( a , b , c , d )

	//--------------------------------------------
	// Peyda kardane faseleye yek noghte ta safhe.
	//--------------------------------------------
	inline float Plane::GetDistance(const Vector& vcPoint) const
	{
		return ( (m_vNormal*vcPoint) + m_fDistance );

	} // Distance

	//---------------------------------
	// Mohasebeye ax + by + cz + d = 0.
	//---------------------------------
	inline void Plane::CalcABCD()
	{
		m_fA = m_vNormal.x;
		m_fB = m_vNormal.y;
		m_fC = m_vNormal.z;
		m_fD = -(m_fA * m_vPoint.x) - (m_fB * m_vPoint.y) - (m_fC * m_vPoint.z);

	} // CalcABCD

	//----------------------------------------------
	// Matrix Reflection ra sakhte va bar migardone.
	//----------------------------------------------
	Matrix* Plane::GetReflectMatrix()
	{
		Matrix* m;
		m = KGE_NEW(Matrix)();

		m->_11 = -2 * m_fA * m_fA + 1;
		m->_12 = -2 * m_fB * m_fA;
		m->_13 = -2 * m_fC * m_fA;

		m->_21 = -2 * m_fA * m_fB;
		m->_22 = -2 * m_fB * m_fB + 1;
		m->_23 = -2 * m_fC * m_fB;

		m->_31 = -2 * m_fA * m_fC;
		m->_32 = -2 * m_fB * m_fC;
		m->_33 = -2 * m_fC * m_fC + 1;

		m->_41 = -2 * m_fA * m_fD;
		m->_42 = -2 * m_fB * m_fD;
		m->_43 = -2 * m_fC * m_fD;

		return m;

	} // GetReflectMatrix

	//-----------------------------------------------------
	// Builds a matrix that flattens geometry into a plane.
	//-----------------------------------------------------
	Matrix* Plane::GetShadowMatrix( Vector &vLightDir )
	{
		Matrix* m;
		m = KGE_NEW(Matrix)();

		float d = m_fA * vLightDir.x
				+ m_fB * vLightDir.y
				+ m_fC * vLightDir.z
				+ m_fD * vLightDir.w ;

		m->_11 = d - m_fA * vLightDir.x;
		m->_12 = 0.0f - m_fA * vLightDir.y;
		m->_13 = 0.0f - m_fA * vLightDir.z;
		m->_14 = 0.0f - m_fA * vLightDir.w;

		m->_21 = 0.0f - m_fB * vLightDir.x;
		m->_22 = d - m_fB * vLightDir.y ;
		m->_23 = 0.0f - m_fB * vLightDir.z;
		m->_24 = 0.0f - m_fB * vLightDir.w;

		m->_31 = 0.0f - m_fC * vLightDir.x;
		m->_32 = 0.0f - m_fC * vLightDir.y;
		m->_33 = d - m_fC * vLightDir.z ;
		m->_34 = 0.0f - m_fC * vLightDir.w;

		m->_41 = 0.0f - m_fD * vLightDir.x;
		m->_42 = 0.0f - m_fD * vLightDir.y;
		m->_43 = 0.0f - m_fD * vLightDir.z;
		m->_44 = d - m_fD * vLightDir.w ;

		return m;

	} // GetShadowMatrix

	//-----------------------------------------------------------------------------
	// Normalize the plane.
	//-----------------------------------------------------------------------------
	void Plane::Normalize()
	{
		Vector v(m_fA, m_fB, m_fC);
		float len = v.GetLength();
		m_fA = m_fA / len;
		m_fB = m_fB / len;
		m_fC = m_fC / len;
		m_fD = m_fD / len;

		m_vNormal.set(m_fA, m_fB, m_fC);
		m_fDistance = m_fD;

	} // Normalize

	//-----------------------------------------------------------------------------
	// Gets the intersection with another plane and return the line
	//-----------------------------------------------------------------------------
	bool Plane::GetIntersectionWithPlane( Plane *Other , Ray& ray)
	{
		const float fn00 = m_vNormal.GetLength();
		const float fn01 = m_vNormal * Other->m_vNormal;
		const float fn11 = Other->m_vNormal.GetLength();
		const double det = fn00 * fn11 - fn01 * fn01;

		if (fabs(det) < Rounding_Error_Double)
			return false;

		const double invdet = 1.0 / det;
		const double fc0	= (fn11 * -m_fD + fn01 * Other->m_fD) * invdet;
		const double fc1	= (fn00 * -Other->m_fD + fn01 * m_fD) * invdet;

		Vector v;
		v.Cross(m_vNormal, Other->m_vNormal);
		ray.Set(v ,	m_vNormal * (float)fc0 + Other->m_vNormal * (float)fc1);

		return true;

	} // GetIntersectionWithPlane

	//-----------------------------------------------------------------------------
	// Gets the intersection with a line and return the intersected point
	//-----------------------------------------------------------------------------
	bool Plane::GetIntersectionWithLine( Ray *ray, Vector &outPoint )
	{
		float f2 = m_vNormal * ray->Target;

		if (f2 == 0)
			return false;

		float f =- (m_vNormal * ray->Position + m_fD) / f2; 

		outPoint = ray->Position + (ray->Target * f);

		return true;

	} // GetIntersectionWithLine

	//-----------------------------------------------------------------------------
	// Gets the intersection with three planes and return the intersected point
	//-----------------------------------------------------------------------------
	bool Plane::GetIntersectionWithPlanes( Plane *o1, Plane *o2, Vector &outPoint )
	{
		Ray ray;
		if (GetIntersectionWithPlane(o1, ray))
			return o2->GetIntersectionWithLine(&ray, outPoint);

		return false;

	} // GetIntersectionWithPlanes

	//------------------------------------------------------------------------------------
	// Computes the dot product of a plane and a 3D vector. The w parameter of the vector is assumed to be 1.
	//------------------------------------------------------------------------------------
	float Plane::DotCoord( const Vector& v ) const
	{
		return m_fA * v.x + m_fB * v.y + m_fC * v.z + m_fD;

	} // DotCoord

	//------------------------------------------------------------------------------------
	// Computes the dot product of a plane and a 3D vector. The w parameter of the vector is assumed to be 0.
	//------------------------------------------------------------------------------------
	float Plane::DotNormal( const Vector& v ) const
	{
		return m_fA * v.x + m_fB * v.y + m_fC * v.z;

	} // DotNormal

} // math

} // kge
