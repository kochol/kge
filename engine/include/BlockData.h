#pragma once

#include "ProfilerBlock.h"
#include "String.h"
#include "Timer.h"
#include "Profiler.h"

namespace kge
{
	class BlockData
	{
	public:

		//! Constructor
		BlockData(core::stringc name): m_Name(name)
		{
			m_pBlock = Profiler::GetPointer()->GetBlock(name);
		}

		//! Destructor
		~BlockData()
		{
			m_pBlock->AddBlockData(m_Timer->GetTime(false));
		}

	protected:

		core::Timer				m_Timer;
		core::stringc			m_Name;
		ProfilerBlock		*	m_pBlock;

	}; // BlockData

} // kge
