#include "../../Include/en/CmpMesh.h"
#include "../../Include/en/ComponentType.h"
#include "../../Include/core/KgeMemory.h"

namespace kge
{
namespace en
{
	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class ID
	//------------------------------------------------------------------------------------
	int CmpMesh::GetClassID()
	{
		return ComponentType<CmpMesh>::GetID();

	} // GetClassID

	//------------------------------------------------------------------------------------
	// Every component must declare this function for returning their class name
	//------------------------------------------------------------------------------------
	std::string CmpMesh::GetClassName()
	{
		static std::string strName = "CmpMesh";

		return strName;

	} // GetClassName

	//------------------------------------------------------------------------------------
	// Clone
	//------------------------------------------------------------------------------------
	Component* CmpMesh::Clone()
	{
		CmpMesh* p = KGE_NEW(CmpMesh)(strMeshName);

		return p;

	} // Clone

} // en

} // kge
