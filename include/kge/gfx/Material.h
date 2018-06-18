// File name: Material.h
// Des: The Material class.
// Date: 2/1/1386
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../KgeUnknown.h"
#include "../kgedef.h"
#include "../core/String.h"
#include "../core/KgeMemory.h"
#include "Color.h"
#include "Shader.h"
#include "Texture.h"

namespace kge
{
namespace gfx
{
	/*! The Material class.
	*/
class Material: public KgeUnknown
{
public:
	Material() : Shininess(0.0f), Alpha(false), TextureAlphal(false), Culling(true)
	{	
		shader = KGE_NEW(ShaderInstance)();
		for (int i = 0; i < 8; i++)
			ppTexture[i] = NULL;

	}

	~Material()
	{
		KGE_DELETE(shader, ShaderInstance);
	}

	Material& operator = (const Material& mat)
	{
		Name			= mat.Name;
		Ambient			= mat.Ambient;
		Diffuse			= mat.Diffuse;
		Specular		= mat.Specular;
		Emissive		= mat.Emissive;
		Shininess		= mat.Shininess;
		for (int i = 0; i < 8; i++)
			ppTexture[i] = mat.ppTexture[i];
		*shader = *mat.shader;
		Alpha			= mat.Alpha;
		TextureAlphal	= mat.TextureAlphal;
		Culling			= mat.Culling;

		return *this;
	}

	kge::core::String	Name;			//!< Material name
	Colorf				Ambient;		//!< Ambient color
	Colorf				Diffuse;		//!< Diffuse color
	Colorf				Specular;		//!< Specular color
	Colorf				Emissive;		//!< Emissive color
	float				Shininess;		//!< 0 - 128 Derakhshandegi.
	Texture			*	ppTexture[8];	//!< Textures.
	ShaderInstance	*	shader;			//!< A Pointer to the shader if it's exist.
	bool				Alpha;			//!< Transparency.
	bool				TextureAlphal;	//!< Texture Alpha
	bool				Culling;
	Colorf				Frensel;		//!< Frensel color
	float				FrenselVal;		//!< Frensel value

}; // Material

} // gfx

} // kge

#endif // MATERIAL_H