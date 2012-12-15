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
	params.RendererName = "d3d11";
	if (!dev.Init(params))
		return -1;
	kge::gfx::Renderer*	pRen = dev.GetRenderer();

	pRen->SetClearColor(kge::gfx::Color(200, 100, 200));

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		pRen->EndRendering();
	}

	return 0;
}
