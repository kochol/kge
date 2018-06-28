#pragma once

#include "Node3D.h"
#include "../../math/Matrix.h"

namespace kge
{
	namespace en
	{
		class KGE_API Camera: public Node3D
		{
			public:

			//! Constructor
			Camera() { }

			//! Destructor
			virtual ~Camera() { }
		
			//! Every component must declare this function for returning their class ID
			virtual int GetClassID();
			
			//! Every component must declare this function for returning their class name
			virtual std::string GetClassName();

			//! Clone a component
			virtual Component* Clone();

			math::Vector	Target,
							Up,
							Right;
			math::Matrix	View,
							Projection;
			float			Fov,
							Near,
							Far;
			bool 			IsActive;				

		}; // Camera

	} // en

} // kge
