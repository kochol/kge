#ifndef KGE_KGEDEF_H
#define KGE_KGEDEF_H

#define MAXID 65535		// MAXID = NULL for u32 IDs

//************************************
// Version Information
//************************************
#define KGE_VERSION_NUM "0.1.0"
#define KGE_VERSION_MAJOR 0
#define KGE_VERSION_MINOR 1
#define KGE_VERSION_RELEASE 0
#define KGE_VERSION_SUFFIX ""
#define KGE_VERSION_NAME "Kochol Game Engine Beta version 0.1.0"

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
	#include "msvc_def.h"
#elif KGE_COMPILER == KGE_COMPILER_GNUC
	#include "gnuc_def.h"
#else
	#error "kge dosn't supoort this compiler"
#endif 

//**************************************
// typedef
//**************************************
namespace kge
{
	typedef unsigned int	uint;

	// 64-bit architecture
#if defined(__x86_x64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#error "kge dosen't support 64-bit compilition"
#else // 32-bit CPU architecture
	//! An 8-bit unsigned char
	typedef unsigned char	u8;

	//! A 16-bit unsigned short
	typedef unsigned short	u16;

	//! A 16-bit signed short
	typedef signed short	s16;

	//! 32bit signed integer
	typedef int				s32;

	//! 32bit unsigned integer
	typedef unsigned int	u32;

	//! 32bit unsigned long
	typedef unsigned long	ul32;

	//! 32bit floating point
	typedef float			f32;

	//! 64bit floating point
	typedef double			f64;

#endif

} // kge

#endif // KGE_KGEDEF_H
