#include <cstdio>
#include <map>
#include "../../Headers/core/KgeMemoryTrack.h"
#include "../../include/kge/core/KgeMemory.h"
#include "../../include/kge/config.h"
#include "../../include/kge/io/Logger.h"

namespace kge
{
	namespace core
	{

		static std::map<size_t, MemoryInfo*>		g_sMemory;

		//------------------------------------------------------------------------------------
		// Add memory
		//------------------------------------------------------------------------------------
		void KgeMemoryTrack::AddMemory(void* ptr, u32 BytesCount, const char* file, int line, const char* function)
		{
#		ifdef KGE_TRACK_MEMORY
			static u32 AllocNo = 0;
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
		bool KgeMemoryTrack::RemoveMemory(void* ptr)
		{
#		ifdef KGE_TRACK_MEMORY
			std::map<size_t, MemoryInfo*>::iterator it = g_sMemory.find((size_t)ptr);
			if (it == g_sMemory.end())
			{
				io::Logger::Log(io::ELM_Error, "The %d pointer is not allocated with KGE_NEW or it was deallocated already.", (size_t)ptr);
				return false;
			}

			delete it->second;
			g_sMemory.erase(it);

#		endif // KGE_TRACK_MEMORY

			return true;

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
			int bytes = 0;
			io::Logger::Log(io::ELM_Information, "No   \tAddress\t\tsize\tfunction            \tline\tfile");
			for (it = g_sMemory.begin(); it != g_sMemory.end(); it++)
			{
				mi = it->second;
				bytes += mi->BytesCount;
				io::Logger::Log(io::ELM_Information, "%05d\t0x%08X\t%d\t%-20s\t%d\t%s", mi->AllocNo, mi->Data, mi->BytesCount,
					mi->Function, mi->Line, mi->FileName);
			}
			io::Logger::Warning("%d memory leaks found.\n%d Bytes in size.", g_sMemory.size(), bytes);
#		endif

		} // LogMemoryLeaks

	} // core

} // kge
