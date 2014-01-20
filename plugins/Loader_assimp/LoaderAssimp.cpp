#include "LoaderAssimp.h"
#include "../../Include/core/KgeMemory.h"
#include "../../Include/io/Stream.h"
#include "../../Include/io/Logger.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace kge
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	LoaderAssimp::LoaderAssimp()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	LoaderAssimp::~LoaderAssimp()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Loads a resource from a FileSystem and return its pointer.
	//------------------------------------------------------------------------------------
	Resource* LoaderAssimp::LoadResource( io::Stream* pStream )
	{
		// Create an instance of the Importer class  
		Assimp::Importer importer;  

		// Read file
		u8* pBuffer = KGE_NEW_ARRAY(u8, pStream->GetSize());
		pStream->Read((void*)pBuffer, pStream->GetSize());

		const aiScene* scene = importer.ReadFileFromMemory
			(pBuffer,
			pStream->GetSize(),
			aiProcess_Triangulate      |        
			aiProcess_JoinIdenticalVertices  |        
			aiProcess_SortByPType);

		if (!scene)  
		{  
			io::Logger::Error(importer.GetErrorString());
			return NULL;  
		}  


		return NULL;

	} // LoadResource
	
} // kge
