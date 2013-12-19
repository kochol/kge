#ifndef KGE_FILESYSTEM_H
#define KGE_FILESYSTEM_H

#include <string>

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
			virtual bool CanLoad(std::string FileName);

			//! Loads a file and return its Stream
			/*!
				\param FileName The name of the file to load.
				\return Returns the loaded stream.
			 */
			virtual Stream* Load(std::string FileName);

		}; // FileSysem

	} // io

} // kge

#endif // KGE_FILESYSTEM_H
