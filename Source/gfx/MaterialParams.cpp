// File name: MaterialParams.cpp
// Des: The Material params class.
// Date: 28/4/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#include <stdio.h>

#include "../../Include/gfx/MaterialParams.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/core/mem_fun.h"
#include "../../Include/sn/Light.h"
#include "../../Include/sn/AnimatedMesh.h"
#include "../../Include/gfx/CMatPhongLighting.h"
#include "../../Include/efx/EffectManager.h"



namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	MaterialParams::MaterialParams(): DirLightCount(0), PointLightCount(0),
		SpotLightCount(0), ReceiveShadow(false), NormalMap(false),
		DetailMap(false), SkeletalMesh(false), CustomVertexShader(false),
		CustomPixelShader(false), eLightingType(ELIT_Simple),
		eFogType(EFT_NONE), TextureMap(true), eLightingMode(ELM_Unknown),
		pVertexShader(NULL), pPixelShader(NULL), BoneCount(0), m_bPSinputAdded(false),
		m_bVSinputAdded(false), m_pVshMatNorm(NULL), m_pVshLightDir(NULL),
		m_pVshLightPos(NULL), m_pVshMatWV(NULL), NeedWorldViewMatrix(false),
		Instanced(false), m_pVshMatBone(NULL), m_pVshMatWVP(NULL), m_pVshEyePos(NULL),
		m_pPshLightDir(NULL), SpecularMap(false), m_pLightCus(NULL),
		NeedWorldMatrix(false), m_pVshMatWorld(NULL), m_pVshMatLight(NULL),
		m_pVshLightDirColor(NULL), m_pVshLightPosColor(NULL), m_pPshLightDirColor(NULL),
		m_pPshLightPosColor(NULL), m_pPshLightPos(NULL), Deferred(false), AlphaMap(false),
		Frensel(false), m_pPshFrenselVal(NULL), m_pPshFrenselColor(NULL)
	{
		m_pFun = core::mem_fun(this, &MaterialParams::SetShaderConstants);
		for (int i = 0; i < EPU_Count; i++)
		{
			m_iPsUsagesIn[i] = 0;
			m_iPsUsagesOut[i] = 0;
			m_iVsUsagesIn[i] = 0;
			m_iVsUsagesOut[i] = 0;
		}

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	MaterialParams::~MaterialParams()
	{
		delete m_pFun;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Copy operator
	//------------------------------------------------------------------------------------
	void MaterialParams::operator=(const MaterialParams& mp )
	{
		DirLightCount			= mp.DirLightCount;
		PointLightCount			= mp.PointLightCount;
		SpotLightCount			= mp.SpotLightCount;
		ReceiveShadow			= mp.ReceiveShadow;
		NormalMap				= mp.NormalMap;
		DetailMap				= mp.DetailMap;
		SpecularMap				= mp.SpecularMap;
		SkeletalMesh			= mp.SkeletalMesh;
		CustomVertexShader		= mp.CustomVertexShader;
		CustomPixelShader		= mp.CustomPixelShader;
		eLightingType			= mp.eLightingType;
		eFogType				= mp.eFogType;
		eLightingMode			= mp.eLightingMode;
		TextureMap				= mp.TextureMap;
		BoneCount				= mp.BoneCount;
		Instanced				= mp.Instanced;
		NeedWorldMatrix			= mp.NeedWorldMatrix;
		NeedWorldViewMatrix		= mp.NeedWorldViewMatrix;
		Deferred				= mp.Deferred;
		AlphaMap				= mp.AlphaMap;
		Frensel					= mp.Frensel;
		std::map<int, CustomMaterial*>::const_iterator
			itCMat				= mp.m_mCustomMaterials.begin();
		std::map<int, CustomMaterial*>::const_iterator
			itCmatEnd			= mp.m_mCustomMaterials.end();
		for (; itCMat != itCmatEnd; ++itCMat)
		{
			m_mCustomMaterials[itCMat->second->GetType()] = itCMat->second->GetCopy();
		}
		if (mp.m_pLightCus)
			m_pLightCus = mp.m_pLightCus->GetCopy();

	} // operator=

	//------------------------------------------------------------------------------------
	// Checks the same vertex shader with other MaterialParams
	//------------------------------------------------------------------------------------
	bool MaterialParams::operator==(const MaterialParams& mp )
	{

		return false;

	} // operator==

	//------------------------------------------------------------------------------------
	// Checks the same vertex shader with other MaterialParams
	//------------------------------------------------------------------------------------
	bool MaterialParams::HasSameVertexShader( MaterialParams* mp )
	{
		if (Deferred)
		{
			if (
				   NormalMap	!= mp->NormalMap
				|| SkeletalMesh != mp->SkeletalMesh
				|| BoneCount	!= mp->BoneCount
				|| Instanced	!= mp->Instanced
				|| Frensel		!= mp->Frensel
				)
				return false;
		}
		else
		{
			if (DirLightCount != mp->DirLightCount
				|| PointLightCount != mp->PointLightCount
				|| SpotLightCount  != mp->SpotLightCount
				|| NormalMap != mp->NormalMap
				|| SkeletalMesh != mp->SkeletalMesh
				|| eLightingMode != mp->eLightingMode
				|| eLightingType != mp->eLightingType
				|| BoneCount != mp->BoneCount
				|| Instanced != mp->Instanced
				|| ReceiveShadow != mp->ReceiveShadow)
				return false;
		}

		if (mp->m_mCustomMaterials.size() != m_mCustomMaterials.size())
			return false;

		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= mp->m_mCustomMaterials.end();
		std::map<int, CustomMaterial*>::iterator
			itMP;
		for (; m_itCMat != m_mCustomMaterials.end(); ++m_itCMat)
		{
			itMP = mp->m_mCustomMaterials.find(m_itCMat->second->GetType());
			if (itMP == m_itCmatEnd)
				return false;
		}

		return true;

	} // HasSameVertexShader

	//------------------------------------------------------------------------------------
	// Checks the same pixel shader with other MaterialParams
	//------------------------------------------------------------------------------------
	bool MaterialParams::HasSamePixelShader( MaterialParams* mp )
	{
		if (Deferred)
		{
			if (
				   NormalMap	!= mp->NormalMap
				|| DetailMap	!= mp->DetailMap
				|| TextureMap	!= mp->TextureMap
				|| AlphaMap		!= mp->AlphaMap
				|| Frensel		!= mp->Frensel
				)
				return false;
		}
		else
		{
			if (DirLightCount != mp->DirLightCount
				|| PointLightCount != mp->PointLightCount
				|| SpotLightCount  != mp->SpotLightCount
				|| NormalMap != mp->NormalMap
				|| DetailMap != mp->DetailMap
				|| TextureMap != mp->TextureMap
				|| eLightingMode != mp->eLightingMode
				|| eLightingType != mp->eLightingType
				|| SpecularMap != mp->SpecularMap
				|| ReceiveShadow != mp->ReceiveShadow)
				return false;

		}

		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= mp->m_mCustomMaterials.end();
		std::map<int, CustomMaterial*>::iterator
			itMP;
		for (; m_itCMat != m_mCustomMaterials.end(); ++m_itCMat)
		{
			itMP = mp->m_mCustomMaterials.find(m_itCMat->second->GetType());
			if (itMP == m_itCmatEnd)
				return false;
		}

		return true;

	} // HasSamePixelShader

	//------------------------------------------------------------------------------------
	// Adds the custom material part to this material parameter
	//------------------------------------------------------------------------------------
	void MaterialParams::AddCustomMaterialPart( CustomMaterial* pCustomMat )
	{
		m_itCMat = m_mCustomMaterials.find(pCustomMat->GetType());
		if (m_itCMat == m_mCustomMaterials.end())
			m_mCustomMaterials[pCustomMat->GetType()] = pCustomMat;

	} // AddCustomMaterialPart

	//------------------------------------------------------------------------------------
	// Create the vertex shader code depends on the needs and returns its code
	//------------------------------------------------------------------------------------
	void MaterialParams::CreateVertexShaderCodeHLSL()
	{
		//*************************************
		// Default code
		//*************************************
		AddVertexShaderInput("float4 inPosition", EPU_Position);
		AddVertexShaderOutput("float4 outPosition", EPU_Position);
		// if need texture coordinates
		if (TextureMap)
		{
			AddVertexShaderInput("float2 inTexcoord", EPU_Texcoord);
			AddVertexShaderOutput("float2 outTexcoord", EPU_Texcoord);
		}

		m_sVsMain = "";

		// Call custom materials for changing the position
		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= m_mCustomMaterials.end();
		for (; m_itCMat != m_itCmatEnd; ++m_itCMat)
		{
			m_itCMat->second->CreateVertexShaderCode(this, EVSE_ChangePosition);
		}

		// if receives shadows
		if (ReceiveShadow && !Deferred)
		{
			AddVertexShaderOutput("float4 outProjTex", EPU_Texcoord);
			m_sVsHeader += "float4x4 matLight;\n";
			if (!Instanced)
			{
				m_sVsHeader += "float4x4 matWorld;\n";
				NeedWorldMatrix = true;
			}
		}

		// If it is an instanced mesh
		if (Instanced)
		{
			m_sVsHeader += "float4x4 matViewProjection;\n";
			AddVertexShaderInput("float4 inInstance0", EPU_Texcoord);
			AddVertexShaderInput("float4 inInstance1", EPU_Texcoord);
			AddVertexShaderInput("float4 inInstance2", EPU_Texcoord);
			AddVertexShaderInput("float4 inInstance3", EPU_Texcoord);
			m_sVsMain += "float4x4 matWorld;\n"\
				"matWorld = float4x4(";
			if (BoneCount > 0)
			{
				m_sVsHeader += "sampler2D sampBones;//: register(s257);\n";
				AddVertexShaderInput("int inBoneID", EPU_Color);
				m_sVsMain += "float4(inInstance0.xyz, 0.0), float4(inInstance1.xyz, 0.0), float4(inInstance2.xyz, 0.0), float4(inInstance0.w, inInstance1.w, inInstance2.w, 1.0));\n"\
					"float4 matB0 = tex2Dlod(sampBones, float4(inInstance3.xy + float2((float)inBoneID * 0.01171875              , 0.0), 0.0, 1.0));\n"\
					"float4 matB1 = tex2Dlod(sampBones, float4(inInstance3.xy + float2((float)inBoneID * 0.01171875 + 0.00390625 , 0.0), 0.0, 1.0));\n"\
					"float4 matB2 = tex2Dlod(sampBones, float4(inInstance3.xy + float2((float)inBoneID * 0.01171875 + 0.0078125  , 0.0), 0.0, 1.0));\n"\
					"float4x4 matBone = float4x4(float4(matB0.xyz, 0.0), float4(matB1.xyz, 0.0), float4(matB2.xyz, 0.0), float4(matB0.w, matB1.w, matB2.w, 1.0));\n"\
					"//inPosition += float4(matB0.w, matB1.w, matB2.w, 0.0);\n";
			}
			else
			{
				m_sVsMain += "inInstance0, inInstance1, inInstance2, inInstance3);\n";
			}

			m_sVsMain += "float4x4 matWVP = mul(matWorld, matViewProjection);\n";

			if (BoneCount > 0)
			{
				m_sVsMain += "matWVP = mul(matBone, matWVP);\n";
			}

			m_sVsMain += "outPosition = mul( inPosition, matWVP );\n";

			// if receive shadow
			if (ReceiveShadow && !Deferred)
			{
				m_sVsMain += "float4x4 matProj = mul(matWorld, matLight);";

				if (BoneCount > 0)
				{
					m_sVsMain += "matProj = mul(matBone, matProj);\n";
				}

				m_sVsMain += "outProjTex = mul( inPosition, matProj );\n";
			}
		}
		else
		{
			m_sVsHeader += "float4x4 matWorldViewProjection;\n";

			// If we need hardware skinning
			if (BoneCount > 0)
			{
				m_sVsHeader += "float4x4 matBones[";
				m_sVsHeader += BoneCount;
				m_sVsHeader += "];\n";
				AddVertexShaderInput("int inBoneID", EPU_Color);
				m_sVsMain   += "float4x4 matWVP = mul(matBones[inBoneID], matWorldViewProjection);\n";
				m_sVsMain	+= "outPosition = mul( inPosition, matWVP );\n";
			}
			else
			{
				m_sVsMain += "outPosition = mul( inPosition, matWorldViewProjection );\n";
			}

			// if receive shadow
			if (ReceiveShadow && !Deferred)
			{
				m_sVsMain += "float4x4 matProj = mul(matWorld, matLight);";

				if (BoneCount > 0)
				{
					m_sVsMain += "matProj = mul(matBones[inBoneID], matProj);\n";
				}

				m_sVsMain += "outProjTex = mul( inPosition, matProj );\n";
			}

		}

		if (TextureMap)
		{
			m_sVsMain	+= "outTexcoord = inTexcoord;\n";
		}

		//*************************************
		// Lighting code
		//*************************************
		int LightCount = DirLightCount + PointLightCount + SpotLightCount;

		// if no lighting is required
		if (eLightingType == ELIT_UnLit || LightCount == 0)
		{
		} // if ELIT_UnLit

		// Has light
		else
		{
			if (!Instanced)
				m_sVsHeader += "float3x3 matNormal;\n";
			else
			{
				m_sVsMain += "float3x3 matNormal = (float3x3)matWorld;\n";
			}

			AddVertexShaderInput("float3 inNormal", EPU_Normal);

			if (BoneCount > 0)
			{
				if (Instanced)
				{
					m_sVsMain   += "matNormal = mul((float3x3)matBone, matNormal);\n";
				}
				else
				{
					m_sVsMain   += "matNormal = mul((float3x3)matBones[inBoneID], matNormal);\n";
				}
			}

			m_sVsMain += "float3 Normal = mul(inNormal, matNormal);\n";
			if (Instanced)
			{
				m_sVsMain += "Normal = normalize(Normal);\n";
			}

			if (Deferred)
			{
				AddVertexShaderOutput("float4 outPosW", EPU_Texcoord);
 				if (BoneCount == 0)
				{
					if (Instanced)
						m_sVsMain += "outPosW = mul(inPosition, mul(matWorld, matWorldView));\n";
					else
						m_sVsMain += "outPosW = mul(inPosition, matWorldView);\n";
				}
				else
				{
					if (Instanced)
						m_sVsMain += "outPosW = mul(inPosition, mul(matBone, matWorldView));\n";
					else
						m_sVsMain += "outPosW = mul(inPosition, mul(matBones[inBoneID], matWorldView));\n";
				}
				AddVertexShaderOutput("float3 outNormal", EPU_Texcoord);
				m_sVsMain += "outNormal = Normal;\n";
				if (NeedWorldViewMatrix == false)
				{
					m_sVsHeader += "float4x4 matWorldView;\n";
					NeedWorldViewMatrix = true;
				}
				// Send the view direction to pixel shader
				if (Frensel)
				{
					AddVertexShaderOutput("float3 outViewDir", EPU_Texcoord);
					m_sVsHeader += "float3 eyePos;\n";
					if (Instanced == false)
					{
						if (!NeedWorldMatrix)
						{
							m_sVsHeader += "float4x4 matWorld;\n";
							NeedWorldMatrix = true;
						}
						if (BoneCount == 0)
							m_sVsMain += "float4 PW = mul(inPosition, matWorld);\n";
						else
							m_sVsMain += "float4 PW = mul(inPosition, mul(matBones[inBoneID], matWorld));\n";
					}
					else
					{
						if (BoneCount == 0)
							m_sVsMain += "float4 PW = mul(inPosition, matWorld);\n";
						else
							// I can't remember the algorithm for animated mesh instancing maybe below code is wrong
							m_sVsMain += "float4 PW = mul(inPosition, mul(matBones, matWorld));\n";
					}
					m_sVsMain += "outViewDir = normalize(eyePos - PW.xyz);\n";
				}
			}

			if (Deferred == false)
			{
				if (DirLightCount == 1)
				{
					m_sVsHeader += "float3 fvLightDir;\n"\
						"float3 fvLightDirColor;\n";
				}
				else if (DirLightCount > 1)
				{
					m_sVsHeader += "float3 fvLightDir[";
					m_sVsHeader += DirLightCount;
					m_sVsHeader += "];\n";
					m_sVsHeader += "float3 fvLightDirColor[";
					m_sVsHeader += DirLightCount;
					m_sVsHeader += "];\n";
				}

				if (PointLightCount == 1)
				{
					m_sVsHeader += "float3 fvLightPos;\n";
					m_sVsHeader += "float3 fvLightPosColor;\n";
				}
				else if (PointLightCount > 1)
				{
					m_sVsHeader += "float3 fvLightPos[";
					m_sVsHeader += PointLightCount;
					m_sVsHeader += "];\n";
					m_sVsHeader += "float3 fvLightPosColor[";
					m_sVsHeader += PointLightCount;
					m_sVsHeader += "];\n";
				}

				// Simple lighting
				if (eLightingType == ELIT_Simple)
				{
					SimpleLightingCM::GetSingletonPtr()->CreateVertexShaderCode(this, EVSE_Unknown);

				} // if ELIT_Simple

				// Phong lighting
				if (eLightingType == ELIT_Phong)
				{
					CMatPhong::GetSingletonPtr()->CreateVertexShaderCode(this, EVSE_Lighting);
					m_pLightCus = CMatPhong::GetSingletonPtr()->GetCopy();

				} // if ELIT_Phong
			}
		}

		m_sVertexShader = m_sVsHeader;
		m_sVertexShader += "void main(";
		m_sVertexShader += m_sVsInput;
		m_sVertexShader += ")\n{\n";
		m_sVertexShader += m_sVsMain;
		m_sVertexShader += "}\n";

		printf("\n\nVertex Shader\n\n%s\n", m_sVertexShader.ToCharPointer());

		pVertexShader = gfx::Renderer::GetSingletonPtr()->
			CreateVertexShaderFromString(m_sVertexShader.ToCharPointer(),"main", ESV_VS3);

		// Get shader handles
		if (pVertexShader)
		{
			m_pVshMatWVP = pVertexShader->GetConstatnt("matWorldViewProjection");
			m_pVshEyePos = pVertexShader->GetConstatnt("eyePos");
			if (Instanced)
				m_pVshMatWVP = pVertexShader->GetConstatnt("matViewProjection");
			if (BoneCount > 0 && !Instanced)
				m_pVshMatBone = pVertexShader->GetConstatnt("matBones");
			if (LightCount > 0)
				m_pVshMatNorm = pVertexShader->GetConstatnt("matNormal");
			if (DirLightCount > 0)
			{
				m_pVshLightDir = pVertexShader->GetConstatnt("fvLightDir");
				m_pVshLightDirColor = pVertexShader->GetConstatnt("fvLightDirColor");
			}
			if (PointLightCount > 0)
			{
				m_pVshLightPos = pVertexShader->GetConstatnt("fvLightPos");
				m_pVshLightPosColor = pVertexShader->GetConstatnt("fvLightPosColor");
			}
			if (NeedWorldViewMatrix)
				m_pVshMatWV = pVertexShader->GetConstatnt("matWorldView");
			if (ReceiveShadow)
				m_pVshMatLight = pVertexShader->GetConstatnt("matLight");
			if (NeedWorldMatrix)
				m_pVshMatWorld = pVertexShader->GetConstatnt("matWorld");
		}

		// Call custom materials for getting their handles
		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= m_mCustomMaterials.end();
		for (; m_itCMat != m_itCmatEnd; ++m_itCMat)
		{
			m_itCMat->second->CreateVertexShaderCode(this, EVSE_GetVertexShaderHandles);
		}
		if (m_pLightCus)
			m_pLightCus->CreateVertexShaderCode(this, EVSE_GetVertexShaderHandles);

		if (pVertexShader)
			pVertexShader->ConnectOnPreRender(core::mem_fun(this, &MaterialParams::SetVertexShaderConstants));

	} // CreateVertexShaderCodeHLSL

	//------------------------------------------------------------------------------------
	// Create the pixel shader code depends on the needs and returns its code
	//------------------------------------------------------------------------------------
	void MaterialParams::CreatePixelShaderCodeHLSL()
	{
		//*************************************
		// Default code
		//*************************************
		m_sPsHeader = "";
		m_sPsInput  = "";
		m_sPsOutput = "float4 outColor;\n ";
		m_sPsMain	= "";

		if (Deferred)
		{
			m_sPsHeader +=
				"struct PSOUT\n"\
				"{\n"\
				"	float4 Pos: COLOR0;\n"\
				"	float4 Dif: COLOR1;\n"\
				"	float4 Nor: COLOR2;\n"\
				"};\n";
			m_sPsOutput = "PSOUT psout;\n";
		}

		// if need texture coordinates
		if (TextureMap)
		{
			m_sPsHeader += "sampler2D baseMap;\n";
			AddPixelShaderInput("float2 inTexcoord", EPU_Texcoord);
			m_sPsMain   += "float4 BaseColor = tex2D( baseMap, inTexcoord );\n";
		}
		else
		{
			m_sPsMain += "float4 BaseColor = float4(1.0, 0.0, 0.0, 1.0);\n";
		}

		// Specular map
		if (SpecularMap)
		{
			m_sPsHeader += "sampler2D specMap;\n";
			m_sPsMain   += "float4 SpecularColor = tex2D( specMap, inTexcoord );\n";
		}

		// Normal map
		if (NormalMap)
		{

		}

		// Deferred shading
		if (Deferred)
		{
			m_sPsHeader +=
				"float4 encode (float3 n)\n"\
				"{\n"\
				"	float p = sqrt(n.z*8+8);\n"\
				"	return float4(n.xy/p + 0.5,0,0);\n"\
				"}\n";
			if (AlphaMap)
			{
				m_sPsMain +=
					"if (BaseColor.a < 0.3f)\n"\
					"{\n"\
					"	clip(-1);\n"\
					"}\n";
			}
			AddPixelShaderInput("float4 inPosW", EPU_Texcoord);
			AddPixelShaderInput("float3 inNormal", EPU_Texcoord);
			m_sPsMain +=
				"psout.Pos = length(inPosW);\n"\
				"float3 nN = normalize(inNormal);\n"\
				"psout.Nor = encode(nN);\n";
			m_sPsMain += "psout.Dif = BaseColor;\n";
			if (Frensel)
			{
				AddPixelShaderInput("float3 inViewDir", EPU_Texcoord);
				m_sPsHeader +=
					"float fFrensel;\n"\
					"float4 FrenselColor;\n";
				m_sPsMain +=
					"float fVdotN = max(0.0, dot(inViewDir, nN));\n"\
					"psout.Dif += saturate(fFrensel - fVdotN) * FrenselColor;\n";
			}
		}

		// Receive shadow
		if (ReceiveShadow && !Deferred)
		{
			efx::ShadowMapProjection* pSMP = efx::EffectManager::GetSingletonPtr()->GetShadowMapProjection();
			if (pSMP)
			{
				AddPixelShaderInput("float4 inProjTex", EPU_Texcoord);
				m_sPsHeader += pSMP->GetShaderCode();
			}
			else
				ReceiveShadow = false;
		}

		//*************************************
		// Lighting code
		//*************************************
		int LightCount = DirLightCount + PointLightCount + SpotLightCount;

		// if no lighting is required
		if (eLightingType == ELIT_UnLit)
		{
			m_sPsMain += "outColor = BaseColor;\n";

		} // if ELIT_UnLit
		else if (LightCount == 0)
		{
			m_sPsMain += "outColor = BaseColor * 0.2;\n";
		}

		// Has light
		else
		{
			if (Deferred == false)
			{
				// Simple lighting
				if (eLightingType == ELIT_Simple)
				{
					// Per vertex lighting
					if (eLightingMode == ELM_PerVertex || eLightingMode == ELM_Unknown)
					{
						AddPixelShaderInput("float3 fNdotL", EPU_Texcoord);
						m_sPsMain += "outColor.xyz = 0.2 * BaseColor.xyz + BaseColor.xyz * (fNdotL";
						if (ReceiveShadow)
						{
							m_sPsMain += " * GetShadow(inProjTex)\n";
						}
						m_sPsMain += ");\n outColor.a = BaseColor.a;";
					}
					// Per pixel lighting
					else
					{

					}

				} // if ELIT_Simple

				// Phong lighting
				if (eLightingType == ELIT_Phong)
				{
					CMatPhong::GetSingletonPtr()->CreatePixelShaderCode(this, EPSE_Lighting);

				} // if ELIT_Phong
			}
		}

		m_sPixelShader = m_sPsHeader;
		if (Deferred)
			m_sPixelShader += "PSOUT main(";
		else
			m_sPixelShader += "float4 main(";
		m_sPixelShader += m_sPsInput;
		if (Deferred)
			m_sPixelShader += ")\n{\n";
		else
			m_sPixelShader += "):COLOR0\n{\n";
		m_sPixelShader += m_sPsOutput;
		m_sPixelShader += m_sPsMain;
		if (Deferred)
			m_sPixelShader += "return psout;\n";
		else
			m_sPixelShader += "return outColor;\n";
		m_sPixelShader += "}\n";

		printf("\n\nPixel Shader\n\n%s\n", m_sPixelShader.ToCharPointer());

		pPixelShader = Renderer::GetSingletonPtr()->
			CreatePixelShaderFromString(m_sPixelShader.ToCharPointer(), "main", ESV_PS3);

		// Get constants
		if (pPixelShader)
		{
			m_pPshFrenselVal	= pPixelShader->GetConstatnt("fFrensel");
			m_pPshFrenselColor	= pPixelShader->GetConstatnt("FrenselColor");
		}

		// Call custom materials for getting their handles
		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= m_mCustomMaterials.end();
		for (; m_itCMat != m_itCmatEnd; ++m_itCMat)
		{
			m_itCMat->second->CreatePixelShaderCode(this, EPSE_GetPixelShaderHandles);
		}
		if (m_pLightCus)
			m_pLightCus->CreatePixelShaderCode(this, EPSE_GetPixelShaderHandles);

	} // CreatePixelShaderCodeHLSL

	void MaterialParams::SetVertexShaderConstants()
	{
	}

	void MaterialParams::SetShaderConstants( ShaderInstance* pSI )
	{
		// Set WorldViewProjection Matrix
		math::Matrix mat = Renderer::GetSingletonPtr()->GetTransForm(ETM_Projection) *
			Renderer::GetSingletonPtr()->GetTransForm(ETM_View) *
			Renderer::GetSingletonPtr()->GetTransForm(ETM_World);
		if (m_pVshMatWVP)
			pVertexShader->SetConstant(m_pVshMatWVP, mat.m_fMat, 16);

		if (m_pVshMatWV)
		{
			if (Instanced)
				mat = Renderer::GetSingletonPtr()->GetTransForm(ETM_View);
			else
				mat = Renderer::GetSingletonPtr()->GetTransForm(ETM_View) * (*(pSI->m_pOwner->GetFinalMatrix()));
			pVertexShader->SetConstant(m_pVshMatWV, mat.m_fMat, 16);
		}

		// Set world matrix
		if (m_pVshMatWorld)
			pVertexShader->SetConstant(m_pVshMatWorld, pSI->m_pOwner->GetFinalMatrix()->m_fMat, 16);

		// Set Light matrix
		if (m_pVshMatLight)
		{
			efx::ShadowMapProjection* pSMP = efx::EffectManager::GetSingletonPtr()->GetShadowMapProjection();
			pVertexShader->SetConstant(m_pVshMatLight, pSMP->GetMatrix()->m_fMat, 16);
		}

		// Bones
		if (m_pVshMatBone)
		{
			math::Matrix* mat = KGE_NEW_CLASS_ARRAY(math::Matrix, BoneCount);
			sn::Bone* pBones = ((sn::AnimatedMesh*)pSI->m_pOwner)->GetBones();
			for ( int i = 0; i < BoneCount; i++ )
			{
				mat[i] = pBones[i].m_matFinal;
			}
			pVertexShader->SetConstant(m_pVshMatBone, &mat[0].m_fMat[0], BoneCount * 16);
			delete[] mat;
		}

		// Lighting
		const core::PriorityArray<sn::Light*, float>& lights = Device::GetSingletonPtr()->GetSceneManager()->GetNearestLight
			(pSI->m_pOwner->GetPosition(), DirLightCount + PointLightCount);
		int LightCount = DirLightCount + PointLightCount;

		// Directional lighting
		float* fDir = NULL;
		float* fDirColor = NULL;
		if (LightCount > 0)
		{
			fDir = KGE_NEW_ARRAY(float, LightCount * 3);
			fDirColor = KGE_NEW_ARRAY(float, LightCount * 3);
		}
		if (m_pVshMatNorm)
		{
			math::Matrix matI;
			if (pSI->m_pOwner->GetParent())
			{
				matI = *pSI->m_pOwner->GetFinalMatrix();
			}
			else
			{
				math::Vector v = pSI->m_pOwner->GetRotation();
				matI.SetRotation(&v.x);
			}
			float f[9];
			f[0] = matI.m_fMat[0];
			f[1] = matI.m_fMat[1];
			f[2] = matI.m_fMat[2];
			f[3] = matI.m_fMat[4];
			f[4] = matI.m_fMat[5];
			f[5] = matI.m_fMat[6];
			f[6] = matI.m_fMat[8];
			f[7] = matI.m_fMat[9];
			f[8] = matI.m_fMat[10];
			pVertexShader->SetConstant(m_pVshMatNorm, f, 9);
		}
		if (m_pVshLightDir || m_pPshLightDir)
		{
			size_t sf = sizeof(float);
			for (int i = 0; i < DirLightCount; i++)
			{
				memcpy(&fDir[i * 3], &(lights[i]->GetLightData()->Direction.x), 3 * sf);
				memcpy(&fDirColor[i * 3], lights[i]->GetLightData()->Diffuse.c, 3 * sf);
			}
			if (m_pVshLightDir)
				pVertexShader->SetConstant(m_pVshLightDir, fDir, 3 * DirLightCount);
			if (m_pVshLightDirColor)
				pVertexShader->SetConstant(m_pVshLightDirColor, fDirColor, 3 * DirLightCount);
			if (m_pPshLightDir)
				pPixelShader->SetConstant(m_pPshLightDir, fDir, 3 * DirLightCount);
			if (m_pPshLightDirColor)
				pPixelShader->SetConstant(m_pPshLightDirColor, fDirColor, 3 * DirLightCount);
		}
		if (m_pVshLightPos || m_pPshLightPos)
		{
			size_t sf = sizeof(float);
			for (int i = DirLightCount; i < LightCount; i++)
			{
				memcpy(&fDir[i * 3], &(lights[i]->GetLightData()->Position.x), 3 * sf);
				memcpy(&fDirColor[i * 3], lights[i]->GetLightData()->Diffuse.c, 3 * sf);
			}
			if (m_pVshLightPos)
				pVertexShader->SetConstant(m_pVshLightPos, &fDir[DirLightCount * 3], 3 * PointLightCount);
			if (m_pVshLightPosColor)
				pVertexShader->SetConstant(m_pVshLightPosColor, &fDirColor[DirLightCount * 3], 3 * PointLightCount);
			if (m_pPshLightPos)
				pPixelShader->SetConstant(m_pPshLightPos, &fDir[DirLightCount * 3], 3 * PointLightCount);
			if (m_pPshLightPosColor)
				pPixelShader->SetConstant(m_pPshLightPosColor, &fDirColor[DirLightCount * 3], 3 * PointLightCount);
		}
		if (LightCount > 0)
		{
			KGE_DELETE_ARRAY(fDir);
			KGE_DELETE_ARRAY(fDirColor);
		}

		// Set eyePos
		if (m_pVshEyePos)
		{
			sn::SceneManager* smgr = Device::GetSingletonPtr()->GetSceneManager();
			sn::SceneNode	* pCam = smgr->GetActiveCamera();
			math::Vector		vp = pCam->GetPosition();
			pVertexShader->SetConstant(m_pVshEyePos, &vp.x, 3);
		}

		// Set frensel
		if (m_pPshFrenselVal)
		{
			pPixelShader->SetConstant(m_pPshFrenselVal, &pSI->m_pOwner->GetMaterial(0)->FrenselVal, 1);
			pPixelShader->SetConstant(m_pPshFrenselColor, pSI->m_pOwner->GetMaterial(0)->Frensel.c, 4);
		}

		// Tell the custom Materials to set their constants
		m_itCMat				= m_mCustomMaterials.begin();
		m_itCmatEnd				= m_mCustomMaterials.end();
		for (; m_itCMat != m_itCmatEnd; ++m_itCMat)
		{
			m_itCMat->second->SetShaderConstants(pSI);
		}
		if (m_pLightCus)
			m_pLightCus->SetShaderConstants(pSI);

	} // SetShaderConstants

	//------------------------------------------------------------------------------------
	// adds a pixel shader input string
	//------------------------------------------------------------------------------------
	void MaterialParams::AddPixelShaderInput(const char* ch, ParamUsage pu )
	{
		if (m_bPSinputAdded)
			m_sPsInput += ", ";
		m_sPsInput	   += "in ";
		m_sPsInput	   += ch;
		m_sPsInput	   += " : ";
		m_sPsInput	   += ParamUsageNames[pu];
		m_sPsInput	   += m_iPsUsagesIn[pu];
		m_bPSinputAdded = true;
		m_iPsUsagesIn[pu]++;

	} // AddPixelShaderInput

	//------------------------------------------------------------------------------------
	// adds a vertex shader input string
	//------------------------------------------------------------------------------------
	void MaterialParams::AddVertexShaderInput( const char* ch, ParamUsage pu )
	{
		if (m_bVSinputAdded)
			m_sVsInput += ", ";
		m_sVsInput	   += "in ";
		m_sVsInput	   += ch;
		m_sVsInput	   += " : ";
		m_sVsInput	   += ParamUsageNames[pu];
		m_sVsInput	   += m_iVsUsagesIn[pu];
		m_bVSinputAdded = true;
		m_iVsUsagesIn[pu]++;

	} // AddVertexShaderInput

	//------------------------------------------------------------------------------------
	// adds a vertex shader output string
	//------------------------------------------------------------------------------------
	void MaterialParams::AddVertexShaderOutput( const char* ch, ParamUsage pu )
	{
		if (m_bVSinputAdded)
			m_sVsInput += ", ";
		m_sVsInput	   += "out ";
		m_sVsInput	   += ch;
		m_sVsInput	   += " : ";
		m_sVsInput	   += ParamUsageNames[pu];
		m_sVsInput	   += m_iVsUsagesOut[pu];
		m_bVSinputAdded = true;
		m_iVsUsagesOut[pu]++;

	} // AddVertexShaderOutput

} // gfx

} // kge
