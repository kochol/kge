#pragma once

#include "Node3D.h"
#include "../../gfx/MeshBuffer.h"

namespace kge
{
	namespace en
	{
		class KGE_API Mesh: public Node3D
		{
		public:

			//! Constructor
			Mesh():MeshRes(NULL) { }

			//! Destructor
			virtual ~Mesh() { }
		
			//! Every component must declare this function for returning their class ID
			virtual int GetClassID();
			
			//! Every component must declare this function for returning their class name
			virtual std::string GetClassName();

			//! Clone a component
			virtual Component* Clone();

			gfx::MeshBuffer		*	MeshRes;

		}; // Mesh

	} // en

} // kge
