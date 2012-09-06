#include "../include/KgeMemory.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "LoaderDDS.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib, "../../bin/Debug/kge.lib")
#endif

namespace kge
{
	class LoaderDDSPlugin : public Plugin<Loader>
	{
	public:

		//! Constructor
		LoaderDDSPlugin()
		{
			m_ePluginType = EPT_Loader;
		}

		//! Destructor
		~LoaderDDSPlugin()
		{
		}

		//! Create the RendererD3D9 plugin
		Loader* Create()
		{			
			return KGE_NEW(LoaderDDS)();
		}

	}; // RendererD3D9Plugin

} // kge

extern "C"
{
	void KGE_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::LoaderDDSPlugin *pPR = KGE_NEW(kge::LoaderDDSPlugin)();		
		kge::io::Logger::Info("DDS texture loader plugin registered. version: %d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			PluginMgr.RegisterLoader(pPR));
	}
}
