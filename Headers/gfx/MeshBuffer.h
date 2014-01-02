// File name: MeshBuffer.h
// Des: This class is for buffering the meshes.
// Date: 3/10/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef MESHBUFFER_H
#define MESHBUFFER_H

#include "../../Include/gfx/Structs.h"
#include "../../Include/gfx/Animation.h"
#include "../../Include/gfx/Material.h"
#include "../../Include/kgedef.h"
#include "../../Include/core/String.h"
#include "../../Include/sn/Mesh.h"
#include "../../Include/sn/Bone.h"

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Forward declaration 
	//------------------------------------------------------------------------------------
	class GeometryBatch;

class MeshBuffer : public KgeUnknown
{
public:

	// Constructor.
	MeshBuffer();

	// Destructor.
	~MeshBuffer();

	/// Override AddRef to find out if we need instancing
	void AddRef();

	/// Override DecRef to find out if we don't need instancing anymore
	void DecRef();

	//--------------------------------------------------------------------
	//   P R O P E R T I E S
	//--------------------------------------------------------------------

	core::String		m_sName;			// Name of the mesh file.

	sn::Mesh		*	m_pMeshes;			// Pointer to the mesh array.

	sn::Bone		*	m_pBones;			// Pointer to the bones array.

	Animation		*	m_pAnimation;		// Pointer to the Animations.

	u32					m_iMeshCount,		// The size of the meshes.
						m_iBoneCount;		// The size of the bone count.

	Shader			*	m_pVertexShader,	// The mesh buffer vertex shader
					*	m_pPixelShader;		// The mesh buffer pixel shader

	GeometryBatch	*	m_pBatch;			///< The Geometry Batch for instanced meshes

	bool				m_bShaderSet,		//!< Checks if the shader params is set for the hardware skinning.
						m_bInstanced;		///< Is it instanced or not?

}; // MeshBuffer


//!
MeshBuffer* LoadStaticMesh(const char* filename, Renderer* renderer, sn::SceneManager* sceneManager);

//!
void SaveStaticMesh(MeshBuffer* meshBuffer);

//!
MeshBuffer* LoadAnimatedMesh(const char* filename, Renderer* renderer, sn::SceneManager* sceneManager);

//!
void SaveAnimatedMesh(MeshBuffer* meshBuffer);


} // gfx

} // kge

#endif // MESHBUFFER_H
