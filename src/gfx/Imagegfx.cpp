// File name: Image.cpp
// Des: This class is working with images.
// Date: 28/05/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#include "../../include/kge/gfx/Image.h"
#include "../../include/kge/ResourceManager.h"
#include "../../include/kge/Device.h"
#include "../../include/kge/io/Logger.h"
// Include DevIL.
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <IL/devil_internal_exports.h>

#pragma comment(lib, "DevIL.lib")
#pragma comment(lib, "ilu.lib")

namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor don't use this but use ImageManager And call add from there.
	//------------------------------------------------------------------------------------
	Image::Image( const u32 Handle, const char* FileName, const char* Name, void* ExtraParam ):
		Resource(Handle, FileName, Name, ExtraParam)
	{
		bool b = this->LoadImageFromFile();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Constructor for creating images later.
	//------------------------------------------------------------------------------------
	Image::Image(const char* Name):
		Resource(-1, NULL, Name, NULL)
	{
		this->m_iHandle = Device::GetSingletonPtr()->GetImageManager()->Add(this);

		// Generate the main image name to use.
		ilGenImages (1, &m_iImgID);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Image::~Image()
	{
		ilDeleteImage(m_iImgID);
		Device::GetSingletonPtr()->GetImageManager()->RemoveResource(this);
	}

	//------------------------------------------------------------------------------------
	// Creates an Image and store it.
	//------------------------------------------------------------------------------------
	bool Image::CreateImage( int Width, int Height, int Depth /*= 1*/, int BytesPerPixel /*= 4*/,
							ImageFormat imgFormat /*= EIF_BGRA*/, void* Data /*= NULL*/ )
	{
		m_eFmt = imgFormat;
		ILenum fmt;
		switch (imgFormat)
		{
		case EIF_RGB:
			fmt = IL_RGB;
			m_ibpp = 24;
			m_iBpp = 3;
			break;

		case EIF_RGBA:
			fmt = IL_RGBA;
			m_ibpp = 32;
			m_iBpp = 4;
			break;

		case EIF_BGR:
		    fmt = IL_BGR;
			m_ibpp = 24;
			m_iBpp = 3;
			break;

		case EIF_BGRA:
			fmt = IL_BGRA;
			m_ibpp = 32;
			m_iBpp = 4;
			break;

		case EIF_A:
			fmt = IL_ALPHA;
			m_ibpp = 8;
			m_iBpp = 1;
			break;
		}
		m_iWidth = Width;
		m_iHeight = Height;
		ilBindImage(m_iImgID);
		ILboolean b = ilTexImage(Width, Height, Depth, BytesPerPixel, fmt, IL_UNSIGNED_BYTE, Data);

		return b;

	} // CreateImage

	//------------------------------------------------------------------------------------
	// Returns the image data
	//------------------------------------------------------------------------------------
	u8* Image::GetData(u32 MipMapLevel)
	{
		ilBindImage(m_iImgID);
		if (!ilActiveMipmap(MipMapLevel))
		{
			CheckDevilErrors(m_pFileName);
			io::Logger::Log(io::ELM_Error, "Incorrect mipmap level for image=%", m_pName);
			return 0;
		}
		return ilGetData();

	} // GetData

	//------------------------------------------------------------------------------------
	// Returns a region of the image. Note: the OutData have not to be NULL
	//------------------------------------------------------------------------------------
	u32 Image::GetPixels( int XOff, int YOff, int ZOff, int Width, int Height, int Depth,
						  ImageFormat imgFormat, u8* OutData, u32 MipMapLevel )
	{
		ILenum fmt;
		switch (imgFormat)
		{
		case EIF_RGB:
			fmt = IL_RGB;
			break;

		case EIF_RGBA:
			fmt = IL_RGBA;
			break;

		case EIF_BGR:
			fmt = IL_BGR;
			break;

		case EIF_BGRA:
			fmt = IL_BGRA;
			break;
		}

		ilBindImage(m_iImgID);
		if (!ilActiveMipmap(MipMapLevel))
		{
			CheckDevilErrors(m_pFileName);
			io::Logger::Log(io::ELM_Error, "Incorrect mipmap level for image=%", m_pName);
			return 0;
		}
		ILuint i = 0;//ilCopyPixels(XOff, YOff, ZOff, Width, Height, Depth, fmt, IL_UNSIGNED_BYTE, OutData);
		CheckDevilErrors(m_pFileName);

		return i;

	} // GetPixels

	//------------------------------------------------------------------------------------
	// Sets the image data
	//------------------------------------------------------------------------------------
	u32 Image::SetData( u8* pData )
	{
		ilBindImage(m_iImgID);
		return ilSetData(pData);

	} // SetData

	//------------------------------------------------------------------------------------
	// Returns a region of the image. Note: the OutData have not to be NULL
	//------------------------------------------------------------------------------------
	void Image::SetPixels( int XOff, int YOff, int ZOff, int Width, int Height, int Depth,
						   ImageFormat imgFormat, u8* OutData, u32 MipMapLevel )
	{
		ILenum fmt;
		switch (imgFormat)
		{
		case EIF_RGB:
			fmt = IL_RGB;
			break;

		case EIF_RGBA:
			fmt = IL_RGBA;
			break;

		case EIF_BGR:
			fmt = IL_BGR;
			break;

		case EIF_BGRA:
			fmt = IL_BGRA;
			break;
		}

		ilBindImage(m_iImgID);
		if (!ilActiveMipmap(MipMapLevel))
		{
			CheckDevilErrors(m_pFileName);
			io::Logger::Log(io::ELM_Error, "Incorrect mipmap level for image=%", m_pName);
			return;
		}
		ilSetPixels(XOff, YOff, ZOff, Width, Height, Depth, fmt, IL_UNSIGNED_BYTE, OutData);
		CheckDevilErrors(m_pFileName);

	} // SetPixels

	//------------------------------------------------------------------------------------
	// Scales the image or current mipmap.
	//------------------------------------------------------------------------------------
	u32 Image::Scale( u32 Width, u32 Height, u32 Depth, u32 MipMapLevel /*= 0*/ )
	{
		ilBindImage(m_iImgID);
		if (!ilActiveMipmap(MipMapLevel))
		{
			CheckDevilErrors(m_pFileName);
			io::Logger::Log(io::ELM_Error, "Incorrect mipmap level for image=%", m_pName);
			return 0;
		}

		//iluImageParameter(ILU_FILTER, ILU_SCALE_BOX);

		ILimage		*ret = ilGetCurImage();

		ILimage		*Temp  = NULL;//iluScale_(ret, Width, Height, Depth);
		if (!ret)
			return false;

		if (!ilTexImage(Temp->Width, Temp->Height, Temp->Depth, Temp->Bpp, Temp->Format, Temp->Type, Temp->Data))
		{
			return false;
		}

		if (MipMapLevel == 0)
		{
			m_iWidth = Width;
			m_iHeight = Height;
		}

		return true;

	} // Scale

	//------------------------------------------------------------------------------------
	// Build MipMap for this Image
	//------------------------------------------------------------------------------------
	u8 Image::BuildMipMaps()
	{
		ilBindImage(m_iImgID);
		return 0;//iluBuildMipmaps();

	} // BuildMipMaps


	//------------------------------------------------------------------------------------
	// Building the mipmap for a tile based image
	//------------------------------------------------------------------------------------
	bool Image::BuildTileMipMap( int NumCols, int NumRows )
	{
		BuildMipMaps();

		int tW			= m_iWidth  / NumCols;
		int tH			= m_iHeight / NumRows;
		int mipmapLevel = 1;
		int maxmipmaplevel = 0;
		u8* pTempData	= KGE_NEW_ARRAY(u8, tW * tH * 4);
		int t = 1, tth, ttw;

		Image* imgTemp = KGE_NEW(Image)("Temp");
		imgTemp->CreateImage(tW, tH, 1, 4, EIF_RGBA, 0);

		int wi = m_iWidth;
		while (wi > NumCols)
		{
			wi /= 2;
			maxmipmaplevel++;
		}

		io::Logger::Log(io::ELM_Warning, "mmm = %d", maxmipmaplevel);

		for ( int j = 0; j < maxmipmaplevel - 1; j++)
		{
			for (int y = 0; y < NumRows; y++)
			{
				for (int x = 0; x < NumCols; x++)
				{
					GetPixels(x * tW , y * tH, 1, tW, tH, 1, EIF_BGRA, pTempData);
					imgTemp->SetData(pTempData);
					t = math::pow(2, mipmapLevel);
					ttw = tW / t;
					tth = tH / t;
					imgTemp->Scale(ttw, tth, 1);
					SetPixels(x * ttw, y * tth, 1, ttw, tth, 1, EIF_BGRA,
						imgTemp->GetData(), mipmapLevel);

				} // for x

			} // for y

			mipmapLevel++;

		} // for j

		// TODO: Not tested
		KGE_DELETE_ARRAY(pTempData);
		KGE_DELETE(imgTemp, Image);

		return true;

	} // BuildTileMipMap

	//------------------------------------------------------------------------------------
	// Loads an image from a file
	//------------------------------------------------------------------------------------
	bool Image::LoadImageFromFile()
	{
		// Generate the main image name to use.
		ilGenImages (1, &m_iImgID);

		// Bind this image name.
		ilBindImage (m_iImgID);
		ilFormatFunc(IL_BGRA);

		// Loads the image specified by File into the ImgId image.
		if (!ilLoadImage (m_pFileName))
		{
			CheckDevilErrors( m_pFileName );
			return false;
		}

		m_iWidth  = ilGetInteger(IL_IMAGE_WIDTH);			// width
		m_iHeight = ilGetInteger(IL_IMAGE_HEIGHT);			// height
		m_ibpp    = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);	// Bits per pixel
		m_iBpp    = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL); // bytes per pixel

		if (m_ibpp == 8)
		{
//			ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
//			m_ibpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
			m_eFmt = EIF_A;
			m_iBpp = 1;
		}
		else if (m_ibpp==16)
		{
			ilConvertImage(IL_BGR, IL_UNSIGNED_BYTE);
			m_ibpp = ilGetInteger(IL_IMAGE_BITS_PER_PIXEL);
			m_eFmt = EIF_BGR;
		}

		CheckDevilErrors( m_pFileName );

		return true;

	} // LoadImageFromFile

	//------------------------------------------------------------------------------------
	// Returns the compressed data
	//------------------------------------------------------------------------------------
	u8* Image::GetCompressData( CompressType ct, u32& Size )
	{
		ilBindImage(m_iImgID);
		//iluFlipImage();

		u8*	outdata = NULL;
		switch (ct)
		{
		case ECT_DXT1:
			Size = ilGetDXTCData(NULL, 0, IL_DXT1);
			outdata = KGE_NEW_ARRAY(u8, Size);
			ilGetDXTCData(outdata, Size, IL_DXT1);
			break;
		case ECT_DXT3:
			Size = ilGetDXTCData(NULL, 0, IL_DXT3);
			outdata = KGE_NEW_ARRAY(u8, Size);
			ilGetDXTCData(outdata, Size, IL_DXT3);
			break;
		case ECT_DXT5:
			Size = ilGetDXTCData(NULL, 0, IL_DXT5);
			outdata = KGE_NEW_ARRAY(u8, Size);
			ilGetDXTCData(outdata, Size, IL_DXT5);
			break;
		}

		return outdata;
	}

	//------------------------------------------------------------------------------------
	// Check for devil errors.
	//------------------------------------------------------------------------------------
	void Image::CheckDevilErrors( const char* TextureName )
	{
		ILenum Error;
		while ((Error = ilGetError()))
		{
			if ( Error == IL_COULD_NOT_OPEN_FILE )
				kge::io::Logger::Log(io::ELM_Error , "Could not load image: %s" , TextureName );
			else if (Error != IL_INVALID_EXTENSION)
				kge::io::Logger::Log( kge::io::ELM_Error , "Error from DevIL on %s: ", TextureName);// , iluErrorString(Error));
		}

	} // CheckDevilErrors

} // gfx

} // kge
