// File name: VertexShaderDX.h
// Des: This class is for using shaders in directx9.
// Date: 4/5/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifdef  WIN32
#ifndef SHADERDX_H
#define SHADERDX_H

#include "../../include/gfx/shader.h"
#include <d3d9.h>
#include <d3dx9.h>

namespace kge
{
namespace gfx
{
// This class is for using shaders in DirectX9.
class VertexShaderDX : public Shader
{
public:

	// Constructor
	VertexShaderDX(IDirect3DDevice9* Device, IDirect3DVertexShader9* vShader, ID3DXConstantTable* ConstTable,
			 IDirect3DPixelShader9* pShader, ID3DXConstantTable* PConstTable
			 ,char* FileName, char* Name);

	// Destructor.
	~VertexShaderDX();

	// Get the pointer for accessing constants in the shader code.
	ShaderHandle GetConstatnt(char* Name);

	// Set a shader varible with it's handler.
	bool SetVarible(ShaderHandle Handler, const float* Values, u32 Count);
	bool SetVarible(ShaderHandle Handler, const int* Values, u32 Count);

	// Set vertex shader registers.
	virtual bool SetConstant(const float* Values, u32 StartRegisterIndex,
		u32 Count);

	// Active the shader in graphics card.
	void Render();

	// Returns the texture stage
	int GetTextureIndex(char* Name);

protected:
	IDirect3DDevice9	  * m_pDevice;			// D3D device.
	IDirect3DVertexShader9* m_pVShader;			// Vertex Shader
	ID3DXConstantTable    *	m_pConstTable;		// Vertex Shader Constant Table
	IDirect3DPixelShader9 * m_pPShader;			// Pixel Shader
	ID3DXConstantTable    *	m_pPSConstTable;	// Pixel Shader Constant Table

}; // VertexShaderDX

} // gfx

} // kge

#endif // SHADERDX_H
#endif // WIN32