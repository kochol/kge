#include "../../Include/efx/xpsm.h"
#include "../../Include/sn/Light.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/math/math.h"
#include <math.h>

namespace kge
{
	namespace efx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		XPSM::XPSM(int ShadowMapSize, sn::SceneManager* pSmgr): 
			ShadowMapProjection(ShadowMapSize, pSmgr)
		{
			pSmgr->SetStoreShadowPoints(true);
			m_pRen = gfx::Renderer::GetSingletonPtr();

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		XPSM::~XPSM()
		{
		
		} // Destructor

		//------------------------------------------------------------------------------------
		// Inits the XPSM
		//------------------------------------------------------------------------------------
		bool XPSM::Init()
		{
			// Create shadow map
			if (!gfx::Renderer::GetSingletonPtr()
				->CreateRenderableTexture( &m_pShadowMap, 
				m_iShadowMapSize,
				m_iShadowMapSize,
				gfx::ETF_A8R8G8B8))
			{
				io::Logger::Log(io::ELM_Error, "Can't create the shadow map.");
				return false;
			}

			m_ViewPort.X		= 0;
			m_ViewPort.Y		= 0;
			m_ViewPort.Width	= m_iShadowMapSize;
			m_ViewPort.Height	= m_iShadowMapSize;

			// Create shaders
			const char strVS[] = "float4x4 matViewProjection;\n"\
				"struct VS_INPUT\n"\
				"{\n"\
				"	float4 Position : POSITION0;\n"\
				"};\n"\
				"struct VS_OUTPUT \n"\
				"{\n"\
				"	float4 Position : POSITION0;\n"\
				"	float4 TexCoord0 : TEXCOORD0;\n"\
				"};\n"\
				"VS_OUTPUT main( VS_INPUT Input )\n"\
				"{\n"\
				"	VS_OUTPUT Output;\n"\
				"	Output.Position = mul( Input.Position, matViewProjection );\n"\
				"	Output.TexCoord0 = float4(Output.Position.zzz, Output.Position.w);\n"
				"	return( Output );\n"\
				"}\n";

			m_pVertexShader = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS, "main", gfx::ESV_VS2);

			m_shMatWVP		= m_pVertexShader->GetConstatnt("matViewProjection");

			const char strPS[] =   
				"float4 main(in float4 TexCoord0 : TEXCOORD0) : COLOR0\n"\
				"{\n"\
				"	float depth = TexCoord0.b / TexCoord0.a;\n"
				"	return float4(depth.xxx,1.0);\n"\
				"}";
			m_pPixelShader = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(strPS, "main", gfx::ESV_PS2);

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

			m_pVertexShader2 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS2, "main", gfx::ESV_VS2);

			m_shMatWVP2		= m_pVertexShader2->GetConstatnt("matViewProjection");

			const char strPS2[] =   
				"sampler2D Texture0;\n"\
				"float4 main(float2 tex: TEXCOORD0) : COLOR0\n"\
				"{\n"\
				"	return float4(0.5,0.5,0.5, tex2D( Texture0, tex ).a);\n"\
				"}";

			m_pPixelShader2 = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(strPS2, "main", gfx::ESV_PS2);

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

			m_pVertexShader3 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS3, "main", gfx::ESV_VS2);

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

			m_pVertexShader4 = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString(strVS4, "main", gfx::ESV_VS2);

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

			m_pProj			= KGE_NEW(math::Matrix)();
			m_pOrtho		= KGE_NEW(math::Matrix)();
			m_pFinal		= KGE_NEW(math::Matrix)();
			m_pFrustum		= new math::Frustum();
			m_pRen			= gfx::Renderer::GetSingletonPtr();
			m_pActiveCamera = m_pSmgr->GetActiveCamera();
			m_pShadowCamera = m_pSmgr->AddCameraNode(math::Vector(1.0f, 1.0f, 1.0f), math::Vector(), math::Vector(0.0f, 1.0f, 0.0f));
			if (m_pActiveCamera)
				m_pSmgr->SetActiveCamera(m_pActiveCamera);

			m_pShaderCode	= "sampler2D ShadowMapS: register(s7);\n"\
				"float sample_shadow(float2 tc, float2 shift, float depth_cmp)\n"\
				"{\n"\
				"	float  texsize = 1024.0;\n"\
				"float  offset = 0.5 / texsize;\n"\
				"tc += shift * (1.0 / texsize);\n"\
				"\n"\
				"float2 Texc0  = tc + float2( offset, offset);\n"\
				"float2 Texc1  = tc + float2(-offset,-offset);\n"\
				"float2 Texc2  = tc + float2(-offset, offset);\n"\
				"float2 Texc3  = tc + float2( offset,-offset);\n"\
				"float4 depth  = float4(depth_cmp - tex2D(ShadowMapSampler, Texc0).x, \n"\
				"depth_cmp - tex2D(ShadowMapSampler, Texc1).x,\n"\
				"depth_cmp - tex2D(ShadowMapSampler, Texc2).x, \n"\
				"depth_cmp - tex2D(ShadowMapSampler, Texc3).x);\n"\
				"float4  compare = step(depth, 0.0);\n"\
				"float2 	fr 		= frac	(Texc0*texsize);\n"\
				"float2 	ifr 	= float2(1.0,1.0) - fr;\n"\
				"float4 	fr4 	= float4(ifr.x*ifr.y, ifr.x*fr.y, fr.x*ifr.y,  fr.x*fr.y);\n"\
				"\n"\
				"return  dot(compare, fr4);\n"\
				"}\n"\
				"float GetShadow(float4 ftex)\n"\
				"{\n"\
				"float2 shadowTexCoord = ftex.xy / ftex.w;\n"\
				"float  shadowTestDepth = (ftex.z - ZBias) / ftex.w;\n"\
				"\n"\
				"float shadow;\n"\
				"shadow  = sample_shadow(shadowTexCoord, float2( 1.0, 1.0), shadowTestDepth);\n"\
				"shadow += sample_shadow(shadowTexCoord, float2(-1.0,-1.0), shadowTestDepth);\n"\
				"shadow += sample_shadow(shadowTexCoord, float2( 1.0,-1.0), shadowTestDepth);\n"\
				"//	shadow += sample_shadow(shadowTexCoord, float2(-1.0, 1.0), shadowTestDepth);\n"\
				"return shadow;\n"
				"}\n";

			gfx::Renderer::GetSingletonPtr()->SetTextureParams(kge::gfx::ETP_Point, 7);

			return true;

		} // Init

		//------------------------------------------------------------------------------------
		// The works must to do before rendering the object.
		//------------------------------------------------------------------------------------
		void XPSM::PreRender(float elapsedTime)
		{
			sn::Light* lit = m_pSmgr->GetNearestLight(kge::math::Vector());
			// If there is a directional light in the scene
			if (lit->GetLightData()->Type != gfx::ELT_Directional)
				return;

			math::Vector m_lightDir = lit->GetLightData()->Direction;
			math::Matrix m_View = m_pRen->GetTransForm(gfx::ETM_View);
			bool hit = m_pSmgr->GetShadowHit();
			std::vector<math::AABB>* pShadowReceiverPoints 
				= m_pSmgr->GetShadowReciverPoints();

			//  these are the limits specified by the physical camera
			//  gamma is the "tilt angle" between the light and the view direction.
			m_fCosGamma = m_lightDir.x * m_View._31 +
				m_lightDir.y * m_View._32 +
				m_lightDir.z * m_View._33;

			if (!hit)
			{
				m_zNear = ZNEAR_MIN;
				m_zFar = ZFAR_MAX;
				m_fSlideBack = 0.f;
			}
			else
			{
				float min_z = 1e32f;
				float max_z = 0.f;
				for (unsigned int i=0;i < pShadowReceiverPoints->size(); i++) 
				{
					min_z = math::Min(min_z, (*pShadowReceiverPoints)[i].m_vMin.z);
					max_z = math::Max(max_z, (*pShadowReceiverPoints)[i].m_vMax.z);
				}
				m_zNear = math::Max(ZNEAR_MIN, min_z);  
				m_zFar = math::Min( ZFAR_MAX, max_z );
				m_fSlideBack = 0.f;
			}

		} // PreRender

		//------------------------------------------------------------------------------------
		// The works must to do after rendering the object.
		//------------------------------------------------------------------------------------
		void XPSM::Render()
		{
			BuildXPSMProjectionMatrix();
			m_pRen->SetRenderTarget(0, m_pShadowMap);
			m_pRen->SetClearColor(kge::gfx::Colorf(255,255,255));
			m_pRen->SetViewPort(m_ViewPort);
			if (!m_pRen->Clear(true, true, true)) return;

			m_pRen->SetTransForm(&m_LightViewProj, gfx::ETM_Projection);
			m_pRen->SetTransForm(NULL, gfx::ETM_View);
			m_pRen->SetMaterial(m_pMaterial);
			kge::math::Matrix mat;
			
			bool bHasAlphaTest;
			for (size_t i = 0, size = m_pSmgr->m_vShadowCasterNodes.size(); i < size; ++i)
			{
				if (!m_pSmgr->m_vShadowCasterNodes[i]->HasShadow()) continue;

				mat = m_LightViewProj * (*m_pSmgr->m_vShadowCasterNodes[i]->GetFinalMatrix());
				m_pVertexShader->SetConstant(m_shMatWVP, mat.m_fMat, 16);
				m_pSmgr->m_vShadowCasterNodes[i]->Draw(false, false, true, false, false);
				m_pVertexShader->Render();
			}

			m_pRen->SetRenderTarget();
 			core::RectI vp(0, 0, m_pActiveCamera->GetViewportWidth(), m_pActiveCamera->GetViewportHeight());
 			m_pRen->SetViewPort(vp);
 			m_pRen->SetTexture(m_pShadowMap, 7);
 			m_pRen->SetTextureParams(kge::gfx::ETP_Clamp, 7);
 			m_pSmgr->SetActiveCamera(m_pActiveCamera);

		} // Render

		//------------------------------------------------------------------------------------
		// The works must to do after rendering all of the scene nodes.
		//------------------------------------------------------------------------------------
		void XPSM::PostRenderEverything()
		{
		} // PostRenderEverything

		//------------------------------------------------------------------------------------
		// Builds XPSM Projection Matrix
		//------------------------------------------------------------------------------------
		void XPSM::BuildXPSMProjectionMatrix()
		{
			// coef - constant tuned for near/far objects trade off and
			// scene scale
			const float coef = 0.05f * m_XPSM_Coef;
			// epsilonW - define how far to "infinity" warping can be pushed
			// in critical case
			const float epsilonW = m_XPSM_EpsilonW;

			std::vector<math::AABB>* pShadowReceiverPoints 
				= m_pSmgr->GetShadowReciverPoints();
			std::vector<math::AABB>* pShadowCasterPoints 
				= m_pSmgr->GetShadowCasterPoints();

			// shadow casters and receivers all defined in VIEW SPACE
			// copy shadow casters and receivers
			std::vector<gfx::Vertex3> shadowCasters; 
			shadowCasters.reserve( pShadowCasterPoints->size()*8 + 8 );
			std::vector<gfx::Vertex3> shadowReceivers; 
			shadowReceivers.reserve( pShadowReceiverPoints->size()*8 + 8 );

			int i;
			std::vector<math::AABB>::iterator shadowCastersIt = pShadowCasterPoints->begin();
			while (shadowCastersIt != pShadowCasterPoints->end() )
			{
				math::AABB& box = *shadowCastersIt++;
				for (i = 0; i < 8; ++i) shadowCasters.push_back( box.v[i].pos );
			}

			std::vector<math::AABB>::iterator shadowReceiversIt = pShadowReceiverPoints->begin();
			while (shadowReceiversIt != pShadowReceiverPoints->end() )
			{
				math::AABB& box = *shadowReceiversIt++;
				for (i = 0; i < 8; ++i) shadowReceivers.push_back( box.v[i].pos );
			}

			// transform light direction into view space
			sn::Light* lit = m_pSmgr->GetNearestLight(kge::math::Vector());
			math::Vector m_lightDir = lit->GetLightData()->Direction;
			math::Matrix m_View = m_pRen->GetTransForm(gfx::ETM_View);

			math::Vector viewLightDir = m_lightDir;
			m_View.TransFormVec2(viewLightDir);
			viewLightDir.Negate();

			// build light space lookAt matrix
			math::Matrix lightSpace;
			math::Vector eyeVector(0.0f, 0.0f, 0.0f);
			math::Vector upVector(0.0f, 1.0f,0.0f);
			lightSpace.SetViewLookatLH(eyeVector, viewLightDir, upVector);

			// transform view vector into light space
			//  (0, 0, 1) * LightSpace
			math::Vector V(lightSpace._31, 
				lightSpace._32, 
				lightSpace._33);

			// project view vector into xy plane and find
			// unit projection vector
			math::Vector2F unitP(V.x, V.y);

			float unitPLength = sqrtf(unitP.x * unitP.x +
				unitP.y * unitP.y);

			// PPLS (post projection light space) transformation
			math::Matrix PPLSTransform;

			// check projection vector length 
			if (unitPLength > 0.1f)
			{
				// use perspective reparameterization
				// normalize projection vector
				unitP.x *= 1.0f / unitPLength;
				unitP.y *= 1.0f / unitPLength;

				// transform shadow casters and receivers to light space
				// transform shadow casters into light space
				std::vector<gfx::Vertex3> LSshadowCasters; 
				LSshadowCasters.resize(shadowCasters.size());
				lightSpace.TransFormVec2(&LSshadowCasters[0], &shadowCasters[0], shadowCasters.size());

				// transform shadow receivers into light space
				std::vector<gfx::Vertex3> LSshadowReceivers; 
				LSshadowReceivers.resize(shadowReceivers.size());
				lightSpace.TransFormVec2(&LSshadowReceivers[0], &shadowReceivers[0], shadowReceivers.size());

				// project casters and receivers points into unit projection vector and
				// find minimal value 
				float minCastersProj = 1.0f;
				for (i = 0; i < shadowCasters.size(); ++i)
				{
					float proj = LSshadowCasters[i].X * unitP.x + 
						LSshadowCasters[i].Y * unitP.y;
					minCastersProj = math::Min(minCastersProj, proj);
				}

				float minRecieversProj = 1.0f;
				for (i = 0; i < shadowReceivers.size(); ++i)
				{
					float proj = LSshadowReceivers[i].X * unitP.x + 
						LSshadowReceivers[i].Y * unitP.y;
					minRecieversProj = math::Min(minRecieversProj, proj);
				}

				// find focus region interval minimal point 
				// max(min Casters, min Receivers)
				float minFocusRegionProj = math::Max(minCastersProj, minRecieversProj);

				// find maximum projection vector length
				// 1 / (minProj * maxProjeVectorLength  + 1) != 0
				// minProj * maxProjVectorLength + 1 = thresholdW
				// !!!! TEST + RECEIVERS && CASTERS CODE
				float maxLengthP = (epsilonW - 1.0f) / minFocusRegionProj;

				// find optimal (fixed warping) projection vector length
				float cosGamma = V.x * unitP.x + V.y * unitP.y;
				float lengthP = coef / cosGamma;

				// clip projection vector length
				if (maxLengthP > 0.0f && 
					lengthP > maxLengthP)
				{
					lengthP = maxLengthP;
				}

				// calculate Projection matrix
				kge::math::Matrix Projection(1.0f, 0.0f, 0.0f, unitP.x * lengthP,
					0.0f, 1.0f, 0.0f, unitP.y * lengthP,
					0.0f, 0.0f, 1.0f, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);

				// to maximize shadow map resolution
				// calculate Z rotation basis 
				kge::math::Matrix ZRotation(unitP.x,  unitP.y, 0.0f, 0.0f,
					unitP.y, -unitP.x, 0.0f, 0.0f,
					0.0f,        0.0f, 1.0f, 0.0f,
					0.0f,        0.0f, 0.0f, 1.0f);

				// combine transformations
				PPLSTransform = Projection * ZRotation;
			}
			else
			{
				// miner lamp case, use orthogonal shadows
				PPLSTransform.Identity();
			}

			// final, combined PPLS transformation: LightSpace * PPLS Transform
			kge::math::Matrix PPLSFinal;
			PPLSFinal = lightSpace * PPLSTransform;

			// transform casters and receivers into PPLS (post projection light space)
			std::vector<gfx::Vertex3> PPLScasters;
			std::vector<gfx::Vertex3> PPLSreceivers;
			TransformWithWClip(shadowCasters, PPLScasters, 
				PPLSFinal, epsilonW);
			TransformWithWClip(shadowReceivers, PPLSreceivers, 
				PPLSFinal, epsilonW);

			// find PPLS AABB's for caster*s and receivers
			math::AABB PPLSCastersAABB;
			for (int i = 0; i < PPLScasters.size(); i++)
			{
				PPLSCastersAABB.AddInternalPoint(&PPLScasters[i]);
			}
			math::AABB PPLSReceiversAABB;
			for (int i = 0; i < PPLSreceivers.size(); i++)
			{
				PPLSReceiversAABB.AddInternalPoint(&PPLSreceivers[i]);
			}

			// find post projection space focus region
			math::AABB PPLSFocusRegionAABB;
			IntersectAABB2D(PPLSFocusRegionAABB, PPLSCastersAABB, PPLSReceiversAABB);

			// build focus region linear basis
			float minZ = math::Min(PPLSCastersAABB.m_vMin.z, PPLSReceiversAABB.m_vMin.z);
			float maxZ = math::Max(PPLSCastersAABB.m_vMax.z, PPLSReceiversAABB.m_vMax.z);
			kge::math::Vector minPtBox = PPLSFocusRegionAABB.m_vMin;
			kge::math::Vector maxPtBox = PPLSFocusRegionAABB.m_vMax;

			// find focusRegionBasis^-1
			math::Matrix unitCubeBasis(1.0f / (maxPtBox.x - minPtBox.x), 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f / (maxPtBox.y - minPtBox.y), 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f / (maxZ - minZ), 0.0f,
				-minPtBox.x /(maxPtBox.x - minPtBox.x), 
				-minPtBox.y / (maxPtBox.y - minPtBox.y),
				-minZ / (maxZ - minZ),
				1.0f);

			// transform from a unit cube into d3d normalized space
			math::Matrix normalizedSpaceD3D(2.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 2.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				-1.0f,-1.0f, 0.0f, 1.0f);

			// finally multiply all transformations
			m_LightViewProj = m_View * PPLSFinal;
			m_LightViewProj = m_LightViewProj * unitCubeBasis;
			m_LightViewProj = m_LightViewProj * normalizedSpaceD3D;

			// compute correct PPLS Z bias
			m_XPSM_ZBias = 10.0f * m_XPSM_Bias / (maxZ - minZ) ;

		} // BuildXPSMProjectionMatrix

		void XPSM::TransformWithWClip( 
			const std::vector<gfx::Vertex3>& inVectors, 
			std::vector<gfx::Vertex3>& outVectors, 
			const math::Matrix& transform, 
			float wThreshold )
		{
			outVectors.resize(0);
			outVectors.reserve(inVectors.size());

			int i;
			for (i = 0; i < inVectors.size(); ++i)
			{
				double X = inVectors[i].X*transform[0] + inVectors[i].Y*transform[4] + inVectors[i].Z*transform[8] + transform[12];
				double Y = inVectors[i].X*transform[1] + inVectors[i].Y*transform[5] + inVectors[i].Z*transform[9] + transform[13];
				double Z = inVectors[i].X*transform[2] + inVectors[i].Y*transform[6] + inVectors[i].Z*transform[10] + transform[14];
				double w = inVectors[i].X*transform[3] + inVectors[i].Y*transform[7] + inVectors[i].Z*transform[11] + transform[15];

				if (w > wThreshold)
				{
					gfx::Vertex3 result(X / w, Y / w, Z / w);
					outVectors.push_back(result);
				}
			}

		}

		bool XPSM::IntersectAABB2D( math::AABB& c, const math::AABB& a, const math::AABB& b )
		{
			c.m_vMin.z = c.m_vMax.z = 0.0f;

			if (a.m_vMax.x < b.m_vMin.x || a.m_vMin.x > b.m_vMax.x)
				return false;

			if (a.m_vMax.y < b.m_vMin.y || a.m_vMin.y > b.m_vMax.y)
				return false;

			if (a.m_vMax.x <= b.m_vMax.x)
				c.m_vMax.x = a.m_vMax.x;
			else
				c.m_vMax.x = b.m_vMax.x;

			if (a.m_vMax.y <= b.m_vMax.y)
				c.m_vMax.y = a.m_vMax.y;
			else
				c.m_vMax.y = b.m_vMax.y;

			if (a.m_vMin.x <= b.m_vMin.x)
				c.m_vMin.x = b.m_vMin.x;
			else
				c.m_vMin.x = a.m_vMin.x;

			if (a.m_vMin.y <= b.m_vMin.y)
				c.m_vMin.y = b.m_vMin.y;
			else
				c.m_vMin.y = a.m_vMin.y;

			// x
			if (a.m_vMax.x <= b.m_vMax.x)
				c.m_vMax.x = a.m_vMax.x;
			else
				c.m_vMax.x = b.m_vMax.x;

			if (a.m_vMin.x <= b.m_vMin.x)
				c.m_vMin.x = b.m_vMin.x;
			else
				c.m_vMin.x = a.m_vMin.x;

			// y
			if (a.m_vMax.y <= b.m_vMax.y)
				c.m_vMax.y = a.m_vMax.y;
			else
				c.m_vMax.y = b.m_vMax.y;

			if (a.m_vMin.y <= b.m_vMin.y)
				c.m_vMin.y = b.m_vMin.y;
			else
				c.m_vMin.y = a.m_vMin.y;

		}
	} // efx

} // kge
