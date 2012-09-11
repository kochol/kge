#ifndef KGE_PROFILER_H
#define KGE_PROFILER_H

#include "kgedef.h"
#include <map>
#include "String.h"

namespace kge
{
	class ProfilerBlock;

	//! The profiler interface for profiling the functions.
	class KGE_API Profiler
	{
	protected:

		//! Constructor
		Profiler();

	public:

		//! Destructor
		~Profiler();

		//! Return the profiler pointer
		static Profiler*	GetPointer();

		//! Returns the block with its name
		ProfilerBlock*		GetBlock(const core::stringc& sName);

		//! Returns the string with profiling data.
		core::stringc GetData();

	protected:

		std::map<core::stringc, ProfilerBlock*>		m_mBlocks;

	}; // Profiler

} // kge

#endif // KGE_PROFILER_H
