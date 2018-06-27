// File name: TextureDX9.cpp
// Des: This class is for direct3d textures.
// Date: 22/6/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "RendererDX.h"
#include "texturedx9.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/gfx/ImageData.h"
// Include DevIL.
#include <IL/il.h>
#include <IL/ilu.h>
//#include "../../libs/devil/ilut.h"
#include <IL/devil_internal_exports.h>



//#include "../../Libs/physfs/physfs.h"

#include <string>
#include <fstream>
#include <iterator>
#include <algorithm>

//////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)

#define DDS_MAGIC 0x20534444 // "DDS "

struct DDS_PIXELFORMAT
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwFourCC;
	DWORD dwRGBBitCount;
	DWORD dwRBitMask;
	DWORD dwGBitMask;
	DWORD dwBBitMask;
	DWORD dwABitMask;
};

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define FOURCC_NULL ((D3DFORMAT)(MAKEFOURCC('N','U','L','L')))

typedef struct
{
	DWORD dwSize;
	DWORD dwFlags;
	DWORD dwHeight;
	DWORD dwWidth;
	DWORD dwPitchOrLinearSize;
	DWORD dwDepth;
	DWORD dwMipMapCount;
	DWORD dwReserved1[11];
	DDS_PIXELFORMAT ddspf;
	DWORD dwCaps;
	DWORD dwCaps2;
	DWORD dwCaps3;
	DWORD dwCaps4;
	DWORD dwReserved2;
} DDS_HEADER;

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////////

#ifdef WIN32

#define RGB16BIT(r,g,b) ((b) + ((g) << 5) + ((r) << 11)) // change 24 bit to 16 bit
#define RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TextureDX9::TextureDX9(LPDIRECT3DDEVICE9 pd3dDevice, Renderer* pRenderer,
		const u32 Handle, const char* FileName, const char* Name, void* ExtraParam):
	Texture(Handle, FileName, Name, ExtraParam), m_ppCopyData(NULL),
		m_pd3dDevice(pd3dDevice), m_pRenderer(pRenderer), m_pTexture(0), m_pDepth(0),
		m_pTextureSurface(0), m_pDepthSurface(0),
		m_tFormat(D3DFMT_UNKNOWN), m_dFormat(D3DFMT_UNKNOWN),
		m_ZStencilBuffer(false), m_mst(D3DMULTISAMPLE_NONE),
		m_msq(0), m_bRenderable(false)
	{
		
	} // Constructor

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TextureDX9::TextureDX9(LPDIRECT3DDEVICE9 pd3dDevice, Renderer* pRenderer, 
		ImageData* pImg): Texture(pImg), m_ppCopyData(NULL),
		m_pd3dDevice(pd3dDevice), m_pRenderer(pRenderer), m_pTexture(0), m_pDepth(0),
		m_pTextureSurface(0), m_pDepthSurface(0),
		m_tFormat(D3DFMT_UNKNOWN), m_dFormat(D3DFMT_UNKNOWN),
		m_ZStencilBuffer(false), m_mst(D3DMULTISAMPLE_NONE),
		m_msq(0), m_bRenderable(false)
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

		// Get d3d device
		LPDIRECT3DDEVICE9 pD3DDevice = (LPDIRECT3DDEVICE9)Renderer::GetSingletonPtr()->GetDirect3dDevice();
		
		// Finding texture format
		D3DFORMAT format = D3DFMT_UNKNOWN;
		bool bCompressed = false;
		switch (m_Tfmt)
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
			bCompressed = true;
			break;

		case ETF_DXT2:
			format = D3DFMT_DXT2;
			bCompressed = true;
			break;

		case ETF_DXT3:
			format = D3DFMT_DXT3;
			bCompressed = true;
			break;

		case ETF_DXT4:
			format = D3DFMT_DXT4;
			bCompressed = true;
			break;

		case ETF_DXT5:
			format = D3DFMT_DXT5;
			bCompressed = true;
			break;

		default:
			io::Logger::Error("The texture format is not supported. Format = %s\tFileName = %s", TextureFormatName[m_Tfmt], pImg->GetFileName());
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
			if (bCompressed)
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
		this->Release();

		if (m_bRenderable)
		{
			static_cast<RendererDX*>(m_pRenderer)->RemoveFromTexturesList(this);
		}

		if (m_ppCopyData)
		{
			for (int i = 0; i < m_iMipmapsCount; i++)
			{
				KGE_DELETE_ARRAY(m_ppCopyData[i]);
			}
			KGE_DELETE_ARRAY(m_ppCopyData);
		}

		Device::GetSingletonPtr()->GetTextureManager()->RemoveResource(this);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Load the texture from file.
	//------------------------------------------------------------------------------------
	bool TextureDX9::LoadTexture()
	{
		this->Release();
		m_bRenderable = false;

		//////////////////////////////////////////////////////////////////////////

		std::string name(m_pFileName);
		std::string::size_type pos = name.rfind(".dds");

		if (pos != std::string::npos && (pos + 4 == name.length()))
		{
			////// load from archive

			//std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			//std::string path("../../"); // 
			//std::string::size_type pos = name.find(path);
			//if (pos != std::string::npos)
			//{
			//	name = name.substr(pos + path.size());
			//}

			//if (!PHYSFS_exists(name.c_str())) return false;
			//PHYSFS_File* file = PHYSFS_openRead(name.c_str());

			////if (!PHYSFS_exists(m_pFileName)) return false;
			////PHYSFS_File* file = PHYSFS_openRead(m_pFileName);

			//const unsigned int length = PHYSFS_fileLength(file);

			//if (length < sizeof(DWORD) + sizeof(DDS_HEADER)) return false;

			//std::vector<unsigned char> buffer(length);

			//const unsigned int readLen = PHYSFS_read(file, &buffer[0], sizeof(unsigned char), length);

			//PHYSFS_close(file);

			//if (readLen != length)
			//{
			//	return false;
			//}

			//////

			////// load from file

			std::ifstream ifile(m_pFileName, std::ios::binary);
			std::vector<unsigned char> buffer;

			std::copy(std::istreambuf_iterator<char>(ifile), std::istreambuf_iterator<char>(),
				std::back_insert_iterator<std::vector<unsigned char> >(buffer));

			if (buffer.size() < sizeof(DWORD) + sizeof(DDS_HEADER)) { return false; }

			//////

			const unsigned char* pSrc = &buffer[0];
			const unsigned char* pEnd = &buffer[0] + buffer.size();
			const DWORD magicNumber = *(reinterpret_cast<const DWORD*>(pSrc));

			if (magicNumber != DDS_MAGIC) { return false; }

			pSrc += sizeof(DWORD);

			const DDS_HEADER header = *(reinterpret_cast<const DDS_HEADER*>(pSrc));

			pSrc += sizeof(DDS_HEADER);

			if (header.dwSize != sizeof(DDS_HEADER) ||
				header.ddspf.dwSize != sizeof(DDS_PIXELFORMAT) ||
				!header.dwWidth ||
				!header.dwHeight)
			{
				return false;
			}

			D3DFORMAT format = D3DFMT_UNKNOWN;
			unsigned int bitsPerPixel = 0;
			unsigned int bcBytesPerBlock = 0;

			if (header.ddspf.dwFlags & DDS_RGB)
			{
				if (header.ddspf.dwRGBBitCount == 32)
				{
					if (header.ddspf.dwABitMask == 0xff000000)
					{
						if (header.ddspf.dwRBitMask == 0x00ff0000 &&
							header.ddspf.dwGBitMask == 0x0000ff00 &&
							header.ddspf.dwBBitMask == 0x000000ff)
						{
							format = D3DFMT_A8R8G8B8;	
						}
						else if (header.ddspf.dwRBitMask == 0x000000ff &&
							header.ddspf.dwGBitMask == 0x0000ff00 &&
							header.ddspf.dwBBitMask == 0x00ff0000)
						{
							format = D3DFMT_A8B8G8R8;
						}
					}
					else if (header.ddspf.dwABitMask == 0x00000000)
					{
						if (header.ddspf.dwRBitMask == 0x00ff0000 &&
							header.ddspf.dwGBitMask == 0x0000ff00 &&
							header.ddspf.dwBBitMask == 0x000000ff)
						{
							format = D3DFMT_X8R8G8B8;	
						}
						else if (header.ddspf.dwRBitMask == 0x000000ff &&
							header.ddspf.dwGBitMask == 0x0000ff00 &&
							header.ddspf.dwBBitMask == 0x00ff0000)
						{
							format = D3DFMT_X8B8G8R8;
						}
					}
					bitsPerPixel = header.ddspf.dwRGBBitCount;
				}
				else if (header.ddspf.dwRGBBitCount == 24)
				{
					if (header.ddspf.dwABitMask == 0x00000000 &&
						header.ddspf.dwRBitMask == 0x00ff0000 &&
						header.ddspf.dwGBitMask == 0x0000ff00 &&
						header.ddspf.dwBBitMask == 0x000000ff)
					{
						//format = D3DFMT_R8G8B8;
					}
					bitsPerPixel = header.ddspf.dwRGBBitCount;
				}
			}
			else if (header.ddspf.dwFlags & DDS_FOURCC)
			{
				if (MAKEFOURCC('D', 'X', 'T', '1') == header.ddspf.dwFourCC)
				{
					format = D3DFMT_DXT1;
					bitsPerPixel = 4;
					bcBytesPerBlock = 8;
				}
				else if (MAKEFOURCC('D', 'X', 'T', '2') == header.ddspf.dwFourCC)
				{
					format = D3DFMT_DXT2;
					bitsPerPixel = 8;
					bcBytesPerBlock = 16;
				}
				else if (MAKEFOURCC('D', 'X', 'T', '3') == header.ddspf.dwFourCC)
				{
					format = D3DFMT_DXT3;
					bitsPerPixel = 8;
					bcBytesPerBlock = 16;
				}
				else if (MAKEFOURCC('D', 'X', 'T', '4') == header.ddspf.dwFourCC)
				{
					format = D3DFMT_DXT4;
					bitsPerPixel = 8;
					bcBytesPerBlock = 16;
				}
				else if (MAKEFOURCC('D', 'X', 'T', '5') == header.ddspf.dwFourCC)
				{
					format = D3DFMT_DXT5;
					bitsPerPixel = 8;
					bcBytesPerBlock = 16;
				}
			}

			if (D3DFMT_UNKNOWN == format)
			{
				io::Logger::Log("Can't create texture from ", m_pFileName, io::ELM_Error);
				return false;
			}

			unsigned int numMipmaps = header.dwMipMapCount ? header.dwMipMapCount : 1;
			m_iWidth = header.dwWidth;
			m_iHeight = header.dwHeight;
			m_tFormat = format;

			////////////////////////////
			while ((m_iWidth > m_pRenderer->GetMaxTextureSize() || m_iHeight > m_pRenderer->GetMaxTextureSize()) && (numMipmaps > 1))
			{
				unsigned int rowSize = 0;
				unsigned int numRows = 0;

				if (bcBytesPerBlock)
				{
					rowSize = std::max<unsigned int>(1, m_iWidth / 4) * bcBytesPerBlock;
					numRows = std::max<unsigned int>(1, m_iHeight / 4);
				}
				else
				{
					rowSize = (m_iWidth * bitsPerPixel + 7) / 8;
					numRows = m_iHeight;
				}

				pSrc += rowSize * numRows;

				--numMipmaps;

				m_iWidth = m_iWidth >> 1;
				m_iHeight = m_iHeight >> 1;
				if (m_iWidth == 0) { m_iWidth = 1; }
				if (m_iHeight == 0) { m_iHeight = 1; }
			}
			////////////////////////////

			if (FAILED(m_pd3dDevice->CreateTexture(m_iWidth, m_iHeight, numMipmaps,
				0, format, D3DPOOL_MANAGED, &m_pTexture, NULL)))
			{
				io::Logger::Log("Can't create texture from ", m_pFileName, io::ELM_Error);
				return false;
			}

			D3DLOCKED_RECT lockedRect = {0};
			unsigned int rowSize = 0;
			unsigned int numRows = 0;
			unsigned int size = 0;
			unsigned int width = m_iWidth;
			unsigned int height = m_iHeight;

			for (unsigned int i = 0; i < numMipmaps; ++i)
			{
				if (bcBytesPerBlock)
				{
					rowSize = std::max<unsigned int>(1, width / 4) * bcBytesPerBlock;
					numRows = std::max<unsigned int>(1, height / 4);
				}
				else
				{
					rowSize = (width * bitsPerPixel + 7) / 8;
					numRows = height;
				}

				size = rowSize * numRows;

				if (pSrc + size > pEnd)
				{
					this->Release();
					io::Logger::Log("Can't create texture from ", m_pFileName, io::ELM_Error);
					return false;
				}

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

		} 
		else
		{
			//////////////////////////////////////////////////////////////////////////
			ILuint ImgId; // IL image ID

			// Generate the main image name to use.
			ilGenImages (1, &ImgId);

			// Bind this image name.
			ilBindImage (ImgId);
			ilFormatFunc(IL_BGR);

			// Loads the image specified by File into the ImgId image.
			if (!ilLoadImage (m_pFileName))
			{
				CheckDevilErrors( m_pFileName );
				return false;
			}

			ILint w = ilGetInteger(IL_IMAGE_WIDTH);				// width
			ILint h = ilGetInteger(IL_IMAGE_HEIGHT);			// height
			ILint bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);	// Bits per pixel
			ILint Bpp = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // bytes per pixel
			m_iBpp    = Bpp;

			D3DFORMAT fmt;
			CompressType ct = (CompressType)(int)m_pExtraParam;
			ILuint Size = 0;

			if ( ct == ECT_DXT5 && m_pRenderer->CanDo(EGCC_DXT5) )
			{
				fmt  = D3DFMT_DXT5;
				m_tFormat = fmt;
				Size = ilGetDXTCData(NULL, 0, IL_DXT5);
			}
			else if ( ct == ECT_DXT3 && m_pRenderer->CanDo(EGCC_DXT3) )
			{
				fmt  = D3DFMT_DXT3;
				m_tFormat = fmt;
				Size = ilGetDXTCData(NULL, 0, IL_DXT3);
			}
			else if ( ct == ECT_DXT1 && m_pRenderer->CanDo(EGCC_DXT1) )
			{
				fmt  = D3DFMT_DXT1;
				m_tFormat = fmt;
				Size = ilGetDXTCData(NULL, 0, IL_DXT1);
			}
			else
			{
				ct = ECT_None;
				if (bpp == 8)
				{
					fmt = D3DFMT_R5G6B5;
					m_tFormat = fmt;
					ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
					bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
				}
				else if (bpp==16)
				{
					fmt = D3DFMT_R5G6B5;
					m_tFormat = fmt;
					ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
					bpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
				}
				else if (bpp==24)
				{
					fmt = D3DFMT_R5G6B5;
					m_tFormat = fmt;
				}
				else if (bpp==32)
				{
					fmt = D3DFMT_A8R8G8B8;
					m_tFormat = fmt;
				}
				else
				{
					io::Logger::Log(io::ELM_Error, "Direct3D renderer dose not support this texture type. Please try OpenGL renderer. texture name = %s", m_pFileName);
					ilDeleteImages(1, &ImgId);
					return false;
				}
			}

			DWORD usage = 0;
			bool HardwareMipMaps = false;

			// Enable hardware mip map generation.
			D3DCAPS9 caps;
			m_pd3dDevice->GetDeviceCaps(&caps);
			if (caps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
			{
				usage = D3DUSAGE_AUTOGENMIPMAP;
				HardwareMipMaps = true;
			}

			if (FAILED(m_pd3dDevice->CreateTexture(w, h, 0 // mipmap (0 = auto mipmap)
				, usage, fmt, D3DPOOL_MANAGED, 
				&m_pTexture, NULL)))
			{
				io::Logger::Log("Can't create texture from ", m_pFileName, io::ELM_Error);
				ilDeleteImages(1, &ImgId);
				return false;
			}

			D3DLOCKED_RECT d3dRect;
			if (FAILED(m_pTexture->LockRect(0, &d3dRect, NULL, 0)))
			{
				io::Logger::Log("Can't lock the texture. ", m_pFileName , io::ELM_Error);
				ilDeleteImages(1, &ImgId);
				return false;
			}

			int LineW;
			void* pData;
			if ( ct == ECT_DXT5 )
			{
				io::Logger::Log("DXT5");
				ilGetDXTCData(d3dRect.pBits, Size, IL_DXT5);
			}
			else if ( ct == ECT_DXT3 )
			{
				io::Logger::Log("DXT3");
				ilGetDXTCData(d3dRect.pBits, Size, IL_DXT3);
			}
			else if ( ct == ECT_DXT1 )
			{
				io::Logger::Log("DXT1");
				ilGetDXTCData(d3dRect.pBits, Size, IL_DXT1);
			}
			else
			{
				io::Logger::Log("No compression");
				if (fmt == D3DFMT_A8R8G8B8)
				{
					LineW = d3dRect.Pitch >> 2;
					pData = (ul32*)d3dRect.pBits;
				}
				else
				{
					LineW = d3dRect.Pitch >> 1;
					pData = (u16*)d3dRect.pBits;
				}

				if (bpp==24)
				{
					if (ilIsValid(IL_PCX, m_pFileName))
					{
						int z = h-1;
						u8* tData = ilGetData();
						for (int y = 0; y < h; y++)
						{
							for (int x = 0; x < w; x++)
							{
								u8	  B = (tData[((h-1-y)*w + x)*3 + 0]) >> 3,
									G = (tData[((h-1-y)*w + x)*3 + 1]) >> 2,
									R = (tData[((h-1-y)*w + x)*3 + 2]) >> 3;
								u16 color = RGB16BIT((int)(float)R,
									(int)(float)G,
									(int)(float)B);
								((u16*)d3dRect.pBits)[z*LineW+x]=color;
							}
							z--;
						}
					}
					else if (ilIsValid(IL_JPG, m_pFileName) || ilIsValid(IL_PNG, m_pFileName))
					{
						u8* tData = ilGetData();
						for (int y = 0; y < h; y++)
						{
							for (int x = 0; x < w; x++)
							{
								u8	  R = (tData[(y*w + x)*3 + 0]) >> 3,
									G = (tData[(y*w + x)*3 + 1]) >> 2,
									B = (tData[(y*w + x)*3 + 2]) >> 3;
								u16 color = RGB16BIT((int)(float)R,
									(int)(float)G,
									(int)(float)B);
								((u16*)d3dRect.pBits)[y*LineW+x]=color;
							}
						}
					}
					else
					{
						u8* tData = ilGetData();
						for (int y = 0; y < h; y++)
						{
							for (int x = 0; x < w; x++)
							{
								u8	  B = (tData[((h-1-y)*w + x)*3 + 0]) >> 3,
									G = (tData[((h-1-y)*w + x)*3 + 1]) >> 2,
									R = (tData[((h-1-y)*w + x)*3 + 2]) >> 3;
								u16 color = RGB16BIT((int)(float)R,
									(int)(float)G,
									(int)(float)B);
								((u16*)d3dRect.pBits)[y*LineW+x]=color;
							}
						}
					}
				}
				else if (bpp==32)
				{
					u8* tData = ilGetData();
					for (int y = 0; y < h; y++)
					{
						for (int x = 0; x < w; x++)
						{
							u8	  R = (tData[((h-1-y)*w + x)*4 + 0]),
								G = (tData[((h-1-y)*w + x)*4 + 1]),
								B = (tData[((h-1-y)*w + x)*4 + 2]),
								A = (tData[((h-1-y)*w + x)*4 + 3]);

							u32 color = RGB32BIT(A, R, G, B);
							((u32*)d3dRect.pBits)[y*LineW+x]=color;
						}
					}
				}
			}

			m_pTexture->UnlockRect(0);

			// Auto generate mipmaps on hardware
			if (HardwareMipMaps)
			{
				io::Logger::Log("MipMaping...");
				m_pTexture->GenerateMipSubLevels();
			}

			// TODO: Create mip maps if hardware can't create mipmaps.
			//iluBuildMipmaps();

			// We're done with our image, so we go ahead and delete it.
			ilDeleteImages(1, &ImgId);

			CheckDevilErrors( m_pFileName );
			//////////////////////////////////////////////////////////////////////////
		}
		//////////////////////////////////////////////////////////////////////////

		io::Logger::Log("Texture loaded: ", m_pFileName);

		return true;

	} // LoadTexture

	//------------------------------------------------------------------------------------
	// Create a renderable texture.
	//------------------------------------------------------------------------------------
	bool TextureDX9::CreateRenderableTexture(int width, int height, TextureFormat tf
		, D3DFORMAT DSfmt , bool CreateZStencilBuffer, bool AntiAliasing,
		D3DMULTISAMPLE_TYPE mst, ul32 msq)
	{
		this->Release();

		HRESULT hr;
		D3DFORMAT format = D3DFMT_A8R8G8B8;
		int iPool = D3DPOOL_DEFAULT;

		switch (tf)
		{
			//
			// ETF_A32B32G32R32F
			//
		case ETF_A32B32G32R32F:
			format = D3DFMT_A32B32G32R32F;
			m_iBpp = 16;
			break;

		case ETF_R32F:
			format = D3DFMT_R32F;
			m_iBpp = 4;
			break;

		case ETF_R16G16F:
			format = D3DFMT_G16R16F;
			m_iBpp = 4;
			break;

		case ETF_A8R8G8B8:
			format = D3DFMT_A8R8G8B8;
			m_iBpp = 4;
			break;

		case ETF_A8:
			format = D3DFMT_A8;
			m_iBpp = 1;
			break;

		case ETF_NULL:
			format = FOURCC_NULL;
			m_iBpp = 0;
			iPool = D3DMULTISAMPLE_4_SAMPLES;
			break;

		} // switch texture formats

		hr = m_pd3dDevice->CreateTexture(width, 
										 height, 
										 1, 
										 D3DUSAGE_RENDERTARGET, 
										 format, 
										 D3DPOOL_DEFAULT,
										 &m_pTexture,
										 NULL);

		if (FAILED(hr))
		{
			io::Logger::Log(io::ELM_Error,
				"Can't create renderable texture with width=%i, height=%i and textureformat=%s", 
				width, height, TextureFormatName[tf]);
			return false;
		}

		if (!m_bRenderable)
		{
			reinterpret_cast<RendererDX*>(m_pRenderer)->AddToTexturesList(this);
			m_bRenderable = true;
		}

		if (FAILED(m_pTexture->GetSurfaceLevel(0, &m_pTextureSurface)))
		{
			io::Logger::Log(io::ELM_Error,
				"Can't get renderable texture surface with width=%i, height=%i and textureformat=%s", 
				width, height, TextureFormatName[tf]);
			return false;
		}

		if (CreateZStencilBuffer && !AntiAliasing)
		{
			hr = m_pd3dDevice->CreateTexture(width, 
				height, 
				1, 
				D3DUSAGE_DEPTHSTENCIL, 
				DSfmt, 
				D3DPOOL_DEFAULT,
				&m_pDepth,
				NULL);
			if (FAILED(hr))
			{
				io::Logger::Log(io::ELM_Error,
					"Can't create depth stencil buffer for renderable texture with width=%i, height=%i and textureformat=%s", 
					width, height, TextureFormatName[tf]);
				m_pDepth = NULL;
				m_pDepthSurface = NULL;
				m_bAA = AntiAliasing;
				m_iWidth = width;
				m_iHeight = height;
				m_tFormat = format;
				m_dFormat = DSfmt;
				m_Tfmt = tf;
				return true;
			}

			if (FAILED(m_pDepth->GetSurfaceLevel(0, &m_pDepthSurface)))
			{
				io::Logger::Log(io::ELM_Error,
					"Can't get depth stencil buffer for renderable texture surface with width=%i, height=%i and textureformat=%s", 
					width, height, TextureFormatName[tf]);
				return false;
			}
		}

		m_bAA = AntiAliasing;
		m_iWidth = width;
		m_iHeight = height;
		m_tFormat = format;
		m_dFormat = DSfmt;
		m_Tfmt = tf;
		m_ZStencilBuffer = CreateZStencilBuffer;
		m_mst = mst;
		m_msq = msq;

		return true;

	} // CreateRenderableTexture

	//------------------------------------------------------------------------------------
	// Check for devil errors.
	//------------------------------------------------------------------------------------
	void TextureDX9::CheckDevilErrors( const char* TextureName )
	{
		ILenum Error;
		while ((Error = ilGetError())) 
		{
			if ( Error == IL_COULD_NOT_OPEN_FILE )
				kge::io::Logger::Log(io::ELM_Error , "Could not load texture: %s" , TextureName );
			else if (Error != IL_INVALID_EXTENSION)
				kge::io::Logger::Log( kge::io::ELM_Error , "Error from DevIL on %s: %s", TextureName , iluErrorString(Error));
		}
	} // CheckDevilErrors

	//------------------------------------------------------------------------------------
	// Create texture
	//------------------------------------------------------------------------------------
	bool TextureDX9::CreateTexture( int Width, int Height, TextureFormat f, int numMipMaps, bool StoreCopy )
	{
		this->Release();
		m_bRenderable = false;

		int maxmipmap = 0;

		if (numMipMaps >= 0)
		{
			Width  = GetPowerOf2Size(Width, maxmipmap);
			Height = GetPowerOf2Size(Height, maxmipmap);
			io::Logger::Log(io::ELM_Warning, "w=%d h=%d", Width, Height);
		}
		D3DFORMAT fmt = D3DFMT_UNKNOWN;
		switch (f)
		{
		case ETF_A8R8G8B8:
			fmt = D3DFMT_A8R8G8B8;
			m_iBpp = 4;
			break;
		case ETF_A8L8:
			fmt =  D3DFMT_A8L8;
			m_iBpp = 2;
			break;
		case ETF_A32B32G32R32F:
			fmt = D3DFMT_A32B32G32R32F;
			m_iBpp = 16;
			break;
		case ETF_A8:
			fmt = D3DFMT_A8;
			m_iBpp = 1;
			break;
		case ETF_DXT5:
			fmt  = D3DFMT_DXT5;
			m_iBpp = 1;
			m_bCompressed = true;
			break;

		default:
			io::Logger::Log(io::ELM_Warning, "No matching format type for %s", TextureFormatName[f]);
			break;
		}
		DWORD usage = 0;
		if (numMipMaps == 0 && !m_bCompressed)
			usage = D3DUSAGE_AUTOGENMIPMAP;
		if (numMipMaps == -1)
			numMipMaps = 1;
		//if (FAILED(m_pd3dDevice->CreateTexture(Width, Height, numMipMaps
		//	, usage | D3DUSAGE_DYNAMIC, fmt, D3DPOOL_DEFAULT, 
		//	&m_pTexture, NULL)))
		if (FAILED(m_pd3dDevice->CreateTexture(Width, Height, numMipMaps
			, usage, fmt, D3DPOOL_MANAGED, &m_pTexture, NULL)))
		{
			io::Logger::Log( io::ELM_Error, 
				"Can't create D3D texture. Name = %s, TextureFormat = %s ", 
				m_pName, TextureFormatName[f]);
			return false;
		}
		if (usage == D3DUSAGE_AUTOGENMIPMAP)
			numMipMaps = maxmipmap;
		m_iMipmapsCount = numMipMaps;
		if (StoreCopy)
		{
			if (m_iBpp > 0)
			{
				if (m_ppCopyData)
				{
					for (int i = 0; i < m_iMipmapsCount; i++)
					{
						KGE_DELETE_ARRAY(m_ppCopyData[i]);
					}
					KGE_DELETE_ARRAY(m_ppCopyData);
				}

				m_bStoreCopy = true;
				m_ppCopyData = KGE_NEW_ARRAY(u8*, numMipMaps);
				int w , h, t;
				for (int i = 0;i < numMipMaps; i++)
				{
					t = math::pow(2, i);
					w = Width / t;
					h = Height / t;
					m_ppCopyData[i] = KGE_NEW_ARRAY(u8, w * h * m_iBpp);
					memset(m_ppCopyData[i], 0, w * h * m_iBpp);
				}
			}
		}

		m_iWidth = Width;
		m_iHeight = Height;
		m_tFormat = fmt;
		m_Tfmt = f;

		return true;

	} // Create texture

	//------------------------------------------------------------------------------------
	// Sets the texture data
	//------------------------------------------------------------------------------------
	bool TextureDX9::SetData( u8* Data, int DataSize, int mipmapLevel  )
	{
		D3DLOCKED_RECT d3dRect;
		if (FAILED(m_pTexture->LockRect(mipmapLevel, &d3dRect, NULL, 0 )))
		{
			io::Logger::Log("Can't lock the texture. Name = ", m_pName , io::ELM_Error);
			return false;
		}

		memcpy(d3dRect.pBits, Data, DataSize);
		m_pTexture->UnlockRect(mipmapLevel);

		if (m_bStoreCopy && m_ppCopyData)
		{
		    memcpy(m_ppCopyData[mipmapLevel], Data, DataSize);
		}

		return true;

	} // SetData

	//------------------------------------------------------------------------------------
	// Sets a region of texture data
	//------------------------------------------------------------------------------------
	bool TextureDX9::SetData( int X, int Y, int Width, int Height, u8* Data, u32 DataSize, int mipmapLevel /*= 0*/ )
	{
		D3DLOCKED_RECT d3dRect;
		RECT rect;
		rect.left	 = X;
		rect.bottom = Y + Height;
		rect.right  = X + Width;
		rect.top    = Y;

		if (FAILED(m_pTexture->LockRect(mipmapLevel, &d3dRect, &rect, 0 )))
		{
			io::Logger::Log("Can't lock the texture. Name = ", m_pName , io::ELM_Error);
			return false;
		}

		u8* pTdata = (u8*)d3dRect.pBits;
		u8* pdata  = Data;
		u8* pCopyData = NULL;
		if (m_ppCopyData)
		{
			pCopyData = m_ppCopyData[mipmapLevel];
			pCopyData += Y * d3dRect.Pitch + X * m_iBpp;
		}
		for (int y = 0; y < Height; y++)
		{
			memcpy(pTdata, pdata, Width * m_iBpp);
 			if (m_bStoreCopy && m_ppCopyData)
 			{
 				memcpy(pCopyData, pdata, Width * m_iBpp);
 			}
			pTdata += d3dRect.Pitch;
			if (m_bStoreCopy && pCopyData)
			    pCopyData += d3dRect.Pitch;
			pdata  += Width * m_iBpp;
		}

		m_pTexture->UnlockRect(mipmapLevel);

		return true;

	} // SetData(Rect)

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void TextureDX9::OnLost()
	{
		//if (m_bRenderable/* || m_ppCopyData*/) this->Release();
		this->Release();

	} // OnLost

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void TextureDX9::OnReset()
	{
		this->CreateRenderableTexture(m_iWidth, m_iHeight, m_Tfmt, m_dFormat, m_ZStencilBuffer, m_bAA, m_mst, m_msq);
/*
		if (m_bRenderable)
		{
			this->CreateRenderableTexture(m_width, m_height, m_Tfmt, m_dFormat, m_ZStencilBuffer, m_bAA, m_mst, m_msq);
		}
		else if (m_ppCopyData)
		{
			if (!CreateTexture(m_width, m_height, m_Tfmt, m_iNumMipMap, false))
				return;
			
			for (int i = 0; i < m_iNumMipMap; ++i)
			{
				const int t = math::pow(2, i);
				const int w = m_width / t;
				const int h = m_height / t;

				unsigned int rowBytes = 0;
				unsigned int numRows = 0;
				if (m_tFormat == D3DFMT_DXT1 || m_tFormat == D3DFMT_DXT2 || m_tFormat == D3DFMT_DXT3 ||
					m_tFormat == D3DFMT_DXT4 || m_tFormat == D3DFMT_DXT5)
				{
					int numBlocksWide = 0;
					if (w > 0)
						numBlocksWide = math::Max(1, w / 4);
					int numBlocksHigh = 0;
					if (h > 0)
						numBlocksHigh = math::Max(1, h / 4);
					int numBytesPerBlock = (m_tFormat == D3DFMT_DXT1) ? 8 : 16;
					rowBytes = numBlocksWide * numBytesPerBlock;
					numRows = numBlocksHigh;
				}
				else
				{
					rowBytes = (w * (m_iBpp * 8) + 7) / 8;
					numRows = h;
				}

				D3DLOCKED_RECT lockedRect = {0};
				HRESULT hr = m_pTexture->LockRect(i, &lockedRect, NULL, 0);
				if (FAILED(hr))
				{
					io::Logger::Log("Can't lock the texture. Name = ", m_pName , io::ELM_Error);
					return;
				}

				u8* pSrc = m_ppCopyData[i];
				u8* pDest = reinterpret_cast<u8*>(lockedRect.pBits);
				for (unsigned int j = 0; j < numRows; ++j)
				{
					memcpy(pDest, pSrc, rowBytes);
					pDest += lockedRect.Pitch;
					pSrc += rowBytes;
				}

				m_pTexture->UnlockRect(i);
			}
		}
*/
	} // OnReset

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	void TextureDX9::Release()
	{
		if (m_pTextureSurface)
		{
			m_pTextureSurface->Release();
			m_pTextureSurface = 0;
		}

		if (m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = 0;
		}

		if (m_pDepthSurface)
		{
			m_pDepthSurface->Release();
			m_pDepthSurface = 0;
		}

		if (m_pDepth)
		{
			m_pDepth->Release();
			m_pDepth = 0;
		}

	} // Release

	void* TextureDX9::Lock()
	{
		D3DLOCKED_RECT d3dRect;
		if (FAILED(m_pTexture->LockRect(0, &d3dRect, NULL, 0 )))
		{
			io::Logger::Log("Can't lock the texture. Name = ", m_pName , io::ELM_Error);
			return NULL;
		}

		return d3dRect.pBits;

	}

	void TextureDX9::UnLock()
	{
		m_pTexture->UnlockRect(0);
	}

} // gfx

} // kge

#endif // WIN32
