//###############################################################
//#	file:	matrix.cpp
//#	date:	june-07-2010
//#	author:	Pooya Shahinfar
//###############################################################

#include "../include/Matrix.h"
#include <cmath>
#include <string.h>

namespace kge
{
	namespace math
	{

		// prototypes
		KGE_FORCE_INLINE void MatrixMul(Matrix &,const Matrix&,const Matrix&);


		//------------------------------------------------------------------------------------
		// constructor
		//------------------------------------------------------------------------------------
		Matrix::Matrix()
		{
			// load zero matrix
#if KGE_USE_SSE2
			m1 = m2 = m3 = m4 = _mm_setzero_ps();
#else
			memset(elements,0,sizeof(float) * 16);
#endif
		}

		//------------------------------------------------------------------------------------
		// constructor - float*
		//------------------------------------------------------------------------------------
		Matrix::Matrix(const float* mat)
		{
			memcpy(elements,mat,sizeof (float) * 16 );
		}

		// constructor
		Matrix::Matrix( float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44)
		{
			m11 = _11; m12 = _12; m13 = _13; m14 = _14;
			m21 = _21; m22 = _22; m23 = _23; m24 = _24;
			m31 = _31; m32 = _32; m33 = _33; m34 = _34;
			m41 = _41; m42 = _42; m43 = _43; m44 = _44;
		}

		// copy constructor
		Matrix::Matrix(const Matrix &mat)
		{
#if KGE_USE_SSE2 || KGE_USE_SSE4
			m1 = mat.m1;
			m2 = mat.m2;
			m3 = mat.m3;
			m4 = mat.m4;
#else
			memcpy(elements,mat.elements,sizeof(float) * 16);
#endif
		}

		// destructor
		Matrix::~Matrix()
		{
		}

		// Determinant
		float Matrix::Determinant(void)
		{
			return
				(m11 * m22 - m12 * m21) * (m33 * m44 - m34 * m43)
				-(m11 * m23 - m13 * m21) * (m32 * m44 - m34 * m42)
				+(m11 * m24 - m14 * m21) * (m32 * m43 - m33 * m42)
				+(m12 * m23 - m13 * m22) * (m31 * m44 - m34 * m41)
				-(m12 * m24 - m14 * m22) * (m31 * m43 - m33 * m41)
				+(m13 * m24 - m14 * m23) * (m31 * m42 - m32 * m41);
		}

		// LoadIdentity
		void Matrix::LoadIdentity(void)
		{
			memset(elements,0,sizeof(float) * 16);
			m11 = m22 = m33 = m44 = 1.0f;
		}

		// LoadZero
		void Matrix::LoadZero(void)
		{
#if KGE_USE_SSE2
			m1 = m2 = m3 = m4 = _mm_setzero_ps();
#else
			memset(elements,0,sizeof(float) * 16);
#endif
		}

		// Inverse
		void Matrix::Inverse(void)
		{
#if KGE_USE_SSE2
			// Code taken from Intel pdf "Streaming SIMD Extension - Inverse of 4x4 Matrix"
			float* src = elements;
			__m128 minor0, minor1, minor2, minor3;
			__m128 row0, row1, row2, row3;
			__m128 det, tmp1;

			tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src)), (__m64*)(src+ 4));
			row1 = _mm_loadh_pi(_mm_loadl_pi(row1, (__m64*)(src+8)), (__m64*)(src+12));

			row0 = _mm_shuffle_ps(tmp1, row1, 0x88);
			row1 = _mm_shuffle_ps(row1, tmp1, 0xDD);

			tmp1 = _mm_loadh_pi(_mm_loadl_pi(tmp1, (__m64*)(src+ 2)), (__m64*)(src+ 6));
			row3 = _mm_loadh_pi(_mm_loadl_pi(row3, (__m64*)(src+10)), (__m64*)(src+14));

			row2 = _mm_shuffle_ps(tmp1, row3, 0x88);
			row3 = _mm_shuffle_ps(row3, tmp1, 0xDD);

			tmp1 = _mm_mul_ps(row2, row3);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

			minor0 = _mm_mul_ps(row1, tmp1);
			minor1 = _mm_mul_ps(row0, tmp1);

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor0 = _mm_sub_ps(_mm_mul_ps(row1, tmp1), minor0);
			minor1 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor1);
			minor1 = _mm_shuffle_ps(minor1, minor1, 0x4E);

			tmp1 = _mm_mul_ps(row1, row2);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

			minor0 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor0);
			minor3 = _mm_mul_ps(row0, tmp1);

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row3, tmp1));
			minor3 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor3);
			minor3 = _mm_shuffle_ps(minor3, minor3, 0x4E);

			tmp1 = _mm_mul_ps(_mm_shuffle_ps(row1, row1, 0x4E), row3);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);
			row2 = _mm_shuffle_ps(row2, row2, 0x4E);

			minor0 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor0);
			minor2 = _mm_mul_ps(row0, tmp1);

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor0 = _mm_sub_ps(minor0, _mm_mul_ps(row2, tmp1));
			minor2 = _mm_sub_ps(_mm_mul_ps(row0, tmp1), minor2);
			minor2 = _mm_shuffle_ps(minor2, minor2, 0x4E);

			tmp1 = _mm_mul_ps(row0, row1);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

			minor2 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor2);
			minor3 = _mm_sub_ps(_mm_mul_ps(row2, tmp1), minor3);

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor2 = _mm_sub_ps(_mm_mul_ps(row3, tmp1), minor2);
			minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row2, tmp1));

			tmp1 = _mm_mul_ps(row0, row3);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

			minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row2, tmp1));
			minor2 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor2);

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor1 = _mm_add_ps(_mm_mul_ps(row2, tmp1), minor1);
			minor2 = _mm_sub_ps(minor2, _mm_mul_ps(row1, tmp1));

			tmp1 = _mm_mul_ps(row0, row2);
			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0xB1);

			minor1 = _mm_add_ps(_mm_mul_ps(row3, tmp1), minor1);
			minor3 = _mm_sub_ps(minor3, _mm_mul_ps(row1, tmp1));

			tmp1 = _mm_shuffle_ps(tmp1, tmp1, 0x4E);

			minor1 = _mm_sub_ps(minor1, _mm_mul_ps(row3, tmp1));
			minor3 = _mm_add_ps(_mm_mul_ps(row1, tmp1), minor3);

			det = _mm_mul_ps(row0, minor0);
			det = _mm_add_ps(_mm_shuffle_ps(det, det, 0x4E), det);
			det = _mm_add_ss(_mm_shuffle_ps(det, det, 0xB1), det);
			tmp1 = _mm_rcp_ss(det);

			det = _mm_sub_ss(_mm_add_ss(tmp1, tmp1), _mm_mul_ss(det, _mm_mul_ss(tmp1, tmp1)));
			det = _mm_shuffle_ps(det, det, 0x00);

			minor0 = _mm_mul_ps(det, minor0);
			_mm_storel_pi((__m64*)(src), minor0);
			_mm_storeh_pi((__m64*)(src+2), minor0);

			minor1 = _mm_mul_ps(det, minor1);
			_mm_storel_pi((__m64*)(src+4), minor1);
			_mm_storeh_pi((__m64*)(src+6), minor1);

			minor2 = _mm_mul_ps(det, minor2);
			_mm_storel_pi((__m64*)(src+ 8), minor2);
			_mm_storeh_pi((__m64*)(src+10), minor2);

			minor3 = _mm_mul_ps(det, minor3);
			_mm_storel_pi((__m64*)(src+12), minor3);
			_mm_storeh_pi((__m64*)(src+14), minor3);

#else
			float s = Determinant();
			if (s == 0.0) return;
			s = 1/s;
			this->Set(
				s*(m22*(m33*m44 - m34*m43) + m23*(m34*m42 - m32*m44) + m24*(m32*m43 - m33*m42)),
				s*(m32*(m13*m44 - m14*m43) + m33*(m14*m42 - m12*m44) + m34*(m12*m43 - m13*m42)),
				s*(m42*(m13*m24 - m14*m23) + m43*(m14*m22 - m12*m24) + m44*(m12*m23 - m13*m22)),
				s*(m12*(m24*m33 - m23*m34) + m13*(m22*m34 - m24*m32) + m14*(m23*m32 - m22*m33)),
				s*(m23*(m31*m44 - m34*m41) + m24*(m33*m41 - m31*m43) + m21*(m34*m43 - m33*m44)),
				s*(m33*(m11*m44 - m14*m41) + m34*(m13*m41 - m11*m43) + m31*(m14*m43 - m13*m44)),
				s*(m43*(m11*m24 - m14*m21) + m44*(m13*m21 - m11*m23) + m41*(m14*m23 - m13*m24)),
				s*(m13*(m24*m31 - m21*m34) + m14*(m21*m33 - m23*m31) + m11*(m23*m34 - m24*m33)),
				s*(m24*(m31*m42 - m32*m41) + m21*(m32*m44 - m34*m42) + m22*(m34*m41 - m31*m44)),
				s*(m34*(m11*m42 - m12*m41) + m31*(m12*m44 - m14*m42) + m32*(m14*m41 - m11*m44)),
				s*(m44*(m11*m22 - m12*m21) + m41*(m12*m24 - m14*m22) + m42*(m14*m21 - m11*m24)),
				s*(m14*(m22*m31 - m21*m32) + m11*(m24*m32 - m22*m34) + m12*(m21*m34 - m24*m31)),
				s*(m21*(m33*m42 - m32*m43) + m22*(m31*m43 - m33*m41) + m23*(m32*m41 - m31*m42)),
				s*(m31*(m13*m42 - m12*m43) + m32*(m11*m43 - m13*m41) + m33*(m12*m41 - m11*m42)),
				s*(m41*(m13*m22 - m12*m23) + m42*(m11*m23 - m13*m21) + m43*(m12*m21 - m11*m22)),
				s*(m11*(m22*m33 - m23*m32) + m12*(m23*m31 - m21*m33) + m13*(m21*m32 - m22*m31)));
#endif

		}

		// Transpose
		void Matrix::Transpose( void )
		{
#if KGE_USE_SSE2
			_MM_TRANSPOSE4_PS(m1,m2,m3,m4);
#else
#undef n_swap
#define n_swap(x,y) { float t=x; x=y; y=t; }
			n_swap(m12, m21);
			n_swap(m13, m31);
			n_swap(m14, m41);
			n_swap(m23, m32);
			n_swap(m24, m42);
			n_swap(m34, m43);
#endif
		}

		// Set
		void Matrix::Set(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44 )
		{
			m11 = _11; m12 = _12; m13 = _13; m14 = _14;
			m21 = _21; m22 = _22; m23 = _23; m24 = _24;
			m31 = _31; m32 = _32; m33 = _33; m34 = _34;
			m41 = _41; m42 = _42; m43 = _43; m44 = _44;
		}

		// Set - float*
		void Matrix::Set(const float* m)
		{
			memcpy(elements,m,sizeof(float) * 16);
		}

#if KGE_USE_SSE2
		void Matrix::Set( const __m128& _m1 ,const __m128& _m2, const __m128& _m3,const __m128& _m4)
		{
			m1 = _m1; m2 = _m2; m3 = _m3; m4 = _m4;
		}
#endif
		// operator ()
		float Matrix::operator()(int row,int column) const
		{
			return elements[(--row * 4) + --column];
		}

		// operator * , Matrix
		Matrix Matrix::operator*(const Matrix& mat) const
		{
			Matrix temp;
			MatrixMul(temp,*this,mat);
			return temp;
		}

		//------------------------------------------------------------------------------------
		// operator *= , Matrix
		//------------------------------------------------------------------------------------
		Matrix& Matrix::operator*=(const Matrix& right)
		{
			MatrixMul(*this,*this,right);
			return *this;
		}


		//------------------------------------------------------------------------------------
		// operator +
		//------------------------------------------------------------------------------------
		Matrix Matrix::operator+(const Matrix& m) const
		{
			Matrix temp;
			//temp.Set (m.m11 + m11,m_
			return temp;
		}

		//----------------------------------------------------------------------------------
		//  makes this a left handed Orthogonal projection matrix
		//----------------------------------------------------------------------------------
		void Matrix::SetOrthoLH( float width, float height, float znear, float zfar )
		{
			LoadIdentity();
			elements[0] = 2.0f / width;
			elements[5] = 2.0f / height;
			elements[10] = 1.0f / (zfar - znear);
			elements[14] = znear / (znear - zfar);

		} // SetOrthoLH

		//----------------------------------------------------------------------------------
		// Sets the left hand perspective projection matrix
		//----------------------------------------------------------------------------------
		void Matrix::SetPerspectiveLH( float width, float height, float Fov, float Near, float Far )
		{
			float ys = 1.0f / tanf(Fov * 0.5f);
			float xs = ys / (width / height);
			m12=m13=m14=m21=m23=m24=m31=m32=m41=m42=m44=0.0f;
			m11 = xs;
			m22 = ys;
			m33 = Far/(Far-Near);
			m34 = 1.0f;
			m43 = (-Near)*Far/(Far-Near);

		} // SetPerspectiveLH

		//----------------------------------------------------------------------------------
		//  makes this a left handed Orthogonal projection matrix
		//----------------------------------------------------------------------------------
		void Matrix::SetOrthoOffscreenLH( float minx, float maxx, float miny, float maxy, float znear, float zfar )
		{
			LoadIdentity();
			elements[ 0] = 2.0f / (maxx - minx);
			elements[ 5] = 2.0f / (maxy - miny);
			elements[10] = 1.0f / (zfar - znear);
			elements[12] = (minx + maxx) / (minx - maxx);
			elements[13] = (miny + maxy) / (miny - maxy);
			elements[14] = znear / (znear - zfar);

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
				vcY.Set(0.0f, 1.0f, 0.0f);

				vcTemp = Z * Z.y;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// take z axis if still too short
				if (fL < 1e-6f)
				{
					vcY.Set(0.0f, 0.0f, 1.0f);

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

			m11 = X.x;
			m12 = vcUp.x;
			m13 = Z.x;
			m21 = X.y;
			m22 = vcUp.y;
			m23 = Z.y;
			m31 = X.z;
			m32 = vcUp.z;
			m33 = Z.z;
			m41 = -(X * vPos);
			m42 = -(vcUp * vPos);
			m43 = -(Z * vPos);
			m44 = 1.0f;
			m14 = m24 = m34 = 0.0f;

		} // SetViewLookatLH

		//------------------------------------------------------------------------------------
		// mul , Matrix * Matrix
		//------------------------------------------------------------------------------------
		KGE_FORCE_INLINE void MatrixMul(Matrix &result,const Matrix& ma,const Matrix& mb)
		{
#if KGE_USE_SSE2

			result.Set(
				_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m1, ma.m1, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
				_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m2, ma.m2, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
				_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m3, ma.m3, _MM_SHUFFLE(3,3,3,3)), mb.m4)),
				_mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(0,0,0,0)), mb.m1), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(1,1,1,1)), mb.m2)), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(2,2,2,2)), mb.m3)), _mm_mul_ps(_mm_shuffle_ps(ma.m4, ma.m4, _MM_SHUFFLE(3,3,3,3)), mb.m4))
				);

#else
			result.Set(ma.m11*mb.m11 + ma.m12*mb.m21 + ma.m13*mb.m31 + ma.m14*mb.m41,
				ma.m11*mb.m12 + ma.m12*mb.m22 + ma.m13*mb.m32 + ma.m14*mb.m42,
				ma.m11*mb.m13 + ma.m12*mb.m23 + ma.m13*mb.m33 + ma.m14*mb.m43,
				ma.m11*mb.m14 + ma.m12*mb.m24 + ma.m13*mb.m34 + ma.m14*mb.m44,
				ma.m21*mb.m11 + ma.m22*mb.m21 + ma.m23*mb.m31 + ma.m24*mb.m41,
				ma.m21*mb.m12 + ma.m22*mb.m22 + ma.m23*mb.m32 + ma.m24*mb.m42,
				ma.m21*mb.m13 + ma.m22*mb.m23 + ma.m23*mb.m33 + ma.m24*mb.m43,
				ma.m21*mb.m14 + ma.m22*mb.m24 + ma.m23*mb.m34 + ma.m24*mb.m44,
				ma.m31*mb.m11 + ma.m32*mb.m21 + ma.m33*mb.m31 + ma.m34*mb.m41,
				ma.m31*mb.m12 + ma.m32*mb.m22 + ma.m33*mb.m32 + ma.m34*mb.m42,
				ma.m31*mb.m13 + ma.m32*mb.m23 + ma.m33*mb.m33 + ma.m34*mb.m43,
				ma.m31*mb.m14 + ma.m32*mb.m24 + ma.m33*mb.m34 + ma.m34*mb.m44,
				ma.m41*mb.m11 + ma.m42*mb.m21 + ma.m43*mb.m31 + ma.m44*mb.m41,
				ma.m41*mb.m12 + ma.m42*mb.m22 + ma.m43*mb.m32 + ma.m44*mb.m42,
				ma.m41*mb.m13 + ma.m42*mb.m23 + ma.m43*mb.m33 + ma.m44*mb.m43,
				ma.m41*mb.m14 + ma.m42*mb.m24 + ma.m43*mb.m34 + ma.m44*mb.m44);
#endif

		}

	} // math

} // kge
