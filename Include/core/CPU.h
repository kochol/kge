// File name: CPU.h
// Des: This class checks the CPU features like CPU cores, SSE support
// Date: 25/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef CPU_H
#define CPU_H

#include "../kgedef.h"

namespace kge
{
namespace core
{
//! This class checks the CPU functionality like CPU cores, SSE support
class KGE_API CPU
{
public:

	//! Returns the number of CPU cores available.
	static int GetCPUCount();

	/**
	 *  This function returns the L1 cache line size of the CPU
	 *
	 *  This is useful for determining multi-threaded structure padding
	 *  or SIMD prefetch sizes.
	 */
	static int GetCPUCacheLineSize();

	/**
	 *  This function returns true if the CPU has the RDTSC instruction.
	 */
	static bool HasRDTSC();

	/**
	 *  This function returns true if the CPU has AltiVec features.
	 */
	static bool HasAltiVec();

	/**
	 *  This function returns true if the CPU has MMX features.
	 */
	static bool HasMMX();

	/**
	 *  This function returns true if the CPU has 3DNow! features.
	 */
	static bool Has3DNow();

	/**
	 *  This function returns true if the CPU has SSE features.
	 */
	static bool HasSSE();

	/**
	 *  This function returns true if the CPU has SSE2 features.
	 */
	static bool HasSSE2();

	/**
	 *  This function returns true if the CPU has SSE3 features.
	 */
	static bool HasSSE3();

	/**
	 *  This function returns true if the CPU has SSE4.1 features.
	 */
	static bool HasSSE41();

	/**
	 *  This function returns true if the CPU has SSE4.2 features.
	 */
	static bool HasSSE42();

}; // CPU

} // core

} // kge

#endif // CPU_H
