// File name: LightGL.cpp
// Des: in file interface asli kar ba nor ha dar OpenGL hast.
// Date: 10/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "RendererGL.h"
#include "LightGL.h"
#include "../../Include/math/math.h"



namespace kge
{
namespace gfx
{
	//---------------------------------------------------
	// Sazande ke ye pointer be renderer ro negar midare.
	//---------------------------------------------------
	LightGL::LightGL(RendererGL* renderer)
	{
		m_pRenderer = renderer;

	} // Sazande

	//----------------------------
	// Nore ambient ro set mikone.
	//----------------------------
	void LightGL::SetAmbientLight(const Colorf& color)
	{
//		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &color.r);

	} // SetAmbientLight

	//-------------------------------------------
	// Ye nore jahat daro dar sahne ezafe mikone.
	//-------------------------------------------
	void LightGL::SetLight(LightData *pLight, int Index)
	{
// 		if ( Index > 7 )
// 			return;
// 
// 		GLenum eIndex = GL_LIGHT0 + Index;
// 		glLightfv(eIndex, GL_AMBIENT, &pLight->Ambient.r);
// 		glLightfv(eIndex, GL_DIFFUSE, &pLight->Diffuse.r);
// 		glLightfv(eIndex, GL_SPECULAR, &pLight->Specular.r);
// 		
// 		GLfloat lightPos[4];
// 		if (pLight->Type == ELT_Directional)
// 		{
// 			lightPos[0] = -pLight->Direction.x;
// 			lightPos[1] = pLight->Direction.y;
// 			lightPos[2] = -pLight->Direction.z;
// 			lightPos[3] = 0.0f;
// 		}
// 		else if (pLight->Type == ELT_Point)
// 		{
// 			lightPos[0] = pLight->Position.x;
// 			lightPos[1] = pLight->Position.y;
// 			lightPos[2] = pLight->Position.z;
// 			lightPos[3] = 1.0f;
// 		}
// 		glLightfv(eIndex, GL_POSITION, lightPos);
// 		glEnable(eIndex);

	} // SetLight(LightDir)

} // gfx

} // kge
