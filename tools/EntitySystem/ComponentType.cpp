#include "../../Include/en/ComponentType.h"
#include "../../Include/en/Component.h"

std::map<int, kge::en::Component*>	m_mComs;

namespace kge
{
	namespace en
	{
		int assign::AssignComponentID()
		{
			static int i = 0;
			return i++;
		}


		void ComponentCreator::AddComponent( int id, Component* pCmp )
		{
			m_mComs[id] = pCmp;
		}

		Component* ComponentCreator::CreateComponent( int id )
		{
			return m_mComs[id]->Clone();
		}

	} // en

} // kge
