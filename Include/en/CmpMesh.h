#pragma once

#include "../kgedef.h"
#include "Component.h"

namespace kge
{
	namespace sn
	{
		class StaticMesh;
	}
	namespace en
	{
		class KGELIB_API CmpMesh : public Component
		{
		public:

			//! Constructor
			CmpMesh(std::string MeshName): strMeshName(MeshName), m_pStaticMesh(NULL) {}

			//! Every component must declare this function for returning their class ID
			int GetClassID();

			//! Every component must declare this function for returning their class name
			std::string GetClassName();

			std::string			strMeshName;			//!< The Mesh name
			
			sn::StaticMesh	*	m_pStaticMesh;

		}; // CmpMesh

	} // en

} // kge
