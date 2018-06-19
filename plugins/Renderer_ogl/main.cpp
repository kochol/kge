#include "../../include/kge/core/KgeMemory.h"
#include "../../include/kge/PluginManager.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/gfx/Renderer.h"
#include "RendererGL.h"

#if KGE_COMPILER == KGE_COMPILER_MSVC
#pragma comment(lib, "../../bin/Debug/kge.lib")
#endif

namespace kge
{
	class RendererOGLPlugin : public Plugin<gfx::Renderer>
	{
	public:

		//! Constructor
		RendererOGLPlugin()
		{
			m_ePluginType = EPT_Renderer;
		}

		//! Destructor
		~RendererOGLPlugin()
		{
		}

		//! Create the RendererD3D9 plugin
		gfx::Renderer* Create()
		{			
			return KGE_NEW(gfx::RendererGL)();
		}

	}; // RendererD3D9Plugin

} // kge

extern "C"
{
	void KGELIB_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::RendererOGLPlugin *pPR = KGE_NEW(kge::RendererOGLPlugin)();		
		kge::io::Logger::Info("OpenGL renderer plugin registered. version: %d.%d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			pPR->GetEnginePatchVersion(),
			PluginMgr.RegisterRenderer(pPR));
	}
}
