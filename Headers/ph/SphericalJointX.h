// File name: SphericalJointX.h
// Des: This class is for creating PhysX Spherical Joint
// Date: 25/02/1389
// Programmer: Hadi Robati (hadirobati)
#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_PH_SPHERICAL_JOINT_X_H
#define KGE_PH_SPHERICAL_JOINT_X_H

namespace kge
{
	namespace ph
	{
		class SphericalJointX : public SphericalJoint
		{
		public:
			//! Constructor
			SphericalJointX( PhysXManager *pMan , Actor *a1 , Actor *a2 , math::Vector *Anchor ); 

			//! Distructor
			~SphericalJointX() { /* Nothing To Do*/ }

			void			setSwingAxis( math::Vector* swingAxis );
			math::Vector*	getSwingAxis( void );

			void			setSwingLimit( float value , float restitution );
			void			setTwistLimit( float a , float b , float c , float d  );

		protected:
			NxSphericalJoint*	m_pSphericalJoint;
		};
	} // ph
} // kge

#endif // KGE_PH_SPHERICAL_JOINT_X_H

#endif // KGE_USE_PHYSX