#include "../../Headers/gfx/GeometryBatch.h"
#include "../../Include/gfx/DynamicHardwareBuffer.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/gfx/Structs.h"
#include "../../Headers/gfx/MeshBuffer.h"
#include "../../Include/sn/StaticMesh.h"
#include "../../Include/sn/AnimatedMesh.h"
#include "../../Include/math/Matrix.h"
#include "../../Include/gfx/MaterialManager.h"

namespace kge
{
namespace gfx
{
	DynamicHardwareBuffer	*	g_pStaticInstancesVB	= NULL;
	DynamicHardwareBuffer	*	g_pAnimatedInstancesVB	= NULL;
	Texture					*	g_pTextureAnimations	= NULL;
	float					*	g_pTempTextureData		= NULL;

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	GeometryBatch::GeometryBatch(MeshBuffer* mb): m_pMeshBuffer(mb), m_pInstancedVB(0)
	{
		if (mb->m_iBoneCount > 0)
		{
			// Create data needed for instancing animated mesh meshes
			if (!g_pAnimatedInstancesVB)
				g_pStaticInstancesVB = KGE_NEW(DynamicHardwareBuffer)(256, 64, 0, 30);
			if (!g_pTextureAnimations)
			{
				// Create the texture that we store animations
				int handle			 = Device::GetSingletonPtr()->GetTextureManager()->Add(NULL, NULL, "g_pTextureAnimations");
				g_pTextureAnimations = Device::GetSingletonPtr()->GetTextureManager()->GetResource(handle);
				g_pTextureAnimations->CreateTexture(256, 256, gfx::ETF_A32B32G32R32F, 1);
				g_pTempTextureData = KGE_NEW_ARRAY(float, 256 * 256 * 4);
			}
			m_pInstancedVB = g_pStaticInstancesVB;
			m_pVDec = Renderer::GetSingletonPtr()->GetVertexDec(EVT_PJ0NT1BT2I3);

			// Calculate max animated instance
			int pixelNeed = mb->m_iBoneCount * 3;
			if (pixelNeed > 256)
				m_iMaxAnimatedInstance = 0;
			else
				m_iMaxAnimatedInstance = (256 / pixelNeed) * 256;

			m_iPixelNeedCount = pixelNeed;
			m_fX = pixelNeed / 256.0f;
			m_fY = 1.0f / 256.0f; 
		}
		else
		{
			// Create data needed for instancing static meshes
			if (!g_pStaticInstancesVB)
				g_pStaticInstancesVB = KGE_NEW(DynamicHardwareBuffer)(100, 64, 0, 100);
			else
				g_pStaticInstancesVB->AddRef();
			m_pInstancedVB = g_pStaticInstancesVB;
			m_pVDec = Renderer::GetSingletonPtr()->GetVertexDec(EVT_P0NT1BT2I3);
		}

		for (u32 i = 0; i < mb->m_iMeshCount; i++)
		{
			mb->m_pMeshes[i].m_pVertexDec = m_pVDec;
		}

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	GeometryBatch::~GeometryBatch()
	{
		if (m_pInstancedVB) m_pInstancedVB->DecRef();

	} // Destructor

	//------------------------------------------------------------------------------------
	// Adds an static mesh as instance.
	//------------------------------------------------------------------------------------
	bool GeometryBatch::AddInstance( sn::StaticMesh* pSmesh )
	{
		m_vStaticInstances.push_back(pSmesh);

		return true;

	} // AddInstance

	//------------------------------------------------------------------------------------
	// Adds an animated mesh as instance.
	//------------------------------------------------------------------------------------
	bool GeometryBatch::AddInstance( sn::AnimatedMesh* pAmesh )
	{
		if (m_vAnimatedInstances.size() == m_iMaxAnimatedInstance)
			return false;

		m_vAnimatedInstances.push_back(pAmesh);

		return true;

	} // AddInstance

	//------------------------------------------------------------------------------------
	// Update the instance buffer and Render the instances
	//------------------------------------------------------------------------------------
	void GeometryBatch::Render()
	{

		if (m_vStaticInstances.size() > 0)
			RenderStaticInstances();

		if (m_vAnimatedInstances.size() > 0)
			RenderAnimatedInstances();

	} // Render

	//------------------------------------------------------------------------------------
	// Render static instances
	//------------------------------------------------------------------------------------
	void GeometryBatch::RenderStaticInstances(Material* pMat1, Material* pMat2)
	{
		for (size_t i = 0; i < m_vStaticInstances.size(); i++)
		{
			m_pInstancedVB->AppendData(m_vStaticInstances[i]->GetFinalMatrix()->m_fMat, 1);
		}

		for (u32 i = 0; i < m_vStaticInstances[0]->GetMaterialCount(); i++)
		{
			if (!m_vStaticInstances[0]->GetMaterial(i)->shader->m_MaterialParams.Instanced)
			{
				m_vStaticInstances[0]->GetMaterial(i)->shader->m_MaterialParams.Instanced = true;
				m_vStaticInstances[0]->GetMaterial(i)->shader->m_bMatParamsChanged = true;
				Device::GetSingletonPtr()->GetSceneManager()->CheckForMaterialsChanges(m_vStaticInstances[0]);
			}
		}

		m_pInstancedVB->Commit();
		Renderer* ren = Renderer::GetSingletonPtr();
		ren->SetVertexDec(m_pVDec);
		ren->SetStreamSourceFreq(0, m_vStaticInstances.size());
		ren->SetStreamSourceFreq(1, m_vStaticInstances.size());
		ren->SetStreamSourceFreq(2, m_vStaticInstances.size());
		ren->SetStreamSourceFreq(3, MAXID);
		ren->SetVertexBuffer(m_pInstancedVB->GetBuffer(), 3);
		ren->SetTransForm(NULL);
		for (u32 i = 0; i < m_pMeshBuffer->m_iMeshCount; i++)
		{
 			if (pMat1)
 			{
 				if (m_vStaticInstances[0]->GetMaterial(i)->TextureAlphal)
 				{
					pMat2->ppTexture[0] = m_vStaticInstances[0]->GetMaterial(i)->ppTexture[0];
 					ren->SetMaterial(pMat2);
 					m_pMeshBuffer->m_pMeshes[i].Render(true, true, false);
 				}
 				else
 				{
 					ren->SetMaterial(pMat1);
 					m_pMeshBuffer->m_pMeshes[i].Render(true, false, false);
 				}
 			}
 			else
 			{
				ren->SetMaterial(m_vStaticInstances[0]->GetMaterial(i));
				m_pMeshBuffer->m_pMeshes[i].Render();
			}
		}

		ren->SetStreamSourceFreq(0, 1);
		ren->SetStreamSourceFreq(1, 1);
		ren->SetStreamSourceFreq(2, 1);
		ren->SetStreamSourceFreq(3, 1);

		if (!pMat1)
			m_vStaticInstances.clear();

	} // RenderStaticInstances

	//------------------------------------------------------------------------------------
	// Render animated instances
	//------------------------------------------------------------------------------------
	void GeometryBatch::RenderAnimatedInstances()
	{
		// Create animation texture
		float tx = 0.0f;
		float ty = 0.0f;
		float fMat[16] = {0};
		int BoneCount = m_pMeshBuffer->m_iBoneCount;
		int Offset = 0;
		for (size_t i = 0; i < m_vAnimatedInstances.size(); i++)
		{
			sn::Bone* pBones = m_vAnimatedInstances[i]->GetBones();
			Offset = 256 * 4 * i;
			for (int b = 0; b < BoneCount; b++)
			{
				pBones[b].m_matFinal.Get4x3Matrix(fMat);
// 				static float r = 0.0f;
// 				r += 0.0001f;
//  				fMat[9] = r;//rand() % 10;
//  				fMat[10] = r;//rand() % 10;
//  				fMat[11] = r;//rand() % 10;
				memcpy(&g_pTempTextureData[Offset], fMat, 12 * sizeof(float));
				Offset += 12;
			}

			// Add world matrix and texcoord to VB
			m_vAnimatedInstances[i]->GetFinalMatrix()->Get4x3Matrix(fMat);
			fMat[12] = 0.0f;
			fMat[13] = i * m_fY;
			m_pInstancedVB->AppendData(fMat, 1);
		}

		// Send animations to texture
		g_pTextureAnimations->SetData(0,0,256,m_vAnimatedInstances.size(),(u8*)g_pTempTextureData, 256 * 256 * 4 * 4);

		for (u32 i = 0; i < m_vAnimatedInstances[0]->GetMaterialCount(); i++)
		{
			if (!m_vAnimatedInstances[0]->GetMaterial(i)->shader->m_MaterialParams.Instanced)
			{
				m_vAnimatedInstances[0]->GetMaterial(i)->shader->m_MaterialParams.Instanced = true;
				m_vAnimatedInstances[0]->GetMaterial(i)->shader->m_MaterialParams.BoneCount = BoneCount;
				m_vAnimatedInstances[0]->GetMaterial(i)->shader->m_bMatParamsChanged = true;
				Device::GetSingletonPtr()->GetSceneManager()->CheckForMaterialsChanges(m_vAnimatedInstances[0]);
			}
		}

		m_pInstancedVB->Commit();
		Renderer* ren = Renderer::GetSingletonPtr();
		ren->SetVertexDec(m_pVDec);
		ren->SetStreamSourceFreq(0, m_vAnimatedInstances.size());
		ren->SetStreamSourceFreq(1, m_vAnimatedInstances.size());
		ren->SetStreamSourceFreq(2, m_vAnimatedInstances.size());
		ren->SetStreamSourceFreq(3, MAXID);
		ren->SetVertexBuffer(m_pInstancedVB->GetBuffer(), 3);
		ren->SetTransForm(NULL);
// 		int j = m_vAnimatedInstances[0]->GetMaterial(0)->shader->m_pVertexShader->GetTextureIndex("sampBones");
// 		printf("%d %d\n", j);
		for (u32 i = 0; i < m_pMeshBuffer->m_iMeshCount; i++)
		{
			ren->SetMaterial(m_vAnimatedInstances[0]->GetMaterial(i));
			ren->SetTexture(g_pTextureAnimations, 257);
			m_pMeshBuffer->m_pMeshes[i].Render();
		}

		ren->SetStreamSourceFreq(0, 1);
		ren->SetStreamSourceFreq(1, 1);
		ren->SetStreamSourceFreq(2, 1);
		ren->SetStreamSourceFreq(3, 1);

		m_vAnimatedInstances.clear();

	} // RenderAnimatedInstances

	//------------------------------------------------------------------------------------
	// For rendering shadows
	//------------------------------------------------------------------------------------
	void GeometryBatch::RenderShadow( Material* pMat1, Material* pMat2 )
	{
		if (m_vStaticInstances.size() > 0)
			RenderStaticInstances(pMat1, pMat2);

	} // RenderShadow

} // gfx

} // kge
