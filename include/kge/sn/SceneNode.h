// File name: SceneNode.h
// Des: In class SceneNode kheili mohem hast baghie tozihat ro ghabl az tarife class
//		bekhonid.
// Date: 3/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef SCENENODE_H
#define SCENENODE_H

#include <vector>
#include "../efx/Effect.h"
#include "../math/Matrix.h"
#include "../math/Sphere.h"
#include "../gfx/Material.h"
#include "../math/AABB.h"

namespace kge
{
namespace gfx
{
// ProtoType
class Renderer;

} // gfx

namespace math
{
//	class AABB;

} // math

namespace sn
{

//! The node type
enum NodeType
{
	ENT_Camera,
	ENT_Light,
	ENT_Mesh,
	ENT_StaticMesh,
	ENT_AnimatedMesh,
	ENT_TileTerrain,
	ENT_TileTerrain2,
	ENT_Decal,
	ENT_ParticleSystem,
	ENT_AnimatedTexture,
	ENT_RibbonTrail,
	ENT_Listener,
	ENT_Sound,
	ENT_UnKnown

}; // Node Type

class SceneManager;

//! In class chizi bishtar az ye Interface nist vali baes mishe ke khili rahat masalan
//! ye tofang ro be daste ye Sarbaz bedid.
class KGE_API SceneNode: public KgeUnknown
{
	friend class SceneManager;
public:

	//! Constructor
	SceneNode();

	//! Destructor
	virtual ~SceneNode();

	//! Adds a node as child.
	virtual void AddChild(SceneNode* child);

	//! The jobs before rendering like culling and calculate position
	virtual void PreRender(float elapsedTime = 0.0f) = 0;

	//! Render the objects
	virtual void Render() = 0;

	//! The jobs after rendering
	virtual void PostRender() = 0;

	/*! Draw the node with/without its Materials and Transforms.
	\param WithMaterial Draw it with its own materials or not?
	\param WithMaterial Draw it with its own Transformations or not?
	*/
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition = true, bool bNormalTexcoord = true, bool bTangentBinormal = true ) = 0;

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut ) { return false; }


	//! Sets the scene manager and renderer pointer.
	virtual void SetSceneManager(SceneManager* smgr);

	//! Set the position of the scenenode.
	virtual void SetPosition(const math::Vector& v);

	//! Set the rotation of the scenenode.
	virtual void SetRotation(const math::Vector& v);

	//! Set the scale of the scenenode.
	virtual void SetScale(const math::Vector& v);

	//! Set Cutting Enable or Disable
	virtual void SetAutomaticCulling( bool isTrue ) { m_bAutoCulling = isTrue; }

	//! Set Visible Enable or Disable
	virtual void SetVisible( bool isVisible );

	//! Get Visible
	virtual bool GetVisible() { return m_bVis; }

	//! Is it visible in this frame?
	virtual bool IsVisibleInFrame() { return m_bVisFrame; }

	//! Returns the node position.
	virtual const math::Vector& GetPosition() const;

	//! Returns the node rotation.
	virtual const math::Vector& GetRotation() const;

	//! Returns the node scale.
	virtual const math::Vector& GetScale() const;
	
	//! Get the absolute matrix.
	virtual math::Matrix* GetAbsoluteMatrix() const;

	//! Get the final matrix.
	virtual math::Matrix* GetFinalMatrix() { return m_pFinalMat; }

	//! Get the count of materials this node has.
	virtual u32 GetMaterialCount();

	/*! Get the node material by its index.
	\param Index The material index. Some nodes have more than one material.
	*/
	virtual gfx::Material* GetMaterial(u32 Index);

	//! Returns the node bounding box.
	virtual math::Sphere* GetBoundingBox() { return m_pBB; }

	//! Returns the node Axis aligned bounding box.
	virtual math::AABB* GetAxisAlignedBoundingBox() { return m_pAABB; }

	// TODO: Every scene nodes must declare this function.
	//! Returns the node type.
	virtual NodeType GetType() const { return m_eNodeType; }

	/*! Removes a child from this node.
		\param child The pointer to the child.
		\param Delete If pass true it will be deleted from memory.
	 */
	virtual void RemoveChild(SceneNode* child);

	//! Removes all children of this node.
	virtual void RemoveChildren();

	//! Returns the node parent.
	virtual const SceneNode* GetParent() { return m_pParent; }

	//! Sets the node parent.
	virtual void SetParent(SceneNode* parent);

	//! Sets the node name
	virtual void SetName(const char* name)
	{
		m_sName = name;
	}

	//! Gets the node name
	virtual core::String* GetName() 
	{ 
		return &m_sName; 
	}

	//! Sets the node ID
	virtual void SetID(int ID) 
	{
		m_iID = ID;
	}

	//! Gets the node ID
	virtual int GetID()
	{
		return m_iID;
	}

	//!
	void EnableShadow(bool enable);

	//!
	bool HasShadow() const { return m_hasShadow; }

	//! Update final matrix
	virtual void UpdateFinalMat() {}

protected:

	core::String		m_sName;				// Node name
	int					m_iID;					// Node ID
	SceneManager *   m_pSnMan;					// SceneManager pointer.
	gfx::Renderer*   m_pRenderer;				// Renderer pointer.
	bool		     m_bVis,					// Visible.
					 m_bAutoCulling,			// Automatic culling.
					 m_bVisFrame;				// Is it visible in this frame?

	SceneNode	 *   m_pParent;					// The node parent.
	std::vector<
		SceneNode*>	 m_vChilds;					// The node childs.
	std::vector<int> m_vEffectType;	    	    // Type effect haye haye Object.
	std::vector<efx
		::Effect*>   m_vEffect;				    // Effect haye Object.
	math::Matrix*	 m_pAbsMat,					// The absolute transform matrix.
				*	 m_pFinalMat;				// The final transform matrix.
	math::Vector	 m_Position,				// The node position.
					 m_Rotation,				// The node rotation.
					 m_Scale;					// The node scale.
	math::Sphere*	 m_pBB;						// The node bounding box.
	math::AABB*		 m_pAABB;					// The node Axis Aligned bounding box.
	NodeType			m_eNodeType;			// The node type
	bool                m_hasShadow;

	// Check if this node already has this effect or not.
	virtual bool HasThisEffect ( efx::EffectType p );

}; // SceneNode

} // sn

} // kge

#endif // SCENENODE_H
