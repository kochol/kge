// File name: Mesh.h
// Des: In class ye mesh standard hast ke tavasote engine namayesh dade mishe.
// Date: 13/7/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/sn/Mesh.h"
#include "../../Include/gfx/MeshManager.h"
#include "../../Include/math/Plane.h"



namespace kge
{
namespace sn
{
	//----------------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------------
	Mesh::Mesh():m_pIndexBuffer(NULL), m_pvbPosition(NULL), m_pvbNormalTexcoord(NULL),
		m_pvbTangentBinormal(NULL), m_pPos(NULL), m_pNorTex(NULL), m_pVerts(NULL),
		m_pTangentBinormal(NULL), m_pNorTexColor(NULL), m_pIndices(NULL), 
		m_pIndices32(NULL), m_Bone(NULL), m_bInstanced(false)
	{
		m_pBB		    = KGE_NEW(math::Sphere)();
		m_pAABB         = KGE_NEW(math::AABB)();
		m_pVertexDec    = gfx::Renderer::GetSingletonPtr()->GetVertexDec(gfx::EVT_P0NT1BT2I3);
		m_eNodeType		= ENT_Mesh;
		m_pSnMan		= Device::GetSingletonPtr()->GetSceneManager();
		m_pRenderer		= gfx::Renderer::GetSingletonPtr();

	} // Constructor

	//----------------------------------------------------------------------------------------
	// Constructor
	//----------------------------------------------------------------------------------------
	Mesh::Mesh(SceneManager *sn, gfx::Renderer *rn):
		m_pIndexBuffer(NULL), m_pvbPosition(NULL), m_pvbNormalTexcoord(NULL),
		m_pvbTangentBinormal(NULL), m_pPos(NULL), m_pNorTex(NULL), m_pVerts(NULL),
		m_pTangentBinormal(NULL), m_pNorTexColor(NULL), m_pIndices(NULL), 
		m_pIndices32(NULL), m_bInstanced(false), m_Bone(0)
	{
		m_pSnMan		= sn;
		m_pRenderer		= rn;
		m_pBB			= KGE_NEW(math::Sphere)();
		m_pAABB			= KGE_NEW(math::AABB)();
		m_pVertexDec	= rn->GetVertexDec(gfx::EVT_P0NT1BT2I3);
		m_eNodeType		= ENT_Mesh;

	} // Constructor

	//----------------------------------------------------------------------------------------
	// Destructor
	//----------------------------------------------------------------------------------------
	Mesh::~Mesh()
	{
		if (m_NumVerts > 0)
			KGE_DELETE_ARRAY(m_pVerts);
		KGE_DELETE_ARRAY(m_pPos);
		KGE_DELETE_ARRAY(m_pNorTex);
		KGE_DELETE_ARRAY(m_pNorTexColor);
		KGE_DELETE_ARRAY(m_pIndices);
		KGE_DELETE_ARRAY(m_pIndices32);
		KGE_DELETE(m_pBB, Sphere);
		KGE_DELETE(m_pIndexBuffer, HardwareBuffer);
		KGE_DELETE(m_pvbPosition, HardwareBuffer);
		KGE_DELETE(m_pvbNormalTexcoord, HardwareBuffer);
		KGE_DELETE(m_pvbTangentBinormal, HardwareBuffer);
		KGE_DELETE_ARRAY(m_Bone);
		KGE_DELETE(m_pAABB, AABB);
		KGE_DELETE_ARRAY(m_pIndices);

	} // Destructor

	//----------------------------------------------------------------------------------------
	// Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	//----------------------------------------------------------------------------------------
	void Mesh::PreRender(float elapsedTime)
	{
	} // PreRender

	//----------------------------------------------------------------------------------------
	// Render the mesh.
	//----------------------------------------------------------------------------------------
	void Mesh::Render()
	{
		Render(true, true, true);

	} // Render

	//----------------------------------------------------------------------------------------
	// Render with options
	//----------------------------------------------------------------------------------------
	void Mesh::Render( bool bPosition, bool bNormalTexcoord, bool bTangentBinormal )
	{
		m_pRenderer->SetVertexDec(m_pVertexDec);
		if (bPosition)
			m_pRenderer->SetVertexBuffer(m_pvbPosition);
		if (bNormalTexcoord)
			m_pRenderer->SetVertexBuffer(m_pvbNormalTexcoord, 1);
		if (bTangentBinormal)
			m_pRenderer->SetVertexBuffer(m_pvbTangentBinormal, 2);
		if (m_NumIndices > 0)
			m_pRenderer->SetIndexBuffer(m_pIndexBuffer);

		m_pRenderer->DrawTriangleList(m_NumVerts, m_NumIndices, m_pVertexDec, 0, 0);

	} // Render

	//--------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//--------------------------------------------
	void Mesh::PostRender()
	{
		// Searching for effects to enable them if needed.
		if (m_vEffectType.size() > 0)
			for (u32 i = 0; i < m_vEffectType.size(); i++)
			{
				switch (m_vEffectType[i])
				{
					// Mirror Plane.
				case efx::EET_MirrorPlane:
					
					break;

				} // switch (*it)

			} // for it

	} // Post Render

	//--------------------------------
	// Ezafe kardane effect be mesh.
	//--------------------------------
	bool Mesh::AddEffect ( efx::EffectType p , efx::Effect** ppOut )
	{
		return false;

	} // AddEffect

	//---------------------
	// Create Mirror Plane.
	//---------------------
	void Mesh::CreateMirrorPlane(efx::Effect** ppCreatedMirror)
	{
		math::Vector v0 = m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( v0 , v1 , v2 );

		*ppCreatedMirror = new efx::MirrorPlane( m_pRenderer , this );
		((efx::MirrorPlane*)(*ppCreatedMirror))->Init( MeshPlane.GetReflectMatrix() );

		m_vEffect.push_back(*ppCreatedMirror);
		
	} // CreateMirrorPlane

	//---------------------
	// Create Shadow Plane.
	//---------------------
	void Mesh::CreateShadowPlane(efx::Effect** ppCreatedShadow, math::Vector &vLightDir)
	{
		math::Vector v0 = m_pVerts[0].pos.ToVector();
		math::Vector v1 = m_pVerts[1].pos.ToVector();
		math::Vector v2 = m_pVerts[2].pos.ToVector();

		math::Plane MeshPlane;
		MeshPlane.Set( v0 , v1 , v2 );

		*ppCreatedShadow = new kge::efx::ShadowPlane( m_pRenderer , this );
		((efx::ShadowPlane*)(*ppCreatedShadow))->Init( MeshPlane.GetShadowMatrix(vLightDir) );

		m_vEffect.push_back(*ppCreatedShadow);
		
	} // CreateShadowPlane

	void Mesh::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{
		Render(bPosition, bNormalTexcoord, bTangentBinormal);

	} // draw

	//------------------------------------------------------------------------------------
	// Prepare the buffers for multi stream
	//------------------------------------------------------------------------------------
	void Mesh::PrepareBuffers( bool CreateTangents, bool Dynamic)
	{
		if (m_pPos)
		{
			PrepareBuffers2(CreateTangents, Dynamic);
			return;
		}
		else if (!m_pVerts)
			return;
		if (!m_pvbTangentBinormal && CreateTangents)
			CreateTangentBinormalBuffers(Dynamic);
		if (m_pvbPosition)
			return;

		gfx::Vertex3* pos = KGE_NEW_ARRAY(gfx::Vertex3, m_NumVerts);
		gfx::Vertex3T* NorTex = KGE_NEW_ARRAY(gfx::Vertex3T, m_NumVerts);
		for (unsigned int i = 0; i < m_NumVerts; i++)
		{
			pos[i] = m_pVerts[i].pos;
			NorTex[i].pos = m_pVerts[i].Nor;
			NorTex[i].tex = m_pVerts[i].tex;
		}
		m_pvbPosition = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer(pos, m_NumVerts, 12, Dynamic);
		KGE_DELETE_ARRAY(pos);

		m_pvbNormalTexcoord = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer(NorTex, m_NumVerts, 20, Dynamic);
		KGE_DELETE_ARRAY(NorTex);

		if (!m_pIndexBuffer)
		{
			if (m_pIndices)
				m_pIndexBuffer = gfx::Renderer::GetSingletonPtr()->CreateIndexBuffer(m_pIndices, m_NumIndices);
			if (m_pIndices32)
				m_pIndexBuffer = gfx::Renderer::GetSingletonPtr()->CreateIndexBuffer(m_pIndices, m_NumIndices, gfx::EIBT_32Bit);
		}

	} // PrepareBuffers

	//------------------------------------------------------------------------------------
	// Create tangent and binormal buffers
	//------------------------------------------------------------------------------------
	void Mesh::CreateTangentBinormalBuffers( bool Dynamic )
	{
		if (!m_pTangentBinormal)
			m_pTangentBinormal = KGE_NEW_ARRAY(gfx::Vertex3, m_NumVerts * 2);
		if (m_pIndices)
			gfx::CalcBinormalsTangents<gfx::Vertex3TN>
				(m_pVerts, m_pTangentBinormal, m_NumVerts, m_pIndices, m_NumIndices);
		if (m_pIndices32)
			gfx::CalcBinormalsTangents32<gfx::Vertex3TN>
			(m_pVerts, m_pTangentBinormal, m_NumVerts, m_pIndices32, m_NumIndices);
		m_pvbTangentBinormal = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer
			(m_pTangentBinormal, m_NumVerts, 24, Dynamic);

	} // CreateTangentBinormalBuffers

	//------------------------------------------------------------------------------------
	// Prepare the buffers for multi stream
	//------------------------------------------------------------------------------------
	void Mesh::PrepareBuffers2( bool CreateTangents, bool Dynamic )
	{
		if (!m_pvbTangentBinormal && CreateTangents)
			CreateTangentBinormalBuffers2(Dynamic);
		
		KGE_DELETE(m_pvbPosition, HardwareBuffer);
		KGE_DELETE(m_pvbNormalTexcoord, HardwareBuffer);

		m_pvbPosition = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer(m_pPos, m_NumVerts, 12, Dynamic);

		if (m_pNorTex)
		{
			m_pvbNormalTexcoord = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer(m_pNorTex, m_NumVerts, 20, Dynamic);
		}
		else if (m_pNorTexColor)
		{
			m_pvbNormalTexcoord = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer(m_pNorTexColor, m_NumVerts, sizeof(gfx::VertexNTC), Dynamic);
		}

		if (!m_pIndexBuffer)
		{
			if (m_pIndices)
				m_pIndexBuffer = gfx::Renderer::GetSingletonPtr()->CreateIndexBuffer(m_pIndices, m_NumIndices);
			if (m_pIndices32)
				m_pIndexBuffer = gfx::Renderer::GetSingletonPtr()->CreateIndexBuffer(m_pIndices32, m_NumIndices, gfx::EIBT_32Bit);
		}

	} // PrepareBuffers2

	//------------------------------------------------------------------------------------
	// Create tangent and binormal buffers
	//------------------------------------------------------------------------------------
	void Mesh::CreateTangentBinormalBuffers2( bool Dynamic )
	{
		if (!m_pTangentBinormal)
			m_pTangentBinormal = KGE_NEW_ARRAY(gfx::Vertex3, m_NumVerts * 2);
		if (m_pNorTex)
		{
			if (m_pIndices)
				gfx::CalcBinormalsTangents
					(m_pPos, m_pNorTex, m_pTangentBinormal, m_NumVerts, m_pIndices, m_NumIndices);
			if (m_pIndices32)
				gfx::CalcBinormalsTangents32
					(m_pPos, m_pNorTex, m_pTangentBinormal, m_NumVerts, m_pIndices32, m_NumIndices);
		}
		else if (m_pNorTexColor)
		{
			if (m_pIndices)
				gfx::CalcBinormalsTangents
					(m_pPos, m_pNorTexColor, m_pTangentBinormal, m_NumVerts, m_pIndices, m_NumIndices);
			if (m_pIndices32)
				gfx::CalcBinormalsTangents32
					(m_pPos, m_pNorTexColor, m_pTangentBinormal, m_NumVerts, m_pIndices32, m_NumIndices);
		}
		m_pvbTangentBinormal = gfx::Renderer::GetSingletonPtr()->CreateVertexBuffer
			(m_pTangentBinormal, m_NumVerts, 24, Dynamic);

	} // CreateTangentBinormalBuffers2

	//------------------------------------------------------------------------------------
	// Refills vertex and index buffers
	//------------------------------------------------------------------------------------
	void Mesh::RefillBuffers()
	{
		if (m_pvbPosition && m_pPos)
		{
			m_pvbPosition->SetData(m_pPos, 0, 0, 0);
		}

		if (m_pvbNormalTexcoord)
		{   
			if (m_pNorTex)
			{
				m_pvbNormalTexcoord->SetData(m_pNorTex, 0, 0, 0);
			}
			else if (m_pNorTexColor)
			{
				m_pvbNormalTexcoord->SetData(m_pNorTexColor, 0, 0, 0);
			}
		}

		if (m_pIndexBuffer)
		{
			if (m_pIndices)
			{
				m_pIndexBuffer->SetData(m_pIndices, 0, 0, 0);
			}
			else if (m_pIndices32)
			{
				m_pIndexBuffer->SetData(m_pIndices32, 0, 0, 0);
			}
		}

	} // RefillBuffers

} // sn

} // kge
