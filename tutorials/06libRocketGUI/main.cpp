#include "../../Include/Device.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/math/math.h"
#include "../../Include/io/Logger.h"
#include "../../Include/gui/libRocketKGERenderer.h"
#include "../../Include/gui/libRocketKGESystem.h"
#include "../../Include/gui/EventManager.h"
#include "../../Include/gui/EventInstancer.h"
#include "../../Include/gui/EventHandler.h"
#include "../../Include/PluginManager.h"
#include <stdio.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include <Rocket/Controls.h>

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#pragma comment(lib, "../../bin/debug/libRocketKGERenderer.lib")
#pragma comment(lib, "RocketCore.lib")
#pragma comment(lib, "RocketControls.lib")
#pragma comment(lib, "RocketDebugger.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#pragma comment(lib, "../../bin/release/libRocketKGERenderer.lib")
#pragma comment(lib, "RocketCore.lib")
#endif
#endif

Rocket::Core::Context* context = NULL;

// start button handler
class EventHandlerDemo : public EventHandler
{
public:

	//! Constructor
	EventHandlerDemo()
	{

	}

	//! Destructor
	~EventHandlerDemo()
	{

	}

	void ProcessEvent(Rocket::Core::Event& event, const Rocket::Core::String& value)
	{
		if (value == "start")
		{
			kge::io::Logger::Debug("Start button pressed.");
		}
	}

protected:

	
}; // EventHandlerDemo

int main()
{
	// Init kge
	kge::Device dev;

	kge::InitParameters params;
	params.RendererType = kge::gfx::ERA_OpenGL;
	params.InputMgrName = "OIS";
	params.AAMode = kge::gfx::EAAM_Off;
// 	params.Width = 1024;
// 	params.Height = 768;
	dev.Init(params);

	kge::gfx::Renderer*	pRen = dev.GetRenderer();
	if (!pRen)
		return -1;

	// Init libRocket
	kge::libRocketKGERenderer kge_renderer;
	Rocket::Core::SetRenderInterface(&kge_renderer);

	kge::libRocketKGESystem system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(params.Width, params.Height));
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
	Rocket::Core::String strDir = "../../media/gui/";

	for (int i = 0; i < sizeof(font_names) / sizeof(Rocket::Core::String); i++)
	{
		Rocket::Core::FontDatabase::LoadFontFace(strDir + font_names[i]);
	}

	Rocket::Debugger::Initialise(context);
	Rocket::Debugger::SetVisible(true);

	// Initialise the event instancer and handlers.
	EventInstancer* event_instancer = new EventInstancer();
	Rocket::Core::Factory::RegisterEventListenerInstancer(event_instancer);
	event_instancer->RemoveReference();

	EventManager::RegisterEventHandler("demo", new EventHandlerDemo());

	// Start the game.
	EventManager::m_DirStr = "../../media/gui/";
	EventManager::LoadWindow("demo");

	pRen->SetClearColor(kge::gfx::Colorf(100, 100, 100));


	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		system_interface.Render();
		pRen->EndRendering();
	}

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	return 0;

} // main

