#include "../include/FileSystemManager.h"
#include "../include/FileSystem.h"
#include "../include/Logger.h"
#include "../include/File.h"

namespace kge
{
	namespace io
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		FileSystem::FileSystem()
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		FileSystem::~FileSystem()
		{

		} // Destructor

		//------------------------------------------------------------------------------------
		// Check if this FileSystem can load this file.
		//------------------------------------------------------------------------------------
		bool FileSystem::CanLoad( core::stringc FileName )
		{
			// Get search paths
			const core::DynamicArray<core::stringc>& aSearchPaths = 
				FileSystemManager::getSingletonPtr()->GetSearchPath();

			// Looking for files
			core::stringc str;
			for (u32 i = 0; i < aSearchPaths.size(); i++)
			{
				str = aSearchPaths[i];
				size_t strsize = str.size();

				// Add / to the end of the path if it hasn't any
				if (str[strsize - 1] != '/' && str[strsize - 1] != '\\')
					str += '/';

				str += FileName;
				io::Logger::Debug("%s", str.c_str());

				// Check if the file exist
				if (io::File::Exist(str))
				{
					io::Logger::Debug("Can load %s file.", str.c_str());
					return true;
				}
			}

			return false;

		} // CanLoad

	} // io

} // kge
