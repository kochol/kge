#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

int main()
{
	kge::Device dev;

	kge::InitParameters params;
	params.RendererName = "ogl";

	dev.Init(params);

	kge::gfx::Renderer*	pRen = dev.GetRenderer();
	if (!pRen)
        return -1;

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		pRen->EndRendering();
	}

	return 0;
}
