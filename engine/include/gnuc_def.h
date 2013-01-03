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

#endif // KGE_GNUC_H
