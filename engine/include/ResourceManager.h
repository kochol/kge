#ifndef KGE_RESOURCEMANAGER_H
#define KGE_RESOURCEMANAGER_H

#include <vector>
#include <stack>
#include "Resource.h"
#include "Functor.h"
#include "KgeMemory.h"
#include "FileSystemManager.h"
#include "Stream.h"
#include "Loader.h"

namespace kge
{
	//! Resource manager take care of loading and unloading resources.
	template <class T>
	class ResourceManager
	{
	public:

		//! Constructor
		ResourceManager()
		{

		} // Constructor

		//! Destructor
		virtual ~ResourceManager()
		{
			// Searching for resources and delete them.
			for(typename std::vector<T*>::iterator it = m_vResources.begin();
				it != m_vResources.end(); it++)
			{
				if ((*it))
				{
					KGE_DELETE((*it), T);
					(*it) = NULL;
				}
			}

			m_vResources.clear();

			// Delete resource loaders
			for (int i = 0; i < m_vLoaders.size(); i++)
			{
				KGE_DELETE(m_vLoaders[i], Loader);
			}
			m_vLoaders.clear();

		} // Destructor

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
		T* GetResource(u32 handle)
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
			io::Stream* pStream = io::FileSystemManager::getSingletonPtr()->Load(FileName);
			if (!pStream)
				return NULL;

			// Ask loaders if any one can load this resource
			for (size_t i = 0; i < m_vLoaders.size(); i++)
			{
				if (!m_vLoaders[i]->IsALoadableFileExtension(FileName))
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

			return pResource;

		} // Load

	protected:

		std::vector<T*>			m_vResources;		/**< Stores the resources */
		std::stack<u32>			m_sHandles;			/**< Stores the unused handles number*/
		std::vector<Loader*>		m_vLoaders;			//!< Stores the resource loaders

	}; // ResourceManager

} // kge

#endif // KGE_RESOURCEMANAGER_H
