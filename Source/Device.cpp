// File name: Device.cpp
// Date: 10/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include <iostream>
#include <time.h>
#include "../Include/kgedef.h"

#include "../Include/Device.h"
#include "../Include/PluginManager.h"
#include "../Include/gfx/Renderer.h"
#include "../Include/gfx/MaterialManager.h"
//#include "../Include/gui/GuiManager.h"
#include "../Include/core/mem_fun.h"
#include "../Include/efx/EffectManager.h"
#include "../Headers/gfx/LoaderDDS.h"
#include "../Headers/gfx/LoaderStbImage.h"
#include "../Include/io/FileSystemManager.h"
#include "../Include/io/FileSystem.h"
#include "../Include/gui/GuiManager.h"

// sound system
#include "../Headers/av/fmod/FmodSoundSystem.h"
#include "../Headers/av/bass/BassSoundSystem.h"

#ifdef WIN32
#include "../Headers/io/win32.h"
#endif // WIN32

#include "../Headers/io/Linux.h"
#include "../Headers/io/SdlWindow.h"
#include "../Include/io/Logger.h"

#include "../Headers/ph/PhysXManager.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "physfs_d.lib")
#else
#pragma comment(lib, "physfs.lib")
#endif

KGE_API kge::u32 HardwareBufferID	= 0;
KGE_API kge::u32 VertexDecID		= 0;
KGE_API kge::u32 VertexShaderSet	= MAXID;
KGE_API kge::u32 PixelShaderSet		= MAXID;
KGE_API kge::Device				*	g_pDev			= NULL;
KGE_API kge::gfx::Renderer		*	g_pRenderer		= NULL;
KGE_API kge::efx::EffectManager	*	g_pEffectManager= NULL;

namespace kge
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Device::Device(): m_pPhysManager(NULL), m_pFontManager(NULL),
		m_pEffectManager(NULL), m_pSoundSystem(0),
		m_pSnMan(0), m_pRenderer(0), m_pWindow(0), m_pTextureManager(0),
		m_pVertexShaderManager(0), m_pPixelShaderManager(0), m_pImageMan(0),
		m_pGuimgr(0)
	{
		srand( (unsigned int)time( NULL ) );
		g_pDev = this;
		m_pPluginmgr = PluginManager::GetSingletonPtr();

	} // Device()

	// ******* *******
	// Takhrib konande
	// ******* *******
	Device::~Device()
	{
		KGE_DELETE(m_pSnMan, SceneManager);
		KGE_DELETE(m_pRenderer, Renderer);
		KGE_DELETE(m_pWindow, IWindow);
		KGE_DELETE(m_pEffectManager, EffectManager);
		if (m_pImageMan)
		{
			delete m_pImageMan;
			m_pImageMan = NULL;
		}
		KGE_DELETE(m_pGuimgr, GuiManager);
		gfx::MaterialManager* m = gfx::MaterialManager::GetSingletonPtr();
		KGE_DELETE(m, MaterialManager);

		KGE_DELETE(m_pSoundSystem, SoundSystem);		// deleting sound system

		// Delete resource managers
		if (m_pFontManager)
		{
			delete m_pFontManager;
			m_pFontManager = NULL;
		}
		if (m_pVertexShaderManager)
		{
			delete m_pVertexShaderManager;
			m_pVertexShaderManager = NULL;
		}
		if (m_pPixelShaderManager)
		{
			delete m_pPixelShaderManager;
			m_pPixelShaderManager = NULL;
		}
		if (m_pTextureManager)
		{
			delete m_pTextureManager;
			m_pTextureManager = NULL;
		}

		m_pPluginmgr->Release();

#ifdef KGE_TRACK_MEMORY
		core::KgeMemoryTrack::LogMemoryLeaks();
#endif

		g_pDev = NULL;

	} // ~Device()

	//------------------------------------------------------------------------------------
	// Returns the Device class pointer.
	//------------------------------------------------------------------------------------
	Device* Device::GetSingletonPtr()
	{ 		
		return g_pDev;

	} // GetSingletonPtr

	//------------------------------------------------------------------------------------
	// Initialize the engine
	//------------------------------------------------------------------------------------
	bool Device::Init(InitParameters &params)
	{
		// Add file system
		io::FileSystem* pFileSys = KGE_NEW(io::FileSystem);
		io::FileSystemManager::getSingletonPtr()->RegisterFileSystem(pFileSys);

		if (params.hwnd == NULL)
		{
#ifdef KGE_USE_SDL
		m_pWindow = KGE_NEW(io::SdlWindow)();
#else
	#ifdef WIN32
		m_pWindow = KGE_NEW(io::Win32)();
	#else // LINUX
		m_pWindow = KGE_NEW(io::Linux)();
	#endif // WIN32
#endif // KGE_USE_SDL
		}

		// Auto API Select
#ifdef LINUX
		ra = gfx::ERA_OpenGL;
#endif // LINUX

		// Welcome TEXT
		io::Logger::Log(io::ELM_Information, "This software is using %s.", KGE_VERSION_NAME);

		m_Params = params;

		// Select Renderer
		core::String RendererName;
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		if (params.RendererType == gfx::ERA_DirectX9)
		{
			RendererName = "Renderer_d3d9";
		}
		else if (params.RendererType == gfx::ERA_OpenGL)
		{
			RendererName = "Renderer_ogl";
		}

#elif (KGE_PLATFORM == KGE_PLATFORM_LINUX ) || (KGE_PLATFORM == KGE_PLATFORM_APPLE)
		RendererName = "Renderer_ogl";

#endif 

		// Load the renderer plugin
		if (!m_pPluginmgr->LoadPlugin(RendererName.ToCharPointer()))
			return false;

		m_pRenderer = m_pPluginmgr->GetRendererPlugin(0)->Create();
		
		// Init the Window
		if (params.hwnd == NULL)
		{
			m_pWindow->Init(params.Width, params.Height, params.Bits, params.FullScreen, params.RendererType);
			params.hwnd		= m_pWindow->GetHandler();
			m_Params.hwnd	= m_pWindow->GetHandler();
		}

		// Init the Renderer
		if(!m_pRenderer->Init(this, params))
		{
			//io::Logger::Log("Trying another renderer API.");
			//if (!SwapRenderer())
			{
				io::Logger::Log("Initializing renderer API failed.");
				return false;
			}
		} // if (!Renderer::Init)

		m_pRenderer->SetPerspectiveLH();

		m_pGuimgr = KGE_NEW(gui::GuiManager)(m_pRenderer, m_pSnMan);

		//m_pPluginmgr = new PluginManager(this);

		// initializing sound system
#if KGE_USE_SOUND_SYSTEM == 1

#if KGE_USE_FMOD == 1
		m_pSoundSystem = KGE_NEW(av::FmodSoundSystem)(1000.0f, 1000.0f);
#elif KGE_USE_BASS == 1
		m_pSoundSystem= KGE_NEW(av::BassSoundSystem)(1000.0f, 1000.0f);
#elif KGE_USE_OPENAL == 1
		//m_pSoundSystem == new  TODO:
#endif 

		if (!m_pSoundSystem->Init(params.hwnd))
		{
			io::Logger::Log("Initializing sound system failed.");
			return false;
		}

#endif // KGE_USE_SOUND_SYSTEM

		// Create the resource managers
		m_pImageMan = new ResourceManager<gfx::Image>(NULL);

		m_pTextureManager = new ResourceManager<gfx::Texture>((core::Functor5<bool , Resource** , const u32 , const char* , const char* , 
			void* >*)core::mem_fun5(gfx::Renderer::GetSingletonPtr(), &gfx::Renderer::AddTexture));
		LoaderDDS* pDDSLoader = KGE_NEW(LoaderDDS);
		m_pTextureManager->RegisterLoader(pDDSLoader);
		LoaderStbImage* pLoaderStbImage = KGE_NEW(LoaderStbImage);
		m_pTextureManager->RegisterLoader(pLoaderStbImage);

		//m_pShaderManager = new ResourceManager<gfx::Shader>(NULL);

		m_pVertexShaderManager = new ResourceManager<gfx::Shader>((core::Functor5<bool, Resource**, const u32,
			const char*, const char*, void*>*)core::mem_fun5(gfx::Renderer::GetSingletonPtr(), &gfx::Renderer::AddVertexShader));

		m_pPixelShaderManager = new ResourceManager<gfx::Shader>((core::Functor5<bool, Resource**, const u32,
			const char*, const char*, void*>*)core::mem_fun5(gfx::Renderer::GetSingletonPtr(), &gfx::Renderer::AddPixelShader));

		m_pFontManager	 = new ResourceManager<gui::Font>
			((core::Functor5<bool , Resource** , const u32 , const char* , const char* , 
			void* >*)core::mem_fun5(m_pGuimgr, &gui::GuiManager::AddFont));

		// Create effect manager
		m_pEffectManager = KGE_NEW(efx::EffectManager)();

		// Create Material Manager
		gfx::MaterialManager* mmgr = KGE_NEW(gfx::MaterialManager)();

		m_pSnMan = KGE_NEW(sn::SceneManager)(m_pRenderer);

		m_pRenderer->SetMainSceneManager(m_pSnMan);

		return true;

	} // Init

	// *** *** ***
	// Run the KGE
	// *** *** ***
	bool Device::Run()
	{
		return m_pWindow->Run();
	} // Run

	// *** *** ******** **** ******
	// Get the Renderer from Device
	// *** *** ******** **** ******
	gfx::Renderer* Device::GetRenderer()
	{
		return m_pRenderer;
	} // GetRenderer()

	//------------------------------
	// returns the rendering window.
	//------------------------------
	io::IWindow* Device::GetWindow()
	{
		return m_pWindow;

	} // GetWindow

	//------------------------------------------------------------------------------------
	// Returns the requested physics engine manager.
	//------------------------------------------------------------------------------------
	ph::PhysManager* Device::GetPhysManager( ph::PhysicsEngineType enumPhysType )
	{
		if (m_pPhysManager)
			return m_pPhysManager;

		switch (enumPhysType)
		{
		case ph::EPET_PhysX:
#ifdef KGE_USE_PHYSX
			m_pPhysManager = new ph::PhysXManager();
			m_pPhysManager->Init();
#else
			io::Logger::Log(io::ELM_Warning, "This version dose not compiled with PhysX.");
#endif
			break;
		default:
			io::Logger::Log(io::ELM_Warning, "The requested physics engine not found.");
			break;
		}

		return m_pPhysManager;

	} // GetPhysManager

	io::IWindow* Device::CreateNewWindow()
	{
		// TODO check that can we use SDL to create this function
#ifdef WIN32
		io::IWindow* w = KGE_NEW(io::Win32)();
		return w;
#endif // WIN32

		return NULL;
	}

} // kge
