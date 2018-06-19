#include <SDL2/SDL_mutex.h>
#include "../../include/kge/core/KgeCond.h"
#include "../../include/kge/core/KgeMutex.h"

namespace kge
{
namespace core
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	KgeCond::KgeCond()
	{
		m_pCond = SDL_CreateCond();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	KgeCond::~KgeCond()
	{
		SDL_DestroyCond(m_pCond);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Wait on the condition variable, unlocking the provided mutex.
	//------------------------------------------------------------------------------------
	void KgeCond::Wait( KgeMutex* pMutex )
	{
		SDL_CondWait(m_pCond, pMutex->m_pSdlMutex);

	} // Wait

	//------------------------------------------------------------------------------------
	// Restart one of the threads that are waiting on the condition variable.
	//------------------------------------------------------------------------------------
	void KgeCond::Signal()
	{
		SDL_CondSignal(m_pCond);

	} // Signal

	//------------------------------------------------------------------------------------
	// Restart all threads that are waiting on the condition variable.
	//------------------------------------------------------------------------------------
	void KgeCond::Broadcast()
	{
		SDL_CondBroadcast(m_pCond);

	} // Broadcast

} // core

} // kge
