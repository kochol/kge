// File name: AnimatedMesh.h
// Des: This is a node class for animated meshes.
// Date: 25/1/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef ANIMATEDMESH_H
#define ANIMATEDMESH_H

#include "SceneNode.h"
#include "Bone.h"
#include "../gfx/Animation.h"
#include "../core/Timer.h"
#include "../../Include/sn/Mesh.h"

namespace kge
{
namespace sn
{

class KGE_API AnimatedMesh : public SceneNode
{
public:

	//! Constructor.
	AnimatedMesh(gfx::MeshBuffer* mb);

	//! Destructor.
	~AnimatedMesh();

	//! Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	virtual void PreRender(float elapsedTime);

	//! Render kardane Object.
	virtual void Render();

	//! Karhaee ke bad az render bayad anjam beshe.
	virtual void PostRender();

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut );

	/*! Draw the node with/without its Materials and Transforms.
		\param WithMaterial Draw it with its own materials or not?
		\param WithMaterial Draw it with its own Transformations or not?
	 */
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	/*! Set the mesh animation time.
		\param StartTime The starting time for animation.
		\param EndTime   The time which the animation must stop from that time if you want to stop it at the end pass -1 to do it auto.
		\param Loop		 Loop the animation by true.
	*/
	virtual void SetAnimationTime(float StartTime = 0.0f  ,
								  float EndTime   = -1.0f ,
								  bool  Loop	  = true ,
								  bool  Blend     = true);

	/*! Set the mesh animation frame.
		\param StartTime The starting frame for animation.
		\param EndTime   The frame which the animation must stop from that frame if you want to stop it at the end pass -1 to do it auto.
		\param Loop		 Loop the animation by true.
	*/
	virtual void SetAnimationFrame(int  StartTime = 0  ,
								   int  EndTime   = -1 ,
								   bool Loop	  = true,
								   bool Blend     = true);

	//! Sets the action frame
	void SetActionFrame(int ActionFrame);

	//! Returns true if action frame of animation reached. Note: It only returns true one time for each frame
	bool IsActionFrameReached() 
	{
		bool b = m_bActionFrameReached;
		m_bActionFrameReached = false;
		return b;
	}

	/*! Set the animation speed default value is 1.0f
		\param Speed The animation speed.
	*/
	virtual void SetAnimationSpeed(float Speed) { m_fSpeed = Speed; }

	//! Returns the animation speed
	float GetAnimationSpeed() { return m_fSpeed; }

	/*! Set the shader for all materials.
		\param shader Shader to set.
	*/
	virtual void SetShader(gfx::ShaderInstance* shader);

	//! Get the count of materials this node has.
	virtual u32 GetMaterialCount();

	/*! Get the node material by its index.
	\param Index The material index. Some nodes have more than one material.
	\return Returns the requested material.
	*/
	virtual gfx::Material* GetMaterial(u32 Index);

	/*! Get a bone by its name.
	\param Name The bone name.
	\return Returns the requested bone.
	*/
	virtual Bone* GetBoneByName(const char* Name);

	virtual Bone* GetBones() {return m_pBones;}

	virtual u32 GetBonesCount() {return m_iNumBones;}

	/*! Get a pointer to function to call it at the end of animation.
	*/
	virtual void ConnectOnAnimationEnd(kge::core::Functor1<void, void*>* pFun);

	//! Stop from calling user function.
	virtual void DisconnectOnAnimationEnd();

	//! Set your data for sending back them on callbacks.
	virtual void SetUserData(void* data) { m_pUData = data; }

	//! Get your data.
	virtual void* GetUserData() { return m_pUData; }

	//! For internal use
	void SetFileName(core::String* str) { m_pFileName = str; }

	//! Returns the mesh file name
	core::String* GetFileName() { return m_pFileName; }

	//! Sets the global animation speed
	static void SetGlobalAnimationSpeed(float fSpeed) { m_sAnimSpeed = fSpeed; }

	//!
	static float GetGlobalAnimationSpeed() { return m_sAnimSpeed; }

	//!
	void RemoveChildrenFromBones();

protected:

	gfx::MeshBuffer	*	m_pMeshBuffer;	//! Mesh buffer pointer	

	sn::Mesh		*	m_pMeshes;		//! Meshes.

	core::String	*	m_pFileName;	//!*< Mesh file name
	
	Bone			*	m_pBones;		//! Bones.

	gfx::Animation  *	m_pAnimation;	//! Animation.

	u32					m_iNumMeshes,	//! The count of meshes.
						m_iNumBones;	//! The count of bones.

	float			  m_fStartTime,		//!< The start time of animation.
					  m_fActionFrame,	//!< The action time of animation.
					  m_fEndTime,		//!< The end time of animation.
					  m_fLastTime,		//!< The last time of animation.
					  m_fSpeed,			//!< The animation speed.
					  m_fBlendTime;

	bool			  m_bLoop,			//! Loop the animation or not.
					  m_bBlend,
					  m_bActionFrameReached,
					  m_bCheckForActionFrame;
					  
	u32				* m_pTLastFrameData,
					* m_pRLastFrameData,
					* m_pTCurFrameData,
					* m_pRCurFrameData;

	//core::Timer		* m_pTimer;			//! Timer for calculating the animation time.

	gfx::Shader*	  m_pVHSshader,		// Vertex Shader for hardware skinning.
					* m_pPHSshader;		// Pixel Shader for hardware skinning.
	gfx::ShaderHandle m_shBones,	// 
					  m_shViewProj,
					  m_shDiffColor,
					  m_shWorld,
					  m_shDir;
	gfx::ShaderInstance	*	m_pShaderInstance;

	gfx::Material*  m_pMaterials;		//! Mesh materials

	//! Pointer to user function.
	kge::core::Functor1<void, void*>* m_pEndAniFunc; 

	//! User data.
	void* m_pUData;

	//! Global animation speed
	static float    m_sAnimSpeed;

	//! Animates the mesh.
	virtual void Animate(float elapsedTime);	

	//! Renders the mesh depend on their Bones.
	virtual void RenderMesh();

	// Create Mirror Plane
	virtual void CreateMirrorPlane(efx::Effect** ppCreatedMirror);

	// Create Shadow Plane
	virtual void CreateShadowPlane(efx::Effect** ppCreatedShadow);

	// Create hardware skinning effect.
	virtual void CreateHardwareSkin();

	// Set the shader variables for HardwareSkinning.
	virtual void SetShaderConstsForHS(gfx::ShaderInstance* si);

}; // AnimatedMesh

} // sn

} // kge

#endif // ANIMATEDMESH_H