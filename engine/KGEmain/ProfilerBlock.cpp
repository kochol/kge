#include "../include/ProfilerBlock.h"
#include <stdio.h>

namespace kge
{
	namespace core
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		ProfilerBlock::ProfilerBlock( const core::stringc& name ):
	m_sName(name),
		m_iTotalCall(0),
		m_iTotalFrame(0),
		m_iFrameCall(0),
		m_iAveFrameCall(0),
		m_iMaxFrameCall(0),
		m_iTotalTime(0),
		m_iFrameTime(0),
		m_iAveFrameTime(0),
		m_iMaxFrameTime(0),
		m_iMaxTime(0),
		m_iAveTime(0)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	ProfilerBlock::~ProfilerBlock()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Tell the profiler when frame ended and calculate the average and maximum time
	//------------------------------------------------------------------------------------
	void ProfilerBlock::EndFrame()
	{
		m_iTotalFrame++;
		m_iAveFrameCall = m_iTotalCall / m_iTotalFrame;
		if (m_iFrameCall > m_iMaxFrameCall)
			m_iMaxFrameCall = m_iFrameCall;
		m_iAveFrameTime = m_iTotalTime / m_iTotalFrame;
		if (m_iFrameTime > m_iMaxFrameTime)
			m_iMaxFrameTime = m_iFrameTime;
		m_iAveTime = m_iTotalTime / m_iTotalCall;

		m_iFrameCall = m_iFrameTime = 0;

	} // EndFrame

	//------------------------------------------------------------------------------------
	// Adds a profiler block data
	//------------------------------------------------------------------------------------
	void ProfilerBlock::AddBlockData( const int& iTime )
	{
		m_iTotalCall++;
		m_iFrameCall++;
		m_iTotalTime += iTime;
		m_iFrameTime += iTime;
		if (iTime > m_iMaxTime)
			m_iMaxTime = iTime;

	} // AddBlockData

	//------------------------------------------------------------------------------------
	// Appends the data to the string
	//------------------------------------------------------------------------------------
	void ProfilerBlock::GetData( core::stringc &str )
	{
		char ch[256];
		sprintf(ch, "%s %d %d %d %d\n",
			m_sName.c_str(),
			m_iTotalCall,
			m_iTotalTime,
			m_iAveTime,
			m_iMaxTime
			);
		str += ch;

	} // GetData

	} // core

} // kge
