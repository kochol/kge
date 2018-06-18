#pragma once

#include "../kgedef.h"

namespace kge
{
	namespace core
	{
		class KGE_API Task
		{
		public:
		
			//! Constructor
			Task() {}
		
			//! Destructor
			virtual ~Task() {}

			//! Do the task
			virtual Task* Do() = 0;
		
		protected:
		
			
		}; // Task

	} // core

} // kge
