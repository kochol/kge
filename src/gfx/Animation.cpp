#include "../../include/kge/gfx/Animation.h"


kge::gfx::Animation::~Animation()
{ 
	KGE_DELETE_CLASS_ARRAY(m_pAnis, AnimationData, m_iAnimationCount);
}

int kge::gfx::GetKey(float fTime, const kge::gfx::KeyFrame* p, int num)
{
	int begin = 0;
	int end = num - 1;

	while (begin <= end)
	{
		int m = (begin + end) / 2;

		if (p[m].m_fTime < fTime)
		{
			begin = m + 1;
		}
		else
		{
			if (m > 0)
			{
				if (p[m-1].m_fTime < fTime)
				{
					return m;
				}
				else
				{
					end = m - 1;
				}
			}
			else
			{
				return 0;
			}
		}
	}

	//return -1; // not found
	return 0; 
}

