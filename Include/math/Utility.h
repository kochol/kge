// File name: Utility.h
// Des: Utility functions
// Date: 21/10/1389
// Programmer: Nader Golbaz

#ifndef UTILITY_H
#define UTILITY_H

#include "../kgedef.h"

namespace kge
{

namespace math
{

	class Vector;

	//! This function returns a random floating-point number between lower and upper bounds
	float KGE_API RandomFloat(float lowerBound, float upperBound);

	//! This function returns a random normalized vector
	Vector KGE_API RandomVector();

	//! This function returns true if two numbers are almost equal
	bool KGE_API AlmostEqual(float x, float y);

	//!
	void KGE_API BezierCurve(Vector& result, const Vector* points, int num, float t);

	//!
	void KGE_API CatmullRom(
		Vector& result,
		const Vector& p1,
		const Vector& p2,
		const Vector& p3,
		const Vector& p4,
		float t);

} // math

} // kge

#endif // UTILITY_H
