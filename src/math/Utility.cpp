// File name: Utility.cpp
// Des: Utility functions
// Date: January 11, 2011
// Programmer: Nader Golbaz

//#include <cassert>
#include <cstdlib>
#include <cmath>
#include "../../include/kge/math/Utility.h"
#include "../../include/kge/math/Vector.h"

namespace kge
{

namespace math
{

	//------------------------------------------------------------------------------------
	// This function returns a random floating-point number between lower and upper bounds
	//------------------------------------------------------------------------------------
	float RandomFloat(float lowerBound, float upperBound)
	{
		//assert(lowerBound <= upperBound);
		lowerBound = lowerBound > upperBound ? upperBound : lowerBound;

		return rand() / static_cast<float>(RAND_MAX) * (upperBound - lowerBound) + lowerBound;
	}

	//-------------------------------------------------
	// This function returns a random normalized vector
	//-------------------------------------------------
	Vector RandomVector()
	{
		Vector vec(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f);

		vec.Normalize();

		return vec;
	}

	//-----------------------------------------------------------
	// This function returns true if two numbers are almost equal
	//-----------------------------------------------------------
	bool AlmostEqual(float x, float y)
	{
		return fabs(x / y - 1.0f) < 0.0001f;
	}

	//-----------------------------------------------------------
	// Binomial Coefficients (0 <= k <= n)
	//-----------------------------------------------------------
	int BinomialCoefficient(int n, int k)
	{
		int a = 1;

		for (int i = n; i > n - k; --i)
		{
			a *= i;
		}
		
		int b = 1;

		for (int i = k; i > 1; --i)
		{
			b *= i;
		}

		return a / b;
	}

	//-----------------------------------------------------------
	// Bernstein polynomial
	//-----------------------------------------------------------
	float BernsteinPolynomial(int i, int n, float t)
	{
		return BinomialCoefficient(n, i) * std::pow(t, i) * std::pow(1.0f - t, n - i);
	}

	//-----------------------------------------------------------
	// B�zier Curves
	//-----------------------------------------------------------
	void BezierCurve(Vector& result, const Vector* points, int num, float t)
	{
		result.Reset();

		for (int i = 0; i < num; ++i)
		{
			result += points[i] * BernsteinPolynomial(i, num - 1, t);
		}
	}

	//-----------------------------------------------------------
	// Catmull-Rom Splines
	//-----------------------------------------------------------
	void CatmullRom(Vector& result, const Vector& p1, const Vector& p2,
		const Vector& p3, const Vector& p4, float t)
	{
		const float t2 = t * t;
		const float t3 = t2 * t;

		result = (p1 * (-t3 + 2.0f * t2 - t) + p2 * (3.0f * t3 - 5.0f * t2 + 2.0f) +
			p3 * (-3.0f * t3 + 4.0f * t2 + t) + p4 * (t3 - t2)) * 0.5f;
	}

} // math

} // kge
