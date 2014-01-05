// File name: StaticMesh.h
// Des: This is a node class for static meshes which is more faster.
// Date: 19/4/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef STATICMESH_H
#define STATICMESH_H

#include "SceneNode.h"
#include "../../Include/sn/Mesh.h"

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Forward declaration 
	//------------------------------------------------------------------------------------
	class MeshBuffer;

} // gfx

namespace sn
{
//! This is a node class for static meshes which is more faster.
class KGE_API StaticMesh : public SceneNode
{
public:

	//! Constructor.
	StaticMesh(gfx::MeshBuffer* pMeshBuffer);

	//! Destructor.
	~StaticMesh();

	//! Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! Render kardane Object.
	virtual void Render();

	//! Karhaee ke bad az render bayad anjam beshe.
	virtual void PostRender();

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut );

	/*! Draw the node with/without its Materials and Transforms.
		\param WithMaterial Draw it with its own materials or not?
		\param WithMaterial Draw it with its own Transformations or not?
	 */
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	virtual void SetShader(gfx::ShaderInstance* shader);

	//! Get the count of materials this node has.
	virtual u32 GetMaterialCount();

	/*! Get the node material by its index.
	\param Index The material index. Some nodes have more than one material.
	*/
	virtual gfx::Material* GetMaterial(u32 Index);

	//! For internal use
	void SetFileName(core::String* str) { m_pFileName = str; }

	//! Returns the mesh file name
	core::String* GetFileName() { return m_pFileName; }

	//! Returns the mesh count for this node
	u32 GetMeshCount() { return m_iNumMeshes; }

	//! Gets the mesh
	sn::Mesh* GetMesh(u32 index) { return &m_pMeshes[index]; }

	//! Update final matrix
	virtual void UpdateFinalMat();

protected:

	sn::Mesh		*	m_pMeshes;			//! Meshes.
	
	core::String	*	m_pFileName;	//!*< Mesh file name

	u32					m_iNumMeshes;		//! The count of meshes.

	gfx::Material	*	m_pMaterials;		//! Mesh materials

	gfx::MeshBuffer	*	m_pMeshBuffer;

	// Create Mirror Plane
	virtual void CreateMirrorPlane(efx::Effect** ppCreatedMirror);

	// Create Shadow Plane
	virtual void CreateShadowPlane(efx::Effect** ppCreatedShadow);

}; // StaticMesh

} // sn

} // kge

#endif // STATICMESH_H