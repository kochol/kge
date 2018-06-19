// File name: CPU.cpp
// Des: This class checks the CPU features like CPU cores, SSE support
// Date: 25/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/core/CPU.h"
#include <string.h>
#include <exception>
#include <SDL2/SDL_cpuinfo.h>

// *** G O L O B A L S ***
bool g_bSSE;
// --- G O L O B A L S ---

namespace kge
{
namespace core
{

	//------------------------------------------------------------------------------------
	// Returns the number of CPU cores available.
	//------------------------------------------------------------------------------------
	int CPU::GetCPUCount()
	{
		return SDL_GetCPUCount();

	} // GetCPUCount

	//------------------------------------------------------------------------------------
	// This function returns the L1 cache line size of the CPU
	//------------------------------------------------------------------------------------
	int CPU::GetCPUCacheLineSize()
	{
		return SDL_GetCPUCacheLineSize();

	} // GetCPUCacheLineSize

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has the RDTSC instruction.
	//------------------------------------------------------------------------------------
	bool CPU::HasRDTSC()
	{
		return SDL_HasRDTSC();

	} // HasRDTSC

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has AltiVec features.
	//------------------------------------------------------------------------------------
	bool CPU::HasAltiVec()
	{
		return SDL_HasAltiVec();

	} // HasAltiVec

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has MMX features.
	//------------------------------------------------------------------------------------
	bool CPU::HasMMX()
	{
		return SDL_HasMMX();

	} // HasMMX

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has 3DNow! features.
	//------------------------------------------------------------------------------------
	bool CPU::Has3DNow()
	{
		return SDL_Has3DNow();

	} // Has3DNow

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has SSE features.
	//------------------------------------------------------------------------------------
	bool CPU::HasSSE()
	{
		return SDL_HasSSE();

	} // HasSSE

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has SSE2 features.
	//------------------------------------------------------------------------------------
	bool CPU::HasSSE2()
	{
		return SDL_HasSSE2();

	} // HasSSE2

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has SSE3 features.
	//------------------------------------------------------------------------------------
	bool CPU::HasSSE3()
	{
		return SDL_HasSSE3();

	} // HasSSE3

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has SSE4.1 features.
	//------------------------------------------------------------------------------------
	bool CPU::HasSSE41()
	{
		return SDL_HasSSE41();

	} // HasSSE41

	//------------------------------------------------------------------------------------
	// This function returns true if the CPU has SSE4.2 features.
	//------------------------------------------------------------------------------------
	bool CPU::HasSSE42()
	{
		return SDL_HasSSE42();

	} // HasSSE42

} // core

} // kge