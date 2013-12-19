#ifndef KGE_RAYCAST_REPORT_H
#define KGE_RAYCAST_REPORT_H

#include "ActorDynamic.h"
#include "Material.h"

namespace kge
{
	namespace ph
	{
		enum ERCP_Raycast
		{
				ERCP_STATIC_SHAPES		= (1<<0),								//!< Hits static shapes
				ERCP_DYNAMIC_SHAPES		= (1<<1),								//!< Hits dynamic shapes
				ERCP_ALL_SHAPES			= ERCP_STATIC_SHAPES|ERCP_DYNAMIC_SHAPES//!< Hits both static & dynamic shapes
		}; // ERCP_Raycast

		enum ERHB_Raycast
		{
			ERHB_RAYCAST_SHAPE		= (1<<0),								//!< "shape" member of #NxRaycastHit is valid
			ERHB_RAYCAST_IMPACT		= (1<<1),								//!< "worldImpact" member of #NxRaycastHit is valid
			ERHB_RAYCAST_NORMAL		= (1<<2),								//!< "worldNormal" member of #NxRaycastHit is valid
			ERHB_RAYCAST_FACE_INDEX	= (1<<3),								//!< "faceID" member of #NxRaycastHit is valid
			ERHB_RAYCAST_DISTANCE	= (1<<4),								//!< "distance" member of #NxRaycastHit is valid
			ERHB_RAYCAST_UV			= (1<<5),								//!< "u" and "v" members of #NxRaycastHit are valid
			ERHB_RAYCAST_FACE_NORMAL= (1<<6),								//!< Same as NX_RAYCAST_NORMAL but computes a non-smoothed normal
			ERHB_RAYCAST_MATERIAL	= (1<<7) 								//!< "material" member of #NxRaycastHit is valid
		}; // ERHB_Raycast

		class RaycastReport
		{
			friend class RaycastX;
			friend class PhysXManager;
		public:
			/* Constructor */
			RaycastReport(){ /* Nothing To Do */ }

			/* Distructor */
			~RaycastReport(){ /* Nothing To Do */ }

			ActorDynamic*	getActor		( void ){ return m_pActor; }
			math::Vector*	getWorldImpact	( void ){ return m_pWorldImpact; }
			math::Vector*	getWorldNormal	( void ){ return m_pWorldNormal; }
			unsigned int	getFaceID		( void ){ return m_pFaceID; }
			unsigned int	getInternalFaceID(void ){ return m_pInternalFaceID; }
			float			getDistance		( void ){ return m_pDistance; }
			float			getU			( void ){ return m_pU; }
			float			getV			( void ){ return m_pV; }
			Material*		getMaterial		( void ){ return m_pMaterial; }
			unsigned int	getFlags		( void ){ return m_pFlags; }

		protected:
			ActorDynamic*	m_pActor;
			math::Vector*	m_pWorldImpact;
			math::Vector*	m_pWorldNormal;
			unsigned int	m_pFaceID;
			unsigned int	m_pInternalFaceID;
			float			m_pDistance;
			float			m_pU;
			float			m_pV;
			Material*		m_pMaterial;
			unsigned int	m_pFlags;
		}; // Raycast
	} // ph
} // kge

#endif // KGE_RAYCAST_REPORT_H