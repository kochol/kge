// File name: Frustum.h
// Des: This class is for calculate frustum views.
// Date: 16/11/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Plane.h"

namespace kge
{
namespace math
{

enum FrustumSide
{
	//! The RIGHT side of the frustum
	EFS_Right	= 0,		
	//! The LEFT side of the frustum
	EFS_Left	= 1,		
	//! The BOTTOM side of the frustum
	EFS_Bottom	= 2,		
	//! The TOP side of the frustum
	EFS_Top		= 3,		
	//! The far side of the frustum
	EFS_Far		= 4,		
	//! The near side of the frustum
	EFS_Near	= 5			

}; // FrustumSide

class KGE_API Frustum
{
public:

	//! Constructor
	Frustum();

	//! Destructor
	~Frustum();

	//! Checks the frustum collision with sphere and returns ECT_In or ECT_Out
	CollisionType Collision(const Sphere* pSphere) const;

	//! Checks the frustum collision with sphere
	bool TestSphere(const Sphere* pSphere) const;

	//! Checks the frustum collision with AABB and returns ECT_In or ECT_Out
	CollisionType Collision(const AABB* pBox) const;

	/*! Create the frustum culling with camera matrices.
		\param pMatrixView The camera's view matrix.
		\param pMatrixProjection The camera's projection matrix.
	 */
	virtual void Init(const Matrix* pMatrixView, const Matrix* pMatrixProjection);

	//! Returns the point which is on the far left upper corner inside the the view frustum.
	Vector GetFarLeftUpPoint();

	//! Returns the point which is on the far left bottom corner inside the the view frustum.
	Vector GetFarLeftDownPoint();

	//! Returns the point which is on the far Right upper corner inside the the view frustum.
	Vector GetFarRightUpPoint();

	//! Returns the point which is on the far Right bottom corner inside the the view frustum.
	Vector GetFarRightDownPoint();

	//! Returns the bounding box
	AABB* GetBoundingBox();

	//! Returns the center of the frustum
	Vector* GetCenter();

	//! Returns the 8 frustum corners
	Vector* GetCorners();

	//! TestSweptSphere
	bool TestSweptSphere(Sphere& sphere, Vector& sweepDir) const;

	Vector* m_pCameraPosition;

protected:

	Plane			*	m_pSides;

	AABB			*	m_pAABB;

	bool				m_bCalcAABB;

	Matrix				m_mViewProj;

	Vector				m_vCorners[8],
						m_vCenter;

}; // Frustum

} // math

} // kge

#endif // FRUSTUM_H
