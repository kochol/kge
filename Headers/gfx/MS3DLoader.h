// File name: MS3DLoader.h
// Des: This class loads the MS3D meshes and animations.
// Date: 25/1/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef MS3DLOADER_H
#define MS3DLOADER_H

#include "../../Include/sn/AnimatedMesh.h"
#include "../../Include/sn/StaticMesh.h"
#include "../../Include/kgedef.h"
#include "../../Include/gfx/Structs.h"
#include "../../Include/gfx/MeshBuffer.h"

  ////////////////////////////////////////////////////////////////////////////////////
 // In struct ha makhsose file MS3D hastand.                                       //
////////////////////////////////////////////////////////////////////////////////////

using namespace kge;
using namespace kge::gfx;

// byte-align structures
#ifdef _MSC_VER
#	pragma pack(push, packing)
#	pragma pack(1)
#	define PACK_STRUCT
#elif defined(__GNUC__)
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

// MS3D Vertex Structure
struct ms3dVertex
{
	u8      m_ucFlags; //Editor flags, unused for the loader
	Vertex3 m_vVert;  //X,Y,Z coordinates
	char	m_cBone; //Bone ID (-1 = no bone)
	u8		m_ucUnused;
} PACK_STRUCT; // ms3dVertex

// MS3D Triangle data structure
struct ms3dTriangle
{
	u16		m_usFlags;			//Editor flags
	u16		m_usVertIndices[3]; //Vertex Indices
	Vertex3 m_vNormals[3];		//Vertex normals;
	float	m_fTexCoords[2][3];	//Texture coordinates
	u8		m_ucSmoothing;		//Smoothing group
	u8		m_ucGroup;			//Group index
} PACK_STRUCT;

// Group of triangles in the ms3d file
struct ms3dGroup
{
	u8   m_ucFlags;		//Editor flags again
	char m_cName[32];	//Name of the mesh
	u16  m_usNumTris;	//Number of triangles in the group
	u16* m_uspIndices;	//Triangle Indices
	char m_cMaterial;	//Material index, -1 = no material
	
	//Let it clean up after itself like usual
	ms3dGroup()
	{
		m_ucFlags = 0;
		m_cName[0] = 0;
		m_usNumTris = 0;
		m_uspIndices = 0;
		m_cMaterial = -1;
	}

	~ms3dGroup()
	{
		KGE_DELETE_ARRAY(m_uspIndices);
	}
} PACK_STRUCT;

// Material information for the mesh
struct ms3dMaterial
{
	char  m_cName[32];		//Material name
	float m_fAmbient[4];	//Ambient values
	float m_fDiffuse[4];	//Diffuse values
	float m_fSpecular[4];	//Specular values
	float m_fEmissive[4];	//Emissive values
	float m_fShininess;		//0 - 128
	float m_fTransparency;	//0 - 1
	char  m_cMode;			//unused
	char  m_cTexture[128];  //Texture map file
	char  m_cAlpha[128];	//Alpha map file
} PACK_STRUCT;

// Bone Joints for animation
struct ms3dJoint
{
	// Data from file
	u8 m_ucpFlags; //Editor flags
	char m_cName[32]; //Bone name
	char m_cParent[32]; //Parent name
	float m_vRotation[3]; //Starting rotation
	float m_vPosition[3]; //Starting position
	u16 m_usNumRotFrames; //Number of rotation frames
	u16 m_usNumTransFrames; //Number of translation frames
	KeyFrame * m_RotKeyFrames; //Rotation keyframes
	KeyFrame * m_TransKeyFrames; //Translation keyframes
	// Data not loaded from file
	short m_sParent; //Parent joint index
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop, packing)
#endif

#undef PACK_STRUCT

namespace kge
{
namespace gfx
{

// MS3D Loader Class
class MS3DLoader
{
public:

	/*! Counstructor.
	\param renderer A pointer to Renderer.
	*/
	MS3DLoader(Renderer* renderer, sn::SceneManager* smgr);

	// Destructor.
	~MS3DLoader();

	// Load a Mesh from MS3D file format.
	// \param FileName esme file e ke bayad load shavd.
	// \return agar movafagh shod file ro load kone true bar migardone.
	MeshBuffer* Load(const char* FileName);

	// Load a Mesh from MS3D file format.
	MeshBuffer* LoadStatic(const char* FileName);

protected:

	u16				m_iNumVerts,		// The count of vertices.
					m_iNumTriangles,	// The count of triangles.
					m_iNumMeshes,		// The count of meshes.
					m_iNumMaterials,	// The count of materials.
					m_iNumJoints;		// The count of bones.

	float			m_fFPS,				// The frames per second for animation.
					m_fTotalTime;		// The total animation time.

	int				m_iFramesCount;		// The frames count.

	ms3dVertex*		m_pVertices;		// The vertices.

	ms3dTriangle*	m_pTriangles;		// The triangles.

	ms3dGroup*		m_pMeshes;			// The Meshes.

	ms3dMaterial*	m_pMaterials;		// The Materials.

	ms3dJoint*		m_pJoints;			// The Bones.
	
	Renderer*		m_pRenderer;		// The Renderer.

	sn::SceneManager* m_pSnMan;			// The Scene Manager.

	// Load the ms3d mesh from file.
	bool LoadFile(const char* FileName);

	// Loads the ms3d mesh from archive.
	bool LoadFromArchive(const char* filename);

}; // M3SDLoader

} // gfx

} // kge

#endif // MS3DLOADER_H