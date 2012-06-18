#ifndef KGE_FILESYSTEM_H
#define KGE_FILESYSTEM_H

#include "String.h"

namespace kge
{
	namespace io
	{
		class KGE_API FileSystem
		{
		public:

			//! Constructor
			FileSystem();

			//! Destructor
			virtual ~FileSystem();

			//! Check if this FileSystem can load this file.
			/*!
				\param FileName The name of the file to check.
				\return Returns true if possible.
			 */
			virtual bool CanLoad(core::stringc FileName);

		}; // FileSysem

	} // io

} // kge

#endif // KGE_FILESYSTEM_H
