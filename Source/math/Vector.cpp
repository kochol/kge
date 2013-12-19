// File name: Vector.cpp
// Des: in class baraye kar ba vector ha hast. be har noghte ee toye faza ye vector 
//		migan. baraye inke betavan az SSE dar mohasebat estefade kard in vector daraye
//		4 jahat hast x, y, z, w.
// Date: 25/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/math/Vector.h"
#include "../../Include/gfx/Structs.h"



extern bool g_bSSE;

namespace kge
{
namespace math
{

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	Vector::Vector(const gfx::Vertex3 &init )
		: x(init.X), y(init.Y), z(init.Z), w(1.0f)
	{
	} // Constructor
/*
	//-----------------------------------------------------------------------------
	// Returns the vector length
	//-----------------------------------------------------------------------------
	float Vector::GetLength()
	{
		float f;

		if(!g_bSSE)
		{
			f = (float)sqrt(x*x + y*y + z*z);
		}
		else
		{
			float *pf=&f;
			w=0.0f;
#ifdef WIN32
			__asm
			{
				mov ecx, pf			//point to result register
				mov esi, this		//vector U
				movups xmm0, [esi]	//vector U in xmm0
				mulps xmm0, xmm0	//multiply
				movaps xmm1, xmm0	//copy result
				shufps xmm1, xmm1, 4Eh	//shuffle: f1,f0,f3,f2
				addps xmm0, xmm1
				movaps xmm1, xmm0	//copy result
				shufps xmm1, xmm0, 11h	
				addps xmm0, xmm1
				sqrtss xmm0, xmm0	//square root
				movss [ecx], xmm0	//move result to f
			}
#endif // WIN32
			w = 1.0f;
		}

		return f;

	} // Get Length
	
	// ****** ******** ***** ****** ** ****** *
	// Badast avardane toole bordar be tavane 2
	// ****** ******** ***** ****** ** ****** *
	float Vector::GetSqrLength() const
	{
		return (x*x + y*y + z*z);

	} // Get Square Length

	// ****** ******* *** ** *******
	// jahate bordaro bar ax mikone.
	// ****** ******* *** ** *******
	void Vector::Negate()
	{
		x=-x;
		y=-y;
		z=-z;

	} // Negate

	// ****** ** ** ******* ****** ****** *******
	// bordar ra be bordare Normal tabdil mikone.
	// ****** ** ** ******* ****** ****** *******
	void Vector::Normalize()
	{
		//if(!g_bSSE)
		//{
			float f = (float)sqrt(x*x + y*y + z*z);
			if (f!=0.0f)
			{
				x/=f;
				y/=f;
				z/=f;
			}
//		}
//		else
//		{
//			w=0.0f;
//#ifdef WIN32
//			__asm
//			{
//				mov esi, this
//				movups xmm0,[esi]
//				movaps xmm2, xmm0
//				mulps xmm0, xmm0
//				movaps xmm1, xmm0
//				shufps xmm1,  xmm1, 4Eh
//				addps xmm0, xmm1
//				movaps xmm1, xmm0
//				shufps xmm1, xmm1, 11h
//				addps xmm0, xmm1
//	
//				rsqrtps xmm0, xmm0	; reciprocak square root
//				mulps xmm2, xmm0
//				movups [esi], xmm2
//			}
//#endif // WIN32
//			w=1.0f;
//		}
	} // Normalize

	// **** ** ** ******
	// Zarb do ta vector
	// **** ** ** ******
	float Vector::operator *(const Vector &v)
	{
		return (x*v.x + y*v.y + z*v.z);

	} // vector * vector = float

	// ******** ** ** *******
	// Tafrighe do ta vector.
	// ******** ** ** *******
	Vector Vector::operator-(const Vector &v)const
	{
		return Vector(x-v.x, y-v.y, z-v.z);

	} // Vector - Vector
*/
	//! Tafrighe do ta vector.
	Vector Vector::operator-(const gfx::Vertex3 &v)const
	{
		return Vector(x-v.X, y-v.Y, z-v.Z);
	}
/*
	//------------------------------------------------------------------------------------
	// Adds two vector.
	//------------------------------------------------------------------------------------
	Vector Vector::operator+(const Vector &v)const
	{
		return Vector(x+v.x, y+v.y, z+v.z);

	} // Vector - Vector

	//------------------------------------------------------------------------------------
	// Adds two vector.
	//------------------------------------------------------------------------------------
	void Vector::operator+=( const Vector &v )
	{
		x += v.x;
		y += v.y;
		z += v.z;

	} // += Vector

	//------------------------------------------------------------------------------------
	// Vector * float
	//------------------------------------------------------------------------------------
	Vector Vector::operator*(float f) const
	{
		return Vector(x*f, y*f, z*f);

	} // Vector * float
*/
	//------------------------------------------------------------------------------------
	// Dot product.
	//------------------------------------------------------------------------------------
	float Vector::operator*(const gfx::Vertex3 &v ) const
	{
		return x*v.X + y*v.Y + z*v.Z;

	} 	// Dot product ( Vertex3 )
/*
	// ** *****
	// /= float
	// ** *****
	void Vector::operator/=(float f)
	{
		x /= f;
		y /= f;
		z /= f;

	} // /= float

	// **** ******* ** ** ****** ** ***** ******* **** ***** *** ** ***
	// Zarb khareji do ta Vector ke hasel Vectori hast amood bar on do.
	// **** ******* ** ** ****** ** ***** ******* **** ***** *** ** ***
	void Vector::Cross(const Vector &v1, const Vector &v2)
	{
		x=v1.y*v2.z-v1.z*v2.y;
		y=v1.z*v2.x-v1.x*v2.z;
		z=v1.x*v2.y-v1.y*v2.x;
		w=1.0f;

	} // Cross
*/
	//------------------------------------------------------------------------------------
	// Cross two vertex3 
	//------------------------------------------------------------------------------------
	void Vector::Cross( const gfx::Vertex3 &v1, const gfx::Vertex3 &v2 )
	{
		x=v1.Y*v2.Z-v1.Z*v2.Y;
		y=v1.Z*v2.X-v1.X*v2.Z;
		z=v1.X*v2.Y-v1.Y*v2.X;
		w=1.0f;

	} // Cross(Vertex3)

} // math

} // kge