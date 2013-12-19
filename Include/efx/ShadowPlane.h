// File name: ShadowPlane.h
// Des: This class create planar shodow on the flat mesh.
// Date: 07-Apr-2007

#ifndef SHADOWPLANE_H
#define SHADOWPLANE_H

#include "Effect.h"
#include "../../Include/gfx/Renderer.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"


namespace kge
{
namespace efx
{

class KGE_API ShadowPlane : public Effect
{
public:
	//! Constructor
	ShadowPlane( gfx::Renderer* rn , sn::SceneNode* sn );
	
	//! Destructor
	~ShadowPlane();

	/*! Initialize Shadow Plane.
		\param Shadow The created Matrix by plane.
		\return If succeed return true.
	*/
	bool Init ( math::Matrix* Shadow );

	//! Add nodes of mesh that they have shadow
	void AddNodes( sn::SceneNode* pNodes );

	//! Prerender setting.
	void PreRender(float elapsedTime = 0.0f);

	void Render();

	//! Post render setting.
	void PostRenderEverything();

	//! Set the shadow matrix.
	void SetShadowMatrix(const kge::math::Matrix& mat);

protected:
	gfx::Renderer*				m_pRenderer;	       
	sn::SceneNode*				m_pSceneNode;
	math::Matrix*				m_pShadowMatrix;      
	std::vector<sn::SceneNode*> m_vNodes;

}; // ShadowPlane

}  // efx

}  // kge

#endif // SHADOWPLANE_H