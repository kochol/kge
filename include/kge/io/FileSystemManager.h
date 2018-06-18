#ifndef KGE_FILESYSTEMMANAGER_H
#define KGE_FILESYSTEMMANAGER_H

#include <string>
#include <vector>
#include "../kgedef.h"

namespace kge
{
	namespace io
	{
		class FileSystem;
		class Stream;

		//! FileSystemManager is the main manager for FileSystem 
		class KGE_API FileSystemManager
		{
			private:

			//! private Constructor
			FileSystemManager();

		public:

			//! Destructor
			~FileSystemManager();

			//! Returns the FileSystemManager pointer
			static FileSystemManager* getSingletonPtr( );

			//! Add a search path to the FileSystemManager to search these paths on resource loading.
			/*!
				\param Path The path that will be added to search paths.
			 */
			void AddSearchPath(std::string& Path);

			//! Returns a list of search paths.
			const std::vector<std::string>& GetSearchPath();

			//! Register a FileSystem with FileSystemManager
			void RegisterFileSystem(FileSystem* pFS);

			//! Loads a stream from file systems plugins
			Stream* Load(std::string& FileName);

			//! Release the memory.
			void Release();

		protected:

			std::vector<std::string>		m_aSearchPath;		//!< The search paths array
			std::vector<FileSystem*>		m_aFileSystems;		//!< The registered FileSystem plugins array

		}; // FileSystemManager

	} // io

} // kge

#endif // KGE_FILESYSTEMMANAGER_H
