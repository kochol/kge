// File name: BumpMap.h
// Des: This class is for creating bumpmap effect.
// Date: 30/6/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef BUMPMAP_H
#define BUMPMAP_H

#include "Effect.h"
#include "../sn/StaticMesh.h"

namespace kge
{
namespace efx
{
class KGE_API BumpMap : public Effect
{
public:

	//! Constructor
	BumpMap(sn::StaticMesh* mesh, sn::SceneManager* smgr,
			sn::Mesh* meshes, u32 NumMeshes);
	
	//! Destructor
	virtual ~BumpMap();

	//! Nothing to do.
	virtual void AddNodes( sn::SceneNode* pNodes ) { /* Nothing to do */ }

	//! The works must to do before rendering the object.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! The works must to do after rendering the object.
	virtual void Render();

	//! The works must to do after rendering all of the scene nodes.
	virtual void PostRenderEverything();

protected:
	sn::StaticMesh  * m_pMesh;
	sn::Mesh			 * m_pMeshes;
	u32					   m_iNumMeshes;
	sn::SceneManager* m_pSnmgr;

}; // BumpMap

} // efx

} // kge

#endif // BUMPMAP_H