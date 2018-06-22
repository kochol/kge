// File name: TextureDX9.h
// Des: This class is for direct3d textures.
// Date: 15/6/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_TEXTUREDX9_H
#define KGE_TEXTUREDX9_H

#ifdef WIN32

#include <d3d9.h>
#include "../../include/kge/gfx/Texture.h"
#include "../../include/kge/gfx/Renderer.h"

namespace kge
{
namespace gfx
{
	class ImageData;

// Direct3D texture
class TextureDX9 : public Texture
{
	friend class RendererDX;
public:

	// Constructor
	TextureDX9(LPDIRECT3DDEVICE9 pd3dDevice, Renderer* pRenderer,
		const u32 Handle, const char* FileName, const char* Name, void* ExtraParam);

	// Constructor
	TextureDX9(LPDIRECT3DDEVICE9 pd3dDevice, Renderer* pRenderer, ImageData* pImg);

	// Destructor
	~TextureDX9();

	// Load the texture from file.
	bool LoadTexture();

	// Create a render able texture.
	bool CreateRenderableTexture(int width, int height, TextureFormat tf, 
		D3DFORMAT DSfmt, bool CreateZStencilBuffer, bool AntiAliasing,
		D3DMULTISAMPLE_TYPE mst, ul32 msq);

	// Create texture
	bool CreateTexture(int Width, int Height, TextureFormat fmt, int numMipMaps,bool StoreCopy);

	// Sets the texture data
	bool SetData(u8* Data, int DataSize, int mipmapLevel );

	//! Sets a region of texture data
	bool SetData(int X, int Y, int Width, int Height, u8* Data, u32 DataSize, int mipmaplevel = 0);

	//!
	void OnLost();

	//!
	void OnReset();

	u8* GetCopyData(int MipMapLevel) 
	{
		if (m_ppCopyData)
			return m_ppCopyData[MipMapLevel];
		return NULL;
	}

	void* GetD3DTexture() { return (void*)m_pTexture; }

	virtual void* Lock();

	virtual void UnLock();

protected:

	LPDIRECT3DDEVICE9   m_pd3dDevice;
	LPDIRECT3DTEXTURE9	m_pTexture,
						m_pDepth;
	
	LPDIRECT3DSURFACE9  m_pTextureSurface,
						m_pDepthSurface;

	Renderer*			m_pRenderer;

	u8**				m_ppCopyData;

	// Check for devil errors.
	void CheckDevilErrors( const char* TextureName );

private:
	D3DFORMAT           m_tFormat;
	D3DFORMAT           m_dFormat;

	bool                m_ZStencilBuffer;
	D3DMULTISAMPLE_TYPE m_mst;
	ul32                m_msq;

	bool                m_bRenderable;
	void Release();

}; // TextureDX9

} // gfx

} // kge

#endif // WIN32

#endif // KGE_TEXTUREDX9_H
