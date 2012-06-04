#include "../Include/FileSystemManager.h"

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

	} // io

} // kge
