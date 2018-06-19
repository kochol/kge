// File name: Decal.cpp
// Des: This class create decals on terrain
// Date: 3/8/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#include "../../include/kge/sn/Decal.h"
#include "../../include/kge/sn/TileTerrain.h"
#include "../../include/kge/gfx/Renderer.h"



namespace kge
{
namespace sn
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Decal::Decal():m_pVertices(NULL), m_pIndices(NULL), 
		m_iICount(0), m_iVCount(0), m_fRot(0.0f), m_iIndex(0),
		m_bPosChanged(false), m_fScale(1.0f), m_pTerrain(NULL),
		m_totalTime(0.0f), m_elapsedTime(0.0f), m_alpha(1.0f)
	{
		m_pMaterial		= KGE_NEW(gfx::Material)();
		m_pMatTexture	= KGE_NEW(math::Matrix)();
		float f[3] = {0.5f, 0.5f, 0.0f};
		m_pMatTexture->SetTranslation(f);
		m_eNodeType		= ENT_Decal;

		m_pMaterial->Alpha = true;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Decal::~Decal()
	{
		m_pTerrain->RemoveDecal(this);
		KGE_DELETE_ARRAY(m_pIndices);
		KGE_DELETE(m_pMaterial, Material);
		KGE_DELETE(m_pMatTexture, Matrix);

	} // Destructor

	//------------------------------------------------------------------------------------
	// The jobs before rendering like culling and calculate position
	//------------------------------------------------------------------------------------
	void Decal::PreRender(float elapsedTime)
	{
		if (m_bVis)
		{
			m_elapsedTime += elapsedTime;
			m_alpha = m_totalTime ? (m_totalTime - m_elapsedTime) / m_totalTime : 1.0f;
			m_alpha = m_alpha < 0.0f ? 0.0f : m_alpha;
			m_pSnMan->RegNode(this, ERO_Decal);
		}

	} // PreRender

	//------------------------------------------------------------------------------------
	// Render the objects
	//------------------------------------------------------------------------------------
	void Decal::Render()
	{
		gfx::Renderer::GetSingletonPtr()->SetTransForm(m_pAbsMat);
		gfx::Renderer::GetSingletonPtr()->SetMaterial(m_pMaterial);

		gfx::Renderer::GetSingletonPtr()->SetVertexBuffer(*m_ppVertexBuffer);
		gfx::Renderer::GetSingletonPtr()->SetIndexBuffer(*m_ppIndexBuffer);
		gfx::Renderer::GetSingletonPtr()->DrawTriangleList(m_iVCount, m_iICount, 
			gfx::Renderer::GetSingletonPtr()->GetVertexDec(gfx::EVT_V3T), 0, m_iIbOffset);

	} // Render

	//------------------------------------------------------------------------------------
	// The jobs after rendering
	//------------------------------------------------------------------------------------
	void Decal::PostRender()
	{

	} // PostRender

	//------------------------------------------------------------------------------------
	// Draw the node with/without its Materials and Transforms.
	//------------------------------------------------------------------------------------
	void Decal::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{

	} // Draw

	//------------------------------------------------------------------------------------
	// Rotates the decal
	//------------------------------------------------------------------------------------
	void Decal::SetRotate( float Radian )
	{
		m_fRot += Radian;
		if (m_bPosChanged)
		{
			Radian = m_fRot;
			m_bPosChanged = false;
		}

		gfx::Vertex3 v;
		v.X = 0.0;
		v.Y = 0.0;
		v.Z = Radian;
		float f[3] = {0.5f, 0.5f, 0.0f};
		m_pMatTexture->SetRotation(&v);
		m_pMatTexture->SetTranslation(f);
		math::Matrix mat;
		f[0] = -0.5;
		f[1] = -0.5;
		mat.SetTranslation(f);
		*m_pMatTexture *= mat;
		for (int i = 0; i < m_iVCount; i++)
		{
			m_pMatTexture->Transform2DVec(m_pVertices[i].tex);
		}

	} // SetRotate

	//------------------------------------------------------------------------------------
	// Scales the decal the value must be between 0.0 and 100.0	
	//------------------------------------------------------------------------------------
	void Decal::SetScale( float Scale )
	{
		if (Scale <= 0.0f)
			return;
		m_fScale = 1.0 / Scale;

		float f[3] = {0.5f, 0.5f, 0.0f};
		m_pMatTexture->SetTranslation(f);
		m_pMatTexture->m_fMat[0]   = m_fScale;
		m_pMatTexture->m_fMat[5]   = m_fScale;
		m_pMatTexture->m_fMat[10]  = m_fScale;
		math::Matrix mat;
		f[0] = -0.5;
		f[1] = -0.5;
		mat.SetTranslation(f);
		*m_pMatTexture *= mat;

		for (int i = 0; i < m_iVCount; i++)
		{
			m_pMatTexture->Transform2DVec(m_pVertices[i].tex);
		}
	} // SetScale

	//--------------------------------------------
	//
	//--------------------------------------------
	void Decal::SetFadeoutTime(float fadeoutTime)
	{
		m_totalTime = fadeoutTime;
		m_elapsedTime = 0.0f;
		m_alpha = 1.0f;
	}

} // sn

} // kge
