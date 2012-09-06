#ifndef KGE_COMPONENT_H
#define KGE_COMPONENT_H

#include "String.h"

namespace kge
{
	namespace en
	{
		class Component
		{
		public:

			//! Every component must declare this function for returning their class ID
			virtual int GetClassID() = 0;
			
			//! Every component must declare this function for returning their class name
			virtual core::stringc GetClassName() = 0;

		protected:


		}; // Component

	} // en

} // kge

#endif // KGE_COMPONENT_H
