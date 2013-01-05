#include "TextureOGL.h"
#include "../include/Image.h"
#include "../include/Settings.h"
#include "../include/Logger.h"
#include "initGL.h"

namespace kge
{
    namespace gfx
    {
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
        TextureOGL::TextureOGL(Image* pImg): Texture(pImg)
        {
			// Create texture from image
			u8* pSrc = pImg->GetImageData();

			// Resize the texture if it is more than the MaxTextureSize
			Settings* pS = Settings::GetPointer();
			while ((m_iWidth > pS->MaxTextureSize || m_iHeight > pS->MaxTextureSize) && (m_iMipmapsCount > 1))
			{
				unsigned int rowSize = 0;
				unsigned int numRows = 0;

				if (m_iBpp)
				{
					rowSize = std::max<unsigned int>(1, m_iWidth / 4) * m_iBpp;
					numRows = std::max<unsigned int>(1, m_iHeight / 4);
				}
				else
				{
					rowSize = (m_iWidth * m_ibpp + 7) / 8;
					numRows = m_iHeight;
				}

				pSrc += rowSize * numRows;

				--m_iMipmapsCount;

				m_iWidth = m_iWidth >> 1;
				m_iHeight = m_iHeight >> 1;
				if (m_iWidth == 0) { m_iWidth = 1; }
				if (m_iHeight == 0) { m_iHeight = 1; }
			}

			// Finding texture format
			GLenum format = 0;
			bool Compressed = false;
			switch (m_Format)
			{
			case ETF_A8R8G8B8:
				format = GL_RGBA;
				break;

			case ETF_A8B8G8R8:
				format = GL_BGRA;
				break;

			case ETF_X8R8G8B8:
				format = GL_RGBA;	// TODO: I'm not sure about this
				break;

			case ETF_X8B8G8R8:
				format = GL_BGRA;	// TODO: I'm not sure about this
				break;

			case ETF_DXT1:
				format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				Compressed = true;
				break;


			case ETF_DXT3:
				format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				Compressed = true;
				break;

			case ETF_DXT5:
				format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				Compressed = true;
				break;

			default:
				io::Logger::Error("The texture format is not supported. Format = %s\tFileName = %s", TextureFormatName[m_Format], pImg->GetFileName());
				return;

			}

			// Create OpenGL texture
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &m_iTexID);
			glBindTexture(GL_TEXTURE_2D, m_iTexID);

			unsigned int rowSize = 0;
			unsigned int numRows = 0;
			unsigned int size = 0;
			unsigned int width = m_iWidth;
			unsigned int height = m_iHeight;

			for (unsigned int i = 0; i < m_iMipmapsCount; ++i)
			{
				if (m_iBpp)
				{
					rowSize = std::max<unsigned int>(1, width / 4) * m_iBpp;
					numRows = std::max<unsigned int>(1, height / 4);
				}
				else
				{
					rowSize = (width * m_ibpp + 7) / 8;
					numRows = height;
				}

				size = rowSize * numRows;

				// Send the texture data
				if (Compressed)
					glCompressedTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, size, 
						(GLvoid*)pSrc);
				else
					glTexImage2D(GL_TEXTURE_2D, i, format, width, height, 0, GL_BGR_EXT, 
						GL_UNSIGNED_BYTE, (GLvoid*)pSrc);

				pSrc += size;

				width = width >> 1;
				height = height >> 1;
				if (width == 0) { width = 1; }
				if (height == 0) { height = 1; }
			}

        } // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
        TextureOGL::~TextureOGL()
        {

        } // Destructor

    }
}
