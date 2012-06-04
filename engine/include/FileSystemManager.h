#ifndef KGE_FILESYSTEMMANAGER_H
#define KGE_FILESYSTEMMANAGER_H

namespace kge
{
	namespace io
	{
		class FileSystem;

		//! FileSystemManager is the main manager for FileSystem 
		class KGE_API FileSystemManager
		{
		protected:

			//! protected Constructor
			FileSystemManager();

		public:

			//! Destructor
			~FileSystemManager();

			//! Returns the FileSystemManager pointer
			static FileSystemManager* getSingletonPtr( );

		protected:
		private:

		}; // FileSystemManager

	} // io

} // kge

#endif // KGE_FILESYSTEMMANAGER_H
