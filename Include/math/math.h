// File name: math.h
// Des: Dar in file tamame tabe haye riyazi gharar migire.
// Date: 30/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_MATH_H
#define KGE_MATH_H

namespace kge
{
//! Mathematics classes
namespace math
{
	class Sphere;
	class Vector;
	class Plane;

	//! The collision type of objects.
	enum CollisionType
	{
		ECT_Out		 ,			/**< The Object is outside of somthing like a frustum. */
		ECT_In		 ,			/**< The Object is inside of somthing like a frustum. */
		ECT_Front    ,			/**< The Object is front of somthing like a plane. */
		ECT_Behind   ,			/**< The Object is behind of somthing like a plane. */
		ECT_Intersect,			/**< The Object is intersecting with another object. */
		ECT_NotIntersect		/**< The Object is not intersecting with another object. */

	}; // Collision type

	//! P number.
	const float PI          = 3.141592654f;
	const float TwoPI       = 6.283185307f;
	const float PiOver2     = 1.570796326f;

	const float fDegToRad = 0.0174532925f;
	const float fRadToDeg = 57.295779513f;

	const float fEpsilon  = 0.000001f;

	//! Darage ra be radian tabdil mikone.
	/*!
		\param Degree Zaviye bar hasbe daraje.
		\return Zaviye bar hasbe radian.
	*/
	inline float DegToRad(float Degree)
	{
		return (Degree * fDegToRad);

	} // DegToRad


	//! Radian ra be darage tabdil mikone.
	/*!
		\param Radian Zaviye bar hasbe radian.
		\return Zaviye bar hasbe darage.
	*/
	inline float RadToDeg(float Radian)
	{
		return (Radian / PI * 180);

	} // RadToDeg
	
	inline float abs(float Number)
	{
		if ( Number < 0 ) return -Number;
		
		return Number;

	} // abs

	//! power x ^ y
	inline int pow(int x, int y)
	{
		//if (y == 0)
		//	return 1;
		//return pow(x, y - 1) * x;
		int result = 1;
		for (; y > 0; --y)
			result *= x;
		return result;

	} // pow

	// max - float
	inline float Max(float A,float B)
	{
		return (A>=B) ? A : B;
	}


	// min - float
	inline float Min(float A,float B)
	{
		return (A<=B) ? A : B;
	}


	//! this function tests if the projection of a bounding sphere along the light direction intersects the view frustum 
	bool SweptSpherePlaneIntersect(float& t0, float& t1, 
		const Plane& plane, const Sphere& sphere, const Vector& sweepDir);

} // math

} // kge

#endif // KGE_MATH_H