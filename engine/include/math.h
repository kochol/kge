//#############################################################################
//#	desc:	mathematical functions
//#	file:	math.h
//#	author:	Pooya Shahinfar (Pswin)
//#############################################################################

#ifndef KGE_MATH_H
#define KGE_MATH_H

#include <cmath>

// Streaming SIMD Extension headers
#if KGE_USE_SSE2
	#include <emmintrin.h>
#elif KGE_USE_SSE3
	#include <pmmintrin.h>
#elif KGE_USE_SSE4
	#include <smmintrin.h>
#endif


namespace kge
{
	namespace math
	{
		//=============================
		// constants
		//=============================
		
		// PI
		#ifdef PI
			#undef PI
		#endif
		#define PI ( 3.1415926535897932 )

		// Epsilon
		#ifdef EPSILON
			#undef EPSILON
		#endif
		#define EPSILON ( 0.00001f )
		
		//=============================
		// integers
		//=============================

		//! returns the absolute value of parameter _val
		inline int absi(int _val)
		{
			int tmp = _val >> 31; 
			return ( (_val ^ tmp) - tmp );
		}

		//! returns the negate absolute value of parameter _val
		inline int nabsi(int _val)
		{
			int tmp = _val >> 31; 
			return ( tmp - (_val ^ tmp) );
		}

		//! returns the sign of parameter _val
		inline int sgni(int _val)
		{
			return ( (_val >> 31) - (-_val >> 31) );
		}

		//! returns maximum value of paramters _val1 and _val2
		inline int maxi(int _val1, int _val2)
		{
			int tmp = _val1 - _val2;
			return ( _val1 -  ( tmp & ( tmp >> 31 ) ) );
		}

		//! returns minmum value of parameter _val1 and _val2
		inline int mini(int _val1, int _val2)
		{
			int tmp = _val1 - _val2;
			return ( _val1 -  ( tmp & ~( tmp >> 31 ) ) );
		}

		//! clamp value between lower and upper bound
		inline int clampi( const int _val, const int _min, const int _max )
		{
			return ( _val < _min ) ? _min : ( ( _val < _max ) ? _val  : _max );
		}
		
		//=============================
		// floats
		//=============================

		//! returns the absolute value of parameter _val
		inline float absf( float _val )
		{
			return ( _val > 0.0f ) ? (_val) : (-_val);
		}

		//! returns the negate absolute value of parameter _val
		inline float nabsf( float _val )
		{
			return ( _val > 0.0f ) ? (-_val) : (_val);
		}
		
		//! returns true if parameters _val1 and _val2 are equal
		inline bool equalf( float _val1, float _val2 )
		{
			float tmp = _val1 - _val2;
			if ( math::absf(tmp) > EPSILON )
				return false;
			return true;
		}

		//! returns the sign of parameter _val
		inline float sgnf( float _val )
		{
			if ( math::equalf( _val, 0.0f ) )
				return 0.0f;
			return (_val > 0.0f ) ? 1.0f : -1.0f ;
		}

		//! returns maximum value of paramters _val1 and _val2
		inline float maxf( float _val1, float _val2 )
		{
			return ( _val1 > _val2) ? _val1 : _val2;
		}

		//! returns minmum value of parameter _val1 and _val2
		inline float minf( float _val1, float _val2 )
		{
			return ( _val1 < _val2) ? _val1 : _val2;
		}

				//! clamp value between lower and upper bound
		inline float clampf( const float _val, const float _min, const float _max )
		{
			return ( _val < _min ) ? _min : ( ( _val < _max ) ? _val  : _max );
		}

		//=============================
		// 
		//=============================

		//! convert radian to degree
		inline int toDeg(float _rad)
		{
			return (int)180.0f * (_rad / PI);
		}

		//! convert degree to radian
		inline float toRad(int _deg)
		{
			return _deg * PI / 180.0f;
		}

		//! generate an random value between _min and _max
		inline float random(float _min,float _max)
		{
			float rndValue = (float)rand() / (float)RAND_MAX;
			return ( _min + ( _max-_min ) * rndValue );
		}


	} // math
} // kge
#endif // KGE_MATH_H