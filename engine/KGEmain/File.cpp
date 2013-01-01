#include "../include/File.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#	include <io.h>
#else
#	include <unistd.h>
#endif

namespace kge
{
	namespace io
	{

		//------------------------------------------------------------------------------------
		// Check if the file exist
		//------------------------------------------------------------------------------------
		bool File::Exist( const core::stringc& cstrFileName )
		{
			if (access(cstrFileName.c_str(), 0) != -1)
				return true;

			return false;

		} // Exist

	} // ip

} // kge
