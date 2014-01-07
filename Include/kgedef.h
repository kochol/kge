// File name: kgedef.h
// Des: The definitions
// Date: 6/5/1385(jalali) , updated: 6,5,1389(jalali)
// Programmer: Ali Akbar Mohammadi(Kochol), Pooya Shahinfar (Pswin)

#ifndef KGEDEF_H
#define KGEDEF_H

#include "config.h"

#define kgedecref(s) if (s) {s->DecRef(); s = NULL;}

//************************************
// Version Information
//************************************
#define KGE_VERSION_NUM 0.0.11
#define KGE_VERSION_MAJOR 0
#define KGE_VERSION_MINOR 0
#define KGE_VERSION_RELEASE 11
#define KGE_VERSION_SUFFIX ""
#define KGE_VERSION_NAME "Kochol Game Engine 0.0.11"

#define KGE_VERSION    ((KGE_VERSION_MAJOR << 16) | (KGE_VERSION_MINOR << 8) | KGE_VERSION_RELEASE)

//**************************************
// compiler and platform 
//**************************************

#define KGE_PLATFORM_UNKNOWN 0	// Unknown platform
#define KGE_PLATFORM_WINDOWS 1	// Microsoft Windows
#define KGE_PLATFORM_LINUX 2	// Linux OS
#define KGE_PLATFORM_APPLE 3	// Apple MAC-OS
#define KGE_PLATFORM_ANDROID 4	// Android platform

#define KGE_COMPILER_UNKNOWN 0	// Unknown compiler
#define KGE_COMPILER_MSVC 1		// Microsoft Visual C++
#define KGE_COMPILER_GNUC 2		// GNU C/C++ compilers
#define KGE_COMPILER_BORL 3		// Borland C/C++ compiler
#define KGE_COMPILER_NDK  4		// NDK build system

#define KGE_ARCHITECTURE_UNKNOWN 0	// Unknown architecture
#define KGE_ARCHITECTURE_32 1		// 32-bit architecture CPU
#define KGE_ARCHITECTURE_64 2		// 64-bit architecture CPU

// find compiler type and version
#if defined( _MSC_VER )
	#define KGE_COMPILER KGE_COMPILER_MSVC
	#define KGE_COMP_VER _MSC_VER

#elif defined( __GNUC__ )
	#define KGE_COMPILER KGE_COMPILER_GNUC
	#define KGE_COMP_VER (((__GNUC__)*100) + \
			(__GNUC_MINOR__*10) + \
			__GNUC_PATCHLEVEL__)
	
#elif defined( __BORLANDC__ )
	#define KGE_COMPILER KGE_COMPILER_BORL
	#define KGE_COMP_VER __BCPLUSPLUS__

#elif defined( NDK )
	#define KGE_COMPILER KGE_PLATFORM_ANDROID
	#define KGE_COMP_VER 7

#else
	#define KGE_COMPILER KGE_COMPILER_UNKNOWN
	#pragma error "No known compiler. Compiling Abrot!"
#endif

// finds the platform
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WIN64)
	#define KGE_PLATFORM KGE_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__)
	#define KGE_PLATFORM KGE_PLATFORM_APPLE
#elif defined(ANDROID)
	#define KGE_PLATFORM KGE_PLATFORM_ANDROID
#elif defined(__unix__)
	#define KGE_PLATFORM KGE_PLATFORM_LINUX
#else
	#define KGE_PLATFORM KGE_PLATFORM_UNKNOWN
#endif

// finds the CPU architecture type
#if defined(__x86_x64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
	#define KGE_ARCHITECTURE_TYPE KGE_ARCHITECTURE_64
#else
	#define KGE_ARCHITECTURE_TYPE KGE_ARCHITECTURE_32
#endif

//*************************************
// Defining debug mode
//*************************************
#if defined(DEBUG) || defined(_DEBUG)
	#define KGE_DEBUG_MODE 1
#else
	#define KGE_DEBUG_MODE 0
#endif

//**************************************
// unicode
//**************************************
#if defined(_UNICODE) && (KGE_USE_UNICODE == 1)
	#define KGE_SUPPORT_UNICODE 1
#else 
	#define KGE_SUPPORT_UNICODE 0
#endif

//**************************************
// compiler specific config
//**************************************

#if KGE_COMPILER == KGE_COMPILER_MSVC
	// common compiler defines
	#define KGE_USE_MSVC_ASM 1
#ifdef KGE_EXPORT
	#define KGE_API __declspec(dllexport)
#else
	#define KGE_API __declspec(dllimport)
#endif
#ifdef KGELIB_EXPORTS
#	define KGELIB_API __declspec( dllexport )
#else
#	define KGELIB_API __declspec( dllimport )
#endif
	#define KGE_IMPORT __declspec( dllimport )
	#define NAKED_FUNC  void _declspec (naked)


	// Unwanted VC++ level 4 warnings to disable.
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

	// Unwanted VC++ level 1 warnings to disable.
	#pragma warning(disable : 4996) // 'function': was declared deprecated

	// It'd be nice to turn these on, they can't be used in DEBUG due to the varargs stuff.
	#if KGE_DEBUG_MODE
	#else
		#pragma warning(disable : 4189) // local variable is initialized but not referenced
		#pragma warning(disable : 4505) // unreferenced local function has been removed
	#endif


#else
	#define KGE_API
	#define KGE_IMPORT
	#define NAKED_FUNC void
#endif

//**************************************
// Entity system thread safe
//**************************************

//**************************************
// constants 
//**************************************

#define MAXID 65535		// MAXID = NULL for u32 IDs
#define KGE_BB_SPHERE
#define KGE_DEVICE_LOST 0x8001
#define KGE_DEVICE_RESET 0x8002

//**************************************
// type definations
//**************************************

namespace kge
{

	//! Unsigned integer
	typedef unsigned int 	uint;

	// 64-bit architecture
#if defined(__x86_x64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#	error "kge dosen't support 64-bit compilition yet"
#else // 32-bit CPU architecture

	//! A 32-bit unsigned int
	typedef unsigned int   u32;

	//! A 32-bit signed int
	typedef signed int     s32;

	//! A 64-bit unsigned int
	typedef unsigned long long   u64;

	//! A 64-bit signed int
	typedef signed long long     s64;

	//! A 32-bit unsigned integer = DWORD
	typedef unsigned long  ul32;

	//! A 16-bit unsigned short
	typedef unsigned short u16;

	//! A 16-bit signed short
	typedef signed short   s16;

	//! An 8-bit unsigned char
	typedef unsigned char  u8;

	const double Rounding_Error_Double = 0.00000001;

	//! 32bit floating point
	typedef float			f32;

	//! 64bit floating point
	typedef double			f64;

#endif

} // kge

#endif // KGEDEF_H
