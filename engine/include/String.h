#ifndef KGE_STRING_H
#define KGE_STRING_H

#include <string>

namespace kge
{
	namespace core
	{
		typedef std::string stringc;
		typedef std::wstring stringw;

		//! KGE string class
		class String
		{
		public:

			//! Copy the char* to char*
			static char* StringCopy(const char* Src);
	
	#if KGE_PLATFORM != KGE_PLATFORM_ANDROID

			//! Converts A stringw type to stringc type
			static stringc Convert(const stringw& Src);
			
			//! Converts A stringc type to stringw type
			static stringw Convert(const stringc& Src);

	#endif 
	
		}; // String

	} // core

} // kge

#endif // KGE_STRING_H
