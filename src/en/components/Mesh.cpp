#include "../../../include/kge/en/components/Mesh.h"
#include "../../../include/kge/en/ComponentType.h"
#include "../../../include/kge/core/KgeMemory.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class ID
	//------------------------------------------------------------------------------------
	int Mesh::GetClassID()
	{
		return ComponentType<Mesh>::GetID();

	} // GetClassID

	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class name
	//------------------------------------------------------------------------------------
	std::string Mesh::GetClassName()
	{
		static std::string strName = "Mesh";

		return strName;

	} // GetClassName

	//------------------------------------------------------------------------------------
	// Clone
	//------------------------------------------------------------------------------------
	Component* Mesh::Clone()
	{
		Mesh* p = KGE_NEW(Mesh)(*this);

		return p;

	} // Clone

} // en

} // kge