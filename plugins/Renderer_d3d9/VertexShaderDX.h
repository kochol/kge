// File name: VertexShaderDX.h
// Des: This class is for using shaders in directx9.
// Date: 4/5/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifdef  WIN32
#ifndef VERTEXSHADERDX_H
#define VERTEXSHADERDX_H

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
	VertexShaderDX(IDirect3DDevice9* Device, const u32 handle,
        IDirect3DVertexShader9* vShader, ID3DXConstantTable* ConstTable,
        const char* FileName, const char* Name);

	// Destructor.
	~VertexShaderDX();

	// Get the pointer for accessing constants in the shader code.
	ShaderHandle GetConstatnt(const char* Name);

	// Set a shader variable with it's handler.
	bool SetConstant(ShaderHandle Handler, const float* Values, u32 Count);
	bool SetConstant(ShaderHandle Handler, const int* Values, u32 Count);

	// Set vertex shader registers.
	/*virtual*/ bool SetConstant(const float* Values, u32 StartRegisterIndex,
		u32 Count);

	// Active the shader in graphics card.
	void Render();

	// Returns the texture stage
	int GetTextureIndex(const char* Name);

protected:
	IDirect3DDevice9	  * m_pDevice;			// D3D device.
	IDirect3DVertexShader9* m_pShader;			// Vertex Shader
	ID3DXConstantTable    *	m_pConstTable;		// Vertex Shader Constant Table

}; // VertexShaderDX

} // gfx

} // kge

#endif // VERTEXSHADERDX_H

#endif // WIN32
