#ifndef KGE_ENUMS_H
#define KGE_ENUMS_H

namespace kge
{
	namespace gfx
	{
		enum AntiAliasingMode
		{
			EAAM_Off,
			EAAM_MSAA

		}; // AntiAliasingMode

	} // gfx

	namespace io
	{
		//! Where to print logs
		enum LogMode
		{
			ELM_Console //!< print logs to console

		}; // LogMode

		//! What to log
		enum LogLevel
		{
			ELL_EveryThing,		//!< Logs everything
			ELL_Information,	//!< Logs informations, warnings and errors
			ELL_Warning,		//!< Logs warnings and errors
			ELL_Error,			//!< Logs errors
			ELL_Nothing			//!< No logs

		}; // LogLevel

		//! For controling mouse pointer
		enum CURSOR_TYPES
		{
			CUR_SHOW,		//!< Show mouse cursor in the window
			CUR_HIDE,		//!< Hide mouse cursor in the window
			CUR_POINTER		//!< Set mouse cursor to the default cursor

		}; // mouse cursors

	} // io

} // kge

#endif // KGE_ENUMS_H
