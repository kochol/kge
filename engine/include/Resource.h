#ifndef KGE_RESOURCE_H
#define KGE_RESOURCE_H

#include "kgedef.h"
#include "KgeUnknown.h"

namespace kge
{
	//! Abstract class for resources like textures, meshes, sounds, shaders, ...
	class KGE_API Resource: public KgeUnknown
	{
		template <class T>
		friend class ResourceManager;

	public:

		//! Constructor
		Resource();

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

		//! Is resource loaded or not?
		bool IsLoaded() { return m_bIsLoaded; }

	protected:

		char*	m_pFileName;			/**< Resource File Name */
		char*	m_pName;				/**< Resource Name */
		void*	m_pExtraParam;			/**< Extra parameter */
		u32		m_iHandle;				/**< Resource Handle */
		bool	m_bIsLoaded;			//!< Is resource loaded and ready to use?
		bool	m_bLoadOnBackground;	//!< Load resource on a background thread or not?
		void*	m_pMyMgr;				//!< The resource manager pointer.

	}; // Resource

} // kge

#endif // KGE_RESOURCE_H
