#include "../../Include/core/TaskManagerThread.h"
#include "../../Include/core/Task.h"
#include "../../Include/core/KgeMemory.h"
#include "../../Include/core/KgeMutex.h"

namespace kge
{
namespace core
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TaskManagerThread::TaskManagerThread(): m_iTaskCount(0),  m_bThreadStop(true)
	{
		m_pMutex = KGE_NEW(KgeMutex);
		m_pMutex->On();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TaskManagerThread::~TaskManagerThread()
	{
		KGE_DELETE(m_pMutex, KgeMutex);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Thread function for internal use
	//------------------------------------------------------------------------------------
	int TaskManagerThread::ThreadProc()
	{
		while (m_bIsRunning)
		{
			m_pMutex->On();
			while (m_iTaskCount)
			{

			}
			m_pMutex->Off();
			
			// Tell the Main thread to stop this thread
		}

		return 0;

	} // ThreadProc

	//------------------------------------------------------------------------------------
	// Adds a new task to queue
	//------------------------------------------------------------------------------------
	int TaskManagerThread::AddTask( Task* pTask )
	{
		m_iTaskCount++;
		if (m_bThreadStop)
		{
			m_bThreadStop = false;
			m_pMutex->Off();
		}

		return m_iTaskCount;

	} // AddTask

} // core

} // kge
