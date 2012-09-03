#include <iostream>

#include "../include/Logger.h"
#include "../include/KgeMutex.h"
#include "../include/os.h"

#if KGE_PLATFORM == KGE_PLATFORM_LINUX
#	include <stdarg.h>
#	include <curses.h>
#endif

#if KGE_PLATFORM == KGE_PLATFORM_ANDROID
#	include <android/log.h>
#	include <unistd.h>
#	define  LOG_TAG    "kge"
#	define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#	define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#	define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#	define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif

namespace kge
{
	namespace io
	{
		static core::KgeMutex			m_sQueueMutex;	//!< Queue protector
		static std::queue<LogStruct>	m_qLogs;		//!< Logs queue
		LogLevel	Logger::m_sLogLevel = ELL_EveryThing;
		LogMode		Logger::m_sLogMode  = ELM_Console;

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		HANDLE OutputH;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX

#   define RESET_COLOR "\e[m"
#   define WARNING_COLOR "\e[33m"
#   define ERROR_COLOR "\e[31m"
#   define DEBUG_COLOR "\e[47;30m"
#   define SUCCESS_COLOR "\e[32m"

#endif

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Logger::Logger(): m_bRun(true)
		{

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

			// Begin the log thread
			Begin();

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Logger::~Logger()
		{
			// Stop the log class
			m_bRun = false;
			End();

		} // Destructor

		//------------------------------------------------------------------------------------
		// Information level logs
		//------------------------------------------------------------------------------------
		void Logger::Info(const char* Text, ...)
		{
			if (m_sLogLevel == ELL_Warning || m_sLogLevel == ELL_Error
				|| m_sLogLevel == ELL_Nothing)
				return;

			static char Message[1024];

			va_list va;
			va_start(va, Text);
			vsprintf(Message, Text, va);
			va_end(va);

			LogStruct sl;
			sl.logMsg  = Message;
			sl.logType = ELT_Info;

			m_sQueueMutex.On();
			m_qLogs.push(sl);
			m_sQueueMutex.Off();

		} // Info

		//------------------------------------------------------------------------------------
		// Warning level logs
		//------------------------------------------------------------------------------------
		void Logger::Warning(const char* Text, ...)
		{
			if (m_sLogLevel == ELL_Error || m_sLogLevel == ELL_Nothing)
				return;

			static char Message[1024];

			va_list va;
			va_start(va, Text);
			vsprintf(Message, Text, va);
			va_end(va);

			LogStruct sl;
			sl.logMsg  = Message;
			sl.logType = ELT_Warning;

			m_sQueueMutex.On();
			m_qLogs.push(sl);
			m_sQueueMutex.Off();

		} // Warning

		//------------------------------------------------------------------------------------
		// Error level logs
		//------------------------------------------------------------------------------------
		void Logger::Error(const char* Text, ...)
		{
			if (m_sLogLevel == ELL_Nothing)
				return;

			static char Message[1024];

			va_list va;
			va_start(va, Text);
			vsprintf(Message, Text, va);
			va_end(va);

			LogStruct sl;
			sl.logMsg  = Message;
			sl.logType = ELT_Error;

			m_sQueueMutex.On();
			m_qLogs.push(sl);
			m_sQueueMutex.Off();

		} // Error

		//------------------------------------------------------------------------------------
		// Debug level logs
		//------------------------------------------------------------------------------------
		void Logger::Debug(const char* Text, ...)
		{

			if (m_sLogLevel != ELL_EveryThing)
				return;

			static char Message[1024];

			va_list va;
			va_start(va, Text);
			vsprintf(Message, Text, va);
			va_end(va);

			LogStruct sl;
			sl.logMsg  = Message;
			sl.logType = ELT_Debug;

			m_sQueueMutex.On();
			m_qLogs.push(sl);
			m_sQueueMutex.Off();

		} // Debug

		//------------------------------------------------------------------------------------
		// Success level logs
		//------------------------------------------------------------------------------------
		void Logger::SuccessKGE(const char* Text, ...)
		{

			if (m_sLogLevel == ELL_Nothing)
				return;

			static char Message[1024];

			va_list va;
			va_start(va, Text);
			vsprintf(Message, Text, va);
			va_end(va);

			LogStruct sl;
			sl.logMsg  = Message;
			sl.logType = ELT_Success;

			m_sQueueMutex.On();
			m_qLogs.push(sl);
			m_sQueueMutex.Off();

		} // Success

		//------------------------------------------------------------------------------------
		// Thread function for internal use
		//------------------------------------------------------------------------------------
		ul32 Logger::ThreadProc()
		{
			LogStruct ls;
			while (m_bRun)
			{
				CheckLogs(ls);
				// Sleep for a while
				os::KGESleep(250);
			}

			CheckLogs(ls);

			return 0;

		} // ThreadProc

		//------------------------------------------------------------------------------------
		// Check for logs queue
		//------------------------------------------------------------------------------------
		void Logger::CheckLogs(LogStruct &ls)
		{
			while (m_qLogs.size() > 0)
			{
				//! Get the first log
				m_sQueueMutex.On();
				ls = m_qLogs.front();
				m_qLogs.pop();
				m_sQueueMutex.Off();

				switch (m_sLogMode)
				{
				case ELM_Console:
					LogConsole(ls);
					break;
				}
			}

		} // CheckLogs

		//------------------------------------------------------------------------------------
		// Print the logs to the console
		//------------------------------------------------------------------------------------
		void Logger::LogConsole(LogStruct const &ls)
		{
			switch (ls.logType)
			{
			case ELT_Info:
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN |
										FOREGROUND_BLUE);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(RESET_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGI("%s\n", ls.logMsg.c_str());

#endif

				break;

			case ELT_Warning:
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(WARNING_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGW("%s\n", ls.logMsg.c_str());

#endif

				break;

			case ELT_Error:
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SetConsoleTextAttribute(OutputH, FOREGROUND_RED);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(ERROR_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGE("%s\n", ls.logMsg.c_str());

#endif

				break;

			case ELT_Debug:
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SetConsoleTextAttribute(OutputH, BACKGROUND_RED | BACKGROUND_GREEN |
										BACKGROUND_BLUE);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(DEBUG_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGD("%s\n", ls.logMsg.c_str());

#endif

				break;

			case ELT_Success:
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				SetConsoleTextAttribute(OutputH, FOREGROUND_GREEN);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(SUCCESS_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGI("%s\n", ls.logMsg.c_str());

#endif

				break;
			}

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
			std::cout << ls.logMsg.c_str() << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
			printf("%s" RESET_COLOR "\n", ls.logMsg.c_str());

#endif

		} // LogConsole

	} // io

} // kge
