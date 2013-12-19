// File name: TileTerrainShapeX.h
// Des: This class is for creating physics for terrain.
// Date: 8/7/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "PhysXManager.h"
#include "UserAllocatorX.h"
#include "ErrorStreamX.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_TILETERRAINSHAPEX_H
#define KGE_TILETERRAINSHAPEX_H

namespace kge
{
namespace ph
{
class TileTerrainShapeX: public TileTerrainShape
{
public:

	//! Constructor
	TileTerrainShapeX(PhysXManager *pMan ,sn::TileTerrain* terrain);

	//! Destructor
	~TileTerrainShapeX();

protected:

	sn::TileTerrain*	m_pTerrain;
	ErrorStream			gPhErrorStream;
	UserAllocator*		gPhAllocator; 
	NxTriangleMesh*		terrainMesh;
	NxActor*			gLocalActor;

}; // TileTerrainShapeX

} // ph

} // kge

#endif // KGE_TILETERRAINSHAPEX_H

#endif // KGE_USE_PHYSX
