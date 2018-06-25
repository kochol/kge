#include "../../include/kge/en/CmpPosition.h"
#include "../../include/kge/en/ComponentType.h"
#include "../../include/kge/core/KgeMemory.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class ID
	//------------------------------------------------------------------------------------
	int CmpPosition::GetClassID()
	{
		return ComponentType<CmpPosition>::GetID();

	} // GetClassID

	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class name
	//------------------------------------------------------------------------------------
	std::string CmpPosition::GetClassName()
	{
		static std::string strName = "CmpPosition";

		return strName;

	} // GetClassName

	//------------------------------------------------------------------------------------
	// Clone a component
	//------------------------------------------------------------------------------------
	Component* CmpPosition::Clone()
	{
		CmpPosition* p = KGE_NEW(CmpPosition)(x, y, z);

		return p;

	} // Clone

} // en

} // kge
