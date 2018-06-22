// File name: MS3DLoader.cpp
// Des: This class loads the MS3D meshes and animations.
// Date: 25/1/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Headers/gfx/MS3DLoader.h"
#include "../../include/kge/sn/Mesh.h"
#include "../../include/kge/io/File.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/sn/Bone.h"
#include "../../include/kge/gfx/Animation.h"
#include "../../include/kge/gfx/MeshManager.h"
#include "../../include/kge/core/ArrayPriority.h"



//#include "../../Libs/physfs/physfs.h"

namespace kge
{
namespace gfx
{

	//--------------
	// Constructor.
	//--------------
	MS3DLoader::MS3DLoader(Renderer* renderer, sn::SceneManager* smgr):
		m_pVertices(NULL), m_pTriangles(NULL), m_pMeshes(NULL),
		m_pMaterials(NULL), m_pJoints(NULL), m_fFPS(0.0f), m_fTotalTime(0.0f),
		m_iFramesCount(0), m_pRenderer(renderer), m_pSnMan(smgr)
	{

	} // Constructor.

	//------------
	// Destructor.
	//------------
	MS3DLoader::~MS3DLoader()
	{
		KGE_DELETE_ARRAY(m_pVertices);
		KGE_DELETE_ARRAY(m_pTriangles);
		KGE_DELETE_CLASS_ARRAY(m_pMeshes, ms3dGroup, m_iNumMeshes);
		KGE_DELETE_ARRAY(m_pMaterials);
		KGE_DELETE_ARRAY(m_pJoints);

	} // Destructor.

	//-----------------------------------
	// Load a Mesh from MS3D file format.
	//-----------------------------------
	MeshBuffer* MS3DLoader::Load(const char* FileName)
	{
		if ( !LoadFile(FileName) )
			return NULL;

		//if ( !LoadFromArchive(FileName) )
		//    return NULL;

		// Global

		//-----------------
		// Creating Meshes.
		//-----------------
		sn::Mesh *pMeshes;

		pMeshes = KGE_NEW_CLASS_ARRAY(sn::Mesh, m_iNumMeshes);

		u32 VertsCount = 0;
		core::ArrayPri<u16> IndexArray;
		math::Sphere* bb;
		math::AABB*	aabb;
		for ( int i = 0 ; i < m_iNumMeshes ; i++ )
		{
			// get bounding box.
			bb = pMeshes[i].GetBoundingBox();
			aabb = pMeshes[i].GetAxisAlignedBoundingBox();

			// 1. calculate the number of vertices.
			VertsCount = 0;
			IndexArray.Clear();

			// 1-2 sort the indices and create mesh indices.
			u16* Indices = KGE_NEW_ARRAY(u16, m_pMeshes[i].m_usNumTris * 3);
			for ( u16 j = 0; j < m_pMeshes[i].m_usNumTris; j++ )
			{
				ms3dTriangle * pCurTri = &m_pTriangles[m_pMeshes[i].m_uspIndices[j]];
				IndexArray.PushBackOne(m_pMeshes[i].m_uspIndices[j] , (int)pCurTri->m_usVertIndices[0]);
				IndexArray.PushBackOne(m_pMeshes[i].m_uspIndices[j] , (int)pCurTri->m_usVertIndices[1]);
				IndexArray.PushBackOne(m_pMeshes[i].m_uspIndices[j] , (int)pCurTri->m_usVertIndices[2]);
				Indices[j * 3    ] = pCurTri->m_usVertIndices[0];
				Indices[j * 3 + 1] = pCurTri->m_usVertIndices[1];
				Indices[j * 3 + 2] = pCurTri->m_usVertIndices[2];

			} // for j

			// Assign indices to the mesh.
			pMeshes[i].m_pIndices   = Indices;
			pMeshes[i].m_NumIndices = m_pMeshes[i].m_usNumTris * 3;

			Vertex3*   VerticesPos = KGE_NEW_ARRAY(Vertex3, IndexArray.Count());
			Vertex3T* VerticesNorT = KGE_NEW_ARRAY(Vertex3T, IndexArray.Count());
			pMeshes[i].m_Bone      = KGE_NEW_ARRAY(s16, IndexArray.Count());
			pMeshes[i].m_NumVerts  = IndexArray.Count();
			pMeshes[i].m_pRenderer = m_pRenderer;
			pMeshes[i].m_sName     = m_pMeshes[i].m_cName;

			// Create the Material.
			int MatID = (int)m_pMeshes[i].m_cMaterial;
			if (MatID != -1)
			{
				pMeshes[i].m_Mat.Name = m_pMaterials[MatID].m_cName;
				memcpy ( &pMeshes[i].m_Mat.Ambient , &m_pMaterials[MatID].m_fAmbient[0] , 68 );

				if ( m_pMaterials[MatID].m_fTransparency < 1.0f )
				{
					pMeshes[i].m_Mat.Alpha = true;
					pMeshes[i].m_Mat.Diffuse.a = m_pMaterials[MatID].m_fTransparency;
				}
				else
					pMeshes[i].m_Mat.Alpha = false;

				// Load the textures.
				if ( strcmp(m_pMaterials[MatID].m_cTexture , "") )
				{
					core::String str, str2;
					str = FileName;
					if (!str.Select(0 , str.FindLast("/"), str2))
						str.Select(0 , str.FindLast("\\"), str2);
					str = m_pMaterials[MatID].m_cTexture;
					if ( str.Find(".\\") > -1 )
						str2 += &m_pMaterials[MatID].m_cTexture[2];
					else
						str2 += m_pMaterials[MatID].m_cTexture;
					pMeshes[i].m_Mat.ppTexture[0] = m_pSnMan->AddTexture(str2.ToCharPointer());
				}
				else
					pMeshes[i].m_Mat.ppTexture[0] = NULL;

				pMeshes[i].m_bHasMat     = true;
			}
			else
				pMeshes[i].m_bHasMat = false;

			// Create the Vertices.
			for (u32 c = 0; c < IndexArray.Count(); c++)
			{
				int VertexIndex;
				u16 TriIndex			= IndexArray.GetItem(c, VertexIndex);
				ms3dTriangle * pCurTri	= &m_pTriangles[TriIndex];
				VerticesPos[c]			= m_pVertices[VertexIndex].m_vVert;
				pMeshes[i].m_Bone[c]	= m_pVertices[VertexIndex].m_cBone;

				// Create bounding box.
				bb->AddPoint( &VerticesPos[c] );
				aabb->AddInternalPoint( &VerticesPos[c] );

				for (int m = 0; m < m_pMeshes[i].m_usNumTris * 3; m++)
				{
					if (Indices[m] == VertexIndex)
						Indices[m] = c;
				}

				if ( pCurTri->m_usVertIndices[0] == (u16)VertexIndex )
				{
					VerticesNorT[c].tex.X	= pCurTri->m_fTexCoords[0][0];
					VerticesNorT[c].tex.Y	= pCurTri->m_fTexCoords[1][0];
					VerticesNorT[c].pos		= pCurTri->m_vNormals[0];
				}
				else if ( pCurTri->m_usVertIndices[1] == (u16)VertexIndex )
				{
					VerticesNorT[c].tex.X	= pCurTri->m_fTexCoords[0][1];
					VerticesNorT[c].tex.Y	= pCurTri->m_fTexCoords[1][1];
					VerticesNorT[c].pos		= pCurTri->m_vNormals[1];
				}
				else if ( pCurTri->m_usVertIndices[2] == (u16)VertexIndex )
				{
					VerticesNorT[c].tex.X	= pCurTri->m_fTexCoords[0][2];
					VerticesNorT[c].tex.Y	= pCurTri->m_fTexCoords[1][2];
					VerticesNorT[c].pos		= pCurTri->m_vNormals[2];
				}
			} // for c (Vertex count)

			pMeshes[i].m_pPos		= VerticesPos;
			pMeshes[i].m_pNorTex	= VerticesNorT;
			RecalculateNormals_Smooth(VerticesPos, VerticesNorT, pMeshes[i].m_NumVerts, pMeshes[i].m_pIndices, pMeshes[i].m_NumIndices);

		} // for Meshes.

		//-----------------------------
		// Create Bones And Animations.
		//-----------------------------
		sn::Bone	 * Bones	= KGE_NEW_CLASS_ARRAY(sn::Bone, m_iNumJoints);
		AnimationData* AniDatas = KGE_NEW_CLASS_ARRAY(AnimationData, m_iNumJoints);
		Animation	 * Ani		= KGE_NEW(Animation);

		for ( u16 i = 0; i < m_iNumJoints; i++)
		{
			// Copy the bone name.
			Bones[i].SetName(m_pJoints[i].m_cName);

			// Copy the Start values.
			Bones[i].SetStartPosition(m_pJoints[i].m_vPosition);
			Bones[i].SetStartRotation(m_pJoints[i].m_vRotation);
			Bones[i].SetParent(m_pJoints[i].m_sParent);

			// Copy the Animation data.
			AniDatas[i].m_iNumTransKeys = m_pJoints[i].m_usNumTransFrames;
			AniDatas[i].m_iNumRotKeys	= m_pJoints[i].m_usNumRotFrames;
			AniDatas[i].m_pTransKeys	= m_pJoints[i].m_TransKeyFrames;
			AniDatas[i].m_pRotKeys		= m_pJoints[i].m_RotKeyFrames;

		} // For Bones.

		Ani->m_pAnis			= AniDatas;
		Ani->m_fFPS				= m_fFPS;
		Ani->m_fTotalTime		= m_fTotalTime;
		Ani->m_iFramesCount		= m_iFramesCount;
		Ani->m_iAnimationCount	= m_iNumJoints;

		// Set up the meshes with the joints starting values.
		if ( m_iNumJoints > 0 )
		{
			u32 x;
			for (x = 0; x < m_iNumJoints; x++)
			{
				Bones[x].m_matLocal.SetRotation(&m_pJoints[x].m_vRotation[0]);
				Bones[x].m_matLocal.SetTranslation(&m_pJoints[x].m_vPosition[0]);

				if (Bones[x].m_iParent != -1)
				{
					Bones[x].m_matAbs = Bones[Bones[x].m_iParent].m_matAbs;
					Bones[x].m_matAbs *= Bones[x].m_matLocal;
				}
				else
				{
					Bones[x].m_matAbs = Bones[x].m_matLocal;
				}
				Bones[x].m_matFinal = Bones[x].m_matAbs;

			} // for (Joints)

			for (x = 0; x < m_iNumMeshes; x++)
			{
				for (u32 z = 0; z < pMeshes[x].m_NumVerts; z++)
				{
					if (pMeshes[x].m_Bone[z] == -1)
						continue;
					math::Matrix * mat = &Bones[pMeshes[x].m_Bone[z]].m_matFinal;

					mat->InverseTranslateVec(pMeshes[x].m_pPos[z]);
					mat->InverseRotateVec(pMeshes[x].m_pPos[z]);
					mat->InverseRotateVec(pMeshes[x].m_pNorTex[z].pos);

				} // for (Vertices)

				pMeshes[x].PrepareBuffers(false, false);

			} // for (Meshes)

		} // if the mesh has any joints
		else
		{
			for (u32 x = 0; x < m_iNumMeshes; x++)
			{
				pMeshes[x].PrepareBuffers();
			}

		}

		// Save the mesh to a mesh buffer.
		MeshBuffer* mb   = KGE_NEW(MeshBuffer)();
		mb->m_pMeshes    = pMeshes;
		mb->m_pBones     = Bones;
		mb->m_pAnimation = Ani;
		mb->m_iMeshCount = m_iNumMeshes;
		mb->m_iBoneCount = m_iNumJoints;
		mb->m_sName      = FileName;

		// Free the memory.
		KGE_DELETE_ARRAY(m_pVertices);
		KGE_DELETE_ARRAY(m_pTriangles);
		KGE_DELETE_CLASS_ARRAY(m_pMeshes, ms3dGroup, m_iNumMeshes);
		KGE_DELETE_ARRAY(m_pMaterials);
		KGE_DELETE_ARRAY(m_pJoints);

		return mb;

	} // Load [animatedmesh*]

	//-----------------------------------
	// Load a Mesh from MS3D file format.
	//-----------------------------------
	MeshBuffer* MS3DLoader::LoadStatic(const char* FileName)
	{
		if ( !LoadFile(FileName) )
			return NULL;

		//if ( !LoadFromArchive(FileName) )
		//	return NULL;

		//-----------------
		// Creating Meshes.
		//-----------------
		sn::Mesh *pMeshes = KGE_NEW_CLASS_ARRAY(sn::Mesh, m_iNumMeshes);

		u32 VertsCount = 0;
		math::Sphere* bb;
		math::AABB*	aabb;
		for ( int i = 0 ; i < m_iNumMeshes ; i++ )
		{
			// get bounding box.
			bb = pMeshes[i].GetBoundingBox();
			aabb = pMeshes[i].GetAxisAlignedBoundingBox();

			// 1. calculate the number of vertices.
			VertsCount = 0;

			// 1-2 sort the indices and create mesh indices.
			u16* Indices = KGE_NEW_ARRAY(u16, m_pMeshes[i].m_usNumTris * 3);
			for ( u16 j = 0; j < m_pMeshes[i].m_usNumTris; j++ )
			{
				Indices[j * 3    ] = j * 3    ;
				Indices[j * 3 + 1] = j * 3 + 1;
				Indices[j * 3 + 2] = j * 3 + 2;

			} // for j

			// Assign indices to the mesh.
			pMeshes[i].m_pIndices   = Indices;
			pMeshes[i].m_NumIndices = m_pMeshes[i].m_usNumTris * 3;

			Vertex3TN* Vertices    = KGE_NEW_ARRAY(Vertex3TN, pMeshes[i].m_NumIndices);
			pMeshes[i].m_Bone      = KGE_NEW_ARRAY(s16, pMeshes[i].m_NumIndices);
			pMeshes[i].m_NumVerts  = pMeshes[i].m_NumIndices;
			pMeshes[i].m_pRenderer = m_pRenderer;
			pMeshes[i].m_sName     = m_pMeshes[i].m_cName;

			// Create the Material.
			int MatID = (int)(float)m_pMeshes[i].m_cMaterial;
			if (MatID != -1)
			{
				pMeshes[i].m_Mat.Name = m_pMaterials[MatID].m_cName;
				memcpy ( &pMeshes[i].m_Mat.Ambient , &m_pMaterials[MatID].m_fAmbient[0] , 68 );

				if ( m_pMaterials[MatID].m_fTransparency < 1.0f )
				{
					pMeshes[i].m_Mat.Alpha = true;
					pMeshes[i].m_Mat.Diffuse.a = m_pMaterials[MatID].m_fTransparency;
				}
				else
					pMeshes[i].m_Mat.Alpha = false;

				// Load the textures.
				if ( strcmp(m_pMaterials[MatID].m_cTexture , "") )
				{
					core::String str, str2;
					str = FileName;
					if (!str.Select(0 , str.FindLast("/"), str2))
						str.Select(0 , str.FindLast("\\"), str2);
					str = m_pMaterials[MatID].m_cTexture;
					if ( str.Find(".\\") > -1 )
						str2 += &m_pMaterials[MatID].m_cTexture[2];
					else
						str2 += m_pMaterials[MatID].m_cTexture;
					pMeshes[i].m_Mat.ppTexture[0] = m_pSnMan->AddTexture(str2.ToCharPointer());
				}
				else
				{
					pMeshes[i].m_Mat.ppTexture[0] = NULL;
				}

				pMeshes[i].m_bHasMat     = true;
			}
			else
				pMeshes[i].m_bHasMat = false;

			// Create the Vertices.
			for (u32 y = 0; y < m_pMeshes[i].m_usNumTris; y++)
			{
				ms3dTriangle * pCurTri	= &m_pTriangles[m_pMeshes[i].m_uspIndices[y]];

				for (int f = 0; f < 3; f++)
				{
					int VertexIndex = pCurTri->m_usVertIndices[f];
					int c = y * 3 + f;
					Vertices[c].pos			= m_pVertices[VertexIndex].m_vVert;
					pMeshes[i].m_Bone[c]	= m_pVertices[VertexIndex].m_cBone;
					Vertices[c].tex.X	= pCurTri->m_fTexCoords[0][f];
					Vertices[c].tex.Y	= pCurTri->m_fTexCoords[1][f];
					Vertices[c].Nor		= pCurTri->m_vNormals[f];

					// Create bounding box.
					bb->AddPoint( &Vertices[c].pos );
					aabb->AddInternalPoint( &Vertices[c].pos );
				}
			}

			pMeshes[i].m_pVerts = Vertices;

			pMeshes[i].PrepareBuffers();
			//pMeshes[i].m_pIndexBuffer  = m_pRenderer->CreateIndexBuffer(Indices,
			//							 m_pMeshes[i].m_usNumTris * 3);

			// TODO
			//			delete[] Vertices;

		} // for Meshes.

		// Set The Animated mesh Pointers.
		// Save the mesh to a mesh buffer.
		MeshBuffer* mb   = KGE_NEW(MeshBuffer)();
		mb->m_pMeshes    = pMeshes;
		mb->m_iMeshCount = m_iNumMeshes;
		mb->m_sName      = FileName;

		// Free the memory.
		KGE_DELETE_ARRAY(m_pVertices);
		KGE_DELETE_ARRAY(m_pTriangles);
		KGE_DELETE_CLASS_ARRAY(m_pMeshes, ms3dGroup, m_iNumMeshes);
		KGE_DELETE_ARRAY(m_pMaterials);
		KGE_DELETE_ARRAY(m_pJoints);

		return mb;

	} // Load Static

	//------------------------------
	// Load the ms3d mesh from file.
	//------------------------------
	bool MS3DLoader::LoadFile(const char* FileName)
	{
		u8* Buffer = 0;
		u8* ptr = 0;

		io::File f;
		if (!f.Open(FileName))
			return false;

		// Create the memory for reading the file.
		u32 FileSize = f.GetSize();
		Buffer = KGE_NEW_ARRAY(u8, FileSize); //new (std::nothrow) u8[FileSize];
		ptr = Buffer;

		if (!Buffer)
		{
			io::Logger::Log("Out Of Memory.", io::ELM_Error);
			return false;
		}

		// Read file to the Buffer.

		if (f.Read(Buffer, FileSize) == -1)
		{
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}

		// Check out the header, it should be 10 bytes, MS3D000000
		if (memcmp(ptr, "MS3D000000", 10) != 0)
		{
			io::Logger::Log(FileName, " is not a valid Milkshape 3D file",
							io::ELM_Error);
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}
		ptr += 10;

		// Check the version (should be 3 or 4)
		if (*(int *)ptr != 3 && *(int *)ptr != 4)
		{
			io::Logger::Log(FileName, " is the wrong version, should be 3 or 4",
							io::ELM_Error);
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}
		ptr += 4;

		// Read the vertices
		// Number of vertices
		m_iNumVerts = *(u16 *)ptr;
		ptr += 2;

		// Allocate memory
		m_pVertices = KGE_NEW_ARRAY(ms3dVertex, m_iNumVerts);
		if (!m_pVertices)
		{
			io::Logger::Log("Out Of Memory.", io::ELM_Error);
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}
		// Copy the vertices
		memcpy(m_pVertices, ptr, m_iNumVerts * sizeof(ms3dVertex));
		ptr += m_iNumVerts * sizeof(ms3dVertex);

		//	Read the triangles
		m_iNumTriangles = *(u16 *)ptr;
		ptr += 2;
		// Alloc memory for triangles
		m_pTriangles = KGE_NEW_ARRAY(ms3dTriangle, m_iNumTriangles);
		if (!m_pTriangles)
		{
			io::Logger::Log("Out Of Memory.", io::ELM_Error);
			KGE_DELETE_ARRAY(m_pVertices);
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}
		// Copy triangles
		memcpy(m_pTriangles, ptr, m_iNumTriangles * sizeof(ms3dTriangle));
		ptr += m_iNumTriangles * sizeof(ms3dTriangle);

		// Load mesh groups
		m_iNumMeshes = *(u16 *)ptr;
		ptr += 2;
		// Alloc memory for the mesh data
		m_pMeshes = KGE_NEW_CLASS_ARRAY(ms3dGroup, m_iNumMeshes);
		if (!m_pMeshes)
		{
			io::Logger::Log("Out Of Memory.", io::ELM_Error);
			KGE_DELETE_ARRAY(m_pTriangles);
			KGE_DELETE_ARRAY(m_pVertices);
			KGE_DELETE_ARRAY(Buffer);
			return false;
		}
		// Copy the mesh data
		u32 x;
		for (x = 0; x < m_iNumMeshes; x++)
		{
			// Copy the first part of the data
			memcpy(&m_pMeshes[x], ptr, 35);
			ptr += 35;
			// Allocate triangle index memory
			m_pMeshes[x].m_uspIndices = KGE_NEW_ARRAY(u16, m_pMeshes[x].m_usNumTris);
			// Copy triangle index data, plus the material index
			memcpy(m_pMeshes[x].m_uspIndices, ptr, m_pMeshes[x].m_usNumTris*2);
			ptr += m_pMeshes[x].m_usNumTris*2;
			m_pMeshes[x].m_cMaterial = ptr[0];
			ptr++;
		}

		// Read material information
		m_iNumMaterials = *(u16 *)ptr;
		ptr += 2;
		// Alloc memory
		m_pMaterials = KGE_NEW_ARRAY(ms3dMaterial, m_iNumMaterials);
		// Copy material information
		for(x = 0; x < m_iNumMaterials; x++)
		{
			memcpy(&m_pMaterials[x], ptr, 361);
			ptr += 361;
		}

		m_fFPS = *(float*)ptr;
		ptr += 8;
		m_iFramesCount = *(int*)ptr;
		ptr += 4;

		m_fTotalTime = (m_iFramesCount / m_fFPS);

		//Read in joint and animation info
		m_iNumJoints = *(u16 *)ptr;
		ptr += 2;
		//Allocate memory
		m_pJoints = KGE_NEW_ARRAY(ms3dJoint, m_iNumJoints);
		//Read in joint info
		for(x = 0; x < m_iNumJoints; x++)
		{
			memcpy(&m_pJoints[x], ptr, 93);
			ptr += 93;
			// Allocate memory
			m_pJoints[x].m_RotKeyFrames = KGE_NEW_ARRAY(KeyFrame, m_pJoints[x].m_usNumRotFrames);
			m_pJoints[x].m_TransKeyFrames = KGE_NEW_ARRAY(KeyFrame, m_pJoints[x].m_usNumTransFrames);
			// copy keyframe information
			memcpy(m_pJoints[x].m_RotKeyFrames, ptr, m_pJoints[x].m_usNumRotFrames * sizeof(KeyFrame));
			ptr += m_pJoints[x].m_usNumRotFrames * sizeof(KeyFrame);
			memcpy(m_pJoints[x].m_TransKeyFrames, ptr, m_pJoints[x].m_usNumTransFrames * sizeof(KeyFrame));
			ptr += m_pJoints[x].m_usNumTransFrames * sizeof(KeyFrame);

			//for (int l = 0; l < m_pJoints[x].m_usNumTransFrames; l++)
			//	m_pJoints[x].m_TransKeyFrames[l].m_fParam[2] = -m_pJoints[x].m_TransKeyFrames[l].m_fParam[2];
			//for (int l = 0; l < m_pJoints[x].m_usNumRotFrames; l++)
			//{
			//	//m_pJoints[x].m_RotKeyFrames[l].m_fParam[0] = -m_pJoints[x].m_RotKeyFrames[l].m_fParam[0];
			//	//m_pJoints[x].m_RotKeyFrames[l].m_fParam[1] = m_pJoints[x].m_RotKeyFrames[l].m_fParam[1];
			//	m_pJoints[x].m_RotKeyFrames[l].m_fParam[2] = -m_pJoints[x].m_RotKeyFrames[l].m_fParam[2];
			//}
		}

		//Find the parent joint array indices
		for(x = 0; x < m_iNumJoints; x++)
		{
			//If the bone has a parent
			if(m_pJoints[x].m_cParent[0] != '\0')
			{
				//Compare names of the parent bone of x with the names of all bones
				for(int y = 0; y < m_iNumJoints; y++)
				{
					//A match has been found
					if(strcmp(m_pJoints[y].m_cName, m_pJoints[x].m_cParent) == 0)
					{
						m_pJoints[x].m_sParent = y;
					}
				}
			}
			//The bone has no parent
			else
			{
				m_pJoints[x].m_sParent = -1;
			}
		}

		KGE_DELETE_ARRAY(Buffer);

//		for (int i = 0; i < m_iNumVerts; i++)
//		{
//			m_pVertices[i].m_vVert.Z = -m_pVertices[i].m_vVert.Z;
//		}
		//
		//for (int i = 0; i < m_iNumTriangles; i++)
		//{
		//	m_pTriangles[i].m_vNormals[0].Z = -m_pTriangles[i].m_vNormals[0].Z;
		//	m_pTriangles[i].m_vNormals[1].Z = -m_pTriangles[i].m_vNormals[1].Z;
		//	m_pTriangles[i].m_vNormals[2].Z = -m_pTriangles[i].m_vNormals[2].Z;
		//}

#ifdef WIN32
		f.Close();

#endif // WIN32

		return true;

	} // Load File

	//------------------------------
	// Loads the ms3d mesh from archive.
	//------------------------------
	bool MS3DLoader::LoadFromArchive(const char* filename)
	{
		//std::string name(filename);
		//std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		//std::string path("../../media/"); //
		//std::string::size_type pos = name.find(path);
		//if (pos != std::string::npos)
		//{
		//	name = name.substr(pos + path.size());
		//}

		//if (!PHYSFS_exists(name.c_str())) return false;
		//PHYSFS_File* file = PHYSFS_openRead(name.c_str());



		//if (!PHYSFS_exists(filename)) return false;

		//PHYSFS_File* file = PHYSFS_openRead(filename);

		//const unsigned int length = PHYSFS_fileLength(file);

		//u8* buffer = new (std::nothrow) u8[length];

		//if (!buffer)
		//{
		//	delete[] buffer;
		//	PHYSFS_close(file);
		//	return false;
		//}

		//const unsigned int readLen = PHYSFS_read(file, buffer, sizeof(u8), length);

		//PHYSFS_close(file);

		//if (readLen != length)
		//{
		//	delete[] buffer;
		//	return false;
		//}

		//const u8* ptr = buffer;

		//// Check out the header, it should be 10 bytes, MS3D000000
		//if (memcmp(ptr, "MS3D000000", 10) != 0)
		//{
		//	io::Logger::Log(filename, " is not a valid Milkshape 3D file",
		//		io::ELM_Error);
		//	delete[] buffer;
		//	return false;
		//}

		//ptr += 10;

		//// Check the version (should be 3 or 4)
		//if (*(int *)ptr != 3 && *(int *)ptr != 4)
		//{
		//	io::Logger::Log(filename, " is the wrong version, should be 3 or 4",
		//		io::ELM_Error);
		//	delete[] buffer;
		//	return false;
		//}

		//ptr += 4;

		//// Read the vertices
		//// Number of vertices
		//m_iNumVerts = *(u16 *)ptr;
		//ptr += 2;

		//// Allocate memory
		//m_pVertices = new (std::nothrow) ms3dVertex[m_iNumVerts];

		//if (!m_pVertices)
		//{
		//	io::Logger::Log("Out Of Memory.", io::ELM_Error);
		//	delete[] buffer;
		//	return false;
		//}

		//// Copy the vertices
		//memcpy(m_pVertices, ptr, m_iNumVerts * sizeof(ms3dVertex));
		//ptr += m_iNumVerts * sizeof(ms3dVertex);

		////	Read the triangles
		//m_iNumTriangles = *(u16 *)ptr;
		//ptr += 2;
		//// Alloc memory for triangles
		//m_pTriangles = new (std::nothrow) ms3dTriangle[m_iNumTriangles];
		//if (!m_pTriangles)
		//{
		//	io::Logger::Log("Out Of Memory.", io::ELM_Error);
		//	delete[] m_pVertices;
		//	delete[] buffer;
		//	return false;
		//}
		//// Copy triangles
		//memcpy(m_pTriangles, ptr, m_iNumTriangles * sizeof(ms3dTriangle));
		//ptr += m_iNumTriangles * sizeof(ms3dTriangle);

		//// Load mesh groups
		//m_iNumMeshes = *(u16 *)ptr;
		//ptr += 2;
		//// Alloc memory for the mesh data
		//m_pMeshes = new (std::nothrow) ms3dGroup[m_iNumMeshes];
		//if (!m_pMeshes)
		//{
		//	io::Logger::Log("Out Of Memory.", io::ELM_Error);
		//	delete[] m_pTriangles;
		//	delete[] m_pVertices;
		//	delete[] buffer;
		//	return false;
		//}
		//// Copy the mesh data
		//for (u32 x = 0; x < m_iNumMeshes; x++)
		//{
		//	// Copy the first part of the data
		//	memcpy(&m_pMeshes[x], ptr, 35);
		//	ptr += 35;
		//	// Allocate triangle index memory
		//	m_pMeshes[x].m_uspIndices = new u16[m_pMeshes[x].m_usNumTris];
		//	// Copy triangle index data, plus the material index
		//	memcpy(m_pMeshes[x].m_uspIndices, ptr, m_pMeshes[x].m_usNumTris*2);
		//	ptr += m_pMeshes[x].m_usNumTris*2;
		//	m_pMeshes[x].m_cMaterial = ptr[0];
		//	ptr++;
		//}

		//// Read material information
		//m_iNumMaterials = *(u16 *)ptr;
		//ptr += 2;
		//// Alloc memory
		//m_pMaterials = new ms3dMaterial[m_iNumMaterials];
		//// Copy material information
		//for(u32 x = 0; x < m_iNumMaterials; x++)
		//{
		//	memcpy(&m_pMaterials[x], ptr, 361);
		//	ptr += 361;
		//}

		//m_fFPS = *(float*)ptr;
		//ptr += 8;
		//m_iFramesCount = *(int*)ptr;
		//ptr += 4;

		//m_fTotalTime = (m_iFramesCount / m_fFPS);

		////Read in joint and animation info
		//m_iNumJoints = *(u16 *)ptr;
		//ptr += 2;
		////Allocate memory
		//m_pJoints = new ms3dJoint[m_iNumJoints];
		////Read in joint info
		//for(u32 x = 0; x < m_iNumJoints; x++)
		//{
		//	memcpy(&m_pJoints[x], ptr, 93);
		//	ptr += 93;
		//	// Allocate memory
		//	m_pJoints[x].m_RotKeyFrames = new KeyFrame[m_pJoints[x].m_usNumRotFrames];
		//	m_pJoints[x].m_TransKeyFrames = new KeyFrame[m_pJoints[x].m_usNumTransFrames];
		//	// copy key frame information
		//	memcpy(m_pJoints[x].m_RotKeyFrames, ptr, m_pJoints[x].m_usNumRotFrames * sizeof(KeyFrame));
		//	ptr += m_pJoints[x].m_usNumRotFrames * sizeof(KeyFrame);
		//	memcpy(m_pJoints[x].m_TransKeyFrames, ptr, m_pJoints[x].m_usNumTransFrames * sizeof(KeyFrame));
		//	ptr += m_pJoints[x].m_usNumTransFrames * sizeof(KeyFrame);

		//}

		////Find the parent joint array indices
		//for(u32 x = 0; x < m_iNumJoints; x++)
		//{
		//	//If the bone has a parent
		//	if(m_pJoints[x].m_cParent[0] != '\0')
		//	{
		//		//Compare names of the parent bone of x with the names of all bones
		//		for(int y = 0; y < m_iNumJoints; y++)
		//		{
		//			//A match has been found
		//			if(strcmp(m_pJoints[y].m_cName, m_pJoints[x].m_cParent) == 0)
		//			{
		//				m_pJoints[x].m_sParent = y;
		//			}
		//		}
		//	}
		//	//The bone has no parent
		//	else
		//	{
		//		m_pJoints[x].m_sParent = -1;
		//	}
		//}

		//delete[] buffer;

		return true;

	} // LoadFromArchive

} // gfx

} // kge
