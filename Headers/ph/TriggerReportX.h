#ifndef KGE_USER_TRIGGER_X_H
#define KGE_USER_TRIGGER_X_H

#include "../../Include/ph/TriggerReport.h"
#include "../../Include/ph/ActorDynamic.h"
#include "../../Include/core/Functor.h"

namespace kge
{
	namespace ph
	{
		class TriggerReportX : public NxUserTriggerReport
		{
		public:
			/* Constructor */
			TriggerReportX(){ /* Nothing to Do */ }
			
			virtual ~TriggerReportX(){ /* Nothing to Do */ }

			virtual void  onTrigger (NxShape &triggerShape, NxShape &otherShape, NxTriggerFlag status)
			{
				if( !m_pTriggerReportFunction )
				{
					kge::io::Logger::Log( "UserTrigger::OnTrigger : Function Pointer is NULL\n" );
					return;
				}

				NxActor& a1 = triggerShape.getActor();
				NxActor& a2 = otherShape.getActor();

				int s = (int)(status);

				if( a1.userData && a2.userData )
				{
					ActorDynamic* aD1 = ((ActorDynamic*)(a1.userData));
					ActorDynamic* aD2 = ((ActorDynamic*)(a2.userData));
					m_pTriggerReportFunction->Call( aD1 , aD2 , &s );
				}
			}

			void setTriggerFunc( core::Functor3<void,ActorDynamic*,ActorDynamic*,int *> *pFunc )
			{
				m_pTriggerReportFunction = pFunc;
			}

		protected:
			core::Functor3<void,ActorDynamic*,ActorDynamic*,int *> *m_pTriggerReportFunction;
		}; // UserTriggerX
	} // ph
} // kge
#endif // KGE_USER_TRIGGER_X_H
