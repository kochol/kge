// File name: ShaderGL.cpp
// Des: This class is for using shaders in OpenGL.
// Date: 22/10/1386
// Programmer: Ali Akbar Moahammadi(kochol) Pooya Shahinfar (Psiwn)

#include "ShaderGL.h"
#include "../../include/kge/io/Logger.h"

namespace kge
{
namespace gfx
{
	void checkForCgError(const char *situation, const char* ShaderName = NULL)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR) 
		{
			io::Logger::Error("%s: %s: %s\n",
				ShaderName, situation, string);
		}
	}

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	ShaderGL::ShaderGL(CGprogram program,
		const u32 handle, char* FileName, char* Name): 
		Shader(handle, FileName, Name, NULL),
		m_Program(program)
	{

	} // Constructor

	//------------
	// Destructor.
	//------------
	ShaderGL::~ShaderGL()
	{
 	} // Destructor

	//------------------------------------------------------------
	// Get the pointer for accessing constants in the shader code.
	//------------------------------------------------------------
	ShaderHandle ShaderGL::GetConstatnt(const char* Name)
	{
		ShaderHandle sh = (ShaderHandle)cgGetNamedParameter(m_Program, Name);
		checkForCgError("GetConstant");
		if (!sh)
			io::Logger::Warning("%s shader parameter not found.", Name);
		return sh;

	} // GetConstatnt

	//----------------------------------------
	// Set a shader varible with it's handler.
	//----------------------------------------
	bool ShaderGL::SetConstant(ShaderHandle Handler, const float* Values, u32 Count)
	{
		cgSetParameterValuefr((CGparameter)Handler, Count, Values);
		checkForCgError("cgSetParameterValuefc");
		return true;

	} // SetVarible

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	bool ShaderGL::SetConstant(ShaderHandle Handler, const int* Values, u32 Count)
	{
		cgSetParameterValueir((CGparameter)Handler, Count, Values);
		checkForCgError("cgSetParameterValueic");
		return true;

	} // SetVarible

	//------------------------------------
	// Active the shader in graphics card.
	//------------------------------------
	void ShaderGL::Render()
	{
		cgGLBindProgram(m_Program);
		checkForCgError("cgGLBindProgram");

	} // Render

} // gfx

} // kge
