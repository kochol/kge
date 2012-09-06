#ifndef KGE_TEXTURE_H
#define KGE_TEXTURE_H

#include "Resource.h"

namespace kge
{
	namespace gfx
	{
		//! The interface for working with textures in engine
		class KGE_API Texture: public Resource
		{
		public:

			//! Destructor
			virtual ~Texture() {}

		};
	}
}

#endif // KGE_TEXTURE_H
