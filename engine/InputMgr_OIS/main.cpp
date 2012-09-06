#include "../include/KgeMemory.h"
#include "../include/PluginManager.h"
#include "../include/Logger.h"
#include "InputMgrOIS.h"

#pragma comment(lib, "../../bin/Debug/kge.lib")

namespace kge
{
	class InputMgrOISPlugin : public Plugin<io::InputManager>
	{
	public:

		//! Constructor
		InputMgrOISPlugin()
		{
			m_ePluginType = EPT_InputManager;
		}

		//! Destructor
		~InputMgrOISPlugin()
		{
		}

		//! Create the RendererD3D9 plugin
		io::InputManager* Create()
		{			
			return KGE_NEW(io::InputMgrOIS)();
		}

	}; // InputMgrOISPlugin

} // kge

extern "C"
{
	void KGE_API RegisterPlugin(kge::PluginManager& PluginMgr)
	{
		kge::InputMgrPlugin *pPR = KGE_NEW(kge::InputMgrOISPlugin)();		
		kge::io::Logger::Info("OIS input manager plugin registered. version: %d.%d , Index: %d", 
			pPR->GetEngineMajorVersion(), 
			pPR->GetEngineMinorVersion(),
			PluginMgr.RegisterInputManager(pPR));
	}
}
