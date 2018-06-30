// File name: VertexShaderDX.cpp
// Des: This class is for using shaders in directx9.
// Date: 4/5/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "VertexShaderDX.h"
#include "../../include/kge/io/logger.h"
#include "../../include/kge/Device.h"

KGE_IMPORT extern kge::u32	VertexShaderSet;

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor.
	//------------------------------------------------------------------------------------
	VertexShaderDX::VertexShaderDX(IDirect3DDevice9* Device, const u32 handle, IDirect3DVertexShader9* vShader,
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
	VertexShaderDX::~VertexShaderDX()
	{
		if ( m_pShader )
			m_pShader->Release();
		if ( m_pConstTable )
			m_pConstTable->Release();
		if (m_pFun)
		{
			delete m_pFun;
			m_pFun = NULL;
		}

		// delete from resource manager
		Device::GetSingletonPtr()->GetVertexShaderManager()->RemoveResource(this);

	} // Destructor

	//------------------------------------------------------------------------------------
	// Get the pointer for accessing constants in the shader code.
	//------------------------------------------------------------------------------------
	ShaderHandle VertexShaderDX::GetConstatnt(const char* Name)
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
	bool VertexShaderDX::SetConstant(ShaderHandle Handler, const float* Values, u32 Count)
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
	bool VertexShaderDX::SetConstant(ShaderHandle Handler, const int* Values, u32 Count)
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
	bool VertexShaderDX::SetConstant(const float* Values, u32 StartRegisterIndex,
										   u32 Count)
	{
		if ( FAILED ( m_pDevice->SetVertexShaderConstantF( StartRegisterIndex, Values, Count ) ) )
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
	void VertexShaderDX::Render()
	{
		if (VertexShaderSet != m_iHandle)
		{
			VertexShaderSet = m_iHandle;
			m_pDevice->SetVertexShader(m_pShader);
		}

	} // Render

	//------------------------------------------------------------------------------------
	// Returns the texture stage
	//------------------------------------------------------------------------------------
	int VertexShaderDX::GetTextureIndex( const char* Name )
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
