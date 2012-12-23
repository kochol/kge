#ifndef KGE_TEXTUREDX9_H
#define KGE_TEXTUREDX9_H

#include "../include/Texture.h"

class IDirect3DTexture9;

namespace kge
{
	namespace gfx
	{
		class TextureDX9: public Texture
		{
		public:
		
			//! Constructor
			TextureDX9(Image* pImg);
		
			//! Destructor
			~TextureDX9();
		
			IDirect3DTexture9	*	m_pTexture;

		protected:
			
			
		private:
		
		}; // TextureDX9

	} // gfx

} // kge

#endif // KGE_TEXTUREDX9_H
