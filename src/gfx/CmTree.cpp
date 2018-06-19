// File name: CmTree.cpp
// Des: The tree custom Material.
// Date: 24/6/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/gfx/CmTree.h"
#include "../../include/kge/gfx/MaterialParams.h"

extern float g_fElaspedTime;
extern int   g_iFrameNumber;

namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Gets the singleton pointer
	//------------------------------------------------------------------------------------
	CMatTree* CMatTree::GetSingletonPtr()
	{
		static CMatTree Obj;

		return &Obj;

	} // GetSingletonPtr

	//------------------------------------------------------------------------------------
	// Adds custom vertex transform for trees
	//------------------------------------------------------------------------------------
	bool CMatTree::CreateVertexShaderCode( MaterialParams* pMP, VertexShaderEntry ese )
	{
		switch (ese)
		{
		case EVSE_ChangePosition:
			pMP->m_sVsMain += 
				"inPosition.x += gTree.x * inPosition.y;\n"\
				"inPosition.z += gTree.y * inPosition.y;\n";
			pMP->m_sVsHeader += "float2 gTree;\n";
			break;

		case EVSE_GetVertexShaderHandles:
			m_shVSgTree = pMP->pVertexShader->GetConstatnt("gTree");
			break;
		}

		return false;

	} // CreateVertexShaderCode

	//------------------------------------------------------------------------------------
	// Nothing to do for pixel shader
	//------------------------------------------------------------------------------------
	bool CMatTree::CreatePixelShaderCode( MaterialParams* pMP, PixelShaderEntry ese )
	{
		return false;

	} // CreatePixelShaderCode

	//------------------------------------------------------------------------------------
	// Set gTree
	//------------------------------------------------------------------------------------
	void CMatTree::SetShaderConstants( ShaderInstance* pSI )
	{
		if (m_iLastFrameNo != g_iFrameNumber)
		{
			m_fTime += g_fElaspedTime;
			m_iLastFrameNo = g_iFrameNumber;
		}
		if (m_shVSgTree)
		{
			float cosTime = cosf(m_fTime);
			m_fgTree[0] = cosTime * m_fVar * cosTime;
			m_fgTree[1] = sinf(m_fTime) * m_fVar * cosTime;
			pSI->m_pVertexShader->SetConstant(m_shVSgTree, m_fgTree, 2);
		}

	} // SetShaderConstants

	//------------------------------------------------------------------------------------
	// Get a copy
	//------------------------------------------------------------------------------------
	CustomMaterial* CMatTree::GetCopy()
	{
		CMatTree* p = KGE_NEW(CMatTree)();
		p->m_fVar   = m_fVar;

		return p;

	} // GetCopy

} // gfx

} // kge