#include <stdio.h>
#include "../../engine/include/Logger.h"

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

int main()
{
	kge::io::Logger log;
	for (int i = 0; i < 35; i++)
	{
		switch (i % 5)
		{
		case 0:
			kge::io::Logger::Info("Information %d", i);
			break;

		case 1:
			kge::io::Logger::Warning("Warning %d", i);
			break;

		case 2:
			kge::io::Logger::Error("Error %d", i);
			break;

		case 3:
			kge::io::Logger::Debug("Debug %d", i);
			break;

        case 4:
            kge::io::Logger::SuccessKGE("Success %d", i);

		}
	}
	getchar();

} // main
