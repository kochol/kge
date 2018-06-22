// File name: Vector.h
// Des: in class baraye kar ba vector ha hast. be har noghte ee toye faza ye vector 
//		migan. baraye inke betavan az SSE dar mohasebat estefade kard in vector daraye
//		4 jahat hast x, y, z, w.
// Date: 24/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef VECTOR_H
#define VECTOR_H

//#include <cassert>
#include <cmath>
#include "../kgedef.h"
#include "math.h"

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace gfx
{
	struct Vertex3;

} // gfx

namespace math
{

class KGE_API Vector
{
public:
	union
	{
		float v[4];
		struct
		{
			float x, y, z, w;
		};
	};

	//! New Vector x=y=z=0.0f,w=1.0f;
	Vector()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

	//! New Vector set the x, y, z
	Vector(float x, float y, float z, float w = 1.0f)
		: x(x), y(y), z(z), w(w) {}

	//! New Vector from vertex3
	Vector(const gfx::Vertex3 &init);

	//! New Vector from Vector
	//Vector(const Vector &other)
	//{
	//	x = other.x;
	//	y = other.y;
	//	z = other.z;
	//	w = other.w;
	//}

	//! Assignment operator
	//Vector& operator =(const Vector &other)
	//{
	//    x = other.x;
	//    y = other.y;
	//    z = other.z;
	//    w = other.w;
	//    return *this;
	//}

	//! Set the x,y,z w(optional)
	void set(float _x, float _y, float _z, float _w = 1.0f) 
		{x = _x; y = _y; z = _z; w = _w;}

	//! Reset Vector to default values
	void Reset()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	//! Returns the vector length
	float GetLength() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	//! Badast avardane toole bordar be tavane 2
	float GetSqrLength() const
	{
		return x*x + y*y + z*z;
	}

	//! jahate bordaro bar ax mikone.
	void Negate()
	{
		x = -x;
		y = -y;
		z = -z;
		//w = -w;
	}

	//! bordar ra be bordare Normal tabdil mikone.
	void Normalize()
	{
		float length = sqrtf(x*x + y*y + z*z);
		length = fEpsilon > length ? fEpsilon : length;
		x /= length;
		y /= length;
		z /= length;
	}

	//! Dot product.
	float operator*(const Vector &v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	//! Dot product.
	float operator*(const gfx::Vertex3 &v) const;

	//! Tafrighe do ta vector.
	Vector operator-(const Vector &v) const
	{
		return Vector(x - v.x, y - v.y, z - v.z);
	}

	//! Tafrighe do ta vector.
	Vector operator-(const gfx::Vertex3 &v) const;

	//! Adds two vector.
	Vector operator+(const Vector &v) const
	{
		return Vector(x + v.x, y + v.y, z + v.z);
	}

	//! Adds two vector.
	void operator+=(const Vector &v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	//! Vector * float
	Vector operator*(float f) const
	{
		return Vector(x * f, y * f, z * f);
	}

	//! Vector / float
	Vector operator/(float f) const
	{
		f = fEpsilon > f ? fEpsilon : f;
		return Vector(x / f, y / f, z / f);
	}

	//! /= float
	void operator/=(float f)
	{
		f = fEpsilon > f ? fEpsilon : f;
		x /= f;
		y /= f;
		z /= f;
	}

	//! Checking for not equality.
	bool operator != (const Vector &nv) const
	{
		if ( x != nv.x)
			return true;
		if ( y != nv.y)
			return true;
		if ( z != nv.z)
			return true;

		return false;
	}

	//! Checking for equal.
	bool operator == (const Vector &nv) const
	{
		if ( x == nv.x && y == nv.y && z == nv.z)
			return true;

		return false;
	}

	//! Zarb khareji do ta Vector ke hasel Vectori hast amood bar on do.
	void Cross(const Vector &v1, const Vector &v2)
	{
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
		w = 1.0f;
	}

	//! Cross two vertex3 
	void Cross(const gfx::Vertex3 &v1, const gfx::Vertex3 &v2);

}; // Vector

//! 2D Vector
template <typename T>
class Vector2
{
public:

	//! Sazande.
	Vector2() : x(0), y(0) {}

	//! Sazande ke mitonid bahash x,y ra meghdar bedid.
	Vector2(T x, T y) : x(x), y(y) {}

	//~Vector2() {}

	float GetSqrDistance(const Vector2<T> &v) const
	{
		T tx = x - v.x;
		T ty = y - v.y;
		return tx * tx + ty * ty;
	}

	Vector2<T> operator-(const Vector2<T> &v) const
	{
		return Vector2<T>(x - v.x, y - v.y);
	}

	void Normalize()
	{
		float length = sqrtf(x*x + y*y);
		length = fEpsilon > length ? fEpsilon : length;
		x /= length;
		y /= length;
	}

	//! Dot product.
	float operator*(const Vector2<T> &v)
	{
		return x * v.x + y * v.y;

	}

	//! Dot product.
	bool operator==(const Vector2<T> &v) const
	{
		return (x == v.x && y == v.y);

	}

	T	  x;			//! x
	T	  y;			//! y

}; // Vector2

typedef Vector2<float> Vector2F;
typedef Vector2<int>   Vector2I;

} // math

} // kge

#endif // VECTOR_H