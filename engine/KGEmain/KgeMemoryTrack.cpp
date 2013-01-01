#include <map>
#include "KgeMemoryTrack.h"
#include "../include/KgeMemory.h"
#include "../include/Config.h"
#include "../include/Logger.h"

namespace kge
{
	namespace core
	{

		static std::map<size_t, MemoryInfo*>		g_sMemory;

		//------------------------------------------------------------------------------------
		// Add memory
		//------------------------------------------------------------------------------------
		void KgeMemoryTrack::AddMemory(void* ptr, u32 BytesCount,  char* file, int line, const char* function)
		{
#		ifdef KGE_TRACK_MEMORY
			static int AllocNo = 0;
			AllocNo++;
			MemoryInfo* mi	= new MemoryInfo;
			mi->Data		= ptr;
			mi->BytesCount	= BytesCount;
			mi->FileName	= file;
			mi->Line		= line;
			mi->Function	= function;
			mi->AllocNo     = AllocNo;
			g_sMemory[(size_t)ptr]  = mi;

#		endif // KGE_TRACK_MEMORY

		} // AddMemory

		//------------------------------------------------------------------------------------
		// Removes memory
		//------------------------------------------------------------------------------------
		void KgeMemoryTrack::RemoveMemory(void* ptr)
		{
#		ifdef KGE_TRACK_MEMORY
			std::map<size_t, MemoryInfo*>::iterator it = g_sMemory.find((size_t)ptr);
			if (it == g_sMemory.end())
			{
				io::Logger::Error("The %d pointer is not allocated with KGE_NEW or it was deallocated allready.", (size_t)ptr);
				return;
			}

			delete it->second;
			g_sMemory.erase(it);

#		endif // KGE_TRACK_MEMORY

		} // RemoveMemory

		//------------------------------------------------------------------------------------
		// Logs memory leakages
		//------------------------------------------------------------------------------------
		void KgeMemoryTrack::LogMemoryLeaks()
		{
#		ifdef KGE_TRACK_MEMORY
			if (g_sMemory.size() == 0)
			{
				io::Logger::SuccessKGE("Congratulation no memory leaks found :)");
				return;
			}
			std::map<size_t, MemoryInfo*>::iterator it;
			MemoryInfo* mi;
			io::Logger::Error("***************************\nWriting the memory leaks\n***************************\n");
			io::Logger::Info("No   \tAddress\t\tsize\tfunction            \tline\tfile");
			for (it = g_sMemory.begin(); it != g_sMemory.end(); it++)
			{
				mi = it->second;
				io::Logger::Info("%05d\t0x%08X\t%d\t%-20s\t%d\t%s", mi->AllocNo, mi->Data, mi->BytesCount,
					mi->Function, mi->Line, mi->FileName);
			}
#		endif

		} // LogMemoryLeaks

	} // core

} // kge
