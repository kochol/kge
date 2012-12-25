#include "../include/Profiler.h"
#include "../include/ProfilerBlock.h"
#include "../include/KgeMemory.h"

namespace kge
{
	namespace core
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Profiler::Profiler()
		{

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Profiler::~Profiler()
		{

		} // Destructor

		//------------------------------------------------------------------------------------
		// Return the profiler pointer
		//------------------------------------------------------------------------------------
		Profiler* Profiler::GetPointer()
		{
			static Profiler sp;

			return &sp;

		} // GetPointer

		//------------------------------------------------------------------------------------
		// Returns the block with its name
		//------------------------------------------------------------------------------------
		ProfilerBlock* Profiler::GetBlock( const core::stringc& sName )
		{
			std::map<core::stringc, ProfilerBlock*>::iterator it;
			it = m_mBlocks.find(sName);

			if (it != m_mBlocks.end())
				return it->second;

			// Create a new block
			ProfilerBlock*	pPB = KGE_NEW(ProfilerBlock)(sName);

			m_mBlocks.insert(std::pair<core::stringc, ProfilerBlock*>(sName, pPB));

			return pPB;

		} // GetBlock

		//------------------------------------------------------------------------------------
		// Returns the string with profiling data.
		//------------------------------------------------------------------------------------
		core::stringc Profiler::GetData()
		{
			core::stringc str;
			std::map<core::stringc, ProfilerBlock*>::iterator it;

			for (it = m_mBlocks.begin(); it != m_mBlocks.end(); it++)
			{
				it->second->EndFrame();
				it->second->GetData(str);
			}

			return str;

		} // GetData

		//------------------------------------------------------------------------------------
		// Release the memory taken
		//------------------------------------------------------------------------------------
		void Profiler::Release()
		{
			std::map<core::stringc, ProfilerBlock*>::iterator it;

			for (it = m_mBlocks.begin(); it != m_mBlocks.end(); it++)
			{
				KGE_DELETE(it->second, ProfilerBlock)
			}

			m_mBlocks.clear();

		} // Release

	} // core

} // kge
