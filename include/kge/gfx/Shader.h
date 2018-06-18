// File name: Shader.h
// Des: This class is for using shaders.
// Date: 3/5/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_SHADER_H
#define KGE_SHADER_H

#include "MaterialParams.h"
#include "../Resource.h"
#include "../kgedef.h"
#include "../core/Functor.h"

namespace kge
{
namespace sn
{
	class SceneNode;

}

namespace gfx
{
//! A handler for shader constant that you can change them with this handler.
typedef char* ShaderHandle;

//! This class is for using shaders.
class Shader: public Resource
{
public:

	//! Constructor
	Shader(const u32 Handle, const char* FileName, const char* Name, void* ExtraParam):
	  Resource(Handle, FileName, Name, ExtraParam), m_pFun(0) {}

	//! Destructor.
	virtual ~Shader() {}

	/*! Get the pointer for accessing constants in the shader code.
		The shader constants are write only.
		\param Name The constant name that you defined in the shader code.
		\return Returns the pointer that you must use it in the SetConstant function.
	*/
	virtual ShaderHandle GetConstatnt(const char* Name)
	{
		return NULL;
	}

	/*! Set a shader variable with it's handler.
		\param Handler The handler to the shader variable.
		\param Values The pointer to the values.
		\param Count Array size.
		\return Returns true if succeed.
	*/
	virtual bool SetConstant(ShaderHandle Handler, const float* Values, u32 Count) 
	{
		return false;
	}
	virtual	bool SetConstant(ShaderHandle Handler, const int* Values, u32 Count) 
	{
		return false;
	}

	/*! Set shader registers.
		\param Values The data you want to set in the vertex shader register.
		\param StartRegisterIndex The index of the start constant register like c3
		\param Count Amount of registers to be set. One register consists of 4 floats.
		\return Returns true if succeed.
	*/
	virtual bool SetConstant(const float* Values, u32 StartRegisterIndex,
										 u32 Count = 1) 
	{
		return false;
	}

	/*! Connect your function to this class with this function so the engine can call it.<br>
		Sample code:
		\code
		* class A
		* {
		* public:
		* 	void SetShaderConstants()
		* 	{
		* 		// Add your shader related code.
		* 		m_pShader->Set ...
		* 	}

		* protected:
		* 	kge::gfx::Shader* m_pShader;
		* };

		* void SetShaderConstants()
		* {
		* 	// Add your shader related code.
		* 	shader->Set ...
		* }

		* int main()
		* {
		* 	A a;
		* 	// Connect your pointer to member function.
		* 	shader->ConnectOnPreRender(kge::core::mem_fun(a, &A::SetShaderConstants));
		* 	// Or connect your pointer to function.
		* 	shader->ConnectOnPreRender(kge::core::ptr_fun(SetShaderConstants));
		* }

		\endcode
	*/
	virtual void ConnectOnPreRender(kge::core::Functor0<void>* pFun)
	{	m_pFun = pFun;	}

	/*! This function is for internal use.
	Before rendering the object this function calls user function.
	*/
	virtual void PreRender()
	{	
		if (m_pFun)
			m_pFun->Call();	
	}

	/*! This function is for internal use.
	Active the shader in graphics card.
	*/
	virtual void Render() { }

	//! Returns the texture stage
	virtual int GetTextureIndex(const char* Name) 
	{
		return 0;
	}

protected:
	kge::core::Functor0<void>* m_pFun;		/**< This is for calling user function. */

}; // Shader

// Shader Instance
class ShaderInstance 
{
public:

	ShaderInstance(): m_pFun(NULL), m_pMatParams(NULL), m_pVertexShader(NULL),
		m_pPixelShader(NULL), m_pUserData(NULL), m_bMatParamsChanged(true),
		m_pOwner(NULL) {	}

	~ShaderInstance() {}

	Shader						*	m_pVertexShader,
								*	m_pPixelShader;

	kge::core::Functor1
		<void,
		 ShaderInstance*>		*	m_pFun;

	void						*	m_pUserData;
	MaterialParams					m_MaterialParams;
	bool							m_bMatParamsChanged;

	sn::SceneNode				*	m_pOwner;

	ShaderInstance& operator = (const ShaderInstance& si)
	{
		m_pVertexShader = si.m_pVertexShader;
		m_pPixelShader  = si.m_pPixelShader;
		m_pFun			= si.m_pFun;
		m_pUserData		= si.m_pUserData;
		m_MaterialParams= si.m_MaterialParams;
		m_bMatParamsChanged = si.m_bMatParamsChanged;
		m_pMatParams    = si.m_pMatParams;
		m_pOwner        = si.m_pOwner;

		return *this;
	}

	//! Sets the material params for rendering
	void SetMaterialParams(MaterialParams* pVal) 
	{
		m_pMatParams = pVal; 
		m_bMatParamsChanged = false;
		m_pVertexShader = pVal->pVertexShader;
		m_pPixelShader  = pVal->pPixelShader;
		m_pFun = pVal->m_pFun;

	} // SetMaterialParams

protected:
	MaterialParams				*	m_pMatParams;	// This MaterialParams is given by material manager don't change its values

}; // ShaderInstance

} // gfx

} // kge

#endif // KGE_SHADER_H
