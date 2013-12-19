// File name: Light.cpp
// Des: in file interface asli kar ba nor ha hast.
// Date: 25/2/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/gfx/Lighting.h"



namespace kge
{
namespace gfx
{
	//----------------------------------------------------------------
	// Ye nore jahat daro dar sahne ezafe mikone. Mesle nore Khorshid.
	//----------------------------------------------------------------
	LightData* Lighting::AddDirectionalLight(const math::Vector& vDir, 
				 					  const Colorf& color)
	{
		LightData* lit = KGE_NEW(LightData)();
		lit->Direction = vDir;
		lit->Ambient   = color * 0.4f;
		lit->Diffuse   = color;
		lit->Specular  = color * 0.6f;
		lit->Type	   = ELT_Directional;

		return lit;

	} // AddDirectionalLight

	//-----------------------------------------------------------------------------------
	// Ye nore noghte i ro be sahne ezafe mikone ke noresh dar tamame jahat pakhsh mishe.
	//-----------------------------------------------------------------------------------
	LightData* Lighting::AddPointLight(const math::Vector& vPosition, const Colorf& color)
	{
		LightData* lit = KGE_NEW(LightData)();
		lit->Ambient   = color * 0.4f;
		lit->Diffuse   = color;
		lit->Specular  = color * 0.6f;
		lit->Position  = vPosition;
		lit->Range     = 500.0f;
		lit->Type 	   = ELT_Point;

		return lit;

	} // AddPointLight

	//----------------------------------------------------------------
	// Ye nore spot ro be sahne ezafe mikone mesle nore cheragh ghove.
	//----------------------------------------------------------------
	LightData* Lighting::AddSpotLight(const math::Vector& vPosition, const math::Vector& vDirection,
					   const Colorf& color)
	{
		LightData* lit = KGE_NEW(LightData)();
		lit->Ambient   = color * 0.0f;
		lit->Diffuse   = color;
		lit->Specular  = color * 0.6f;
		lit->Position  = vPosition;
		lit->Direction = vDirection;
		lit->Range     = 500.0f;
		lit->Phi       = 0.9f;
		lit->Theta	   = 0.4f;
		lit->Type	   = ELT_Spot;

		return lit;

	} // AddSpotLight

} // gfx

} // kge