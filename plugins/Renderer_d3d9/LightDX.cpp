// File name: LightDX.cpp
// Des: in file interface asli kar ba nor ha dar Direct3D 9 hast.
// Date: 10/8/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifdef WIN32

#include "rendererdx.h"
#include "lightdx.h"



namespace kge
{
namespace gfx
{
	//---------------------------------------------------
	// Sazande ke ye pointer be renderer ro negar midare.
	//---------------------------------------------------
	LightDX::LightDX(RendererDX* renderer)
	{
		m_pRenderer = renderer;
		m_cAmbient.r = 0.0f;
		m_cAmbient.g = 0.0f;
		m_cAmbient.b = 0.0f;

	} // Sazande

	// Nore ambient ro set mikone.
	void LightDX::SetAmbientLight(const Colorf& color)
	{
		m_cAmbient = color;
		int nRed   = (int)(color.r * 255.0f);
		int nGreen = (int)(color.g * 255.0f);
		int nBlue  = (int)(color.b * 255.0f);
		m_pRenderer->m_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(nRed, 
																	nGreen, nBlue));

	} // SetAmbientLight

	//-------------------------------------------
	// Ye nore jahat daro dar sahne ezafe mikone.
	//-------------------------------------------
	void LightDX::SetLight(LightData *pLight, int Index)
	{
		if ( Index > 7 )
			return;

		D3DLIGHT9 light;
		::ZeroMemory(&light, sizeof(light));

		memcpy(&light.Ambient, &pLight->Ambient, sizeof(float) * 4);
		memcpy(&light.Diffuse, &pLight->Diffuse, sizeof(float) * 4);
		memcpy(&light.Specular, &pLight->Specular, sizeof(float) * 4);

		switch ( pLight->Type )
		{
			case ELT_Directional:
				light.Type = D3DLIGHT_DIRECTIONAL;
				memcpy(&light.Direction, &pLight->Direction.x, sizeof(float) * 3);
				break;
			
			case ELT_Point:
				light.Type = D3DLIGHT_POINT;
				memcpy(&light.Position, &pLight->Position.x, sizeof(float) * 3);
				light.Range = pLight->Range;
				light.Attenuation0 = 1.0f;
				light.Attenuation1 = 0.0f;
				light.Attenuation2 = 0.0f;
				break;

			case ELT_Spot:
				light.Type = D3DLIGHT_SPOT;
				memcpy(&light.Position, &pLight->Position.x, sizeof(float) * 3);
				memcpy(&light.Direction, &pLight->Direction.x, sizeof(float) * 3);
				light.Range = pLight->Range;
				light.Falloff      = 1.0f;
				light.Attenuation0 = 1.0f;
				light.Attenuation1 = 0.0f;
				light.Attenuation2 = 0.0f;
				light.Theta        = pLight->Theta;
				light.Phi		   = pLight->Phi;
				break;
		}

		m_pRenderer->m_pd3dDevice->SetLight(Index, &light);
		m_pRenderer->m_pd3dDevice->LightEnable(Index, TRUE);

	} // SetLight(LightDir)

} // gfx

} // kge

#endif // WIN32