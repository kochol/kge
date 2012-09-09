#ifndef KGE_PROFILERBLOCK_H
#define KGE_PROFILERBLOCK_H

#include "pdef.h"
#include "String.h"

namespace kge
{
	//! The block that store block data for each function
	class PKGE_API ProfilerBlock
	{
	public:

		//! Constructor
		ProfilerBlock(const core::stringc& name);

		//! Destructor
		~ProfilerBlock();

		//! Tell the profiler when frame ended and calculate the average and maximum time
		void EndFrame();

		//! Adds a profiler block data
		void AddBlockData(const int& iTime);

	protected:

		core::stringc		m_sName;			//!< The Block name
		int					m_iTotalCall,		//!< Total Call from begining
							m_iFrameCall,		//!< Total call in last frame
							m_iAveFrameCall,	//!< Average call count in frames
							m_iMaxFrameCall,	//!< The maximum the function called in a single frame
							m_iTotalTime,		//!< Total time that this function takes from begining
							m_iFrameTime,		//!< Total time that takes in last frame
							m_iAveFrameTime,	//!< Average time that takes in a frame
							m_iMaxFrameTime,	//!< The maximum frame time
							m_iMaxTime,			//!< The maximum time
							m_iAveTime;			//!< Average time the function takes;

	}; // ProfilerBlock

} // kge

#endif // KGE_PROFILERBLOCK_H
