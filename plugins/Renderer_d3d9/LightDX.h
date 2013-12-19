// File name: LightDX.h
// Des: in file interface asli kar ba nor ha dar Direct3D 9 hast.
// Date: 10/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifdef WIN32

#ifndef LIGHTDX_H
#define LIGHTDX_H

namespace kge
{
namespace gfx
{
class LightDX : public Lighting
{
	friend class RendererDX;
public:

	//Constructor keeps a pointer to renderer. 
	LightDX(RendererDX* renderer);

	// destructor	
	~LightDX() {}

	// Set the Ambient's light.
	void SetAmbientLight(const Colorf& color);

	// Adds a directional light to the scene .
	void SetLight(LightData *pLight, int Index);

protected:

	RendererDX *m_pRenderer;		//A pointer to RendererDX.

}; // LightDX

} // gfx

} // kge

#endif // LIGHTDX_H

#endif // WIN32