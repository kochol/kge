#ifndef KGE_COMPONENTTYPE_H
#define KGE_COMPONENTTYPE_H

namespace kge
{
	namespace en
	{
		//! Assign an ID
		static int AssignComponentID = 0;

		template<typename T>
		class ComponentType
		{
			int m_siID;

			//! Constructor
			ComponentType()
			{
				m_siID = AssignComponentID++;
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