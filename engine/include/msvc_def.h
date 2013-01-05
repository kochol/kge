//#############################################################################
//#	desc:	defination and compiler setting for Microsoft Visual C++
//#	file:	msvc_def.h
//#	cdate:	nov-25-2011
//#	author:	Pooya Shainfar (Pswin)
//#############################################################################

#ifndef KGE_MSVC_DEF_H
#define KGE_MSVC_DEF_H

//=====================================
// os and compiler specific includes
//=====================================
#include <windows.h>
#include <tchar.h>
#include "Config.h"

//=====================================
// compiler warnings
//=====================================

// Unwanted VC++ level 4 warnings to disable.
#pragma warning(disable : 4049)
#pragma warning(disable : 4311)
#pragma warning(disable : 4312)
#pragma warning(disable : 4244) // conversion to float, possible loss of data
#pragma warning(disable : 4699) // creating precompiled header
#pragma warning(disable : 4200) // Zero-length array item at end of structure, a VC-specific extension
#pragma warning(disable : 4100) // unreferenced formal parameter
#pragma warning(disable : 4514) // unreferenced inline function has been removed
#pragma warning(disable : 4201) // nonstandard extension used : nameless struct/union
#pragma warning(disable : 4710) // inline function not expanded
#pragma warning(disable : 4714) // __forceinline function not expanded
#pragma warning(disable : 4702) // unreachable code in inline expanded function
#pragma warning(disable : 4711) // function selected for autmatic inlining
#pragma warning(disable : 4127) // Conditional expression is constant
#pragma warning(disable : 4512) // assignment operator could not be generated
#pragma warning(disable : 4245) // conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch
#pragma warning(disable : 4389) // signed/unsigned mismatch
#pragma warning(disable : 4251) // needs to have dll-interface to be used by clients of class
#pragma warning(disable : 4275) // non dll-interface class used as base for dll-interface class
#pragma warning(disable : 4511) // copy constructor could not be generated
#pragma warning(disable : 4284) // return type is not a UDT or reference to a UDT
#pragma warning(disable : 4355) // this used in base initializer list
#pragma warning(disable : 4291) // typedef-name '' used as synonym for class-name ''
#pragma warning(disable : 4324) // structure was padded due to __declspec(align())
#pragma warning(disable : 4996) // 'function': was declared deprecated
#pragma warning(disable : 4800) // 'function': was declared deprecated

// It'd be nice to turn these on, they can't be used in DEBUG due to the varargs stuff.
#if defined(DEBUG) || defined(_DEBUG)
#else
	#pragma warning(disable : 4189) // local variable is initialized but not referenced
	#pragma warning(disable : 4505) // unreferenced local function has been removed
#endif


//=====================================
// C++11 (C++0x) Support check
//=====================================
#if _MSC_VER >= 1600 
	#define KGE_CPP11_COMPILER 1
#else
	#define KGE_CPP11_COMPILER 0
#endif

//=====================================
// DLL export / import
//=====================================
#ifdef KGE_EXPORT
	#define KGE_API __declspec( dllexport )
#else
	#define KGE_API __declspec( dllimport )
#endif

#ifdef KGELIB_EXPORTS
#define KGELIB_API __declspec( dllexport )
#else
#define KGELIB_API __declspec( dllimport )
#endif

#define KGE_IMPORT __declspec( dllimport )

//=====================================
// wrong compiler settings
//=====================================

// If C++ exception handling is disabled, force guarding to be off.
#ifndef _CPPUNWIND
	#error "Bad VC++ option: C++ exception handling must be enabled"
#endif

// Make sure characters are unsigned.
#ifdef _CHAR_UNSIGNED
	#error "Bad VC++ option: Characters must be signed"
#endif


//=====================================
// Other Macros
//=====================================

#define KGE_FORCE_INLINE __forceinline
#define KGE_NO_INLINE __declspec(noinline)
#define KGE_NAKED_FUNC void _declspec(naked)

#if KGE_USE_SIMD == 1
	#define KGE_ALIGN __declspec(align(16))
#else
	#define KGE_ALIGN
#endif


//=====================================
// type definitions
//=====================================

#endif // KGE_MSVC_DEF_H
