#include "KgeNedmalloc.h"
#include "../../libs/nedmalloc/nedmalloc.c"
#include "KgeMemoryTrack.h"

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Allocate bytes
		//------------------------------------------------------------------------------------
		void* KgeNedmalloc::AllocateBytes(u32 BytesCount,  char* file, int line, const char* function)
		{
			// Allocate bytes
			void* ret = nedalloc::nedmalloc(BytesCount);

			// Track memory
			KgeMemoryTrack::AddMemory(ret, BytesCount, file, line, function);

			return ret;

		} // AllocateBytes

		//------------------------------------------------------------------------------------
		// Deallocate bytes
		//------------------------------------------------------------------------------------
		void KgeNedmalloc::DeallocateBytes(void* ptr)
		{
			// Free memory
			nedalloc::nedfree(ptr);

			// Remove memory from tracker
			KgeMemoryTrack::RemoveMemory(ptr);

		} // DeallocateBytes

	} // core

} // kge
