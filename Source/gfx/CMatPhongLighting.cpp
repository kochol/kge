// File name: CMatPhongLighting.cpp
// Des: The phong lighting model
// Date: 5/9/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/gfx/CMatPhongLighting.h"
#include "../../Include/gfx/MaterialParams.h"
#include "../../Include/gfx/Shader.h"

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		CMatPhong::CMatPhong()
		{
			m_eType = ECMT_PhongLighting;

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		CMatPhong::~CMatPhong()
		{
		} // Destructor

		//------------------------------------------------------------------------------------
		// Returns the singleton pointer
		//------------------------------------------------------------------------------------
		CustomMaterial* CMatPhong::GetSingletonPtr()
		{
			static CMatPhong p;
			return &p;

		} // GetSingletonPtr

		//------------------------------------------------------------------------------------
		// Adds Vertex shader code
		//------------------------------------------------------------------------------------
		bool CMatPhong::CreateVertexShaderCode(MaterialParams* pMP, VertexShaderEntry ese)
		{
			switch (ese)
			{
				// Lighting
			case EVSE_Lighting:
				// Per-pixel Lighting
				if (pMP->eLightingMode == ELM_PerPixel || pMP->eLightingMode == ELM_Unknown)
				{
					// Normal
					pMP->AddVertexShaderOutput("float3 outNormal", EPU_Normal);
					pMP->m_sVsMain += "outNormal = Normal;\n";

					// Eye position and view direction
					pMP->AddVertexShaderOutput("float3 outViewDirection", EPU_Texcoord);
					pMP->m_sVsHeader += "float3 eyePos;\n";
					if (!pMP->NeedWorldMatrix)
					{
						pMP->NeedWorldMatrix = true;
						pMP->m_sVsHeader += "float4x4 matWorld;\n";
					}
					pMP->m_sVsMain	 += 
						"float3 fvObjectPosition = mul( inPosition, matWorld ).xyz;\n"\
						"outViewDirection = eyePos - fvObjectPosition;\n";

					// PositionW
					if (pMP->PointLightCount > 0)
					{
						pMP->AddVertexShaderOutput("float3 PositionW", EPU_Texcoord);
						pMP->m_sVsMain += "PositionW = mul( inPosition, matWorld ).xyz;\n";
					}
				}
				// Per vertex lighting
				else
				{
				}

				break;

				// Get vertex shader handles
			case EVSE_GetVertexShaderHandles:
				// Get eyePos constant
				pMP->m_pVshEyePos = pMP->pVertexShader->GetConstatnt("eyePos");

				break;

			}

			return true;

		} // CreateVertexShaderCode

		//------------------------------------------------------------------------------------
		// Adds pixel shader code
		//------------------------------------------------------------------------------------
		bool CMatPhong::CreatePixelShaderCode(MaterialParams* pMP, PixelShaderEntry ese)
		{
			switch (ese)
			{
				// Lighting code
			case EPSE_Lighting:
				// Per-pixel Lighting
				if (pMP->eLightingMode == ELM_PerPixel || pMP->eLightingMode == ELM_Unknown)
				{
					// PositionW
					if (pMP->PointLightCount > 0)
					{
						pMP->AddPixelShaderInput("float3 PositionW", EPU_Texcoord);
						pMP->m_sVsMain += "PositionW = mul( inPosition, matWorld ).xyz;\n";
					}

					// Normal
					pMP->AddPixelShaderInput("float3 inNormal", EPU_Normal);
					pMP->m_sPsMain += "float3 Normal = normalize(inNormal);\n";
					if (pMP->NormalMap)
					{
						pMP->m_sPsMain += "";
					}

					// Eye position and view direction
					pMP->AddPixelShaderInput("float3 inViewDirection", EPU_Texcoord);
					pMP->m_sPsMain += "float3 fvViewDirection = normalize(inViewDirection);\n";

					// Directional lighting
					pMP->m_sPsMain += "float3 fTotalNdotL = float3(0.0f, 0.0f, 0.0f);\n";
					pMP->m_sPsMain += "float3 fTotalRdotV = float3(0.0f, 0.0f, 0.0f);\n";

					if (pMP->DirLightCount == 1)
					{
						pMP->m_sPsHeader += "float3 fvLightDir;\n"\
							"float3 fvLightDirColor;\n";
					}
					else if (pMP->DirLightCount > 1)
					{
						pMP->m_sPsHeader += "float3 fvLightDir[";
						pMP->m_sPsHeader += pMP->DirLightCount;
						pMP->m_sPsHeader += "];\n";
						pMP->m_sPsHeader += "float3 fvLightDirColor[";
						pMP->m_sPsHeader += pMP->DirLightCount;
						pMP->m_sPsHeader += "];\n";
					}

					if (pMP->PointLightCount == 1)
					{
						pMP->m_sPsHeader += "float3 fvLightPos;\n";
						pMP->m_sPsHeader += "float3 fvLightPosColor;\n";
					}
					else if (pMP->PointLightCount > 1)
					{
						pMP->m_sPsHeader += "float3 fvLightPos[";
						pMP->m_sPsHeader += pMP->PointLightCount;
						pMP->m_sPsHeader += "];\n";
						pMP->m_sPsHeader += "float3 fvLightPosColor[";
						pMP->m_sPsHeader += pMP->PointLightCount;
						pMP->m_sPsHeader += "];\n";
					}

					if (pMP->DirLightCount == 1)
					{						
						// Diffuse lighting
						pMP->m_sPsMain += 
							"float fNdotL = max(0.0f, dot(Normal, fvLightDir));\n"\
							"fTotalNdotL += fvLightDirColor * fNdotL;\n";
						// Specular lighting
						pMP->m_sPsMain += 
							"if (fNdotL > 0.0f)\n"\
							"{\n"\
							"	float3 fvReflection = normalize(((2.0f * Normal) * (fNdotL)) - fvLightDir);\n"\
							"	fTotalRdotV += fvLightDirColor * pow(max(0.0f, dot(fvReflection, fvViewDirection)), 15);\n"\
							"}\n";

					}
					else if (pMP->DirLightCount > 1)
					{
						// Diffuse lighting
						pMP->m_sPsMain += "for (int di = 0; di < ";
						pMP->m_sPsMain += pMP->DirLightCount;
						pMP->m_sPsMain += 
							"; di++)\n"\
							"{\n";
						pMP->m_sPsMain += 
							"	float fNdotL = max(0.0f, dot(Normal, fvLightDir[di]));\n"\
							"	fTotalNdotL += fvLightDirColor[di] * fNdotL;\n";

						// Specular lighting
						pMP->m_sPsMain += 
							"	if  (fNdotL > 0.0f)\n"\
							"	{\n"\
							"		float3 fvReflection = normalize(((2.0f * Normal) * (fNdotL)) - fvLightDir[di]);\n";
						pMP->m_sPsMain += 
							"		fTotalRdotV += fvLightDirColor[di] * pow(max(0.0f, dot(fvReflection, fvViewDirection)), 15);\n"\
							"	}\n"\
							"}\n";

					}
					if (pMP->PointLightCount == 1)
					{						
						// Diffuse lighting
						pMP->m_sPsMain += 
							"float3 litdir = normalize(fvLightPos - PositionW);\n"\
							"float fNdotLP = max(0.0f, dot(Normal, litdir));\n"\
							"fTotalNdotL += fvLightPosColor * fNdotLP;\n";
						// Specular lighting
						pMP->m_sPsMain += "float3 fvReflectionP = normalize(((2.0f * Normal) * (fNdotLP)) - litdir);\n";
						pMP->m_sPsMain += "fTotalRdotV += fvLightPosColor * pow(max(0.0f, dot(fvReflectionP, fvViewDirection)), 15);\n";

					}
					else if (pMP->PointLightCount > 1)
					{
						// Diffuse lighting
						pMP->m_sPsMain += "for (int di = 0; di < ";
						pMP->m_sPsMain += pMP->PointLightCount;
						pMP->m_sPsMain += 
							"; di++)\n"\
							"{\n";
						pMP->m_sPsMain += 
							"	float3 litdir = normalize(fvLightPos[di] - PositionW);\n"\
							"	float fNdotLP = max(0.0f, dot(Normal, litdir));\n"\
							"	fTotalNdotL += fvLightPosColor[di] * fNdotLP;\n";
						// Specular lighting
						pMP->m_sPsMain += 
							"	float3 fvReflectionP = normalize(((2.0f * Normal) * (fNdotLP)) - litdir);\n";
						pMP->m_sPsMain += 
							"	fTotalRdotV += fvLightPosColor[di] * pow(max(0.0f, dot(fvReflectionP, fvViewDirection)), 15);\n"\
							"}\n";

					}

					pMP->m_sPsMain += "float3 Spec = float3(0.0f,0.0f,0.0f);\n";

					// Receive shadow
					if (pMP->ReceiveShadow)
					{
						pMP->m_sPsMain += "float fShadow = GetShadow(inProjTex);\n"\
							"fTotalNdotL *= fShadow;\n"\
							"if (fShadow > 0.9)\n";
					}

					// Calculate the specular color
 					if (!pMP->SpecularMap)
 						pMP->m_sPsMain += "Spec = fTotalRdotV;\n";
 					else
 						pMP->m_sPsMain += "Spec = fTotalRdotV * SpecularColor.xyz;\n";

					// Final Lighting
					pMP->m_sPsMain += "outColor.xyz = 0.2 * BaseColor.xyz + BaseColor.xyz * fTotalNdotL + Spec;\n";
					pMP->m_sPsMain += "outColor.a = BaseColor.a;\n";
				}
				// Per vertex lighting
				else
				{
				}

				break;

				// Get the pixel shader constants
			case EPSE_GetPixelShaderHandles:
				if (pMP->DirLightCount > 0)
				{
					pMP->m_pPshLightDir = pMP->pPixelShader->GetConstatnt("fvLightDir");
					pMP->m_pPshLightDirColor = pMP->pPixelShader->GetConstatnt("fvLightDirColor");
					pMP->m_pPshLightPos = pMP->pPixelShader->GetConstatnt("fvLightPos");
					pMP->m_pPshLightPosColor = pMP->pPixelShader->GetConstatnt("fvLightPosColor");
				}
				break;
			}
			return true;

		} // CreatePixelShaderCode

		//------------------------------------------------------------------------------------
		// Set the constants
		//------------------------------------------------------------------------------------
		void CMatPhong::SetShaderConstants( ShaderInstance* pSI )
		{
		} // SetShaderConstants

		//------------------------------------------------------------------------------------
		// Get a copy
		//------------------------------------------------------------------------------------
		//CustomMaterial* CMatPhong::GetCopy()
		//{
		//} // GetCopy

	} // gfx

} // kge
