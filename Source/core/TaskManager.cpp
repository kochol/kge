#include "../../Include/core/TaskManager.h"
#include "../../Include/core/TaskManagerThread.h"
#include "../../Include/core/Task.h"
#include "../../Include/core/CPU.h"
#include "../../Include/core/KgeMemory.h"
#include "../../Include/core/KgeMutex.h"

KGE_API kge::core::TaskManager	*	g_pTaskManager = NULL;

namespace kge
{
namespace core
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TaskManager::TaskManager()
	{
		g_pTaskManager = this;

		// Create thread as CPU cores count
		m_iThreadCount = CPU::GetCPUCount();
		m_pThreads = KGE_NEW_CLASS_ARRAY(TaskManagerThread, m_iThreadCount);
		for (int i = 0; i < m_iThreadCount; i++)
		{
			m_pThreads[i].Begin();
		}

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TaskManager::~TaskManager()
	{
		g_pTaskManager = NULL;

		// Delete threads
		KGE_DELETE_CLASS_ARRAY(m_pThreads, TaskManagerThread, m_iThreadCount);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Returns the TaskManager singleton pointer
	//------------------------------------------------------------------------------------
	TaskManager* TaskManager::GetSingletonPointer()
	{
		if (!g_pTaskManager)
			g_pTaskManager = KGE_NEW(TaskManager);

		return g_pTaskManager;

	} // GetSingletonPointer

	//------------------------------------------------------------------------------------
	// Adds a Task to process
	//------------------------------------------------------------------------------------
	void TaskManager::AddTask( Task* pTask )
	{
		if (!pTask)
			return;

		// Find a thread that has less task.
		TaskManagerThread* p = NULL;
		int mintask = MAXID;
		for (int i = 0; i < m_iThreadCount; i++)
		{
			if (m_pThreads[i].GetTaskCount() < mintask)
			{
				mintask = m_pThreads[i].GetTaskCount();
				p = &m_pThreads[i];
			}
		}

		p->AddTask(pTask);

	} // AddTask

} // core

} // kge
