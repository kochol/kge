#ifndef KGE_COMPONENTTYPE_H
#define KGE_COMPONENTTYPE_H

#include "../kgedef.h"

namespace kge
{
	namespace en
	{
		//! Assign an ID
		class KGELIB_API assign
		{
		public:
			static int AssignComponentID();
		};

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

				return ct.m_siID;

			} // GetID

		}; // ComponentType

	} // en

} // kge

#endif // KGE_COMPONENTTYPE_H