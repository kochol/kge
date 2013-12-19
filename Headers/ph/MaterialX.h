// File name: MaterialX.h
// Des: This class is for creating PhysX Material
// Date: 22/12/1388
// Programmer: Hadi Robati (hadirobati)

#include "PhysXManager.h"

#ifdef KGE_USE_PHYSX

#ifndef KGE_MATERIAL_X_H
#define KGE_MATERIAL_X_H

namespace kge
{
	namespace ph
	{
		class MaterialX : public Material
		{
		public:
			//! Constructor
			MaterialX( PhysXManager* phMan );

			//! Constructor
			MaterialX( PhysXManager* phMan , float Restitution , float staticFriction , float dynamicFriction , float dynamicFrictionV, 
						float staticFrictionV , kge::math::Vector *dir );
			
			//! Distructor
			~MaterialX() {};

			void	setRestitution		( float Restitution );

			float	getRestitution		( void );

			void	setStaticFriction	( float staticFriction );

			float	getStaticFriction	( void );

			void	setDynamicFriction	( float dynamicFriction );

			float	getDynamicFriction	( void );

			void	setDynamicFrictionV	( float dynamicFrictionV );

			float	getDynamicFrictionV	( void );

			void	setDirOfAnisotropy	( kge::math::Vector *dir );

			void	setStaticFrictionV	( float staticFrictionV );

			float	getStaticFrictionV	( void );

			char	isValid				( void );

			void	setToDefault		( void );

			NxMaterialIndex getMaterialIndex( void );

		protected:
			NxScene*		m_gNxScene;
			NxMaterialIndex m_gMaterialIndex;
		};
	}
}
#endif // KGE_MATERIAL_X_H

#endif // KGE_USE_PHYSX