// File name: BumpMap.cpp
// Des: This class is for creating bumpmap effect.
// Date: 30/6/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../include/kge/efx/BumpMap.h"

namespace kge
{
namespace efx
{
	//-------------
	// Constructor.
	//-------------
	BumpMap::BumpMap(sn::StaticMesh* mesh, sn::SceneManager* smgr, 
		sn::Mesh* meshes, u32 NumMeshes):
			m_pMesh(mesh), m_pSnmgr(smgr), m_pMeshes(meshes), 
				m_iNumMeshes(NumMeshes)
	{
	} // Constructor

	//------------
	// Destructor.
	//------------
	BumpMap::~BumpMap()
	{
	} // Destructor

	//--------------------------------------------------
	// The works must to do before rendering the object.
	//--------------------------------------------------
	void BumpMap::PreRender(float elapsedTime)
	{
	} // PreRender

	//-------------------------------------------------
	// The works must to do after rendering the object.
	//-------------------------------------------------
	void BumpMap::Render()
	{
	} // Render

	//-------------------------------------------------------------
	// The works must to do after rendering all of the scene nodes.
	//-------------------------------------------------------------
	void BumpMap::PostRenderEverything()
	{
	} // PostRenderEverything

} // efx

} // kge