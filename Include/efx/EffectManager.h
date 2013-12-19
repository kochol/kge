// File name: EffectManager.h
// Des: This class manages effects and work with scene manager
// Date: 16/12/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef EFFECTMANAGER_H
#define EFFECTMANAGER_H

#include "ShadowMapProjection.h"
#include "Refraction.h"

namespace kge
{
namespace efx
{
class Bloom;

class KGE_API EffectManager
{
public:

	//! Constructor
	EffectManager();

	//! Destructor
	~EffectManager();

	//! Returns the EffectManager class pointer.
	static EffectManager* GetSingletonPtr();

	//! The scene manager calls this function when it calls every node prerender function
	void AfterSceneNodesPreRender(float elapsedTime = 0.0f);

	//! The scene manager calls this function when it calls every node render function
	void AfterSceneNodesRender(float elapsedTime = 0.0f);

	//! Creates a ShadowMapProjection class and returns its pointer
	ShadowMapProjection* AddShadowMapProjection(int ShadowMapSize = 512);

    //! 
    void RemoveShadowMapProjection(ShadowMapProjection* pShadowMapProj);

	//! Gets the shadow map
	ShadowMapProjection* GetShadowMapProjection();

    //! 
    void AddRefractionEffect();

    //!
    void RemoveRefractionEffect();

	//! Returns the refraction effect then you can add nodes to it.
	Refraction* GetRefractionEffect();

    //! The texture for rendered scene 
	gfx::Texture* GetSceneTexture() { return m_pColorMap; }

	//! Returns a temp render target (size = screen size) for index 0 to 7
	gfx::Texture* GetTempRenderTarget(int Index);
	
	/// For internal use. Renderer calls it on window resize
	void OnResize();

	/// For internal use. Renderer calls it on device reset
	void OnReset();

	/// Create the post bloom effect and returns its pointer
	Bloom* GetBloomEffect();

protected:

	bool							m_bNeedColor;
	gfx::Texture				*	m_pColorMap,
								*	m_pTempRenderTarget[8];

	std::vector<Effect*>			m_arrShadows,
									m_arrMaterials,
									m_arrPostEffects;

	ShadowMapProjection			*	m_pShadow;

	Refraction					*	m_pRefraction;

	Bloom						*	m_pBloom;

	void	NeedColorMap();

}; //  EffectManager

} // efx

} // kge

#endif // EFFECTMANAGER_H
