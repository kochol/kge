// File name: LightGL.h
// Des: in file interface asli kar ba nor ha dar OpenGL hast.
// Date: 10/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef LIGHTGL_H
#define LIGHTGL_H

namespace kge
{
namespace gfx
{
class LightGL : public Lighting
{
	friend class RendererGL;
public:

	// Sazande ke ye pointer be renderer ro negar midare.
	LightGL(RendererGL* renderer);

	// Mokhareb
	~LightGL() {}

	// Nore ambient ro set mikone.
	void SetAmbientLight(const Colorf& color);

	// Ye nore jahat daro dar sahne ezafe mikone.
	void SetLight(LightData *pLight, int Index);

protected:

	RendererGL *m_pRenderer;		// ye eshare gar be RendererGL.

}; // LightGL

} // gfx

} // kge

#endif // LIGHTGL_H
