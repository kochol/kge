#include "LoaderDDS.h"
#include "../include/Stream.h"
#include "../include/Logger.h"
#include "../include/Texture.h"
#include "../include/Renderer.h"
#include "../include/enums.h"
#include "../include/Image.h"

//! Renderer public pointer
KGE_IMPORT extern kge::gfx::Renderer*	g_pRenderer;

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
		m_aFileExtension.push_back("dds");
		m_aFileExtension.push_back("Dds");
		m_aFileExtension.push_back("DDs");
		m_aFileExtension.push_back("DDS");
		m_aFileExtension.push_back("DdS");
		m_aFileExtension.push_back("dDS");

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
		if (pStream->GetSize() < sizeof(DWORD) + sizeof(DDS_HEADER)) 
		{ 
			io::Logger::Error("Can't load %s DDS texture. The file size is very low and it can not be a valid DDS texture.", 
				core::String::Convert(pStream->GetName()).c_str());
			return NULL; 
		}

		// Create the buffer
		std::vector<unsigned char> buffer(pStream->GetSize());
		pStream->Read((void*)&buffer[0], pStream->GetSize());

		const u8* pSrc = &buffer[0];
		const u8* pEnd = &buffer[0] + buffer.size();
		const DWORD magicNumber = *(reinterpret_cast<const DWORD*>(pSrc));

		if (magicNumber != DDS_MAGIC) 
		{
			io::Logger::Error("Can't load %s DDS texture. The file header is incorrect and it can not be a valid DDS texture.", 
				core::String::Convert(pStream->GetName()).c_str());
			return NULL; 
		}

		pSrc += sizeof(DWORD);

		const DDS_HEADER header = *(reinterpret_cast<const DDS_HEADER*>(pSrc));

		pSrc += sizeof(DDS_HEADER);

		if (header.dwSize != sizeof(DDS_HEADER) ||
			header.ddspf.dwSize != sizeof(DDS_PIXELFORMAT) ||
			!header.dwWidth ||
			!header.dwHeight)
		{
			io::Logger::Error("Can't load %s DDS texture. The file header is incorrect and it can not be a valid DDS texture.", 
				core::String::Convert(pStream->GetName()).c_str());
			return NULL; 
		}

		// Finding the texture format
		gfx::TextureFormat format = gfx::ETF_NONE;
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
						format = gfx::ETF_A8R8G8B8;	
					}
					else if (header.ddspf.dwRBitMask == 0x000000ff &&
						header.ddspf.dwGBitMask == 0x0000ff00 &&
						header.ddspf.dwBBitMask == 0x00ff0000)
					{
						format = gfx::ETF_A8B8G8R8;
					}
				}
				else if (header.ddspf.dwABitMask == 0x00000000)
				{
					if (header.ddspf.dwRBitMask == 0x00ff0000 &&
						header.ddspf.dwGBitMask == 0x0000ff00 &&
						header.ddspf.dwBBitMask == 0x000000ff)
					{
						format = gfx::ETF_X8R8G8B8;	
					}
					else if (header.ddspf.dwRBitMask == 0x000000ff &&
						header.ddspf.dwGBitMask == 0x0000ff00 &&
						header.ddspf.dwBBitMask == 0x00ff0000)
					{
						format = gfx::ETF_X8B8G8R8;
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
				format = gfx::ETF_DXT1;
				bitsPerPixel = 4;
				bcBytesPerBlock = 8;
			}
			else if (MAKEFOURCC('D', 'X', 'T', '2') == header.ddspf.dwFourCC)
			{
				format = gfx::ETF_DXT2;
				bitsPerPixel = 8;
				bcBytesPerBlock = 16;
			}
			else if (MAKEFOURCC('D', 'X', 'T', '3') == header.ddspf.dwFourCC)
			{
				format = gfx::ETF_DXT3;
				bitsPerPixel = 8;
				bcBytesPerBlock = 16;
			}
			else if (MAKEFOURCC('D', 'X', 'T', '4') == header.ddspf.dwFourCC)
			{
				format = gfx::ETF_DXT4;
				bitsPerPixel = 8;
				bcBytesPerBlock = 16;
			}
			else if (MAKEFOURCC('D', 'X', 'T', '5') == header.ddspf.dwFourCC)
			{
				format = gfx::ETF_DXT5;
				bitsPerPixel = 8;
				bcBytesPerBlock = 16;
			}
		}

		if (format == gfx::ETF_NONE)
		{
			io::Logger::Error("Can't load %s DDS texture. The texture format is not supported.", 
				core::String::Convert(pStream->GetName()).c_str());
			return NULL; 
		}

		unsigned int numMipmaps = header.dwMipMapCount ? header.dwMipMapCount : 1;

		// calculate image size
		uint rowSize = 0;
		uint numRows = 0;
		uint size = 0;
		uint width = header.dwWidth;
		uint height = header.dwHeight;
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

			size += rowSize * numRows;

			width = width >> 1;
			height = height >> 1;
			if (width == 0) { width = 1; }
			if (height == 0) { height = 1; }
		}

		// Create image resource
		gfx::Image* pimg = KGE_NEW(gfx::Image)(0, core::String::Convert(pStream->GetName()).c_str(), (u8*)pSrc, size, header.dwWidth, header.dwHeight, bcBytesPerBlock, bitsPerPixel, format, numMipmaps);

		// TODO: Create texture from an Image
		gfx::Texture* pTex = g_pRenderer->CreateTexture(pimg);

		// Maybe we want to send it to image resource manager later but for now we simply delete it.
		KGE_DELETE(pimg, Image);

		io::Logger::Info("Texture loaded: %s", core::String::Convert(pStream->GetName()).c_str());

		return pTex;

	} // LoadResource

} // kge
