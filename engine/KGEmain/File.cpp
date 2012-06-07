#include "../include/File.h"
#include <io.h>

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
