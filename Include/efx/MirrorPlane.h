// File name: MirrorPlane.h
// Des: In class be shey haye 3D mitone effecte Ayne bede.
// Date: 18/10/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef MIRRORPLANE_H
#define MIRRORPLANE_H

#include "Effect.h"
#include "../../Include/gfx/Renderer.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"


namespace kge
{
namespace efx
{

class KGE_API MirrorPlane : public Effect
{
public:
	//! Constructor
	MirrorPlane( gfx::Renderer* rn, sn::SceneNode* pMirror );
	
	//! Destructor
	~MirrorPlane();

	/*! Sakhtane Mirror Plane va amade kardanesh baraye kar.
		\param Reflect Matrixi ke tavasote plane sakhte shode hast.
		\return Agar movafagh shod true bar migardone.
	 */
	bool Init( math::Matrix* Reflect );

	//! Add nodes of mesh that reflecting in mirror
	void AddNodes( sn::SceneNode* pNodes );

	//! Tanzimat ghabl az render on mesh.
	void PreRender(float elapsedTime = 0.0f);

	//! Tsnzimat bad az render on mesh.
	void Render();

	//! kar hayi ke bad az tamam shodan render hame chiz bayad anjam beshe.
	void PostRenderEverything();

protected:
	gfx::Renderer*		m_pRenderer;	       
	sn::SceneNode*	m_pMirror;
	math::Matrix*		m_pReflectMatrix;      
	std::vector<sn::SceneNode*> m_vNodes;

}; // MirrorPlane

}  // efx

}  // kge

#endif // MIRRORPLANE_H