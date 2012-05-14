#ifndef KGE_KGEUNKNOWN_H
#define KGE_KGEUNKNOWN_H

#include "kgedef.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <string>

#ifndef NULL
#define NULL 0
#endif // NULL

namespace kge
{
    //! This class is for refrence counting and lock and unlocking object for multi thread purpose.
    class KGE_API KgeUnknown
    {
    public:

        //! Constructor
        KgeUnknown(): m_uRefCount(1), m_bLocked(false) {}

        //! Destructor
        virtual ~KgeUnknown() {}

        //! Adds a refrence and return refrenc count
        u32 AddRef() 
		{
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
        InterlockedIncrement((volatile LONG*)&m_uRefCount);
		return m_uRefCount;
#else
			return ++m_uRefCount;
#endif
		} // AddRef

        //! Decrese a refrence count and return refrence count. Deletes object when refrence count becomes 0
        u32 DecRef()
        {
#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
        InterlockedDecrement((volatile LONG*)&m_uRefCount);
#else
            --m_uRefCount;
#endif
            if (m_uRefCount == 0)
            {
                delete this;
                return 0;
            }
            return m_uRefCount;

        } // DecRef

        //! Locks the object returns true if succed
        bool Lock()
        {
            if (m_bLocked)
                return false;
            m_bLocked = true;
            return true;

        } // Lock

        //! Returns true if object is locked
        bool IsLocked() {return m_bLocked;}

        //! Unlock the object
        void UnLock() {m_bLocked = false;}
		
		//! Returns instance debug text
		const std::string GetDebugText() { return m_sDebugText; }

    protected:

		/*! sets the instance debug text.
		\param text The instace debug text.
		*/
        void SetDebugText(char* name) {m_sDebugText = name;}

        u32						m_uRefCount;        //!< Refrence count
        bool                    m_bLocked;          //!< Check for object locking
		std::string				m_sDebugText;       //!< Class debug name

    }; // KgeUnknown

} // kge

#endif // KGE_KGEUNKNOWN_H
