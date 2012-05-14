#include "../../engine/include/PluginManager.h"
#include "../../engine/include/Logger.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/kgedef.h"
#include <iostream>
#include <stdio.h>

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

int main()
{
	kge::io::Logger log;
	kge::PluginManager* pmgr = kge::PluginManager::GetSingletonPtr();

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)

	pmgr->LoadPlugin("Renderer_ogl");

// #endif KGE_PLATFORM == KGE_PLATFORM_WINDOWS

#elif (KGE_PLATFORM == KGE_PLATFORM_LINUX)

    pmgr->LoadPlugin("Renderer_ogl");

#endif

    //pmgr.LoadPlugin("Renderer_ogl");

    if (pmgr->GetRendererPluginsCount() > 0)
    {
        kge::gfx::Renderer* ren = pmgr->GetRendererPlugin(0)->Create();

        kge::InitParameters params;

        ren->Init(params);
    }
	getchar();

	return 0;
}
