// File name: TileTerrainShapeX.cpp
// Des: This class is for creating physics for terrain.
// Date: 8/7/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Headers/ph/TileTerrainShapeX.h"
#include "../../Headers/ph/cookingX.h"
#include "../../Headers/ph/ErrorStreamX.h"
#include "../../Headers/ph/UserAllocatorX.h"
#include "../../Headers/ph/StreamX.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
namespace ph
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TileTerrainShapeX::TileTerrainShapeX( PhysXManager *pMan , sn::TileTerrain* terrain )
	{
		gPhAllocator = new UserAllocator();

		NxU32* ii = terrain->GetIndices();
		NxTriangleMeshDesc terrainDesc;
		terrainDesc.numVertices					= terrain->GetVertexCount();
		terrainDesc.numTriangles				= terrain->GetIndexesCount() / 3;
		terrainDesc.pointStrideBytes			= sizeof(gfx::Vertex3);
		terrainDesc.triangleStrideBytes			= 3*sizeof(NxU32);
		terrainDesc.points						= terrain->GetPositions();
		terrainDesc.triangles					= ii;
		terrainDesc.flags						= 0;

		terrainDesc.heightFieldVerticalAxis		= NX_Y;
		terrainDesc.heightFieldVerticalExtent	= -1000.0f;


		bool status = InitCooking(/*gPhAllocator, &gPhErrorStream*/);
		if (!status) 
			kge::io::Logger::Log("Unable to initialize the cooking library. Please make sure that you have correctly installed the latest version of the AGEIA PhysX SDK.");

		MemoryWriteBuffer buf;
		status = CookTriangleMesh(terrainDesc, buf);
		if (!status)
			printf("Unable to cook a triangle mesh.");

		MemoryReadBuffer readBuffer(buf.data);
		terrainMesh = pMan->getPhysxSDK()->createTriangleMesh(readBuffer);

		NxTriangleMeshShapeDesc terrainShapeDesc;
		terrainShapeDesc.meshData				= terrainMesh;
		terrainShapeDesc.shapeFlags				= NX_SF_FEATURE_INDICES;

		NxActorDesc ActorDesc;
		ActorDesc.shapes.pushBack(&terrainShapeDesc);
		gLocalActor = pMan->getPhysxScene()->createActor(ActorDesc);
		gLocalActor->userData = (void*)0;

		CloseCooking();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TileTerrainShapeX::~TileTerrainShapeX()
	{

	} // Destructor

} // ph

} // kge

#endif // KGE_USE_PHYSX
