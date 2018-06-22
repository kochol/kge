// This tutorial shows how to use keyboard inputs in Kochol Game Engine 
// We recommend if you want test KGE change the codes in this tutorial
// You have to read tut01 before this tutorial we will delete old comments in this tutorial

#include "../../include/kge/kge.h"

#pragma comment(lib, "../../bin/debug/kge.lib")

kge::Device					dev;

kge::gfx::Renderer		*	pRen;

kge::sn::SceneManager	*	pSnMgr;

kge::sn::Camera			*	pCam;

kge::sn::SceneNode		*	pMesh;

// The classes for Input/Output are in io namespace
// The Keyboard class is used for getting keys status
kge::io::Keyboard			key;

// A constant for camera speed
const float					fCamSpeed = 3.0f;

// In keys function we will find the pressed keys and move the camera in our world
void keys(float elaspedtime)
{
	// We can find the pressed key with KeyDown function in Keyboard class
	// If you want to know about a letter key is pressed you have to pass it as uppercase
	if (key.KeyDown('W'))
		// Move the camera forward
		pCam->MoveBF(fCamSpeed * elaspedtime);
	if (key.KeyDown('S'))
		// Move the camera backward by passing a negative number to MoveBF function
		pCam->MoveBF(-fCamSpeed * elaspedtime);
	if (key.KeyDown('D'))
		// Move the camera Right
		pCam->MoveRL(fCamSpeed * elaspedtime);
	if (key.KeyDown('A'))
		// Move the camera left by passing a negative number to MoveRL function
		pCam->MoveRL(-fCamSpeed * elaspedtime);

	// Now we want to enable FPS camera by F1 key and disable it with escape key
	if (key.KeyDown(kge::io::EK_F1))
		// It is simple
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
		kge::math::Vector(-75, 75, -75),	
		kge::math::Vector(0, 30, 0),		
		kge::math::Vector(0, 1, 0)	
		);

	// Load an Animated mesh is simple.
	pRen->SetTextureParams(kge::gfx::ETP_Linear);
	pMesh = pSnMgr->AddAnimatedMeshNode("../../media/models/dwarf/dwarf1.ms3d");
	for (int i = 0; i < pMesh->GetMaterialCount(); i++)
	{
		pMesh->GetMaterial(i)->shader->m_MaterialParams.eLightingType = kge::gfx::ELIT_UnLit;
		pMesh->GetMaterial(i)->shader->m_bMatParamsChanged = true;
	}

	pRen->SetClearColor(kge::gfx::Colorf(0.2,0.2,0.2,0.1));

	kge::core::Timer t;	

	while (dev.Run())
	{
		// Store the elapsed time between frames
		float fElaspedTime = t.GetTimeElapsed();

		// Check for the keys
		keys(fElaspedTime);

		pRen->BeginRendering(true, true, false);

		pSnMgr->RenderAll(fElaspedTime);

		pRen->EndRendering();
	}
}