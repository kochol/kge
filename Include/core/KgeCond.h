#pragma once

struct SDL_cond;

namespace kge
{
	namespace core
	{
		class KgeCond
		{
		public:
		
			//! Constructor
			KgeCond();
		
			//! Destructor
			~KgeCond();
		
		protected:
		
			SDL_cond		*	m_pCond;
			
		}; // KgeCond

	} // core

} // kge
