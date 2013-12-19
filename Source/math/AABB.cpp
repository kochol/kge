// File name: AABB.cpp
// Des: This class is for working with axis aligned bounding boxes.
// Date: 11/08/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/math/AABB.h"
#include "../../Include/math/Ray.h"
#include "../../Include/gfx/Renderer.h"

namespace kge
{
namespace math
{

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	AABB::AABB(): m_vMin(0,0,0), m_vMax(0,0,0), m_bFirst(true)
	{
		CreateIndices();
		UpdateLines();

		m_iType = KGE_SHAPE_AABB;
	} // Constructor (   )

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	AABB::AABB(const gfx::Vertex3& init ): m_vMin(init), m_vMax(init), m_bFirst(false)
	{
		CreateIndices();
		UpdateLines();

		m_iType = KGE_SHAPE_AABB;
	} // Constructor ( Vertex3 )

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	AABB::AABB( const Vector& init ): m_vMin(init), m_vMax(init), m_bFirst(false)
	{
		CreateIndices();
		UpdateLines();

		m_iType = KGE_SHAPE_AABB;

	} // Constructor ( Vector )

	//-----------------------------------------------------------------------------
	// Constructor
	//-----------------------------------------------------------------------------
	AABB::AABB( const AABB &init ): m_vMin(init.m_vMin), m_vMax(init.m_vMax), m_bFirst(false)
	{
		CreateIndices();
		UpdateLines();

		m_iType = KGE_SHAPE_AABB;
	} // Constructor ( AABB )

	//-----------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------
	AABB::~AABB()
	{

	} // Destructor

	//-----------------------------------------------------------------------------
	// Adds a point to bounding box
	//-----------------------------------------------------------------------------
	void AABB::AddInternalPoint( gfx::Vertex3* point )
	{
		if (m_bFirst)
		{
			m_bFirst = false;
			m_vMin.x = point->X;
			m_vMin.y = point->Y;
			m_vMin.z = point->Z;
			m_vMax.x = point->X;
			m_vMax.y = point->Y;
			m_vMax.z = point->Z;
			UpdateLines();
			return;
		}
		if (point->X < m_vMin.x)
			m_vMin.x = point->X;
		if (point->Y < m_vMin.y)
			m_vMin.y = point->Y;
		if (point->Z < m_vMin.z)
			m_vMin.z = point->Z;

		if (point->X > m_vMax.x)
			m_vMax.x = point->X;
		if (point->Y > m_vMax.y)
			m_vMax.y = point->Y;
		if (point->Z > m_vMax.z)
			m_vMax.z = point->Z;

		UpdateLines();
		
	} // AddInternalPoint

	//-----------------------------------------------------------------------------
	// Adds a point to bounding box
	//-----------------------------------------------------------------------------
	void AABB::AddInternalPoint( Vector* point )
	{
		if (m_bFirst)
		{
			m_bFirst = false;
			m_vMin.x = point->x;
			m_vMin.y = point->y;
			m_vMin.z = point->z;
			m_vMax.x = point->x;
			m_vMax.y = point->y;
			m_vMax.z = point->z;
			UpdateLines();
			return;
		}
		if (point->x < m_vMin.x)
			m_vMin.x = point->x;
		if (point->y < m_vMin.y)
			m_vMin.y = point->y;
		if (point->z < m_vMin.z)
			m_vMin.z = point->z;

		if (point->x > m_vMax.x)
			m_vMax.x = point->x;
		if (point->y > m_vMax.y)
			m_vMax.y = point->y;
		if (point->z > m_vMax.z)
			m_vMax.z = point->z;

		UpdateLines();

	} // AddInternalPoint

	//-----------------------------------------------------------------------------
	// Adds a point to bounding box
	//-----------------------------------------------------------------------------
	void AABB::AddInternalPoint(const Vector &point)
	{
		if (m_bFirst)
		{
			m_bFirst = false;
			m_vMin.x = point.x;
			m_vMin.y = point.y;
			m_vMin.z = point.z;
			m_vMax.x = point.x;
			m_vMax.y = point.y;
			m_vMax.z = point.z;
			UpdateLines();
			return;
		}
		if (point.x < m_vMin.x)
			m_vMin.x = point.x;
		if (point.y < m_vMin.y)
			m_vMin.y = point.y;
		if (point.z < m_vMin.z)
			m_vMin.z = point.z;

		if (point.x > m_vMax.x)
			m_vMax.x = point.x;
		if (point.y > m_vMax.y)
			m_vMax.y = point.y;
		if (point.z > m_vMax.z)
			m_vMax.z = point.z;

		UpdateLines();

	} // AddInternalPoint

	//-----------------------------------------------------------------------------
	// Adds an internal AABB
	//-----------------------------------------------------------------------------
	void AABB::AddAABB( const AABB* bb )
	{
		if (m_bFirst)
		{
			m_bFirst = false;
			m_vMin = bb->m_vMin;
			m_vMax = bb->m_vMax;
			UpdateLines();
			return;
		}

		if (bb->m_vMin.x < m_vMin.x)
			m_vMin.x = bb->m_vMin.x;
		if (bb->m_vMin.y < m_vMin.y)
			m_vMin.y = bb->m_vMin.y;
		if (bb->m_vMin.z < m_vMin.z)
			m_vMin.z = bb->m_vMin.z;

		if (bb->m_vMax.x > m_vMax.x)
			m_vMax.x = bb->m_vMax.x;
		if (bb->m_vMax.y > m_vMax.y)
			m_vMax.y = bb->m_vMax.y;
		if (bb->m_vMax.z > m_vMax.z)
			m_vMax.z = bb->m_vMax.z;
		
		UpdateLines();

	} // AddAABB

	//-----------------------------------------------------------------------------
	// Only checks for Y
	//-----------------------------------------------------------------------------
	void AABB::AddY( float y )
	{
		if (m_bFirst)
		{
			m_bFirst = false;
			m_vMin.y = y;
			m_vMax.y = y;
			UpdateLines();
			return;
		}
		if (y < m_vMin.y)
			m_vMin.y = y;
		if (y > m_vMax.y)
			m_vMax.y = y;

		UpdateLines();

	} // AddY

	//-----------------------------------------------------------------------------
	// Returns true if the ray and box collide.
	//-----------------------------------------------------------------------------
	bool AABB::IntersectRay( Ray* pRay )
	{
		float	tmin = -1e+20f,
				tmax = 1e+20f,
				tmin_,
				tmax_;

		for (int i = 0; i < 3; i++)
		{
			if (pRay->Direction.v[i] > 0.0f)
			{
				tmin_ = (m_vMin.v[i] - pRay->Position.v[i]) / pRay->Direction.v[i];
				tmax_ = (m_vMax.v[i] - pRay->Position.v[i]) / pRay->Direction.v[i];
			}
			else if(pRay->Direction.v[i] < 0.0f)
			{
				tmin_ = (m_vMax.v[i] - pRay->Position.v[i]) / pRay->Direction.v[i];
				tmax_ = (m_vMin.v[i] - pRay->Position.v[i]) / pRay->Direction.v[i];
			}
			else
				continue;
			if (tmin_ > tmin)
				tmin = tmin_;
			if (tmax_ < tmax)
				tmax = tmax_;
		}

		return (tmin <= tmax);

	} // IntersectRay

	//----------------------------------------
	// Returns true if point is inside the box
	//----------------------------------------
	bool AABB::Inside(const math::Vector& point) const
	{
		if (point.x < m_vMin.x || point.y < m_vMin.y || point.z < m_vMin.z
			|| point.x > m_vMax.x || point.y > m_vMax.y || point.z > m_vMax.z)
		{
			return false;
		}

		return true;
	}

	//-------------------------------------------
	// Returns true if two boxes are intersecting
	//-------------------------------------------
	bool AABB::Intersect(const AABB& box) const
	{
		if (box.m_vMax.x < m_vMin.x || box.m_vMax.y < m_vMin.y || box.m_vMax.z < m_vMin.z
			|| box.m_vMin.x > m_vMax.x || box.m_vMin.y > m_vMax.y || box.m_vMin.z > m_vMax.z)
		{
			return false;
		}

		return true;
	}

	//-----------------------------------------------------------------------------
	// Checks for swap min and max 
	//-----------------------------------------------------------------------------
	void AABB::Repair()
	{
		float t;
		for (int i = 0; i < 3; i++)
		{
			if (m_vMin.v[i] > m_vMax.v[i])
			{
				t			= m_vMin.v[i];
				m_vMin.v[i] = m_vMax.v[i];
				m_vMax.v[i] = t;
			}
		}
		UpdateLines();
	} // Repair

	void AABB::DebugRender()
	{
		// TODO:
// 		gfx::Renderer::GetSingletonPtr()->Disable(gfx::ERF_Lighting);
// 		gfx::Renderer::GetSingletonPtr()->SetTexture(NULL, 0);
// 		gfx::Renderer::GetSingletonPtr()->DrawLineList(v, 8, i, 24);
// 		gfx::Renderer::GetSingletonPtr()->Enable(gfx::ERF_Lighting);
	}

	void AABB::UpdateLines()
	{
		v[0].pos.X = m_vMin.x;
		v[0].pos.Y = m_vMin.y;
		v[0].pos.Z = m_vMin.z;
		v[1].pos.X = m_vMax.x;
		v[1].pos.Y = m_vMin.y;
		v[1].pos.Z = m_vMin.z;
		v[2].pos.X = m_vMin.x;
		v[2].pos.Y = m_vMax.y;
		v[2].pos.Z = m_vMin.z;
		v[3].pos.X = m_vMin.x;
		v[3].pos.Y = m_vMin.y;
		v[3].pos.Z = m_vMax.z;
		v[4].pos.X = m_vMax.x;
		v[4].pos.Y = m_vMax.y;
		v[4].pos.Z = m_vMax.z;
		v[5].pos.X = m_vMin.x;
		v[5].pos.Y = m_vMax.y;
		v[5].pos.Z = m_vMax.z;
		v[6].pos.X = m_vMax.x;
		v[6].pos.Y = m_vMin.y;
		v[6].pos.Z = m_vMax.z;
		v[7].pos.X = m_vMax.x;
		v[7].pos.Y = m_vMax.y;
		v[7].pos.Z = m_vMin.z;
	}

	void AABB::CreateIndices()
	{
		i[0] = 0;
		i[1] = 1;
		i[2] = 0;
		i[3] = 2;
		i[4] = 0;
		i[5] = 3;
		i[6] = 4;
		i[7] = 5;
		i[8] = 5;
		i[9] = 3;
		i[10] = 5;
		i[11] = 2;
		i[12] = 6;
		i[13] = 4;
		i[14] = 6;
		i[15] = 3;
		i[16] = 6;
		i[17] = 1;
		i[18] = 4;
		i[19] = 7;
		i[20] = 7;
		i[21] = 1;
		i[22] = 7;
		i[23] = 2;

		v[0].c.c = 0xffffffff; 
		v[1].c.c = 0xffffffff; 
		v[2].c.c = 0xffffffff; 
		v[3].c.c = 0xffffffff; 
		v[4].c.c = 0xffffffff; 
		v[5].c.c = 0xffffffff; 
		v[6].c.c = 0xffffffff; 
		v[7].c.c = 0xffffffff; 
	}


	//-----------------------------------------------------------------------------
	// resets the bounding box
	//-----------------------------------------------------------------------------
	void AABB::Reset()
	{
		m_vMin.set(0,0,0);
		m_vMax.set(0,0,0);
		m_bFirst = true;

	} // Reset

	void AABB::ResetY()
	{
		m_vMin.y = 0;
		m_vMax.y = 0;
		m_bFirst = true;

	}


	kge::math::Vector AABB::GetCenter()
	{
		return Vector
			(
				(m_vMin.x + m_vMax.x) / 2,
				(m_vMin.y + m_vMax.y) / 2,
				(m_vMin.z + m_vMax.z) / 2
			);

	}

} // math

} // kge
