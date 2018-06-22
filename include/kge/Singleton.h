// File name: KgeSingleton.h
// Des: This class is for classes that you want have one instance per application.
// Date: 30/12/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_SINGLETON_H
#define KGE_SINGLETON_H

#include <assert.h>
#include "kgedef.h"


#if KGE_COMPAILER == KGE_COMPILER_GNUC
	#pragma GCC visibility push(default)
#endif

namespace kge
{
	//! The singleton class. Maybe singleton design pattern is not good any more
	//! And we need to delete singleton from engine soon.
	template <typename T>
	class KGE_API Singleton
	{
	public:

		//! Constructor
		Singleton() 
		{
			assert(!m_pSingleton);
			m_pSingleton = static_cast<T*>(this);

		} // Constructor

		//! Destructor
		~Singleton()
		{
			assert(m_pSingleton);
			m_pSingleton = 0;

		} // Destructor

	protected:

		static T* m_pSingleton;

	}; // KgeSingleton

} // kge

#if KGE_COMPAILER == KGE_COMPILER_GNUC  
	#pragma GCC visibility pop
#endif

#endif // KGE_SINGLETON_H
