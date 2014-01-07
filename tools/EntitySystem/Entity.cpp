#include "../../Include/en/Entity.h"
#include "../../Include/en/Component.h"
#ifdef KGE_EN_TS
#	include "../../Include/core/KgeLock.h"
#endif // KGE_EN_TS

#ifdef KGE_USE_RAKNET
#	pragma comment(lib, "RakNetDLLDebug.lib")
#endif // KGE_USE_RAKNET

#pragma comment(lib, "../../bin/Debug/kge.lib")

namespace kge
{
	namespace en
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Entity::Entity(int ID): m_iID(ID), Type(0)
		{
#ifdef KGE_EN_TS
			m_pMutex = KGE_NEW(core::KgeMutex);
#endif // KGE_EN_TS

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Entity::~Entity()
		{

		} // Destructor

		//------------------------------------------------------------------------------------
		// Adds a component to the entity
		//------------------------------------------------------------------------------------
		void Entity::AddComponent( Component* pCmp )
		{
#ifdef KGE_EN_TS
			core::KgeLock lock(m_pMutex);
#endif // KGE_EN_TS

			m_itComponents = m_vComponents.find(pCmp->GetClassID());
			if (m_itComponents == m_vComponents.end())
			{
				m_vComponents.insert(std::pair<int, std::vector<Component*> >(pCmp->GetClassID(), std::vector<Component*>()));
			}

			m_vComponents[pCmp->GetClassID()].push_back(pCmp);

		} // AddComponent

		//------------------------------------------------------------------------------------
		// Returns a component by its ID
		//------------------------------------------------------------------------------------
		Component* Entity::GetComponent( int iCmpID )
		{
#ifdef KGE_EN_TS
			core::KgeLock lock(m_pMutex);
#endif // KGE_EN_TS

			m_itComponents = m_vComponents.find(iCmpID);
			if (m_itComponents == m_vComponents.end())
			{
				return NULL;
			}

			if (m_itComponents->second.size() > 0)
				return m_itComponents->second[0];

			return NULL;

		} // GetComponent

		//------------------------------------------------------------------------------------
		// Returns a list of components by its ID
		//------------------------------------------------------------------------------------
		std::vector<Component*> Entity::GetComponents( int iCmpID )
		{
#ifdef KGE_EN_TS
			core::KgeLock lock(m_pMutex);
#endif // KGE_EN_TS

			m_itComponents = m_vComponents.find(iCmpID);
			if (m_itComponents == m_vComponents.end())
			{
				return std::vector<Component*>();
			}
			return m_itComponents->second;

		} // GetComponents

#ifdef KGE_USE_RAKNET
		//------------------------------------------------------------------------------------
		// Serialize the entity and its components to RakNet::BitStream
		//------------------------------------------------------------------------------------
		void Entity::Serialize( RakNet::BitStream bs, bool write )
		{
			bs.Serialize(write, m_iID);
			bs.Serialize(write, Type);

#ifdef KGE_EN_TS
			core::KgeLock lock(m_pMutex);
#endif // KGE_EN_TS

			// Serialize components
			for (m_itComponents = m_vComponents.begin(); 
				 m_itComponents != m_vComponents.end(); 
				 m_itComponents++)
			{
				for (int i = 0; i < m_itComponents->second.size(); i++)
				{
					//m_itComponents->second[i]
				}
			}

		} // Serialize

#endif // KGE_USE_RAKNET

	} // en

} // kge
