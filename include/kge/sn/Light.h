// File name: Light.h
// Des: This is a lighting scene node.
// Date: 28/4/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef LIGHT_H
#define LIGHT_H

#include "../gfx/Structs.h"
#include "SceneNode.h"

namespace kge
{
namespace sn
{
class KGE_API Light : public SceneNode
{
public:

	//! Constructor
	Light(gfx::LightData* light, SceneManager* smgr, gfx::Renderer* renderer);

	//! Destructor
	~Light();

	//! Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	void PreRender(float elapsedTime = 0.0f);

	//! Render kardane Object.
	void Render();

	//! Karhaee ke bad az render bayad anjam beshe.
	void PostRender();

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	bool AddEffect ( efx::EffectType p, efx::Effect** ppOut );

	/*! Draw the node with/without its Materials and Transforms.
		\param WithMaterial Draw it with its own materials or not?
		\param WithMaterial Draw it with its own Transformations or not?
	 */
	void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	//! Set the position of the light.
	virtual void SetPosition(const math::Vector& v);

	//! Returns the light position.
	virtual const math::Vector& GetPosition() const;

	//! Returns true if the light position changed since this function last call.
	virtual bool IsPositionChanged();

	/// Returns the light data to read/write them.
	gfx::LightData* GetLightData() const;

protected:
	gfx::LightData* m_pLight;

	bool			m_bPosChanged;		// Is light position changed?

}; // Light

} //sn

} // kge

#endif // LIGHT_H