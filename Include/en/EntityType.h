#pragma once

#include "../kgedef.h"
#include <map>
#include <vector>

namespace kge
{
	namespace en
	{
		class Component;
		class World;
		class Entity;

		class EntityType
		{
		public:
		
			//! Constructor
			EntityType();
		
			//! Destructor
			~EntityType();

			//! Returns the EntityType ID
			int GetID() {return m_iID;}

			//! Adds a component
			void AddComponent(Component* pCmp);

			//! Creates an entity
			Entity* CreateEntity();
		
		protected:

			std::map<int, std::vector<Component*> >				m_vComponents;			//!< The list of components
			std::map<int, std::vector<Component*> >::iterator	m_itComponents;			//!< The helper iterator
			int													m_iID;					//!< Entity ID
			
		}; // EntityType

	} // en

} // kge
