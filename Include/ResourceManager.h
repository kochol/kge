// File name: ResourceManager.h
// Des: This class is for managing resources in KGE
// Date: 14/6/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_RESOURCEMANAGER_H
#define KGE_RESOURCEMANAGER_H

#include <vector>
#include <stack>
#include "Resource.h"
#include "core/Functor.h"
#include "Loader.h"
#include "io/FileSystemManager.h"
#include "io/Stream.h"

namespace kge
{

#if ((KGE_PLATFORM == KGE_PLATFORM_WINDOWS) && (KGE_COMPILER == KGE_COMPILER_MSVC))
#define STR_COMPARE _stricmp
#else
#define STR_COMPARE strcmp
#endif

//! Resource manager take care of loading and unloading resources.
template <class T>
class ResourceManager: public KgeUnknown
{
public:

	//! Constructor
	ResourceManager(core::Functor5<bool , Resource** , const u32 , const char*  , const char* , 
		void* >*	CreateResourceFunction)
	{
		m_pCreateResourceFn = CreateResourceFunction;

	} // Constructor

	//! Destructor
	virtual ~ResourceManager()
	{
		// Searching for resource.
		for(std::vector<T*>::iterator it = m_vResources.begin(); 
			it != m_vResources.end(); it++)
		{
			T* t = (*it);
			if (t)
			{
				t->~T();
				core::KgeMemory::DeallocateBytes(t);
			}
		}

		delete m_pCreateResourceFn;

	} // Destructor

	//! Loads or find a resource and return its handle.
	virtual u32 Add(const char* FileName, void* ExtraParams, const char* Name)
	{
		// Searching for resource.
		for(std::vector<T*>::iterator it = m_vResources.begin(); 
			it != m_vResources.end(); it++)
		{
			if ((*it))
			{
				if ((*it)->GetFileName() && (*it)->GetName() && FileName && Name)
				{
					if ( !STR_COMPARE((*it)->GetFileName(), FileName) && !STR_COMPARE((*it)->GetName(), Name) )
					{
						(*it)->AddRef();
						return (*it)->GetHandle();
					}
				}
				else if ((*it)->GetFileName() && FileName)
				{
					if ( !STR_COMPARE((*it)->GetFileName(), FileName) )
					{
						(*it)->AddRef();
						return (*it)->GetHandle();
					}
				}
			}
		}

		// Resource not loaded yet.
		T* pResource = NULL;
		u32 handle;
		bool add;
		if (m_sHandles.size() > 0)
		{
			handle = m_sHandles.top();
			m_sHandles.pop();
			add	   = false;
		}
		else
		{
			handle = (u32)m_vResources.size();
			add    = true;
		}

		if (m_pCreateResourceFn)
		{
			m_pCreateResourceFn->Call((Resource**)&pResource, handle, FileName, Name, ExtraParams);
		}
		else
		{
			pResource = new T(handle, FileName, Name, NULL);
		}

		pResource->AddRef();

		// Add it to array
		if (add)
			m_vResources.push_back(pResource);
		else
			m_vResources[handle] = pResource;

		return handle;

	} // Add

	//! Loads or find a resource and return its handle.
	virtual u32 Add2(const char* FileName, const char* pCh, void* ExtraParams, const char* Name)
	{
		// Searching for resource.
		for(std::vector<T*>::iterator it = m_vResources.begin(); 
			it != m_vResources.end(); it++)
		{
			if ((*it))
			{
				if ((*it)->GetFileName() && (*it)->GetName() && FileName && Name)
				{
					if ( !STR_COMPARE((*it)->GetFileName(), FileName) && !STR_COMPARE((*it)->GetName(), Name) )
					{
						(*it)->AddRef();
						return (*it)->GetHandle();
					}
				}
				else if ((*it)->GetFileName() && FileName)
				{
					if ( !STR_COMPARE((*it)->GetFileName(), FileName) )
					{
						(*it)->AddRef();
						return (*it)->GetHandle();
					}
				}
			}
		}

		// Resource not loaded yet.
		T* pResource = NULL;
		u32 handle;
		bool add;
		if (m_sHandles.size() > 0)
		{
			handle = m_sHandles.top();
			m_sHandles.pop();
			add	   = false;
		}
		else
		{
			handle = (u32)m_vResources.size();
			add    = true;
		}

		if (m_pCreateResourceFn)
		{
			m_pCreateResourceFn->Call((Resource**)&pResource, handle, FileName, pCh, ExtraParams);
		}
		else
		{
			pResource = new T(handle, FileName, Name, NULL);
		}

		pResource->AddRef();

		// Add it to array
		if (add)
			m_vResources.push_back(pResource);
		else
			m_vResources[handle] = pResource;

		return handle;

	} // Add2

	//! Adds a manual resource and returns its handle.
	virtual u32 Add(T* pResource)
	{
		u32 handle;
		if (m_sHandles.size() > 0)
		{
			handle = m_sHandles.top();
			m_sHandles.pop();
		}
		else
			handle = (u32)m_vResources.size();
		((Resource*)pResource)->m_iHandle = handle;

		if (pResource->GetHandle() >= m_vResources.size())
		{
			m_vResources.push_back(pResource);
			if (pResource->GetHandle() != m_vResources.size() - 1)
				pResource->m_iHandle = (u32)m_vResources.size() - 1;
		}
		else
			m_vResources[pResource->GetHandle()] = pResource;

			pResource->m_pMyMgr = (void*)this;

			return pResource->GetHandle();

	} // AddManualResource

	//! Removes a resource from list. 
	//! Dose not delete the Resource only remove it from list
	void RemoveResource(T* pResource)
	{
		if (pResource->GetHandle() < m_vResources.size())
		{
			m_sHandles.push(pResource->GetHandle());
			m_vResources[pResource->GetHandle()] = NULL;
		}

	} // RemoveResource

	//! Returns a resource.
	virtual T* GetResource(u32 handle)
	{
		return m_vResources[handle];

	} // GetResource

	//! returns resource handle before creating resource then use Add(Resource*, u32 Handel) function
	//! to set the loaded resource.
	u32 GetNewHandle()
	{
		u32 handle;
		if (m_sHandles.size() > 0)
		{
			handle = m_sHandles.top();
			m_sHandles.pop();
		}
		else
			handle = (u32)m_vResources.size();

		return handle;

	} // GetNewHandle

	//! Register resource loaders for this loaders.
	void RegisterLoader(Loader* pLoader)
	{
		m_vLoaders.push_back(pLoader);
	}

	//! Loads the resource and return its pointer
	T* Load(const char* FileName, void* ExtraParams, const char* Name)
	{
		// Searching for resource.
		for(typename std::vector<T*>::iterator it = m_vResources.begin();
			it != m_vResources.end(); it++)
		{
			if ((*it))
			{
				if ((*it)->GetFileName() && (*it)->GetName() && FileName && Name)
				{
					if ( !strcmp((*it)->GetFileName(), FileName) && !strcmp((*it)->GetName(), Name) )
					{
						(*it)->AddRef();
						return (*it);
					}
				}
				else if ((*it)->GetFileName() && FileName)
				{
					if ( !strcmp((*it)->GetFileName(), FileName) )
					{
						(*it)->AddRef();
						return (*it);
					}
				}
			}
		}

		// Resource not loaded yet.
		T* pResource = NULL;
		u32 handle;
		bool Add;
		if (m_sHandles.size() > 0)
		{
			handle = m_sHandles.top();
			Add	   = false;
		}
		else
		{
			handle = (u32)m_vResources.size();
			Add    = true;
		}

		// Ask file system to load this stream
		io::Stream* pStream = io::FileSystemManager::getSingletonPtr()->Load(std::string(FileName));
		if (!pStream)
			return NULL;

		// Ask loaders if any one can load this resource
		for (size_t i = 0; i < m_vLoaders.size(); i++)
		{
			if (!m_vLoaders[i]->IsALoadableFileExtension(core::String(FileName)))
				continue;

			pResource = (T*)m_vLoaders[i]->LoadResource(pStream);
			if (pResource)
				break;
		}

		KGE_DELETE(pStream, Stream);

		if (!pResource)
			return NULL;

		pResource->AddRef();

		// Add it to array
		if (Add)
			m_vResources.push_back(pResource);
		else
		{
			m_vResources[handle] = pResource;
			m_sHandles.pop();
		}

		pResource->m_pMyMgr = (void*)this;
		((Resource*)pResource)->m_iHandle = handle;

		return pResource;

	} // Load

protected:

	std::vector<T*>			m_vResources;		/**< Stores the resources */
	std::stack<u32>			m_sHandles;			/**< Stores the unused handles number*/
	core::Functor5<bool , Resource** , const u32 , const char* , const char* , 
		void* >*			m_pCreateResourceFn;/**< Pointer to the function which creats the resource. */
	std::vector<Loader*>	m_vLoaders;			//!< Stores the resource loaders

}; // ResourceManager

} // kge

#endif // KGE_RESOURCEMANAGER_H
