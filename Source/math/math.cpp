#include "../../Include/math/math.h"
#include "../../Include/math/Sphere.h"
#include "../../Include/math/Plane.h"
#include "../../Include/math/Vector.h"

namespace kge
{
	namespace math
	{

		//------------------------------------------------------------------------------------
		// this function tests if the projection of a bounding sphere along the light direction intersects the view frustum 
		//------------------------------------------------------------------------------------
		bool SweptSpherePlaneIntersect( float& t0, float& t1, 
			const Plane& plane, const Sphere& sphere, const Vector& sweepDir )
		{
			float b_dot_n = plane.DotCoord(sphere.GetCenter());
			float d_dot_n = plane.DotNormal(sweepDir);

			if (d_dot_n == 0.f)
			{
				if (b_dot_n <= sphere.GetRadius())
				{
					//  effectively infinity
					t0 = 0.f;
					t1 = 1e32f;
					return true;
				}
				else
					return false;
			}
			else
			{
				float tmp0 = ( sphere.GetRadius() - b_dot_n) / d_dot_n;
				float tmp1 = (-sphere.GetRadius() - b_dot_n) / d_dot_n;
				t0 = Min(tmp0, tmp1);
				t1 = Max(tmp0, tmp1);
				return true;
			}

		}
	} // math

} // kge
