#ifndef KGE_IMAGE_H
#define KGE_IMAGE_H

#include "Resource.h"
#include "enums.h"

namespace kge
{
	namespace gfx
	{
		//! The image resource class
		class KGE_API Image: public Resource
		{
			friend class Texture;
		public:
		
			//! Constructor
			Image(const u32 Handle, const char* FileName, u8* data, int datasize, int width, int height, 
				int BytePerPixel, int bitPerPixel, TextureFormat format, int mipmapcount);
		
			//! Destructor
			virtual ~Image();

			//! Returns the image data size
			virtual int GetImageDataSize() { return m_iDataSize; }

			//! Returns the image data pointer
			virtual u8* GetImageData() { return m_pImageData; }

			//! Converts the image format
			virtual bool Convert(TextureFormat ToThisFormat);
		
		protected:

			u8				*	m_pImageData;
			int					m_iWidth,
								m_iHeight,
								m_iDataSize,
								m_iBpp,
								m_ibpp,
								m_iMipmapsCount;
			TextureFormat		m_Format;
			
		private:
		
		}; // Image

	} // gfx

} // kge

#endif // KGE_IMAGE_H
