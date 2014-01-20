#include "../../Include/core/KgeMemory.h"
#include "../../Include/PluginManager.h"
#include "../../Include/io/Logger.h"
#include "LoaderAssimp.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib, "../../bin/Debug/kge.lib")
#pragma comment(lib, "assimp.lib")
#endif

namespace kge
{
	class LoaderAssimpPlugin : public Plugin<Loader>
	{
	public:

		//! Constructor
		LoaderAssimpPlugin()
		{
			m_ePluginType = EPT_MeshLoader;
		}

		//! Destructor
		~LoaderAssimpPlugin()
		{
		}

		//! Create the RendererD3D9 plugin
		Loader* Create()
		{			
			return KGE_NEW(LoaderAssimp)();
		}

	}; // LoaderAssimpPlugin

} // kge

extern "C"
{
	void KGELIB_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::LoaderAssimpPlugin *pPR = KGE_NEW(kge::LoaderAssimpPlugin)();		
		kge::io::Logger::Info("ASSIMP loader plugin registered. version: %d.%d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			pPR->GetEnginePatchVersion(),
			PluginMgr.RegisterLoader(pPR));
	}
}
