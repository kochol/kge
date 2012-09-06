//#############################################################################
//#	desc:	Gnu C/C++ compiler specific configurations and definations
//#	file:	gnuc_def.h
//#	cdate:	nov-11-2011, 1390/09/04 (jalali)
//# author:	Pooya Shahinfar (Pswin)
//#############################################################################

#ifndef KGE_GNUC_DEF_H
#define KGE_GNUC_DEF_H

#include "Config.h"

//=====================================
// check compiler version
//=====================================
#if (( __GNUC__ * 10) + __GNUC_MINOR__) < 44
	#pragma error "KGE at last need GCC 4.4 for compiling"
#endif

//=====================================
//  C++11 (C++0x) Support check
//=====================================
#if defined(__GXX_EXPERIMENTAL_CPP0X__) || defined(__GXX_EXPERIMENTAL_CXX0X__)
	#define KGE_CPP11_COMPILER 1
#endif

//=====================================
// other macros
//=====================================

//! no operation?!
#ifdef __noop
	#undef __noop
	#define __noop(...)
#endif

#define KGE_FORCE_INLINE __inline __attribute__ ((always_inline))
#define KGE_NO_INLINE __attribute__ ((noinline))
#define KGE_NAKED_FUNC void __attribute__ ((naked))


//=====================================
// DLL export / import
//=====================================
#if KGE_USE_SIMD == 1
	#define KGE_ALIGN __attribute__ ((aligned (16)))
#else
	#define KGE_ALIGN
#endif

//#ifdef KGE_EXPORT
//	#define KGE_API  __attribute__ ((visibility("default")))
//#else
//	#define KGE_API __attribute__ ((visibility("hidden")))
//#endif
//
//#define KGE_IMPORT __attribute__ ((visibility("hidden")))
#define KGE_API
#define KGE_IMPORT
//=====================================
// type definations
//=====================================

namespace kge
{
// 64-bit architecture
#if defined(__x86_x64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
	#error "kge dosen't support 64-bit compilition"
#else // 32-bit cpu architecture
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


#endif // KGE_GNUC_H
