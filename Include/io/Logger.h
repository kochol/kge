// File name: Logger.h
// Des: Dar in file tamame matn haye ferestade shode az tarafe engine ya user zabt
//      mishe.
// Date: 14/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_LOGGER_H
#define KGE_LOGGER_H

#include "../kgedef.h"
#include "../core/Functor.h"

#define KGE_CONSOLE // Define this to use console logger we can add the XML, HTML or
				   // TextFile dar ayande

namespace kge
{
namespace io
{
enum LogMode
{
	ELM_Information,
	ELM_Error,
	ELM_Warning
};

class KGE_API Logger
{
public:
	static void Log(const char* message, LogMode lm = ELM_Information);

	static void Log(const char* message, const char* Text, LogMode lm = ELM_Information);

	static void Log(LogMode lm, const char* Text, ...);

	static core::Functor1<void, const char*>	*pf;

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

}; // Logger

} // io

} // kge

#endif // KGE_LOGGER_H
