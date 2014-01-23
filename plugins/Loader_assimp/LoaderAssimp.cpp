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
		m_aFileExtension.push_back("dae");
		m_aFileExtension.push_back("blend");
		m_aFileExtension.push_back("bvh");
		m_aFileExtension.push_back("ase");
		m_aFileExtension.push_back("obj");
		m_aFileExtension.push_back("ply");
		m_aFileExtension.push_back("dxf");
		m_aFileExtension.push_back("ifc");
		m_aFileExtension.push_back("nff");
		m_aFileExtension.push_back("smd");
		m_aFileExtension.push_back("vta");
		m_aFileExtension.push_back("mdl");
		m_aFileExtension.push_back("md2");
		m_aFileExtension.push_back("md3");
		m_aFileExtension.push_back("pk3");
		m_aFileExtension.push_back("mdc");
		m_aFileExtension.push_back("md5mesh");
		m_aFileExtension.push_back("md5anim");
		m_aFileExtension.push_back("md5camera");
		m_aFileExtension.push_back("x");
		m_aFileExtension.push_back("q3o");
		m_aFileExtension.push_back("q3s");
		m_aFileExtension.push_back("raw");
		m_aFileExtension.push_back("ac");
		m_aFileExtension.push_back("stl");
		m_aFileExtension.push_back("irrmesh");
		m_aFileExtension.push_back("irr");
		m_aFileExtension.push_back("off");
		m_aFileExtension.push_back("ter");
		m_aFileExtension.push_back("mdl");
		m_aFileExtension.push_back("hmp");
		m_aFileExtension.push_back("material");
		m_aFileExtension.push_back("lwo");
		m_aFileExtension.push_back("lws");
		m_aFileExtension.push_back("lxo");
		m_aFileExtension.push_back("csm");
		m_aFileExtension.push_back("cob");
		m_aFileExtension.push_back("scn");
		m_aFileExtension.push_back("xgl");
		m_aFileExtension.push_back("zgl");

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
			gfx::Vertex3T* pNT	 = KGE_NEW_ARRAY(gfx::Vertex3T, mesh->mNumVertices); 
			for (int k = 0; k < mesh->mNumVertices; k++)
			{
				pVerts[k].X = mesh->mVertices[k].x;
				pVerts[k].Y = mesh->mVertices[k].y;
				pVerts[k].Z = mesh->mVertices[k].z;
				pNT[k].pos.X = mesh->mNormals[k].x;
				pNT[k].pos.Y = mesh->mNormals[k].y;
				pNT[k].pos.Z = mesh->mNormals[k].z;
				pNT[k].tex.X = mesh->mTextureCoords[0][k].x;
				pNT[k].tex.Y = mesh->mTextureCoords[0][k].y;
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
			pMeshes[i].m_pNorTex	= pNT;
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
