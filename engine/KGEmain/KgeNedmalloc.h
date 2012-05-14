#ifndef KGE_KGENEDMALLOC_H
#define KGE_KGENEDMALLOC_H

#include "../include/kgedef.h"

namespace kge
{
	namespace core
	{
		//! Ned allocator class
		class KGE_API KgeNedmalloc
		{
		public:

			//! Allocate bytes
			static void* AllocateBytes(u32 BytesCount,  char* file = 0, int line = 0, const char* function = 0);

			//! Deallocate bytes
			static void DeallocateBytes(void* ptr);

		}; // KgeNedmalloc

	} // core

} // kge

#endif // KGE_KGENEDMALLOC_H
