#include "../../Include/core/KgeMemory.h"
#include "../../Include/PluginManager.h"
#include "../../Include/io/Logger.h"
#include "LoaderAssimp.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib, "../../bin/Debug/kge.lib")
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
	void KGE_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::LoaderAssimpPlugin *pPR = KGE_NEW(kge::LoaderAssimpPlugin)();		
		kge::io::Logger::Info("D3D9 renderer plugin registered. version: %d.%d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			pPR->GetEnginePatchVersion(),
			PluginMgr.RegisterLoader(pPR));
	}
}
