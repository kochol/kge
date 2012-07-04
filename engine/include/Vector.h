#ifndef KGE_VECTOR_H
#define KGE_VECTOR_H

#include "kgedef.h"

namespace kge
{
	namespace math
	{
		//! The 4D vector class
		class KGE_API Vector
		{
		public:

			union 
			{
				struct  
				{
					float	x,
							y,
							z,
							w;
				};

				float		elements[4];

			}; // union

			//! New Vector x=y=z=0.0f,w=1.0f;
			Vector()
				: x(0.0f), y(0.0f), z(0.0f), w(1.0f) {}

			//! New Vector set the x, y, z
			Vector(float x, float y, float z, float w = 1.0f)
				: x(x), y(y), z(z), w(w) {}

			//! Set the x,y,z w(optional)
			void Set(float _x, float _y, float _z, float _w = 1.0f) 
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

			//! Returns the squared length of vector
			float GetSqrLength() const
			{
				return x*x + y*y + z*z;
			}

			//! Negative the vector
			void Negate()
			{
				x = -x;
				y = -y;
				z = -z;
				//w = -w;
			}

			//! Normalize the vector
			//! The normalized vector length is 1 and the direction is unchanged
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

			//! Minus two vector
			Vector operator-(const Vector &v) const
			{
				return Vector(x - v.x, y - v.y, z - v.z);
			}


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

			//! Cross two vector.
			void Cross(const Vector &v1, const Vector &v2)
			{
				x = v1.y * v2.z - v1.z * v2.y;
				y = v1.z * v2.x - v1.x * v2.z;
				z = v1.x * v2.y - v1.y * v2.x;
				w = 1.0f;
			}

		}; // Vector

	} // math

} // kge

#endif // KGE_VECTOR_H
