// File name: TextureGL.h
// Des: This class is for OpenGL textures.
// Date: 20/12/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_TEXTUREGL_H
#define KGE_TEXTUREGL_H

#include "../../Include/gfx/Texture.h"
#include "../../Include/gfx/Renderer.h"

namespace kge
{
namespace gfx
{
	class ImageData;

// OpenGL texture
class TextureGL : public Texture
{
public:

	// Constructor
	TextureGL(
		const u32 Handle, const char* FileName, const char* Name, void* ExtraParam);

	// Constructor
	TextureGL(ImageData* pImg);

	// Destructor
	~TextureGL();

	//! returns id of the texture
	u32 GetID(void) const {return m_uTexID;};

	//! Create an empty texture
	virtual bool CreateTexture(int Width, int Height, 
		TextureFormat fmt = ETF_A8R8G8B8, int numMipMaps = 0,
		bool StoreCopy = false);

	//! Sets the texture data
	virtual bool SetData(u8* Data, int DataSize, int mipmapLevel = 0);

	//! Sets a region of texture data
	virtual bool SetData(int X, int Y, int Width, int Height, u8* Data,
							u32 DataSize, int mipmaplevel = 0);

protected:
	u32 m_uTexID;
	int texInternalFormat;
	int texDataFormat;
	int texDestFormat;

	// Check for devil errors.
	void CheckDevilErrors( const char* TextureName );

private:

}; // TextureGL
} // gfx
} // Kochol Game Engine

#endif // KGE_TEXTUREGL_H
