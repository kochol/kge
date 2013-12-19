// File name: Material.h
// Des: This class is Material uses Physic Material
// Date: 19/12/1388
// Programmer: Hadi Robati (hadirobati)

#ifndef KGE_MATERIAL_H
#define KGE_MATERIAL_H

namespace kge
{
	namespace ph
	{
		class Material
		{
		public:

			//! Constructor
			Material() { /* Nothing To Do */ }

			//! Constructor
			Material( float Restitution , float staticFriction , float dynamicFriction , float dynamicFrictionV, 
				float staticFrictionV , kge::math::Vector *dir );

			virtual ~Material() { /* Nothing To Do */ }

			virtual void	setRestitution		( float Restitution ) = 0;

			virtual float	getRestitution		( void ) = 0;

			virtual void	setStaticFriction	( float staticFriction ) = 0;

			virtual float	getStaticFriction	( void ) = 0;

			virtual void	setDynamicFriction	( float dynamicFriction ) = 0;

			virtual float	getDynamicFriction	( void ) = 0;

			virtual void	setDynamicFrictionV	( float dynamicFrictionV ) = 0;

			virtual float	getDynamicFrictionV	( void ) = 0;

			virtual void	setDirOfAnisotropy	( kge::math::Vector *dir ) = 0;

			virtual void	setStaticFrictionV	( float staticFrictionV ) = 0;

			virtual float	getStaticFrictionV	( void ) = 0;

			virtual char	isValid				( void ) = 0;

			virtual void	setToDefault		( void ) = 0;

			virtual unsigned short getMaterialIndex( void ) = 0;
		};
	} // ph
} // kge

#endif // KGE_MATERIAL_H