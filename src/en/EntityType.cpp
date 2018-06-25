#include "../../include/kge/en/EntityType.h"
#include "../../include/kge/en/Component.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	EntityType::EntityType(): m_iID(-1)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	EntityType::~EntityType()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Adds a component
	//------------------------------------------------------------------------------------
	void EntityType::AddComponent( Component* pCmp )
	{
		m_itComponents = m_vComponents.find(pCmp->GetClassID());
		if (m_itComponents == m_vComponents.end())
		{
			m_vComponents.insert(std::pair<int, std::vector<Component*> >(pCmp->GetClassID(), std::vector<Component*>()));
		}

		m_vComponents[pCmp->GetClassID()].push_back(pCmp);

	} // AddComponent

	//------------------------------------------------------------------------------------
	// Creates an entity
	//------------------------------------------------------------------------------------
	Entity* EntityType::CreateEntity()
	{
		return NULL;

	} // CreateEntity

} // en

} // kge
