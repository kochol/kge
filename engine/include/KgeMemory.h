#ifndef KGE_KGEMEMORY_H
#define KGE_KGEMEMORY_H

#include "Config.h"
#include "kgedef.h"
#include "../KGEmain/KgeNedmalloc.h"

namespace kge
{
	namespace core
	{
#	if KGE_MEMORY_ALLOCATOR == KGE_MEMORY_NEDMALLOC
		class KGE_API KgeMemory: public kge::core::KgeNedmalloc {};

#	endif
	} // core

} // kge

#define KGE_NEW(T) new (kge::core::KgeMemory::AllocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
#define KGE_DELETE(ptr, T) if (ptr) {(ptr)->~T(); kge::core::KgeMemory::DeallocateBytes(ptr); ptr = NULL; }

#define KGE_NEW_ARRAY(T, count) (T*)kge::core::KgeMemory::AllocateBytes(sizeof(T) * count, __FILE__, __LINE__, __FUNCTION__)
#define KGE_DELETE_ARRAY(ptr) if (ptr) { kge::core::KgeMemory::DeallocateBytes(ptr); ptr = NULL; }

#endif // KGE_KGEMEMORY_H
