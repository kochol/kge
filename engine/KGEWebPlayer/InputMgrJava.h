#ifndef KGE_INPUTMGRJAVA_H
#define KGE_INPUTMGRJAVA_H

#include "../include/InputManager.h"
#include "../include/KgeMemory.h"
#include "../include/Plugin.h"

namespace kge
{
	namespace io
	{
		class InputMgrJava: public InputManager
		{
		public:

			//! Constructor
			InputMgrJava();

			//! Initialise the InputManager
			void Initialise( void* hwnd ) {}

			//! Capture the inputs
			void Capture() {}

			void sendKeyPressed(int key);
			void sendKeyReleased(int key);
			void sendMouseMoved(int x, int y);
			void sendMouseScrolled(int z);
			void sendMousePressed(int id);
			void sendMouseReleased(int id);

		}; // InputMgrJava

		class KGE_API InputMgerJavaPlugin: public Plugin<io::InputManager>
		{
		public:

			//! Constructor
			InputMgerJavaPlugin()
			{
				m_ePluginType = EPT_InputManager;
			}

			//! Destructor
			~InputMgerJavaPlugin()
			{
			}

			//! Create the RendererD3D9 plugin
			io::InputManager* Create()
			{			
				return KGE_NEW(io::InputMgrJava)();
			}

		}; // InputMgerJavaPlugin

	} // io

} // kge

#endif // KGE_INPUTMGRJAVA_H
