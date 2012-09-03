#ifndef KGE_RESOURCEMANAGER_H
#define KGE_RESOURCEMANAGER_H

#include <vector>
#include <stack>
#include "Resource.h"
#include "Functor.h"
#include "KgeMemory.h"

namespace kge
{
	//! Resource manager take care of loading and unloading resources.
	template <class T>
	class KGE_API ResourceManager
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
				if ((*it))
				{
					KGE_DELETE((*it), T);
					(*it) = NULL;
				}
			}

			m_vResources.clear();

			if (m_pCreateResourceFn)
			{
				delete m_pCreateResourceFn;
				m_pCreateResourceFn = NULL;
			}

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
						if ( !strcmp((*it)->GetFileName(), FileName) && !strcmp((*it)->GetName(), Name) )
						{
							(*it)->AddRef();
							return (*it)->GetHandle();
						}
					}
					else if ((*it)->GetFileName() && FileName)
					{
						if ( !strcmp((*it)->GetFileName(), FileName) )
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
			bool Add;
			if (m_sHandles.size() > 0)
			{
				handle = m_sHandles.top();
				m_sHandles.pop();
				Add	   = false;
			}
			else
			{
				handle = (u32)m_vResources.size();
				Add    = true;
			}

			if (m_pCreateResourceFn)
			{
				m_pCreateResourceFn->Call((Resource**)&pResource, handle, FileName, Name, ExtraParams);
			}
			else
			{
				pResource = KGE_NEW(T)(handle, FileName, Name, NULL);
			}

			pResource->AddRef();

			// Add it to array
			if (Add)
				m_vResources.push_back(pResource);
			else
				m_vResources[handle] = pResource;

			return handle;

		} // Add

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

			return pResource->GetHandle();

		} // AddManualResource

		void RemoveResource(T* pResource)
		{
			m_sHandles.push(pResource->GetHandle());
			m_vResources[pResource->GetHandle()] = NULL;

		} // RemoveResource

		//! Returns a resource.
		virtual T* GetResource(u32 handle)
		{
			return m_vResources[handle];

		} // GetResource

		// returns resource handle before creating resource then use Add(Resource*, u32 Handel) function
		// to set the loaded resource.
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

	protected:

		std::vector<T*>			m_vResources;		/**< Stores the resources */
		std::stack<u32>			m_sHandles;			/**< Stores the unused handles number*/
		core::Functor5<bool , Resource** , const u32 , const char* , const char* , 
			void* >*			m_pCreateResourceFn;/**< Pointer to the function which creats the resource. */

	}; // ResourceManager

} // kge

#endif // KGE_RESOURCEMANAGER_H
