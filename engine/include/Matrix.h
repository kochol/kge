//###############################################################
//#	title:	4x4 Matrix
//#	file:	matrix.h
//#	date:	june-07-2010
//#	author:	Pooya Shahinfar
//###############################################################

#ifndef KGE_MATRIX_H
#define KGE_MATRIX_H

#include "kgedef.h"
#include "math.h"

namespace kge
{
	namespace math
	{
		class KGE_API Matrix
		{
		public:

			//! matrix elements
			union
			{

				#if KGE_USE_SSE2
				struct
				{
					__m128 m1;
					__m128 m2;
					__m128 m3;
					__m128 m4;
				};
				#endif

				float elements[16];

				struct
				{
					float m11, m12, m13, m14;
					float m21, m22, m23, m24;
					float m31, m32, m33, m34;
					float m41, m42, m43, m44;
				};

				struct
				{
					float row1[4];
					float row2[4];
					float row3[4];
					float row4[4];
				};

			}; // union

			//! default constructor
			Matrix();

			//! constructors
			Matrix(const float*);
			Matrix( float _11, float _12, float _13, float _14,
					float _21, float _22, float _23, float _24,
					float _31, float _32, float _33, float _34,
					float _41, float _42, float _43, float _44);

			//! copy-constructor
			Matrix (const Matrix&);

			//! destructor
			~Matrix();

			//!	calculates the determinant of matrix
			float determinant(void);

			//! load identity matrix
			void loadIdentity(void);

			//! load zero matrix
			void loadZero(void);

			//! inverse the matrix
			void inverse(void);

			//! transpose matrix
			void transpose(void);

			//! set matrix values in row-major order
			void set(	float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44 );
			void set( const float*);

			#if KGE_USE_SSE2
				void set(const __m128& ,const __m128&, const __m128&,const __m128& );
			#endif

			// operator ()
			float operator()(int row,int column) const;

			/*! operator *
				Multiplies two matrices together
			*/
			Matrix operator*(const Matrix&) const;
			Matrix& operator*= (const Matrix&);

			//! operator + , Adds two matrices together
			Matrix operator+(const Matrix&) const;
			Matrix& operator+=(const Matrix&);

			//! operator - , Subtracts two matrices from each other
			Matrix operator-(const Matrix&) const;
			Matrix& operator-=(const Matrix&);

			//!	returns the floating point array of the matrix for external purposes
			operator const float*() const { return elements; }
			operator float*() { return elements; }

		} ; // Matrix

	} // math

} // kge

#endif // KGE_MATRIX_H
