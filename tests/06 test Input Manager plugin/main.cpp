#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/InputManager.h"
#include "../../engine/include/Logger.h"

#ifdef _DEBUG
#pragma comment(lib, "../../bin/debug/kge.lib")
#else
#pragma comment(lib, "../../bin/release/kge.lib")
#endif

class TestKeyListener : public kge::io::KeyListener
{
public:

	bool KeyPressed(const kge::io::KeyCode arg)
	{
		kge::io::Logger::Debug("A key is down");
		return true;

	}

	bool KeyReleased(const kge::io::KeyCode arg)
	{
		kge::io::Logger::Debug("A key released");
		return true;
	}

}; // TestKeyListener

int main()
{
	kge::Device dev;
	kge::InitParameters params;
//	params.InputMgrName = "OIS";
	params.RendererName = "ogl";
	dev.Init(params);
	kge::gfx::Renderer*	pRen = dev.GetRenderer();
//	kge::io::InputManager* pInputMgr = kge::io::InputManager::getSingletonPtr();
//	TestKeyListener keylis;
//	pInputMgr->AddKeyListener(&keylis, "test");

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
		pRen->EndRendering();
	}

	return 0;
}
