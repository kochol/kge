// File name: Frustum.h
// Des: This class is for calculate frustum views.
// Date: 16/11/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../include/kge/math/Frustum.h"
#include "../../include/kge/io/Logger.h"

#include "../../include/kge/math/Triangle.h"



namespace kge
{
namespace math
{
	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	Frustum::Frustum()
	{
		m_pSides = KGE_NEW_ARRAY(Plane, 6);
		m_pAABB  = KGE_NEW(AABB);

	} // Constructor

	//-----------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------
	Frustum::~Frustum()
	{
		KGE_DELETE_ARRAY(m_pSides);
		KGE_DELETE(m_pAABB, AABB);

	} // Destructor

	//---------------------------------------------------------------------------------------
	// Checks the frustum collision with sphere and returns ECT_In or ECT_Out.
	//---------------------------------------------------------------------------------------
	CollisionType Frustum::Collision(const Sphere* pSphere) const
	{
		CollisionType r;
		for ( int i = 0; i < 6; i++ )
		{
			r = m_pSides[i].Collision(pSphere);
			if ( r == ECT_Behind )
			{
				return ECT_Out;
			}
		}
		return ECT_In;

	} // Collision ( Sphere )

	//------------------------------------------------------------------------------------
	// Checks the frustum collision with sphere
	//------------------------------------------------------------------------------------
	bool Frustum::TestSphere( const Sphere* pSphere ) const
	{
		CollisionType r;
		for ( int i = 0; i < 6; i++ )
		{
			r = m_pSides[i].Collision(pSphere);
			if ( r == ECT_Behind )
			{
				return false;
			}
		}
		return true;

	} // TestSphere

	//---------------------------------------------------------------------------------------
	// Checks the frustum collision with AABB and returns ECT_In or ECT_Out.
	//---------------------------------------------------------------------------------------
	kge::math::CollisionType Frustum::Collision( const AABB* pBox ) const
	{

		CollisionType r, ct = ECT_In;
		for ( int i = 0; i < 6; i++ )
		{
			r = m_pSides[i].Collision(pBox);
			if ( r == ECT_Behind )
			{
				return ECT_Out;
			}
			else if (r == ECT_Intersect)
			{
				ct = r;
			}
		}
		return ct;

	} // Collision ( AABB )

	//-----------------------------------------------------------------------------
	// Create the frustum culling with camera matrices.
	//-----------------------------------------------------------------------------
	void Frustum::Init(const Matrix* pMatrixView, const Matrix* pMatrixProjection)
	{
		float* clip;

		m_mViewProj = (*pMatrixProjection) * (*pMatrixView);
		clip = &m_mViewProj.m_fMat[0];

		m_pSides[EFS_Right].Set(clip[ 3] - clip[ 0],
								clip[ 7] - clip[ 4],
								clip[11] - clip[ 8],
								clip[15] - clip[12]);
		m_pSides[EFS_Right].Normalize();

		m_pSides[EFS_Left].Set(clip[ 3] + clip[ 0],
							   clip[ 7] + clip[ 4],
							   clip[11] + clip[ 8],
							   clip[15] + clip[12]);
		m_pSides[EFS_Left].Normalize();

		m_pSides[EFS_Bottom].Set(clip[ 3] + clip[ 1],
								 clip[ 7] + clip[ 5],
								 clip[11] + clip[ 9],
								 clip[15] + clip[13]);
		m_pSides[EFS_Bottom].Normalize();

		m_pSides[EFS_Top].Set(clip[ 3] - clip[ 1],
							  clip[ 7] - clip[ 5],
							  clip[11] - clip[ 9],
							  clip[15] - clip[13]);
		m_pSides[EFS_Top].Normalize();

		m_pSides[EFS_Far].Set( clip[ 3] - clip[ 2],
							   clip[ 7] - clip[ 6],
							   clip[11] - clip[10],
							   clip[15] - clip[14]);
		m_pSides[EFS_Far].Normalize();

		m_pSides[EFS_Near].Set( clip[ 2],
								clip[ 6],
								clip[10],
								clip[14]);
		m_pSides[EFS_Near].Normalize();

		m_bCalcAABB = true;

	} // Init

	//-----------------------------------------------------------------------------
	// Returns the point which is on the far left upper corner inside the the view frustum.
	//-----------------------------------------------------------------------------
	Vector Frustum::GetFarLeftUpPoint()
	{
		Vector v;
		m_pSides[EFS_Far].GetIntersectionWithPlanes
			(&m_pSides[EFS_Top],
			 &m_pSides[EFS_Left],
			 v);

		return v;

	} // GetFarLeftUpPoint

	//-----------------------------------------------------------------------------
	// Returns the point which is on the far left bottom corner inside the the view frustum.
	//-----------------------------------------------------------------------------
	Vector Frustum::GetFarLeftDownPoint()
	{
		Vector v;
		m_pSides[EFS_Far].GetIntersectionWithPlanes
			(&m_pSides[EFS_Bottom],
			 &m_pSides[EFS_Left],
			 v);

		return v;

	} // GetFarLeftDownPoint

	//-----------------------------------------------------------------------------
	// Returns the point which is on the far Right upper corner inside the the view frustum.
	//-----------------------------------------------------------------------------
	Vector Frustum::GetFarRightUpPoint()
	{
		Vector v;
		m_pSides[EFS_Far].GetIntersectionWithPlanes
			(&m_pSides[EFS_Top],
			 &m_pSides[EFS_Right],
			 v);

		return v;

	} // GetFarRightUpPoint

	//-----------------------------------------------------------------------------
	// Returns the point which is on the far Right bottom corner inside the the view frustum.
	//-----------------------------------------------------------------------------
	Vector Frustum::GetFarRightDownPoint()
	{
		Vector v;
		m_pSides[EFS_Far].GetIntersectionWithPlanes
			(&m_pSides[EFS_Bottom],
			 &m_pSides[EFS_Right],
			 v);

		return v;

	} // GetFarRightDownPoint

	//-----------------------------------------------------------------------------
	// Returns the bounding box
	//-----------------------------------------------------------------------------
	AABB* Frustum::GetBoundingBox()
	{
		if (!m_bCalcAABB) { return m_pAABB; }

		Matrix mat = m_mViewProj;
		mat.Inverse();

		m_pAABB->m_vMin.set(-1, -1, -1);
		m_pAABB->m_vMax.set(1, 1, 1);
		m_pAABB->UpdateLines();
		for (int i = 0; i < 8; i++)
		{
			m_vCorners[i] = m_pAABB->v[i].pos.ToVector();
			mat.TransFormVec2(m_vCorners[i]);
		}

		// Calculate the center point and AABB
		m_pAABB->Reset();
		m_vCenter.set(0, 0, 0);
		for (int i = 0; i < 8; i++)
		{
			m_pAABB->AddInternalPoint(&m_vCorners[i]);
			m_vCenter += m_vCorners[i];
		}
		m_vCenter /= 8.0f;

		m_bCalcAABB = false;
		
		return m_pAABB;

	} // GetBoundingBox

	//-----------------------------------------------------------------------------
	// Returns the center of the frustum
	//-----------------------------------------------------------------------------
	Vector* Frustum::GetCenter()
	{
		GetBoundingBox();

		return &m_vCenter;

	} // GetCenter

	//-----------------------------------------------------------------------------
	// Returns the 8 frustum corners
	//-----------------------------------------------------------------------------
	Vector* Frustum::GetCorners()
	{
		GetBoundingBox();

		return m_vCorners;

	} // GetCorners

	//------------------------------------------------------------------------------------
	// TestSweptSphere
	//------------------------------------------------------------------------------------
	bool Frustum::TestSweptSphere( Sphere& sphere, Vector& sweepDir ) const
	{
		//  algorithm -- get all 12 intersection points of the swept sphere with the view frustum
		//  for all points >0, displace sphere along the sweep driection.  if the displaced sphere
		//  is inside the frustum, return TRUE.  else, return FALSE
		float displacements[12];
		int cnt = 0;
		float a, b;
		bool inFrustum = true;

		for (int i=0; i<6; i++)
		{
			if (SweptSpherePlaneIntersect(a, b, m_pSides[i], sphere, sweepDir))
			{
				if (a>=0.f)
					displacements[cnt++] = a;
				if (b>=0.f)
					displacements[cnt++] = b;
			}
		}

		for (int i=0; i<cnt; i++)
		{
			Sphere displacedSphere(sphere);
			displacedSphere.m_vCenter += sweepDir * displacements[i];
			displacedSphere.m_fRadius *= 1.1f;
			if (Collision(&displacedSphere) != ECT_In)
				return false;
		}
		return inFrustum;

	} // TestSweptSphere

} // math

} // kge
