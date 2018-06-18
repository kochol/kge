// File name: Effect.h
// Des: In class interface class haye effect hast.
// Date: 17/10/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef EFFECT_H
#define EFFECT_H

#include "../kgedef.h"
#include <vector>

namespace kge
{

namespace sn
{
class SceneNode;

} // sn

//! Effects classes.
namespace efx
{

//! Noe effecthai ke mikhahid ezafe konid.
enum EffectType
{
	EET_ShadowPlane = 0,			/**< Saye e ke faghat roye ye plane miyofte. */
	EET_MirrorPlane = 1,			/**< Mirror ke bar roye ye plane ghabele ejrast. */
	EET_BumpMap,					/**< Bump Mapping effect. */
	EET_HardWareSkinning,			/**< Hardware skinning is for calculating skeletal animation on GPU. */
	EET_Cartoon,					/**< Cartoon effect. */
	EET_Shadow,						//!< Shadow effects.
	EET_Bloom,
	EET_PointLightInside,
	EET_PointLightOutside

}; // EffectType


class KGE_API Effect
{
public:

	//! Constructor
	Effect() {}
	
	//! Destructor
	virtual ~Effect() {}

	//! Add nodes of mesh that effect influencing on it.
	virtual void AddNodes( sn::SceneNode* pNodes )
	{
		m_arrNodes.push_back(pNodes);

	} // AddNodes

	//! The works must to do before rendering the object.
	virtual void PreRender(float elapsedTime = 0.0f) = 0;

	//! The works must to do after rendering the object.
	virtual void Render() = 0;

	//! The works must to do after rendering all of the scene nodes.
	virtual void PostRenderEverything() = 0;

	//! For internal use.
	virtual void OnReset() {}

protected:

	std::vector<sn::SceneNode*>			m_arrNodes;

}; // Effect

}  // efx

}  // kge
 
#endif // EFFECT_H