// File name: ActorManagerX.h
// Des: This class is for creating PhysX Actor
// Date: 20/10/1388
// Programmer: Hadi Robati (hadirobati)

#include "PhysXManager.h"
#ifdef KGE_USE_PHYSX

#ifndef KGE_PLANEX_H
#define KGE_PLANEX_H

namespace kge
{
	namespace ph
	{
		class PlaneX:public Plane
		{
		public:
			//! Constructor
			PlaneX( PhysXManager *pPhysMan , kge::sn::SceneNode *kgePlane ,  kge::math::Vector *normal );
			PlaneX( PhysXManager *pPhysMan , kge::math::Vector *pos , kge::math::Vector *normal , float d );
			
			//! Destructor
			~PlaneX();

		protected:
			NxActor*		localPlane;

		}; // PlaneX
	} // ph
} // kge

#endif // KGE_PLANEX_H
#endif // KGE_USE_PHYSX