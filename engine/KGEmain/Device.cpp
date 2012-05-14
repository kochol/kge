#include "../include/KgeMemory.h"
#include "../include/Device.h"
#include "../include/KgeWindow.h"
#include "WinWindow.h"
#include "../include/Renderer.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "KgeMemoryTrack.h"
#include "../include/InputManager.h"

kge::gfx::Renderer*	g_pRenderer = NULL;

namespace kge
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Device::Device(): m_pWindow(NULL), m_pInputMgr(NULL)
	{
		m_pPluginMgr = PluginManager::GetSingletonPtr();
		m_pLogger	 = KGE_NEW(io::Logger)();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Device::~Device()
	{
		KGE_DELETE(m_pWindow, KgeWindow);
		KGE_DELETE(g_pRenderer, Renderer);
		m_pPluginMgr->Release();

		// Delete the logger in last
		KGE_DELETE(m_pLogger, Logger);

		// Writing the memory leaks
		io::Logger log;
		core::KgeMemoryTrack::LogMemoryLeaks();

	} // Destructor

	//------------------------------------------------------------------------------------
	// Returns the Device object pointer.
	//------------------------------------------------------------------------------------
	Device* Device::GetSingletonPtr()
	{
		static Device me;
		return &me;

	} // GetSingletonPtr

	//------------------------------------------------------------------------------------
	// Initialize KGE with given parameters
	//------------------------------------------------------------------------------------
    bool Device::Init(InitParameters& params)
    {
		// Creating the rendering window
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

		if (!params.hwnd)
			m_pWindow = KGE_NEW(io::WinWindow)();
#endif

		if (m_pWindow)
			if (!m_pWindow->Init(params))
				return false;

		// Creating the Renderers
		if (g_pRenderer)
		{
			return false;
		}

		params.RendererName.insert(0, "Renderer_");
		if (!m_pPluginMgr->LoadPlugin(params.RendererName.c_str()))
			return false;

		g_pRenderer = m_pPluginMgr->GetRendererPlugin(0)->Create();
		g_pRenderer->Init(params);

		// Creating the InputManager
		if (params.InputMgrName.size() > 0)
		{
			params.InputMgrName.insert(0, "InputMgr_");
			if (m_pPluginMgr->LoadPlugin(params.InputMgrName.c_str()))
			{
				m_pInputMgr = m_pPluginMgr->GetInputManagerPlugin(0)->Create();
				m_pInputMgr->Initialise(params.hwnd);
			}
		}

		return true;

    } // Init

	//------------------------------------------------------------------------------------
	// Run the KGE
	//------------------------------------------------------------------------------------
	bool Device::Run()
	{
		bool b = true;
		if (m_pWindow)
			b = m_pWindow->Run();

		if (m_pInputMgr)
			m_pInputMgr->Capture();

		return b;

	} // Run

	//! Get the renderer
	gfx::Renderer* Device::GetRenderer()
	{
		return g_pRenderer;

	} // GetRenderer

} // kge
