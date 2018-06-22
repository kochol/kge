#ifndef KGE_COMPONENTTYPE_H
#define KGE_COMPONENTTYPE_H

#include "../kgedef.h"
#include <map>
#include "../core/KgeMemory.h"

namespace kge
{
	namespace en
	{
		class Component;

		//! Assign an ID
		class KGELIB_API assign
		{
		public:
			static int AssignComponentID();
		};


		class KGELIB_API ComponentCreator
		{
			//! Constructor
			ComponentCreator();

		public:

			//! Destructor
			~ComponentCreator();

			static void AddComponent(int id, Component* pCmp);

			static Component* CreateComponent(int id);

		protected:			

		}; // ComponentCreator

		template<typename T>
		class ComponentType
		{
			int m_siID;

			//! Constructor
			ComponentType()
			{
				m_siID = assign::AssignComponentID();
			}

		public:

			static int GetID()
			{
				static ComponentType<T> ct;
				static bool Added = false;
				if (!Added)
				{
					Added = true;
					ComponentCreator::AddComponent(ct.m_siID, KGE_NEW(T));
				}
				return ct.m_siID;

			} // GetID

		}; // ComponentType

	} // en

} // kge

#endif // KGE_COMPONENTTYPE_H