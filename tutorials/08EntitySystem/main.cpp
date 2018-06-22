// This tutorial shows how to use keyboard inputs in Kochol Game Engine 
// We recommend if you want test KGE change the codes in this tutorial
// You have to read tut01 before this tutorial we will delete old comments in this tutorial

#include "../../include/kge/kge.h"

// Entity System
#include "../../include/kge/en/Entity.h"
#include "../../include/kge/en/CmpPosition.h"
#include "../../include/kge/en/WorldManager.h"
#include "../../include/kge/en/World.h"
#include "../../include/kge/en/EntityManager.h"
#include "../../include/kge/en/CmpMesh.h"
#include "../../include/kge/en/SystemMesh.h"

#pragma comment(lib, "../../bin/debug/EntitySystem.lib")
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
	dev.Init(params);

	pRen	= dev.GetRenderer();
	pSnMgr	= dev.GetSceneManager();

	pCam = pSnMgr->AddCameraNode
		(
		kge::math::Vector(5, 5, 5),	
		kge::math::Vector(),		
		kge::math::Vector(0, 1, 0)	
		);

	// Create a world
	kge::en::WorldManager wm;
	kge::en::World* pW = wm.CreateWorld();

	// Add systems to our world
	kge::en::SystemMesh sysMesh;
	pW->AddSystem(&sysMesh);

	// Create an Entity
	kge::en::Entity* pE = wm.GetEntityManager()->CreateEntity();
	kge::en::CmpPosition c(0, 0, 0);
	kge::en::CmpMesh cm("../../media/models/hex.ms3d");
	printf("%d\n%d\n", c.GetClassID(), cm.GetClassID());
	pE->AddComponent(&c);
	pE->AddComponent(&cm);
	pW->AddEntity(pE);

	kge::core::Timer t;	

	while (dev.Run())
	{
		// Store the elapsed time between frames
		float fElaspedTime = t.GetTimeElapsed();

		// Check for the keys
		keys(fElaspedTime);

		if (key.KeyDown('Z'))
			c.x -= fCamSpeed * fElaspedTime;
		if (key.KeyDown('X'))
			c.x += fCamSpeed * fElaspedTime;
		if (key.KeyDown('C'))
			c.z -= fCamSpeed * fElaspedTime;
		if (key.KeyDown('V'))
			c.z += fCamSpeed * fElaspedTime;

		pW->Update(fElaspedTime);

		pRen->BeginRendering(true, true, false);

		pSnMgr->RenderAll(fElaspedTime);

		pRen->EndRendering();
	}

}