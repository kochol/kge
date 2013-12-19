// File name: ShadowMapProjection.h
// Des: This effect is a shadow technique that is simple and fast without any self shadowing and it is good for shadowing on terrain 
// Date: 16/12/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef SHADOWMAPPROJECTION_H
#define SHADOWMAPPROJECTION_H

#include "Effect.h"
#include "../sn/SceneManager.h"
#include "../core/Structs.h"

namespace kge
{
namespace efx
{

	const float ZFAR_MAX = 800.0f;
	const float ZNEAR_MIN = 1.0f;
	const int TEXDEPTH_HEIGHT_20 = 1536;
	const int TEXDEPTH_WIDTH_20 = 1536;
	const int TEXDEPTH_SIZE_11 = 1024;
	const float W_EPSILON = 0.001f;
	const float SMQUAD_SIZE = 800.0f;
	const int NUM_OBJECTS = 400;
	const float VIEW_ANGLE = 60.f;

class KGE_API ShadowMapProjection: public Effect
{
public:

	//! Constructor
	ShadowMapProjection(int ShadowMapSize, sn::SceneManager* pSmgr);

	//! Destructor
	virtual ~ShadowMapProjection();

	//! Add nodes of mesh that effect influencing on it.
	virtual void AddNodes( sn::SceneNode* pNodes );

	//! The works must to do before rendering the object.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! The works must to do after rendering the object.
	virtual void Render();

	//! The works must to do after rendering all of the scene nodes.
	virtual void PostRenderEverything();

	//! Inits the shadow map
	virtual bool Init();

	virtual const char* GetShaderCode();

	virtual math::Matrix* GetMatrix() {return m_pBiasFinal;}

	//! For internal use
	virtual void OnReset();

	gfx::Texture			*	m_pShadowMap;	//!< The shadow map texture
	gfx::Colorf					m_cShadowColor;

protected:

	char					*	m_pShaderCode;

	sn::SceneManager		*	m_pSmgr;		//!< The scene manager pointer

	sn::Camera              *   m_pActiveCamera;
	sn::Camera              *   m_pShadowCamera;

	gfx::Renderer			*	m_pRen;


	gfx::Shader				*	m_pVertexShader,	//!< The vertex shader for rendering shadow map
							*	m_pPixelShader,		//!< The pixel shader for rendering shadow map
							*	m_pVertexShader2,	//!< The vertex shader for rendering shadow map with alpha test
							*	m_pPixelShader2,	//!< The pixel shader for rendering shadow map with alpha test
							*	m_pVertexShader3,	//!< The vertex shader for rendering shadow map with instancing
							*	m_pVertexShader4;	//!< The vertex shader for rendering shadow map with instancing & alpha test

	gfx::ShaderHandle			m_shMatWVP,		//!< Shader handle for world view projection matrix.
								m_shMatWVP2,
								m_shMatWVP3,
								m_shMatWVP4;

	gfx::ShaderInstance		*	m_pShaderInstance,
							*	m_pShaderInstance2,
							*	m_pShaderInstance3,
							*	m_pShaderInstance4;

	gfx::Material			*	m_pMaterial,	//!< The shadow material
							*	m_pMaterial2,
							*	m_pMaterial3,
							*	m_pMaterial4;

	core::RectI					m_ViewPort;		//!< The view port for rendering in shadow map.

	math::Matrix			*	m_pProj,
							*	m_pOrtho,
							*	m_pBiasMatrix,
							*	m_pBiasFinal,
							*	m_pFinal;

	math::Frustum			*	m_pFrustum;

	int							m_iShadowMapSize;

	struct MeshMatrix 
	{
		sn::Mesh		*	pMesh;
		math::Matrix	*	pMatrix;

		MeshMatrix(sn::Mesh* pMesh, math::Matrix* pMatrix)
			: pMesh(pMesh), pMatrix(pMatrix) {}
	};

	void AddMeshes(std::vector<MeshMatrix>& Alpha, std::vector<MeshMatrix>& NoAlpha, sn::SceneNode* mesh);
								
}; // ShadowMapProjection

} // efx

} // kge

#endif // SHADOWMAPPROJECTION_H
