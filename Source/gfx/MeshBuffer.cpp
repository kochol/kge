// File name: MeshBuffer.h
// Des: This class is for buffering the meshes.
// Date: 3/10/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/gfx/MeshBuffer.h"
#include "../../Headers/gfx/GeometryBatch.h"



#include "../../Libs/physfs/physfs.h"

#include <string>
#include <fstream>
#include <vector>
#include <iterator>

namespace kge
{
namespace gfx
{
	//--------------------------------------------------------------------
	// Constructor.
	//--------------------------------------------------------------------
	MeshBuffer::MeshBuffer(): Resource(0, 0, 0, 0),
		m_pMeshes(NULL), m_pBones(NULL), m_pAnimation(NULL),
		m_iMeshCount(0), m_iBoneCount(0), m_pVertexShader(NULL),
		m_pPixelShader(NULL), m_bShaderSet(false), m_pBatch(NULL),
		m_bInstanced(false)
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	MeshBuffer::MeshBuffer( const u32 Handle, const char* FileName, const char* Name, 
		void* ExtraParam ): Resource(Handle, FileName, Name, ExtraParam),
		m_pMeshes(NULL), m_pBones(NULL), m_pAnimation(NULL),
		m_iMeshCount(0), m_iBoneCount(0), m_pVertexShader(NULL),
		m_pPixelShader(NULL), m_bShaderSet(false), m_pBatch(NULL),
		m_bInstanced(false)
	{

	} // Constructor

	//--------------------------------------------------------------------
	// Destructor.
	//--------------------------------------------------------------------
	MeshBuffer::~MeshBuffer()
	{
		KGE_DELETE_CLASS_ARRAY(m_pMeshes, Mesh, m_iMeshCount);
		KGE_DELETE_CLASS_ARRAY(m_pBones, Bone, m_iBoneCount);
		KGE_DELETE(m_pAnimation, Animation);
		KGE_DELETE(m_pBatch, GeometryBatch);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Override AddRef to find out if we need instancing
	//------------------------------------------------------------------------------------
	void MeshBuffer::AddRef()
	{
		KgeUnknown::AddRef();

		if (!m_bInstanced)
		{
			if (GetRefCount() > 10)
			{
				// Create instancing for animated mesh
				if (m_pBones)
				{
					//m_pBatch     = new GeometryBatch(this);
					//m_bInstanced = true;
					//Device::GetSingletonPtr()->GetSceneManager()->AddGeometryBatch(m_pBatch);
					//io::Logger::Log("Start instancing mesh: ", m_sName.ToCharPointer());
				}
				// Create instancing for static meshes
				else
				{
					m_pBatch     = KGE_NEW(GeometryBatch)(this);
					m_bInstanced = true;
					Device::GetSingletonPtr()->GetSceneManager()->AddGeometryBatch(m_pBatch);
					io::Logger::Log("Start instancing mesh: ", m_sName.ToCharPointer());
				}
			}
		}
		

	} // AddRef

	//------------------------------------------------------------------------------------
	// Override DecRef to find out if we don't need instancing anymore
	//------------------------------------------------------------------------------------
	void MeshBuffer::DecRef()
	{

		KgeUnknown::DecRef();

	} // DecRef

	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	MeshBuffer* LoadStaticMesh(const char* filename, Renderer* renderer, sn::SceneManager* sceneManager)
	{
		////// load from archive

		//std::string name(filename);
		//std::string path("../../"); // 
		//std::string::size_type pos = name.find(path);
		//if (pos != std::string::npos)
		//{
		//	name = name.substr(pos + path.size());
		//}

		//if (!PHYSFS_exists(name.c_str())) return NULL;
		//PHYSFS_File* file = PHYSFS_openRead(name.c_str());

		////if (!PHYSFS_exists(filename)) return NULL;
		////PHYSFS_File* file = PHYSFS_openRead(filename);

		//const unsigned int length = PHYSFS_fileLength(file);

		//if (!length) return NULL;

		//std::vector<unsigned char> buffer(length);

		//const unsigned int readLen = PHYSFS_read(file, &buffer[0], sizeof(unsigned char), length);

		//PHYSFS_close(file);

		//if (readLen != length)
		//{
		//	return NULL;
		//}

		//////

		////// load from file

		std::ifstream ifile(filename, std::ios::binary);

		if (!ifile) return NULL;

		std::vector<unsigned char> buffer;

		std::copy(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>(),
			std::back_insert_iterator<std::vector<unsigned char> >(buffer));

		if (buffer.empty()) return NULL;

		//////

		const unsigned char* ptr = &buffer[0];

		// Meshes
		u32 numMeshes = 0;
		//ifile.read(reinterpret_cast<char*>(&numMeshes), sizeof(u32));
		memcpy(&numMeshes, ptr, sizeof(u32));
		ptr += sizeof(u32);

		std::vector<char> strBuffer;

		sn::Mesh* pMeshes = KGE_NEW_ARRAY(sn::Mesh, numMeshes);

		for (u32 i = 0; i < numMeshes; ++i)
		{
			pMeshes[i].m_pRenderer = renderer;

			// Mesh name
			u32 nameLen = 0;
			//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
			memcpy(&nameLen, ptr, sizeof(u32));
			ptr += sizeof(u32);

			if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

			//ifile.read(&strBuffer[0], nameLen);
			memcpy(&strBuffer[0], ptr, nameLen);
			ptr += nameLen;

			strBuffer[nameLen] = 0;
			pMeshes[i].m_sName = &strBuffer[0];

			// Material
			//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_bHasMat), sizeof(bool));
			memcpy(&pMeshes[i].m_bHasMat, ptr, sizeof(bool));
			ptr += sizeof(bool);

			if (pMeshes[i].m_bHasMat)
			{
				nameLen = 0;
				//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
				memcpy(&nameLen, ptr, sizeof(u32));
				ptr += sizeof(u32);

				if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

				//ifile.read(&strBuffer[0], nameLen);
				memcpy(&strBuffer[0], ptr, nameLen);
				ptr += nameLen;

				strBuffer[nameLen] = 0;
				pMeshes[i].m_Mat.Name = &strBuffer[0];

				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_Mat.Ambient.c[0]), sizeof(float) * 4);
				memcpy(&pMeshes[i].m_Mat.Ambient.c[0], ptr, sizeof(float) * 4);
				ptr += sizeof(float) * 4;

				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_Mat.Diffuse.c[0]), sizeof(float) * 4);
				memcpy(&pMeshes[i].m_Mat.Diffuse.c[0], ptr, sizeof(float) * 4);
				ptr += sizeof(float) * 4;

				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_Mat.Alpha), sizeof(bool));
				memcpy(&pMeshes[i].m_Mat.Alpha, ptr, sizeof(bool));
				ptr += sizeof(bool);

				nameLen = 0;
				//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
				memcpy(&nameLen, ptr, sizeof(u32));
				ptr += sizeof(u32);

				if (nameLen)
				{
					if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

					//ifile.read(&strBuffer[0], nameLen);
					memcpy(&strBuffer[0], ptr, nameLen);
					ptr += nameLen;

					strBuffer[nameLen] = 0;
					pMeshes[i].m_Mat.ppTexture[0] = sceneManager->AddTexture(&strBuffer[0]);
				}
			}

			// Sphere
			math::Sphere* sphere = pMeshes[i].GetBoundingBox();
			//ifile.read(reinterpret_cast<char*>(&sphere->m_vCenter.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vCenter.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_vMax.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vMax.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_vMin.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vMin.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_fRadius), sizeof(float));
			memcpy(&sphere->m_fRadius, ptr, sizeof(float));
			ptr += sizeof(float);

			// AABB
			math::AABB* box = pMeshes[i].GetAxisAlignedBoundingBox();
			//ifile.read(reinterpret_cast<char*>(&box->m_vMax.v[0]), sizeof(float) * 4);
			memcpy(&box->m_vMax.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&box->m_vMin.v[0]), sizeof(float) * 4);
			memcpy(&box->m_vMin.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			box->UpdateLines();
			box->CreateIndices();

			// Vertices
			//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_NumVerts), sizeof(u32));
			memcpy(&pMeshes[i].m_NumVerts, ptr, sizeof(u32));
			ptr += sizeof(u32);

			pMeshes[i].m_pVerts = KGE_NEW_ARRAY(Vertex3TN, pMeshes[i].m_NumVerts);

			for (u32 j = 0; j < pMeshes[i].m_NumVerts; ++j)
			{
				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_pVerts[j].pos.X), sizeof(float) * 3);
				memcpy(&pMeshes[i].m_pVerts[j].pos.X, ptr, sizeof(float) * 3);
				ptr += sizeof(float) * 3;

				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_pVerts[j].Nor.X), sizeof(float) * 3);
				memcpy(&pMeshes[i].m_pVerts[j].Nor.X, ptr, sizeof(float) * 3);
				ptr += sizeof(float) * 3;

				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_pVerts[j].tex.X), sizeof(float) * 2);
				memcpy(&pMeshes[i].m_pVerts[j].tex.X, ptr, sizeof(float) * 2);
				ptr += sizeof(float) * 2;
			}

			// Indices
			//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_NumIndices), sizeof(u32));
			memcpy(&pMeshes[i].m_NumIndices, ptr, sizeof(u32));
			ptr += sizeof(u32);

			pMeshes[i].m_pIndices = KGE_NEW_ARRAY(u16, pMeshes[i].m_NumIndices);

			if (pMeshes[i].m_NumIndices)
			{
				//ifile.read(reinterpret_cast<char*>(&pMeshes[i].m_pIndices[0]),
				//	sizeof(u16) * pMeshes[i].m_NumIndices);
				memcpy(&pMeshes[i].m_pIndices[0], ptr, sizeof(u16) * pMeshes[i].m_NumIndices);
				ptr += sizeof(u16) * pMeshes[i].m_NumIndices;
			}

			pMeshes[i].PrepareBuffers();
		}

		MeshBuffer* mb = KGE_NEW(MeshBuffer);
		mb->m_pMeshes = pMeshes;
		mb->m_iMeshCount = numMeshes;
		mb->m_sName = filename;
		return mb;
	}

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void SaveStaticMesh(MeshBuffer* meshBuffer)
	{
		std::string name(meshBuffer->m_sName.ToCharPointer());
		std::string::size_type pos = name.rfind(".ms3d");

		if (pos != std::string::npos && (pos + 5 == name.length()))
		{
			name.erase(pos);
		}

		name.append(".smesh");

		std::ofstream ofile(name.c_str(), std::ios::binary);

		if (!ofile) return;

		// Meshes
		ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_iMeshCount), sizeof(u32));

		for (u32 i = 0; i < meshBuffer->m_iMeshCount; ++i)
		{
			// Mesh name
			u32 nameLen = meshBuffer->m_pMeshes[i].m_sName.GetLenght();
			ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
			ofile.write(meshBuffer->m_pMeshes[i].m_sName.ToCharPointer(), nameLen);

			// Material
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_bHasMat), sizeof(bool));

			if (meshBuffer->m_pMeshes[i].m_bHasMat)
			{
				nameLen = meshBuffer->m_pMeshes[i].m_Mat.Name.GetLenght();
				ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
				ofile.write(meshBuffer->m_pMeshes[i].m_Mat.Name.ToCharPointer(), nameLen);

				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Ambient.c[0]), sizeof(float) * 4);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Diffuse.c[0]), sizeof(float) * 4);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Alpha), sizeof(bool));

				if (meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0] &&
					meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName())
				{
					nameLen = strlen(meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName());
					ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
					ofile.write(meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName(), nameLen);
				}
				else
				{
					nameLen = 0;
					ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
				}
			}

			// Sphere
			math::Sphere* sphere = meshBuffer->m_pMeshes[i].GetBoundingBox();
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vCenter.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vMax.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vMin.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_fRadius), sizeof(float));

			// AABB
			math::AABB* box = meshBuffer->m_pMeshes[i].GetAxisAlignedBoundingBox();
			ofile.write(reinterpret_cast<const char*>(&box->m_vMax.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&box->m_vMin.v[0]), sizeof(float) * 4);

			// Vertices
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_NumVerts), sizeof(u32));

			for (u32 j = 0; j < meshBuffer->m_pMeshes[i].m_NumVerts; ++j)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pVerts[j].pos.X), sizeof(float) * 3);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pVerts[j].Nor.X), sizeof(float) * 3);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pVerts[j].tex.X), sizeof(float) * 2);
			}

			// Indices
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_NumIndices), sizeof(u32));

			if (meshBuffer->m_pMeshes[i].m_NumIndices)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pIndices[0]),
					sizeof(u16) * meshBuffer->m_pMeshes[i].m_NumIndices);
			}
		}
	}

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	MeshBuffer* LoadAnimatedMesh(const char* filename, Renderer* renderer, sn::SceneManager* sceneManager)
	{
		////// load from archive

		//std::string name(filename);
		//std::string path("../../"); // 
		//std::string::size_type pos = name.find(path);
		//if (pos != std::string::npos)
		//{
		//	name = name.substr(pos + path.size());
		//}

		//if (!PHYSFS_exists(name.c_str())) return NULL;
		//PHYSFS_File* file = PHYSFS_openRead(name.c_str());

		////if (!PHYSFS_exists(filename)) return NULL;
		////PHYSFS_File* file = PHYSFS_openRead(filename);

		//const unsigned int length = PHYSFS_fileLength(file);

		//if (!length) return NULL;

		//std::vector<unsigned char> buffer(length);

		//const unsigned int readLen = PHYSFS_read(file, &buffer[0], sizeof(unsigned char), length);

		//PHYSFS_close(file);

		//if (readLen != length)
		//{
		//	return NULL;
		//}

		//////

		////// load from file

		std::ifstream ifile(filename, std::ios::binary);

		if (!ifile) return NULL;

		std::vector<unsigned char> buffer;

		std::copy(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>(),
			std::back_insert_iterator<std::vector<unsigned char> >(buffer));

		if (buffer.empty()) return NULL;

		//////

		const unsigned char* ptr = &buffer[0];

		// Bones
		u32 numBones = 0;
		//ifile.read(reinterpret_cast<char*>(&numBones), sizeof(u32));
		memcpy(&numBones, ptr, sizeof(u32));
		ptr += sizeof(u32);

		std::vector<char> strBuffer;

		sn::Bone* bones	= KGE_NEW_CLASS_ARRAY(sn::Bone, numBones);

		for (u32 i = 0; i < numBones; ++i)
		{
			u32 nameLen = 0;
			//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
			memcpy(&nameLen, ptr, sizeof(u32));
			ptr += sizeof(u32);

			if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

			//ifile.read(&strBuffer[0], nameLen);
			memcpy(&strBuffer[0], ptr, nameLen);
			ptr += nameLen;

			strBuffer[nameLen] = 0;
			bones[i].m_sName = &strBuffer[0];

			//ifile.read(reinterpret_cast<char*>(&bones[i].m_vStartPosition.X), sizeof(float) * 3);
			memcpy(&bones[i].m_vStartPosition.X, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;

			//ifile.read(reinterpret_cast<char*>(&bones[i].m_vStartRotation.X), sizeof(float) * 3);
			memcpy(&bones[i].m_vStartRotation.X, ptr, sizeof(float) * 3);
			ptr += sizeof(float) * 3;
			
			//ifile.read(reinterpret_cast<char*>(&bones[i].m_iParent), sizeof(int));
			memcpy(&bones[i].m_iParent, ptr, sizeof(int));
			ptr += sizeof(int);

			//ifile.read(reinterpret_cast<char*>(&bones[i].m_matLocal.m_fMat[0]), sizeof(float) * 16);
			memcpy(&bones[i].m_matLocal.m_fMat[0], ptr, sizeof(float) * 16);
			ptr += sizeof(float) * 16;

			//ifile.read(reinterpret_cast<char*>(&bones[i].m_matAbs.m_fMat[0]), sizeof(float) * 16);
			memcpy(&bones[i].m_matAbs.m_fMat[0], ptr, sizeof(float) * 16);
			ptr += sizeof(float) * 16;

			//ifile.read(reinterpret_cast<char*>(&bones[i].m_matFinal.m_fMat[0]), sizeof(float) * 16);
			memcpy(&bones[i].m_matFinal.m_fMat[0], ptr, sizeof(float) * 16);
			ptr += sizeof(float) * 16;
		}

		AnimationData* animData = KGE_NEW_CLASS_ARRAY(AnimationData, numBones);
		Animation* anim = KGE_NEW(Animation);
		anim->m_pAnis = animData;

		// Animations
		//ifile.read(reinterpret_cast<char*>(&anim->m_fFPS), sizeof(float));
		memcpy(&anim->m_fFPS, ptr, sizeof(float));
		ptr += sizeof(float);

		//ifile.read(reinterpret_cast<char*>(&anim->m_fTotalTime), sizeof(float));
		memcpy(&anim->m_fTotalTime, ptr, sizeof(float));
		ptr += sizeof(float);

		for (u32 i = 0; i < numBones; ++i)
		{
			//ifile.read(reinterpret_cast<char*>(&animData[i].m_iNumRotKeys), sizeof(u16));
			memcpy(&animData[i].m_iNumRotKeys, ptr, sizeof(u16));
			ptr += sizeof(u16);

			animData[i].m_pRotKeys = KGE_NEW_ARRAY(KeyFrame, animData[i].m_iNumRotKeys);

			if (animData[i].m_iNumRotKeys)
			{
				//ifile.read(reinterpret_cast<char*>(&animData[i].m_pRotKeys[0]),
				//	sizeof(KeyFrame) * animData[i].m_iNumRotKeys);
				memcpy(&animData[i].m_pRotKeys[0], ptr, sizeof(KeyFrame) * animData[i].m_iNumRotKeys);
				ptr += sizeof(KeyFrame) * animData[i].m_iNumRotKeys;
			}

			//ifile.read(reinterpret_cast<char*>(&animData[i].m_iNumTransKeys), sizeof(u16));
			memcpy(&animData[i].m_iNumTransKeys, ptr, sizeof(u16));
			ptr += sizeof(u16);
			
			animData[i].m_pTransKeys = KGE_NEW_ARRAY(KeyFrame, animData[i].m_iNumTransKeys);

			if (animData[i].m_iNumTransKeys)
			{
				//ifile.read(reinterpret_cast<char*>(&animData[i].m_pTransKeys[0]),
				//	sizeof(KeyFrame) * animData[i].m_iNumTransKeys);
				memcpy(&animData[i].m_pTransKeys[0], ptr, sizeof(KeyFrame) * animData[i].m_iNumTransKeys);
				ptr += sizeof(KeyFrame) * animData[i].m_iNumTransKeys;
			}
		}

		u32 numMeshes = 0;
		//ifile.read(reinterpret_cast<char*>(&numMeshes), sizeof(u32));
		memcpy(&numMeshes, ptr, sizeof(u32));
		ptr += sizeof(u32);

		sn::Mesh* meshes = KGE_NEW_CLASS_ARRAY(sn::Mesh, numMeshes);

		for (u32 i = 0; i < numMeshes; ++i)
		{
			meshes[i].m_pRenderer = renderer;

			// Mesh name
			u32 nameLen = 0;
			//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
			memcpy(&nameLen, ptr, sizeof(u32));
			ptr += sizeof(u32);

			if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

			//ifile.read(&strBuffer[0], nameLen);
			memcpy(&strBuffer[0], ptr, nameLen);
			ptr += nameLen;

			strBuffer[nameLen] = 0;
			meshes[i].m_sName = &strBuffer[0];

			// Material
			//ifile.read(reinterpret_cast<char*>(&meshes[i].m_bHasMat), sizeof(bool));
			memcpy(&meshes[i].m_bHasMat, ptr, sizeof(bool));
			ptr += sizeof(bool);

			if (meshes[i].m_bHasMat)
			{
				nameLen = 0;
				//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
				memcpy(&nameLen, ptr, sizeof(u32));
				ptr += sizeof(u32);

				if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

				//ifile.read(&strBuffer[0], nameLen);
				memcpy(&strBuffer[0], ptr, nameLen);
				ptr += nameLen;

				strBuffer[nameLen] = 0;
				meshes[i].m_Mat.Name = &strBuffer[0];

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_Mat.Ambient.c[0]), sizeof(float) * 4);
				memcpy(&meshes[i].m_Mat.Ambient.c[0], ptr, sizeof(float) * 4);
				ptr += sizeof(float) * 4;

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_Mat.Diffuse.c[0]), sizeof(float) * 4);
				memcpy(&meshes[i].m_Mat.Diffuse.c[0], ptr, sizeof(float) * 4);
				ptr += sizeof(float) * 4;

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_Mat.Alpha), sizeof(bool));
				memcpy(&meshes[i].m_Mat.Alpha, ptr, sizeof(bool));
				ptr += sizeof(bool);

				nameLen = 0;
				//ifile.read(reinterpret_cast<char*>(&nameLen), sizeof(u32));
				memcpy(&nameLen, ptr, sizeof(u32));
				ptr += sizeof(u32);

				if (nameLen)
				{
					if (nameLen + 1 > strBuffer.size()) strBuffer.resize(nameLen + 1);

					//ifile.read(&strBuffer[0], nameLen);
					memcpy(&strBuffer[0], ptr, nameLen);
					ptr += nameLen;

					strBuffer[nameLen] = 0;
					meshes[i].m_Mat.ppTexture[0] = sceneManager->AddTexture(&strBuffer[0]);
				}
			}

			// Sphere
			math::Sphere* sphere = meshes[i].GetBoundingBox();
			//ifile.read(reinterpret_cast<char*>(&sphere->m_vCenter.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vCenter.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_vMax.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vMax.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_vMin.v[0]), sizeof(float) * 4);
			memcpy(&sphere->m_vMin.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&sphere->m_fRadius), sizeof(float));
			memcpy(&sphere->m_fRadius, ptr, sizeof(float));
			ptr += sizeof(float);

			// AABB
			math::AABB* box = meshes[i].GetAxisAlignedBoundingBox();
			//ifile.read(reinterpret_cast<char*>(&box->m_vMax.v[0]), sizeof(float) * 4);
			memcpy(&box->m_vMax.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			//ifile.read(reinterpret_cast<char*>(&box->m_vMin.v[0]), sizeof(float) * 4);
			memcpy(&box->m_vMin.v[0], ptr, sizeof(float) * 4);
			ptr += sizeof(float) * 4;

			box->UpdateLines();
			box->CreateIndices();

			// Vertices
			//ifile.read(reinterpret_cast<char*>(&meshes[i].m_NumVerts), sizeof(u32));
			memcpy(&meshes[i].m_NumVerts, ptr, sizeof(u32));
			ptr += sizeof(u32);

			meshes[i].m_pPos = KGE_NEW_ARRAY(Vertex3, meshes[i].m_NumVerts);
			meshes[i].m_pNorTex = KGE_NEW_ARRAY(Vertex3T, meshes[i].m_NumVerts);
			meshes[i].m_Bone = KGE_NEW_ARRAY(s16, meshes[i].m_NumVerts);

			for (u32 j = 0; j < meshes[i].m_NumVerts; ++j)
			{
				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_pPos[j].X), sizeof(float) * 3);
				memcpy(&meshes[i].m_pPos[j].X, ptr, sizeof(float) * 3);
				ptr += sizeof(float) * 3;

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_pNorTex[j].pos.X), sizeof(float) * 3);
				memcpy(&meshes[i].m_pNorTex[j].pos.X, ptr, sizeof(float) * 3);
				ptr += sizeof(float) * 3;

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_pNorTex[j].tex.X), sizeof(float) * 2);
				memcpy(&meshes[i].m_pNorTex[j].tex.X, ptr, sizeof(float) * 2);
				ptr += sizeof(float) * 2;

				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_Bone[j]), sizeof(s16));
				memcpy(&meshes[i].m_Bone[j], ptr, sizeof(s16));
				ptr += sizeof(s16);
			}

			// Indices
			//ifile.read(reinterpret_cast<char*>(&meshes[i].m_NumIndices), sizeof(u32));
			memcpy(&meshes[i].m_NumIndices, ptr, sizeof(u32));
			ptr += sizeof(u32);

			meshes[i].m_pIndices = KGE_NEW_ARRAY(u16, meshes[i].m_NumIndices);

			if (meshes[i].m_NumIndices)
			{
				//ifile.read(reinterpret_cast<char*>(&meshes[i].m_pIndices[0]),
				//	sizeof(u16) * meshes[i].m_NumIndices);
				memcpy(&meshes[i].m_pIndices[0], ptr, sizeof(u16) * meshes[i].m_NumIndices);
				ptr += sizeof(u16) * meshes[i].m_NumIndices;
			}

			meshes[i].PrepareBuffers();
		}

		MeshBuffer* mb = KGE_NEW(MeshBuffer);
		mb->m_pBones = bones;
		mb->m_pAnimation = anim;
		mb->m_pMeshes = meshes;
		mb->m_iMeshCount = numMeshes;
		mb->m_iBoneCount = numBones;
		mb->m_sName = filename;
		return mb;
	}

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void SaveAnimatedMesh(MeshBuffer* meshBuffer)
	{
		std::string name(meshBuffer->m_sName.ToCharPointer());
		std::string::size_type pos = name.rfind(".ms3d");

		if (pos != std::string::npos && (pos + 5 == name.length()))
		{
			name.erase(pos);
		}

		name.append(".amesh");

		std::ofstream ofile(name.c_str(), std::ios::binary);

		if (!ofile) return;

		// Bones
		ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_iBoneCount), sizeof(u32));

		for (u32 i = 0; i < meshBuffer->m_iBoneCount; ++i)
		{
			u32 nameLen = meshBuffer->m_pBones[i].m_sName.GetLenght();
			ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
			ofile.write(meshBuffer->m_pBones[i].m_sName.ToCharPointer(), nameLen);

			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_vStartPosition.X), sizeof(float) * 3);
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_vStartRotation.X), sizeof(float) * 3);
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_iParent), sizeof(int));
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_matLocal.m_fMat[0]), sizeof(float) * 16);
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_matAbs.m_fMat[0]), sizeof(float) * 16);
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pBones[i].m_matFinal.m_fMat[0]), sizeof(float) * 16);
		}

		// Animations
		ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->m_fFPS), sizeof(float));
		ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->m_fTotalTime), sizeof(float));

		for (u32 i = 0; i < meshBuffer->m_iBoneCount; ++i)
		{
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumRotKeys), sizeof(u16));

			if (meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumRotKeys)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->GetAnimation(i)->m_pRotKeys[0]),
					sizeof(KeyFrame) * meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumRotKeys);
			}

			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumTransKeys), sizeof(u16));

			if (meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumTransKeys)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pAnimation->GetAnimation(i)->m_pTransKeys[0]),
					sizeof(KeyFrame) * meshBuffer->m_pAnimation->GetAnimation(i)->m_iNumTransKeys);
			}
		}

		// Meshes
		ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_iMeshCount), sizeof(u32));

		for (u32 i = 0; i < meshBuffer->m_iMeshCount; ++i)
		{
			// Mesh name
			u32 nameLen = meshBuffer->m_pMeshes[i].m_sName.GetLenght();
			ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
			ofile.write(meshBuffer->m_pMeshes[i].m_sName.ToCharPointer(), nameLen);

			// Material
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_bHasMat), sizeof(bool));

			if (meshBuffer->m_pMeshes[i].m_bHasMat)
			{
				nameLen = meshBuffer->m_pMeshes[i].m_Mat.Name.GetLenght();
				ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
				ofile.write(meshBuffer->m_pMeshes[i].m_Mat.Name.ToCharPointer(), nameLen);

				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Ambient.c[0]), sizeof(float) * 4);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Diffuse.c[0]), sizeof(float) * 4);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Mat.Alpha), sizeof(bool));

				if (meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0] &&
					meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName())
				{
					nameLen = strlen(meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName());
					ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
					ofile.write(meshBuffer->m_pMeshes[i].m_Mat.ppTexture[0]->GetFileName(), nameLen);
				}
				else
				{
					nameLen = 0;
					ofile.write(reinterpret_cast<const char*>(&nameLen), sizeof(u32));
				}
			}

			// Sphere
			math::Sphere* sphere = meshBuffer->m_pMeshes[i].GetBoundingBox();
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vCenter.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vMax.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_vMin.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&sphere->m_fRadius), sizeof(float));

			// AABB
			math::AABB* box = meshBuffer->m_pMeshes[i].GetAxisAlignedBoundingBox();
			ofile.write(reinterpret_cast<const char*>(&box->m_vMax.v[0]), sizeof(float) * 4);
			ofile.write(reinterpret_cast<const char*>(&box->m_vMin.v[0]), sizeof(float) * 4);

			// Vertices
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_NumVerts), sizeof(u32));

			for (u32 j = 0; j < meshBuffer->m_pMeshes[i].m_NumVerts; ++j)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pPos[j].X), sizeof(float) * 3);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pNorTex[j].pos.X), sizeof(float) * 3);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pNorTex[j].tex.X), sizeof(float) * 2);
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_Bone[j]), sizeof(u16));
			}

			// Indices
			ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_NumIndices), sizeof(u32));

			if (meshBuffer->m_pMeshes[i].m_NumIndices)
			{
				ofile.write(reinterpret_cast<const char*>(&meshBuffer->m_pMeshes[i].m_pIndices[0]),
					sizeof(u16) * meshBuffer->m_pMeshes[i].m_NumIndices);
			}
		}
	}

} // gfx

} // kge
