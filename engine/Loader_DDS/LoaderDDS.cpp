#include "LoaderDDS.h"
#include "../include/Stream.h"

// byte-align structures
#ifdef _MSC_VER
#	pragma pack(push, packing)
#	pragma pack(1)
#	define PACK_STRUCT
#elif defined(__GNUC__)
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

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
} PACK_STRUCT;

#define DDS_FOURCC      0x00000004  // DDPF_FOURCC
#define DDS_RGB         0x00000040  // DDPF_RGB
#define DDS_RGBA        0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
#define DDS_LUMINANCE   0x00020000  // DDPF_LUMINANCE
#define DDS_ALPHA       0x00000002  // DDPF_ALPHA
#define FOURCC_NULL ((D3DFORMAT)(MAKEFOURCC('N','U','L','L')))

struct DDS_HEADER
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
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop, packing)
#endif

#undef PACK_STRUCT

namespace kge
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	LoaderDDS::LoaderDDS()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	LoaderDDS::~LoaderDDS()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Loads a resource from a FileSystem and return its pointer.
	//------------------------------------------------------------------------------------
	Resource* LoaderDDS::LoadResource( io::Stream* pStream )
	{
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
		m_width = header.dwWidth;
		m_height = header.dwHeight;
		m_tFormat = format;

		////////////////////////////
		while ((m_width > m_pRenderer->GetMaxTextureSize() || m_height > m_pRenderer->GetMaxTextureSize()) && (numMipmaps > 1))
		{
			unsigned int rowSize = 0;
			unsigned int numRows = 0;

			if (bcBytesPerBlock)
			{
				rowSize = std::max<unsigned int>(1, m_width / 4) * bcBytesPerBlock;
				numRows = std::max<unsigned int>(1, m_height / 4);
			}
			else
			{
				rowSize = (m_width * bitsPerPixel + 7) / 8;
				numRows = m_height;
			}

			pSrc += rowSize * numRows;

			--numMipmaps;

			m_width = m_width >> 1;
			m_height = m_height >> 1;
			if (m_width == 0) { m_width = 1; }
			if (m_height == 0) { m_height = 1; }
		}
		////////////////////////////

		if (FAILED(m_pd3dDevice->CreateTexture(m_width, m_height, numMipmaps,
			0, format, D3DPOOL_MANAGED, &m_pTexture, NULL)))
		{
			io::Logger::Log("Can't create texture from ", m_pFileName, io::ELM_Error);
			return false;
		}

		D3DLOCKED_RECT lockedRect = {0};
		unsigned int rowSize = 0;
		unsigned int numRows = 0;
		unsigned int size = 0;
		unsigned int width = m_width;
		unsigned int height = m_height;

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

		io::Logger::Log("Texture loaded: ", m_pFileName);

		return true;

		return NULL;

	} // LoadResource

} // kge
