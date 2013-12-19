// File name: PixelShaderDX.h
// Des: This class is for using shaders in directx9.
// Date: 4/5/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifdef  WIN32
#ifndef PIXELSHADERDX_H
#define PIXELSHADERDX_H

#include "../../include/gfx/shader.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace kge
{
namespace gfx
{
// This class is for using shaders in DirectX9.
class PixelShaderDX : public Shader
{
public:

	// Constructor
	PixelShaderDX(IDirect3DDevice9* Device, const u32 handle,
		IDirect3DPixelShader9* pShader, ID3DXConstantTable* PConstTable
		, const char* FileName, const char* Name);

	// Destructor.
	~PixelShaderDX();

	// Get the pointer for accessing constants in the shader code.
	ShaderHandle GetConstatnt(const char* Name);

	// Set a shader variable with it's handler.
	bool SetConstant(ShaderHandle Handler, const float* Values, u32 Count);
	bool SetConstant(ShaderHandle Handler, const int* Values, u32 Count);

	// Set vertex shader registers.
	virtual bool SetConstant(const float* Values, u32 StartRegisterIndex,
		u32 Count);

	// Active the shader in graphics card.
	void Render();

	// Returns the texture stage
	int GetTextureIndex(const char* Name);

protected:
	IDirect3DDevice9	  * m_pDevice;			// D3D device.
	IDirect3DPixelShader9 * m_pShader;			// Pixel Shader
	ID3DXConstantTable    *	m_pConstTable;		// Pixel Shader Constant Table

}; // PixelShaderDX

} // gfx

} // kge

#endif // PIXELSHADERDX_H

#endif // WIN32
