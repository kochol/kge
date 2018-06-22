// This tutorial shows how to use Kochol Game Engine
// We recommend if you want test KGE change the codes in this tutorial

// Include the kge.h and it will include the other header files automatically
// But its not the best way to use KGE when your project is big. The compile time
// will be increased in this way.
#include "../../include/kge/kge.h"

#include "../../include/kge/core/TaskManager.h"

// Tell the compiler to link with kge.lib
#pragma comment(lib, "../../bin/debug/kge.lib")

// Everything for KGE is in kge namespace
// The Device class is the main class that initialize the KGE and its submodules
kge::Device					dev;

// The classes for working with graphics card is in gfx namespace. Advanced users
// can use classes in this name space for custom rendering.
// The renderer class has many functions for sending commands to graphic card.
kge::gfx::Renderer		*	pRen;

// KGE has two layer the first layer is flexible and the second one is easy to use.
// The second layer uses first layer to make tasks easy for end user.
// The sn namespace contains everything that is related to scene and graphics rendering
// and it makes tasks easy for you.
// The main class in sn namespace is SceneManager that can manages the scene nodes in
// engine and know about the rendering path.
kge::sn::SceneManager	*	pSnMgr;

// We need a camera to view our world from it
kge::sn::Camera			*	pCam;

// Define a scene node for storing the mesh pointer
kge::sn::SceneNode		*	pMesh;

int main()
{
	// First we need to initialize the device before doing anything else
	// The InitParameters structure contains the parameters that we want to pass
	// them to KGE. We can accept default parameters for now
	kge::InitParameters	params;
	params.RendererType = kge::gfx::ERA_OpenGL;
	dev.Init(params);

	kge::core::TaskManager::GetSingletonPointer();

	// After initializing of engine we can get the pointer of Renderer and the SceneManager
	pRen	= dev.GetRenderer();
	pSnMgr	= dev.GetSceneManager();

	// Now we need a camera to view our world. We ask from SceneManger to add a camera node
	// to the scene
	pCam = pSnMgr->AddCameraNode
		(
		kge::math::Vector(5, 5, 5),	// The position of camera
		kge::math::Vector(0, 0, 0),		// The target of camera. The point that camera look at
		kge::math::Vector(0, 1, 0)	// The up vector, shows the up view direction
		);

	dev.GetPluginManager()->LoadPlugin("Loader_assimp");

	// Now we load a 3D model as static mesh
	pMesh = pSnMgr->AddStaticMeshNode("../../media/models/box.ms3d", true);

	if (pMesh)
	{
		// Disable lighting this part will be changed or explained later
		pMesh->GetMaterial(0)->shader->m_MaterialParams.eLightingType = kge::gfx::ELIT_UnLit;
		pMesh->GetMaterial(0)->shader->m_bMatParamsChanged = true;
//		pMesh->GetMaterial(0)->ppTexture[0] = pSnMgr->AddTexture("../../media/textures/duckcm.tga");
	}

	// We add a timer to get elapsed time
	kge::core::Timer t;

	pRen->SetClearColor(kge::gfx::Colorf(111,111,111));

	// The main loop that render the scene
	// The Device.Run function handles the engine device
	while (dev.Run())
	{
		// Begin the rendering and clear the color buffer
		pRen->BeginRendering(true, true, false);

		// Tell the scene manager to render everything and pass the time elapsed
		pSnMgr->RenderAll(t.GetTimeElapsed());

		// Show the scene on window
		pRen->EndRendering();
	}
}
