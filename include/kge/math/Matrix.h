// File name: Matrix.h
// Des: In class Matrix haye 4x4 hast in matrix ha dar graphic 3D karhaye ziadi ro
//		anjam midan.
// Date: 26/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef MATRIX_H
#define MATRIX_H

#include "../kgedef.h"
#include "../../Include/math/Vector.h"

namespace kge
{
namespace gfx
{
// Az ghabl tarif mikonim.
struct Vertex3;
struct Vertex2;

} // gfx

namespace math
{
// Pre defines.
class Sphere;
class AABB;

class KGE_API Matrix
{
public:

	union
	{
//		float m[4][4];
		float m_fMat[16];
		struct
		{
			float _11,_12,_13,_14;
			float _21,_22,_23,_24;
			float _31,_32,_33,_34;
			float _41,_42,_43,_44;
		};
	};

	//! Constructor: Identity the matrix.
	Matrix() :
		_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
		_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
		_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
		_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
		{}
	
	//! Sazande ba float*
	Matrix(float *f);

	//! sazande ba 16 ta float
	Matrix (float f11, float f12, float f13, float f14,
			float f21, float f22, float f23, float f24, 
			float f31, float f32, float f33, float f34,
			float f41, float f42, float f43, float f44);

	//! make identity matrix
	void Identity()
	{
		_12 = _13 = _14 = _21 = _23 = _24 = _31 = _32 = _34 = _41 = _42 = _43 = 0.0f;
		_11 = _22 = _33 = _44 = 1.0f;
	}

	float operator[] (u32 i) const
	{
		return m_fMat[i];
	}

	//! Makes the inverse of this matrix.
	bool Inverse();

	//! Create the inverse of this matrix
	bool GetInverse(Matrix& m);

	//! Zarb do ta matrix dar ham.
	const Matrix operator *(const Matrix &m) const;

	//! Zarb do ta matrix dar ham.
	Matrix& operator *=(const Matrix &m);

	//! Tranform a vector with this matrix.
	Vector operator * (const Vector &v) const;

	float& operator()(int row, int col) { return m_fMat[row * 4 + col]; }

	//! Baraye jabe jaee noghat az in estefade mishe.
	void Translate(float dx, float dy, float dz);
	
	//! Chrkhesh dore mehvare Y ha.
	void RotY(float angle);

	//! Sets matrix elements
	void Set(	float m11, float m12, float m13, float m14,
				float m21, float m22, float m23, float m24,
				float m31, float m32, float m33, float m34,
				float m41, float m42, float m43, float m44 );

	//! Build a rotation matrix
	void SetRotation(const gfx::Vertex3* v3);

	//! Build a rotation matrix
	void SetRotation(const float* f);

	//! Set the translation values of the matrix.
	void SetTranslation(const gfx::Vertex3* v3);

	//! Get the translation values from the matrix.
	Vector GetTranslation();

	//! Set the translation values of the matrix.
	void SetTranslation(const float* f);

	//! Set the scale values of the matrix.
	void SetScale(const math::Vector& v);

	//! Transform a sphere.
	void TransformSphere(const Sphere& sphere, Sphere& result) const;

	//! Transform an AABB
	void TransformBox(const AABB& box, AABB& result) const;

	//! Transform an AABB
	void TransformBox2(const AABB& box, AABB& result) const;

	//! Translate a vector based on the inverse matrix.
	void InverseTranslateVec(gfx::Vertex3& v3) const;

	//! Rotate a vector using the inverse of the matrix.
	void InverseRotateVec(gfx::Vertex3& v3) const;

	//! Translate a vector based on the inverse matrix.
	void InverseTranslateVec(Vector& v3) const;

	//! Rotate a vector using the inverse of the matrix.
	void InverseRotateVec(Vector& v3) const;

	//! Translate a vector using this matrix.
	void TranslateVec(Vector& v3) const;

	//! Rotate a vector using this matrix.
	void RotateVec(Vector& v3) const;

	//! Transform a vector using this matrix
	void TransFormVec(Vector& v3) const
	{
		Vector tmp = v3;
		v3.x = tmp.x*m_fMat[0] + tmp.y*m_fMat[4] + tmp.z*m_fMat[8] + m_fMat[12];
		v3.y = tmp.x*m_fMat[1] + tmp.y*m_fMat[5] + tmp.z*m_fMat[9] + m_fMat[13];
		v3.z = tmp.x*m_fMat[2] + tmp.y*m_fMat[6] + tmp.z*m_fMat[10] + m_fMat[14];
	}

	//! Transform a vector using this matrix
	void TransFormVec2(Vector& v3) const;

	//! Transform a vector using this matrix
	void TransFormVec2(gfx::Vertex3& v3) const;

	//! Transform a vector using this matrix
	void TransFormVec2(gfx::Vertex3* v3Out, gfx::Vertex3* vIn, int size) const;

	//! Transform
//	void TransFormCoordVec(Vector& v3) const;

	//! Transform a 2D vector
	void Transform2DVec(kge::gfx::Vertex2& v2) const;

	//! Copy operator
	const void operator =(const Matrix& rMat);

	//!  makes this a left handed Orthogonal projection matrix
	void SetOrthoLH(float width, float height, float znear, float zfar);

	//!  makes this a left handed Orthogonal projection matrix
	void SetOrthoOffscreenLH(float minx, float maxx, float miny, float maxy, float znear, float zfar);

	//! Sets the left hand perspective projection matrix
	void SetPerspectiveLH(float width, float height, float Fov, float Near, float Far);

	//! Create a view look at matrix
	void SetViewLookatLH(math::Vector &vPos, math::Vector &vPoint, 
		math::Vector &vUp);

	void Get4x3Matrix(float* pOut);

}; // Matrix

//! Transposes this matrix
void Transpose(Matrix& mat);

} // math

} // kge

#endif // MATRIX_H