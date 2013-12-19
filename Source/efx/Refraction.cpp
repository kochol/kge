// File name: Refraction.cpp
// Des: This class is for refraction effects
// Date: 7/2/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/efx/Refraction.h"
#include "../../Include/core/mem_fun.h"
#include "../../Include/sn/SceneNode.h"
#include "../../Include/sn/StaticMesh.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/math/Matrix.h"

namespace kge
{
namespace efx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Refraction::Refraction()
		: m_fTime(0.0f), m_pVertexShader(0), m_pPixelShader(0), m_pMatViewProj(0), m_pMatView(0)
	{
		const char pV[] = "static const float3 fvLightPosition = float3(-100,-100,0);"\
		"float2 texScale;"\
		"	float3 fvEyePosition;"\
		"float4x4 matView;"\
		"float4x4 matViewProjection;"\
		"float fTime0_X;"\
		"struct VS_INPUT "\
		"{"\
		"	float4 Position : POSITION0;"\
		"	float2 Texcoord : TEXCOORD0;"\
		"	float3 Normal :   NORMAL0;"\
		"	float3 Binormal : BINORMAL0;"\
		"	float3 Tangent :  TANGENT0;"\
		"};"\
		"struct VS_OUTPUT "\
		"{"\
		"	float4 Position :        POSITION0;"\
		"	float4 Texcoord :        TEXCOORD0;"\
		"	float3 ViewDirection :   TEXCOORD1;"\
		"	float3 LightDirection:   TEXCOORD2;"\
		"	float3 eyeLinear: TEXCOORD3;"\
		"};"\
		"VS_OUTPUT main( VS_INPUT Input )"\
		"{"\
			"VS_OUTPUT Output;"\
			"float4 pPos = mul( Input.Position, matViewProjection );"\
			"Output.Position         = pPos;"\
			"const float tc = sin(fTime0_X);"\
			"Output.Texcoord.xy      = Input.Texcoord * texScale + tc;"\
			"Output.Texcoord.zw      = Input.Texcoord * texScale + tc * 0.25;"\
			"float3 fvObjectPosition = mul( Input.Position, matView ).xyz;"\
			"float3 fvViewDirection  = fvEyePosition - fvObjectPosition;"\
			"float3 fvLightDirection = fvLightPosition - fvObjectPosition;"\
			"float3 fvNormal         = mul( float4(Input.Normal, 1.0f), matView ).xyz;"\
			"float3 fvBinormal       = mul( float4(Input.Binormal, 1.0f), matView ).xyz;"\
			"float3 fvTangent        = mul( float4(Input.Tangent, 1.0f), matView ).xyz;"\
			"Output.ViewDirection.x  = dot( fvTangent, fvViewDirection );"\
			"Output.ViewDirection.y  = dot( fvBinormal, fvViewDirection );"\
			"Output.ViewDirection.z  = dot( fvNormal, fvViewDirection );"\
			"Output.LightDirection.x = dot( fvTangent, fvLightDirection );"\
			"Output.LightDirection.y = dot( fvBinormal, fvLightDirection );"\
			"Output.LightDirection.z = dot( fvNormal, fvLightDirection );"\
		"	Output.eyeLinear = pPos.xyw;"\
		"	return( Output );"\
		"}";

		const char pP[] = "static const float4 fvSpecular = float4(0.5188, 0.5065, 0.3195, 1.0);\n"\
			"static const float fSpecularPower = 30.0f;\n"\
			"static const float bumpiness = 0.01f;\n"\
			"static const float2 offsets[12] = \n"\
			"{\n"\
			"-0.326212, -0.405805,\n"\
			"-0.840144, -0.073580,\n"\
			"-0.695914,  0.457137,\n"\
			"-0.203345,  0.620716,\n"\
			"0.962340, -0.194983,\n"\
			"0.473434, -0.480026,\n"\
			"0.519456,  0.767022,\n"\
			"0.185461, -0.893124,\n"\
			"0.507431,  0.064425,\n"\
			"0.896420,  0.412458,\n"\
			"-0.321940, -0.932615,\n"\
			"-0.791559, -0.597705,\n"\
			"};\n"\
			"sampler2D bumpMap;\n"\
			"sampler2D reflection: register(s6);\n"\
			"struct PS_INPUT \n"\
			"{\n"\
			"float4 Texcoord :        TEXCOORD0;\n"\
			"float3 ViewDirection :   TEXCOORD1;\n"\
			"float3 LightDirection :  TEXCOORD2;\n"\
			"float3 eyeLinear :	 	TEXCOORD3;\n"\
			"};\n"\
			"float4 main( PS_INPUT Input ) : COLOR0\n"\
			"{      \n"\
			"float4 WaterColor = float4(0.0,0.2748,0.2748,1.0);\n"\
			"float3 fvLightDirection = normalize( Input.LightDirection );\n"\
			"float3 fvNormal         = tex2D( bumpMap, Input.Texcoord.xy ).xyz;\n"\
			"fvNormal.xy += tex2D( bumpMap, Input.Texcoord.zw ).xy;\n"\
			"fvNormal = normalize( ( fvNormal * 2.0f ) - 1.0f );\n"\
			"float  fNDotL           = max( 0.0f, dot( fvNormal, fvLightDirection )) + 0.3; \n"\
			"float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); \n"\
			"float3 fvViewDirection  = normalize( Input.ViewDirection );\n"\
			"float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );\n"\
			"float4 fvTotalSpecular  = fvSpecular * pow( fRDotV, fSpecularPower );\n"\
			"float4 fvDif            = fNDotL * WaterColor;\n"\
			"// Sample position, offset for some bumpiness\n"\
			"float2 mid;\n"\
			"mid.x = 0.5 * Input.eyeLinear.x / Input.eyeLinear.z+0.5;\n"\
			"mid.y = -0.5 * Input.eyeLinear.y / Input.eyeLinear.z+0.5;\n"\
			"mid += fvNormal.xy * bumpiness;\n"\
			"float4 refl = tex2D(reflection, mid);\n"\
			"refl.a = 0.3;\n"\
			"// Blur the reflection according to depth\n"\
			"float4 blurred = refl;\n"\
			"for (int i = 0; i < 12; i++){\n"\
			"blurred += tex2D(reflection, mid + 0.02 * refl.a * refl.a  * offsets[i]);\n"\
			"}\n"\
			"// Fade with depth\n"\
			"float depthFade = saturate(1 - 0.5 * refl.a);\n"\
			"return fvDif + fvTotalSpecular + depthFade * (blurred / 13);\n"\
			"}\n";

		m_pVertexShader			= gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(pV, "main", gfx::ESV_VS3);
		m_pPixelShader			= gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(pP, "main", gfx::ESV_PS3);

		sh_fvEyePosition		= m_pVertexShader->GetConstatnt("fvEyePosition");
		sh_matView				= m_pVertexShader->GetConstatnt("matView");
		sh_matViewProjection	= m_pVertexShader->GetConstatnt("matViewProjection");
		sh_fTime0_X				= m_pVertexShader->GetConstatnt("fTime0_X");
		sh_f2TexScale			= m_pVertexShader->GetConstatnt("texScale");

		m_v2TexScale.x			= 2.5f;
		m_v2TexScale.y			= 2.5f;

		m_pMatViewProj			= KGE_NEW(math::Matrix)();
		m_pMatView				= KGE_NEW(math::Matrix)();

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Refraction::~Refraction()
	{
		if (m_pVertexShader)
			m_pVertexShader->DecRef();

		if (m_pPixelShader)
			m_pPixelShader->DecRef();

		delete m_pMatView;
		delete m_pMatViewProj;

	} // Destructor

	//------------------------------------------------------------------------------------
	// The works must to do before rendering the object.
	//------------------------------------------------------------------------------------
	void Refraction::PreRender(float elapsedTime)
	{
		m_fTime += elapsedTime * 0.01f;

	} // PreRender

	//------------------------------------------------------------------------------------
	// The works must to do after rendering the object.
	//------------------------------------------------------------------------------------
	void Refraction::Render()
	{
		*m_pMatViewProj = gfx::Renderer::GetSingletonPtr()->GetTransForm(gfx::ETM_Projection) * 
			gfx::Renderer::GetSingletonPtr()->GetTransForm(gfx::ETM_View);
		*m_pMatView		=  gfx::Renderer::GetSingletonPtr()->GetTransForm(gfx::ETM_View);
		m_pMatView->_14 = m_pMatView->_41;
		m_pMatView->_24 = m_pMatView->_42;
		m_pMatView->_34 = m_pMatView->_43;
		m_pMatView->_41 = 0;
		m_pMatView->_42 = 0;
		m_pMatView->_43 = 0;

		sn::SceneManager* smgr = Device::GetSingletonPtr()->GetSceneManager();
		sn::SceneNode	* sn;
		kge::sn::SceneNode* pCam = smgr->GetActiveCamera();
		math::Vector vp = pCam->GetPosition();
		m_pVertexShader->SetConstant(sh_fvEyePosition, &vp.x, 3);
		m_pMatView->Identity();
		m_pVertexShader->SetConstant(sh_matView, m_pMatView->m_fMat, 16);
		m_pVertexShader->SetConstant(sh_fTime0_X, &m_fTime, 1);
		m_pVertexShader->SetConstant(sh_f2TexScale, &m_v2TexScale.x, 2);

		for (std::vector<sn::SceneNode*>::iterator it = m_arrNodes.begin();
			it != m_arrNodes.end(); ++it)
		{
			sn = *it;
			sn->PreRender();
// 			if (smgr->IsVisible(sn) == math::ECT_In)
// 			{
				sn->Render();
//			}
		}

	} // Render

	//------------------------------------------------------------------------------------
	// The works must to do after rendering all of the scene nodes.
	//------------------------------------------------------------------------------------
	void Refraction::PostRenderEverything()
	{

	} // PostRenderEverything


	void Refraction::AddNodes( sn::SceneNode* pNode )
	{
		Effect::AddNodes(pNode);
		gfx::ShaderInstance* pSI= KGE_NEW(gfx::ShaderInstance)();
		pSI->m_pVertexShader	= m_pVertexShader;
		pSI->m_pPixelShader		= m_pPixelShader;
		pSI->m_pUserData		= pNode;
		pSI->m_pFun				= core::mem_fun(this, &Refraction::SetShaderConstant);
		for (u32 i = 0; i < pNode->GetMaterialCount(); i++)
		{
			pNode->GetMaterial(i)->shader = pSI;
		}
		if (pNode->GetType() == sn::ENT_StaticMesh)
		{
			sn::StaticMesh* sm = (sn::StaticMesh*)pNode;
			for(u32 i = 0; i < sm->GetMeshCount(); i++)
			{
				sm->GetMesh(i)->PrepareBuffers(true);
			}
		}
		pNode->SetVisible(false);

	} // AddNodes

	void Refraction::SetShaderConstant( gfx::ShaderInstance* pSI )
	{		
		math::Matrix mat;
		mat = *m_pMatViewProj * *((sn::SceneNode*)pSI->m_pUserData)->GetFinalMatrix();
		pSI->m_pVertexShader->SetConstant(sh_matViewProjection, mat.m_fMat, 16);

	} // SetShaderConstant

	//------------------------------------------------------------------------------------
	// Sets the texcoord scale
	//------------------------------------------------------------------------------------
	void Refraction::SetTexScale( float x, float y )
	{
		m_v2TexScale.x = x;
		m_v2TexScale.y = y;

	} // SetTexScale

} // efx

} // kge
