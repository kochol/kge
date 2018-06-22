// File name: ShaderGL.h
// Des: This class is for using shaders in OpenGL.
// Date: 22/10/1386 (jalali)
// Programmer: Pooya Shahinfar (Psiwn), Ali Akbar Mohamadi(Kochol)

#ifndef SHADERGL_H
#define SHADERGL_H

#include "../../include/kge/gfx/Shader.h"
#include "../../Libs/glew/glew.h"
#include <Cg/cg.h>
#include <Cg/cgGL.h>

namespace kge
{
namespace gfx
{
// This class is for using shaders in OpenGL.
class ShaderGL : public Shader
{
public:

	// Constructor
	ShaderGL(CGprogram program,
		const u32 handle, char* FileName, char* Name);

	// Destructor.
	~ShaderGL();

	// Get the pointer for accessing constants in the shader code.
	ShaderHandle GetConstatnt(const char* Name);

	// Set a shader varible with it's handler.
	bool SetConstant(ShaderHandle Handler, const float* Values, u32 Count);
	bool SetConstant(ShaderHandle Handler, const int* Values, u32 Count);

	// Active the shader in graphics card.
	void Render();

	// Returns the texture stage
	int GetTextureIndex(char* Name);

protected:

	CGprogram		m_Program;
	bool is_linked;

}; // ShaderGL

} // gfx

} // kge

#endif // SHADERGL_H
