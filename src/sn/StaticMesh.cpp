// File name: AnimatedMesh.cpp
// Des: This is a node class for static meshes which is more faster.
// Date: 19/4/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/sn/StaticMesh.h"
#include "../../include/kge/sn/Light.h"
#include "../../include/kge/io/Logger.h"
#include "../../include/kge/math/Quat.h"
#include "../../include/kge/math/Plane.h"
#include "../../include/kge/gfx/MeshBuffer.h"
#include "../../Headers/gfx/GeometryBatch.h"

namespace kge
{
namespace sn
{

	//-------------
	// Constructor.
	//-------------
	StaticMesh::StaticMesh(gfx::MeshBuffer* mb):
		m_pMeshBuffer(mb)
	{
		mb->AddRef();
		m_pMeshes	 = mb->m_pMeshes;
		m_iNumMeshes = mb->m_iMeshCount;
		m_eNodeType	 = ENT_StaticMesh;
		m_pBB		 = KGE_NEW(math::Sphere)();
		m_pAABB		 = KGE_NEW(math::AABB)();
		m_pMaterials = KGE_NEW_CLASS_ARRAY(gfx::Material, m_iNumMeshes);
		m_pMeshes[0].AddRef();
		for (unsigned int i = 0; i < m_iNumMeshes; i++)
		{
			m_pAABB->AddAABB(m_pMeshes[i].GetAxisAlignedBoundingBox());
			m_pMaterials[i] = m_pMeshes[i].m_Mat;
			m_pMaterials[i].shader->m_pOwner = this;
		}
		m_Scale.set(1,1,1);
		m_Rotation.set(0,0,0);

	} // Constructor

	//------------
	// Destructor.
	//------------
	StaticMesh::~StaticMesh()
	{
		if ( m_pMeshes )
		{
			m_pMeshes[0].DecRef();
			m_pMeshes = NULL;
		}
		KGE_DELETE(m_pBB, Sphere);
		KGE_DELETE(m_pAABB, AABB);
		KGE_DELETE_CLASS_ARRAY(m_pMaterials, Material, m_iNumMeshes);
		m_pMeshBuffer->DecRef();

	} // Destructor

	//-------------------------------------------------------------------------
	// Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	//-------------------------------------------------------------------------
	void StaticMesh::PreRender(float elapsedTime)
	{
		if (m_pParent)
		{
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
		}
		else
		{
			*m_pFinalMat = *m_pAbsMat;
		}

		if (m_bVis)
		{
			if ( !m_bAutoCulling || m_pSnMan->IsVisible(this) == math::ECT_In )
			{
				if (m_pMeshBuffer->m_bInstanced)
				{
					m_bVisFrame = true;
					m_pMeshBuffer->m_pBatch->AddInstance(this);
				}
				else
				{
					m_pSnMan->RegNode(this);
				}
			}
		}

	} // PreRender

	//-----------------------
	// Render kardane Object.
	//-----------------------
	void StaticMesh::Render()
	{
		m_pRenderer->SetTransForm(m_pFinalMat);
		for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
		{
			m_pRenderer->SetMaterial(&m_pMaterials[x]);
			m_pMeshes[x].Render();
		}

	} // Render

	//--------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//--------------------------------------------
	void StaticMesh::PostRender()
	{
	} // PostRender

	bool StaticMesh::AddEffect ( efx::EffectType p, efx::Effect** ppOut )
	{
		if ( this->HasThisEffect(p) )
			return true;

		switch ( p )
		{
			// Shadow Plane
			case efx::EET_ShadowPlane:
				CreateShadowPlane( ppOut );
				break;

			// Mirror Plane
			case efx::EET_MirrorPlane:
				CreateMirrorPlane( ppOut );
				break;

			// Bump Map
			case efx::EET_BumpMap:
				break;

			// Default
			default:
				return false;
		} // switch ( p )

		m_vEffectType.push_back(p);
		m_pSnMan->AddEffect(*ppOut, p);

		return true;

	} // AddEffect

	//---------------------
	// Create Mirror Plane.
	//---------------------
	void StaticMesh::CreateMirrorPlane( efx::Effect** ppCreatedMirror )
	{
		math::Vector v0 = m_pMeshes[0].m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pMeshes[0].m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pMeshes[0].m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( v0 , v1 , v2 );

		*ppCreatedMirror = KGE_NEW(efx::MirrorPlane)( m_pRenderer , this );
		((efx::MirrorPlane*)(*ppCreatedMirror))->Init( MeshPlane.GetReflectMatrix() );

		m_vEffect.push_back(*ppCreatedMirror);

	} // CreateMirrorPlane

	//---------------------
	// Create Shadow Plane.
	//---------------------
	void StaticMesh::CreateShadowPlane( efx::Effect** ppCreatedShadow )
	{
		sn::Light* lit = m_pSnMan->GetNearestLight(this->GetPosition());
		if (!lit)
		{
			io::Logger::Log("Can't create the shadow plane. No light found.", io::ELM_Warning);
			return;
		}
		math::Vector v;
		if (lit->GetLightData()->Type == gfx::ELT_Directional)
		{
			v   = lit->GetLightData()->Direction;
			v.Negate();
			v.w = 0.0f;
		}
		else
		{
			v   = lit->GetLightData()->Position;
			v.w = 1.0f;
		}

		math::Vector v0 = m_pMeshes[0].m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pMeshes[0].m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pMeshes[0].m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( v0 , v1 , v2 );

		*ppCreatedShadow = KGE_NEW(kge::efx::ShadowPlane)( m_pRenderer , this );
		((efx::ShadowPlane*)(*ppCreatedShadow))->Init( MeshPlane.GetShadowMatrix(v) );

		m_vEffect.push_back(*ppCreatedShadow);

	} // CreateShadowPlane

	//---------------------------------------------------------
	// Draw the node with/without its Materials and Transforms.
	//---------------------------------------------------------
	void StaticMesh::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{
		if ( WithTransform )
			m_pRenderer->SetTransForm(m_pAbsMat);
		for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
		{
			if ( WithMaterial )
				m_pRenderer->SetMaterial(&m_pMaterials[x]);
			m_pMeshes[x].Render(bPosition, bNormalTexcoord, bTangentBinormal);
		}

	} // Draw

	//------------------------------
	//
	//------------------------------
	void StaticMesh::SetShader(gfx::ShaderInstance* pSh)
	{
		for ( u32 x = 0 ; x < m_iNumMeshes; x++ )
		{
			m_pMaterials[x].shader = pSh;
		}

	} // SetShader

	//------------------------------------------
	// Get the count of materials this node has.
	//------------------------------------------
	u32 StaticMesh::GetMaterialCount()
	{
		return m_iNumMeshes;

	} // GetMaterialCount

	//------------------------------------
	// Get the node material by its index.
	//------------------------------------
	gfx::Material* StaticMesh::GetMaterial(u32 Index)
	{
		return &m_pMaterials[Index];

	} // GetMaterial

	//------------------------------------------------------------------------------------
	// Update final matrix
	//------------------------------------------------------------------------------------
	void StaticMesh::UpdateFinalMat()
	{
		if (m_pParent)
		{
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
		}
	} // UpdateFinalMat

} // sn

} // kge
