#pragma once

#include "../kgedef.h"
#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/Context.h>

namespace kge
{
	class KGELIB_API libRocketKGESystem: public Rocket::Core::SystemInterface
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

		//! Updates and render the context
		void Render();

	protected:

		Rocket::Core::Context	*	m_pContext;

	}; // libRocketKGESystem

} // kge
