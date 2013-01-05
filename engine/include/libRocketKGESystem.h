#pragma once

#include "kgedef.h"
#include "Mouse.h"
#include "Keyboard.h"
#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Context.h>

namespace kge
{
	class KGELIB_API libRocketKGESystem: public Rocket::Core::SystemInterface, public io::MouseListener, public io::KeyListener
	{
	public:

		//! Constructor
		libRocketKGESystem();

		/// Get the number of seconds elapsed since the start of the application.
		/// @return Elapsed time, in seconds.
		float GetElapsedTime();

		/// Log the specified message.
		/// @param[in] type Type of log message, ERROR, WARNING, etc.
		/// @param[in] message Message to log.
		/// @return True to continue execution, false to break into the debugger.
		bool LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message);

		//! Sets the rocket context for sending inputs to it.
		void SetContext(Rocket::Core::Context*	pContext);

	protected:

		Rocket::Core::Context	*	m_pContext;

		bool MouseMoved( const io::MouseState &arg );
		bool MousePressed( const io::MouseState &arg, io::MouseButtonID id );
		bool MouseReleased( const io::MouseState &arg, io::MouseButtonID id );

		bool KeyPressed(const io::KeyCode arg);
		bool KeyReleased(const io::KeyCode arg);	

		int GetKeyModifierState();

	}; // libRocketKGESystem

} // kge
