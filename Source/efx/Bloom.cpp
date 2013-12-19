// File name: Bloom.h
// Des: Bloom post effect
// Date: 29/5/1390
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/efx/Bloom.h"
#include "../../Include/efx/EffectManager.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/gui/Image.h"
#include "../../Include/gui/GuiManager.h"

namespace kge
{
namespace efx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Bloom::Bloom(): m_bEnable(true)
	{
		//***************************************
		// Create Image
		//***************************************
		m_pMat				= KGE_NEW(gfx::Material)();

		gfx::Renderer*	ren = gfx::Renderer::GetSingletonPtr();
		kge::core::RectI ri;
		ri.X = 0;
		ri.Y = 0;
		ri.Width = ren->GetWindowWidth() / 4;
		ri.Height = ren->GetWindowHeight() / 4;
		float height = ren->GetWindowHeight();
		float width = ren->GetWindowWidth();

		m_pImage = Device::GetSingletonPtr()->GetGuiManager()->AddImage("../media/box.png", ri);
		m_pImage->m_BackImage = EffectManager::GetSingletonPtr()->GetSceneTexture();

		//***************************************
		// Create 4x down filter pixel shader
		//***************************************
		char ps4xDownFilterCode[] = 
			"float2 PixelCoordsDownFilter[16];\n"\
			"\n"\
			"sampler2D g_samSrcColor: register(s6);\n"\
			"\n"\
			"//-----------------------------------------------------------------------------\n"\
			"// Pixel Shader: DownFilter\n"\
			"// Desc: Perform a high-pass filter and on the source texture  and scale down.\n"\
			"//-----------------------------------------------------------------------------\n"\
			"float4 DownFilter( in float2 Tex : TEXCOORD0 ) : COLOR0\n"\
			"{\n"\
			"//return tex2D( g_samSrcColor, Tex);\n"\
			"    float4 Color = 0;\n"\
			"\n"\
			"    for (int i = 0; i < 16; i++)\n"\
			"    {\n"\
			"        Color += tex2D( g_samSrcColor, Tex +  PixelCoordsDownFilter[i].xy );\n"\
			"    }\n"\
			"\n"\
			"    return Color / 16;\n"\
			"}\n";

		m_pPs4xDownFilter = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(ps4xDownFilterCode, "DownFilter", gfx::ESV_PS2);
		m_shPixelCoordsDownFilter = m_pPs4xDownFilter->GetConstatnt("PixelCoordsDownFilter");

		m_fPixelCoordsDownFilter[0] = 1.5 / width;
		m_fPixelCoordsDownFilter[1] = -1.5 / height;
		m_fPixelCoordsDownFilter[2] = 1.5 / width;
		m_fPixelCoordsDownFilter[3] = -0.5 / height;
		m_fPixelCoordsDownFilter[4] = 1.5 / width;
		m_fPixelCoordsDownFilter[5] =  0.5 / height;
		m_fPixelCoordsDownFilter[6] = 1.5 / width;
		m_fPixelCoordsDownFilter[7] =  1.5 / height;
		m_fPixelCoordsDownFilter[8] = 0.5 / width;
		m_fPixelCoordsDownFilter[9] = -1.5 / height;
		m_fPixelCoordsDownFilter[10] = 0.5 / width;
		m_fPixelCoordsDownFilter[11] = -0.5 / height;
		m_fPixelCoordsDownFilter[12] = 0.5 / width;
		m_fPixelCoordsDownFilter[13] =  0.5 / height;
		m_fPixelCoordsDownFilter[14] = 0.5 / width;
		m_fPixelCoordsDownFilter[15] =  1.5 / height;
		m_fPixelCoordsDownFilter[16] =-0.5 / width;
		m_fPixelCoordsDownFilter[17] = -1.5 / height;
		m_fPixelCoordsDownFilter[18] =-0.5 / width;
		m_fPixelCoordsDownFilter[19] = -0.5 / height;
		m_fPixelCoordsDownFilter[20] =-0.5 / width;
		m_fPixelCoordsDownFilter[21] =  0.5 / height;
		m_fPixelCoordsDownFilter[22] =-0.5 / width;
		m_fPixelCoordsDownFilter[23] =  1.5 / height;
		m_fPixelCoordsDownFilter[24] =-1.5 / width;
		m_fPixelCoordsDownFilter[25] = -1.5 / height;
		m_fPixelCoordsDownFilter[26] =-1.5 / width;
		m_fPixelCoordsDownFilter[27] = -0.5 / height;
		m_fPixelCoordsDownFilter[28] =-1.5 / width;
		m_fPixelCoordsDownFilter[29] =  0.5 / height;
		m_fPixelCoordsDownFilter[30] =-1.5 / width;
		m_fPixelCoordsDownFilter[31] =  1.5 / height;

		//***************************************
		// Bright pass
		//***************************************
		char psBrightPass[] = 
			"sampler2D g_txSrcColor;\n"\
			"\n"\
			"static const float Luminance = 0.1f;\n"\
			"static const float fMiddleGray = 0.18f;\n"\
			"static const float fWhiteCutoff = 0.8f;\n"\
			"\n"\
			"//-----------------------------------------------------------------------------\n"\
			"// Pixel Shader: BrightPassFilter\n"\
			"// Desc: Perform a high-pass filter on the source texture\n"\
			"//-----------------------------------------------------------------------------\n"\
			"float4 BrightPassFilter( in float2 Tex : TEXCOORD0 ) :  COLOR0\n"\
			"{\n"\
			"    float3 ColorOut = tex2D( g_txSrcColor, Tex ).rgb;\n"\
			"//return float4(Tex.x,Tex.y,0.0, 1.0);\n"\
			"    ColorOut *= fMiddleGray / ( Luminance + 0.001f );\n"\
			"    ColorOut *= ( 1.0f + ( ColorOut / ( fWhiteCutoff *  fWhiteCutoff ) ) );\n"\
			"    ColorOut -= 5.0f;\n"\
			"\n"\
			"    ColorOut = max( ColorOut, 0.0f );\n"\
			"\n"\
			"    ColorOut /= ( 10.0f + ColorOut );\n"\
			"\n"\
			"    return float4( ColorOut, 1.0f );\n"\
			"}\n";

		m_pPsBrightPass = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(psBrightPass, "BrightPassFilter", gfx::ESV_PS2);

		//***************************************
		// Vertical Bloom
		//***************************************
		char psVerticlaBloom[] =
			"sampler2D g_samSrcColor;\n"\
			"\n"\
			"static const int g_cKernelSize = 13;\n"\
			"float2 PixelKernel[g_cKernelSize];\n"\
			"static const float BlurWeights[g_cKernelSize] = \n"\
			"{\n"\
			"    0.002216,\n"\
			"    0.008764,\n"\
			"    0.026995,\n"\
			"    0.064759,\n"\
			"    0.120985,\n"\
			"    0.176033,\n"\
			"    0.199471,\n"\
			"    0.176033,\n"\
			"    0.120985,\n"\
			"    0.064759,\n"\
			"    0.026995,\n"\
			"    0.008764,\n"\
			"    0.002216,\n"\
			"};\n"\
			"\n"\
			"static const float BloomScale = 1.5f;\n"\
			"\n"\
			"//-----------------------------------------------------------------------------\n"\
			"// Pixel Shader: HorizontalBlur\n"\
			"// Desc: Blurs the image vertically\n"\
			"//-----------------------------------------------------------------------------\n"\
			"float4 PostProcessPS( float2 Tex : TEXCOORD0 ) : COLOR0\n"\
			"{\n"\
			"    float4 Color = 0;\n"\
			"\n"\
			"    for (int i = 0; i < g_cKernelSize; i++)\n"\
			"    {    \n"\
			"        Color += tex2D( g_samSrcColor, Tex +  PixelKernel[i].xy ) * BlurWeights[i];\n"\
			"    }\n"\
			"\n"\
			"    return Color * BloomScale;\n"\
			"}\n";

		for (int i = 0; i < 26; i+=2)
			m_fPixelKernelV[i] = 0.0;
		m_fPixelKernelV[1] =    -6.0 / height;
		m_fPixelKernelV[3] =    -5.0 / height;
		m_fPixelKernelV[5] =    -4.0 / height;
		m_fPixelKernelV[7] =    -3.0 / height;
		m_fPixelKernelV[9] =    -2.0 / height;
		m_fPixelKernelV[11] =    -1.0 / height;
		m_fPixelKernelV[13] =     0.0 / height;
		m_fPixelKernelV[15] =     1.0 / height;
		m_fPixelKernelV[17] =     2.0 / height;
		m_fPixelKernelV[19] =     3.0 / height;
		m_fPixelKernelV[21] =     4.0 / height;
		m_fPixelKernelV[23] =     5.0 / height;
		m_fPixelKernelV[25] =     6.0 / height;
		for (int i = 1; i < 26; i+=2)
			m_fPixelKernelH[i] = 0.0;
		m_fPixelKernelH[0] =    -6.0 / width;
		m_fPixelKernelH[2] =    -5.0 / width;
		m_fPixelKernelH[4] =    -4.0 / width;
		m_fPixelKernelH[6] =    -3.0 / width;
		m_fPixelKernelH[8] =    -2.0 / width;
		m_fPixelKernelH[10] =    -1.0 / width;
		m_fPixelKernelH[12] =     0.0 / width;
		m_fPixelKernelH[14] =     1.0 / width;
		m_fPixelKernelH[16] =     2.0 / width;
		m_fPixelKernelH[18] =     3.0 / width;
		m_fPixelKernelH[20] =     4.0 / width;
		m_fPixelKernelH[22] =     5.0 / width;
		m_fPixelKernelH[24] =     6.0 / width;

		m_pPsVerticalBloom = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(psVerticlaBloom, "PostProcessPS", gfx::ESV_PS2);

		if (m_pPsVerticalBloom)
			m_shPixelKernel = m_pPsVerticalBloom->GetConstatnt("PixelKernel");

		// Combine shader
		char psCombine[] = 
			"sampler2D g_samSrcColor;\n"\
			"sampler2D g_samSceneColor: register (s6);\n"\
			"\n"\
			"//-----------------------------------------------------------------------------\n"\
			"// Pixel Shader: Combine\n"\
			"// Desc: Combine the source image with the original image.\n"\
			"//-----------------------------------------------------------------------------\n"\
			"float4 Combine( float2 Tex : TEXCOORD0\n"\
			"                 ) : COLOR0\n"\
			"{\n"\
			"    float3 ColorOrig = tex2D( g_samSceneColor, Tex * 4 ).rgb;\n"\
			"\n"\
			"    ColorOrig = tex2D( g_samSrcColor, Tex ).rgb;// * float3(2.0, 0.0, 0.0);\n"\
			"\n"\
			"    return float4( ColorOrig, 1.0f );\n"\
			"}\n";

		m_pPsCombine = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString(psCombine, "Combine", gfx::ESV_PS2);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Bloom::~Bloom()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// The works must to do before rendering the object.
	//------------------------------------------------------------------------------------
	void Bloom::PreRender( float elapsedTime /*= 0.0f*/ )
	{

	} // PreRender

	//------------------------------------------------------------------------------------
	// The works must to do after rendering the object.
	//------------------------------------------------------------------------------------
	void Bloom::Render()
	{
		if (!m_bEnable)
			return;

		gfx::Renderer*	ren = gfx::Renderer::GetSingletonPtr();
		EffectManager* emgr = EffectManager::GetSingletonPtr();

		ren->Enable(gfx::ERF_2D);
		ren->SetTextureParams(gfx::ETP_Anisotropic, 0);
		ren->SetTextureParams(gfx::ETP_Point, 6);

		m_pImage->SetUV(0.0, 0.0, 1.0, 1.0);
		m_pImage->SetSize(ren->GetWindowWidth() / 4, ren->GetWindowHeight() / 4);

		// 4X down filter
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(1));
		m_pMat->shader->m_pPixelShader = m_pPs4xDownFilter;
		ren->SetMaterial(m_pMat);
		m_pPs4xDownFilter->SetConstant(m_shPixelCoordsDownFilter, m_fPixelCoordsDownFilter, 32);
		m_pImage->m_BackImage = emgr->GetSceneTexture();
		m_pImage->Render();

		m_pImage->SetUV(0.0, 0.0, 0.25, 0.25);

		// Render bright pass in 1/4 size in render target 0
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(0));
		m_pMat->shader->m_pPixelShader = m_pPsBrightPass;
		ren->SetMaterial(m_pMat);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(1);
		m_pImage->Render();

		// Render vertical bloom in render target 1
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(1));
		m_pMat->shader->m_pPixelShader = m_pPsVerticalBloom;
		m_pPsVerticalBloom->SetConstant(m_shPixelKernel, m_fPixelKernelV, 26);
		ren->SetMaterial(m_pMat);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(0);
		m_pImage->Render();

		// Render horizontal bloom in render target 0
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(0));
		m_pMat->shader->m_pPixelShader = m_pPsVerticalBloom;
		m_pPsVerticalBloom->SetConstant(m_shPixelKernel, m_fPixelKernelH, 26);
		ren->SetMaterial(m_pMat);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(1);
		m_pImage->Render();
  
		// Render vertical bloom in render target 1
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(1));
		m_pMat->shader->m_pPixelShader = m_pPsVerticalBloom;
		m_pPsVerticalBloom->SetConstant(m_shPixelKernel, m_fPixelKernelV, 26);
		ren->SetMaterial(m_pMat);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(0);
		m_pImage->Render();
  
		// Render horizontal bloom in render target 0
		ren->SetRenderTarget(0, emgr->GetTempRenderTarget(0));
		m_pMat->shader->m_pPixelShader = m_pPsVerticalBloom;
		m_pPsVerticalBloom->SetConstant(m_shPixelKernel, m_fPixelKernelH, 26);
		ren->SetMaterial(m_pMat);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(1);
		m_pImage->Render();

		// Render result
		ren->SetRenderTarget();
		ren->Disable(gfx::ERF_DepthBuffer);
		m_pMat->shader->m_pPixelShader = m_pPsCombine;//NULL;//
		ren->SetMaterial(m_pMat);
		ren->SetTexture(emgr->GetSceneTexture(), 6);
		m_pImage->m_BackImage = emgr->GetTempRenderTarget(0);//emgr->GetSceneTexture();//
		m_pImage->SetSize(ren->GetWindowWidth(), ren->GetWindowHeight());
		ren->SetStencilBuffer(EET_Bloom, 1);

		m_pImage->Render();

		ren->Enable(gfx::ERF_DepthBuffer);
		ren->SetStencilBuffer(EET_Bloom, 2);
		ren->Disable(gfx::ERF_2D);

	} // Render

} // efx

} // kge
