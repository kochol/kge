// File name: Bloom.h
// Des: Bloom post effect
// Date: 29/5/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_BLOOM_H
#define KGE_BLOOM_H

#include "Effect.h"
#include "../gfx/Shader.h"

namespace kge
{
namespace gui
{
	class Image;
}
namespace gfx
{
	class Material;
}
namespace efx
{
class KGE_API Bloom : public Effect
{
public:

	//! Constructor
	Bloom();

	//! Destructor
	~Bloom();

	//! The works must to do before rendering the object.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! The works must to do after rendering the object.
	virtual void Render();

	//! The works must to do after rendering all of the scene nodes.
	virtual void PostRenderEverything() { }

	void SetEnable(bool bEnable) {m_bEnable = bEnable;}

protected:

	gfx::Shader						*	m_pPs4xDownFilter,
									*	m_pPsBrightPass,
									*	m_pPsVerticalBloom,
									*	m_pPsCombine;

	gfx::ShaderHandle					m_shPixelKernel,
										m_shPixelCoordsDownFilter;

	float								m_fPixelKernelV[26];
	float								m_fPixelKernelH[26];
	float								m_fPixelCoordsDownFilter[32];

	gui::Image						*	m_pImage;

	gfx::Material					*	m_pMat;

	bool								m_bEnable;



private:

}; // Bloom

} // efx

} // kge

#endif // KGE_BLOOM_H
