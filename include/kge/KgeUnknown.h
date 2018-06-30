// File name: KgeUnknown.h
// Des: This class is a base class for all of the KGE classes. We use this class for refrence counting and storing debug informations.
// Date: 12/4/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_KGEUNKNOWN_H
#define KGE_KGEUNKNOWN_H

#include "kgedef.h"
#include "core/KgeMemory.h"


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
	KgeUnknown():
		m_iRefrenceCount(1), m_pDebugText(NULL)
	{
	} // Constructor

	//! Destructor
	virtual ~KgeUnknown() 
	{
		if (m_pDebugText)
			delete[] m_pDebugText;

	} // Destructor

	//! Add a reference to the object.
	virtual void AddRef();

	//! Decrease a reference from the object. if its reference count get 0 then the object will destroy.
	virtual void DecRef();

	//! Return resource reference count
	u32 GetRefCount() { return m_iRefrenceCount; }

	//! returns the instance debug text.
	const char* GetDebugText()
	{
		return m_pDebugText;

	} // GetDebugText

protected:

	/*! sets the instance debug text.
	\param text The function don't copy it but store it pointer and delete it on destructor.
	*/
	void SetDebugText(char* text)
	{
		m_pDebugText = text;

	} // SetDebugText

private:

	u32     m_iRefrenceCount;	// Reference count
	char*   m_pDebugText;		// Debug text

}; // KgeUnknown

} // kge

#endif // KGE_KGEUNKNOWN_H
