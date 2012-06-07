#ifndef KGE_FILE_H
#define KGE_FILE_H

#include "KgeUnknown.h"
#include "String.h"

namespace kge
{
	namespace io
	{
		class File: public KgeUnknown
		{
		public:

			//! Check if the file exist
			static bool Exist(const core::stringc& cstrFileName);

		protected:

		}; // File

	} // io

} // kge

#endif // KGE_FILE_H
