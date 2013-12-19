// File name: MirrorPlane.cpp
// Des: In class be shey haye 3D mitone effecte Ayne bede.
// Date: 18/10/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/efx/MirrorPlane.h"
#include "../../Include/io/Logger.h"

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
	MirrorPlane::MirrorPlane( gfx::Renderer* rn, sn::SceneNode* pMirror ) : 
	m_pRenderer(rn), m_pMirror(pMirror), m_pReflectMatrix(NULL)
	{
	} // Constructor

	//-----------
	// Destructor
	//-----------
	MirrorPlane::~MirrorPlane()
	{
		delete m_pReflectMatrix;
	} // Destructor

	//-----------------------------------------------------
	// Sakhtane Mirror Plane va amade kardanesh baraye kar.
	//-----------------------------------------------------
	bool MirrorPlane::Init ( math::Matrix* Reflect )
	{
		if ( !Reflect )
			return false;

		m_pReflectMatrix = Reflect;

		return true;
	} // Init

	//--------------------------------------------
	// Add nodes of mesh that reflecting in mirror
	//--------------------------------------------
	void MirrorPlane::AddNodes( sn::SceneNode* pNodes )
	{
		m_vNodes.push_back(pNodes);
	}

	//----------------------------------
	// Tanzimat ghabl az render on mesh.
	//----------------------------------
	void MirrorPlane::PreRender(float elapsedTime)
	{
	} // PreRender

	//--------------------------------
	// Tsnzimat bad az render on mesh.
	//--------------------------------
	void MirrorPlane::Render()
	{
	} // Render

	//--------------------------------------------------------------------
	// kar hayi ke bad az tamam shodan render hame chiz bayad anjam beshe.
	//--------------------------------------------------------------------
	void MirrorPlane::PostRenderEverything()
	{
		m_pRenderer->Enable(gfx::ERF_StencilBuffer);
		m_pRenderer->Enable(gfx::ERF_AlphaBlending);
		
		// Setting stencil buffer before draw mirror
		m_pRenderer->SetStencilBuffer(efx::EET_MirrorPlane, 1);

		// Draw Mirror Into Stencil Buffer
		m_pMirror->Render();

		// Position reflection
		math::Matrix FinalMatrix, *T;             

		// Setting stencil buffer before draw reflected objects
		m_pRenderer->SetStencilBuffer(efx::EET_MirrorPlane, 2);

		// Clear the Z Buffer.
		m_pRenderer->Clear(false , true , false);

		// Draw the reflected objects
		for(std::vector<sn::SceneNode*>::iterator it = m_vNodes.begin(); 
			it != m_vNodes.end(); ++it)
		{
			T = (*it)->GetAbsoluteMatrix();
			FinalMatrix = (*T) * (*m_pReflectMatrix);
			m_pRenderer->SetTransForm(&FinalMatrix, gfx::ETM_World);
			(*it)->Draw(true,false);
		}

		// Restore render states
		m_pRenderer->SetStencilBuffer(efx::EET_MirrorPlane, 3);
		m_pRenderer->Disable(gfx::ERF_StencilBuffer);
		m_pRenderer->Disable(gfx::ERF_AlphaBlending);
	} // PostRenderEverything

} // efx

} // kge