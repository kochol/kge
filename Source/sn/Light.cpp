// File name: Light.cpp
// Des: This is a lighting scene node.
// Date: 30/4/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/sn/Light.h"
#include "../../Include/sn/SceneManager.h"
#include "../../Include/gfx/Renderer.h"



namespace kge
{
namespace sn
{
	//------------
	// Constructor
	//------------
	Light::Light(gfx::LightData* light, SceneManager* smgr, gfx::Renderer* renderer)
	{
		m_eNodeType	  = ENT_Light;
		m_pLight      = light;
		m_pRenderer   = renderer;
		m_pSnMan      = smgr;
		m_bPosChanged = false;
		m_pBB		  = KGE_NEW(math::Sphere)();
		m_pBB->SetCenter(m_pLight->Position);
		m_pBB->SetRadius(m_pLight->Range);

	} // Constructor

	//-----------
	// Destructor
	//-----------
	Light::~Light()
	{
		KGE_DELETE(m_pLight, LightData);
		KGE_DELETE(m_pBB, Sphere);

	} // Destructor

	//-------------------------------------------------------------------------
	// Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	//-------------------------------------------------------------------------
	void Light::PreRender(float elapsedTime)
	{
		if (m_pParent)
		{
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
			m_Position = m_pLight->Position;
			m_pFinalMat->TransFormVec(m_Position);
		}
		else
			*m_pFinalMat = *m_pAbsMat;

		if (m_bVis)
		{
			m_pSnMan->RegNode(this, ERO_First);
			if (m_pLight->Type == gfx::ELT_Directional)
				m_bVisFrame = true;
			else
			{
				m_pBB->SetCenter(m_Position);
				m_pBB->SetRadius(m_pLight->Range);
				m_bVisFrame = m_pSnMan->IsLightVisible(this);
			}
		}
		else
			m_bVisFrame = false;

	} // PreRender

	//-----------------------
	// Render kardane Object.
	//-----------------------
	void Light::Render()
	{
		//m_pRenderer->SetTransForm(NULL);
		//m_pRenderer->Lights->SetLight(m_pLight, m_pLight->Index);

	} // Render

	//--------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//--------------------------------------------
	void Light::PostRender()
	{
	} // PostRender

	//--------------------------------------
	// Ezafe kardane effect be  hamin gereh.
	//--------------------------------------
	bool Light::AddEffect ( efx::EffectType p, efx::Effect** ppOut )
	{
		return false;

	} // AddEffect

	//---------------------------------------------------------
	// Draw the node with/without its Materials and Transforms.
	//---------------------------------------------------------
	void Light::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{
	} // Draw

	//------------------------------------------------------------------------------------------
	// Set the position of the light.
	//------------------------------------------------------------------------------------------
	void Light::SetPosition(const math::Vector& v)
	{
		m_pAbsMat->SetTranslation(&v.v[0]);
		m_bPosChanged      = true;
		m_pLight->Position = v;
		m_Position = v;

	} // SetPosition

	//------------------------------------------------------------------------------------------
	// Returns the light position.
	//------------------------------------------------------------------------------------------
	const math::Vector& Light::GetPosition() const
	{
		return m_Position;

	} // GetPosition

	//------------------------------------------------------------------------------------------
	// Returns true if the light position changed since this function last call.
	//------------------------------------------------------------------------------------------
	bool Light::IsPositionChanged()
	{
		if (m_bPosChanged)
		{
			m_bPosChanged = false;
			return true;
		}
		return false;

	} // IsPositionChanged

	//------------------------------------------------------------------------------------------
	// Returns the light data to read/write them.
	//------------------------------------------------------------------------------------------
	gfx::LightData* Light::GetLightData() const
	{
		return m_pLight;

	} // GetLightData

} //sn

} // kge
