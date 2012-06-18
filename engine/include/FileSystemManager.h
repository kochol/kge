#ifndef KGE_FILESYSTEMMANAGER_H
#define KGE_FILESYSTEMMANAGER_H

#include "String.h"
#include "array.h"

namespace kge
{
	namespace io
	{
		class FileSystem;

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
			void AddSearchPath(const core::stringc& Path);

			//! Returns a list of search paths.
			const core::DynamicArray<core::stringc>& GetSearchPath();

			//! Register a FileSystem with FileSystemManager
			void RegisterFileSystem(FileSystem* pFS);

		protected:

			core::DynamicArray<core::stringc>		m_aSearchPath;		//!< The search paths array
			core::DynamicArray<FileSystem*>			m_aFileSystems;		//!< The registered FileSystem plugins array

		}; // FileSystemManager

	} // io

} // kge

#endif // KGE_FILESYSTEMMANAGER_H
