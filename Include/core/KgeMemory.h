#ifndef KGE_KGEMEMORY_H
#define KGE_KGEMEMORY_H

#include "../config.h"
#include "../kgedef.h"
#include "../../Headers/core/KgeNedmalloc.h"
#include "../../Headers/core/KgeMemoryTrack.h"
#include <new>

namespace kge
{
	namespace core
	{
#	if KGE_MEMORY_ALLOCATOR == KGE_MEMORY_NEDMALLOC
		class KGE_API KgeMemory: public kge::core::KgeNedmalloc {};

#	endif
	} // core

} // kge

//! Use KGE_NEW instead of new to track memory leaks and use internal KGE allocator that is faster
//!	example \code kge::Device* pDev = KGE_NEW(kge::Device)(/*Constructor values*/); \endcode
#define KGE_NEW(T) new (kge::core::KgeMemory::AllocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T

//! If you use KGE_NEW to allocate memory then you have to use KGE_DELETE to deallocate it and free memory.
//! First parameter is pointer the second one is class name without anything else like namespace only class name or its parent name
//!	example \code KGE_DELETE(pDev, Device); \endcode
#define KGE_DELETE(ptr, T) if (ptr) {(ptr)->~T(); kge::core::KgeMemory::DeallocateBytes(ptr); ptr = NULL; }
#define KGE_DELETETHIS(ptr, T) if (ptr) {(ptr)->~T(); kge::core::KgeMemory::DeallocateBytes(ptr); }

#define KGE_NEW_ARRAY(T, count) (T*) kge::core::KgeMemory::AllocateBytes(sizeof(T) * count, __FILE__, __LINE__, __FUNCTION__)
#define KGE_NEW_CLASS_ARRAY(T, count) (new (kge::core::KgeMemory::AllocateBytes(sizeof(T) * count + 4, __FILE__, __LINE__, __FUNCTION__)) T[count])
#define KGE_DELETE_ARRAY(ptr) if (ptr) { kge::core::KgeMemory::DeallocateBytes(ptr); ptr = NULL; }
#ifdef _MSC_VER
#define KGE_DELETE_CLASS_ARRAY(ptr, T, count) if (ptr) { 	for (int i = 0; i < count; i++) ptr[i].~T(); kge::core::KgeMemory::DeallocateBytes((void*)((unsigned long long)ptr - sizeof(void*))); ptr = NULL; }
#else
#define KGE_DELETE_CLASS_ARRAY(ptr, T, count) if (ptr) { 	for (int i = 0; i < count; i++) ptr[i].~T(); kge::core::KgeMemory::DeallocateBytes(ptr); ptr = NULL; }
#endif

#define KGE_REALLOC_ARRAY(ptr, T, count)(T*)kge::core::KgeMemory::ReallocateBytes(ptr, sizeof(T) * count, __FILE__, __LINE__, __FUNCTION__)

#endif // KGE_KGEMEMORY_H
