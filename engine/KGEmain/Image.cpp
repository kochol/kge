#include "../include/Image.h"

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Image::Image( const u32 Handle, const char* FileName, u8* data, int datasize, int width, int height, 
			int BytePerPixel, int bitPerPixel, TextureFormat format, int mipmapcount ):
				Resource(Handle, FileName, NULL, NULL), m_iWidth(width), m_iDataSize(datasize),
				m_iHeight(height), m_iBpp(BytePerPixel), m_ibpp(bitPerPixel), m_Format(format)
				, m_iMipmapsCount(mipmapcount)
		{
			// Copy image data
			m_pImageData = KGE_NEW_ARRAY(u8, datasize);
			memcpy((void*)m_pImageData, (void*)data, datasize);

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		Image::~Image()
		{
			KGE_DELETE_ARRAY(m_pImageData);

		} // Destructor

	} // gfx

} // kge
