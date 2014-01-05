#pragma once

#include <queue>
#include "KgeMutex.h"
#include "KgeCond.h"

namespace kge
{
namespace core
{
	//! A thread safe queue
	template<typename Data>
	class TSQueue
	{
	public:

		//! Push an element to the queue
		void push(Data const& data)
		{
			m_mutex.On();
			m_queue.push(data);
			m_mutex.Off();
			m_cond.Signal();
		}

		//! Is queue empty?
		bool empty() const
		{
			m_mutex.On();
			return m_queue.empty();
			m_mutex.Off();
		}

		//! Try to pop an element.
		//! \return Returns false when queue is empty
		bool try_pop(Data& popped_value)
		{
			m_mutex.On();
			if(m_queue.empty())
			{
				return false;
			}

			popped_value=m_queue.front();
			m_queue.pop();
			m_mutex.Off();
			return true;
		}

		//! Pop an element from queue and if queue is empty then waits for an element to come.
		void wait_and_pop(Data& popped_value)
		{
			m_mutex.On();
			while(m_queue.empty())
			{
				m_cond.Wait(m_mutex);
			}

			popped_value=m_queue.front();
			m_queue.pop();
			m_mutex.Off();
		}

	private:

		std::queue<Data>	m_queue;
		KgeMutex			m_mutex;
		KgeCond				m_cond;

	}; // TSQueue

} // core

} // kge
