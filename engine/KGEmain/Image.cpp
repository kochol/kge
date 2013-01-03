#include "../include/Image.h"
#include <string.h>

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

		//------------------------------------------------------------------------------------
		// Converts the image format
		//------------------------------------------------------------------------------------
		bool Image::Convert( TextureFormat ToThisFormat )
		{
			// Convert from ABGR8 to ARGB8
			if (m_Format == ETF_A8B8G8R8 && ToThisFormat == ETF_A8R8G8B8)
			{
				u8* pTemp = KGE_NEW_ARRAY(u8, m_iDataSize);
				for (int i = 0; i < m_iDataSize; i += 4)
				{
					pTemp[i + 0] = m_pImageData[i + 2]; // Red
					pTemp[i + 1] = m_pImageData[i + 1]; // Green
					pTemp[i + 2] = m_pImageData[i + 0]; // Blue
					pTemp[i + 3] = m_pImageData[i + 3]; // Alpha
				}

				KGE_DELETE_ARRAY(m_pImageData);
				m_pImageData = pTemp;
				m_Format = ETF_A8R8G8B8;
				return true;
			}

			return false;

		} // Convert

	} // gfx

} // kge
