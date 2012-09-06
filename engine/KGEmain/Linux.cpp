#include "../include/kgedef.h"
#if KGE_PLATFORM == KGE_PLATFORM_LINUX
#include "../include/os.h"
#include <unistd.h>

namespace kge
{
	namespace os
	{
		// Sleep the thread
		void KGESleep(int miliSeconds)
		{
			usleep(miliSeconds * 1000);

		} // Sleep

	} // os

} // kge

#endif // KGE_PLATFORM_WINDOWS