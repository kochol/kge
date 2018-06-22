// File name: UserNotifyX.h
// Des: This class is User Notify of Physx Events
// Date: 08/01/1389
// Programmer: Hadi Robati (hadirobati)

//#include "PhysXManager.h"

//#ifdef KGE_USE_PHYSX

#ifndef KGE_USER_NOTIFY_X_H
#define KGE_USER_NOTIFY_X_H

// To use physx you need add physx SDK search paths.
//#include <NxPhysics.h>

#include "../../include/kge/ph/UserNotify.h" 
#include "../../include/kge/ph/ActorDynamic.h"
#include "../../include/kge/core/Functor.h"
#include <stdio.h>

namespace kge
{
	namespace ph
	{
		class UserNotifyX : public NxUserNotify
		{
		public:
			/* Constructor */
			UserNotifyX(){ isSetWakeupFuncPtr = false ; isSetSleepFuncPtr = false;/* Nothing to do */ }

			/* Destructor */
			~UserNotifyX() { /* Nothing to do */ }
			
			/* Set Wake Up Function Pointer*/
			void setWakeUpFuncPtr( core::Functor2<void,ActorDynamic**,int>* pFunc )
			{
				isSetWakeupFuncPtr=true;
				m_pWakeupFuncPtr = pFunc;
			}

			/* Set Sleep Function Pointer*/
			void setSleepFuncPtr( core::Functor2<void,ActorDynamic**,int>* pFunc )
			{
				isSetSleepFuncPtr=true;
				m_pSleepFuncPtr = pFunc;
			}

			/*On Sleep PhysX Actor FeedBack*/
			virtual void onSleep(NxActor** actors, NxU32 count)
			{	
				if( !isSetSleepFuncPtr )
					return;

				unsigned int i = count;

				ActorDynamic** actorArray = new ActorDynamic*[ i ];

				while( i-- )
				{
					NxActor* actor = *actors++;
					if( actor->userData )
					{
						ActorDynamic *aD = ((ActorDynamic*)(actor->userData));
						actorArray[ i ] = aD;
					}
					else
						kge::io::Logger::Log( " onSleep : Actor User Data is NULL!!!!\n" );
				}

				m_pSleepFuncPtr->Call( actorArray , count );
			}
			
			/*On Wake PhysX Actor FeedBack*/
			virtual void onWake(NxActor** actors, NxU32 count)
			{
				if( !isSetWakeupFuncPtr )
					return;

				unsigned int i = count;
				ActorDynamic** actorArray = new ActorDynamic*[ i ];

				while( i-- )
				{
					NxActor* actor = *actors++;
					if( actor->userData )
					{
						ActorDynamic *aD = ((ActorDynamic*)(actor->userData));
						actorArray[ i ] = aD;
					}
					else
						kge::io::Logger::Log( "onWake : Actor User Data is NULL!!!!\n" );
				}

 				m_pWakeupFuncPtr->Call( actorArray , count );
			}
			
			/*On Joint Break PhysX FeedBack*/
			virtual bool onJointBreak(NxReal breakingImpulse, NxJoint& brokenJoint)
			{
				return true;
			}

		protected:
			bool isSetWakeupFuncPtr;
			bool isSetSleepFuncPtr;
			core::Functor2<void,ActorDynamic**,int> *m_pWakeupFuncPtr;
			core::Functor2<void,ActorDynamic**,int> *m_pSleepFuncPtr;
		}; // UserNotify
	} // ph
} // kge

#endif // KGE_USER_NOTIFY_X_H
//#endif // KGE_USE_PHYSX