#ifndef KGE_USER_TRIGGER_H
#define KGE_USER_TRIGGER_H

#include "../core/Functor.h"
#include "actorDynamic.h"

namespace kge
{
	namespace ph
	{
		class TriggerReport
		{
		public:
			/* Constructor */
			TriggerReport() { /* Nothing To Do */ }

			~TriggerReport() { /* Nothing To Do */ }

			void setTriggerFunc( core::Functor3<void,ActorDynamic* , ActorDynamic* , int* > *pFunc )
			{
				m_pTriggerReportFunction = pFunc;
			}

			core::Functor3<void,ActorDynamic* , ActorDynamic* , int* >* getTriggerFunc( void )
			{
				return m_pTriggerReportFunction;
			}

		protected:
			core::Functor3<void,ActorDynamic*,ActorDynamic*,int *> *m_pTriggerReportFunction;

		}; // UserTrigger
	} // ph
} // kge

#endif // KGE_USER_TRIGGER_H
