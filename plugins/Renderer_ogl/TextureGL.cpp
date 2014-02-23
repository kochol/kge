// File name: TextureGL.cpp
// Des: This class is for OpenGL textures.
// Date: 20/12/1389
// Programmer: Pooya Shahinfar (Pswin)

#include "TextureGL.h"
#include "../../Include/gfx/ImageData.h"

#include "../../Libs/glew/glew.h"

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	#pragma comment(lib,"opengl32.lib")
#endif

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TextureGL::TextureGL( const u32 Handle, const char* FileName, const char* Name, void* ExtraParam ):
						Texture(Handle, FileName, Name, ExtraParam)
	{
		// enable 2d texturing
		glEnable(GL_TEXTURE_2D);

		// generate texture object
		glGenTextures(1,&m_uTexID);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TextureGL::TextureGL( ImageData* pImg ): Texture(pImg)
	{
		// Create texture from image
		u8* pSrc = pImg->GetImageData();

		// Resize the texture if it is more than the MaxTextureSize
		int maxsize = Device::GetSingletonPtr()->GetParams().MaxTextureSize;
		while ((m_iWidth > maxsize || m_iHeight > maxsize) && (m_iMipmapsCount > 1))
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
		GLenum internalformat = 0;
		bool Compressed = false;
		switch (m_Tfmt)
		{
		case ETF_A8R8G8B8:
			format = GL_RGBA;
			internalformat = GL_RGBA8;
			break;

		case ETF_A8B8G8R8:
			format = GL_BGRA;
			internalformat = GL_RGBA8;
			break;

		case ETF_X8R8G8B8:
			format = GL_RGBA;	// TODO: I'm not sure about this
			internalformat = GL_RGBA8;
			break;

		case ETF_X8B8G8R8:
			format = GL_BGRA;	// TODO: I'm not sure about this
			internalformat = GL_RGBA8;
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
			io::Logger::Error("The texture format is not supported. Format = %s\tFileName = %s", TextureFormatName[m_Tfmt], pImg->GetFileName());
			return;

		}

		// Create OpenGL texture
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &m_uTexID);
		glBindTexture(GL_TEXTURE_2D, m_uTexID);

		if (!GLEW_VERSION_3_0)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		}

		unsigned int rowSize = 0;
		unsigned int numRows = 0;
		unsigned int size = 0;
		unsigned int width = m_iWidth;
		unsigned int height = m_iHeight;

		for (unsigned int i = 0; i < m_iMipmapsCount; ++i)
		{
			if (Compressed)
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
			{
				glTexImage2D(GL_TEXTURE_2D, i, internalformat, width, height, 0, format,
					GL_UNSIGNED_BYTE, (GLvoid*)pSrc);
			}
			if (m_iMipmapsCount == 1)
			{
				if (GLEW_VERSION_3_0)
				{
					glGenerateMipmap(GL_TEXTURE_2D);
				}
			}

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
	TextureGL::~TextureGL()
	{
		// delete texture object
		glDeleteTextures(1,&m_uTexID);
		Device::GetSingletonPtr()->GetTextureManager()->RemoveResource(this);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Check for devil errors.
	//------------------------------------------------------------------------------------
	void TextureGL::CheckDevilErrors( const char* TextureName )
	{


	} // CheckDevilErrors

	//------------------------------------------------------------------------------------
	// CreateTexture
	//------------------------------------------------------------------------------------
	bool TextureGL::CreateTexture( int Width, int Height, TextureFormat fmt /*= ETF_A8R8G8B8*/
									, int numMipMaps /*= 0*/, bool StoreCopy /*= false*/ )
	{
		m_iWidth = Width;
		m_iHeight = Height;
		m_Tfmt = fmt;

		// enable 2D texturing
		glEnable(GL_TEXTURE_2D);

		// bind texture object
		glBindTexture(GL_TEXTURE_2D,m_uTexID);

		// sets texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		// cleaning errors
		glGetError();

		switch (fmt)
		{
		case ETF_A8:
			texInternalFormat = GL_R8;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RED;
			break;
		case ETF_A8L8:
			texInternalFormat = GL_RG8;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RG;
			break;
		case ETF_A8R8G8B8:
			texInternalFormat = GL_RGBA;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RGBA;
			break;
		case ETF_A32B32G32R32F:
			texInternalFormat = GL_RGBA32F_ARB;
			texDataFormat = GL_FLOAT;
			texDestFormat = GL_RGBA;
			break;
		case ETF_DXT1:
			texInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RGB;
		case ETF_DXT3:
			texInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RGBA;
		case ETF_DXT5:
			texInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			texDataFormat = GL_UNSIGNED_BYTE;
			texDestFormat = GL_RGBA;
		}

		// create texture
		glTexImage2D(GL_TEXTURE_2D,0,texInternalFormat,Width,Height
			,0,texDestFormat,texDataFormat,NULL);

		if (glGetError() != GL_NO_ERROR)
		{
			// TODO: log
			io::Logger::Error("Can't create texture");
		}
		return true;
	}

	//------------------------------------------------------------------------------------
	// SetData
	//------------------------------------------------------------------------------------
	bool TextureGL::SetData( u8* Data, int DataSize, int mipmapLevel /*= 0*/ )
	{
		// bind texture object
		Renderer::GetSingletonPtr()->SetTexture(this);

		glTexSubImage2D(GL_TEXTURE_2D, mipmapLevel, 0, 0, m_iWidth, m_iHeight
			, texDestFormat, texDataFormat, (GLvoid*)Data);

		return true;
	}

	//------------------------------------------------------------------------------------
	// SetData
	//------------------------------------------------------------------------------------
	bool TextureGL::SetData( int X, int Y, int Width, int Height, u8* Data, u32 DataSize, int mipmaplevel /*= 0*/ )
	{
		// bind texture object
		Renderer::GetSingletonPtr()->SetTexture(this);

		glTexSubImage2D(GL_TEXTURE_2D, mipmaplevel, X, Y, Width, Height
			, texDestFormat, texDataFormat, (GLvoid*)Data);

		return true;
	}

} // gfx

} // kge
