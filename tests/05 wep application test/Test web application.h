#ifndef TESTWEBAPPLICATION_H
#define TESTWEBAPPLICATION_H

#include "../../engine/include/WebApplication.h"
#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/InputManager.h"
#include "../../engine/include/Logger.h"

kge::Device dev;
kge::InitParameters params;
kge::gfx::Renderer* pRen;

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

namespace kge
{
	namespace web
	{
		class KGE_API TestWebApp: public WebApplication
		{
		public:

			//! Constructor
			TestWebApp() {}

			//! Destructor
			~TestWebApp() {}

			//! The WebPlayer call this function when you have to initialise your application.
			void Initialise() {}

			//! The WebPlayer call this function when you have to update your game step.
			/*! Return false to stop the WebPlayer
			 */
			bool Update() 
			{
				if (dev.Run())
				{
					pRen->BeginRendering(true, true, true);
					pRen->EndRendering();
					return true;
				}

				return false;
			}

			//! The WebPlayer call this function when you have to shutdown your application.
			void ShutDown() {}

			//! The WebPlayer call this function give you the window handle and size for render.
			/*!
				\param hwnd The Window handle
				\param width The window width
				\param height The window height
			 */
			void SetCanvas(int hwnd, int width, int height) 
			{	
				params.hwnd = (void*)hwnd;
				params.InputMgrName = "OIS";
				dev.Init(params);
				pRen = dev.GetRenderer();
				TestKeyListener* keylis = new TestKeyListener();
				io::InputManager::getSingletonPtr()->AddKeyListener(keylis, "test");
			}

			//! Gives the resource path for loading default resources
			void SetResourcePath(const char* path) {}


		}; // TestWebApp

	} // web

} // kge

#endif // TESTWEBAPPLICATION_H
