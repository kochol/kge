#include "../../include/kge/core/KgeMemory.h"
#include "../../include/kge/PluginManager.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/gfx/Renderer.h"

namespace kge
{
    class RendererBgfxPlugin : public Plugin<gfx::Renderer>
    {
        public:

        //! Constructor
        RendererBgfxPlugin()
        {
            m_ePluginType = EPT_Renderer;
        }

        //! Destructor
        ~RendererBgfxPlugin()
        {
        }

        //! Create RendererBgfx Plugin
        gfx::Renderer* Create()
        {
            return KGE_NEW(gfx::)
        }

    }; // RendererBgfxPlugin

} // kge

extern "C"
{
	void KGELIB_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::RendererBgfxPlugin *pPR = KGE_NEW(kge::RendererBgfxPlugin)();
		kge::io::Logger::Info("BGFX renderer plugin registered. version: %d.%d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			pPR->GetEnginePatchVersion(),
			PluginMgr.RegisterRenderer(pPR));
	}
}