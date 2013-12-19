// File name: Bone.h
// Des: This class has information about Joints and sets the .
// Date: 1/2/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef BONE_H
#define BONE_H

#include "SceneNode.h"
#include "../Device.h"
#include "../gfx/Structs.h"
#include "../math/Matrix.h"
#include "../core/String.h"

namespace kge
{
namespace sn
{
class KGE_API Bone: public SceneNode
{
	//friend class gfx::MS3DLoader;
public:

	//! Constructor.
	Bone(): m_bAddedToSnmgr(false)
	{
		m_iParent = -1;
	}

	//! Destructor.
	~Bone()
	{
	}

	//! Adds a node as child.
	void AddChild(SceneNode* child)
	{
		SceneNode::AddChild(child);
		if (!m_bAddedToSnmgr)
		{
			m_bAddedToSnmgr = true;
			m_eNodeType = ENT_Camera;
			Device::GetSingletonPtr()->GetSceneManager()->AddSceneNode(this, ENT_Camera);
			this->AddRef();
		}
	}

	//! PrePare the final matrix
	void PreRender(float elapsedTime)
	{
		*m_pFinalMat = *m_pParent->GetFinalMatrix() * m_matFinal;
	}

	//! Update final matrix
	virtual void UpdateFinalMat()
	{
		*m_pFinalMat = *m_pParent->GetFinalMatrix() * m_matFinal;
		for (size_t i = 0; i < m_vChilds.size(); i++)
		{
			m_vChilds[i]->UpdateFinalMat();
		}
	}

	//! Nothing to do
	void Render() {};

	//! Nothing to do
	void PostRender() {}

	//! Nothing to do
	void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal ) {}

	//! Set a name for this bone.
	void SetName(const char* Name)
	{
		m_sName = Name;
	}

	//! Setting the start position.
	void SetStartPosition(float* Pos)
	{
		m_vStartPosition.X = Pos[0];
		m_vStartPosition.Y = Pos[1];
		m_vStartPosition.Z = Pos[2];
	}

	//! Setting the start rotation.
	void SetStartRotation(float* Rot)
	{
//		m_vStartRotation = Rot;
	}

	//! Setting the parent index. Set -1 for no parent.
	void SetParent(int ParentIndex)
	{
		m_iParent = ParentIndex;
	}

	//! Sets the node parent.
	void SetParent(SceneNode* parent)
	{
		m_pParent = parent;
	}

//protected:

	core::String m_sName;					//! Bone name.

	gfx::Vertex3 m_vStartPosition,			//! The start position.
				 m_vStartRotation;			//! The start rotation.

	math::Matrix m_matLocal,				//! The Local matrix for transforms.
				 m_matAbs,					//! The Absolute matrix = Parent.m_matAbs * m_matLocal.
				 m_matFinal;				//! The Final matrix created from animations.

	int			 m_iParent;					//! The Parent index. -1 means has no parent.

protected:

	bool        m_bAddedToSnmgr;

}; // Bone

} // sn

} // kge

#endif //  BONE_H