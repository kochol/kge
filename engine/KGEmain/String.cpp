#include <string.h>
#include "../include/String.h"
#include "../include/KgeMemory.h"

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Copy the char* to char*
		//------------------------------------------------------------------------------------
		char* String::StringCopy( const char* Src )
		{
			if (!Src)
				return 0;

			size_t len = strlen(Src);
			char* ch = KGE_NEW_ARRAY(char, len+1);
			strcpy(ch, Src);
			ch[len] = 0;

			return ch;

		} // StringCopy

	} // core

} // kge
