/*! \page tutorials Tutorials

- \subpage tut01
- \subpage tut02
- \subpage tut05

*/

//------------------------------------------------------------------------------------
// tut01 "Hello Kochol Game Engine"
//------------------------------------------------------------------------------------

/*!
\page tut01 Hello Kochol Game Engine

\tableofcontents

\section intro Introduction
This tutorial shows you how to setup visual studio for working with KGE then we
will write a program that initialize the engine and adds a simple box to scene.

\section setup Setup your compiler
This section will explains how to setup your compiler for working with KGE
Currently we use Microsoft Visual Studio and I will explain how to configure it ,
but you will also be able to understand everything about compiler setup if you are using another compiler.

For start we need  some Requirements:
Latest Version of KGE pre built. <A class="el" HREF="http://kge3d.org/downloads" >Download the latest SDK</A>

\subsection step1 Step 1: Create a new project
Open Microsoft Visual Studio on your computer and create a new project of win32 Console Application.
Name the project and select its location then click OK.

\image html images/newprojectwizard.jpg

In the appeared Window select Application settings, 
in the additional options check the Empty project check box.
Click on Finish button.

\image html images/appset.jpg

\subsection step2 Step 2: Add header files search path
You have to include the header folder to search path, in order to use the engine.
The header files can be found in the KGE SDK directory "Include" and "Libs".

Let's explain shortly how to specify  this file  in Microsoft Visual Studio:
( maybe different for every IDE and compiler you use )

\subsubsection vs2010 Microsoft Visual Studio 2010
After creating your project goto View menu then select property manager in the opening window select your project->
Debug | Win32 Right click on Microsoft.Cpp.Win32.user and select properties. 
On new opend window select VC++ directories.
In Include directories add KGE SDK "Include" and "Libs" folders path.
In Library directories add KGE SDK "bin/debug" and "libs" folders path.

\image html images/prm2010.jpg

\subsubsection vs2005 Microsoft Visual Studio 2005-2008
Select Tools menu and select Options.
Select the projects entry and then select . Select ‘show directories for include files’ in the combo box , and add “Include” Directories of KGE engine.
Compiler also need to find the library files of engine,so stay in that dialog , select ‘show directories for Library files’ and add the libs directory of engine and also add the “Debug” directory.

Click Button.

\subsubsection vs6 Microsoft Visual Studio 6.0
if you use Microsoft Visual Studio 6.0,Select Menu and select ,
In the Option Window Select Tab and select the ‘Include’ Item in the combo box.
Now Add the "Include" and "Libs" Directory path of the KGE engine folder to the list of directories.
Now the compiler will find the “kge.h” header file.
Compiler also need to find the library files of engine, 
So select the ‘Library’ item in the combo box and add the “Libs” directory of the engine 
and also add the “Debug” directory of engine to find “kge.lib”

That’s it. With your IDE set up like this, you will now be able to develop applications with the Kochol Game Engine.

\subsection step3 Step 3: Lets start!

Just write this program and compile it to make sure that you do all thing right.

\code{.cpp}
#include <kge.h>
#pragma comment(lib, "kge.lib")

kge::Device*    pDev;

int main()
{
	pDev = KGE_NEW(kge::Device)();

	return 0;
}
\endcode

\section hellokge Hello KGE

This tutorial shows how to use Kochol Game Engine 

We recommend if you want test KGE change the codes in this tutorial

\code{.cpp}
// Include the kge.h and it will include the other header files automatically
// But its not the best way to use KGE when your project is big. The compile time 
// will be increased in this way.
#include "../../Include/kge.h"

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
	dev.Init(params);

	// After initializing of engine we can get the pointer of Renderer and the SceneManager
	pRen	= dev.GetRenderer();
	pSnMgr	= dev.GetSceneManager();

	// Now we need a camera to view our world. We ask from SceneManger to add a camera node 
	// to the scene
	pCam = pSnMgr->AddCameraNode
			(
				kge::math::Vector(5, 5, 5),	// The position of camera
				kge::math::Vector(),		// The target of camera. The point that camera look at
				kge::math::Vector(0, 1, 0)	// The up vector, shows the up view direction
			);

	// Now we load a 3D model as static mesh
	pMesh = pSnMgr->AddStaticMeshNode("../../media/models/box.ms3d", true);

	// Disable lighting this part will be changed or explained later
	pMesh->GetMaterial(0)->shader->m_MaterialParams.eLightingType = kge::gfx::ELIT_UnLit;
	pMesh->GetMaterial(0)->shader->m_bMatParamsChanged = true;

	// We add a timer to get elapsed time
	kge::core::Timer t;	

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
\endcode

*/

//------------------------------------------------------------------------------------
// tut02 "02 Camera Movement"
//------------------------------------------------------------------------------------

/*!
\page tut02 Camera Movement

\tableofcontents

\section intro2 Introduction
This tutorial show how to listen to keyboard keys and move your camera.
You have to read
\ref tut01 "Hello Kochol Game Engine" 
tutorial before this tutorial

\section code2 The code for moving the camera
\code{.cpp}
// This tutorial shows how to use keyboard inputs in Kochol Game Engine 
// We recommend if you want test KGE change the codes in this tutorial
// You have to read tut01 before this tutorial we will delete old comments in this tutorial

#include "../../Include/kge.h"

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

	for (int j =0; j < 25; j++)
	{
		for (int i = 0; i < 25; i++)
		{
			pMesh = pSnMgr->AddStaticMeshNode("../../media/models/hex.ms3d", true);
			if (i % 2 == 0)
			pMesh->SetPosition(kge::math::Vector(i * 1.5, 0.0, j * -1.74));
			else
			pMesh->SetPosition(kge::math::Vector(i * 1.5, 0.0, j * -1.74 + 0.87));

			pMesh->GetMaterial(0)->shader->m_MaterialParams.eLightingType = kge::gfx::ELIT_UnLit;
			pMesh->GetMaterial(0)->shader->m_bMatParamsChanged = true;
		}
	}

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
\endcode
*/

//------------------------------------------------------------------------------------
// tut05 "Terrain"
//------------------------------------------------------------------------------------

/*!
\page tut05 Terrain

\tableofcontents

\section intro5 Introduction

This tutorial shows how to use terrain in Kochol Game Engine and
shows the basic usage of tile terrain scene node
You have to read 
\ref tut01 "Hello Kochol Game Engine" 
and 
\ref tut02 "Camera movement"
tutorials before this tutorial.

We will delete old comments in this tutorial.

\section code5 The minimal code for terrain rendering

\code{.cpp}

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

\endcode
*/