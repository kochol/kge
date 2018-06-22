// File name: ShadowPlane.cpp
// Des: This class create planar shodow to the flat mesh.
// Date: 07-Apr-2007

#include "../../include/kge/efx/ShadowPlane.h"

#ifndef NULL
#define NULL 0
#endif


namespace kge
{
namespace efx
{

	//------------
	// Constructor
	//------------
	ShadowPlane::ShadowPlane( gfx::Renderer* rn, sn::SceneNode* sn ) :
	m_pRenderer(rn), m_pSceneNode(sn), m_pShadowMatrix(NULL)
	{
	} // Constructor

	//-----------
	// Destructor
	//-----------
	ShadowPlane::~ShadowPlane()
	{
		delete m_pShadowMatrix;
	} // Destructor

	//------------------------
	// Initialize Shadow Plane
	//------------------------
	bool ShadowPlane::Init( math::Matrix* Shadow )
	{
		if ( !Shadow ) { return false; }

		m_pShadowMatrix = Shadow;

		return true;
	} // Init

	//----------------------------------------
	// Add nodes of mesh that they have shadow
	//----------------------------------------
	void ShadowPlane::AddNodes( sn::SceneNode* pNodes )
	{
		m_vNodes.push_back(pNodes);
	}

	//------------------
	// Prerender setting
	//------------------
	void ShadowPlane::PreRender(float elapsedTime)
	{
	} // PreRender

	//--------------------------------
	// 
	//--------------------------------
	void ShadowPlane::Render()
	{
	} // Render

	//--------------------
	// Post render setting
	//--------------------
	void ShadowPlane::PostRenderEverything()
	{
		m_pRenderer->Enable(gfx::ERF_StencilBuffer);
		m_pRenderer->Enable(gfx::ERF_AlphaBlending);

		// Limit the shadow to the specified plane
		m_pRenderer->SetStencilBuffer(efx::EET_ShadowPlane, 3);
		m_pSceneNode->Render();

		// Position Shadow
		math::Matrix FinalMatrix, *T;             

		// Setting stencil buffer before draw shadow
		m_pRenderer->SetStencilBuffer(efx::EET_ShadowPlane, 1);

		// Render Shadow Mesh
		gfx::Material mat;
		mat.Alpha = true;
		mat.Diffuse = gfx::Colorf(0,0,0);
		mat.Diffuse.a = 0.5f;
		mat.ppTexture[0] = NULL;
		m_pRenderer->SetMaterial(&mat);
		for(std::vector<sn::SceneNode*>::iterator it = m_vNodes.begin(); 
			it != m_vNodes.end(); ++it)
		{
			T = (*it)->GetAbsoluteMatrix();
			FinalMatrix = (*m_pShadowMatrix) * (*T);
			m_pRenderer->SetTransForm(&FinalMatrix);
			(*it)->Draw(false,false,true,false,false);
		}
		// Restore render states
		m_pRenderer->Enable(gfx::ERF_DepthBuffer);
		m_pRenderer->SetStencilBuffer(efx::EET_ShadowPlane, 2);
		m_pRenderer->Disable(gfx::ERF_StencilBuffer);
		m_pRenderer->Disable(gfx::ERF_AlphaBlending);

	} // PostRenderEverything

	//-----------------------
	// Set the shadow matrix.
	//-----------------------
	void ShadowPlane::SetShadowMatrix(const kge::math::Matrix& mat)
	{
		if (m_pShadowMatrix)
		{
			*m_pShadowMatrix = mat;
		}

	} // SetShadowMatrix


} // efx

} // kge