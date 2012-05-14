#include <string>
#include "../include/kgedef.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#include <Windows.h>

// #endif KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX

#include <dlfcn.h>

#endif

#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "../include/KgeMemory.h"

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	PluginManager::PluginManager()
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	PluginManager::~PluginManager()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Release all loaded plugin from memory
	//------------------------------------------------------------------------------------
	void PluginManager::Release()
	{
		for (size_t i = 0; i < m_vRendererPlugins.size(); i++)
		{
			KGE_DELETE(m_vRendererPlugins[i], RendererPlugin);
		}

	} // Release

	//------------------------------------------------------------------------------------
	// Loads a plugin from a .dll or .so file.
	//------------------------------------------------------------------------------------
	int PluginManager::LoadPlugin(const char* name)
	{
		// RegisterPlugin function pointer
		typedef void (*RegisterPlugin) (PluginManager &PM);

		bool bLoadFailed	= false;
		RegisterPlugin pFn	= NULL;


		// Load .dll files under windows OS
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		std::string strPluginName = name;
		strPluginName += ".dll";

		// Load dll and check for success
		HMODULE hMod = LoadLibrary(strPluginName.c_str());

		if (hMod)
		{
			pFn = (RegisterPlugin)GetProcAddress(hMod, "RegisterPlugin");
			if (!pFn)
			{
				io::Logger::Error("RegisterPlugin function dose not find in %s plugin.", strPluginName.c_str());
				return 0;
			}
		}
		else
		{
				io::Logger::Error("can't load the %s plugin. (file not found)", strPluginName.c_str());
				return 0;
		}

// endif KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX

		std::string strPluginName = "./lib";
		strPluginName += name;
		strPluginName += ".so";

        void* handle = dlopen(strPluginName.c_str(), RTLD_LAZY);

        char* error;

        if (!handle)
        {
				io::Logger::Error("can't load the %s plugin. (%s)", strPluginName.c_str(), dlerror());
				return 0;
        }
        // Clear any exiting error
        dlerror();

        // Load the RegisterPlugin function
        pFn = (RegisterPlugin)dlsym(handle, "RegisterPlugin");
        if ((error = dlerror()) != NULL || !pFn)
        {
            io::Logger::Error("RegisterPlugin function dose not find in %s plugin.\n%s", strPluginName.c_str(), error);
            return 0;
        }

#endif

		// Call the RegisterPlugin function
		(*pFn)(*this);
		io::Logger::SuccessKGE("Plugin %s loaded successfully.", strPluginName.c_str());

		return 1;

	} // LoadPlugin

	//------------------------------------------------------------------------------------
	// Returns the PluginManager singleton pointer
	//------------------------------------------------------------------------------------
	PluginManager*	PluginManager::GetSingletonPtr()
	{
		static PluginManager pm;// = NULL;
//		if (!pm)
	//		pm = KGE_NEW(PluginManager)();

		return &pm;

	} // GetSingletonPtr

	//------------------------------------------------------------------------------------
	// With this interface a Renderer Plugin will register himself with PluginManager.
	//------------------------------------------------------------------------------------
	int PluginManager::RegisterRenderer(RendererPlugin* pRenPlug)
	{
		m_vRendererPlugins.push_back(pRenPlug);

		return m_vRendererPlugins.size() - 1;

	} // RegisterRenderer

	//------------------------------------------------------------------------------------
	// With this interface an InputManager Plugin will register himself with PluginManager.
	//------------------------------------------------------------------------------------
	int PluginManager::RegisterInputManager(InputMgrPlugin* pInputMgrPlug)
	{
		m_vInputMgrPlugins.push_back(pInputMgrPlug);

		return m_vInputMgrPlugins.size() - 1;

	} // RegisterInputManager

} // kge
