#ifndef KGE_LOGGER_H
#define KGE_LOGGER_H

#include <queue>
#include "enums.h"
#include "KgeThread.h"
#include <string>

namespace kge
{
	namespace core
	{
		class KgeMutex;
	} // core

	namespace io
	{
		//! Log type
		enum LogType
		{
			ELT_Info,		//!< Information
			ELT_Warning,	//!< Warning
			ELT_Error,		//!< Error
			ELT_Debug,		//!< Debug
			ELT_Success		//!< Successful logs

		}; // LogType

		//! LogStruct is for storing log data.
		struct LogStruct
		{
			std::string		logMsg;		//!< Log message
			LogType			logType;	//!< Log type

		}; // LogStruc

		//! Logger class print logs to console, file or html
		class KGE_API Logger : public core::KgeThread
		{
		public:

			//! Constructor
			Logger();

			//! Destructor
			~Logger();

			//! Information level logs
			static void Info(const char* Text, ...);

			//! Warning level logs
			static void Warning(const char* Text, ...);

			//! Error level logs
			static void Error(const char* Text, ...);

			//! Debug level logs
			static void Debug(const char* Text, ...);

			//! Success level logs
			static void SuccessKGE(const char* Text, ...);

			static LogMode		m_sLogMode;		//!< Log mode
			static LogLevel		m_sLogLevel;	//!< Log level

		protected:

			bool				m_bRun;

			//! Thread function for internal use
			ul32 ThreadProc();

			//! Check for logs queue
			void CheckLogs(LogStruct &ls);

			//! Print the logs to the console
			void LogConsole(LogStruct const &ls);

		}; // Logger

	} // io

} // kge

#endif // KGE_LOGGER_H
