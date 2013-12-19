#ifndef KGE_USER_NOTIFY_H
#define KGE_USER_NOTIFY_H

#include "../core/Functor.h"

namespace kge
{
	namespace ph
	{
		class UserNotify
		{
		public:
			/* Constructor */
			UserNotify() { /*Nothing to Do*/ }

			/* Destructor */
			~UserNotify(){ /* Nothing to Do*/ }

			void setWakeUpFunPtr( core::Functor2<void, ActorDynamic**, int>	* pfunc )
			{
				m_pWakeUpFunc = pfunc;
			}

			void setSleepFunPtr( core::Functor2<void, ActorDynamic**, int>	* pfunc )
			{
				m_pSleepFunc = pfunc;
			}
			
			core::Functor2<void, ActorDynamic**, int>* getSleepFunc( void )
			{
				return m_pSleepFunc;
			}

			core::Functor2<void, ActorDynamic**, int>* getWakeUpFunc( void )
			{
				return m_pWakeUpFunc;
			}

		protected:
			core::Functor2<void, ActorDynamic**, int>	*m_pWakeUpFunc;
			core::Functor2<void, ActorDynamic**, int>	*m_pSleepFunc;
		};
	} // ph
} // kge
#endif // KGE_USER_NOTIFY_H