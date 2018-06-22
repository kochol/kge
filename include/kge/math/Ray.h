// File name: Ray.h
// Des: This class is for working with triangle math
// Date: 20/7/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_RAY_H
#define KGE_RAY_H

#include "math.h"
#include "Vector.h"
#include "../sn/Camera.h"

namespace kge
{
namespace math
{
struct Ray
{
	Vector  Position,
			Target,
			Direction;

	void Set(const Vector& vPosition, const Vector& vTarget)
	{
		Position  = vPosition;
		Target    = vTarget;
		Direction = Target - Position;

	} // Set

	void CalcPickingRay(int x, int y, sn::Camera* pCam)
	{
		Matrix m = pCam->GetProjectionMatrix();
		//core::RectI vp = pCam->GetViewPort();
		float fx, fy;
		fx = (2.0f * x / pCam->GetViewportWidth() - 1.0f) / m.m_fMat[0];//(vp.Width / 2.0f);//
		fy = (-2.0f * y / pCam->GetViewportHeight() + 1.0f) / m.m_fMat[5];//(vp.Height / -2.0f);//
		
		Position.set(0.0f, 0.0f, 0.0f);
		Target.set(fx, fy, 1.0f);
		Direction.set(fx, fy, 1.0f);
		
	} // CalcPickingRay

	//void TransformRay(Matrix* mat)
	//{

	//} // TransformRay

	void TransformRayInverse(const Matrix* mat)
	{
        kge::math::Matrix m = *mat;
		m.Inverse();
		m.TranslateVec(Position);
		m.RotateVec(Target);
		m.TranslateVec(Target);
		Direction = Target - Position;
		Target = Target + (Direction * 2000.0f);
		Direction = Target - Position;

	} // TransformRayInverse

}; // Ray

} // math

} // kge

#endif // KGE_RAY_H
