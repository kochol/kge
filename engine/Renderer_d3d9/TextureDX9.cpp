#include "TextureDX9.h"
#include "../include/Image.h"
#include "../include/Settings.h"
#include "../include/Logger.h"
#include "../include/Renderer.h"
#include <d3d9.h>

//! Renderer public pointer
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		TextureDX9::TextureDX9(Image* pImg): Texture(pImg)
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

			// Get d3d device
			LPDIRECT3DDEVICE9 pD3DDevice = (LPDIRECT3DDEVICE9)g_pRenderer->GetDirect3dDevice();

			// Finding texture format
			D3DFORMAT format = D3DFMT_UNKNOWN;
			switch (m_Format)
			{
			case ETF_A8R8G8B8:
				format = D3DFMT_A8R8G8B8;
				break;

			case ETF_A8B8G8R8:
				format = D3DFMT_A8B8G8R8;
				break;

			case ETF_X8R8G8B8:
				format = D3DFMT_X8R8G8B8;
				break;

			case ETF_X8B8G8R8:
				format = D3DFMT_X8B8G8R8;
				break;

			case ETF_DXT1:
				format = D3DFMT_DXT1;
				break;

			case ETF_DXT2:
				format = D3DFMT_DXT2;
				break;

			case ETF_DXT3:
				format = D3DFMT_DXT3;
				break;

			case ETF_DXT4:
				format = D3DFMT_DXT4;
				break;

			case ETF_DXT5:
				format = D3DFMT_DXT5;
				break;

			default:
				io::Logger::Error("The texture format is not supported. Format = %s\tFileName = %s", TextureFormatName[m_Format], pImg->GetFileName());
				return;

			}

			// Create D3D9 texture
			if (FAILED(pD3DDevice->CreateTexture(m_iWidth, m_iHeight, m_iMipmapsCount,
				0, format, D3DPOOL_MANAGED, &m_pTexture, NULL)))
			{
				io::Logger::Error("Can't create texture from %s", pImg->GetFileName());
				return;
			}

			D3DLOCKED_RECT lockedRect = {0};
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

				if (SUCCEEDED(m_pTexture->LockRect(i, &lockedRect, 0, 0)))
				{
					unsigned char* pDest = reinterpret_cast<unsigned char*>(lockedRect.pBits);

					for (unsigned int j = 0; j < numRows; ++j)
					{
						memcpy_s(pDest, lockedRect.Pitch, pSrc, rowSize);
						pDest += lockedRect.Pitch;
						pSrc += rowSize;
					}

					m_pTexture->UnlockRect(i);
				}

				width = width >> 1;
				height = height >> 1;
				if (width == 0) { width = 1; }
				if (height == 0) { height = 1; }
			}

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		TextureDX9::~TextureDX9()
		{

		} // Destructor

	} // gfx

} // kge
