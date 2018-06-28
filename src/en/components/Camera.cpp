#include "../../../include/kge/en/components/Camera.h"
#include "../../../include/kge/en/ComponentType.h"
#include "../../../include/kge/core/KgeMemory.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class ID
	//------------------------------------------------------------------------------------
	int Camera::GetClassID()
	{
		return ComponentType<Camera>::GetID();

	} // GetClassID

	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class name
	//------------------------------------------------------------------------------------
	std::string Camera::GetClassName()
	{
		static std::string strName = "Camera";

		return strName;

	} // GetClassName

	//------------------------------------------------------------------------------------
	// Clone
	//------------------------------------------------------------------------------------
	Component* Camera::Clone()
	{
		Camera* p = KGE_NEW(Camera)(*this);

		return p;

	} // Clone

} // en

} // kge