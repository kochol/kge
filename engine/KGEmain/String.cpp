#include <stdlib.h>
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

#if KGE_PLATFORM != KGE_PLATFORM_ANDROID

		//------------------------------------------------------------------------------------
		// Converts A stringw type to stringc type
		//------------------------------------------------------------------------------------
		stringc String::Convert( const stringw& Src )
		{
			stringc str(Src.size(), '#');

			wcstombs(&str[0], Src.c_str(), Src.size());

			return str;

		}

		//------------------------------------------------------------------------------------
		// Converts A stringc type to stringw type
		//------------------------------------------------------------------------------------
		stringw String::Convert( const stringc& Src )
		{
			stringw str(Src.size(), L'#');

			mbstowcs(&str[0], Src.c_str(), Src.size());

			return str;

		}

#endif

	} // core

} // kge
