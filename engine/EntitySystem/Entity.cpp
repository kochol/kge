#include "../include/Entity.h"
#include "../include/Component.h"

#pragma comment(lib, "../../bin/Debug/kge.lib")

namespace kge
{
	namespace en
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Entity::Entity()
		{

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
			m_itComponents = m_vComponents.find(pCmp->GetClassID());
			if (m_itComponents == m_vComponents.end())
			{
				m_vComponents.insert(std::pair<int, std::vector<Component*>>(pCmp->GetClassID(), std::vector<Component*>()));
			}

			m_vComponents[pCmp->GetClassID()].push_back(pCmp);

		} // AddComponent

		//------------------------------------------------------------------------------------
		// Returns a component by its ID
		//------------------------------------------------------------------------------------
		Component* Entity::GetComponent( int iCmpID )
		{
			m_itComponents = m_vComponents.find(iCmpID);
			if (m_itComponents == m_vComponents.end())
			{
				return NULL;
			}

			if (m_itComponents->second.size() > 0)
				return m_itComponents->second[0];

			return NULL;

		} // GetComponent

	} // en

} // kge
