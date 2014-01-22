#include "LoaderAssimp.h"
#include "../../Include/core/KgeMemory.h"
#include "../../Include/io/Stream.h"
#include "../../Include/io/Logger.h"
#include "../../Include/sn/Mesh.h"
#include "../../Include/gfx/MeshBuffer.h"
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
		m_aFileExtension.push_back("uc");
		m_aFileExtension.push_back("3d");
		m_aFileExtension.push_back("xml");
		m_aFileExtension.push_back("3ds");
		m_aFileExtension.push_back("tga");
		m_aFileExtension.push_back("pic");

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
			aiProcess_FlipUVs |
			aiProcess_SortByPType);

		if (!scene)  
		{  
			io::Logger::Error(importer.GetErrorString());
			return NULL;  
		}

		// Process the scene

		// Create the materials

		// Create the meshes
		sn::Mesh *pMeshes = NULL;
		if (scene->HasMeshes())
			pMeshes = KGE_NEW_CLASS_ARRAY(sn::Mesh, scene->mNumMeshes);
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			// Create vertices
			gfx::Vertex3* pVerts = KGE_NEW_ARRAY(gfx::Vertex3, mesh->mNumVertices);
			for (int k = 0; k < mesh->mNumVertices; k++)
			{
				pVerts[k].X = mesh->mVertices[k].x;
				pVerts[k].Y = mesh->mVertices[k].y;
				pVerts[k].Z = mesh->mVertices[k].z;
			}

			// Create indices
			u16* indices = KGE_NEW_ARRAY(u16, mesh->mNumFaces * 3);
			for (int k = 0; k < mesh->mNumFaces; k++)
			{
				indices[3 * k    ] = (u16)mesh->mFaces[k].mIndices[0];
				indices[3 * k + 1] = (u16)mesh->mFaces[k].mIndices[1];
				indices[3 * k + 2] = (u16)mesh->mFaces[k].mIndices[2];
			}
			pMeshes[i].m_pIndices	= indices;
			pMeshes[i].m_NumIndices	= mesh->mNumFaces * 3;
			pMeshes[i].m_pPos		= pVerts;
			pMeshes[i].m_NumVerts	= mesh->mNumVertices;
			
			pMeshes[i].PrepareBuffers();
		}

		// Save the mesh to a mesh buffer.
		gfx::MeshBuffer* mb	= KGE_NEW(gfx::MeshBuffer)();
		mb->m_pMeshes		= pMeshes;
		mb->m_iMeshCount	= scene->mNumMeshes;
		mb->m_sName			= pStream->GetName().c_str();

		return mb;

	} // LoadResource
	
} // kge
