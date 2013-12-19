// File name: Image.h
// Des: This class is working with images.
// Date: 28/05/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_GFXIMAGE_H
#define KGE_GFXIMAGE_H

#include "../Resource.h"
#include "Texture.h"

namespace kge
{
namespace gfx
{
//! The image formats
enum ImageFormat
{
	EIF_RGB,
	EIF_RGBA,
	EIF_BGR,
	EIF_BGRA,
	EIF_A

}; // ImageFormat

class KGE_API Image: public Resource
{
public:

	//! Constructor don't use this but use ImageManager And call add from there.
	Image(const u32 Handle, const char* FileName, const char* Name, void* ExtraParam);

	//! Constructor for creating images later.
	Image(const char* Name);

	//! Destructor
	~Image();

	//! Creates an Image and store it.
	bool CreateImage(int Width, int Height, int Depth = 1, int BytesPerPixel = 4,
					 ImageFormat imgFormat = EIF_BGRA, void* Data = NULL);

	//! Returns the image data
	u8*	GetData(u32 MipMapLevel = 0);

	//! copies a region of the image to the OutData. Note: the OutData have not to be NULL
	u32 GetPixels(int XOff, int YOff, int ZOff, int Width, int Height, int Depth,
				   ImageFormat imgFormat, u8* OutData, u32 MipMapLevel = 0);

	//! Sets the image data
	u32 SetData(u8* pData);

	//! Changes the image data
	void SetPixels(int XOff, int YOff, int ZOff, int Width, int Height, int Depth,
		ImageFormat imgFormat, u8* OutData, u32 MipMapLevel = 0);

	//! Scales the image or current mipmap.
	u32 Scale(u32 Width, u32 Height, u32 Depth, u32 MipMapLevel = 0);

	//! Build MipMap for this Image
	u8 BuildMipMaps();

	//! Building the mipmap for a tile based image
	bool BuildTileMipMap(int NumCols, int NumRows);

	//! Returns the image width
	int GetWidth() { return m_iWidth; }

	//! Returns the image height
	int GetHeight() { return m_iHeight; }

	//! Returns the compressed data
	u8*	GetCompressData(CompressType ct, u32& Size);

	int GetBytePerPixel() {return m_iBpp;}
	ImageFormat GetFormat() {return m_eFmt;}

protected:

	u32			m_iImgID;		// Devil Image ID
	int			m_iWidth,		// Width 
				m_iHeight,		// Height
				m_ibpp,			// bits per pixel
				m_iBpp;			// Bytes per pixel
	ImageFormat	m_eFmt;

	//! Loads an image from a file
	bool LoadImageFromFile();

	// Check for devil errors.
	void CheckDevilErrors( const char* TextureName );

}; // Image

} // gfx

} // kge

#endif // KGE_GFXIMAGE_H
