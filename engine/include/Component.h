#ifndef KGE_COMPONENT_H
#define KGE_COMPONENT_H

#include <string>
#include "../CyrusScript/Tools.h"

namespace kge
{
	namespace en
	{
		class Component
		{
		public:

			//! Every component must declare this function for returning their ID
			virtual int GetClassID() = 0;

		protected:


		}; // Component

	} // en

} // kge

#endif // KGE_COMPONENT_H
