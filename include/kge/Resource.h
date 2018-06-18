// File name: Resource.h
// Des: This class is a base class of resources which will be used in KGE
// Date: 14/6/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_RESOURCE_H
#define KGE_RESOURCE_H

#include "KgeUnknown.h"
#include "kgedef.h"

namespace kge
{

//! Abstract class for resources like textures, meshes, sounds, shaders, ...
class KGE_API Resource: public KgeUnknown
{
	template <class T>
	friend class ResourceManager;

public:

	//! Constructor
	Resource(const u32 Handle, const char* FileName, const char* Name, void* ExtraParam);

	//! Destructor
	virtual ~Resource();

	//! Return resource file name
	virtual const char* GetFileName() { return m_pFileName; }

	//! Return resource name
	virtual const char* GetName() { return m_pName; }

	//! Return resource handle
	virtual u32 GetHandle() { return m_iHandle; }

protected:

	char*	m_pFileName;	/**< Resource File Name */
	char*	m_pName;		/**< Resource Name */
	void*	m_pExtraParam;	/**< Extra parameter */
	u32		m_iHandle;		/**< Resource Handle */
	void*	m_pMyMgr;		//!< Resource manager pointer

}; // Resource

} // kge

#endif // KGE_RESOURCE_H
