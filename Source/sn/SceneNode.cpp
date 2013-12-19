// File name: SceneNode.cpp
// Des: In class SceneNode kheili mohem hast baghie tozihat ro ghabl az tarife class
//		bekhonid.
// Date: 17/10/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/sn/SceneNode.h"
#include "../../Include/sn/SceneManager.h"
#include "../../Include/gfx/Renderer.h"

namespace kge
{
namespace sn
{
	//--------------------------------------------------------------------------------------------
	// Constructor
	//--------------------------------------------------------------------------------------------
	SceneNode::SceneNode(): m_pSnMan(NULL), m_bVis(true), m_bAutoCulling(true),
		m_pParent(NULL), m_pBB(NULL), m_pAABB(NULL), m_eNodeType(ENT_UnKnown), m_bVisFrame(false)
		, m_hasShadow(true), m_Rotation(0.0f, 0.0f, 0.0f), m_Scale(1.0f, 1.0f, 1.0f)
	{
		m_pAbsMat   = KGE_NEW(kge::math::Matrix);
		m_pFinalMat = KGE_NEW(kge::math::Matrix);
		m_pRenderer = gfx::Renderer::GetSingletonPtr();
		m_pSnMan    = Device::GetSingletonPtr()->GetSceneManager();

	} // Constructor

	//--------------------------------------------------------------------------------------------
	// Destructor
	//--------------------------------------------------------------------------------------------
	SceneNode::~SceneNode()
	{
		for (std::vector<SceneNode*>::iterator it = m_vChilds.begin();
			it != m_vChilds.end(); ++it)
		{
			while ((*it)->GetRefCount() > 1)
			{
				(*it)->DecRef();
			}
			m_pSnMan->RemoveSceneNode((*it), false);
		}

		KGE_DELETE(m_pAbsMat, Matrix);
		KGE_DELETE(m_pFinalMat, Matrix);

	}// Destructor

	//--------------------------------------------------------------------------------------------
	// Adds a node as child.
	//--------------------------------------------------------------------------------------------
	void SceneNode::AddChild(SceneNode* child)
	{
		m_vChilds.push_back(child);
		child->m_pParent = this;
		child->AddRef();

	} // AddChild

	//--------------------------------------------------------------------------------------------
	// Check if this node already has this effect or not.
	//--------------------------------------------------------------------------------------------
	bool SceneNode::HasThisEffect( efx::EffectType p )
	{
		if ( m_vEffectType.size() > 0 )
		{
			for ( std::vector<int>::iterator it = m_vEffectType.begin();
				  it != m_vEffectType.end(); ++it)
			{
				if ( (*it) == p )
					return true;

			} // for

		} // if

		return false;

	} // Has This Effect

	//---------------------------------------------
	// Sets the scene manager and renderer pointer.
	//---------------------------------------------
	void SceneNode::SetSceneManager(SceneManager* smgr)
	{
		m_pSnMan	= smgr;
		m_pRenderer = smgr->GetRenderer();

	} // SetSceneManager

	//-----------------------------------
	// Set the position of the scenenode.
	//-----------------------------------
	void SceneNode::SetPosition(const math::Vector& v)
	{
		m_pAbsMat->SetTranslation(&v.v[0]);
		m_Position = v;

	} // SetPosition

	//-----------------------------------
	// Set the rotation of the scenenode.
	//-----------------------------------
	void SceneNode::SetRotation(const math::Vector& v)
	{
		if (m_Rotation == v)
			return;

		m_pAbsMat->SetRotation(&v.v[0]);
		m_Rotation = v;
		if (m_Scale != math::Vector(1,1,1))
		{
			math::Matrix smat;
			smat.SetScale(m_Scale);
			*m_pAbsMat *= smat;
		}
		
	} // SetRotation

	//--------------------------------
	// Set the scale of the scenenode.
	//--------------------------------
	void SceneNode::SetScale(const math::Vector& v)
	{
		if (m_Scale == v)
			return;

		if (m_Rotation != math::Vector(0,0,0) && m_Scale != math::Vector(1,1,1))
		{
			math::Matrix smat;
			smat.SetScale(v);
			m_pAbsMat->SetRotation(&m_Rotation.v[0]);
			*m_pAbsMat *= smat;
		}
		else if ( m_Rotation != math::Vector(0,0,0) )
		{
			math::Matrix smat;
			smat.SetScale(v);
			*m_pAbsMat *= smat;
		}
		else
			m_pAbsMat->SetScale(v);

		m_Scale = v;

	} // SetScale

	//-------------------------
	// Get the absolute matrix.
	//-------------------------
	math::Matrix* SceneNode::GetAbsoluteMatrix() const
	{
		return m_pAbsMat;

	} // GetAbsoluteMatrix

	//---------------------------
	// Returns the node position.
	//---------------------------
	const math::Vector& SceneNode::GetPosition() const
	{
		return m_Position;

	} // GetPosition

	//---------------------------
	// Returns the node rotation.
	//---------------------------
	const math::Vector& SceneNode::GetRotation() const
	{
		return m_Rotation;

	} // GetRotation

	//------------------------
	// Returns the node scale.
	//------------------------
	const math::Vector& SceneNode::GetScale() const
	{
		return m_Scale;

	} // GetScale

	//------------------------------------------
	// Get the count of materials this node has.
	//------------------------------------------
	u32 SceneNode::GetMaterialCount()
	{
		return 0;

	} // GetMaterialCount

	//------------------------------------
	// Get the node material by its index.
	//------------------------------------
	gfx::Material* SceneNode::GetMaterial(u32 Index)
	{
		return NULL;

	} // GetMaterial

	//--------------------------------------------------------------------------------------------
	// Removes a child from this node.
	//--------------------------------------------------------------------------------------------
	void SceneNode::RemoveChild(SceneNode* child)
	{
		for(std::vector<SceneNode*>::iterator it = m_vChilds.begin(); 
			it != m_vChilds.end(); ++it)
		{
			if ((*it) == child)
			{
				m_vChilds.erase(it);
				child->DecRef();
				return;
			}
		}

	} // RemoveChild

	//--------------------------------------------------------------------------------------------
	// Removes all children of this node.
	//--------------------------------------------------------------------------------------------
	void SceneNode::RemoveChildren()
	{
		for (std::vector<SceneNode*>::iterator it = m_vChilds.begin();
			it != m_vChilds.end(); ++it)
		{
			while ((*it)->GetRefCount() > 1)
			{
				(*it)->DecRef();
			}
			m_pSnMan->RemoveSceneNode((*it), false);
		}

		m_vChilds.clear();

	} // RemoveChildren

	//--------------------------------------------------------------------------------------------
	// Sets the node parent.
	//--------------------------------------------------------------------------------------------
	void SceneNode::SetParent(SceneNode* parent)
	{
		if (m_pParent)
		{
			m_pParent->RemoveChild(this);
		}

		if (parent)
		{
			parent->AddChild(this);
		}

		m_pParent = parent;

	} // SetParent

	//--------------------------------------------------------------------------------------------
	// 
	//--------------------------------------------------------------------------------------------
	void SceneNode::EnableShadow(bool enable)
	{
		m_hasShadow = enable;

		for (std::vector<SceneNode*>::iterator it = m_vChilds.begin();
			it != m_vChilds.end(); ++it)
		{
			(*it)->EnableShadow(enable);
		}
	}

	 //--------------------------------------------------------------------------------------------
	 // 
	 //--------------------------------------------------------------------------------------------
	 void SceneNode::SetVisible(bool isVisible)
	 {
		 m_bVis = isVisible; 

		 for (std::vector<SceneNode*>::iterator it = m_vChilds.begin();
			 it != m_vChilds.end(); ++it)
		 {
			(*it)->SetVisible(isVisible);
		 }

	 }

} // sn

} // kge