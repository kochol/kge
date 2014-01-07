#ifndef KGE_ENTITY_H
#define KGE_ENTITY_H

#include "../KgeUnknown.h"
#include <map>
#include <vector>

#ifdef KGE_USE_RAKNET
#	include <BitStream.h>
#endif // KGE_USE_RAKNET

namespace kge
{
	namespace core
	{
		class KgeMutex;
	}

	namespace en
	{
		class Component;

		//! This is the base class for entity system in KGE
		class KGELIB_API Entity: public KgeUnknown
		{
		public:

			//! Constructor
			Entity(int ID);

			//! Destructor
			~Entity();

			//! Returns the entity ID.
			int GetID() {return m_iID;}

			//! Adds a component to the entity
			void AddComponent(Component* pCmp);

			//! Returns a component by its ID
			Component* GetComponent(int iCmpID);

			//! Returns a list of components by its ID
			/*!
				Use this function when your entity has multi instance of one component
			 */
			std::vector<Component*> GetComponents(int iCmpID);

#ifdef KGE_USE_RAKNET
			//! Serialize the entity and its components to RakNet::BitStream
			void Serialize(RakNet::BitStream bs, bool write);

#endif // KGE_USE_RAKNET

			//! Entity type
			int Type;

		protected:

			std::map<int, std::vector<Component*> >				m_vComponents;			//!< The list of components
			std::map<int, std::vector<Component*> >::iterator	m_itComponents;			//!< The helper iterator
			int													m_iID;					//!< Entity ID

			//! Define KGE_EN_TS to make Entity System Thread Safe
#ifdef KGE_EN_TS
			core::KgeMutex										*	m_pMutex;
#endif // KGE_EN_TS

		private:

		}; // Entity

	} // en

} // kge

#endif // KGE_ENTITY_H
