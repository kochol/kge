#include "../../include/kge/io/FileSystemManager.h"
#include "../../include/kge/io/FileSystem.h"

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
			Release();

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
		void FileSystemManager::AddSearchPath( std::string& Path )
		{
			m_aSearchPath.push_back(Path);

		} // AddSearchPath

		//------------------------------------------------------------------------------------
		// Returns a list of search paths.
		//------------------------------------------------------------------------------------
		const std::vector<std::string>& FileSystemManager::GetSearchPath()
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

		//------------------------------------------------------------------------------------
		// Loads a stream from file systems plugins
		//------------------------------------------------------------------------------------
		Stream* FileSystemManager::Load( std::string& FileName )
		{
			Stream* p = NULL;
			for (int i = 0; i < m_aFileSystems.size(); i++)
			{
				p = m_aFileSystems[i]->Load(FileName);
				if (p)
					return p;
			}
			
			return NULL;

		} // Load

		//------------------------------------------------------------------------------------
		// Release the memory.
		//------------------------------------------------------------------------------------
		void FileSystemManager::Release()
		{
			m_aFileSystems.clear();
			m_aSearchPath.clear();

		} // Release

	} // io

} // kge
