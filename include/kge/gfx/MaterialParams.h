// File name: MaterialParams.h
// Des: The Material params class.
// Date: 28/4/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_MATERIALPARAMS_H
#define KGE_MATERIALPARAMS_H

#include <map>
#include "../KgeUnknown.h"
#include "CustomMaterial.h"
#include "Structs.h"
#include "../core/String.h"
#include "../core/Functor.h"

namespace kge
{
namespace gfx
{

	class Shader;
	class ShaderInstance;

enum LightingType
{
	ELIT_UnLit,
	ELIT_Simple,
	ELIT_Phong,
	ELIT_Custom

}; // LightingType

enum LightingMode
{
	ELM_Unknown,
	ELM_PerVertex,
	ELM_PerPixel

};

enum CustomMaterialOrder
{
	ECMO_AtBeginning,
	ECMO_AtEnd,

	ECMO_Count
};
enum ParamUsage
{
	EPU_Position,
	EPU_Texcoord,
	EPU_Normal,
	EPU_Color,
	EPU_Tangent,
	EPU_BiNormal,

	EPU_Count

}; // ParamUsage

static const char* ParamUsageNames[] =
{
		"POSITION",
		"TEXCOORD",
		"NORMAL",
		"COLOR",
		"TANGENT",
		"BINORMAL"
};

enum ParamType
{
	EPT_Float,
	EPT_Float2,
	EPT_Float3,
	EPT_Float4,
	EPT_Float3x3,
	EPT_Float4x4,
	EPT_Int,
	EPT_Int2,
	EPT_Int3,
	EPT_Int4,

	EPT_Count
};

static const char* ParamTypeNames[] =
{
	"float",
	"float2",
	"float3",
	"float4",
	"float3x3",
	"float4x4",
	"int",
	"int2",
	"int3",
	"int4"

};

class KGE_API MaterialParams: public KgeUnknown
{
	friend class CustomMaterial;

public:

	//! Constructor
	MaterialParams();

	//! Destructor
	~MaterialParams();

	//! Copy operator
	void operator = (const MaterialParams& mp);

	//! checks equal with MaterialParams
	bool operator == (const MaterialParams& mp);

	//! Checks the same vertex shader with other MaterialParams 
	bool HasSameVertexShader(MaterialParams* mp);

	//! Checks the same pixel shader with other MaterialParams 
	bool HasSamePixelShader(MaterialParams* mp);

	//! Adds the custom material part to this material parameter
	void AddCustomMaterialPart(CustomMaterial* pCustomMat);

	//! Create the vertex shader code depends on the needs and returns its code
	void CreateVertexShaderCodeHLSL();

	//! Create the pixel shader code depends on the needs and returns its code
	void CreatePixelShaderCodeHLSL();

	//! adds a pixel shader input string
	void AddPixelShaderInput(const char* ch, ParamUsage pu);

	//! adds a vertex shader input string
	void AddVertexShaderInput(const char* ch, ParamUsage pu);

	//! adds a vertex shader output string
	void AddVertexShaderOutput(const char* ch, ParamUsage pu);

	int							DirLightCount,
								PointLightCount,
								SpotLightCount,
								BoneCount;

	bool						ReceiveShadow,
								NormalMap,
								TextureMap,
								DetailMap,
								SpecularMap,
								SkeletalMesh,
								Instanced,
								Deferred,
								AlphaMap,
								Frensel;

	LightingType				eLightingType;
	LightingMode				eLightingMode;
	FogType						eFogType;

	Shader					*	pVertexShader,
							*	pPixelShader;

	kge::core::Functor1
		<void,
		ShaderInstance*>	*	m_pFun;

//protected:

	bool						CustomVertexShader,
								CustomPixelShader,
								NeedWorldViewMatrix,
								NeedWorldMatrix;

	core::String				m_sVsHeader,
								m_sVsInput,								
								m_sVsMain,
								m_sVertexShader,
								m_sPsHeader,
								m_sPsInput,
								m_sPsOutput,
								m_sPsMain,
								m_sPixelShader;

	std::map<int, CustomMaterial*>	
								m_mCustomMaterials;
	std::map<int, CustomMaterial*>::iterator
								m_itCMat,
								m_itCmatEnd;

	CustomMaterial			*	m_pLightCus;

	// Vertex shader handles
	char					*	m_pVshMatWVP,
							*	m_pVshMatWV,
							*	m_pVshMatNorm,
							*	m_pVshLightDir,
							*   m_pVshLightPos,
							*	m_pVshLightDirColor,
							*   m_pVshLightPosColor,
							*	m_pVshMatBone,
							*	m_pVshEyePos,
							*	m_pVshMatWorld,
							*	m_pVshMatLight;

	// Pixel shader handles
	char					*	m_pPshLightDir,
							*   m_pPshLightPos,
							*	m_pPshLightDirColor,
							*   m_pPshLightPosColor,
							*	m_pPshFrenselVal,
							*	m_pPshFrenselColor;

protected:

	bool						m_bPSinputAdded,
								m_bVSinputAdded;

	int							m_iVsUsagesOut[EPU_Count],
								m_iPsUsagesOut[EPU_Count],
								m_iVsUsagesIn[EPU_Count],
								m_iPsUsagesIn[EPU_Count];

	void SetVertexShaderConstants();

	void SetShaderConstants(ShaderInstance* pSI);

}; // MaterialParams

} // gfx

} // kge

#endif // KGE_MATERIALPARAMS_H
