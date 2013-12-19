#ifndef KGE_RAYCAST_X_H
#define KGE_RAYCAST_X_H

#include "../../include/core/Functor.h"
#include "../../include/ph/RaycastReport.h"
#include "../../include/ph/ActorDynamic.h"

namespace kge
{
	namespace ph
	{
		class RaycastX : public NxUserRaycastReport , RaycastReport
		{
		public:
			/* Constructor */
			RaycastX() { /* Nothing to do */ }

			/* Distructor */
			virtual ~RaycastX() { /* Nothing to do */ }

			virtual bool onHit(const NxRaycastHit& hits)
			{
				RaycastReport rayHit;
				ActorDynamic* actor;

				actor = (ActorDynamic*)((hits.shape)->getActor().userData);

				rayHit.m_pMaterial = actor->getMaterial();
				rayHit.m_pActor = actor;
				rayHit.m_pDistance = hits.distance;
				rayHit.m_pFaceID = hits.faceID;
				rayHit.m_pFlags = hits.flags;
				rayHit.m_pU = hits.u;
				rayHit.m_pV = hits.v;
				rayHit.m_pWorldImpact = &kge::math::Vector( hits.worldImpact.x , hits.worldImpact.y , hits.worldImpact.z );
				rayHit.m_pWorldNormal = &kge::math::Vector( hits.worldNormal.x , hits.worldNormal.y , hits.worldNormal.z );

				if( m_pRaycastReport )
					return m_pRaycastReport->Call( &rayHit );
				else
					kge::io::Logger::Log( "onHit : Pointer to function is NULL\n" );

				return false;
			}

			void setRaycastReportFunc( core::Functor1<bool,RaycastReport*> *pFunc )
			{
				m_pRaycastReport = pFunc;
			}

		protected:
			core::Functor1<bool,RaycastReport*> *m_pRaycastReport;

		}; // class RaycastX
	} // ph
} // kge

#endif // KGE_RAYCAST_X_H 