// File name: CmTree.h
// Des: The tree custom Material.
// Date: 24/6/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_CMTREE_H
#define KGE_CMTREE_H

#include "CustomMaterial.h"
#include "Shader.h"

namespace kge
{
namespace gfx
{
//! Custom material for trees	
class KGE_API CMatTree : public CustomMaterial
{
public:

	//! Constructor
	CMatTree(): m_shVSgTree(NULL), m_fVar(0.0251), m_fTime(0.0),
		m_iLastFrameNo(0)
		{m_eType = ECMT_Tree;}

	//! Destructor
	~CMatTree() {}

	//! Gets the singleton pointer
	static CMatTree* GetSingletonPtr();

	//! Adds custom vertex transform for trees
	bool CreateVertexShaderCode(MaterialParams* pMP, VertexShaderEntry ese);

	//! Nothing to do for pixel shader
	bool CreatePixelShaderCode(MaterialParams* pMP, PixelShaderEntry ese);

	//! Set gTree
	void SetShaderConstants( ShaderInstance* pSI );

	//! Get a copy
	CustomMaterial* GetCopy();

	float m_fVar;

protected:

	ShaderHandle	m_shVSgTree;
	float			m_fgTree[2],
					m_fTime;
	int				m_iLastFrameNo;

}; // CMatTree

} // gfx

} // kge

#endif // KGE_CMTREE_H
