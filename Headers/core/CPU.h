// File name: CPU.h
// Des: This class will check CPU's feature(.Eg CPU or operating system support SSE, 3DNOW or MMX ?) 
// Date: 25/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef CPU_H
#define CPU_H

namespace kge
{
namespace core
{
// *** C P U   I N F O ***
typedef struct CPUINFO_TYP
{
	bool bSSE;			// SSE
	bool bSSE2;			// SSE2
	bool b3DNOW;		// 3DNOW!(vendor independent)
	bool bMMX;			// MMX support
	bool bEXT;			// extended features available
	bool bMMXEX;		// MMX(AMD specific extensions)
	bool b3DNOWEX;		// 3DNow! (AMD specific extensions)
	char vendor[13];	// vendor name
} CPUINFO;
// --- C P U   I N F O ---

class CPU
{
public:

	// Preparing the engine for using SSE.
	void Init(void);

	// Tamam moshakhaste CPU ro peyda mikone va dar CPUINFO zabt mikone.
	CPUINFO getCPUinfo(void);

	// Aya system amel ham az SSE poshtibani mikone.
	bool OSSupportsSSE();

}; // CPU

} // core

} // kge

#endif // CPU_H
