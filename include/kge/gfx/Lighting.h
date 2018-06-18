// File name: Lighting.h
// Des: in file interface asli kar ba nor ha hast.
// Date: 10/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef LIGHTING_H
#define LIGHTING_H

#include "../KgeUnknown.h"
#include "Color.h"
#include "Structs.h"
#include "../math/Vector.h"

namespace kge
{
namespace gfx
{

//! In class interface asli kar ba nor ha hast.
class KGE_API Lighting: public KgeUnknown
{
public:

	/// Sazande ke tavasote class renderer seda zade mishe.
	Lighting() {}

	/// Mokhareb.
	virtual ~Lighting() {}

	//! Nore ambient ro set mikone. In noe nor nori hast ke dar tamame jahat be ye andaze pakhsh mishe.
	/*!
		\param color Nore ambient.
	 */
	virtual void SetAmbientLight(const Colorf& color) = 0;

	//! Ye nore jahat daro dar sahne ezafe mikone. Mesle nore khorshid.
	/*!
		\param pLight Eshare gar be Nore jahat dari ke mikhahid be sahne ezafe konid.
		\param Index shomare nori ke mikhahid jay gozin shavad ya ezafe shavad.
	*/
	virtual void SetLight(LightData *pLight, int Index) = 0;


	//! Ye nore jahat daro dar sahne ezafe mikone. Mesle nore Khorshid.
	/*!
		\param vDir Jahate nor.
		\param color Range nor.
		\return Yek LightDir barmigardone ba moshakhasate sakhte shode ke ba taghiresh mishe dobare ba SetLight ono ezafe kard.
	*/
	virtual LightData* AddDirectionalLight(const math::Vector& vDir,
				 					   const Colorf& color);

	//! Ye nore noghte i ro be sahne ezafe mikone ke noresh dar tamame jahat pakhsh mishe.
	/*!
		\param Position Mahale gharar giri nor.
		\param Color Range nor.
		\return Yek Nore noghte e ba moshakhasate sakhte shode bar migardone ke mishe ba taghiresh va ferestadane dobareye on be SetLight nore delkhah ro bedast avard.
	 */
	virtual LightData* AddPointLight(const math::Vector& vPosition, const Colorf& color);

	//! Ye nore spot ro be sahne ezafe mikone mesle nore cheragh ghove.
	/*!
		\param Position Mahale gharar giri nor.
		\param Direction Jahate nor ra moshakhas mikone.
		\param Color Range nor.
		\return Yek Nore Spot ra ba moshakhasate sakhte shode bar migardone ke mishe ba taghiresh va ferestadane dobareye on be SetLight nore delkhah ro bedast avard.
	 */
	virtual LightData* AddSpotLight(const math::Vector& vPosition, const math::Vector& vDirection,
							    const Colorf& color);

	//! Returns the ambient light color.
	virtual Colorf GetAmbientLight() { return m_cAmbient; }

protected:

	Colorf m_cAmbient;		// Ambient light color.

}; // Light

} // gfx

} // kge

#endif // LIGHTING_H
