// File name: CustomMaterial.cpp
// Des: The custom Material part class.
// Date: 4/5/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/gfx/CustomMaterial.h"
#include "../../include/kge/gfx/MaterialParams.h"
#include "../../include/kge/gfx/Shader.h"



namespace kge
{

namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	CustomMaterial::CustomMaterial(): m_eType(ECMT_Unknown)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	CustomMaterial::~CustomMaterial()
	{

	} // Destructor


	SimpleLightingCM* SimpleLightingCM::GetSingletonPtr()
	{
		static SimpleLightingCM me;

		return &me;

	} // GetSingletonPtr

	bool SimpleLightingCM::CreateVertexShaderCode( MaterialParams* pMP, VertexShaderEntry ese  )
	{
		// Per vertex lighting
		if (pMP->eLightingMode == ELM_PerVertex || pMP->eLightingMode == ELM_Unknown)
		{
			pMP->AddVertexShaderOutput("float3 fNdotL", EPU_Texcoord);
			pMP->m_sVsMain += "fNdotL = float3(0.0f, 0.0f, 0.0f);\n";

			if (pMP->DirLightCount == 1)
			{
				pMP->m_sVsMain += "fNdotL += fvLightDirColor * max(0.0f, dot(Normal, fvLightDir));\n";
			}
			else if (pMP->DirLightCount > 1)
			{
				pMP->m_sVsMain += "for (int di = 0; di < ";
				pMP->m_sVsMain += pMP->DirLightCount;
				pMP->m_sVsMain += "; di++)\n"\
					"fNdotL += fvLightDirColor[di] * max(0.0f, dot(Normal, fvLightDir[di]));\n";
			}

			if (pMP->PointLightCount == 1)
			{
				pMP->NeedWorldViewMatrix = true;
				// TODO: send light positions in local space then there is no need to transform vertices to WorldView space
				pMP->m_sVsHeader += "float4x4 matWorldView;\n";
				pMP->m_sVsMain += "float3 fvObjectPosition = mul( inPosition, matWorldView ).xyz;\n";
				pMP->m_sVsMain += "fNdotL += fvLightPosColor * max(0.0f, dot(Normal, normalize(fvLightPos - fvObjectPosition)));\n";
			}
			else if (pMP->PointLightCount > 1)
			{
				pMP->NeedWorldViewMatrix = true;
				pMP->m_sVsHeader += "float4x4 matWorldView;\n";
				pMP->m_sVsMain += "float3 fvObjectPosition = mul( inPosition, matWorldView ).xyz;\n";
				pMP->m_sVsMain += "for (int di = 0; di < ";
				pMP->m_sVsMain += pMP->PointLightCount;
				pMP->m_sVsMain += "; di++)\n"\
					"fNdotL += fvLightPosColor[di] * max(0.0f, dot(Normal, normalize(fvLightPos[di] - fvObjectPosition)));\n";
			}
		}
		// Per pixel lighting
		else
		{

		}

		return true;

	}

	bool SimpleLightingCM::CreatePixelShaderCode( MaterialParams* pMP, PixelShaderEntry ese )
	{

		return true;
	}
} // gfx

} // kge
