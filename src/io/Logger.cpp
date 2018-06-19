// File name: Logger.h
// Des: Dar in file tamame matn haye ferestade shode az tarafe engine ya user zabt
//      mishe.
// Date: 14/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/io/Logger.h"

#ifdef KGE_CONSOLE
#include <iostream>
#include <stdio.h>

#ifdef WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <stdarg.h>
#endif // WIN32

#if KGE_PLATFORM == KGE_PLATFORM_LINUX
#	include <stdarg.h>
#	include <curses.h>

# define RESET_COLOR "\e[m"
# define WARNING_COLOR "\e[33m"
# define ERROR_COLOR "\e[31m"
# define DEBUG_COLOR "\e[47;30m"
# define SUCCESS_COLOR "\e[32m"

#endif

#endif // KGE_CONSOLE

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
	core::Functor1<void, const char*>	* Logger::pf = NULL;

#ifdef KGE_CONSOLE

	//-------------------------------------------------------------------------------
	// Log(char* Message)
	//-------------------------------------------------------------------------------
	void Logger::Log(const char* message, LogMode lm)
	{
		if (lm==ELM_Error)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED);
#endif // WIN32
		}
		else if (lm==ELM_Warning)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, BACKGROUND_RED | BACKGROUND_GREEN |
									BACKGROUND_BLUE);
#endif // WIN32
		}
		else
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN |
									FOREGROUND_BLUE);
#endif // WIN32
		}

		std::cout << message << std::endl;

		if (pf)
			pf->Call(message);

	} // Log(char* Message)


	//-------------------------------------------------------------------------------
	// Log(char* Message, char* Text)
	//-------------------------------------------------------------------------------
	void Logger::Log(const char* message, const char* Text, LogMode lm)
	{
		if (lm==ELM_Error)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED);
#endif // WIN32
		}
		else if (lm==ELM_Warning)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, BACKGROUND_RED | BACKGROUND_GREEN |
									BACKGROUND_BLUE);
#endif // WIN32
		}
		else
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN |
									FOREGROUND_BLUE);
#endif // WIN32
		}

		std::cout << message << Text << std::endl;

		if (pf)
		{
			pf->Call(message);
			pf->Call(Text);
		}

	} // Log(char* Message, char* Text)

	//-------------------------------------------------------------------------------
	// Log(char* Message , ...)
	//-------------------------------------------------------------------------------
	void Logger::Log(LogMode lm, const char* Text, ...)
	{
		if (lm==ELM_Error)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED);
#endif // WIN32
		}
		else if (lm==ELM_Warning)
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, BACKGROUND_RED | BACKGROUND_GREEN |
									BACKGROUND_BLUE);
#endif // WIN32
		}
		else
		{
#ifdef WIN32
			HANDLE OutputH;
			OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN |
									FOREGROUND_BLUE);
#endif // WIN32
		}

		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);

		std::cout << Message << std::endl;

		if (pf)
			pf->Call(Message);

	} // Log(char* Message , ...)

	//------------------------------------------------------------------------------------
	// Information level logs
	//------------------------------------------------------------------------------------
	void Logger::Info( const char* Text, ... )
	{
		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		static HANDLE OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN |
			FOREGROUND_BLUE);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		printf(RESET_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
		LOGI("%s\n", Message);

#endif

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		std::cout << Message << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		printf("%s" RESET_COLOR "\n", Message);

#endif

	} // Info

	//------------------------------------------------------------------------------------
	// Warning level logs
	//------------------------------------------------------------------------------------
	void Logger::Warning( const char* Text, ... )
	{
		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		static HANDLE OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, FOREGROUND_RED | FOREGROUND_GREEN);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
        printf(WARNING_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
		LOGW("%s\n", Message);

#endif

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		std::cout << Message << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		printf("%s" RESET_COLOR "\n", Message);

#endif

	} // Warning

	//------------------------------------------------------------------------------------
	// Error level logs
	//------------------------------------------------------------------------------------
	void Logger::Error( const char* Text, ... )
	{
		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		static HANDLE OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, FOREGROUND_RED);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
        printf(ERROR_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
		LOGE("%s\n", Message);

#endif

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		std::cout << Message << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		printf("%s" RESET_COLOR "\n", Message);

#endif

	} // Error

	//------------------------------------------------------------------------------------
	// Debug level logs
	//------------------------------------------------------------------------------------
	void Logger::Debug( const char* Text, ... )
	{
		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);


#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		static HANDLE OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, BACKGROUND_RED | BACKGROUND_GREEN |
								BACKGROUND_BLUE);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
        printf(DEBUG_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
		LOGD("%s\n", ls.logMsg.c_str());

#endif

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		std::cout << Message << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
		printf("%s" RESET_COLOR "\n", Message);

#endif

	} // Debug

	//------------------------------------------------------------------------------------
	// Success level logs
	//------------------------------------------------------------------------------------
	void Logger::SuccessKGE( const char* Text, ... )
	{
		char Message[1024];

		va_list va;
		va_start(va, Text);
		vsprintf(Message, Text, va);
		va_end(va);


#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
		static HANDLE OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(OutputH, FOREGROUND_GREEN);

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
                printf(SUCCESS_COLOR);

#elif KGE_PLATFORM == KGE_PLATFORM_ANDROID
				LOGI("%s\n", Message);

#endif

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
				std::cout << Message << std::endl;

#elif KGE_PLATFORM == KGE_PLATFORM_LINUX
				printf("%s" RESET_COLOR "\n", Message);

#endif

	} // SuccessKGE

#endif // KGE_CONSOLE

} // io

} // kge
