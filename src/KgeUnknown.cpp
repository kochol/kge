#include "../include/kge/KgeUnknown.h"

#ifdef WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

namespace kge
{
	void KgeUnknown::AddRef()
	{
#if ((KGE_PLATFORM == KGE_PLATFORM_WINDOWS) && (KGE_COMPILER == KGE_COMPILER_MSVC))
		InterlockedIncrement((LONG*)&m_iRefrenceCount);
#else
		++m_iRefrenceCount;
#endif
	} // AddRef

	void KgeUnknown::DecRef()
	{
 #if ((KGE_PLATFORM == KGE_PLATFORM_WINDOWS) && (KGE_COMPILER == KGE_COMPILER_MSVC))
		InterlockedDecrement((LONG*)&m_iRefrenceCount);
#else
		--m_iRefrenceCount;
#endif
		if (m_iRefrenceCount == 0)
		{
			KGE_DELETETHIS(this, KgeUnknown);
		}

	} // DecRef
}