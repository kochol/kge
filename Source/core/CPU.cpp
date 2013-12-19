// File name: CPU.cpp
// Des: In class ghabeliat haye cpu ra check mikone masalan az SSE, 3DNOW ya MMX 
//		poshtibani mikone ya aslan system amel ham az onha poshtibani mikone?
// Date: 25/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Headers/core/CPU.h"
#include <string.h>
#include <exception>

// *** G O L O B A L S ***
bool g_bSSE;
// --- G O L O B A L S ---

namespace kge
{
namespace core
{
	// ***** *********** *** ** ***** ****** ** *** ******* **** *******
	// Tamam moshakhaste CPU ro peyda mikone va dar CPUINFO zabt mikone.
	// ***** *********** *** ** ***** ****** ** *** ******* **** *******
	CPUINFO CPU::getCPUinfo()
	{
		CPUINFO info;
		char *pStr = info.vendor;
		int n = 1;
		int *pn = &n;

		memset(&info, 0, sizeof(CPUINFO));

#ifdef WIN32
		// 1. Check for SSE, SSE2 and MMX support.
		try
		{
			_asm
			{
				mov eax,0 // get vendor name
				CPUID
			
				mov esi, pStr
				mov [esi], ebx		// first 4 chars
				mov [esi+4], edx	// next 4 chars
				mov [esi+8], ecx	// final 4 chars

				mov eax, 1			// feature-List
				CPUID

				test edx, 04000000h // test SSE2
				jz _NOSSE2			// jump if negative
				mov [info.bSSE2],1  // true

_NOSSE2: test edx, 02000000h	// test SSE
				jz _NOSSE			// jump if negative
				mov [info.bSSE],1	// true

_NOSSE: test edx, 00800000h		// test MMX
				jz _EXIT1			// jump if negative
				mov [info.bMMX], 1	// true

_EXIT1:							// done
			}
		}
		catch (const int& e)
		{
			return info;		// unknown error
		}

		// 2. Test extended Features
		_asm
		{	
			mov eax, 80000000h	// extended features?
			CPUID
			cmp eax, 80000000h	// >0x80
			jbe _EXIT2			// jump if negative
			mov [info.bEXT], 1	// true

			mov eax,80000001h	
			CPUID
			test edx, 80000000h	// test 3DNOW!
			jz _EXIT2			// jump if negative
			mov [info.b3DNOW],1 // true
_EXIT2:	
		}

		// 3. vendor dependent things
		if((strncmp(info.vendor, "GenuineIntel", 12)==0) && info.bEXT)
		{	// INTEL
			_asm
			{
				mov eax,1		// Feature list
				CPUID
				mov esi, pn		// Brand ID
				mov [esi], ebx
			}
			int m = 0;
			memcpy(&m, pn, sizeof(char));
			n = m;
		}
		else if((strncmp(info.vendor, "AuthenticAMD", 12)==0) && info.bEXT)
		{	// AMD
			_asm
			{
				mov eax, 1
				CPUID
				mov esi,pn
				mov [esi],eax
	
				mov eax,0x80000001
				CPUID

				test edx,0x40000000
				jz _AMD1
				mov [info.b3DNOWEX],1

_AMD1: test edx,0x00400000
				jz _AMD2
				mov [info.bMMXEX],1	// true
_AMD2:
			}
		}
#endif // WIN32
		return info;
	} // Get CPU Information.

	// **** ** ******** ****
	// Does OS Supports SSE?
	// **** ** ******** ****
	bool CPU::OSSupportsSSE()
	{
#ifdef WIN32
		try
		{
			_asm xorps xmm0, xmm0
		}
		catch (const int& e)
		{
			return false;
		}
		return true;
		
#else // WIN32
		
		return false;
		
#endif // LINUX
		
	} // OS Supports SSE

	// ****
	// Init
	// ****
	void CPU::Init()
	{
		g_bSSE = false;
		CPUINFO cpu = getCPUinfo();
		if (cpu.bSSE && OSSupportsSSE())
			g_bSSE = true;
	} // Init

} // core

} // kge