#ifndef KGE_TEXTURE_H
#define KGE_TEXTURE_H

#include "Resource.h"
#include "enums.h"

namespace kge
{
	namespace gfx
	{
		//   F O R W A R D   D E C L A R A T I O N		
		class Image;

		//! The interface for working with textures in engine
		class KGE_API Texture: public Resource
		{
		public:

			//! Constructor
			Texture(Image* pImg);

			//! Destructor
			virtual ~Texture() {}

			//! Gets the texture width
			int GetWidth() {return m_iWidth;}

			//! Gets the texture height
			int GetHeight() {return m_iHeight;}

		protected:

			int					m_iWidth,
								m_iHeight,
								m_iBpp,
								m_ibpp,
								m_iMipmapsCount;
			TextureFormat		m_Format;

		};
	}
}

#endif // KGE_TEXTURE_H
