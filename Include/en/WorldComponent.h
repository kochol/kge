#pragma once

namespace kge
{
	namespace en
	{
		class WorldComponent
		{
		public:
		
			//! Constructor
			WorldComponent() {}
		
			//! Destructor
			virtual ~WorldComponent() {}

			//! World call Update function of component and it calculates its logics
			virtual void Update(float fElasped) = 0;
		
			//! Every component must declare this function for returning their class ID
			virtual int GetClassID() = 0;

		protected:
		
			
		}; // WorldComponent

	} // en

} // kge
