// File name: SceneManager.h
// Des: In class vazife dare ke tamame karhaye marbot be modiriate sahne ro anjam bede.
// Date: 3/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include "../core/PriorityArray.h"
#include "../core/Timer.h"
#include "SceneNode.h"
#include "Camera.h"
#include "AnimatedTexture.h"
#include "ParticleSystem.h"
#include "Sound3D.h"
#include "../kgedef.h"
#include "../ResourceManager.h"
#include "../gfx/Structs.h"
#include "../gfx/Texture.h"
#include "../efx/Effect.h"

namespace kge
{

namespace gfx
{

//! Noe imagei ke mikhahid load shavad.
enum ImageType
{
	E_IT_UnKnown,	//! Agar ke mikhahid khode engine format tasvir ra peyda konad in meghdar ra benevisid(engine az roye pasvand format tasvir ra moshakhas mikonad.
	E_IT_BMP,		//! BMP Image
	E_IT_JPEG		//! JPEG Image
}; // ImageType

//enum ShaderVersion;

class MeshBuffer;
class RendererDX;
class GeometryBatch;

} // gfx

//! Scene Classes.
namespace sn
{
//! Tartibe Render shodan.
enum RenderOrder
{
	ERO_First,
	ERO_Middle,
	ERO_Last,
	ERO_Decal

}; // RenderOrder

//---------------------------
//   P R O T O   T Y P E S
//---------------------------
class AnimatedMesh;
class StaticMesh;
class Light;
class Mesh;
class RibbonTrail;

struct RBuffer
{
	gfx::HardwareBuffer* hb;
	size_t size;

	explicit RBuffer(gfx::HardwareBuffer* hb, size_t size)
		: hb(hb), size(size) {}
};

//! Modiriat konande sahne ha.
class KGE_API SceneManager
{
	friend class kge::gfx::RendererDX;
public:

	//! Sazande class SceneManager
	SceneManager(gfx::Renderer* pRenderer);

	//! Destrctor.
	~SceneManager();

	//! Ezafe kardane ye Node Jadid. Tamame in Node ha parent hastand.
	void AddSceneNode(SceneNode* pSnNode);

	//! Ezafe kardane ye Node Jadid. Tamame in Node ha parent hastand.
	void AddSceneNode(SceneNode* pSnNode , NodeType type);

	//! Vaghti ke hame karhaye ghabl az render anjam shod va render shodan ghati shod
	//! ba in dastor mire baraye render.
	/*!
	  \param pSnNode Node ie ke gharar hast render beshe.
	  \param ro Tartib render ke che moghei render beshe.
	 */
	void RegNode(SceneNode* pSnNode, RenderOrder ro = ERO_Middle);

	//! Removes the scene node from scene manager
	void RemoveSceneNode(SceneNode* pSnNode, bool setParentNull = true);

	//! Render kardane hame chi.
	bool RenderAll(float elapsedTime);

	//! Ezafe kardane ye dorbin be sahne.
	/*!
	  \param Position Makanne dorbin.
	  \param LookAt Be jaye ke dorbin negah mikone.
	  \param UpDirection Jahat bala dar faza.
	  \return Ye pointer baraye Camera barmigardone.
	 */
	Camera* AddCameraNode(const math::Vector &Position, const math::Vector &LookAt,
		const math::Vector &UpDirection = math::Vector(0.0f, 1.0f, 0.0f));

	//!
	//Camera* AddCameraNode(float distance, float theta, float phi,
	//    const math::Vector& target, const math::Vector &upDirection = math::Vector(0.0f, 1.0f, 0.0f));

	//!
	//Camera* AddCameraNode(const math::Vector &position, const math::Vector &target,
	//    const math::Vector &upDirection = math::Vector(0.0f, 1.0f, 0.0f));

	/*! Loads an animated mesh from file the supported formats are:
	MS3D(Milk Shape 3D).
		\param FileName Esme va mahal file .ms3d
		\return Ye pointer be class sakhte shodeye MeshMS3D bar migardone.
	 */
	AnimatedMesh* AddAnimatedMeshNode(const char* FileName);

	/*! Loads a static mesh from file the supported formats are:
	MS3D(Milk Shape 3D).
		\param FileName Esme va mahal file .ms3d
		\return Ye pointer be class sakhte shodeye MeshMS3D bar migardone.
	 */
	StaticMesh* AddStaticMeshNode(const char* FileName, bool enableLighting = true);

	//!
	ParticleSystem* AddParticleSystemNode(const ParticleSystemParameters& p);

	//!
	AnimatedTexture* AddAnimatedTextureNode(const AnimatedTextureParameters& p);

	//!
	RibbonTrail* AddRibbonTrail(
		const char* shaderName,
		const char* textureName,
		const math::Vector& textureRect,
		const gfx::Colorf& initialColor,
		const gfx::Colorf& finalColor,
		float width,
		float speed,
		float age,
		float chaosRatio);

	//! Ye nore jahat daro dar sahne ezafe mikone. Mesle nore Khorshid.
	/*!
		\param vDir Jahate nor.
		\param color Range nor.
		\return Yek LightDir barmigardone ba moshakhasate sakhte shode ke ba taghiresh mishe dobare ba SetLight ono ezafe kard.
	*/
	virtual Light* AddDirectionalLight(math::Vector vDir,
									   const gfx::Colorf& color);

	//! Ye nore noghte i ro be sahne ezafe mikone ke noresh dar tamame jahat pakhsh mishe.
	/*!
		\param Position Mahale gharar giri nor.
		\param Color Range nor.
		\return Yek Nore noghte e ba moshakhasate sakhte shode bar migardone ke mishe ba taghiresh va ferestadane dobareye on be SetLight nore delkhah ro bedast avard.
	 */
	virtual sn::Light* AddPointLight(const math::Vector& vPosition, const gfx::Colorf& color);

	//! Ye nore spot ro be sahne ezafe mikone mesle nore cheragh ghove.
	/*!
		\param Position Mahale gharar giri nor.
		\param Direction Jahate nor ra moshakhas mikone.
		\param Color Range nor.
		\return Yek Nore Spot ra ba moshakhasate sakhte shode bar migardone ke mishe ba taghiresh va ferestadane dobareye on be SetLight nore delkhah ro bedast avard.
	 */
	virtual sn::Light* AddSpotLight(const math::Vector& vPosition, const math::Vector& vDirection,
									const gfx::Colorf& color);

	/*! Ezafe kardane ye texture.
		\param Filename Esme tasvir ke bayad load shavad.
		\return ID texture ra barmigardanad.
	*/
	gfx::Texture* AddTexture(const char* FileName);

	/*! Load a texture
		\param Filename Texture file name to load
		\param Name The texture name this parameter is optional but if you want to texture manager load a texture more than one time take it different names.
		\param ct This is for creating compressed textures if you don't want use compressed textures pass kge::gfx::ECT_None
		\return return the created texture.
	*/
	gfx::Texture* AddTexture(const char* FileName, const char* Name , gfx::CompressType ct = gfx::ECT_DXT5);

	/*! Add a vertex shader
	*/
	gfx::Shader* AddVertexShader(const char* fileName, const char* vertexMain, int version);

	/*! Add a pixel shader
	*/
	gfx::Shader* AddPixelShader(const char* fileName, const char* pixelMain, int version);

	/*!
	*/
	void AddEffect( efx::Effect* pEffect, efx::EffectType Type );

	//! ezafe kardane ye Sound3D be sahne
	sn::Sound3D* AddSound3D(const char* FileName);

	//! Returns the renderer pointer.
	gfx::Renderer* GetRenderer()
	{
		return m_pRenderer;

	} // getRenderer

	/*! Get the nearest light scene node from the given position.
		\param Where The position to find the nearest light.
		\return Returns the nearest light scene node.
		Note: If there is any directional Light it will be returned any time.
	*/
	sn::Light* GetNearestLight(const math::Vector& Where);

	/*! Get the nearest lights scene node from the given position.
	\param Where The position to find the nearest light.
	\return Returns the nearest light scene node.
	Note: If there is any directional Light it will be returned any time.
	*/
	const core::PriorityArray<sn::Light*, float>& GetNearestLight(const math::Vector& Where, int count);

	//! Sets the active camera.
	void SetActiveCamera(Camera* cam)	{ m_pActiveCam = cam; if (m_pActiveCam){m_pActiveCam->m_bChanged = true; m_pActiveCam->m_bProjChanged = true;} }

	//! Returns the active camera.
	Camera* GetActiveCamera()			{ return m_pActiveCam; }

	//! Checks a node for its visibility.
	math::CollisionType IsVisible(SceneNode* node);

	const core::PriorityArray<SceneNode*, int>& GetVisibleMeshes() const { return m_vMidNodes; }

	std::vector<SceneNode*> GetVisibleMeshes(Camera* pCam);

	//! For internal use
	void SetStoreShadowPoints(bool bEnable) { m_bStoreShadowPoints = bEnable; }

	//! For internal use
	bool GetStoreShadowPoints() const { return m_bStoreShadowPoints; }

	void Clear();

	bool GetShadowHit() const  { return m_bHit; }

	/// \internal Add a geometry batch for rendering
	void AddGeometryBatch(gfx::GeometryBatch* pGeoBatch);

	std::vector<SceneNode*>* GetShadowCasterNodes()
	{
		return &m_vShadowCasterNodes;
	}
	std::vector<SceneNode*>* GetShadowReciverNodes()
	{
		return &m_vShadowReciverNodes;
	}
	std::vector<math::AABB>* GetShadowCasterPoints()
	{
		return &m_vShadowCasterPoints;
	}
	std::vector<math::AABB>* GetShadowReciverPoints()
	{
		return &m_vShadowReciverPoints;
	}

	//!
	void EnableShadow(bool enableShadow) { m_bEnableShadow = enableShadow; }

	//! Checks the scene node for material necessary changes
	void CheckForMaterialsChanges(SceneNode* pSn);

	//! Returns the GeometryBatch list
	const std::vector<gfx::GeometryBatch*>& GetInstances() { return m_vGeometryBatches; }

	float GetElaspedTime() { return m_fElaspedTime; }

	std::vector<Light*> GetLightsInFrustum(int& DirCount, int& PointCount, int& SpotCount);

	bool IsLightVisible(Light* pLit);

	math::AABB				*	m_pSceneAABB;

	std::vector<SceneNode*> m_vMeshNodes;			//! The Mesh SceneNodes.

	//////////////////////////////////////////////////////////////////////////
	gfx::HardwareBuffer*    GetParticlesSharedBuffer() { return m_particlesSharedBuffer; }
	void                    SetParticlesSharedBuffer(gfx::HardwareBuffer* ib);

	RBuffer*                GetRBuffer(size_t size);
	void                    FreeRBuffer(RBuffer* rbuffer);
	//////////////////////////////////////////////////////////////////////////

//protected:
	// Node hayi ke dar safhe hastand va tavasote SceneManager negahdari mishavand.
	std::vector<SceneNode*> m_vNodes;				//! Tamame Node haye nashenakhte.
	std::vector<SceneNode*> m_vCamNodes;			//! The Camera SceneNodes.
	std::vector<SceneNode*> m_vLightNodes;			//! The Light SceneNodes.
	//std::vector<SceneNode*> m_vMeshNodes;			//! The Mesh SceneNodes.
	std::vector<SceneNode*> m_vAnimTexNodes;       //! The list of animated texture nodes.
	std::vector<SceneNode*> m_vPSysNodes;          //! The list of particle system nodes.

	std::vector<Mesh*>      m_vMeshList;			//! The List of the meshes for catching.

	float					m_fElaspedTime;

	// Scene Node hayi ke bayad dar frame jari render shavand.
	core::PriorityArray<SceneNode*, int>	m_vFirstNodes;			//! This SceneNodes must be render first.
	core::PriorityArray<SceneNode*, int>	m_vMidNodes;			//! This SceneNodes must be render middle.
	core::PriorityArray<SceneNode*, float>	m_vLastNodes;			//! This SceneNodes must be render last.
	core::PriorityArray<SceneNode*, int>	m_vDecalNodes;			//! This SceneNodes must be render last.

	core::PriorityArray<efx::Effect*, efx::EffectType> m_vEffect;	//! Contains pointers to Effect.

	std::vector<efx::EffectType>			m_vEffectType;			//! Contains pointers to EffectType.

	std::vector<gfx::TextureInfo>			m_vTextures;			//! Textures array.
	gfx::Renderer*          m_pRenderer;			//! The Renderer pointer.

	std::vector<gfx::MeshBuffer*>			m_vMeshBuffer;			// Mesh buffer array.

	core::PriorityArray<sn::Light*, float>	m_vLightList;

	Camera*                 m_pActiveCam;			// Active camera

	gfx::MaterialParams     m_DefaultMatParam;		// Default MaterialParams struct

	bool					m_bStoreShadowPoints;	// Stores shadow points or not
	bool					m_bHit;
	std::vector<SceneNode*>	m_vShadowCasterNodes,
							m_vShadowReciverNodes;
	std::vector<math::AABB>	m_vShadowCasterPoints,
							m_vShadowReciverPoints;

	std::vector<gfx::GeometryBatch*>		m_vGeometryBatches;

	//!
	bool                    m_bEnableShadow;

	//! Deferred shading shaders
	gfx::Shader			*	m_pVsDeferred,
						*	m_pPsDeferred,
						*	m_pPsDeferredShadow,
						*	m_pVsSphere,
						*	m_pPsSphere,
						*	m_pPsPointLight;

	gfx::ShaderHandle		m_VsHalfPixel,
							m_VsmatViewProj,
							m_VsmatViewProjInv,
							m_VsEyePos,
							m_PsMatShadow,
							m_PsEyePos,
							m_PsDirLight,
							m_PsDirLightColor,
							m_PsDirLightSpecColor,
							m_PsLightCount,
							m_PsEyePosShadow,
							m_PsDirLightShadow,
							m_PsDirLightColorShadow,
							m_PsDirLightSpecColorShadow,
							m_PsShadowColor,
							m_VshSphereMatWVP,
							m_PshPointEyePos,
							m_PshPointPointLight,
							m_PshPointPointLightColor,
							m_PshPointPointLightSpecColor;

	float				*	m_pfDirLightData,
						*	m_pfDirLightColorData,
						*	m_pfDirLightSpecColorData;
	int						m_iLightCount;

	StaticMesh			*	m_pSphere;

private:
	//!
	gfx::HardwareBuffer*	m_particlesSharedBuffer;
	std::vector<RBuffer*>	m_rbuffers;
	std::vector<RBuffer*>	m_freeRBuffers;
	size_t					m_numFreeBuffers;

}; // SceneManeger

} // sn

} // kge

#endif // SCENEMANAGER_H
