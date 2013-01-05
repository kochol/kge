#include "../include/libRocketKGESystem.h"
#include "../include/Logger.h"
#include "../include/Timer.h"
#include "../include/InputManager.h"

namespace kge
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	libRocketKGESystem::libRocketKGESystem(): m_pContext(NULL)
	{
		kge::io::InputManager* pInputMgr = kge::io::InputManager::getSingletonPtr();
		if (pInputMgr)
		{
			pInputMgr->AddKeyListener(this, "libRocket");
			pInputMgr->AddMouseListener(this, "libRocket");
		}

	} // Constructor

	//------------------------------------------------------------------------------------
	// Get the number of seconds elapsed since the start of the application.
	//------------------------------------------------------------------------------------
	float libRocketKGESystem::GetElapsedTime()
	{
		static core::Timer t;

		return (float)t.GetTime(false) / 1000000.0f;

	} // GetElapsedTime

	//------------------------------------------------------------------------------------
	// Log the specified message.
	//------------------------------------------------------------------------------------
	bool libRocketKGESystem::LogMessage( Rocket::Core::Log::Type type, const Rocket::Core::String& message )
	{
		switch (type)
		{
		case Rocket::Core::Log::Type::LT_ASSERT:
		case Rocket::Core::Log::Type::LT_ERROR:
			io::Logger::Error("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_DEBUG:
			io::Logger::Debug("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_WARNING:
			io::Logger::Warning("LibRocket: %s", message.CString());
			break;

		case Rocket::Core::Log::Type::LT_INFO:
			io::Logger::Info("LibRocket: %s", message.CString());
			break;
		}

		return true;

	} // LogMessage

	//------------------------------------------------------------------------------------
	// Sets the rocket context for sending inputs to it.
	//------------------------------------------------------------------------------------
	void libRocketKGESystem::SetContext( Rocket::Core::Context* pContext )
	{
		m_pContext = pContext;

	} // SetContext

	bool libRocketKGESystem::MouseMoved( const io::MouseState &arg )
	{
		if (m_pContext)
		{			
			m_pContext->ProcessMouseMove(arg.X.abs, arg.Y.abs, GetKeyModifierState());
			io::Logger::Debug("%d %d", arg.X.abs, arg.Y.abs);
		}

		return true;

	} // MouseMoved

	bool libRocketKGESystem::MousePressed( const io::MouseState &arg, io::MouseButtonID id )
	{
		if (m_pContext)
		{
			m_pContext->ProcessMouseButtonDown((int)id, GetKeyModifierState());
		}

		return true;

	} // MousePressed

	bool libRocketKGESystem::MouseReleased( const io::MouseState &arg, io::MouseButtonID id )
	{
		if (m_pContext)
		{
			m_pContext->ProcessMouseButtonUp((int)id, GetKeyModifierState());
		}

		return true;

	} // MouseReleased

	bool libRocketKGESystem::KeyPressed( const io::KeyCode arg )
	{
		return true;

	} // KeyPressed

	bool libRocketKGESystem::KeyReleased( const io::KeyCode arg )
	{
		return true;

	} // KeyReleased

	int libRocketKGESystem::GetKeyModifierState()
	{
		int keystate = 0;

		kge::io::InputManager* pInputMgr = kge::io::InputManager::getSingletonPtr();
		if (pInputMgr)
		{
			if (pInputMgr->GetKeyboard())
			{
				if (pInputMgr->GetKeyboard()->isModifierDown(io::Keyboard::Modifier::Ctrl))
					keystate |= Rocket::Core::Input::KM_CTRL;
				if (pInputMgr->GetKeyboard()->isModifierDown(io::Keyboard::Modifier::Alt))
					keystate |= Rocket::Core::Input::KM_ALT;
				if (pInputMgr->GetKeyboard()->isModifierDown(io::Keyboard::Modifier::Shift))
					keystate |= Rocket::Core::Input::KM_SHIFT;
			}
		}

		return keystate;

	} // GetKeyModifierState

} // kge
