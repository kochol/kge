#include "../include/kgedef.h"
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#include "../include/os.h"

namespace kge
{
	namespace os
	{
		// Sleep the thread
		void KGESleep(int miliSeconds)
		{
			Sleep(miliSeconds);

		} // Sleep

	} // os

} // kge

#endif // KGE_PLATFORM_WINDOWS