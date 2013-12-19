#include "../../Include/en/ComponentType.h"

namespace kge
{
	namespace en
	{
		int assign::AssignComponentID()
		{
			static int i = 0;
			return i++;
		}

	} // en

} // kge
