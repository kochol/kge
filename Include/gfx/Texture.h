// File name: Texture.h
// Des: This class is for texture interface.
// Date: 6/6/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_TEXTURE_H
#define KGE_TEXTURE_H

#include "../Resource.h"
#include "../io/Logger.h"
#include "../enums.h"

namespace kge
{
namespace gfx
{

enum CompressType
{
	ECT_None,	/**< Don't use any compression*/
	ECT_DXT1,	/**< Use DXT1 compress format*/
	ECT_DXT3,	/**< Use DXT3 compress format*/
	ECT_DXT5	/**< Use DXT5 compress format*/

}; // CompressType

class ImageData;

/*! Texture class 
 */
class KGE_API Texture : public Resource
{
public:

	//! Constructor
	Texture(const u32 Handle, const char* FileName, const char* Name, void* ExtraParam):
	  Resource(Handle, FileName, Name, ExtraParam), m_bAA(false), m_bCompressed(false),
	  m_bStoreCopy(false), m_iMipmapsCount(-1), m_Tfmt(ETF_A8R8G8B8), m_iBpp(4), 
	  m_iWidth(0), m_iHeight(0) {}

	//! ImageData constructor
	Texture(ImageData* pImg);

	//! Destructor
	virtual ~Texture() {}

	//! Create an empty texture
	virtual bool CreateTexture(int Width, int Height, 
		TextureFormat fmt = ETF_A8R8G8B8, int numMipMaps = 0,
		bool StoreCopy = true) 
	{ 
		return false; 

	} // CreateTexture

	//! Sets the texture data
	virtual bool SetData(u8* Data, int DataSize, int mipmapLevel = 0)
	{
		return false;
	}

	//! Sets a region of texture data
	virtual bool SetData(int X, int Y, int Width, int Height, u8* Data, u32 DataSize, int mipmaplevel = 0)
	{
		return false;
	}

	//! Returns a power of 2 size for texture.
	static int GetPowerOf2Size(int size, int& mipmaplevel)
	{
		int t = 0x01;
		int s =0;

		while (t < size)
		{
			t <<= 1;
			s++;
		}

		io::Logger::Log(io::ELM_Warning, "s = %d", s);
		mipmaplevel = s;
		
		return t;

	} // GetPowerOf2Size

	//! Check if the target is an Anti Aliased surface.
	bool IsAntiAliased() {return m_bAA;}

	virtual u8* GetCopyData(int MipMapLevel) {return NULL;}

	int GetNumberOfMipmapLevels() {return m_iMipmapsCount;}

	virtual void* GetD3DTexture() { return NULL; }

	virtual void* Lock() {return NULL;}

	virtual void UnLock() {}

	virtual TextureFormat GetFormat() {return m_Tfmt;}

	int GetWidth() {return m_iWidth;}

	int GetHeight() {return m_iHeight;}

protected:

	TextureFormat	m_Tfmt;

	//! Byte per pixel
	int				m_iBpp,
					m_iMipmapsCount,			//!< Number of mipmaps
					m_iWidth,				//!< Texture width
					m_iHeight,				//!< Texture height
					m_ibpp;

	//! Has Anti Aliasing
	bool				m_bAA,
						m_bCompressed,		//!< Is it compressed
						m_bStoreCopy;		//!< Stores a copy of texture data

}; // Texture

} // gfx

} // kge

#endif // KGE_TEXTURE_H
