#include "../../Include/io/FileSystemManager.h"
#include "../../Include/io/FileSystem.h"
#include "../../Include/io/Logger.h"
#include "../../Include/io/File.h"
#include "../../Include/io/Stream.h"
#include <stdio.h>
#include <string.h>

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
		bool FileSystem::CanLoad( std::string FileName )
		{
			// Get search paths
			const std::vector<std::string>& aSearchPaths =
				FileSystemManager::getSingletonPtr()->GetSearchPath();

			// Looking for files
			std::string str;
			for (u32 i = 0; i < aSearchPaths.size(); i++)
			{
				str = aSearchPaths[i];
				u32 strsize = str.size();

				// Add / to the end of the path if it hasn't any
				if (str[strsize - 1] != '/' && str[strsize - 1] != '\\')
					str += '/';

				str += FileName;
				io::Logger::Debug("%s", str.c_str());

				// Check if the file exist
				if (io::File::Exist(str.c_str()))
				{
					io::Logger::Debug("Can load %s file.", str.c_str());
					return true;
				}
			}

			return false;

		} // CanLoad

		//------------------------------------------------------------------------------------
		// Loads a file and return its Stream
		//------------------------------------------------------------------------------------
		Stream* FileSystem::Load( std::string FileName )
		{
			// Open the file
			FILE*	pF;
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			fopen_s(&pF, FileName.c_str(), "rb");
#else
			pF = fopen(FileName.ToCharPointer(), "rb");
#endif
			if (!pF)
			{
				io::Logger::Error("Can't load %s file. File not found", FileName.c_str());
				return NULL;
			}

			// Get the file size
			fseek(pF, 0, SEEK_END);
			int iEnd = ftell(pF);
			fseek(pF, 0, SEEK_SET);
			int iStart = ftell(pF);
			int size = iEnd - iStart;

			// Read the file
			u8* Buffer = KGE_NEW_ARRAY(u8, size);
			int i = (int)fread(Buffer, 1, size, pF);
			if (i==-1)
				io::Logger::Error("Could not read file: ", FileName.c_str());

			// Create the stream
			Stream* pS = KGE_NEW(Stream)((void*)Buffer, size, FileName.c_str());

			return pS;

		} // Load

	} // io

} // kge
