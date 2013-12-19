// File name: SceneManager.cpp
// Des: This class manages scene for rendering
// Date: 3/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)


#include "../../Include/sn/SceneManager.h"
#include "../../Include/sn/Light.h"
#include "../../Include/sn/Camera.h"
#include "../../Include/sn/RibbonTrail.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/gfx/MaterialManager.h"
#include "../../Include/efx/EffectManager.h"
#include "../../Include/io/Logger.h"
#include "../../Include/core/String.h"
#include "../../Include/core/mem_fun.h"
#include "../../Include/math/Vector.h"
#include "../../Include/math/Sphere.h"
#include "../../Headers/gfx/MS3DLoader.h"
#include "../../Headers/gfx/GeometryBatch.h"
#include "../../Include/math/Utility.h"

#include <iostream>
#include <string.h>
#include <algorithm>



float g_fElaspedTime = 0.0;
int   g_iFrameNumber = 0;

namespace kge
{
namespace sn
{
	// ******* ***** ************
	// Sazande class SceneManager
	// ******* ***** ************
	SceneManager::SceneManager(gfx::Renderer* pRenderer)
		: m_pActiveCam(NULL), m_bStoreShadowPoints(false),
		m_bEnableShadow(false), m_particlesSharedBuffer(0), m_numFreeBuffers(0),
		m_fElaspedTime(0.0), m_pSphere(NULL)
	{
		m_pRenderer		= pRenderer;
		m_pSceneAABB	= KGE_NEW(math::AABB)();
		m_DefaultMatParam.eLightingType = gfx::ELIT_Simple;
		m_DefaultMatParam.TextureMap = true;
		m_bHit = false;

		// prepare particles shared buffer
		const size_t sharedBufferSize = 1000 * 6;
		WORD indices[sharedBufferSize];
		WORD c = 0;

		for (unsigned int j = 0; j < 1000; ++j)
		{
			const unsigned int k = j * 6;
			indices[k] = c;
			indices[k+1] = c + 1;
			indices[k+2] = c + 2;
			indices[k+3] = c + 3;
			indices[k+4] = c;
			indices[k+5] = c + 2;
			c += 4;
		}

		m_particlesSharedBuffer = m_pRenderer->CreateIndexBuffer(indices, sharedBufferSize, gfx::EIBT_16Bit, false);

		m_rbuffers.reserve(256);
		m_freeRBuffers.reserve(256);
		////////////////////

		// Create deferred shading shaders
		const char strVsDeferred[] = 
			"float4x4 matViewProjection;\n"\
			"float4x4 matViewProjectionInv;\n"\
			"float3 EyePos;\n"\
			"float2 halfpixel;\n"\
			"\n"\
			"void main(in float4 inPos: POSITION0, in float2 inTex: TEXCOORD0, out float4 outPos: POSITION, out float2 outTex: TEXCOORD0, out float3 outViewRay: TEXCOORD1)\n"\
			"{\n"\
			"	outPos = inPos;//mul(inPos, matViewProjection);\n"\
			"	outTex = inTex - halfpixel;\n"\
			"	float4 posWorld = mul(inPos, matViewProjectionInv);\n"\
			"	posWorld /= posWorld.w;\n"\
			"	outViewRay = posWorld.xyz - EyePos;\n"\
			"}\n";

		m_pVsDeferred		= m_pRenderer->CreateVertexShaderFromString(strVsDeferred, "main", gfx::ESV_VS3);
		if (m_pVsDeferred)
		{
			m_VsmatViewProj		= m_pVsDeferred->GetConstatnt("matViewProjection");
			m_VsHalfPixel		= m_pVsDeferred->GetConstatnt("halfpixel");
			m_VsmatViewProjInv	= m_pVsDeferred->GetConstatnt("matViewProjectionInv");
			m_VsEyePos			= m_pVsDeferred->GetConstatnt("EyePos");
		}

		const char strPsDeferred[] =
			"sampler2D DifMap: register(s0);\n"\
			"sampler2D PosMap: register(s1);\n"\
			"sampler2D NorMap: register(s2);\n"\
			"float3 EyePos;\n"\
			"float3 DirLight[8];\n"\
			"float3 DirLightColor[8];\n"\
			"float3 DirLightSpecColor[8];\n"\
			"int LightCount;\n"\
			"\n"\
			"float3 decode (float2 enc)\n"\
			"{\n"\
			"    float2 fenc = enc*4-2;\n"\
			"    float f = dot(fenc,fenc);\n"\
			"    float g = sqrt(1-f/4);\n"\
			"    float3 n;\n"\
			"    n.xy = fenc*g;\n"\
			"    n.z = 1-f/2;\n"\
			"    return n;\n"\
			"}\n"\
			"float4 main(in float2 inTex: TEXCOORD0, in float3 inViewRay: TEXCOORD1) : COLOR0\n"\
			"{\n"\
			"	float4 normalData = tex2D(NorMap, inTex);\n"\
			"	float3 normal = decode(normalData.xy);\n"\
			"//	normal = sqrt(1.0 - (normal.x * normal.x + normal.y * normal.y));\n"\
			"	float depthVal = tex2D(PosMap, inTex).r;\n"\
			"	float3 ray = normalize(inViewRay);\n"\
			"	float4 pos;\n"\
			"	pos.xyz = EyePos + ray * depthVal;\n"\
			"	pos.w = 1.0f;\n"\
			"	float4 baseColor = tex2D(DifMap, inTex);\n"\
			"	float3 DifColor = float3(0.0f, 0.0f, 0.0f);\n"\
			"	float3 specColor = float3(0.0f, 0.0f, 0.0f);\n"\
			"	float3 ViewDir = normalize(EyePos - pos.xyz);\n"\
			"	int i = 0;\n"\
			"/*	for (i = 0; i < LightCount[1]; i++)\n"\
			"	{\n"\
			"		float3 litdir = PointLight[i].xyz - pos.xyz;\n"\
			"		float distsqr = dot(litdir, litdir);\n"\
			"		float att = min(1.0, 1.0 - PointLight[i].w * sqrt(distsqr));\n"\
			"		if (att > 0.00001)\n"\
			"		{\n"\
			"			litdir = normalize(litdir);\n"\
			"			float fNdotL = max(0.0, dot(normal, litdir)) * att;\n"\
			"			DifColor += fNdotL * PointLightColor[i];\n"\
			"			//return float4(DifColor, 1.0) * baseColor;\n"\
			"			if (fNdotL > 0.0)\n"\
			"			{\n"\
			"				float3 fvReflection = normalize(((2.0f * normal) * (fNdotL)) - litdir);\n"\
			"				specColor += baseColor.a * pow(max(0.0f, dot(fvReflection, ViewDir)), 15) * att * PointLightColor[i];\n"\
			"			}\n"\
			"		}\n"\
			"	}\n"\
			"*/	for (i = 0; i < LightCount; i++)\n"\
			"	{\n"\
			"		float fNdotL = max(0.0, dot(normal, DirLight[i]));\n"\
			"		DifColor += fNdotL * DirLightColor[i];\n"\
			"		if (fNdotL > 0.0)\n"\
			"		{\n"\
			"			float3 fvReflection = normalize(((2.0f * normal) * (fNdotL)) - DirLight[i]);\n"\
			"			specColor += baseColor.a * pow(max(0.0f, dot(fvReflection, ViewDir)), 15) * DirLightSpecColor[i];\n"\
			"		}\n"\
			"	}\n"\
			"//	return float4(normal, 1.0);//pos;//\n"\
			"	return 0.2f * baseColor + float4(DifColor, 1.0f) * baseColor + float4(specColor, 0.0f);\n"\
			"}\n";

		printf(strPsDeferred);

		m_pPsDeferred = m_pRenderer->CreatePixelShaderFromString(strPsDeferred, "main", gfx::ESV_PS3);
		if (m_pPsDeferred)
		{
			m_PsEyePos					= m_pPsDeferred->GetConstatnt("EyePos");
			m_PsDirLight				= m_pPsDeferred->GetConstatnt("DirLight");
			m_PsDirLightColor			= m_pPsDeferred->GetConstatnt("DirLightColor");
			m_PsDirLightSpecColor		= m_pPsDeferred->GetConstatnt("DirLightSpecColor");
			m_PsLightCount				= m_pPsDeferred->GetConstatnt("LightCount");
		}

		// Create float constants
		m_pfDirLightData			= KGE_NEW_ARRAY(float, 24);
		m_pfDirLightColorData		= KGE_NEW_ARRAY(float, 24);
		m_pfDirLightSpecColorData	= KGE_NEW_ARRAY(float, 24);

		// Create shaders for directional light with shadow
		const char strPsDeferredShadow[] =
			"sampler2D DifMap: register(s0);\n"\
			"sampler2D PosMap: register(s1);\n"\
			"sampler2D NorMap: register(s2);\n"\
			"sampler2D ShadowMapS: register(s7);\n"\
			"matrix matShadow: register(c4);\n"\
			"float3 EyePos;\n"\
			"float3 DirLight;\n"\
			"float3 DirLightColor;\n"\
			"float3 DirLightSpecColor;\n"\
			"float3 ShadowColor;\n"\
			"float GetShadow(float4 f2tex)\n"\
			"{\n"\
			"	return saturate(tex2Dproj(ShadowMapS, f2tex).r + 0.5);\n"\
			"}\n"\
			"\n"\
			"float3 decode (float2 enc)\n"\
			"{\n"\
			"    float2 fenc = enc*4-2;\n"\
			"    float f = dot(fenc,fenc);\n"\
			"    float g = sqrt(1-f/4);\n"\
			"    float3 n;\n"\
			"    n.xy = fenc*g;\n"\
			"    n.z = 1-f/2;\n"\
			"    return n;\n"\
			"}\n"\
			"float4 main(in float2 inTex: TEXCOORD0, in float3 inViewRay: TEXCOORD1) : COLOR0\n"\
			"{\n"\
			"//	return 0;//float4(normal, 1.0);//pos;//\n"\
			"	float4 normalData = tex2D(NorMap, inTex);\n"\
			"	float3 normal = decode(normalData.xy);\n"\
			"	float depthVal = tex2D(PosMap, inTex).r;\n"\
			"	float3 ray = normalize(inViewRay);\n"\
			"	float4 pos;\n"\
			"	pos.xyz = EyePos + ray * depthVal;\n"\
			"	pos.w = 1.0f;\n"\
			"	float4 proj = mul( pos, matShadow );\n"\
			"	float shadow = GetShadow( proj );\n"\
			"	//return float4(shadow,shadow,shadow,0.50);//mul( pos, matShadow );//\n"\
			"	float4 baseColor = tex2D(DifMap, inTex);\n"\
			"	float fNdotL = max(0.0, dot(normal, DirLight)) * shadow;\n"\
			"	if (shadow > 0.9)\n"\
			"	{\n"\
			"		float3 DifColor = fNdotL * DirLightColor;\n"\
			"		float3 specColor = float3(0.0f, 0.0f, 0.0f);\n"\
			"		if (fNdotL > 0.0)\n"\
			"		{\n"\
			"			float3 ViewDir = normalize(EyePos - pos.xyz);\n"\
			"			float3 fvReflection = normalize(((2.0f * normal) * (fNdotL)) - DirLight);\n"\
			"			specColor += baseColor.a * pow(max(0.0f, dot(fvReflection, ViewDir)), 15) * DirLightSpecColor;\n"\
			"		}\n"\
			"		return float4(DifColor * baseColor.rgb + specColor, 1.0f);\n"\
			"	}\n"\
			"	return float4(baseColor.rgb * fNdotL, 1.0);\n"\
			"}\n";

		printf(strPsDeferredShadow);

		m_pPsDeferredShadow				= m_pRenderer->CreatePixelShaderFromString(strPsDeferredShadow, "main", gfx::ESV_PS3);
		if (m_pPsDeferredShadow)
		{
			m_PsMatShadow				= m_pPsDeferredShadow->GetConstatnt("matShadow");
			m_PsEyePosShadow			= m_pPsDeferredShadow->GetConstatnt("EyePos");
			m_PsDirLightShadow			= m_pPsDeferredShadow->GetConstatnt("DirLight");
			m_PsDirLightColorShadow		= m_pPsDeferredShadow->GetConstatnt("DirLightColor");
			m_PsShadowColor				= m_pPsDeferredShadow->GetConstatnt("ShadowColor");
			m_PsDirLightSpecColorShadow = m_pPsDeferredShadow->GetConstatnt("DirLightSpecColor");
		}

		// Create shaders for sphere rendering
		const char strVsSphere[] =
			"float4x4 matViewProjection;\n"\
			"\n"\
			"void main(in float4 inPos: POSITION0, out float4 outPos: POSITION)\n"\
			"{\n"\
			"	outPos = mul(inPos, matViewProjection);\n"\
			"}\n";

		printf(strVsSphere);

		m_pVsSphere = m_pRenderer->CreateVertexShaderFromString(strVsSphere, "main", gfx::ESV_VS3);
		if (m_pVsSphere)
			m_VshSphereMatWVP = m_pVsSphere->GetConstatnt("matViewProjection");

		const char strPsSphere[] =
			"float4 main(): COLOR0\n"\
			"{\n"\
			"	return float4(1,0,0,0);\n"\
			"}\n";

		m_pPsSphere = m_pRenderer->CreatePixelShaderFromString(strPsSphere, "main", gfx::ESV_PS3);

		// Create shaders for point lights in deferred
		const char strPsPointLightDeferred[] =
			"sampler2D DifMap: register(s0);\n"\
			"sampler2D PosMap: register(s1);\n"\
			"sampler2D NorMap: register(s2);\n"\
			"float3 EyePos;\n"\
			"float4 PointLight;\n"\
			"float3 PointLightColor;\n"\
			"float3 PointLightSpecColor;\n"\
			"\n"\
			"float3 decode (float2 enc)\n"\
			"{\n"\
			"    float2 fenc = enc*4-2;\n"\
			"    float f = dot(fenc,fenc);\n"\
			"    float g = sqrt(1-f/4);\n"\
			"    float3 n;\n"\
			"    n.xy = fenc*g;\n"\
			"    n.z = 1-f/2;\n"\
			"    return n;\n"\
			"}\n"\
			"float4 main(in float2 inTex: TEXCOORD0, in float3 inViewRay: TEXCOORD1) : COLOR0\n"\
			"{\n"\
			"	float4 normalData = tex2D(NorMap, inTex);\n"\
			"	float3 normal = decode(normalData.xy);\n"\
			"	float depthVal = tex2D(PosMap, inTex).r;\n"\
			"	float3 ray = normalize(inViewRay);\n"\
			"	float4 pos;\n"\
			"	pos.xyz = EyePos + ray * depthVal;\n"\
			"	pos.w = 1.0f;\n"\
			"	float4 baseColor = tex2D(DifMap, inTex);\n"\
			"	float3 DifColor = float3(0.0f, 0.0f, 0.0f);\n"\
			"	float3 specColor = float3(0.0f, 0.0f, 0.0f);\n"\
			"\n"\
			"	float3 litdir = PointLight.xyz - pos.xyz;\n"\
			"	float distsqr = dot(litdir, litdir);\n"\
			"	float att = (1.0 - PointLight.w * sqrt(distsqr));\n"\
			"	if (att > 0.00001)\n"\
			"	{\n"\
			"		litdir = normalize(litdir);\n"\
			"		float fNdotL = max(0.0, dot(normal, litdir)) * att;\n"\
			"		DifColor = fNdotL * PointLightColor;\n"\
			"		if (fNdotL > 0.0)\n"\
			"		{\n"\
			"			float3 ViewDir = normalize(EyePos - pos.xyz);\n"\
			"			float3 fvReflection = normalize(((2.0f * normal) * (fNdotL)) - litdir);\n"\
			"			specColor = baseColor.a * pow(max(0.0f, dot(fvReflection, ViewDir)), 15) * att * PointLightSpecColor;\n"\
			"		}\n"\
			"	}\n"\
			"	return float4(DifColor * baseColor.rgb + specColor, 1.0f);\n"\
			"}\n";

		printf(strPsPointLightDeferred);

		m_pPsPointLight					= m_pRenderer->CreatePixelShaderFromString(strPsPointLightDeferred, "main", gfx::ESV_PS3);
		if (m_pPsPointLight)
		{
			m_PshPointEyePos				= m_pPsPointLight->GetConstatnt("EyePos");
			m_PshPointPointLight			= m_pPsPointLight->GetConstatnt("PointLight");
			m_PshPointPointLightColor		= m_pPsPointLight->GetConstatnt("PointLightColor");
			m_PshPointPointLightSpecColor	= m_pPsPointLight->GetConstatnt("PointLightSpecColor");
		}

		// Load sphere
		m_pSphere = AddStaticMeshNode("../../media/models/sphere.ms3d", false);
		m_pSphere->SetVisible(false);

	} // SceneManager()

	//------------
	// Destructor.
	//------------
	SceneManager::~SceneManager()
	{
		Clear();

		KGE_DELETE(m_pSceneAABB, AABB);

		// delete mesh buffers
		for(std::vector<gfx::MeshBuffer*>::iterator it = m_vMeshBuffer.begin(); 
			it != m_vMeshBuffer.end(); ++it)
		{
			KGE_DELETE(*it, MeshBuffer);
		}

		KGE_DELETE(m_particlesSharedBuffer, HardwareBuffer);

		for (std::vector<RBuffer*>::iterator it = m_rbuffers.begin(),
			it_end = m_rbuffers.end(); it != it_end; ++it)
		{
			KGE_DELETE((*it)->hb, HardwareBuffer);
			KGE_DELETE(*it, RBuffer);
		}

		KGE_DELETE_ARRAY(m_pfDirLightData);
		KGE_DELETE_ARRAY(m_pfDirLightColorData);
		KGE_DELETE_ARRAY(m_pfDirLightSpecColorData)

	} // Destructor

	// ***** ******* ** **** ******
	// Ezafe kardane ye Node Jadid.
	// ***** ******* ** **** ******
	void SceneManager::AddSceneNode(SceneNode* pSnNode)
	{
		m_vNodes.push_back(pSnNode);

	} // AddSceneNode

	//-----------------------------
	// Ezafe kardane ye Node Jadid.
	//-----------------------------
	void SceneManager::AddSceneNode(SceneNode* pSnNode , NodeType type)
	{
		pSnNode->SetSceneManager(this);
		switch ( type )
		{
		case ENT_Camera:
			m_vCamNodes.push_back ( pSnNode );
			break;

		case ENT_Light:
			m_vLightNodes.push_back ( pSnNode );
			break;

		case ENT_Mesh:
		case ENT_Decal:
		case ENT_TileTerrain:
		case ENT_TileTerrain2:
			m_vMeshNodes.push_back ( pSnNode );
			break;

		case ENT_ParticleSystem:
		case ENT_RibbonTrail:
			m_vPSysNodes.push_back(pSnNode);
			break;
		
		case ENT_AnimatedTexture:
			m_vAnimTexNodes.push_back(pSnNode);
			break;

		} // switch ( type )

	} // AddSceneNode( with NodeType )

	// Removes the scene node from scene manager
	void SceneManager::RemoveSceneNode(SceneNode* pSnNode, bool setParentNull /* = true */)
	{
		if (!pSnNode) { return; }

		if (setParentNull)
		{
			pSnNode->SetParent(NULL);
		}
		
		NodeType type = pSnNode->GetType();
		std::vector<SceneNode*>::iterator it;

		switch ( type )
		{
		case ENT_Camera:
			for (size_t i = 0; i < m_vCamNodes.size(); ++i)
			{
				if (m_vCamNodes[i] == pSnNode)
				{
					m_vCamNodes[i] = m_vCamNodes[m_vCamNodes.size() - 1];
					m_vCamNodes.pop_back();
					pSnNode->DecRef();
					break;
				}
			}
			break;

		case ENT_Light:
			for (size_t i = 0; i < m_vLightNodes.size(); ++i)
			{
				if (m_vLightNodes[i] == pSnNode)
				{
					m_vLightNodes[i] = m_vLightNodes[m_vLightNodes.size() - 1];
					m_vLightNodes.pop_back();
					pSnNode->DecRef();
					break;
				}
			}
			break;

		case ENT_Mesh:
		case ENT_StaticMesh:
		case ENT_AnimatedMesh:
		case ENT_TileTerrain:
		case ENT_Decal:
			for (size_t i = 0; i < m_vMeshNodes.size(); ++i)
			{
				if (m_vMeshNodes[i] == pSnNode)
				{
					m_vMeshNodes[i] = m_vMeshNodes[m_vMeshNodes.size() - 1];
					m_vMeshNodes.pop_back();
					pSnNode->DecRef();
					break;
				}
			}
			break;

		//case ENT_Decal:
		//    for (size_t i = 0; i < m_vMeshNodes.size(); ++i)
		//    {
		//        if (m_vMeshNodes[i] == pSnNode)
		//        {
		//            m_vMeshNodes[i] = m_vMeshNodes[m_vMeshNodes.size() - 1];
		//            m_vMeshNodes.pop_back();
		//            pSnNode->DecRef();
		//            break;
		//        }
		//    }
		//    break;

		case ENT_ParticleSystem:
		case ENT_RibbonTrail:
			for (size_t i = 0; i < m_vPSysNodes.size(); ++i)
			{
				if (m_vPSysNodes[i] == pSnNode)
				{
					m_vPSysNodes[i] = m_vPSysNodes[m_vPSysNodes.size() - 1];
					m_vPSysNodes.pop_back();
					pSnNode->DecRef();
					break;
				}
			}
			break;

		case ENT_AnimatedTexture:
			for (size_t i = 0; i < m_vAnimTexNodes.size(); ++i)
			{
				if (m_vAnimTexNodes[i] == pSnNode)
				{
					m_vAnimTexNodes[i] = m_vAnimTexNodes[m_vAnimTexNodes.size() - 1];
					m_vAnimTexNodes.pop_back();
					pSnNode->DecRef();
					break;
				}
			}
			break;

		case ENT_Sound:
			pSnNode->DecRef();
			break;

		default:
			for (size_t i = 0; i < m_vNodes.size(); ++i)
			{
				if (m_vNodes[i] == pSnNode)
				{
					m_vNodes[i] = m_vNodes[m_vNodes.size() - 1];
					m_vNodes.pop_back();
					break;
				}
			}
			break;

		} // switch ( type )


	} // RemoveSceneNode

	// ****** ** **** ******* ***** ** ****** ***** **** ** ****** ****** ***** ****
	// Vaghti ke hame karhaye ghabl az render anjam shod va render shodan ghati shod
	// ba in dastor mire baraye render.
	// ** ** ****** **** ****** *******
	void SceneManager::RegNode(SceneNode* pSnNode, RenderOrder ro)
	{
		// sorting by textures
		u32 handle = 0;

		if (pSnNode->GetMaterial(0))
		{
			if (pSnNode->GetMaterial(0)->ppTexture[0])
			{
				handle = pSnNode->GetMaterial(0)->ppTexture[0]->GetHandle();
			}
		}

		pSnNode->m_bVisFrame = true;

		switch (ro)
		{
			// ERO_First
		case ERO_First:
			m_vFirstNodes.Add(pSnNode, handle);
			break;

			// ERO_Middle
		case ERO_Middle:
			m_vMidNodes.Add(pSnNode, handle);
			break;

			// ERO_Last
		case ERO_Last:
			{
				// This list contains nodes with alpha blending, therefore we want to sort
				// them based their distance from camera and render them back to front.
				m_vLastNodes.Add(pSnNode, (m_pActiveCam->GetPosition() - pSnNode->GetPosition()).GetSqrLength());
			}
			break;

		case ERO_Decal:
			m_vDecalNodes.Add(pSnNode, handle);
			break;

		} // switch

	} // RegNode


	// ****** ******* **** ****
	// Render kardane hame chi.
	// ****** ******* **** ****
	bool SceneManager::RenderAll(float elapsedTime)
	{
		m_fElaspedTime = elapsedTime;
		g_fElaspedTime = elapsedTime;
		++g_iFrameNumber;

		m_vFirstNodes.Clear();
		m_vMidNodes.Clear();
		m_vLastNodes.Clear();
		m_vDecalNodes.Clear();
		m_pSceneAABB->Reset();
		m_vShadowCasterNodes.clear();
		m_vShadowReciverNodes.clear();
		m_vShadowCasterPoints.clear();
		m_vShadowReciverPoints.clear();

		m_bHit = false;

		// Unknown nodes.
		for(std::vector<SceneNode*>::iterator it = m_vNodes.begin(); 
			it != m_vNodes.end(); ++it)
		{
			(*it)->m_bVisFrame = false;
			(*it)->PreRender();
		}

		// Camera nodes
		for(std::vector<SceneNode*>::iterator it = m_vCamNodes.begin(); 
			it != m_vCamNodes.end(); ++it)
		{
			(*it)->m_bVisFrame = false;
			(*it)->PreRender(elapsedTime);
		}

		// Light nodes
		for(std::vector<SceneNode*>::iterator it = m_vLightNodes.begin(); 
			it != m_vLightNodes.end(); ++it)
		{
			(*it)->m_bVisFrame = false;
			(*it)->PreRender(elapsedTime);
		}

		// Mesh nodes
		for(std::vector<SceneNode*>::iterator it = m_vMeshNodes.begin(); 
			it != m_vMeshNodes.end(); ++it)
		{
			(*it)->m_bVisFrame = false;
			(*it)->PreRender(elapsedTime);
		}
		for(std::vector<SceneNode*>::iterator it = m_vCamNodes.begin(); 
			it != m_vCamNodes.end(); ++it)
		{
			(*it)->UpdateFinalMat();
		}

		// Particle system nodes
		for (size_t i = 0; i < m_vPSysNodes.size(); ++i)
		{
			m_vPSysNodes[i]->m_bVisFrame = false;
			m_vPSysNodes[i]->PreRender(elapsedTime);
		}

		// Animated texture nodes
		for (size_t i = 0, size = m_vAnimTexNodes.size(); i < size; ++i)
		{
			m_vAnimTexNodes[i]->m_bVisFrame = false;
			m_vAnimTexNodes[i]->PreRender(elapsedTime);
		}

		m_vFirstNodes.Sort();
		m_vMidNodes.Sort();
		m_vLastNodes.Sort();
		m_vDecalNodes.Sort();

		// Change the material params if needed.
		for(size_t i = 0, size = m_vMidNodes.Size(); i < size; ++i)
		{
			if (m_vMidNodes[i]->GetType() == ENT_StaticMesh || m_vMidNodes[i]->GetType() == ENT_AnimatedMesh)
			{
				CheckForMaterialsChanges(m_vMidNodes[i]);
			}
		}

		// Prepare effects with effect manager.
		efx::EffectManager::GetSingletonPtr()->AfterSceneNodesPreRender();

		// Render them
		//m_pRenderer->SetClearColor(kge::gfx::Colorf(0.0f, 0.0f, 0.0f, 1.0f));

		// Check for deferred rendering
		if (m_pRenderer->GetRendererPipeline() == ERP_Deferred)
		{
			m_pRenderer->SetRenderTarget(0, m_pRenderer->GetPositionMap());
			m_pRenderer->SetRenderTarget(1, m_pRenderer->GetDiffuseMap());
			m_pRenderer->SetRenderTarget(2, m_pRenderer->GetNormalMap());
		}

		//if (!m_pRenderer->BeginRendering(true, true, true)) { return false; }

		if (m_pActiveCam)
		{
			m_pActiveCam->Render();
		}

		for(size_t i = 0, size = m_vFirstNodes.Size(); i < size; ++i)
		{
			m_vFirstNodes[i]->Render();
		}

		// Render batches
		for (size_t i = 0, size = m_vGeometryBatches.size(); i < size; ++i)
		{
			m_vGeometryBatches[i]->Render();
		}

		for(size_t i = 0, size = m_vMidNodes.Size(); i < size; ++i)
		{
			m_vMidNodes[i]->Render();
		}

		// Check for deferred rendering
		// Render deferred shading
		if (m_pRenderer->GetRendererPipeline() == ERP_Deferred)
		{
			// Prepare for deferred shading
			m_pRenderer->SetRenderTarget(0, NULL);
			m_pRenderer->SetRenderTarget(1, NULL);
			m_pRenderer->SetRenderTarget(2, NULL);
			m_pRenderer->Disable(gfx::ERF_ZWrite);
			m_pRenderer->Disable(gfx::ERF_DepthBuffer);
			m_pRenderer->Disable(gfx::ERF_AlphaTest);
			m_pRenderer->SetTexture(m_pRenderer->GetDiffuseMap(), 0);
			m_pRenderer->SetTexture(m_pRenderer->GetPositionMap(), 1);
			m_pRenderer->SetTexture(m_pRenderer->GetNormalMap(), 2);
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 0);
			m_pRenderer->SetTextureParams(gfx::ETP_Point, 1);
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 2);

			// set constants for rendering the directional and ambient lights
			float fHalfPixel[2];
			fHalfPixel[0] = 0.5f / (float)m_pRenderer->GetWindowWidth();
			fHalfPixel[1] = 0.5f / (float)m_pRenderer->GetWindowHeight();
			math::Matrix mat;
			mat = m_pActiveCam->GetProjectionMatrix();
			m_pVsDeferred->SetConstant(m_VsmatViewProj, mat.m_fMat, 16);
			m_pVsDeferred->SetConstant(m_VsHalfPixel, fHalfPixel, 2);
			mat = m_pActiveCam->GetProjectionMatrix() * m_pActiveCam->GetViewMatrix();
			mat.Inverse();
			m_pVsDeferred->SetConstant(m_VsmatViewProjInv, mat.m_fMat, 16);
			
			// Get light data
			int dircount,pointcount,spotcount;
			int dirindex,pointindex,pointcolorindex,spotindex;
			dirindex = pointindex = pointcolorindex = spotindex = 0;
			std::vector<Light*> vLights = GetLightsInFrustum(dircount, pointcount, spotcount);
			std::vector<Light*> vPointLights;
			Light*	pLastDirLight = NULL;
			m_iLightCount = dircount;
			for (int i = 0; i < vLights.size(); i++)
			{
				Light* pLit = vLights[i];
				switch (pLit->GetLightData()->Type)
				{
				case gfx::ELT_Directional:
					memcpy(&m_pfDirLightData[dirindex], &pLit->GetLightData()->Direction.x, 12);
					memcpy(&m_pfDirLightColorData[dirindex], pLit->GetLightData()->Diffuse.c, 12);
					memcpy(&m_pfDirLightSpecColorData[dirindex], pLit->GetLightData()->Specular.c, 12);
					dirindex += 3;
					pLastDirLight = pLit;
					break;

					// collect the point lights pointer
				case gfx::ELT_Point:
					vPointLights.push_back(pLit);
					pointindex += 4;
					pointcolorindex += 3;
					break;

				case gfx::ELT_Spot:
					break;
				}
			}

			// Check for shadowed light
			efx::ShadowMapProjection* pSMP = efx::EffectManager::GetSingletonPtr()->GetShadowMapProjection();
			if (pSMP &&	m_iLightCount > 0)
			{
				m_iLightCount--;
			}

			// Sets the data for directional lights
			m_pPsDeferred->SetConstant(m_PsDirLight, m_pfDirLightData, 24);
			m_pPsDeferred->SetConstant(m_PsDirLightColor, m_pfDirLightColorData, 24);
			m_pPsDeferred->SetConstant(m_PsDirLightSpecColor, m_pfDirLightSpecColorData, 24);
			m_pPsDeferred->SetConstant(m_PsLightCount, &m_iLightCount, 1);

			// Get Camera position
			kge::math::Vector vCamPos = m_pActiveCam->GetPosition();
			m_pPsDeferred->SetConstant(m_PsEyePos, &vCamPos.x, 3);
			m_pVsDeferred->SetConstant(m_VsEyePos, &vCamPos.x, 3);

			// Draw the ambient and directional light
			m_pVsDeferred->Render();
			m_pPsDeferred->Render();
			m_pRenderer->SetFarPlaneAsScreenQuad(m_pActiveCam);
			m_pRenderer->DrawScreenQuad();

			math::Matrix matVP = m_pActiveCam->GetProjectionMatrix() * m_pActiveCam->GetViewMatrix();
			m_pRenderer->Enable(gfx::ERF_AdditiveBlending);
			m_pRenderer->Enable(gfx::ERF_DepthBuffer);

			// Draw shadowed light
			if (pSMP && pLastDirLight)
			{
				m_pPsDeferredShadow->SetConstant(m_PsDirLightShadow, &pLastDirLight->GetLightData()->Direction.x, 3);
				m_pPsDeferredShadow->SetConstant(m_PsDirLightColorShadow, &pLastDirLight->GetLightData()->Diffuse.r, 3);
				m_pPsDeferredShadow->SetConstant(m_PsDirLightSpecColorShadow, &pLastDirLight->GetLightData()->Specular.r, 3);
				m_pPsDeferredShadow->SetConstant(m_PsEyePosShadow, &vCamPos.x, 3);
				m_pPsDeferredShadow->SetConstant(m_PsMatShadow, pSMP->GetMatrix()->m_fMat, 16);
				m_pPsDeferredShadow->SetConstant(m_PsShadowColor, &pSMP->m_cShadowColor.r, 3);
				m_pPsDeferredShadow->Render();
				m_pRenderer->DrawScreenQuad();
			}

			// Draw the point lights
			m_pRenderer->Enable(gfx::ERF_StencilBuffer);
			for (int i = 0; i < vPointLights.size(); i++)
			{
				Light* pLit = vPointLights[i];

				// Set the sphere position and range
				m_pSphere->SetPosition(pLit->GetPosition());
				float range = pLit->GetLightData()->Range;
				m_pSphere->SetScale(math::Vector(range, range, range));
				m_pSphere->PreRender();
				math::Matrix matWVP = matVP * (*m_pSphere->GetFinalMatrix());

				// Disable color write
				m_pRenderer->Disable(gfx::ERF_ColorRendering);

				// Check if the camera is inside of the light or not
 				if (pLit->GetBoundingBox()->Inside(vCamPos))
 				{
 					// Set Z mode to greater
 					m_pRenderer->SetDepthMode(gfx::EDTM_Greater);
 					// Set stencil buffer to 1 when zpass
 					m_pRenderer->SetStencilBuffer(efx::EET_PointLightInside, 1);
 
 					// Render volume back faces
 					m_pVsSphere->SetConstant(m_VshSphereMatWVP, matWVP.m_fMat, 16);
 					m_pVsSphere->Render();
 					m_pPsSphere->Render();
 					m_pRenderer->SetCullingMode(gfx::ECM_CW);
 					m_pSphere->Draw(false, true, true, false, false);
 				}
 				// Camera is outside the light
 				else
 				{
 					// Set Z mode to default
 					m_pRenderer->SetDepthMode();
 					// Increase stencil buffer for front faces
 					m_pRenderer->SetStencilBuffer(efx::EET_PointLightOutside, 1);
 
 					// Render volume front faces
 					m_pVsSphere->SetConstant(m_VshSphereMatWVP, matWVP.m_fMat, 16);
 					m_pVsSphere->Render();
 					m_pPsSphere->Render();
 					m_pRenderer->SetCullingMode(gfx::ECM_CCW);
 					m_pSphere->Draw(false, true, true, false, false);
 
 					// Decrease the stencil for back faces
 					m_pRenderer->SetStencilBuffer(efx::EET_PointLightOutside, 2);
 
 					// Render volume back faces
 					m_pRenderer->SetCullingMode(gfx::ECM_CW);
 					m_pSphere->Draw(false, true, true, false, false);
 				}

				// Enable color writing
				m_pRenderer->Enable(gfx::ERF_ColorRendering);

				// Draw the point light as screen quad
				m_pVsDeferred->SetConstant(m_VsHalfPixel, fHalfPixel, 2);
				m_pVsDeferred->SetConstant(m_VsmatViewProjInv, mat.m_fMat, 16);
				m_pVsDeferred->SetConstant(m_VsEyePos, &vCamPos.x, 3);
				m_pPsPointLight->SetConstant(m_PshPointEyePos, &vCamPos.x, 3);
				math::Vector vPLData;
				vPLData = pLit->GetPosition();
				vPLData.w = 1.0f / pLit->GetLightData()->Range;
				m_pPsPointLight->SetConstant(m_PshPointPointLight, &vPLData.x, 4);
				m_pPsPointLight->SetConstant(m_PshPointPointLightColor, &pLit->GetLightData()->Diffuse.r, 3);
				m_pPsPointLight->SetConstant(m_PshPointPointLightSpecColor, &pLit->GetLightData()->Specular.r, 3);
				m_pRenderer->Disable(gfx::ERF_DepthBuffer);
				m_pVsDeferred->Render();
				m_pPsPointLight->Render();
				//m_pRenderer->Disable(gfx::ERF_StencilBuffer);
				m_pRenderer->SetStencilBuffer(efx::EET_PointLightOutside, 3);
				m_pRenderer->SetCullingMode(gfx::ECM_CCW);
				m_pRenderer->DrawScreenQuad();
				m_pRenderer->Enable(gfx::ERF_DepthBuffer);

				// Clear the stencil buffer
				if (i < vPointLights.size() - 1)
					m_pRenderer->Clear(false, false, true);
			}

			// Set Z mode to default
			m_pRenderer->SetCullingMode(gfx::ECM_CCW);
			m_pRenderer->SetDepthMode();
			m_pRenderer->Disable(gfx::ERF_StencilBuffer);
			m_pRenderer->Disable(gfx::ERF_AdditiveBlending);
			m_pRenderer->Enable(gfx::ERF_ZWrite);
		}

		// Prepare effects with effect manager.
		efx::EffectManager::GetSingletonPtr()->AfterSceneNodesRender(m_fElaspedTime);

		// Render decals
		m_pRenderer->Disable(gfx::ERF_ZWrite);

		if (m_vDecalNodes.Size())
		{
			gfx::Renderer::GetSingletonPtr()->SetTextureParams(gfx::ETP_BorderColor);
		}
		for(size_t i = 0, size = m_vDecalNodes.Size(); i < size; ++i)
		{
			m_vDecalNodes[i]->Render();
		}
		if (m_vDecalNodes.Size())
		{
			gfx::Renderer::GetSingletonPtr()->SetTextureParams(gfx::ETP_Wrap);
		}

		// Render ParticleSystems, AnimatedTextures & RibbonTrails
		//m_pRenderer->Disable(gfx::ERF_Culling);
		for(int k = m_vLastNodes.Size() - 1; k >= 0 ; --k)
		{
			m_vLastNodes[k]->Render();
		}
		//m_pRenderer->Enable(gfx::ERF_Culling);
		m_pRenderer->Enable(gfx::ERF_ZWrite);
		//

		for(size_t i = 0, size = m_vFirstNodes.Size(); i < size; ++i)
		{
			m_vFirstNodes[i]->PostRender();
		}

		for(size_t i = 0, size = m_vMidNodes.Size(); i < size; ++i)
		{
			m_vMidNodes[i]->PostRender();
		}

		for(size_t i = 0, size = m_vLastNodes.Size(); i < size; ++i)
		{
			m_vLastNodes[i]->PostRender();
		}

		m_vEffect.Sort();

		for(size_t i = 0, size = m_vEffect.Size(); i < size; ++i)
		{
			m_vEffect[i]->PostRenderEverything();
			m_pRenderer->Clear(false, false, true);
		}

		return true;

	} // RenderAll

	// ***** ******* ** ****** ** ******
	// Adds a camera to scene.
	// ***** ******* ** ****** ** ******
	Camera* SceneManager::AddCameraNode(const math::Vector& position,
		const math::Vector& lookAt, const math::Vector& upDirection)
	{
		Camera* cam = KGE_NEW(Camera)(this, position, lookAt, upDirection);

		m_pActiveCam = cam;

		AddSceneNode(cam , ENT_Camera);

		return cam;

	} // AddCameraNode

	// ***** ******* ** ****** ** ******
	// Adds a camera to scene.
	// ***** ******* ** ****** ** ******
 //   Camera* SceneManager::AddCameraNode(float distance, float theta, float phi,
 //       const math::Vector& target, const math::Vector &upDirection)
 //   {
 //       Camera* cam = new Camera(this, distance, theta, phi, target, upDirection);

 //       m_pActiveCam = cam;

 //       AddSceneNode(cam , ENT_Camera);

 //       return cam;

 //   } // AddCameraNode

 //   // ***** ******* ** ****** ** ******
 //   // Adds a camera to scene.
 //   // ***** ******* ** ****** ** ******
	//Camera* SceneManager::AddCameraNode(const math::Vector& position,
 //       const math::Vector& target, const math::Vector& upDirection)
	//{
 //       Camera* cam = new Camera(this, position, target, upDirection);

	//	m_pActiveCam = cam;
	//	
	//	AddSceneNode(cam , ENT_Camera);
	//	
	//	return cam;

	//} // AddCameraNode

	//----------------------------------
	// Loads an animated mesh from file.
	//----------------------------------
	AnimatedMesh* SceneManager::AddAnimatedMeshNode(const char* FileName)
	{
		core::String str;
		str = FileName;

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
		str.ToLower();
#endif

		AnimatedMesh* amesh = NULL;
		MeshBuffer  * mb    = NULL;

		for(std::vector<gfx::MeshBuffer*>::iterator it = m_vMeshBuffer.begin();
			it != m_vMeshBuffer.end(); ++it)
		{
			if ((*it)->m_sName == str)
			{
				mb = (*it);
				break;
			}
		} // For mesh buffers.

		if (!mb)
		{
			if (str.FindLast(".amesh") > 0)
			{
				mb = gfx::LoadAnimatedMesh(str.ToCharPointer(), m_pRenderer, this);
				if (mb)
				{
					io::Logger::Log("Mesh loaded: ", FileName);
					m_vMeshBuffer.push_back(mb);
				}
			}
			else if ( str.FindLast(".ms3d") > 0 )
			{
				gfx::MS3DLoader* ms3d = KGE_NEW(gfx::MS3DLoader)(m_pRenderer, this);
				mb = ms3d->Load(str.ToCharPointer());
				if (mb)
				{
					io::Logger::Log("Mesh loaded: ", FileName);
					m_vMeshBuffer.push_back(mb);
				}
				KGE_DELETE(ms3d, MS3DLoader);

			} // Ms3d loader need.
			else
			{
				io::Logger::Log(io::ELM_Error, "This type of mesh not supported: %s", FileName);
			}
		}

		if (mb)
		{
			amesh = KGE_NEW(AnimatedMesh)(mb);

			for (u32 i = 0; i < mb->m_iMeshCount ; ++i)
			{
				amesh->GetBoundingBox()->AddSphere(mb->m_pMeshes[i].GetBoundingBox());
			}

			amesh->SetSceneManager(this);
			amesh->SetFileName(&mb->m_sName);
			amesh->EnableShadow(m_bEnableShadow);

			AddSceneNode(amesh, sn::ENT_Mesh);

			CheckForMaterialsChanges(amesh);
		}

		return amesh;

	} // AddAnimatedMeshNode

	//-------------------------------
	// Loads a static mesh from file.
	//-------------------------------
	StaticMesh* SceneManager::AddStaticMeshNode(const char* FileName, bool enableLighting)
	{
		core::String str;
		str = FileName;

#if (KGE_PLATFORM == KGE_PLATFORM_WINDOWS)
		str.ToLower();
#endif

		StaticMesh*  smesh = NULL;
		MeshBuffer*  mb    = NULL;

		for(std::vector<gfx::MeshBuffer*>::iterator it = m_vMeshBuffer.begin();
			it != m_vMeshBuffer.end(); ++it)
		{
			if ((*it)->m_sName == str)
			{
				mb = (*it);
				break;
			}
		} // For mesh buffers.

		if (!mb)
		{
			if (str.FindLast(".smesh") > 0)
			{
				mb = gfx::LoadStaticMesh(str.ToCharPointer(), m_pRenderer, this);
				if (mb)
				{
					io::Logger::Log("Mesh loaded: ", FileName);
					m_vMeshBuffer.push_back(mb);
				}
			}
			else if ( str.FindLast(".ms3d") > 0 )
			{
				gfx::MS3DLoader* ms3d = KGE_NEW(gfx::MS3DLoader)(m_pRenderer , this);
				mb = ms3d->LoadStatic(str.ToCharPointer());
				if (mb)
				{
					io::Logger::Log("Mesh loaded: ", FileName);
					m_vMeshBuffer.push_back(mb);
				}
				KGE_DELETE(ms3d, MS3DLoader);

			} // Ms3d loader need.
			else
			{
				io::Logger::Log(io::ELM_Error, "This type of mesh not supported: %s", FileName);
			}
		}

		if (mb)
		{
			smesh = KGE_NEW(StaticMesh)(mb);
			for (u32 i = 0; i < mb->m_iMeshCount ; ++i)
			{
				smesh->GetBoundingBox()->AddSphere(mb->m_pMeshes[i].GetBoundingBox());
				smesh->GetMaterial(i)->shader->m_MaterialParams = m_DefaultMatParam;
				if (!enableLighting)
				{
					smesh->GetMaterial(i)->shader->m_MaterialParams.eLightingType = ELIT_UnLit;
				}
			}

			smesh->SetSceneManager(this);
			smesh->SetFileName(&mb->m_sName);
			smesh->EnableShadow(m_bEnableShadow);

			AddSceneNode(smesh, sn::ENT_Mesh);

			CheckForMaterialsChanges(smesh);
		}

		return smesh;

	} // AddStaticMeshNode

	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	ParticleSystem* SceneManager::AddParticleSystemNode(const ParticleSystemParameters& p)
	{
		ParticleSystem* psys = KGE_NEW(ParticleSystem)(p);

		AddSceneNode(psys, ENT_ParticleSystem);

		psys->SetMaxParticles(p.maxParticles);

		if (p.rendererType == EPRT_QUAD)
		{
			psys->SetShader(p.shaderName);
			psys->SetTexture(p.textureName);
		}
		else if (p.rendererType == EPRT_MESH)
		{
			psys->SetMesh(p.meshName);
		}

		return psys;
	}

	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	AnimatedTexture* SceneManager::AddAnimatedTextureNode(const AnimatedTextureParameters& p)
	{
		AnimatedTexture* animTex = KGE_NEW(AnimatedTexture)(p);

		AddSceneNode(animTex, ENT_AnimatedTexture);

		animTex->SetTexture(p.textureName);
		animTex->SetShader(p.shaderName);
		animTex->SetVertexBuffer();

		return animTex;
	}

	//-------------------------------------------------------
	//
	//-------------------------------------------------------
	RibbonTrail* SceneManager::AddRibbonTrail(
		const char* shaderName,
		const char* textureName,
		const math::Vector& textureRect,
		const gfx::Colorf& initialColor,
		const gfx::Colorf& finalColor,
		float width,
		float speed,
		float age,
		float chaosRatio)
	{
		RibbonTrail* ribbonTrail = KGE_NEW(RibbonTrail)(this, shaderName, textureName, textureRect,
			initialColor, finalColor, width, speed, age, chaosRatio);

		AddSceneNode(ribbonTrail, ENT_RibbonTrail);

		return ribbonTrail;
	}

	//----------------------------------------------------------------
	// Ye nore jahat daro dar sahne ezafe mikone. Mesle nore Khorshid.
	//----------------------------------------------------------------
	sn::Light* SceneManager::AddDirectionalLight(math::Vector vDir, const gfx::Colorf& color)
	{
		m_DefaultMatParam.eLightingType = gfx::ELIT_Simple;
		vDir.Normalize();
		gfx::LightData* lit = m_pRenderer->Lights->AddDirectionalLight(vDir, color);
		sn::Light * light   = KGE_NEW(sn::Light)(lit, this, m_pRenderer);
		AddSceneNode(light, sn::ENT_Light);

		// TODO: Remove this later and put a better design. Get the 8 nearest lights.
		lit->Index = (int)m_vLightNodes.size() - 1;
		m_pRenderer->Lights->SetLight(lit, (int)m_vLightNodes.size() - 1);

		return light;

	} // AddDirectionalLight

	//-----------------------------------------------------------------------------------
	// Ye nore noghte i ro be sahne ezafe mikone ke noresh dar tamame jahat pakhsh mishe.
	//-----------------------------------------------------------------------------------
	sn::Light* SceneManager::AddPointLight(const math::Vector& vPosition, const gfx::Colorf& color)
	{
		m_DefaultMatParam.eLightingType = gfx::ELIT_Simple;
		gfx::LightData* lit = m_pRenderer->Lights->AddPointLight(vPosition, color);
		sn::Light * light   = KGE_NEW(sn::Light)(lit, this, m_pRenderer);
		AddSceneNode(light, sn::ENT_Light);

		// TODO: Remove this later and put a better design. Get the 8 nearest lights.
		lit->Index = (int)m_vLightNodes.size() - 1;
		m_pRenderer->Lights->SetLight(lit, lit->Index);

		return light;

	} // AddPointLight

	//----------------------------------------------------------------
	// Ye nore spot ro be sahne ezafe mikone mesle nore cheragh ghove.
	//----------------------------------------------------------------
	sn::Light* SceneManager::AddSpotLight(const math::Vector& vPosition, const math::Vector& vDirection, const gfx::Colorf& color)
	{
		m_DefaultMatParam.eLightingType = gfx::ELIT_Simple;
		gfx::LightData* lit = m_pRenderer->Lights->AddSpotLight(vPosition, vDirection, color);
		sn::Light * light   = KGE_NEW(sn::Light)(lit, this, m_pRenderer);
		AddSceneNode(light, sn::ENT_Light);

		// TODO: Remove this later and put a better design. Get the 8 nearest lights.
		lit->Index = (int)m_vLightNodes.size() - 1;
		m_pRenderer->Lights->SetLight(lit, (int)m_vLightNodes.size() - 1);

		return light;

	} // AddSpotLight


	// ***** ******* ** ********
	// Ezafe kardane ye texture.
	// ***** ******* ** ********
	gfx::Texture* SceneManager::AddTexture(const char* FileName)
	{
		return Device::GetSingletonPtr()->GetTextureManager()->Load(FileName, NULL, NULL);

	} // AddTexture

	//------------------------------------------------------------------------------------
	// Load a texture
	//------------------------------------------------------------------------------------
	Texture* SceneManager::AddTexture(const char* FileName, const char* Name, gfx::CompressType ct)
	{
		return Device::GetSingletonPtr()->GetTextureManager()->Load(FileName, (void*)ct, Name);

	} // AddTexture

	//------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------
	gfx::Shader* SceneManager::AddVertexShader(const char* fileName, const char* vertexMain, gfx::ShaderVersion version)
	{
		u32 handle = Device::GetSingletonPtr()->GetVertexShaderManager()->Add2(fileName, vertexMain, (void*)&version, 0);
		return Device::GetSingletonPtr()->GetVertexShaderManager()->GetResource(handle);
	}

	//------------------------------------------------------------------------------------
	// 
	//------------------------------------------------------------------------------------
	gfx::Shader* SceneManager::AddPixelShader(const char* fileName, const char* pixelMain, gfx::ShaderVersion version)
	{
		u32 handle = Device::GetSingletonPtr()->GetPixelShaderManager()->Add2(fileName, pixelMain, (void*)&version, 0);
		return Device::GetSingletonPtr()->GetPixelShaderManager()->GetResource(handle);
	}

	// ***** ******* ** ********
	// 
	// ***** ******* ** ********
	void SceneManager::AddEffect( efx::Effect* pEffect, efx::EffectType Type )
	{
		m_vEffect.Add(pEffect, Type);
		m_vEffectType.push_back(Type);

	} // AddEffect

	//----------------------------------------------------------
	// Get the nearest light scene node from the given position.
	//----------------------------------------------------------
	sn::Light* SceneManager::GetNearestLight(const math::Vector& Where)
	{
		if ( m_vLightNodes.empty() )
		{
			return NULL;
		}

		m_vLightList.Clear();

		for(std::vector<SceneNode*>::iterator it = m_vLightNodes.begin(),
			it_end = m_vLightNodes.end(); it != it_end; ++it)
		{
			if (!(*it)->GetVisible()) { continue; }

			if (((sn::Light*)(*it))->GetLightData()->Type == gfx::ELT_Directional)
			{
				return (sn::Light*)(*it);
			}

			m_vLightList.Add((sn::Light*)(*it), (Where - (*it)->GetPosition()).GetSqrLength());

		} // for m_vLightNodes

		m_vLightList.Sort();
		sn::Light* lit = m_vLightList[0];
		return lit;

	} // GetNearestLight

	//----------------------------------------------------------
	// Get the nearest light scene node from the given position.
	//----------------------------------------------------------
	const core::PriorityArray<sn::Light*,float>& SceneManager::GetNearestLight(const math::Vector& Where, int count )
	{
		m_vLightList.Clear();

		if ( m_vLightNodes.empty() )
		{
			return m_vLightList;
		}

		for(std::vector<SceneNode*>::iterator it = m_vLightNodes.begin(),
			it_end = m_vLightNodes.end(); it != it_end; ++it)
		{
			if (m_vLightList.Size() == count) { return m_vLightList; }

			sn::Light* lit = (sn::Light*)(*it);

			if (!lit->GetVisible()) { continue; }

			if (lit->GetLightData()->Type == gfx::ELT_Directional)
			{
				m_vLightList.Add(lit, 0.0f);
				continue;
			}

			const float sqrLength = (Where - lit->GetPosition()).GetSqrLength();

//			if (sqrLength < lit->GetLightData()->Range * lit->GetLightData()->Range + 1.0f)
//			{
				m_vLightList.Add(lit, sqrLength);
//			}

		} // for m_vLightNodes

		m_vLightList.Sort();
		return m_vLightList;

	} // GetNearestLight

	//------------------------------------------------------------------------------------
	// Checks a node for its visibility.
	//------------------------------------------------------------------------------------
	math::CollisionType SceneManager::IsVisible(SceneNode* node)
	{
		if (!m_pActiveCam) { return math::ECT_In; }

		const kge::math::Frustum* f = m_pActiveCam->GetFrustum();
		math::AABB box;
		node->GetFinalMatrix()->TransformBox(*node->GetAxisAlignedBoundingBox(), box);
		math::CollisionType ct = f->Collision(&box);

		// If we need shadow data
		if (m_bStoreShadowPoints)
		{
			kge::math::Matrix modelView;
			modelView = *node->GetFinalMatrix() * m_pRenderer->GetTransForm(gfx::ETM_View);

			Light* lit = GetNearestLight(kge::math::Vector());
			// If there is a directional light in the scene
			if (lit->GetLightData()->Type == gfx::ELT_Directional)
			{
				// sweepDir
				math::Vector sweepDir = lit->GetLightData()->Direction;
				sweepDir.Negate();

				switch (ct)
				{
					// Out
				case math::ECT_Out:
					{
						math::Sphere instanceSphere;
						instanceSphere.AddBox(box);
						if (f->TestSweptSphere(instanceSphere, sweepDir))
						{
							m_bHit = true;
							math::AABB box2;
							modelView.TransformBox2(box, box2);
							m_vShadowCasterNodes.push_back(node);
							m_vShadowCasterPoints.push_back(box2);
						}
					}
					break;

					// In
				case math::ECT_In:
					{
						m_bHit = true;
						math::AABB box2;
						modelView.TransformBox2(box, box2);
						m_vShadowCasterNodes.push_back(node);
						m_vShadowCasterPoints.push_back(box2);
						m_vShadowReciverNodes.push_back(node);
						m_vShadowReciverPoints.push_back(box2);
					}
					break;

					// Intersect
				case math::ECT_Intersect:
					{
						m_bHit = true;
						math::AABB box2;
						modelView.TransformBox2(box, box2);
						m_vShadowCasterNodes.push_back(node);
						m_vShadowCasterPoints.push_back(box2);
						m_vShadowReciverNodes.push_back(node);
						m_vShadowReciverPoints.push_back(box2);
					}
					break;

				}
			}
		}

		if (ct != math::ECT_Out)
		{
			if (node->GetType() == ENT_TileTerrain)
			{
				return ct;
			}

			if (node->HasShadow() && (node->GetType() == ENT_AnimatedMesh ||
				node->GetType() == ENT_StaticMesh))
			{
				m_pSceneAABB->AddAABB(&box);
			}

			ct = math::ECT_In;
		}

		return ct;

	} // IsVisible(StaticMesh)

	//------------------------------------------------------------------------------------
	// Checks the scene node for material necessary changes
	//------------------------------------------------------------------------------------
	void SceneManager::CheckForMaterialsChanges( SceneNode* pSn )
	{
		int DirLightCount   = 0;
		int PointLightCount = 0;
		int SpotLightCount  = 0;

		for(std::vector<SceneNode*>::iterator it = m_vLightNodes.begin(); 
			it != m_vLightNodes.end(); ++it)
		{
			if (((sn::Light*)(*it))->GetLightData()->Type == gfx::ELT_Directional)
			{
				if ((*it)->GetVisible())
				{
					++DirLightCount;
				}
			}
			else if (((sn::Light*)(*it))->GetLightData()->Type == gfx::ELT_Point)
			{
				// TODO Check if the node and light collide each other
				sn::Light* lit = (sn::Light*)(*it);

				if (lit->GetVisible())
				{
					const float sqrLength = (pSn->GetPosition() - lit->GetPosition()).GetSqrLength();
					if (sqrLength < lit->GetLightData()->Range * lit->GetLightData()->Range)
						++PointLightCount;
				}
			}
			else if (((sn::Light*)(*it))->GetLightData()->Type == gfx::ELT_Spot)
			{
				// TODO Check if the node and light collide each other
				if ((*it)->GetVisible())
				{
					++SpotLightCount;
				}
			}

		} // for m_vLightNodes

		for (u32 i = 0, count = pSn->GetMaterialCount(); i < count; ++i)
		{
			bool NeedChanges = false;
			if (pSn->GetMaterial(i)->shader->m_MaterialParams.DirLightCount != DirLightCount)
			{
				NeedChanges = true;
				pSn->GetMaterial(i)->shader->m_MaterialParams.DirLightCount = DirLightCount;
			}
			if (pSn->GetMaterial(i)->shader->m_MaterialParams.PointLightCount != PointLightCount)
			{
				NeedChanges = true;
				pSn->GetMaterial(i)->shader->m_MaterialParams.PointLightCount = PointLightCount;
			}
			if (pSn->GetMaterial(i)->shader->m_MaterialParams.SpotLightCount != SpotLightCount)
			{
				NeedChanges = true;
				pSn->GetMaterial(i)->shader->m_MaterialParams.SpotLightCount = SpotLightCount;
			}
			if (NeedChanges || pSn->GetMaterial(i)->shader->m_bMatParamsChanged)
			{
				pSn->GetMaterial(i)->shader->SetMaterialParams
					(gfx::MaterialManager::GetSingletonPtr()->GetMaterialParam
					(&pSn->GetMaterial(i)->shader->m_MaterialParams));
			}
		}

	} // CheckForMaterialsChanges

	//----------------------------
	//
	//----------------------------
	void SceneManager::Clear()
	{
		m_vFirstNodes.Clear();
		m_vMidNodes.Clear();
		m_vLastNodes.Clear();
		m_vDecalNodes.Clear();

		for (size_t i = 0; i < m_vNodes.size(); ++i)
		{
			KGE_DELETE(m_vNodes[i], SceneNode);
		}
		m_vNodes.clear();

		for (size_t i = 0; i < m_vLightNodes.size(); ++i)
		{
			m_vLightNodes[i]->DecRef();
		}
		m_vLightNodes.clear();

		for (size_t i = 0; i < m_vMeshNodes.size(); ++i)
		{
			KGE_DELETE(m_vMeshNodes[i], SceneNode);
		}
		m_vMeshNodes.clear();

		for (size_t i = 0; i < m_vAnimTexNodes.size(); ++i)
		{
			KGE_DELETE(m_vAnimTexNodes[i], SceneNode);
		}
		m_vAnimTexNodes.clear();

		for (size_t i = 0; i < m_vPSysNodes.size(); ++i)
		{
			KGE_DELETE(m_vPSysNodes[i], SceneNode);
		}
		m_vPSysNodes.clear();

		for (size_t i = 0; i < m_vCamNodes.size(); ++i)
		{
			KGE_DELETE(m_vCamNodes[i], SceneNode);
		}
		m_vCamNodes.clear();

	} // Clear

	void SceneManager::AddGeometryBatch( gfx::GeometryBatch* pGeoBatch )
	{
		m_vGeometryBatches.push_back(pGeoBatch);

	} // AddGeometryBatch

	std::vector<SceneNode*> SceneManager::GetVisibleMeshes( Camera* pCam )
	{
		std::vector<SceneNode*> vis;
		const kge::math::Frustum* f = pCam->GetFrustum();
		math::AABB s;
		math::CollisionType ct; 
		Light* lit = GetNearestLight(kge::math::Vector());
		// If there is a directional light in the scene
		math::Vector sweepDir;
		if (lit->GetLightData()->Type == gfx::ELT_Directional)
		{
			// sweepDir
			sweepDir = lit->GetLightData()->Direction;
			sweepDir.Negate();
		}
		for (size_t i = 0; i < m_vMeshNodes.size(); i++)
		{
			if (m_vMeshNodes[i]->GetVisible() && m_vMeshNodes[i]->HasShadow() && 
				(m_vMeshNodes[i]->GetType() == ENT_StaticMesh || m_vMeshNodes[i]->GetType() == ENT_AnimatedMesh))
			{
				// if mesh is visible and instanced then continue
				if (m_vMeshNodes[i]->IsVisibleInFrame() && 
					m_vMeshNodes[i]->GetMaterial(0)->shader->m_MaterialParams.Instanced)
					continue;

				// if mesh is visible add it to vector
				if (m_vMeshNodes[i]->IsVisibleInFrame())
				{
					vis.push_back(m_vMeshNodes[i]);
					continue;
				}			
				m_vMeshNodes[i]->GetFinalMatrix()->TransformBox(*m_vMeshNodes[i]->GetAxisAlignedBoundingBox(), s);
// 				math::Sphere instanceSphere;
// 				instanceSphere.AddBox(s);
// 				if (f->TestSweptSphere(instanceSphere, sweepDir))
// 					vis.push_back(m_vMeshNodes[i]);
				math::CollisionType ct = f->Collision(&s);
				if (ct == math::ECT_In || ct == math::ECT_Intersect)
					vis.push_back(m_vMeshNodes[i]);
			}
		}

		return vis;
	}

	//////////////////////////////////////////////////////////////////////////
	void SceneManager::SetParticlesSharedBuffer(gfx::HardwareBuffer* ib)
	{
		m_particlesSharedBuffer = ib;
		for (size_t i = 0, size = m_vPSysNodes.size(); i < size; ++i)
		{
			if (ENT_ParticleSystem == m_vPSysNodes[i]->GetType())
			{
				reinterpret_cast<ParticleSystem*>(m_vPSysNodes[i])->SetSharedBuffer(m_particlesSharedBuffer);
			} 
		}

	} // SetParticlesIndexBuffer

	RBuffer* SceneManager::GetRBuffer(size_t size)
	{
		for (size_t i = 0; i < m_numFreeBuffers; ++i)
		{
			if (size <= m_freeRBuffers[i]->size)
			{
				--m_numFreeBuffers;
				RBuffer* temp = m_freeRBuffers[i];
				m_freeRBuffers[i] = m_freeRBuffers[m_numFreeBuffers];
				return temp;
			}
		}

		gfx::HardwareBuffer* pVB = m_pRenderer->CreateVertexBuffer(NULL, size, gfx::EVT_V3CT, true);
		m_rbuffers.push_back(KGE_NEW(RBuffer)(pVB, size));
		m_freeRBuffers.push_back(0);
		return m_rbuffers.back();
	}

	void SceneManager::FreeRBuffer(RBuffer* rbuffer)
	{
		if (!rbuffer) { return; }
		m_freeRBuffers[m_numFreeBuffers] = rbuffer;
		++m_numFreeBuffers;
	}
	//////////////////////////////////////////////////////////////////////////

#if KGE_USE_SOUND_SYSTEM

	sn::Sound3D* SceneManager::AddSound3D( const char* FileName )
	{
		return NULL;
	}

	std::vector<Light*> SceneManager::GetLightsInFrustum(int& DirCount, int& PointCount, int& SpotCount)
	{
		DirCount = PointCount = SpotCount = 0;
		std::vector<Light*> v;
		for (int i = 0; i < m_vLightNodes.size(); i++)
		{
			Light* pLit = (Light*)m_vLightNodes[i];
			if (pLit->IsVisibleInFrame())
			{
				v.push_back(pLit);
				switch (pLit->GetLightData()->Type)
				{
				case gfx::ELT_Directional:
					DirCount++;
					break;

				case gfx::ELT_Point:
					PointCount++;
					break;

				case gfx::ELT_Spot:
					SpotCount++;
					break;
				}
			}
		}

		return v;
	}

	bool SceneManager::IsLightVisible( Light* pLit )
	{
		if (!m_pActiveCam) 
			return true;

		const kge::math::Frustum* f = m_pActiveCam->GetFrustum();
		//math::Sphere sp;
		//pLit->GetFinalMatrix()->TransformSphere(*pLit->GetBoundingBox(), sp);
		math::CollisionType ct = f->Collision(pLit->GetBoundingBox());

		if (ct == math::ECT_Out)
			return false;

		return true;

	} // IsLightVisible

#endif // KGE_USE_SOUND_SYSTEM

} // sn

} // kge
