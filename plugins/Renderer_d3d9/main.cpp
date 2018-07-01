#include "../../include/kge/core/KgeMemory.h"
#include "../../include/kge/PluginManager.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/gfx/Renderer.h"
#include "RendererDX.h"

namespace kge
{
	class RendererD3D9Plugin : public Plugin<gfx::Renderer>
	{
	public:

		//! Constructor
		RendererD3D9Plugin()
		{
			m_ePluginType = EPT_Renderer;
		}

		//! Destructor
		~RendererD3D9Plugin()
		{
		}

		//! Create the RendererD3D9 plugin
		gfx::Renderer* Create()
		{			
			return KGE_NEW(gfx::RendererDX)();
		}

	}; // RendererD3D9Plugin

} // kge

extern "C"
{
	void KGELIB_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::RendererD3D9Plugin *pPR = KGE_NEW(kge::RendererD3D9Plugin)();		
		kge::io::Logger::Info("D3D9 renderer plugin registered. version: %d.%d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			pPR->GetEnginePatchVersion(),
			PluginMgr.RegisterRenderer(pPR));
	}
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
