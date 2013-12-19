// File name: MaterialX.cpp
// Des: This class is for creating PhysX Material
// Date: 22/12/1388
// Programmer: Hadi Robati (hadirobati)

#include "../../Headers/ph/MaterialX.h"
#include "stdio.h"

#ifdef KGE_USE_PHYSX

namespace kge
{
	namespace ph
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		MaterialX::MaterialX( PhysXManager* phMan )
		{
			NxMaterialDesc nxMaterialDesc;

			nxMaterialDesc.setToDefault();

			m_gNxScene = phMan->getPhysxScene();

			m_gMaterialIndex = m_gNxScene->createMaterial( nxMaterialDesc )->getMaterialIndex();
		}

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		MaterialX::MaterialX( PhysXManager* phMan , float Restitution , float staticFriction , float dynamicFriction , 
								float dynamicFrictionV, float staticFrictionV , kge::math::Vector *dir )
		{
			NxMaterialDesc nxMaterialDesc;

			nxMaterialDesc.setToDefault();

			if( Restitution >= 0 )
				nxMaterialDesc.restitution = Restitution;

			if( staticFriction >= 0 )
				nxMaterialDesc.staticFriction = staticFriction;

			if( dynamicFriction >= 0 )
				nxMaterialDesc.dynamicFriction = dynamicFriction;

			if( dynamicFrictionV >= 0 )
				nxMaterialDesc.dynamicFrictionV = dynamicFrictionV;

			if( staticFrictionV >= 0 )
				nxMaterialDesc.staticFrictionV = staticFrictionV;

			if( dir != NULL )
				nxMaterialDesc.dirOfAnisotropy = NxVec3( dir->x , dir->y , dir->z );

			m_gNxScene = phMan->getPhysxScene();

			m_gMaterialIndex = m_gNxScene->createMaterial( nxMaterialDesc )->getMaterialIndex();
		}

		//------------------------------------------------------------------------------------
		//! Set Restitution
		//------------------------------------------------------------------------------------
		void	MaterialX::setRestitution( float Restitution )
		{
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setRestitution( Restitution );
		}

		//------------------------------------------------------------------------------------
		//! Set Restitution
		//------------------------------------------------------------------------------------
		float	MaterialX::getRestitution( void )
		{
			return m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->getRestitution();
		}
		
		//------------------------------------------------------------------------------------
		//! Set Restitution
		//------------------------------------------------------------------------------------
		void	MaterialX::setStaticFriction( float staticFriction )
		{
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setStaticFriction( staticFriction );
		}
		
		//------------------------------------------------------------------------------------
		//! Get Static Friction
		//------------------------------------------------------------------------------------
		float	MaterialX::getStaticFriction( void )
		{
			return m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->getStaticFriction();
		}
		
		//------------------------------------------------------------------------------------
		//! Set Dynamic Friction
		//------------------------------------------------------------------------------------
		void	MaterialX::setDynamicFriction( float dynamicFriction )
		{
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setDynamicFriction( dynamicFriction );
		}

		//------------------------------------------------------------------------------------
		//! Get Dynamic Friction
		//------------------------------------------------------------------------------------
		float	MaterialX::getDynamicFriction( void )
		{
			return m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->getDynamicFriction();
		}

		//------------------------------------------------------------------------------------
		//! Set Dynamic FrictionV
		//------------------------------------------------------------------------------------
		void	MaterialX::setDynamicFrictionV( float dynamicFrictionV )
		{
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setDynamicFrictionV( dynamicFrictionV );
		}

		//------------------------------------------------------------------------------------
		//! Get Dynamic FrictionV
		//------------------------------------------------------------------------------------
		float	MaterialX::getDynamicFrictionV( void )
		{
			return m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->getDynamicFrictionV();				
		}

		//------------------------------------------------------------------------------------
		//! Set tDir Of Anisotropy
		//------------------------------------------------------------------------------------
		void	MaterialX::setDirOfAnisotropy( kge::math::Vector *dir )
		{
			NxVec3 pVec( dir->x , dir->y , dir->z );
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setDirOfAnisotropy( pVec );
		}

		//------------------------------------------------------------------------------------
		//! Set Static FrictionV
		//------------------------------------------------------------------------------------
		void	MaterialX::setStaticFrictionV( float staticFrictionV )
		{
			m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->setStaticFrictionV( staticFrictionV );
		}

		//------------------------------------------------------------------------------------
		//! Set Static FrictionV
		//------------------------------------------------------------------------------------
		float	MaterialX::getStaticFrictionV( void )
		{
			return m_gNxScene->getMaterialFromIndex( m_gMaterialIndex )->getStaticFrictionV();
		}

		//------------------------------------------------------------------------------------
		//! Material is Valid
		//------------------------------------------------------------------------------------
		char	MaterialX::isValid( void )
		{
			return 1;
		}

		//------------------------------------------------------------------------------------
		//! Set To Defalt Material
		//------------------------------------------------------------------------------------
		void	MaterialX::setToDefault( void )
		{
		}
		
		//------------------------------------------------------------------------------------
		//! Get Material Index
		//------------------------------------------------------------------------------------
		NxMaterialIndex MaterialX::getMaterialIndex( void )
		{
			return m_gMaterialIndex;
		}
	} // ph
} // kge

#endif //KGE_USE_PHYSX
