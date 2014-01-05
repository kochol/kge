#include "../../Headers/core/KgeNedmalloc.h"
#include "../../Libs/nedmalloc/nedmalloc.c"
#include "../../Headers/core/KgeMemoryTrack.h"

namespace kge
{
	namespace core
	{
		//------------------------------------------------------------------------------------
		// Allocate bytes
		//------------------------------------------------------------------------------------
		void* KgeNedmalloc::AllocateBytes(u32 BytesCount,  char* file, int line, char* function)
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
			// Remove memory from tracker
			KgeMemoryTrack::RemoveMemory(ptr);

			// Free memory
			nedalloc::nedfree(ptr);

		} // DeallocateBytes

		//------------------------------------------------------------------------------------
		// Reallocate bytes
		//------------------------------------------------------------------------------------
		void* KgeNedmalloc::ReallocateBytes( void* ptr, u32 BytesCount, char* file /*= 0*/, int line /*= 0*/, char* function /*= 0*/ )
		{
			// Allocate bytes
			void* ret = nedalloc::nedrealloc(ptr, BytesCount);

			// Track memory
			KgeMemoryTrack::RemoveMemory(ptr);
			KgeMemoryTrack::AddMemory(ret, BytesCount, file, line, function);

			return ret;

		} // ReallocateBytes

	} // core

} // kge
