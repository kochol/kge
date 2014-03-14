#ifndef KGE_KGEMEMORYTRACK_H
#define KGE_KGEMEMORYTRACK_H

#include "../../Include/kgedef.h"

namespace kge
{
	namespace core
	{
		struct MemoryInfo
		{
			void*			Data;
			u32				BytesCount;
			const char*		FileName;
			const char*		Function;
			int				Line;
			u32				AllocNo;

		}; // MemoryInfo

		//! Tracks the memory for memory leaks
		class KGE_API KgeMemoryTrack
		{
		public:

			//! Add memory
			static void AddMemory(void* ptr, u32 BytesCount, const char* file = 0, int line = 0, const char* function = 0);
		
			//! Removes memory
			static bool RemoveMemory(void* ptr);

			//! Logs memory leakages
			static void LogMemoryLeaks();

		protected:

		}; // KgeMemoryTrack

	} // core
} // kge

#endif // KGE_KGEMEMORYTRACK_H
