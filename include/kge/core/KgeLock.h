#pragma once

#include "KgeMutex.h"

namespace kge
{
namespace core
{
	class KgeLock
	{
	public:
	
		//! Constructor
		KgeLock(KgeMutex* pMutex):m_pMutex(pMutex) {m_pMutex->On();}
	
		//! Destructor
		~KgeLock() {m_pMutex->Off();}
	
	protected:
	
		KgeMutex*	m_pMutex;
		
	}; // KgeLock

} // core

} // kge
