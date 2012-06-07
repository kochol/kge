#include "../include/FileSystemManager.h"
#include "../include/FileSystem.h"

namespace kge
{
	namespace io
	{
		
		//------------------------------------------------------------------------------------
		// protected Constructor
		//------------------------------------------------------------------------------------
		FileSystemManager::FileSystemManager()
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		FileSystemManager::~FileSystemManager()
		{

		} // Destructor

		//------------------------------------------------------------------------------------
		// Returns the FileSystemManager pointer
		//------------------------------------------------------------------------------------
		FileSystemManager* FileSystemManager::getSingletonPtr()
		{
			static FileSystemManager fsm;

			return &fsm;

		} // getSingletonPtr

		//------------------------------------------------------------------------------------
		// Add a search path to the FileSystemManager to search these paths on resource loading.
		//------------------------------------------------------------------------------------
		void FileSystemManager::AddSearchPath( const core::stringw& Path )
		{
			m_aSearchPath.push_back(Path);

		} // AddSearchPath

		//------------------------------------------------------------------------------------
		// Returns a list of search paths.
		//------------------------------------------------------------------------------------
		const core::DynamicArray<core::stringw>& FileSystemManager::GetSearchPath()
		{
			return m_aSearchPath;

		} // GetSearchPath

		//------------------------------------------------------------------------------------
		// Register a FileSystem with FileSystemManager
		//------------------------------------------------------------------------------------
		void FileSystemManager::RegisterFileSystem( FileSystem* pFS )
		{
			m_aFileSystems.push_back(pFS);

		} // RegisterFileSystem

	} // io

} // kge
