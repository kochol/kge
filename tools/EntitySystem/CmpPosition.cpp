#include "../../Include/en/CmpPosition.h"
#include "../../Include/en/ComponentType.h"

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

} // en

} // kge
