// File name: Refraction.h
// Des: This class is for refraction effects
// Date: 7/2/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_REFRACTION_H
#define KGE_REFRACTION_H

#include "Effect.h"
#include "../gfx/Shader.h"

namespace kge
{
namespace math
{
	class Matrix;
}
namespace efx
{
class KGE_API Refraction : public Effect
{
public:

	//! Constructor
	Refraction();

	//! Destructor
	~Refraction();

	//! Add nodes of mesh that effect influencing on it.
	void AddNodes( sn::SceneNode* pNodes );

	//! The works must to do before rendering the object.
	void PreRender(float elapsedTime);

	//! The works must to do after rendering the object.
	void Render();

	//! The works must to do after rendering all of the scene nodes.
	void PostRenderEverything();

	//! Sets the texcoord scale
	void SetTexScale(float x, float y);

protected:

	gfx::Shader			*	m_pVertexShader,
						*	m_pPixelShader;

	gfx::ShaderHandle 		sh_fvEyePosition,
							sh_matView,
							sh_matViewProjection,
							sh_fTime0_X,
							sh_f2TexScale;

	math::Matrix		*	m_pMatViewProj,
						*	m_pMatView;

	float                   m_fTime;

	math::Vector2F			m_v2TexScale;

	void SetShaderConstant(gfx::ShaderInstance* pSI);

}; // Refraction

} // efx

} // kge

#endif // KGE_REFRACTION_H
