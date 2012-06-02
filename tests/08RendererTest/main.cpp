#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/math.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	#ifdef _DEBUG
		#pragma comment(lib, "../../bin/debug/kge.lib")
	#else
		#pragma comment(lib, "../../bin/release/kge.lib")
	#endif
#endif

int main()
{
	kge::Device dev;
	kge::InitParameters params;
	params.RendererName = "ogl";
	dev.Init(params);
	kge::gfx::Renderer*	pRen = dev.GetRenderer();

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		pRen->EndRendering();
	}

	return 0;

} // main
