// File name: Shape.h
// Des: This class is Parent of Shpere and AABB and Capsule
// Date: 18/12/1388
// Programmer: Hadi Robati(hadirobati)

#ifndef SHAPE_H
#define SHAPE_H

namespace kge
{
	namespace math
	{
		enum KGE_SHAPES
		{
			KGE_SHAPE_AABB=0,
			KGE_SHAPE_SPHERE,
			KGE_SHAPE_CAPSULE
		};

		class KGE_API Shape
		{
		public:
			//! Constructor
			Shape(){ /* Do Nothing */ };
			
			//! Distructor
			virtual ~Shape() {}

			virtual int getType( void ) const = 0;

		}; // class Shape
	} // math
} // kge

#endif //SHAPE_H