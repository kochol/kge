#include "../../../include/kge/en/components/Node3D.h"
#include "../../../include/kge/en/ComponentType.h"
#include "../../../include/kge/core/KgeMemory.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class ID
	//------------------------------------------------------------------------------------
	int Node3D::GetClassID()
	{
		return ComponentType<Node3D>::GetID();

	} // GetClassID

	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class name
	//------------------------------------------------------------------------------------
	std::string Node3D::GetClassName()
	{
		static std::string strName = "Node3D";

		return strName;

	} // GetClassName

	//------------------------------------------------------------------------------------
	// Clone
	//------------------------------------------------------------------------------------
	Component* Node3D::Clone()
	{
		Node3D* p = KGE_NEW(Node3D)(*this);

		return p;

	} // Clone

} // en

} // kge