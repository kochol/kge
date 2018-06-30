// File name: PixelShaderDX.cpp
// Des: This class is for using shaders in directx9.
// Date: 4/5/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "PixelShaderDX.h"
#include "../../include/kge/io/logger.h"
#include "../../include/kge/Device.h"

KGE_IMPORT extern kge::u32  PixelShaderSet;

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor.
	//------------------------------------------------------------------------------------
	PixelShaderDX::PixelShaderDX(IDirect3DDevice9* Device, const u32 handle, IDirect3DPixelShader9* vShader,
        ID3DXConstantTable* ConstTable, const char* FileName, const char* Name): Shader(handle, FileName, Name, NULL),					
		m_pShader(vShader), m_pConstTable(ConstTable), m_pDevice(Device)
	{
		m_pFun = NULL;
		if ( m_pConstTable )
			m_pConstTable->SetDefaults(m_pDevice);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor.
	//------------------------------------------------------------------------------------
	PixelShaderDX::~PixelShaderDX()
	{
		if ( m_pShader )
			m_pShader->Release();
		if ( m_pConstTable )
			m_pConstTable->Release();
		KGE_DELETE(m_pFun, Functor0);

		// delete from resource manager
		Device::GetSingletonPtr()->GetPixelShaderManager()->RemoveResource(this);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Get the pointer for accessing constants in the shader code.
	//------------------------------------------------------------------------------------
	ShaderHandle PixelShaderDX::GetConstatnt(const char* Name)
	{
		ShaderHandle h;
		h = (ShaderHandle)m_pConstTable->GetConstantByName(NULL, Name);
		if (!h)
			io::Logger::Log(io::ELM_Error, "Can't find constant %s in HLSL code.", Name);

		return h;

	} // GetConstatnt

	//------------------------------------------------------------------------------------
	// Set a shader variable with it's handler.
	//------------------------------------------------------------------------------------
	bool PixelShaderDX::SetConstant(ShaderHandle Handler, const float* Values, u32 Count)
	{
		if (!Handler)
			return false;

		if ( FAILED(m_pConstTable->SetFloatArray(m_pDevice, (D3DXHANDLE)Handler, Values, Count) ) )
		{
			io::Logger::Log("Can't set HLSL variable.", io::ELM_Error);
			return false;
		}

		return true;

	} // SetConstant

	//------------------------------------------------------------------------------------
	// Set a shader variable with it's handler.
	//------------------------------------------------------------------------------------
	bool PixelShaderDX::SetConstant(ShaderHandle Handler, const int* Values, u32 Count)
	{
		if (!Handler)
			return false;

		if ( FAILED(m_pConstTable->SetIntArray(m_pDevice, (D3DXHANDLE)Handler, Values, Count) ) )
		{
			io::Logger::Log("Can't set HLSL variable.", io::ELM_Error);
			return false;
		}

		return true;

	} // SetConstant

	//-----------------------------
	// Set vertex shader registers.
	//-----------------------------
	bool PixelShaderDX::SetConstant(const float* Values, u32 StartRegisterIndex,
		u32 Count)
	{
		if ( FAILED ( m_pDevice->SetPixelShaderConstantF( StartRegisterIndex, Values, Count ) ) )
		{
			io::Logger::Log(io::ELM_Error, "Can't set vertex shader variable. Start Register: %d Count: %d", 
				StartRegisterIndex, Count);
			return false;
		}

		return true;

	} // SetConstant(for assembly vertex shaders)

	//------------------------------------
	// Active the shader in graphics card.
	//------------------------------------
	void PixelShaderDX::Render()
	{
		if (PixelShaderSet != m_iHandle)
		{
			PixelShaderSet = m_iHandle;
			m_pDevice->SetPixelShader(m_pShader);
		}
	} // Render

	//------------------------------------------------------------------------------------
	// Returns the texture stage
	//------------------------------------------------------------------------------------
	int PixelShaderDX::GetTextureIndex( const char* Name )
	{
		ShaderHandle h = GetConstatnt(Name);
		if (!h)
			return -1;
		UINT count;
		D3DXCONSTANT_DESC d;

		m_pConstTable->GetConstantDesc(
			h,
			&d,
			&count);
		return d.RegisterIndex;

	} // GetTextureIndex

} // gfx

} // kge
