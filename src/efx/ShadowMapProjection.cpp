// File name: ShadowMapProjection.h
// Des: This effect is a shadow technique that is simple and fast without any self shadowing and it is good for shadowing on terrain 
// Date: 16/12/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/efx/ShadowMapProjection.h"
#include "../../include/kge/gfx/Renderer.h"
#include "../../include/kge/sn/Light.h"
#include "../../include/kge/sn/StaticMesh.h"
#include "../../include/kge/sn/Camera.h"
#include "../../Headers/gfx/GeometryBatch.h"

namespace kge
{
namespace efx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	ShadowMapProjection::ShadowMapProjection(int ShadowMapSize, sn::SceneManager* pSmgr)
		: m_pSmgr(pSmgr), m_pShaderInstance(NULL), m_iShadowMapSize(ShadowMapSize),
		m_pMaterial(0), m_pMaterial2(0), m_cShadowColor(0.5f, 0.5f, 0.5f, 1.0f)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	ShadowMapProjection::~ShadowMapProjection()
	{
		delete m_pMaterial;
		delete m_pMaterial2;

	} // Destructor

	//------------------------------------------------------------------------------------
	// Inits the shadow map
	//------------------------------------------------------------------------------------
	bool ShadowMapProjection::Init()
	{
		// Create shadow map
		if (!gfx::Renderer::GetSingletonPtr()
			->CreateRenderableTexture( &m_pShadowMap, 
										m_iShadowMapSize,
										m_iShadowMapSize,
										gfx::ETF_NULL))
		{
			io::Logger::Log(io::ELM_Error, "Can't create the shadow map.");
			return false;
		}

		m_ViewPort.X		= 0;
		m_ViewPort.Y		= 0;
		m_ViewPort.Width	= m_iShadowMapSize;
		m_ViewPort.Height	= m_iShadowMapSize;

		// Set depth bias
		gfx::Renderer::GetSingletonPtr()->SetDepthBias(0.0002f);
		gfx::Renderer::GetSingletonPtr()->SetDepthBiasSlope(2.0f);

		// Create shaders
		const char strVS[] = "float4x4 matViewProjection;\n"\
			"struct VS_INPUT\n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"	VS_OUTPUT Output;\n"\
			"	Output.Position = mul( Input.Position, matViewProjection );\n"\
			"	return( Output );\n"\
			"}\n";

		m_pVertexShader = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS, "main", gfx::ESV_VS3);

		m_shMatWVP		= m_pVertexShader->GetConstatnt("matViewProjection");

		const char strPS[] =   
			"float4 main() : COLOR0\n"\
			"{\n"\
			"	return float4(0.5,0.5,0.5,1);\n"\
			"}";
		m_pPixelShader = NULL;//gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(strPS, "main", gfx::ESV_PS3);//NULL;//

		const char strVS2[] = "float4x4 matViewProjection;\n"\
			"struct VS_INPUT\n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Tex		: TEXCOORD0;\n"\
			"};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Tex		: TEXCOORD0;\n"\
			"};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"	VS_OUTPUT Output;\n"\
			"	Output.Position = mul( Input.Position, matViewProjection );\n"\
			"	Output.Tex      = Input.Tex;\n"\
			"	return( Output );\n"\
			"}\n";

		m_pVertexShader2 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS2, "main", gfx::ESV_VS3);

		m_shMatWVP2		= m_pVertexShader2->GetConstatnt("matViewProjection");

		const char strPS2[] =   
			"sampler2D Texture0;\n"\
			"float4 main(float2 tex: TEXCOORD0) : COLOR0\n"\
			"{\n"\
			"	return float4(0.5,0.5,0.5, tex2D( Texture0, tex ).a);\n"\
			"}";

		m_pPixelShader2 = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(strPS2, "main", gfx::ESV_PS3);

		const char strVS3[] = "float4x4 matViewProjection;\n"\
			"struct VS_INPUT\n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float4 inInstance0 : TEXCOORD1;\n"\
			"	float4 inInstance1 : TEXCOORD2;\n"\
			"	float4 inInstance2 : TEXCOORD3;\n"\
			"	float4 inInstance3 : TEXCOORD4;\n"\
			"};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"	VS_OUTPUT Output;\n"\
			"	float4x4 matWorld = float4x4(Input.inInstance0, Input.inInstance1, Input.inInstance2, Input.inInstance3);\n"
			"	float4x4 matWVP = mul(matWorld, matViewProjection);\n"
			"	Output.Position = mul( Input.Position, matWVP );\n"\
			"	return( Output );\n"\
			"}\n";

		m_pVertexShader3 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS3, "main", gfx::ESV_VS3);

		m_shMatWVP3		= m_pVertexShader->GetConstatnt("matViewProjection");

		const char strVS4[] = "float4x4 matViewProjection;\n"\
			"struct VS_INPUT\n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Tex		: TEXCOORD0;\n"\
			"	float4 inInstance0 : TEXCOORD1;\n"\
			"	float4 inInstance1 : TEXCOORD2;\n"\
			"	float4 inInstance2 : TEXCOORD3;\n"\
			"	float4 inInstance3 : TEXCOORD4;\n"\
			"};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Tex		: TEXCOORD0;\n"\
			"};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"	VS_OUTPUT Output;\n"\
			"	float4x4 matWorld = float4x4(Input.inInstance0, Input.inInstance1, Input.inInstance2, Input.inInstance3);\n"
			"	float4x4 matWVP = mul(matWorld, matViewProjection);\n"
			"	Output.Position = mul( Input.Position, matWVP );\n"\
			"	Output.Tex = Input.Tex;\n"\
			"	return( Output );\n"\
			"}\n";

		m_pVertexShader4 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS4, "main", gfx::ESV_VS3);

		m_shMatWVP4		= m_pVertexShader->GetConstatnt("matViewProjection");

		m_pShaderInstance = KGE_NEW(gfx::ShaderInstance)();
		m_pShaderInstance->m_pVertexShader = m_pVertexShader;
		m_pShaderInstance->m_pPixelShader  = m_pPixelShader;
		m_pShaderInstance->m_pUserData     = this;
		m_pShaderInstance2 = KGE_NEW(gfx::ShaderInstance)();
		m_pShaderInstance2->m_pVertexShader = m_pVertexShader2;
		m_pShaderInstance2->m_pPixelShader  = m_pPixelShader2;
		m_pShaderInstance2->m_pUserData     = this;
		m_pShaderInstance3 = KGE_NEW(gfx::ShaderInstance)();
		m_pShaderInstance3->m_pVertexShader = m_pVertexShader3;
		m_pShaderInstance3->m_pPixelShader  = m_pPixelShader;
		m_pShaderInstance3->m_pUserData     = this;
		m_pShaderInstance4 = KGE_NEW(gfx::ShaderInstance)();
		m_pShaderInstance4->m_pVertexShader = m_pVertexShader4;
		m_pShaderInstance4->m_pPixelShader  = m_pPixelShader2;
		m_pShaderInstance4->m_pUserData     = this;

		// Create material
		m_pMaterial = KGE_NEW(gfx::Material)();
		m_pMaterial->shader = m_pShaderInstance;
		m_pMaterial2 = KGE_NEW(gfx::Material)();
		m_pMaterial2->shader = m_pShaderInstance2;
		m_pMaterial2->TextureAlphal = true;
		m_pMaterial3 = KGE_NEW(gfx::Material)();
		m_pMaterial3->shader = m_pShaderInstance3;
		m_pMaterial4 = KGE_NEW(gfx::Material)();
		m_pMaterial4->shader = m_pShaderInstance4;
		m_pMaterial4->TextureAlphal = true;

		float fOffsetX  = 0.5f + (0.5f / (float)m_iShadowMapSize);
		float fOffsetY  = 0.5f + (0.5f / (float)m_iShadowMapSize);
		unsigned int range = 1;            //note different scale in DX9!
		float fBias     = -0.0002f;
		m_pBiasMatrix	= KGE_NEW(math::Matrix)
			(
			0.5f,     0.0f,     0.0f,         0.0f,
			0.0f,    -0.5f,     0.0f,         0.0f,
			0.0f,     0.0f,     (float)range, 0.0f,
			fOffsetX, fOffsetY, fBias,        1.0f				
			);
		m_pProj			= KGE_NEW(math::Matrix)();
		m_pOrtho		= KGE_NEW(math::Matrix)();
		m_pBiasFinal	= KGE_NEW(math::Matrix)();
		m_pFinal		= KGE_NEW(math::Matrix)();
		m_pFrustum		= KGE_NEW(math::Frustum)();
		m_pRen			= gfx::Renderer::GetSingletonPtr();
		m_pActiveCamera = m_pSmgr->GetActiveCamera();
		m_pShadowCamera = m_pSmgr->AddCameraNode(math::Vector(1.0f, 1.0f, 1.0f), math::Vector(), math::Vector(0.0f, 1.0f, 0.0f));
		if (m_pActiveCamera)
			m_pSmgr->SetActiveCamera(m_pActiveCamera);

		m_pShaderCode	= "sampler2D ShadowMapS: register(s7);\n"\
						  "float GetShadow(float4 f2tex)\n"\
						  "{\n"\
						  "\treturn saturate(tex2Dproj(ShadowMapS, f2tex).r + 0.5);\n"\
						  "}\n";

		gfx::Renderer::GetSingletonPtr()->SetTextureParams(kge::gfx::ETP_Anisotropic, 7);

		return true;

	} // Init

	//------------------------------------------------------------------------------------
	// Add nodes of mesh that effect influencing on it.
	//------------------------------------------------------------------------------------
	void ShadowMapProjection::AddNodes( sn::SceneNode* pNodes )
	{
		Effect::AddNodes(pNodes);

	} // AddNodes

	//------------------------------------------------------------------------------------
	// The works must to do before rendering the object.
	//------------------------------------------------------------------------------------
	void ShadowMapProjection::PreRender(float elapsedTime)
	{
		//math::AABB aabb = *m_pSmgr->m_pSceneAABB;
		m_pActiveCamera = m_pSmgr->GetActiveCamera();

		math::AABB aabb;
		//m_pActiveCamera->GetViewMatrix().TransformBox2(*m_pSmgr->m_pSceneAABB, aabb);
		aabb = *m_pSmgr->m_pSceneAABB;
		const float fScale = 5.0;
		aabb.m_vMin.x -= fScale;
		aabb.m_vMin.y -= fScale;
		aabb.m_vMax.x += fScale;
		aabb.m_vMax.y += fScale;
		aabb.UpdateLines();

		m_pProj->SetPerspectiveLH(m_pActiveCamera->GetViewportWidth(),
								  m_pActiveCamera->GetViewportHeight(),
								  m_pActiveCamera->GetFov(),
								  m_pActiveCamera->GetNearValue(),
								  aabb.m_vMax.z);
		float nearClipOffset = 50.0f;
		float fD			 = aabb.m_vMax.z + nearClipOffset;
		//m_pFrustum->Init(&m_pActiveCamera->GetViewMatrix(), m_pProj);
		//aabb = *m_pFrustum->GetBoundingBox();

		kge::math::Vector vCenter = aabb.GetCenter();//*m_pFrustum->GetCenter();
		kge::math::Vector d = m_pSmgr->GetNearestLight(vCenter)->GetLightData()->Direction;
		d.Normalize();
		d.Negate();
		m_pShadowCamera->SetPosition(vCenter - (d * fD));
		m_pShadowCamera->SetTarget(vCenter);

		kge::math::Matrix matv    = m_pShadowCamera->GetViewMatrix();
		kge::math::Vector Points[8];
		for (int i = 0; i < 8; i++)
		{
			Points[i] = aabb.v[i].pos.ToVector();
		}
		aabb.Reset();
		for (int i = 0; i < 8; i++)
		{
			matv.TransFormVec2(Points[i]);
			aabb.AddInternalPoint(&Points[i]);
		}

		m_pOrtho->SetOrthoOffscreenLH(aabb.m_vMin.x , aabb.m_vMax.x  , aabb.m_vMin.y  , aabb.m_vMax.y , aabb.m_vMin.z  , aabb.m_vMax.z + nearClipOffset);
		m_pShadowCamera->SetProjectionMatrix(m_pOrtho);

	} // PreRender

	//------------------------------------------------------------------------------------
	// The works must to do after rendering the object.
	//------------------------------------------------------------------------------------
	void ShadowMapProjection::Render()
	{
		// Set depth bias
		//gfx::Renderer::GetSingletonPtr()->SetDepthBias(0.0f);
		//gfx::Renderer::GetSingletonPtr()->SetDepthBiasSlope(0.0f);

		m_pRen->SetRenderTarget(0, m_pShadowMap);
		m_pRen->SetClearColor(kge::gfx::Colorf(255,255,255));
		m_pRen->SetViewPort(m_ViewPort);
		if (!m_pRen->BeginRendering(true, true, false)) 
			return;

		// Disable color rendering to speed up rendering when using Hardware shadows
		m_pRen->Disable(gfx::ERF_ColorRendering);
		//m_pRen->SetCullingMode(gfx::ECM_CW);

		m_pShadowCamera->PreRender();
		m_pShadowCamera->Render();
		m_pRen->SetMaterial(m_pMaterial);
		kge::math::Matrix mat;
		std::vector<sn::SceneNode*> meshes = m_pSmgr->GetVisibleMeshes(m_pShadowCamera);
		std::vector<MeshMatrix> arrAlphatest;
		std::vector<MeshMatrix> arrNoAlphatest;
		float fBias = -0.25f * abs(m_pOrtho->_33);
		switch (m_iShadowMapSize)
		{
		case 1024: fBias *= 2.25f; break;
		case 512: fBias *= 2.75f; break;
		case 256: fBias *= 3.0f; break;
		}
		m_pBiasMatrix->_43 = fBias; 
		*m_pFinal = (*m_pOrtho) * m_pShadowCamera->GetViewMatrix(); 
		*m_pBiasFinal = (*m_pBiasMatrix) * (*m_pFinal);
		m_pRen->SetTransForm(m_pFinal, gfx::ETM_Projection);
		m_pRen->SetTransForm(NULL, gfx::ETM_View);
		bool bHasAlphaTest;
		for (size_t i = 0, size = meshes.size(); i < size; ++i)
		{
			if (!meshes[i]->HasShadow()) continue;

			bHasAlphaTest = false;
			for (u32 x = 0; x < meshes[i]->GetMaterialCount(); x++)
			{
				if (meshes[i]->GetMaterial(x)->TextureAlphal)
				{
					AddMeshes(arrAlphatest, arrNoAlphatest, meshes[i]);
					bHasAlphaTest = true;
					break;
				}
			}
			if (bHasAlphaTest)
				continue;
			mat = (*m_pFinal) * (*meshes[i]->GetFinalMatrix());
			m_pVertexShader->SetConstant(m_shMatWVP, mat.m_fMat, 16);

			if (meshes[i]->GetType() == sn::ENT_AnimatedMesh)
				meshes[i]->Draw(false, true, true, false, false);
			else
				meshes[i]->Draw(false, false, true, false, false);

			m_pVertexShader->Render();
		}
		const size_t arrNoAlphatestSize = arrNoAlphatest.size();
		for (u32 i = 0; i < arrNoAlphatestSize; ++i)
		{
			mat = (*m_pFinal) * (*arrNoAlphatest[i].pMatrix);
			m_pVertexShader->SetConstant(m_shMatWVP, mat.m_fMat, 16);
			arrNoAlphatest[i].pMesh->Draw(false, false,true,false,false);
			m_pVertexShader->Render();
		}
		if (!arrAlphatest.empty())
		{
			m_pRen->SetMaterial(m_pMaterial2);
		}
		*m_pFinal = (*m_pOrtho) * (m_pShadowCamera->GetViewMatrix());
		const size_t arrAlphatestSize = arrAlphatest.size();
		for (u32 i = 0; i < arrAlphatestSize; ++i)
		{
			mat = (*m_pFinal) * (*arrAlphatest[i].pMatrix);
			m_pVertexShader->SetConstant(m_shMatWVP, mat.m_fMat, 16);
			m_pRen->SetTexture(arrAlphatest[i].pMesh->m_Mat.ppTexture[0]);
			arrAlphatest[i].pMesh->Draw(false, false,true,true,false);
		}
		// Render instanced objects
		m_pVertexShader->SetConstant(m_shMatWVP, m_pFinal->m_fMat, 16);
		const std::vector<gfx::GeometryBatch*>& vIns = m_pSmgr->GetInstances();
		for (size_t i = 0, size = vIns.size(); i < size; ++i)
		{
			gfx::GeometryBatch* pGB = vIns[i];
			pGB->RenderShadow(m_pMaterial3, m_pMaterial4);
		}

		// Enable color rendering again
		m_pRen->Enable(gfx::ERF_ColorRendering);
		//m_pRen->SetCullingMode(gfx::ECM_CCW);
		
		m_pRen->SetRenderTarget();
		core::RectI vp(0, 0, m_pActiveCamera->GetViewportWidth(), m_pActiveCamera->GetViewportHeight());
		m_pRen->SetViewPort(vp);
		m_pRen->SetTexture(m_pShadowMap, 7);
		m_pRen->SetTextureParams(kge::gfx::ETP_BorderColor, 7);
		m_pRen->SetTextureParams(kge::gfx::ETP_Linear, 7);
		m_pSmgr->SetActiveCamera(m_pActiveCamera);
		// Set depth bias
	//	gfx::Renderer::GetSingletonPtr()->SetDepthBias(0.0002f);
//		gfx::Renderer::GetSingletonPtr()->SetDepthBiasSlope(2.0f);

	} // Render

	//------------------------------------------------------------------------------------
	// The works must to do after rendering all of the scene nodes.
	//------------------------------------------------------------------------------------
	void ShadowMapProjection::PostRenderEverything()
	{

	} // PostRenderEverything

	const char* ShadowMapProjection::GetShaderCode()
	{
		return m_pShaderCode;
	}

	void ShadowMapProjection::AddMeshes( std::vector<MeshMatrix>& Alpha, std::vector<MeshMatrix>& NoAlpha, sn::SceneNode* mesh )
	{
		sn::StaticMesh* pSM;
		if (mesh->GetType() != sn::ENT_StaticMesh)
			return;
		pSM = (sn::StaticMesh*)mesh;

		for (u32 i = 0; i < pSM->GetMaterialCount(); i++)
		{
			if (pSM->GetMaterial(i)->TextureAlphal)
			{
				Alpha.push_back( MeshMatrix(pSM->GetMesh(i), pSM->GetFinalMatrix()) );
			}
			else
			{
				NoAlpha.push_back( MeshMatrix(pSM->GetMesh(i), pSM->GetFinalMatrix()) );
			}
		}
	}

	//------------------------------------------------------------------------------------
	// For internal use
	//------------------------------------------------------------------------------------
	void ShadowMapProjection::OnReset()
	{
		// Set depth bias
		gfx::Renderer::GetSingletonPtr()->SetDepthBias(0.0002f);
		gfx::Renderer::GetSingletonPtr()->SetDepthBiasSlope(2.0f);

	}

} // efx

} // kge
