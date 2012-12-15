#include "../include/KgeMemory.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "../include/Renderer.h"
#include "RendererD3D11.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
	#pragma comment(lib, "../../bin/Debug/kge.lib")
#endif

namespace kge
{
	class RendererD3D11Plugin : public Plugin<gfx::Renderer>
	{
	public:

		//! Constructor
		RendererD3D11Plugin()
		{
			m_ePluginType = EPT_Renderer;
		}

		//! Destructor
		~RendererD3D11Plugin()
		{
		}

		//! Create the RendererD3D9 plugin
		gfx::Renderer* Create()
		{			
			return KGE_NEW(gfx::RendererD3D11)();
		}

	}; // RendererD3D9Plugin

} // kge

extern "C"
{
	void KGE_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::RendererD3D11Plugin *pPR = KGE_NEW(kge::RendererD3D11Plugin)();		
		kge::io::Logger::Info("D3D11 renderer plugin registered. version: %d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			PluginMgr.RegisterRenderer(pPR));
	}
}
