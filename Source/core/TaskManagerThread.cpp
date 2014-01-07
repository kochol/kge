#include "../../Include/core/TaskManagerThread.h"
#include "../../Include/core/TaskManager.h"
#include "../../Include/core/Task.h"
#include "../../Include/core/KgeMemory.h"
#include "../../Include/core/KgeMutex.h"
#include "../../Include/io/Logger.h"

extern kge::core::TaskManager	*	g_pTaskManager;

namespace kge
{
namespace core
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TaskManagerThread::TaskManagerThread()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TaskManagerThread::~TaskManagerThread()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Thread function for internal use
	//------------------------------------------------------------------------------------
	int TaskManagerThread::ThreadProc()
	{
		io::Logger::Debug("TaskManagerThread Begin");
		Task* pTask = NULL;
		while (m_bIsRunning)
		{
			m_TaskQueue.wait_and_pop(pTask);
			g_pTaskManager->AddTask(pTask->Do());
		}

		return 0;

	} // ThreadProc

	//------------------------------------------------------------------------------------
	// Adds a new task to queue
	//------------------------------------------------------------------------------------
	int TaskManagerThread::AddTask( Task* pTask )
	{
		m_TaskQueue.push(pTask);

		return m_TaskQueue.size();

	} // AddTask

} // core

} // kge
