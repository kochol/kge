#ifndef KGE_ENTITY_H
#define KGE_ENTITY_H

#include "KgeUnknown.h"
#include <map>
#include <vector>

namespace kge
{
	namespace en
	{
		class Component;

		//! This is the base class for entity system in KGE
		class KGE_API Entity: public KgeUnknown
		{
		public:

			//! Constructor
			Entity();

			//! Destructor
			~Entity();

			//! Adds a component to the entity
			void AddComponent(Component* pCmp);

			//! Returns a component by its ID
			Component* GetComponent(int iCmpID);

		protected:

			std::map<int, std::vector<Component*>>				m_vComponents;			//!< The list of components
			std::map<int, std::vector<Component*>>::iterator		m_itComponents;			//!< The helper iterator

		private:

		}; // Entity

	} // en

} // kge

#endif // KGE_ENTITY_H
