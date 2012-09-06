#include "../include/KgeMemory.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "../include/Renderer.h"
#include "RendererOGL.h"

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
			return KGE_NEW(gfx::RendererOGL)();
		}

	}; // RendererD3D9Plugin

} // kge

extern "C"
{
	void KGE_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::RendererOGLPlugin *pPR = KGE_NEW(kge::RendererOGLPlugin)();
		kge::io::Logger::Info("OpenGL renderer plugin registered. version: %d.%d , Index: %d",
			pPR->GetEngineMajorVersion(),
			pPR->GetEngineMinorVersion(),
			PluginMgr.RegisterRenderer(pPR));
	}
}
