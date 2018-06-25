#pragma once

#include "../Component.h"
#include "../../math/Vector.h"

namespace kge
{
	namespace en
	{
		class KGE_API Node3D: public Component
		{
		public:

			//! Constructor
			Node3D(): Scale(1.0f, 1.0f, 1.0f) { }
		
			//! Every component must declare this function for returning their class ID
			virtual int GetClassID();
			
			//! Every component must declare this function for returning their class name
			virtual std::string GetClassName();

			//! Clone a component
			virtual Component* Clone();

			math::Vector	Position,
							Rotation,
							Scale;

		}; // Node3D

	} // en

} // kge
