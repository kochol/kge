// File name: Matrix.cpp
// Des: In class Matrix haye 4x4 hast in matrix ha dar graphic 3D karhaye ziadi ro
//		anjam midan.
// Date: 26/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol) , Pooya Shahinfar (Pswin)

#include "../../Include/math/Matrix.h"
#include "../../Include/math/Sphere.h"
#include "../../Include/math/AABB.h"
#include "../../Include/gfx/Structs.h"
#include <math.h>
#include <memory.h>
#include <cfloat>



extern bool g_bSSE;

namespace kge
{
namespace math
{
	//------------------
	// Sazande ba float*
	//------------------
	Matrix::Matrix(float *f)
	{
		memcpy(m_fMat, f, sizeof(float[16]));

	} // Matrix(float* f)

	//-----------------------
	// sazande ba 16 ta float
	//-----------------------
	Matrix::Matrix (float f11, float f12, float f13, float f14,
					float f21, float f22, float f23, float f24, 
					float f31, float f32, float f33, float f34,
					float f41, float f42, float f43, float f44)
	{
		m_fMat[0] = f11;
		m_fMat[1] = f12;
		m_fMat[2] = f13;
		m_fMat[3] = f14;
		m_fMat[4] = f21;
		m_fMat[5] = f22;
		m_fMat[6] = f23;
		m_fMat[7] = f24;
		m_fMat[8] = f31;
		m_fMat[9] = f32;
		m_fMat[10] = f33;
		m_fMat[11] = f34;
		m_fMat[12] = f41;
		m_fMat[13] = f42;
		m_fMat[14] = f43;
		m_fMat[15] = f44;
	} // Matrix(16 ta float)

	//----------------------------------------------------------------------------------
	// make identity matrix
	//----------------------------------------------------------------------------------
	//void Matrix::Identity()
	//{
	//	memset(m_fMat, 0, sizeof(Matrix));
	//	_11=_22=_33=_44=1;

	//} // Identity

	//----------------------------------------------------------------------------------
	// Makes the inverse of this matrix.
	//----------------------------------------------------------------------------------
	bool Matrix::Inverse()
	{
		Matrix temp;

		if (GetInverse(temp))
		{
			*this = temp;
			return true;
		}

		return false;

	} // Inverse

	//----------------------------------------------------------------------------------
	// Create the inverse of this matrix
	//----------------------------------------------------------------------------------
	bool Matrix::GetInverse( Matrix& out )
	{
		Matrix &m = *this;

		float d = (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) -
			(m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) +
			(m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)) +
			(m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) -
			(m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) +
			(m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0));

		if (d == 0.f)
			return false;

		d = 1.f / d;

		out(0, 0) = d * (m(1, 1) * (m(2, 2) * m(3, 3) - m(2, 3) * m(3, 2)) + m(1, 2) * (m(2, 3) * m(3, 1) - m(2, 1) * m(3, 3)) + m(1, 3) * (m(2, 1) * m(3, 2) - m(2, 2) * m(3, 1)));
		out(0, 1) = d * (m(2, 1) * (m(0, 2) * m(3, 3) - m(0, 3) * m(3, 2)) + m(2, 2) * (m(0, 3) * m(3, 1) - m(0, 1) * m(3, 3)) + m(2, 3) * (m(0, 1) * m(3, 2) - m(0, 2) * m(3, 1)));
		out(0, 2) = d * (m(3, 1) * (m(0, 2) * m(1, 3) - m(0, 3) * m(1, 2)) + m(3, 2) * (m(0, 3) * m(1, 1) - m(0, 1) * m(1, 3)) + m(3, 3) * (m(0, 1) * m(1, 2) - m(0, 2) * m(1, 1)));
		out(0, 3) = d * (m(0, 1) * (m(1, 3) * m(2, 2) - m(1, 2) * m(2, 3)) + m(0, 2) * (m(1, 1) * m(2, 3) - m(1, 3) * m(2, 1)) + m(0, 3) * (m(1, 2) * m(2, 1) - m(1, 1) * m(2, 2)));
		out(1, 0) = d * (m(1, 2) * (m(2, 0) * m(3, 3) - m(2, 3) * m(3, 0)) + m(1, 3) * (m(2, 2) * m(3, 0) - m(2, 0) * m(3, 2)) + m(1, 0) * (m(2, 3) * m(3, 2) - m(2, 2) * m(3, 3)));
		out(1, 1) = d * (m(2, 2) * (m(0, 0) * m(3, 3) - m(0, 3) * m(3, 0)) + m(2, 3) * (m(0, 2) * m(3, 0) - m(0, 0) * m(3, 2)) + m(2, 0) * (m(0, 3) * m(3, 2) - m(0, 2) * m(3, 3)));
		out(1, 2) = d * (m(3, 2) * (m(0, 0) * m(1, 3) - m(0, 3) * m(1, 0)) + m(3, 3) * (m(0, 2) * m(1, 0) - m(0, 0) * m(1, 2)) + m(3, 0) * (m(0, 3) * m(1, 2) - m(0, 2) * m(1, 3)));
		out(1, 3) = d * (m(0, 2) * (m(1, 3) * m(2, 0) - m(1, 0) * m(2, 3)) + m(0, 3) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) + m(0, 0) * (m(1, 2) * m(2, 3) - m(1, 3) * m(2, 2)));
		out(2, 0) = d * (m(1, 3) * (m(2, 0) * m(3, 1) - m(2, 1) * m(3, 0)) + m(1, 0) * (m(2, 1) * m(3, 3) - m(2, 3) * m(3, 1)) + m(1, 1) * (m(2, 3) * m(3, 0) - m(2, 0) * m(3, 3)));
		out(2, 1) = d * (m(2, 3) * (m(0, 0) * m(3, 1) - m(0, 1) * m(3, 0)) + m(2, 0) * (m(0, 1) * m(3, 3) - m(0, 3) * m(3, 1)) + m(2, 1) * (m(0, 3) * m(3, 0) - m(0, 0) * m(3, 3)));
		out(2, 2) = d * (m(3, 3) * (m(0, 0) * m(1, 1) - m(0, 1) * m(1, 0)) + m(3, 0) * (m(0, 1) * m(1, 3) - m(0, 3) * m(1, 1)) + m(3, 1) * (m(0, 3) * m(1, 0) - m(0, 0) * m(1, 3)));
		out(2, 3) = d * (m(0, 3) * (m(1, 1) * m(2, 0) - m(1, 0) * m(2, 1)) + m(0, 0) * (m(1, 3) * m(2, 1) - m(1, 1) * m(2, 3)) + m(0, 1) * (m(1, 0) * m(2, 3) - m(1, 3) * m(2, 0)));
		out(3, 0) = d * (m(1, 0) * (m(2, 2) * m(3, 1) - m(2, 1) * m(3, 2)) + m(1, 1) * (m(2, 0) * m(3, 2) - m(2, 2) * m(3, 0)) + m(1, 2) * (m(2, 1) * m(3, 0) - m(2, 0) * m(3, 1)));
		out(3, 1) = d * (m(2, 0) * (m(0, 2) * m(3, 1) - m(0, 1) * m(3, 2)) + m(2, 1) * (m(0, 0) * m(3, 2) - m(0, 2) * m(3, 0)) + m(2, 2) * (m(0, 1) * m(3, 0) - m(0, 0) * m(3, 1)));
		out(3, 2) = d * (m(3, 0) * (m(0, 2) * m(1, 1) - m(0, 1) * m(1, 2)) + m(3, 1) * (m(0, 0) * m(1, 2) - m(0, 2) * m(1, 0)) + m(3, 2) * (m(0, 1) * m(1, 0) - m(0, 0) * m(1, 1)));
		out(3, 3) = d * (m(0, 0) * (m(1, 1) * m(2, 2) - m(1, 2) * m(2, 1)) + m(0, 1) * (m(1, 2) * m(2, 0) - m(1, 0) * m(2, 2)) + m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0)));

		return true;

	} // GetInverse

	// set
	void Matrix::Set(
				float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44 )
	{
		_11 = m11; _12 = m12; _13 = m13; _14 = m14;
		_21 = m21; _22 = m22; _23 = m23; _24 = m24;
		_31 = m31; _32 = m32; _33 = m33; _34 = m34;
		_41 = m41; _42 = m42; _43 = m43; _44 = m44;
	}


	//----------------------------------------------------------------------------------
	// helper function for matrix multiplication
	//----------------------------------------------------------------------------------
	void mul(Matrix &result,const Matrix& m1,const Matrix& m2)
	{
		#if defined(KGE_USE_MSVC_ASM) && defined(KGE_USE_SSE2)

		//Shorten the syntax a bit
		const float *pm1 = m1.m_fMat;
		const float *pm2 = m2.m_fMat;
		float *rf = result.m_fMat;	

		// put transposed of m2 in xmm4-xmm7
		__asm	
			{
				mov			ebx, pm2
				movups		xmm4, [ebx]
				movups		xmm5, [ebx+0x10]
				movups		xmm6, [ebx+0x20]
				movups		xmm7, [ebx+0x30]

				mov			esi, pm1
				mov			edi, rf
				mov			ecx, 2			// process two vectors

			BM5_START:
				// process x
				movss		xmm1, [esi+0x00]
				movss		xmm3, [esi+0x10]
				shufps		xmm1, xmm1, 0x00
				prefetchnta	[esi+0x30]
				shufps		xmm3, xmm3, 0x00
				mulps		xmm1, xmm4
				prefetchnta [edi+0x30]
				mulps		xmm3, xmm4

				// process y
				movss		xmm0, [esi+0x04]
				movss		xmm2, [esi+0x14]
				shufps		xmm0, xmm0, 0x00
				shufps		xmm2, xmm2, 0x00
				mulps		xmm0, xmm5
				mulps		xmm2, xmm5
				addps		xmm1, xmm0
				addps		xmm3, xmm2

				// process z
				movss		xmm0, [esi+0x08]
				movss		xmm2, [esi+0x18]
				shufps		xmm0, xmm0, 0x00
				shufps		xmm2, xmm2, 0x00
				mulps		xmm0, xmm6
				mulps		xmm2, xmm6
				addps		xmm1, xmm0
				addps		xmm3, xmm2

				// process w (hiding some pointer increments between the
				// multiplies)
				movss		xmm0, [esi+0x0C]
				movss		xmm2, [esi+0x1C]
				shufps		xmm0, xmm0, 0x00
				shufps		xmm2, xmm2, 0x00
				mulps		xmm0, xmm7
				add			esi, 0x20
				mulps		xmm2, xmm7
				add			edi, 0x20
				addps		xmm1, xmm0
				addps		xmm3, xmm2

				// write output vectors to memory, and loop
				movups		[edi-0x20], xmm1
				movups		[edi-0x10], xmm3
				dec			ecx
				jnz			BM5_START
			}
		#else

			result.Set(
			(m1.m_fMat[0] * m2.m_fMat[0] + m1.m_fMat[4] * m2.m_fMat[1] + m1.m_fMat[8] * m2.m_fMat[2] + m1.m_fMat[12] * m2.m_fMat[3]),
			(m1.m_fMat[1] * m2.m_fMat[0] + m1.m_fMat[5] * m2.m_fMat[1] + m1.m_fMat[9] * m2.m_fMat[2] + m1.m_fMat[13] * m2.m_fMat[3]),
			(m1.m_fMat[2] * m2.m_fMat[0] + m1.m_fMat[6] * m2.m_fMat[1] + m1.m_fMat[10] * m2.m_fMat[2] + m1.m_fMat[14] * m2.m_fMat[3]),
			(m1.m_fMat[3] * m2.m_fMat[0] + m1.m_fMat[7] * m2.m_fMat[1] + m1.m_fMat[11] * m2.m_fMat[2] + m1.m_fMat[15] * m2.m_fMat[3]),

			(m1.m_fMat[0] * m2.m_fMat[4] + m1.m_fMat[4] * m2.m_fMat[5] + m1.m_fMat[8] * m2.m_fMat[6] + m1.m_fMat[12] * m2.m_fMat[7]),
			(m1.m_fMat[1] * m2.m_fMat[4] + m1.m_fMat[5] * m2.m_fMat[5] + m1.m_fMat[9] * m2.m_fMat[6] + m1.m_fMat[13] * m2.m_fMat[7]),
			(m1.m_fMat[2] * m2.m_fMat[4] + m1.m_fMat[6] * m2.m_fMat[5] + m1.m_fMat[10] * m2.m_fMat[6] + m1.m_fMat[14] * m2.m_fMat[7]),
			(m1.m_fMat[3] * m2.m_fMat[4] + m1.m_fMat[7] * m2.m_fMat[5] + m1.m_fMat[11] * m2.m_fMat[6] + m1.m_fMat[15] * m2.m_fMat[7]),

			(m1.m_fMat[0] * m2.m_fMat[8] + m1.m_fMat[4] * m2.m_fMat[9] + m1.m_fMat[8] * m2.m_fMat[10] + m1.m_fMat[12] * m2.m_fMat[11]),
			(m1.m_fMat[1] * m2.m_fMat[8] + m1.m_fMat[5] * m2.m_fMat[9] + m1.m_fMat[9] * m2.m_fMat[10] + m1.m_fMat[13] * m2.m_fMat[11]),
			(m1.m_fMat[2] * m2.m_fMat[8] + m1.m_fMat[6] * m2.m_fMat[9] + m1.m_fMat[10] * m2.m_fMat[10] + m1.m_fMat[14] * m2.m_fMat[11]),
			(m1.m_fMat[3] * m2.m_fMat[8] + m1.m_fMat[7] * m2.m_fMat[9] + m1.m_fMat[11] * m2.m_fMat[10] + m1.m_fMat[15] * m2.m_fMat[11]),

			(m1.m_fMat[0] * m2.m_fMat[12] + m1.m_fMat[4] * m2.m_fMat[13] + m1.m_fMat[8] * m2.m_fMat[14] + m1.m_fMat[12] * m2.m_fMat[15]),
			(m1.m_fMat[1] * m2.m_fMat[12] + m1.m_fMat[5] * m2.m_fMat[13] + m1.m_fMat[9] * m2.m_fMat[14] + m1.m_fMat[13] * m2.m_fMat[15]),
			(m1.m_fMat[2] * m2.m_fMat[12] + m1.m_fMat[6] * m2.m_fMat[13] + m1.m_fMat[10] * m2.m_fMat[14] + m1.m_fMat[14] * m2.m_fMat[15]),
			(m1.m_fMat[3] * m2.m_fMat[12] + m1.m_fMat[7] * m2.m_fMat[13] + m1.m_fMat[11] * m2.m_fMat[14] + m1.m_fMat[15] * m2.m_fMat[15])
			);

		#endif // SSE2 support

	} // mul


	// **** ** ** ****** *** ****
	// Zarb do ta matrix dar ham.
	// **** ** ** ****** *** ****
	const Matrix Matrix::operator *(const Matrix &mat) const
	{
		Matrix r;
		mul(r,*this,mat);
		return r;
	} // Matrix * Matrix

	//---------------------------
	// Zarb do ta matrix dar ham.
	//---------------------------
	Matrix& Matrix::operator *=(const Matrix &rMat)
	{
		mul(*this,*this,rMat);
		return *this;
	} // *= Matrix

	//----------------------------------------------------------------------------------
	// Tranform a vector with this matrix.
	//----------------------------------------------------------------------------------
	Vector Matrix::operator * (const Vector &v) const
	{
		Vector r;
		r.x    = v.x * _11 + v.y * _21 + v.z * _31 + v.w * _41;
		r.y    = v.x * _12 + v.y * _22 + v.z * _32 + v.w * _42;
		r.z    = v.x * _13 + v.y * _23 + v.z * _33 + v.w * _43;
		r.w    = v.x * _14 + v.y * _24 + v.z * _34 + v.w * _44;

		return r;
	
	} // * Vector

	// ****** **** **** ****** ** ** ******** ******
	// Baraye jabe jaee noghat az in estefade mishe.
	// ****** **** **** ****** ** ** ******** ******
	void Matrix::Translate(float dx, float dy, float dz)
	{
		_41=dx;
		_42=dy;
		_43=dz;
	} // Translate

	// ******** **** ******* * ***
	// Chrkhesh dore mehvare Y ha.
	// ******** **** ******* * ***
	void Matrix::RotY(float a)
	{
		float fCos = cosf(a);
		float fSin = sinf(a);
		_11 = _33 = fCos;
		_13 = -fSin;
		_31 = fSin;
		_22=_44=1.0f;
		_12=_23=_14=_21=_24=_32=_34=_41=_42=_43=0.0f;
	} // RotY

	// ***** * ******** *******
	// Build a rotation matrix.
	// ***** * ******** *******
	void Matrix::SetRotation(const gfx::Vertex3* v3)
	{
		double cx = cos(v3->X);
		double sx = sin(v3->X);
		double cy = cos(v3->Y);
		double sy = sin(v3->Y);
		double cz = cos(v3->Z);
		double sz = sin(v3->Z);

		m_fMat[0] = (float)(cy * cz);
		m_fMat[1] = (float)(cy * sz);
		m_fMat[2] = (float)(-sy);

		m_fMat[4] = (float)(sx * sy * cz - cx * sz);
		m_fMat[5] = (float)(sx * sy * sz + cx * cz);
		m_fMat[6] = (float)(sx * cy);

		m_fMat[8] = (float)(cx * sy * cz + sx * sz);
		m_fMat[9] = (float)(cx * sy * sz - sx * cz);
		m_fMat[10] = (float)(cx * cy);

		m_fMat[15] = 1.0f;
	} // SetRotation(Vertex3)

	//-------------------------
	// Build a rotation matrix.
	//-------------------------
	void Matrix::SetRotation(const float* f)
	{
		float cx = cosf(f[0]);
		float sx = sinf(f[0]);
		float cy = cosf(f[1]);
		float sy = sinf(f[1]);
		float cz = cosf(f[2]);
		float sz = sinf(f[2]);

		m_fMat[0] = (cy * cz);
		m_fMat[1] = (cy * sz);
		m_fMat[2] = (-sy);

		m_fMat[4] = (sx * sy * cz - cx * sz);
		m_fMat[5] = (sx * sy * sz + cx * cz);
		m_fMat[6] = (sx * cy);

		m_fMat[8] = (cx * sy * cz + sx * sz);
		m_fMat[9] = (cx * sy * sz - sx * cz);
		m_fMat[10] = (cx * cy);

		m_fMat[15] = 1.0f;

	} // SetRotation(float*)

	// *** *** *********** ****** ** *** *******
	// Set the translation values of the matrix.
	// *** *** *********** ****** ** *** *******
	void Matrix::SetTranslation(const gfx::Vertex3* v3)
	{
		m_fMat[12] = v3->X;
		m_fMat[13] = v3->Y;
		m_fMat[14] = v3->Z;
	} // SetTranslation(Vertex3)

	//------------------------------------------
	// Set the translation values of the matrix.
	//------------------------------------------
	void Matrix::SetTranslation(const float* f)
	{
		m_fMat[12] = f[0];
		m_fMat[13] = f[1];
		m_fMat[14] = f[2];

	} // SetTranslation(float*)

	//------------------------------------------------------------------------------------
	// Get the translation vaBlues from the matrix.
	//------------------------------------------------------------------------------------
	Vector Matrix::GetTranslation()
	{
		return Vector(m_fMat[12], m_fMat[13], m_fMat[14]);

	} // GetTranslation

	//------------------------------------
	// Set the scale values of the matrix.
	//------------------------------------
	void Matrix::SetScale(const math::Vector& v)
	{
		m_fMat[0] = v.x;
		m_fMat[5] = v.y;
		m_fMat[10] = v.z;

	} // SetScale

	//----------------------------------------------------------------------------------
	// Transform a sphere.
	//----------------------------------------------------------------------------------
	void Matrix::TransformSphere(const Sphere& sphere, Sphere& result) const
	{
		result.SetCenter((*this) * sphere.GetCenter());
		result.SetRadius(sphere.GetRadius());

	} // TransformSphere

	//----------------------------------------------------------------------------------
	// Transform an AABB
	//----------------------------------------------------------------------------------
	void Matrix::TransformBox( const AABB& box, AABB& result ) const
	{
		result.m_vMax = Vector(FLT_MIN, FLT_MIN, FLT_MIN);
		result.m_vMin = Vector(FLT_MAX, FLT_MAX, FLT_MAX);

		Vector v[8] = {
			box.m_vMin,
			Vector(box.m_vMin.x, box.m_vMax.y, box.m_vMin.z),
			Vector(box.m_vMax.x, box.m_vMin.y, box.m_vMin.z),
			Vector(box.m_vMax.x, box.m_vMax.y, box.m_vMin.z),
			Vector(box.m_vMin.x, box.m_vMin.y, box.m_vMax.z),
			Vector(box.m_vMax.x, box.m_vMin.y, box.m_vMax.z),
			Vector(box.m_vMin.x, box.m_vMax.y, box.m_vMax.z),
			box.m_vMax };

		for (int i=0; i<8; i++)
		{
			TransFormVec(v[i]);

			if (result.m_vMax.x < v[i].x) { result.m_vMax.x = v[i].x; }
			if (result.m_vMax.y < v[i].y) { result.m_vMax.y = v[i].y; }
			if (result.m_vMax.z < v[i].z) { result.m_vMax.z = v[i].z; }

			if (result.m_vMin.x > v[i].x) { result.m_vMin.x = v[i].x; }
			if (result.m_vMin.y > v[i].y) { result.m_vMin.y = v[i].y; }
			if (result.m_vMin.z > v[i].z) { result.m_vMin.z = v[i].z; }
		}

	} // TransformBox

	//----------------------------------------------------------------------------------
	// Transform an AABB
	//----------------------------------------------------------------------------------
	void Matrix::TransformBox2( const AABB& box, AABB& result ) const
	{
		result.m_vMax = Vector(FLT_MIN, FLT_MIN, FLT_MIN);
		result.m_vMin = Vector(FLT_MAX, FLT_MAX, FLT_MAX);

		Vector v[8] = {
			box.m_vMin,
			Vector(box.m_vMin.x, box.m_vMax.y, box.m_vMin.z),
			Vector(box.m_vMax.x, box.m_vMin.y, box.m_vMin.z),
			Vector(box.m_vMax.x, box.m_vMax.y, box.m_vMin.z),
			Vector(box.m_vMin.x, box.m_vMin.y, box.m_vMax.z),
			Vector(box.m_vMax.x, box.m_vMin.y, box.m_vMax.z),
			Vector(box.m_vMin.x, box.m_vMax.y, box.m_vMax.z),
			box.m_vMax };

		for (int i=0; i<8; i++)
		{
			TransFormVec2(v[i]);

			if (result.m_vMax.x < v[i].x) { result.m_vMax.x = v[i].x; }
			if (result.m_vMax.y < v[i].y) { result.m_vMax.y = v[i].y; }
			if (result.m_vMax.z < v[i].z) { result.m_vMax.z = v[i].z; }

			if (result.m_vMin.x > v[i].x) { result.m_vMin.x = v[i].x; }
			if (result.m_vMin.y > v[i].y) { result.m_vMin.y = v[i].y; }
			if (result.m_vMin.z > v[i].z) { result.m_vMin.z = v[i].z; }
		}

	} // TransformBox2

	//----------------------------------------------------------------------------------
	// Translate a vector based on the inverse matrix.
	//----------------------------------------------------------------------------------
	void Matrix::InverseTranslateVec(gfx::Vertex3& v3) const
	{
		v3.X -= m_fMat[12];
		v3.Y -= m_fMat[13];
		v3.Z -= m_fMat[14];

	} // InverseTranslateVec(Vertex3)

	//----------------------------------------------------------------------------------
	// Rotate a vector using the inverse of the matrix.
	//----------------------------------------------------------------------------------
	void Matrix::InverseRotateVec(gfx::Vertex3& v3) const
	{
		gfx::Vertex3 v = v3;
		v3.X = v.X * m_fMat[0] + v.Y * m_fMat[1] + v.Z * m_fMat[2];
		v3.Y = v.X * m_fMat[4] + v.Y * m_fMat[5] + v.Z * m_fMat[6];
		v3.Z = v.X * m_fMat[8] + v.Y * m_fMat[9] + v.Z * m_fMat[10];

	} // InverseRotateVec(Vertex3)

	//----------------------------------------------------------------------------------
	// Translate a vector based on the inverse matrix.
	//----------------------------------------------------------------------------------
	void Matrix::InverseTranslateVec(Vector& v3) const
	{
		v3.x -= m_fMat[12];
		v3.y -= m_fMat[13];
		v3.z -= m_fMat[14];

	} // InverseTranslateVec(Vertex3)

	//----------------------------------------------------------------------------------
	// Rotate a vector using the inverse of the matrix.
	//----------------------------------------------------------------------------------
	void Matrix::InverseRotateVec(Vector& v3) const
	{
		Vector v = v3;
		v3.x = v.x * m_fMat[0] + v.y * m_fMat[1] + v.z * m_fMat[2];
		v3.y = v.x * m_fMat[4] + v.y * m_fMat[5] + v.z * m_fMat[6];
		v3.z = v.x * m_fMat[8] + v.y * m_fMat[9] + v.z * m_fMat[10];

	} // InverseRotateVec(Vertex3)

	//----------------------------------------------------------------------------------
	// Translate a vector using this matrix.
	//----------------------------------------------------------------------------------
	void Matrix::TranslateVec( Vector& v3 ) const
	{
		v3.x = v3.x + m_fMat[12];
		v3.y = v3.y + m_fMat[13];
		v3.z = v3.z + m_fMat[14];

	} // TranslateVec

	//----------------------------------------------------------------------------------
	// Rotate a vector using this matrix.
	//----------------------------------------------------------------------------------
	void Matrix::RotateVec( Vector& v3 ) const
	{
		Vector tmp = v3;
		v3.x = tmp.x*m_fMat[0] + tmp.y*m_fMat[4] + tmp.z*m_fMat[8];
		v3.y = tmp.x*m_fMat[1] + tmp.y*m_fMat[5] + tmp.z*m_fMat[9];
		v3.z = tmp.x*m_fMat[2] + tmp.y*m_fMat[6] + tmp.z*m_fMat[10];

	} // RotateVec

	//----------------------------------------------------------------------------------
	// Transform a vector using this matrix
	//----------------------------------------------------------------------------------
	//void Matrix::TransFormVec( Vector& v3 ) const
	//{
	//	Vector tmp = v3;
	//	v3.x = tmp.x*m_fMat[0] + tmp.y*m_fMat[4] + tmp.z*m_fMat[8] + m_fMat[12];
	//	v3.y = tmp.x*m_fMat[1] + tmp.y*m_fMat[5] + tmp.z*m_fMat[9] + m_fMat[13];
	//	v3.z = tmp.x*m_fMat[2] + tmp.y*m_fMat[6] + tmp.z*m_fMat[10] + m_fMat[14];

	//} // TransFormVec

	//----------------------------------------------------------------------------------
	// Transform a vector using this matrix
	//----------------------------------------------------------------------------------
	void Matrix::TransFormVec2( Vector& v3 ) const
	{
		double x = v3.x*m_fMat[0] + v3.y*m_fMat[4] + v3.z*m_fMat[8] + m_fMat[12];
		double y = v3.x*m_fMat[1] + v3.y*m_fMat[5] + v3.z*m_fMat[9] + m_fMat[13];
		double z = v3.x*m_fMat[2] + v3.y*m_fMat[6] + v3.z*m_fMat[10] + m_fMat[14];
		double w = v3.x*m_fMat[3] + v3.y*m_fMat[7] + v3.z*m_fMat[11] + m_fMat[15];

		v3.x = x / w;
		v3.y = y / w;
		v3.z = z / w;

	} // TransFormVec2

	void Matrix::TransFormVec2( gfx::Vertex3& v3 ) const
	{
		double X = v3.X*m_fMat[0] + v3.Y*m_fMat[4] + v3.Z*m_fMat[8] + m_fMat[12];
		double Y = v3.X*m_fMat[1] + v3.Y*m_fMat[5] + v3.Z*m_fMat[9] + m_fMat[13];
		double Z = v3.X*m_fMat[2] + v3.Y*m_fMat[6] + v3.Z*m_fMat[10] + m_fMat[14];
		double w = v3.X*m_fMat[3] + v3.Y*m_fMat[7] + v3.Z*m_fMat[11] + m_fMat[15];

		v3.X = X / w;
		v3.Y = Y / w;
		v3.Z = Z / w;
	}

	void Matrix::TransFormVec2( gfx::Vertex3* v3Out, gfx::Vertex3* v3, int size ) const
	{
		for (int i = 0; i < size; i++)
		{
			double X = v3[i].X*m_fMat[0] + v3[i].Y*m_fMat[4] + v3[i].Z*m_fMat[8] + m_fMat[12];
			double Y = v3[i].X*m_fMat[1] + v3[i].Y*m_fMat[5] + v3[i].Z*m_fMat[9] + m_fMat[13];
			double Z = v3[i].X*m_fMat[2] + v3[i].Y*m_fMat[6] + v3[i].Z*m_fMat[10] + m_fMat[14];
			double w = v3[i].X*m_fMat[3] + v3[i].Y*m_fMat[7] + v3[i].Z*m_fMat[11] + m_fMat[15];

			v3Out[i].X = X / w;
			v3Out[i].Y = Y / w;
			v3Out[i].Z = Z / w;
		}
	}
	//! Transform
//	void TransFormCoordVec(Vector& v3) const


	//----------------------------------------------------------------------------------
	// Copy the matrix
	//----------------------------------------------------------------------------------
	const void Matrix::operator =(const Matrix& rMat)
	{
		memcpy(m_fMat, rMat.m_fMat, sizeof(float[16]));

	} // operator =

	//----------------------------------------------------------------------------------
	// Transform a 2D vector	
	//----------------------------------------------------------------------------------
	void Matrix::Transform2DVec( gfx::Vertex2& v2 ) const
	{
		gfx::Vertex2 tmp = v2;
		v2.X = tmp.X*m_fMat[0] + tmp.Y*m_fMat[4] + m_fMat[12];
		v2.Y = tmp.X*m_fMat[1] + tmp.Y*m_fMat[5] + m_fMat[13];
	}

	//----------------------------------------------------------------------------------
	//  makes this a left handed Orthogonal projection matrix
	//----------------------------------------------------------------------------------
	void Matrix::SetOrthoLH( float width, float height, float znear, float zfar )
	{
		Identity();
		m_fMat[0] = 2.0f / width;
		m_fMat[5] = 2.0f / height;
		m_fMat[10] = 1.0f / (zfar - znear);
		m_fMat[14] = znear / (znear - zfar);

	} // SetOrthoLH

	//----------------------------------------------------------------------------------
	// Sets the left hand perspective projection matrix
	//----------------------------------------------------------------------------------
	void Matrix::SetPerspectiveLH( float width, float height, float Fov, float Near, float Far )
	{
		float ys = 1.0f / tanf(Fov * 0.5f);
		float xs = ys / (width / height);
		_12=_13=_14=_21=_23=_24=_31=_32=_41=_42=_44=0.0f;
		_11 = xs;
		_22 = ys;
		_33 = Far/(Far-Near);
		_34 = 1.0f;
		_43 = (-Near)*Far/(Far-Near);

	} // SetPerspectiveLH

	//----------------------------------------------------------------------------------
	//  makes this a left handed Orthogonal projection matrix
	//----------------------------------------------------------------------------------
	void Matrix::SetOrthoOffscreenLH( float minx, float maxx, float miny, float maxy, float znear, float zfar )
	{
		Identity();
		m_fMat[ 0] = 2.0f / (maxx - minx);
		m_fMat[ 5] = 2.0f / (maxy - miny);
		m_fMat[10] = 1.0f / (zfar - znear);
		m_fMat[12] = (minx + maxx) / (minx - maxx);
		m_fMat[13] = (miny + maxy) / (miny - maxy);
		m_fMat[14] = znear / (znear - zfar);

	} // SetOrthoOffscreenLH

	//------------------------------------------------------------------------------------
	// Create a view look at matrix
	//------------------------------------------------------------------------------------
	void Matrix::SetViewLookatLH( math::Vector &vPos, math::Vector &vPoint, math::Vector &vUp )
	{
		math::Vector Z = vPoint - vPos; // Z
		Z.Normalize();
		math::Vector X;					// X
		X.Cross(vUp, Z);
		X.Normalize();
		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = vUp * Z;
		vcTemp = Z * fDot;
		vcUp = vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis 
		if (fL < 1e-6f) 
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);

				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					return;
				}
			}
		}
		vcUp /= fL;

		_11 = X.x;
		_12 = vcUp.x;
		_13 = Z.x;
		_21 = X.y;
		_22 = vcUp.y;
		_23 = Z.y;
		_31 = X.z;
		_32 = vcUp.z;
		_33 = Z.z;
		_41 = -(X * vPos);
		_42 = -(vcUp * vPos);
		_43 = -(Z * vPos);
		_44 = 1.0f;
		_14 = _24 = _34 = 0.0f;

	} // SetViewLookatLH

	void Matrix::Get4x3Matrix( float* pOut )
	{
		memcpy(pOut, m_fMat, 12 * sizeof(float));
		pOut[3]  = m_fMat[12];
		pOut[7]  = m_fMat[13];
		pOut[11] = m_fMat[14];

	} // Get4x3Matrix

	//-------------------------
	// Transposes this matrix
	//-------------------------
	void Transpose(Matrix& mat)
	{
		mat.Set(
			mat._11, mat._21, mat._31, mat._41,
			mat._12, mat._22, mat._32, mat._42,
			mat._13, mat._23, mat._33, mat._43,
			mat._14, mat._24, mat._34, mat._44);
	}

} // math

} // kge