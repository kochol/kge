// This tutorial shows how to use terrain in Kochol Game Engine 
// This tutorial shows the basic usage of tile terrain scene node
// You have to read tut01 and 02input before this tutorial we will delete old comments in this tutorial

#include "../../Include/kge.h"

#pragma comment(lib, "../../bin/debug/kge.lib")

kge::Device					dev;

kge::gfx::Renderer		*	pRen;

kge::sn::SceneManager	*	pSnMgr;

kge::sn::Camera			*	pCam;

// Use TileTerrain2 scene node to add a terrain
kge::sn::TileTerrain2	*	pTer;

kge::io::Keyboard			key;

const float					fCamSpeed = 3.0f;

void keys(float elaspedtime)
{
	if (key.KeyDown('W'))
		pCam->MoveBF(fCamSpeed * elaspedtime);
	if (key.KeyDown('S'))
		pCam->MoveBF(-fCamSpeed * elaspedtime);
	if (key.KeyDown('D'))
		pCam->MoveRL(fCamSpeed * elaspedtime);
	if (key.KeyDown('A'))
		pCam->MoveRL(-fCamSpeed * elaspedtime);

	if (key.KeyDown(kge::io::EK_F1))
		pCam->AutoRotateByMouse(true);
	if (key.KeyDown(kge::io::EK_ESCAPE))
		pCam->AutoRotateByMouse(false);
}

int main()
{
	kge::InitParameters	params;
	params.RendererType = kge::gfx::ERA_OpenGL;
	dev.Init(params);

	pRen	= dev.GetRenderer();
	pSnMgr	= dev.GetSceneManager();

	pCam = pSnMgr->AddCameraNode
		(
		kge::math::Vector(5, 5, 5),	
		kge::math::Vector(0, 0, 0),		
		kge::math::Vector(0, 1, 0)	
		);

	//------------------------------------------------------------------------------------
	// Now lets create the terrain
	//------------------------------------------------------------------------------------
	pTer = KGE_NEW(kge::sn::TileTerrain2);

	// Load the texture for terrain
	pTer->GetMaterial(0)->ppTexture[0] = pSnMgr->AddTexture("../../media/textures/t1.dds");
	// Set the tile texure parameters
	pTer->SetTileTexture
		( 
		pTer->GetMaterial(0)->ppTexture[0],		// Tile texture
		2048,									// Texture width
		2048,									// texture height
		4,										// Horizontal tiles count
		4,										// Vertical tiles count
		512										// Texture size of each tile
		);

	// Set the terrain scene manager
	pTer->SetSceneManager(pSnMgr);
	pSnMgr->AddSceneNode(pTer, kge::sn::ENT_TileTerrain);

	// Create the terrain triangles and shaders
	// The terrain size must be a number divided by 16
	// for example 16, 32, 48, 64, 80, 96, 112, 128, ...
	pTer->ReCreate
		(
		112,	// Width
		112,	// Height
		0.0,	// Minimum of Random height 
		0.2		// Maximum of Random height 
		);

	// For now you have to add at least one directional light to your scene to make terrain work.
	pSnMgr->AddDirectionalLight(kge::math::Vector(1,1,1), kge::gfx::Colorf(255,255,255));

	pRen->SetClearColor(kge::gfx::Colorf(111,111,111));

	kge::core::Timer t;	

	while (dev.Run())
	{
		float fElaspedTime = t.GetTimeElapsed();

		keys(fElaspedTime);

		pRen->BeginRendering(true, true, false);

		pSnMgr->RenderAll(fElaspedTime);

		pRen->EndRendering();
	}
}
