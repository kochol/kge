#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/math.h"
#include "../../engine/include/Logger.h"
#include "../../engine/include/libRocketKGERenderer.h"
#include "../../engine/include/libRocketKGESystem.h"
#include "../../engine/include/PluginManager.h"
#include <stdio.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#pragma comment(lib, "../../bin/debug/libRocketKGERenderer.lib")
#pragma comment(lib, "RocketCore.lib")
#pragma comment(lib, "RocketDebugger.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#pragma comment(lib, "../../bin/release/libRocketKGERenderer.lib")
#pragma comment(lib, "RocketCore.lib")
#endif
#endif

Rocket::Core::Context* context = NULL;

int main()
{
	// Init kge
	kge::Device dev;

	kge::InitParameters params;
	params.RendererName = "d3d9";
	params.InputMgrName = "OIS";
	params.AAMode = kge::gfx::EAAM_Off;
	params.Width = 1024;
	params.Height = 768;
	dev.Init(params);

	kge::gfx::Renderer*	pRen = dev.GetRenderer();
	if (!pRen)
		return -1;

	// Load DDS loader plugin
	kge::PluginManager::GetSingletonPtr()->LoadPlugin("Loader_DDS");

	// Init libRocket
	kge::libRocketKGERenderer kge_renderer;
	Rocket::Core::SetRenderInterface(&kge_renderer);

	kge::libRocketKGESystem system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		return -1;
	}
	system_interface.SetContext(context);

	// Loads the default fonts from the given path.
	Rocket::Core::String font_names[4];
	font_names[0] = "Delicious-Roman.otf";
	font_names[1] = "Delicious-Italic.otf";
	font_names[2] = "Delicious-Bold.otf";
	font_names[3] = "Delicious-BoldItalic.otf";
	Rocket::Core::String strDir = "D:/sdks/libRocket/Samples/assets/";

	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(strDir + font_names[i]);
	}

	Rocket::Debugger::Initialise(context);
	Rocket::Debugger::SetVisible(true);

	// Load and show the demo document.
	Rocket::Core::ElementDocument* document = context->LoadDocument("D:/sdks/libRocket/Samples/assets/demo.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}

	// Create matrices
	kge::math::Matrix mProj;
	mProj.SetOrthoOffscreenLH(0.0f, params.Width, params.Height, 0.0f, -1.0f, 1.0f);
	pRen->SetTransForm(&mProj, kge::gfx::ETM_Projection);
	pRen->SetTransForm(NULL, kge::gfx::ETM_View);
	pRen->SetTransForm(NULL);

	pRen->SetClearColor(kge::gfx::Color(100,0,100));

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		context->Update();
		context->Render();
		pRen->EndRendering();
	}

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	return 0;

} // main
