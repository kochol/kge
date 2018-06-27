// Filename:	RendererGL.cpp
// Des:			OpenGL Renderer
// Date:		aug-01-2010
// Programmer:	Pooya Shahinfar (Pswin)

/* notes: this is new KGE OpenGL renderer imp which is based on OpenGL 2.0 or later versions.
		  you can set in use version by setting KGE_OPENGL_RENDERER_VER definition in config.h
*/

/* supported versions:
	- OpenGL 2.x  (recommend)
	- OpenGL 3.x
*/

#include "../../Libs/glew/glew.h"
#include "../../include/kge/config.h"

#include "RendererGL.h"
#include "LightGL.h"
#include "ShaderGL.h"
#include "TextureGL.h"
#include "VertexBufferGL.h"
#include "IndexBufferGL.h"
#include "../../include/kge/io/File.h"
#include "../../include/kge/gfx/ImageData.h"
#include "../../include/kge/io/Logger.h"

// external libs
#include <math.h>
#include <IL/il.h>
#include <IL/ilu.h>
//#include "../../Libs/DevIL/ilut.h"

#ifdef KGE_USE_SDL
	#include <SDL2/SDL.h>
#endif

extern KGE_API SDL_Window* g_pSDLwindow;

#if KGE_COMPILER == KGE_COMPILER_MSVC
#	pragma comment(lib, "DevIL.lib")
#	pragma comment(lib, "ilu.lib")
#	pragma comment(lib, "sdl2.lib")
#	pragma comment(lib, "cg.lib")
#	pragma comment(lib, "cggl.lib")
#	pragma comment(lib, "../../Libs/glew32.lib")
#endif 

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace kge
{
namespace gfx
{
	void RendererGL::checkForCgError(const char *situation, const char* ShaderName)
	{
		CGerror error;
		const char *string = cgGetLastErrorString(&error);

		if (error != CG_NO_ERROR) 
		{
			io::Logger::Error("%s: %s: %s\n",
				ShaderName, situation, string);
			if (error == CG_COMPILER_ERROR) 
			{
				io::Logger::Error("%s\n", cgGetLastListing(m_CgContext));
			}
		}
	}

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	RendererGL::RendererGL(): m_pContext(NULL), m_CgContext(NULL)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// destructor
	//------------------------------------------------------------------------------------ 
	RendererGL::~RendererGL()
	{
		if (m_pContext)
			SDL_GL_DeleteContext(m_pContext);

	} // ~RendererGL

	// Init
	bool RendererGL::Init(Device* device, InitParameters &params)
	{
		m_pDevice	= device;
		m_Params	= params;
		m_pWindow	= g_pSDLwindow;

		// set all texture units to 0
		for ( int i = 0 ; i < 8 ; i++)
			m_nTextID[i] = MAXID ;

 		if (params.AAMode == EAAM_MSAA)
 			SetVideoMode(params.Width, params.Height, params.Bits,params.UseStencil,params.FullScreen ,4, params.VSync);
 		else
 			SetVideoMode(params.Width, params.Height, params.Bits,params.UseStencil,params.FullScreen ,0, params.VSync);

		// Needed to initialize DevIL.
		ilInit ();

		// GL cannot use palettes anyway, so convert early.
		ilEnable (IL_CONV_PAL);

		m_pContext = SDL_GL_CreateContext(m_pWindow);

		// Create CG context
 		m_CgContext = cgCreateContext();
 		checkForCgError("creating CG context");
 		cgGLSetDebugMode(CG_TRUE);
 		cgSetParameterSettingMode(m_CgContext, CG_IMMEDIATE_PARAMETER_SETTING);
 
 		m_CgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);
 		cgGLSetOptimalOptions(m_CgVertexProfile);
 		checkForCgError("selecting vertex profile");
 		cgGLEnableProfile(m_CgVertexProfile);
 
 		m_CgFragmentProfile = cgGLGetLatestProfile(CG_GL_FRAGMENT);
 		cgGLSetOptimalOptions(m_CgFragmentProfile);
 		checkForCgError("selecting fragment profile");
 		cgGLEnableProfile(m_CgFragmentProfile);

		m_bUseShaders = true;

		// Init the Light class
		Lights = KGE_NEW(LightGL)(this);

		io::Logger::Log("Using OpenGL for rendering.");
		return AfterInit();

	} // Init

	// *** ******* ***** ** ** ****** ***
	// Pak kardane pixel ha va buffer ha.
	// *** ******* ***** ** ** ****** ***
	bool RendererGL::Clear(bool Pixel, bool Zbuffer, bool Stencil)
	{
		GLbitfield Mask;

		if (Pixel)
			Mask = GL_COLOR_BUFFER_BIT;
		if (Zbuffer)
			Mask |= GL_DEPTH_BUFFER_BIT;
		if (Stencil && m_Params.UseStencil)
			Mask |= GL_STENCIL_BUFFER_BIT;

		glClear(Mask);

		return true;
	} // Clear

	//----------------------------------------------------------------------------------------
	// Creates a renderable texture to use it as a render target
	//----------------------------------------------------------------------------------------
	bool RendererGL::CreateRenderableTexture(Texture** ppOutTexture, int width, int height, 
		TextureFormat tf, bool CreateDepthStencilBuffer, bool AntiAliasing)
	{
		// TODO: write this function for opengl.

		*ppOutTexture = NULL;
		return false;

	} // CreateRenderableTexture

	//----------------------------------------------------------------------------------------
	// Begin Rendering
	//----------------------------------------------------------------------------------------
	bool RendererGL::BeginRendering(bool Pixel, bool Zbuffer, bool Stencil)
	{
		if (!m_bIsSceneRunning)
		{
			Clear(Pixel, Zbuffer, Stencil);

			m_bIsSceneRunning = true;

			// calculate fps.
			m_iFPSTemp++;
			if (m_pTimer->NextFrame())
			{
				m_iFPS = m_iFPSTemp;
				m_iFPSTemp = 0;
			}

			// if camera is 2D
			if (m_bRF[ERF_2D])
			{
				math::Matrix mMat;
				mMat = m_mView * m_mWorld;
// 				glMatrixMode(GL_MODELVIEW);
// 				glLoadMatrixf((GLfloat*)&mMat);
// 				glMatrixMode(GL_PROJECTION);
// 				glLoadMatrixf((GLfloat*)&m_mProj);
				m_bRF[ERF_2D] = false;
			}
			m_iTriCount  = 0;
			m_iDrawCount = 0;
		}

		return true;
	} // BeginRendering

	//------------------------------------------------------------------------------------
	// End Rendering
	//------------------------------------------------------------------------------------
	void RendererGL::EndRendering()
	{
		if (m_bIsSceneRunning)
		{
			SDL_GL_SwapWindow(m_pWindow);			

			m_bIsSceneRunning = false;
		}
	} // EndRendering()

	// *** ******* ***** *** ******
	// Set kardane Range pas zamine
	// *** ******* ***** *** ******
	void RendererGL::SetClearColor(const Colorf& C)
	{
		m_cClearColor = C;
		glClearColor(C.r, C.g, C.b, 1.0f);

	} // SetClearColor

	// *** ** **** ** **** ***** ** **** ******** ****** ****** *** *******
	// bad az Init ba seda zadan in tabe maghadir avalie engine set mishan.
	// *** ** **** ** **** ***** ** **** ******** ****** ****** *** *******
	bool RendererGL::AfterInit()
	{
		// Mohasebeye dorbine 2 bodi.
// 		m_m2D._11 = (float)2/(m_Params.Width);
// 		m_m2D._22 = (float)2/(-m_Params.Height);
// 		m_m2D._12 = m_m2D._13 = m_m2D._14 = m_m2D._21 = m_m2D._23 = m_m2D._24 =
// 		m_m2D._31 = m_m2D._32 = m_m2D._34 = m_m2D._43 = 0.0f;
// 		m_m2D._41 = m_m2D._33 = -1.0f;
// 		m_m2D._44 = m_m2D._42 = 1.0f;
		// Calculate the 2D camera matrix.
		m_m2D.SetOrthoOffscreenLH(0.0f, m_Params.Width, m_Params.Height, 0.0f, 0.0f, 1.0f);

		glViewport(0 ,0 ,m_Params.Width ,m_Params.Height);		// Make our viewport the whole window

		// Enables Depth Testing
		glEnable(GL_DEPTH_TEST);
		// Enable texturing.
		glEnable(GL_TEXTURE_2D);
		// Enable backface culling.
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		// GLEW INIT
 		GLenum err = glewInit();
 
 		if ( err != GLEW_OK )
 		{
 			io::Logger::Log((const char*)glewGetErrorString(err), kge::io::ELM_Error);
 		}
 		else
 		{
 			io::Logger::Log(kge::io::ELM_Information, "OPENGL version: %s", glGetString(GL_VERSION));
 		}

		// Creating a timer.
		m_pTimer = new core::Timer();
		m_pTimer->Interval = 1000;
		m_iFPS = 0;
		m_iFPSTemp = 0;

		// Precalculate the graphics card capacities.
		InitGCC();

		// Create m_pV2
		CustomVertexElement decl[] =
		{
			{0,  0, EVET_Float2, EVEM_Default, EVEU_Position, 0},
			CusVertexEND()
		};		
		m_ppVertexDecs[EVT_V2] = CreateVertexDeclaration(decl, "V2");

		// Create m_pV3
		CustomVertexElement decl2[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3] = CreateVertexDeclaration(decl2, "V3");

		// Create m_pV3C
		CustomVertexElement decl3[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Color, EVEM_Default, EVEU_Color, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3C] = CreateVertexDeclaration(decl3, "V3C");

		// Create m_pV3T
		CustomVertexElement decl4[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3T] = CreateVertexDeclaration(decl4, "V3T");

		// Create m_pV3CT
		CustomVertexElement decl8[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Float4, EVEM_Default, EVEU_Color, 0},
			{0,  28, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3CT] = CreateVertexDeclaration(decl8, "V3CT");

		// Create m_pV3TN
		CustomVertexElement decl5[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Float3, EVEM_Default, EVEU_Normal, 0},
			{0,  24, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3TN] = CreateVertexDeclaration(decl5, "V3TN");

		// Create m_pV3TNC
		CustomVertexElement decl6[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Float3, EVEM_Default, EVEU_Normal, 0},
			{0,  24, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			{0,  32, EVET_Float1, EVEM_Default, EVEU_Color, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3TNC] = CreateVertexDeclaration(decl6, "V3TNC");

		// Create m_pV3TTN
		CustomVertexElement decl7[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_Float3, EVEM_Default, EVEU_Normal, 0},
			{0,  24, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			{0,  32, EVET_Float2, EVEM_Default, EVEU_TexCoord, 1},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_V3TTN] = CreateVertexDeclaration(decl7, "V3TTN");

		// Create m_pP0NT1BT2
		CustomVertexElement decl9[] =
		{
			{0,  0,  EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{1,  0,  EVET_Float3, EVEM_Default, EVEU_Normal,   0},
			{1,  12, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			{2,  0,  EVET_Float3, EVEM_Default, EVEU_Tangent,  0},
			{2,  12, EVET_Float3, EVEM_Default, EVEU_Binormal, 0},
			{3,  0,  EVET_Float4, EVEM_Default, EVEU_TexCoord, 1},
			{3,  16, EVET_Float4, EVEM_Default, EVEU_TexCoord, 2},
			{3,  32, EVET_Float4, EVEM_Default, EVEU_TexCoord, 3},
			{3,  48, EVET_Float4, EVEM_Default, EVEU_TexCoord, 4},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_P0NT1BT2I3] = CreateVertexDeclaration(decl9, "P0NT1BT2");

		// Create m_pP0NTC1BT2
		CustomVertexElement decl11[] =
		{
			{0,  0,  EVET_Float3,   EVEM_Default, EVEU_Position, 0},
			{1,  0,  EVET_Float3,   EVEM_Default, EVEU_Normal, 0},
			{1,  12, EVET_Float2,   EVEM_Default, EVEU_TexCoord, 0},
			{1,  20, EVET_Color, 	EVEM_Default, EVEU_Color, 0},
			{2,  0,  EVET_Float3,   EVEM_Default, EVEU_Tangent, 0},
			{2,  12, EVET_Float3,   EVEM_Default, EVEU_Binormal, 0},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_P0NTC1BT2] = CreateVertexDeclaration(decl11, "P0NTC1BT2");

		// Create m_pPJ0NT1BT2
		CustomVertexElement decl10[] =
		{
			{0,  0, EVET_Float3, EVEM_Default, EVEU_Position, 0},
			{0,  12, EVET_UByte4, EVEM_Default, EVEU_Color, 0},
			{1,  0, EVET_Float3, EVEM_Default, EVEU_Normal, 0},
			{1,  12, EVET_Float2, EVEM_Default, EVEU_TexCoord, 0},
			{2,  0, EVET_Float3, EVEM_Default, EVEU_Tangent, 0},
			{2,  12, EVET_Float3, EVEM_Default, EVEU_Binormal, 0},
			{3,  0,  EVET_Float4, EVEM_Default, EVEU_TexCoord, 1},
			{3,  16, EVET_Float4, EVEM_Default, EVEU_TexCoord, 2},
			{3,  32, EVET_Float4, EVEM_Default, EVEU_TexCoord, 3},
			{3,  48, EVET_Float4, EVEM_Default, EVEU_TexCoord, 4},
			CusVertexEND()
		};		

		m_ppVertexDecs[EVT_PJ0NT1BT2I3] = CreateVertexDeclaration(decl10, "PJ0NT1BT2");

		return true;

	} // AfterInit()

	//----------------------------------------------------------------------------------------
	// Set the renderable target where renderer will render there.
	//----------------------------------------------------------------------------------------
	void RendererGL::SetRenderTarget(int RenderNumber, Texture* RenderTarget)
	{
		// TODO: Write this function for OpenGL

	} // SetRenderTarget

	// *** ******* *******
	// Set kardane dorbin.
	// *** ******* *******
	void RendererGL::SetViewLookatLH(math::Vector &vPos, math::Vector &vPoint,
									 math::Vector &vUp)
	{
		math::Vector Z = vPoint - vPos; // Z
		Z.Normalize();
		math::Vector X;					// X
		X.Cross(vUp, Z);
		X.Normalize();

		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = vUp * Z;
		vcTemp = Z * fDot;
		vcUp = vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis
		if (fL < 1e-6f)
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);

				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					io::Logger::Log("Error: Can't calcute the up vector for camera.", io::ELM_Error);
					return;
				}
			}
		}
		vcUp /= fL;

		m_mView._11 = X.x;
		m_mView._12 = vcUp.x;
		m_mView._13 = Z.x;
		m_mView._21 = X.y;
		m_mView._22 = vcUp.y;
		m_mView._23 = Z.y;
		m_mView._31 = X.z;
		m_mView._32 = vcUp.z;
		m_mView._33 = Z.z;
		m_mView._41 = -(X * vPos);
		m_mView._42 = -(vcUp * vPos);
		m_mView._43 = -(Z * vPos);
		m_mView._44 = 1.0f;
		m_mView._14 = m_mView._24 = m_mView._34 = 0.0f;

// 		glMatrixMode(GL_MODELVIEW);
// 		glLoadMatrixf((GLfloat*)&m_mView);
	} // SetViewLookAtLH

	// *** ******* *******
	// Set kardane dorbin.
	// *** ******* *******
	void RendererGL::SetViewLookatRH(math::Vector &vPos, math::Vector &vPoint,
									 math::Vector &vUp)
	{
		math::Vector Z = vPos -  vPoint; // Z
		Z.Normalize();
		math::Vector X;					// X
		X.Cross(vUp, Z);
		X.Normalize();
		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = vUp * Z;
		vcTemp = Z * fDot;
		vcUp = vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis
		if (fL < 1e-6f)
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);

				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					io::Logger::Log("Error: Can't calcute the up vector for camera.", io::ELM_Error);
					return;
				}
			}
		}
		vcUp /= fL;

		m_mView._11 = X.x;
		m_mView._12 = vcUp.x;
		m_mView._13 = Z.x;
		m_mView._21 = X.y;
		m_mView._22 = vcUp.y;
		m_mView._23 = Z.y;
		m_mView._31 = X.z;
		m_mView._32 = vcUp.z;
		m_mView._33 = Z.z;
		m_mView._41 = -(X * vPos);
		m_mView._42 = -(vcUp * vPos);
		m_mView._43 = -(Z * vPos);
		m_mView._44 = 1.0f;
		m_mView._14 = m_mView._24 = m_mView._34 = 0.0f;

// 		glMatrixMode(GL_MODELVIEW);
// 		glLoadMatrixf((GLfloat*)&m_mView);
	} // SetViewLookAtRH

	// ******* ********** ** *** ****** ** ****** **** *****
	// Matrixe Projection ro set mikone Be sorate Chap dast.
	// ******* ********** ** *** ****** ** ****** **** *****
	void RendererGL::SetPerspectiveLH(float Fov, float Near, float Far)
	{
		float ys = 1/tanf(Fov/2);
		float xs = ys/((float)m_Params.Width/(float)m_Params.Height);
		m_mProj._12=m_mProj._13=m_mProj._14=m_mProj._21=m_mProj._23=m_mProj._24=
			m_mProj._31=m_mProj._32=m_mProj._41=m_mProj._42=m_mProj._44=0.0f;
		m_mProj._11 = xs;
		m_mProj._22 = ys;
		m_mProj._33 = Far/(Far-Near);
		m_mProj._34 = 1.0f;
		m_mProj._43 = -Near*Far/(Far-Near);

// 		glMatrixMode(GL_PROJECTION);
// 		glLoadMatrixf((GLfloat*)&m_mProj);
	} // SetPerspectiveLH

	// ******* ********** ** *** ****** ** ****** **** *****
	// Matrixe Projection ro set mikone Be sorate Rast dast.
	// ******* ********** ** *** ****** ** ****** **** *****
	void RendererGL::SetPerspectiveRH(float Fov, float Near, float Far)
	{
		float ys = 1/tanf(Fov/2);
		float xs = ys/((float)m_Params.Width/(float)m_Params.Height);
		m_mProj._12=m_mProj._13=m_mProj._14=m_mProj._21=m_mProj._23=m_mProj._24=
			m_mProj._31=m_mProj._32=m_mProj._41=m_mProj._42=m_mProj._44=0.0f;
		m_mProj._11 = xs;
		m_mProj._22 = ys;
		m_mProj._33 = Far/(Far-Near);
		m_mProj._34 = -1.0f;
		m_mProj._43 = Near*Far/(Far-Near);
// 		glMatrixMode(GL_PROJECTION);
// 		glLoadMatrixf((GLfloat*)&m_mProj);
	} // SetPerspectiveRH

	// ****** ***** ** ****** ********
	// Maknae Ajsam ra taghir midahad.
	// ****** ***** ** ****** ********
	void RendererGL::SetTransForm(math::Matrix *mat, TransformMode TM)
	{
		math::Matrix mWorld;
		if (mat)
			mWorld = *mat;

		switch (TM)
		{
		case ETM_World:
			m_mWorld = mWorld;
//  			glMatrixMode(GL_MODELVIEW);
//  			glLoadMatrixf((GLfloat*)&mWorld);
			break; // ETM_World

		case ETM_View:
			m_mView = mWorld;
			m_mViewProj = m_mProj * m_mView;
// 			mWorld = mWorld * m_mWorld;
// 			glMatrixMode(GL_MODELVIEW);
// 			glLoadMatrixf((GLfloat*)&mWorld);
			break; // ETM_View

		case ETM_Projection:
// 			glMatrixMode(GL_PROJECTION);
// 			glLoadMatrixf((GLfloat*)mat);
			m_mProj = mWorld;
			m_mViewProj = m_mProj * m_mView;
			break; // ETM_Projection

		} // switch

	} // SetWorldTransform

	//---------------------------------------------------
	// Matrix haye asli dorbin va ajsam ra bar migardone.
	//---------------------------------------------------
	math::Matrix RendererGL::GetTransForm(TransformMode TM)
	{
		switch (TM)
		{
		case ETM_World:
			return m_mWorld;
			break;
		case ETM_View:
			return m_mView;
			break;
		case ETM_Projection:
			if (m_bRF[ERF_2D])
				return m_m2D;
			return m_mProj;
			break;
		case ETM_ViewProjection:
			if (m_bRF[ERF_2D])
				return m_m2D;
			return m_mViewProj;
			break;
		}
		
		return m_mWorld;

	} // GetTransForm

// 	// ***** ******* ** ********
// 	// Ezafe kardane ye texture.
// 	// ***** ******* ** ********
// 	bool RendererGL::AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
// 		const char* Name, void* ExtraParam)
// 	{
// 		// TODO: This function is changed and need to remake
// 
// 		ILuint ImgId; // IL image ID
// 		GLuint TexID; // GL texture ID
// 
// 		// Generate the main image name to use.
// 		ilGenImages (1, &ImgId);
// 
// 		// Bind this image name.
// 		ilBindImage (ImgId);
// 
// 		// Loads the image specified by File into the ImgId image.
// 		if (!ilLoadImage (FileName))
// 		{
// 			CheckDevilErrors( FileName );
// 			return false;
// 		}
// 
//         iluFlipImage();
// 
// 		// Goes through all steps of sending the image to OpenGL.
// 		//TexID = ilutGLBindTexImage();
// 		TexID = ilutGLBindMipmaps();
// 		ilutGLBuildMipmaps();
// 
// 
// 
// 		// We're done with our image, so we go ahead and delete it.
// 		ilDeleteImages(1, &ImgId);
// 
// 		return false;
// 
// 	} // AddTexture

	//-------------------------------------------------------------------------------------------------------
	// This function is for Texture Manager for adding textures use SceneManager AddTexture function instead.
	//-------------------------------------------------------------------------------------------------------
	bool RendererGL::AddTexture(Texture** ppOutTexture, u32 Handle, const char* FileName,
		const char* Name, void* ExtraParam)
	{
		*ppOutTexture = KGE_NEW(gfx::TextureGL)(Handle,FileName,Name,ExtraParam);
		
		return false;

	} // AddTexture

	// ***** *** ***** *******
	// Check for devil errors.
	// ***** *** ***** *******
	void RendererGL::CheckDevilErrors( const char* TextureName )
	{
		ILenum Error;
		while ((Error = ilGetError()))
		{
			if ( Error == IL_COULD_NOT_OPEN_FILE )
				kge::io::Logger::Log(io::ELM_Error , "Could not load texture: %s" , TextureName );
			else
				kge::io::Logger::Log( kge::io::ELM_Error , "Error from DevIL on %s: %s", TextureName , iluErrorString(Error));
		}
	} // CheckDevilErrors

	// *** ******* ******* ***** ** *******
	// Set kardane texture ghabl az render.
	// *** ******* ******* ***** ** *******
	void RendererGL::SetTexture(Texture* pTex, int index)
	{
		if (!pTex)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			glDisable(GL_TEXTURE_2D);
			return;
		}

 		if (m_nTextID[index] != pTex->GetHandle())
 		{
			glActiveTexture(GL_TEXTURE0 + index);
			glEnable(GL_TEXTURE_2D);
 			glBindTexture(GL_TEXTURE_2D, ((TextureGL*)pTex)->GetID());
 			m_nTextID[index] = pTex->GetHandle();
 		}
	} // SetTexture

	void RendererGL::DrawTriangleList(u32 VCount, u32 ICount, VertexDec* CV, u32 VertexStart, u32 StartIndex)
	{
		if (ICount > 0)
		{
			if (StartIndex > 0)
				glDrawElements(GL_TRIANGLES, ICount, m_iIndexBufferType, (GLvoid*)(StartIndex * m_iIndexBufferSize));
			else
				glDrawElements(GL_TRIANGLES, ICount, m_iIndexBufferType, 0);
			m_iTriCount += ICount / 3 * m_iBatchCount;			
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, VCount);
			m_iTriCount += VCount / 3 * m_iBatchCount;
		}
		m_iDrawCount++;

	} // DrawTriangleList

	void RendererGL::DrawTriangleList( HardwareBuffer* VB, HardwareBuffer* IB, u32 VCount, u32 ICount, VertexType eVType /*= EVT_V3TN */ )
	{
		SetVertexDec( m_ppVertexDecs[eVType] ); 	

		SetVertexBuffer(VB, 0);


		if (ICount > 0)
		{
			SetIndexBuffer(IB);

			glDrawElements(GL_TRIANGLES, ICount, m_iIndexBufferType, 0);
			m_iTriCount += ICount / 3 * m_iBatchCount;			
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, VCount);
			m_iTriCount += VCount / 3 * m_iBatchCount;
		}
		m_iDrawCount++;

	} // DrawTriangleList

	//------------------------------------
	// Enable kardane khsiyat haye render.
	//------------------------------------
	void RendererGL::Enable(RenderFlags RF)
	{
		switch (RF)
		{
			//-----------
			// Dorbine 2D
			//-----------
		case ERF_2D:
// 			glMatrixMode(GL_PROJECTION);
// 			glLoadMatrixf((GLfloat*)&m_m2D);
// 			glMatrixMode(GL_MODELVIEW);
// 			glLoadIdentity();
			Disable(ERF_Lighting);
			m_bRF[ERF_2D] = true;
			break;
			//------------------------
			// Nor Pardazi (Lighting).
			//------------------------
		case ERF_Lighting:
			glEnable(GL_LIGHTING);
			m_bRF[ERF_Lighting] = true;
			break;
			/////////////////////////////
			// amizesh (Blending).     //
			/////////////////////////////
		case ERF_AlphaBlending:
			if (m_bRF[ERF_AlphaBlending])
				return;
			glEnable(GL_BLEND);
			m_bRF[ERF_AlphaBlending] = true;
			break;

			//////////////////////
			// Stencil Buffer. //
			////////////////////
		case ERF_StencilBuffer:
			if ( m_Params.UseStencil == false )
				return;
			glEnable(GL_STENCIL_TEST);
			m_bRF[ERF_StencilBuffer] = true;
			break;

			///////////////////////////////
			// Depth Buffer (Z Buffer). //
			/////////////////////////////
		case ERF_DepthBuffer:
			glEnable(GL_DEPTH_TEST);
			m_bRF[ERF_DepthBuffer] = true;
			break;

		case ERF_Culling:
			if (m_bRF[ERF_Culling])
				return;
			glEnable(GL_CULL_FACE);
			m_bRF[ERF_Culling] = true;
			break;

		///////////
		// VSync //
		//////////
		case ERF_VSync:
		#ifdef KGE_USE_SDL
			SDL_GL_SetSwapInterval( 1 );
			m_bRF[ERF_VSync] = true;
		#endif
			break;

			///////////////////////
			//   S C I S S O R   //
			///////////////////////
		case ERF_Scissor:
			glEnable(GL_SCISSOR_TEST);
			m_bRF[ERF_Scissor] = true;
			break;

		} // switch (RF)

	} // Enable

	//-------------------------------------
	// Disable kardane khsiyat haye render.
	//-------------------------------------
	void RendererGL::Disable ( RenderFlags RF )
	{
		math::Matrix mMat;
		switch (RF)
		{

			/////////////////
			// Dorbine 2D //
			///////////////
		case ERF_2D:
			mMat = m_mView * m_mWorld;
// 			glMatrixMode(GL_MODELVIEW);
// 			glLoadMatrixf((GLfloat*)&mMat);
// 			glMatrixMode(GL_PROJECTION);
// 			glLoadMatrixf((GLfloat*)&m_mProj);

			m_bRF[ERF_2D] = false;
			break;
			//////////////////////////////
			// Nor Pardazi (Lighting). //
			////////////////////////////
		case ERF_Lighting:
			glDisable(GL_LIGHTING);
			m_bRF[ERF_Lighting] = false;
			break;
			/////////////////////////////
			// Shafafiyat (Blending). //
			///////////////////////////
		case ERF_AlphaBlending:
			glDisable(GL_BLEND);
			m_bRF[ERF_AlphaBlending] = false;
			break;

			//////////////////////
			// Stencil Buffer. //
			////////////////////
		case ERF_StencilBuffer:
			glDisable(GL_STENCIL_TEST);
			m_bRF[ERF_StencilBuffer] = false;
			break;

			///////////////////////////////
			// Depth Buffer (Z Buffer). //
			/////////////////////////////
		case ERF_DepthBuffer:
			glDisable(GL_DEPTH_TEST);
			m_bRF[ERF_DepthBuffer] = false;
			break;

		///////////
		// VSync //
		//////////
		case ERF_VSync:
		#ifdef KGE_USE_SDL
			SDL_GL_SetSwapInterval( 0 );
			m_bRF[ERF_VSync] = false;
		#endif
			break;

		case ERF_Culling:
			if (!m_bRF[ERF_Culling])
				return;
			glDisable(GL_CULL_FACE);
			m_bRF[ERF_Culling] = false;
			break;

			  ///////////////////////
			 //   S C I S S O R   //
			///////////////////////
		case ERF_Scissor:
			glDisable(GL_SCISSOR_TEST);
			m_bRF[ERF_Scissor] = false;
			break;

		} // switch (RF)

	}  // Disable

	//--------------------------------------------------
	// Set kardane kamele ya matrial besorate automatic.
	//--------------------------------------------------
	void RendererGL::SetMaterial(Material *mat)
	{
//		if( CanDo(EGCC_OpenGL2))
//			glUseProgram(0);

		// Setting the Ambinet color.
// 		glMaterialfv(GL_FRONT, GL_AMBIENT, &mat->Ambient.r);
// 		glMaterialfv(GL_FRONT, GL_DIFFUSE, &mat->Diffuse.r);
// 		glMaterialfv(GL_FRONT, GL_SPECULAR, &mat->Specular.r);

		// Setting the Texture.
		SetTexture(mat->ppTexture[0]);

		if (mat->Culling)
			Enable(ERF_Culling);
		else
			Disable(ERF_Culling);

		// Setting the shader.
//		if( CanDo(EGCC_OpenGL2))
		{
			// Setting the shaders.
			if (mat->shader)
			{
				if (mat->shader->m_pFun)
					mat->shader->m_pFun->Call(mat->shader);
				if (mat->shader->m_pVertexShader)
				{
					mat->shader->m_pVertexShader->PreRender();
					mat->shader->m_pVertexShader->Render();
				}
				else if (m_bUseShaders)
				{
					glUseProgram(0);
				}
				if (mat->shader->m_pPixelShader)
				{
					mat->shader->m_pPixelShader->PreRender();
					mat->shader->m_pPixelShader->Render();
				}
				else if (m_bUseShaders)
				{
					glUseProgram(0);
				}
			}
			else
			{
				if (m_bUseShaders)
				{
					glUseProgram(0);
				}
			}
		}

		m_pLastMaterial = mat;

	} // SetMaterial

	//----------------------------------------------------
	// Set kardane keyfiyat texture ha va address mode ha.
	//----------------------------------------------------
	void RendererGL::SetTextureParams(TextureParams Params, int TextureStage)
	{
		glActiveTexture(GL_TEXTURE0 + TextureStage);

		switch (Params)
		{
		//*** F I L T E R I N G S
		case gfx::ETP_Point:
			glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			break;

		case gfx::ETP_Linear:
			glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			break;

		case gfx::ETP_Anisotropic:
			//check for hardware support
			if (!strstr((char*)glGetString(GL_EXTENSIONS),"GL_EXT_texture_filter_anisotropic"))
			{
				//get max ANISOTROPY and set it
				float maximumAnistropy;
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnistropy);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnistropy);
			}
			break;

		//*** A D D R E S S I N G
		case gfx::ETP_Wrap:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			break;

		case gfx::ETP_BorderColor:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 0x00000000 );
			break;

		case gfx::ETP_Clamp:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			break;

		case gfx::ETP_Mirror:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT_ARB);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT_ARB);
			break;
		}

	} // SetTextureFilters

	//-------------------------------------------------------------------------------------
	// Baraye fahmidane inke aya cart graphic az on ghabelilat ha poshti bani mikone ya na.
	//-------------------------------------------------------------------------------------
	bool RendererGL::CanDo ( GraphicCardCaps Item )
	{
		return m_bGCC[ Item ];

	} // CanDo

	//-------------------------------------------------------
	// Tanzimate stencil buffer baraye effect haye mokhtalef.
	//-------------------------------------------------------
	void RendererGL::SetStencilBuffer(kge::efx::EffectType ET, u8 State)
	{

			if ( ET == efx::EET_MirrorPlane )
		{
			switch ( State )
			{
			// Setting before draw mirror
			case 1:
				glDisable(GL_CULL_FACE);
				glStencilFunc(GL_ALWAYS,0x1,0xffffffff);
				glStencilMask(0xffffffff);
				glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

				// disable writes to the depth and back buffers
				glDepthMask(GL_FALSE);
				glBlendFunc(GL_ZERO,GL_ONE);
				break;

			// Setting after draw mirror and befor draw reflection into mirror
			case 2:
				glDepthMask(GL_TRUE);
				glStencilFunc(GL_EQUAL,0x1,0xffffffff);
				glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

				glBlendFunc(GL_DST_COLOR,GL_ZERO);
				//glCullFace(GL_CW);
				break;
			case 3:

				glStencilFunc(GL_ALWAYS,0x1,0xffffffff);
				glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
				//glCullFace(GL_CCW);
				glEnable(GL_CULL_FACE);
				break;
			}
		}
	if ( ET == efx::EET_ShadowPlane )
		{
			switch ( State )
			{
			case 1:
				glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF );
				glStencilOp(GL_KEEP,GL_INCR,GL_INCR);

				glDepthMask(GL_FALSE);
				glStencilMask(0xffffffff);
				glEnable(GL_EXT_blend_color);
				glBlendColor(1,1,1,0.5);
				glBlendFunc(GL_ZERO,GL_SRC_ALPHA );

				// Use polygon offset
				glEnable( GL_POLYGON_OFFSET_FILL );
				glPolygonOffset( -1, -3 );

				break;
			case 2:
				glDepthMask(GL_TRUE);
				glDisable( GL_POLYGON_OFFSET_FILL );
				glDisable( GL_EXT_blend_color );
				glBlendFunc(GL_ONE,GL_ZERO);

				break;
			case 3:
				glStencilFunc(GL_ALWAYS,0x1,0xffffffff);
				glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
				break;
			}
		}



	} // SetStencilBuffer

	//------------
	// Create font
	//------------
	void RendererGL::AddFont( kge::gui::Font** ppOut,
								 const char* strFontName, ul32 dwHeight )
	{
		*ppOut = NULL;

	} // CreateFont

// 	//-----------------------------------------------------
// 	// Create a Display List(vertex buffer) and store it on video memory.
// 	//-----------------------------------------------------
// 	u32 RendererGL::CreateVertexBuffer(void* Vertices, u32 VCount,
// 									   u16 * Indis   , u32 ICount,
// 									   VertexType eVType)
// 	{
// 		GLsizei stride;
// 		glEnableClientState(GL_VERTEX_ARRAY);
// 		switch (eVType)
// 		{
// 			// EVT_V3
// 		case EVT_V3:
// 			stride = 0;
// 			Vertex3 *vV;
// 			vV = (Vertex3*)Vertices;
// 			glVertexPointer(3,GL_FLOAT,stride,Vertices);
// 			break;
// 
// 			// EVT_V3C
// 		case EVT_V3C:
// 			glEnableClientState(GL_COLOR_ARRAY);
// 			stride = sizeof(Vertex3C);
// 			Vertex3C *vVC;
// 			vVC = (Vertex3C*)Vertices;
// 			glVertexPointer(3,GL_FLOAT,stride,vVC);
// 			glColorPointer(3,GL_INT,stride,&vVC[0].c );
// 			break;
// 
// 			// EVT_V3T
// 		case EVT_V3T:
// 			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
// 			stride = sizeof(Vertex3T) ;
// 			Vertex3T *vVT;
// 			vVT = (Vertex3T*)Vertices;
// 			glVertexPointer(3, GL_FLOAT, stride, &vVT[0].pos);
// 			glTexCoordPointer(2, GL_FLOAT, stride, &vVT[0].tex);
// 			break;
// 
// 			// EVT_V3TN
// 		case EVT_V3TN:
// 			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
// 			glEnableClientState(GL_NORMAL_ARRAY);
// 			stride = sizeof(Vertex3TN) ;
// 			Vertex3TN *vVTN;
// 			vVTN = (Vertex3TN*)Vertices;
// 			glVertexPointer(3, GL_FLOAT, stride, &vVTN[0].pos);
// 			glNormalPointer(GL_FLOAT, stride, &vVTN[0].Nor);
// 			glTexCoordPointer(2, GL_FLOAT, stride, &vVTN[0].tex);
// 			break;
// 
// 			// EVT_V3TNC
// 		case EVT_V3TNC:
// 			glEnableClientState(GL_COLOR_ARRAY);
// 			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
// 			glEnableClientState(GL_NORMAL_ARRAY);
// 			stride = sizeof(Vertex3TNC) ;
// 			Vertex3TNC *vVTNC;
// 			vVTNC = (Vertex3TNC*)Vertices;
// 			glVertexPointer(3,GL_FLOAT,stride,vVTNC);
// 			glNormalPointer(GL_FLOAT, stride, &vVTNC[0].Nor);
// 			glColorPointer(3,GL_FLOAT,stride, &vVTNC[0].Color );
// 			glTexCoordPointer(2, GL_FLOAT, stride, &vVTNC[0].tex);
// 			break;
// 
// 		} // switch (eVType)
// 
// 
// 		// generate display list
// 		int iDList ;
// 		iDList = glGenLists(1);
// 
// 		// check for vaild display list
// 		if (glIsList(iDList) != GL_TRUE)
// 			return -1;
// 
// 		// fill display list
// 		glNewList(iDList,GL_COMPILE);
// 			if (ICount>0)
// 			{
// 				glDrawElements(GL_TRIANGLES, ICount, GL_UNSIGNED_SHORT, Indis);
// 			}
// 			else
// 				glDrawArrays(GL_TRIANGLES, 0, VCount);
// 		glEndList();
// 
// 		// disalbe the array lists
// 		glDisableClientState(GL_NORMAL_ARRAY);
// 		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
// 		glDisableClientState(GL_VERTEX_ARRAY);
// 		glDisableClientState(GL_COLOR_ARRAY);
// 
// 		return iDList;
// 
// 	} // CreateVertexBuffer


	// CreateVertexBuffer
	kge::gfx::HardwareBuffer* RendererGL::CreateVertexBuffer(void* Vertices, u32 VCount, 
											   VertexType eVType ,bool isDynamic )
	{
		// TODO: Implement this function
		u32 id;
		glGenBuffers(1,&id);

		if (!id)
		{
			io::Logger::Log("Creating Vertex buffer failed",io::ELM_Error);
			return NULL;
		}


		GLuint size = 0;
		int stride = 0;

		switch (eVType)
		{
		case EVT_V2:
			size = sizeof(Vertex2) * VCount;
			break;
		case EVT_V3:
			size = sizeof(Vertex3) * VCount;
			break;
		case EVT_V3C:
			size = sizeof(Vertex3C) * VCount;
			stride = sizeof(Vertex3C);
			break;
		case EVT_V3T:
			size = sizeof(Vertex3T) * VCount;
			stride = sizeof(Vertex3T);
			break;
		case EVT_V3TN:
			size = sizeof(Vertex3TN) * VCount;
			stride = sizeof(Vertex3TN);
			break;
		case EVT_V3TNC:
			size = sizeof(Vertex3TNC) * VCount;
			stride = sizeof(Vertex3TNC);
			break;
		case EVT_V3TTN:
			size = sizeof(Vertex3TNC) * VCount;
			stride = sizeof(Vertex3TNC);
			break;
		}

		// Is dynamic?
		GLenum dyn = GL_STATIC_DRAW;
		if (isDynamic)
			dyn = GL_DYNAMIC_DRAW;

		// binding vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER,id);

		glBufferData(GL_ARRAY_BUFFER,size,Vertices, dyn);

		// Create VertexBufferGLvbo
		VertexBufferGL* pVB = KGE_NEW(VertexBufferGL)(VCount, stride, isDynamic);
		pVB->m_uID = id;

		AddHardwareBuffer(pVB);

		return pVB;
	
	} 

	//------------------------------------------------------------------------------------
	// Create a vertex buffer from custom vertex type and store it on video memory.
	//------------------------------------------------------------------------------------
	HardwareBuffer* RendererGL::CreateVertexBuffer
		(	
		void* Vertices,
		u32 VCount,
		u32 Stride,
		bool isDynamic
		)
	{
		// Create a new buffer
		GLuint  bufID;
		glGenBuffers(1, &bufID);

		// Is dynamic?
		GLenum dyn = GL_STATIC_DRAW;
		if (isDynamic)
			dyn = GL_DYNAMIC_DRAW;

		// bind buffer and pass data to it.
		glBindBuffer(GL_ARRAY_BUFFER, bufID);
		glBufferData(GL_ARRAY_BUFFER, VCount * Stride, Vertices, dyn);

		// Create VertexBufferGLvbo
		VertexBufferGL* pVB = KGE_NEW(VertexBufferGL)(VCount, Stride, isDynamic);
		pVB->m_uID = bufID;

		AddHardwareBuffer(pVB);

		return pVB;

	} // CreateVertexBuffer
	

	// CreateVertexShaderFromFile
	Shader* RendererGL::CreateVertexShaderFromFile(const char* VertexFileName, const char* VertexMain, 
		ShaderVersion eVVersion)
	{
		CGprogram CgVertexProgram =
			cgCreateProgramFromFile
			(
			m_CgContext,              /* Cg runtime context */
			CG_SOURCE,                /* Program in human-readable form */
			VertexFileName,  /* Name of file containing program */
			m_CgVertexProfile,        /* Profile: OpenGL ARB vertex program */
			VertexMain,      /* Entry function name */
			NULL
			);                    /* No extra compiler options */
		checkForCgError("creating vertex program from string");
		if (!CgVertexProgram)
			return NULL;

		cgGLLoadProgram(CgVertexProgram);
		checkForCgError("loading vertex program");

		Shader *shader = KGE_NEW(ShaderGL)(CgVertexProgram, 0, NULL, NULL);
		return shader;


	}

	// CreateVertexShaderFromString
	Shader* RendererGL::CreateVertexShaderFromString(const char* VertexCode, const char* VertexMain, 
		ShaderVersion eVVersion)
	{
		CGprogram CgVertexProgram =
			cgCreateProgram
			(
				m_CgContext,              /* Cg runtime context */
				CG_SOURCE,                /* Program in human-readable form */
				VertexCode,				  /* Vertex shader source code */
				m_CgVertexProfile,        /* Profile: OpenGL ARB vertex program */
				VertexMain,      /* Entry function name */
				NULL
			);                    /* No extra compiler options */
		checkForCgError("creating vertex program from string");
		if (!CgVertexProgram)
			return NULL;

		cgGLLoadProgram(CgVertexProgram);
		checkForCgError("loading vertex program");

 		Shader *shader = KGE_NEW(ShaderGL)(CgVertexProgram, 0, NULL, NULL);
 		return shader;

	} // CreateVertexShaderFromString

	// InitGCC
	void RendererGL::InitGCC()
	{
		// EGCC_VertexShader1_1
		m_bGCC[ EGCC_VertexShader1_1 ] = (bool)glewGetExtension("GL_ARB_vertex_shader");

		// EGCC_OpenGL2
		char *temp;
		temp = (char*) glGetString(GL_VERSION);
		if (temp)
		{
			if ( temp[0] == '2' )
				m_bGCC[ gfx::EGCC_OpenGL2 ] = true;
			else
				m_bGCC[ gfx::EGCC_OpenGL2 ] = false;

			if ( temp[0] == '3' )
				m_bGCC[ gfx::EGCC_OpenGL3 ] = true;
			else
				m_bGCC[ gfx::EGCC_OpenGL3 ] = false;

			if ( temp[0] == '4' )
				m_bGCC[ gfx::EGCC_OpenGL4 ] = true;
			else
				m_bGCC[ gfx::EGCC_OpenGL4 ] = false;
		}

		// EGCC_FragmentShader1_1
 		m_bGCC[ gfx::EGCC_FragmentShader1_1 ] = (bool)glewGetExtension("GL_ARB_fragment_shader");
 
 		// EGCC_ShaderObject
 		m_bGCC[ gfx::EGCC_ShaderObject ] = (bool)glewGetExtension("GL_ARB_shader_objects");
 
 		// EGCC_GeometryShader
 		m_bGCC[ gfx::EGCC_GeometryShader4 ] = (bool)glewGetExtension("GL_EXT_geometry_shader4");
 
 		// EGCC_ShaderModel 4
 		m_bGCC[ gfx::EGCC_ShaderModel4 ] = (bool)glewGetExtension("GL_EXT_gpu_shader4");
 
 		// EGCC_ShaderModel 3
 		m_bGCC[ gfx::EGCC_ShaderModel3 ] = (bool)glewGetExtension("GL_EXT_gpu_shader3");
 
 		// EGCC_ShaderModel 2
 		m_bGCC[ gfx::EGCC_ShaderModel2 ] = (bool)glewGetExtension("GL_EXT_gpu_shader2");
 
 		// support for compresed textures
 		m_bGCC[gfx::EGCC_DXT1] = m_bGCC[gfx::EGCC_DXT3] 
 					= m_bGCC[gfx::EGCC_DXT5] = (bool)glewGetExtension("");

	}

	// SetFog
	bool RendererGL::SetFog(FogType mode,Colorf FogColor ,float expDensity,float linearStart,float linearEnd )
	{
		glEnable(GL_FOG);
		switch (mode)
		{
			case EFT_NONE:
				glDisable(GL_FOG);
				return true;
				break;
			case EFT_LINEAR:
				glFogi(GL_FOG_MODE,GL_LINEAR);
				glFogf(GL_FOG_START ,linearStart);
				glFogf(GL_FOG_END ,linearEnd);
				break;
			case EFT_EXP:
				glFogi(GL_FOG_MODE,GL_EXP);
				glFogf(GL_FOG_DENSITY , expDensity);
				break;
			case EFT_EXP2:
				glFogi(GL_FOG_MODE,GL_EXP2);
				glFogf(GL_FOG_DENSITY , expDensity);
				break;
		}
		glHint(GL_FOG_HINT, GL_DONT_CARE);
		float fogcolor[] = {FogColor.r ,FogColor.g,FogColor.b,1.0};
		glFogfv(GL_FOG_COLOR,fogcolor);
		// TODO: fog must be decleared as a enviroment variable for new shader based piple of kge
		return true;
	}

	//---------------------------------------
	// set video settings
	//---------------------------------------
	bool RendererGL::SetVideoMode(int Width, int Height , int Bits,
			  bool UseStencil , bool Fullscreen ,int MultiSample,bool VSync)
	{
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		if ( UseStencil)
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE ,8);

		if( MultiSample > 0 && ((MultiSample%2) == 0))
		{
				glEnable(GL_MULTISAMPLE);
				SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
				SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MultiSample);
		}

		SDL_GL_SetSwapInterval((int)VSync);

		SDL_SetWindowSize(m_pWindow, Width, Height);
		if (Fullscreen != m_Params.FullScreen)
			SDL_SetWindowFullscreen(m_pWindow, (Uint32)Fullscreen);

		// set view port
		glViewport(0 ,0 ,m_Params.Width ,m_Params.Height);		// Make our viewport the whole window

		SetPerspectiveLH(45,1,1000);

		m_Params.Width		= Width;
		m_Params.Height		= Height;
		m_Params.Bits		= Bits;
		m_Params.UseStencil	= UseStencil;
		m_Params.FullScreen	= Fullscreen;
		m_Params.VSync		= VSync;

		return true;
	}

	// SetViewPort
	void RendererGL::SetViewPort(const core::RectI& viewport)
	{
		glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);

	}

	//------------------------------------------------------------------------------------
	// Create a custom vertex declaration and returns its pointer
	//------------------------------------------------------------------------------------
	VertexDec* RendererGL::CreateVertexDeclaration(CustomVertexElement *pVertexInfoArray, core::stringc sName)
	{
		int c = 0;
		std::vector<CustomVertexElement> vCVE;
		while(pVertexInfoArray[c].Stream != 0xFF)
		{
			vCVE.push_back(pVertexInfoArray[c]);
			c++;
		}

		m_vVertexInfoArray.push_back(vCVE);

		VertexDec* pOut	  = KGE_NEW(VertexDec)(sName);
		c = m_vVertexInfoArray.size() - 1;
		int* index = KGE_NEW(int);
		*index = c;
		pOut->m_VertexDec = (void*)index;

		AddVertexDec(pOut);

		return pOut;

	} // CreateVertexDeclaration

	//------------------------------------------------------------------------------------
	// Sets the vertex buffer for multi streaming and rendering
	//------------------------------------------------------------------------------------
	void RendererGL::SetVertexBuffer( HardwareBuffer* pBuffer, int stage )
	{
		if (!pBuffer)
		{
			return;
		}

		if (m_nVertexBufferID[stage] == pBuffer->GetID())
			return;

		m_nVertexBufferID[stage] = pBuffer->GetID();

		// bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, ((VertexBufferGL*)pBuffer)->m_uID);

		// Set the vertex buffer
		std::vector<CustomVertexElement>* p = &m_vVertexInfoArray[*(int*)m_pCurrentVD->m_VertexDec];

		for (uint i = 0; i < p->size(); i++)
		{
			if (p->at(i).Stream != stage)
				continue;

			// find the data type
			GLenum	datatype = GL_FLOAT;
			int		datasize = 0;

			switch (p->at(i).Type)
			{
			case EVET_Float1:
				datasize = 1;
				break;

			case EVET_Float2:
				datasize = 2;
				break;

			case EVET_Float3:
				datasize = 3;
				break;

			case EVET_Float4:
				datasize = 4;
				break;

			case EVET_Color:
				datatype = GL_UNSIGNED_BYTE;
				datasize = 4;
				break;

			}

			// find the usage and set the data
			switch (p->at(i).Usage)
			{
			case EVEU_Position:
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(datasize, datatype, pBuffer->GetStride(), BUFFER_OFFSET(p->at(i).Offset));
				break;

			case EVEU_TexCoord:
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glTexCoordPointer(datasize, datatype, pBuffer->GetStride(), BUFFER_OFFSET(p->at(i).Offset));
				break;

			case EVEU_Color:
				glEnableClientState(GL_COLOR_ARRAY);
				glColorPointer(datasize, datatype, pBuffer->GetStride(), BUFFER_OFFSET(p->at(i).Offset));
				break;
			}
		}

	} // SetVertexBuffer

	//------------------------------------------------------------------------------------
	// Sets the index buffer for rendering
	//------------------------------------------------------------------------------------
	void RendererGL::SetIndexBuffer( HardwareBuffer* pBuffer )
	{
		if (!pBuffer)
		{
			return;
		}

		if (m_nIndexBufferID == pBuffer->GetID())
			return;

		m_nIndexBufferID = pBuffer->GetID();

		// bind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ((IndexBufferGL*)pBuffer)->m_uID);

		m_iIndexBufferSize = pBuffer->GetStride();
		if (m_iIndexBufferSize == 2)
			m_iIndexBufferType = GL_UNSIGNED_SHORT;
		else 
			m_iIndexBufferType = GL_UNSIGNED_INT;
	}

	//------------------------------------------------------------------------------------
	// Creates an Index buffer on video memory
	//------------------------------------------------------------------------------------
	HardwareBuffer* RendererGL::CreateIndexBuffer( void* Indices, u32 ICount, 
		IndexBufferType eIndexBufferType /*= EIBT_16Bit*/, bool isDynamic /*= false*/ )
	{
		// Create a new buffer
		GLuint  bufID;
		glGenBuffers(1, &bufID);

		// Is dynamic?
		GLenum dyn = GL_STATIC_DRAW;
		if (isDynamic)
			dyn = GL_DYNAMIC_DRAW;

		// find index stride
		int Stride = 2;
		if (eIndexBufferType == EIBT_32Bit)
			Stride = 4;

		// bind buffer and pass data to it.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ICount * Stride, Indices, dyn);

		// Create VertexBufferGLvbo
		IndexBufferGL* pIB = KGE_NEW(IndexBufferGL)(ICount, Stride);
		pIB->m_uID = bufID;

		AddHardwareBuffer(pIB);

		return pIB;

	}

	Shader* RendererGL::CreatePixelShaderFromFile( const char* PixelFileName, const char* PixelMain /*= "PSMain"*/, ShaderVersion ePVersion /*= ESV_PS1_1*/ )
	{
		CGprogram CgPixelProgram =
			cgCreateProgramFromFile
			(
			m_CgContext,
			CG_SOURCE,  
			PixelFileName,  
			m_CgFragmentProfile,
			PixelMain, 
			NULL
			);                    /* No extra compiler options */
		checkForCgError("creating pixel program from string");
		if (!CgPixelProgram)
			return NULL;

		cgGLLoadProgram(CgPixelProgram);
		checkForCgError("loading pixel program");

		Shader *shader = KGE_NEW(ShaderGL)(CgPixelProgram, 0, NULL, NULL);
		return shader;

	}

	Shader* RendererGL::CreatePixelShaderFromString( const char* PixelCode, const char* PixelMain /*= "PSMain"*/, ShaderVersion ePVersion /*= ESV_PS1_1*/ )
	{
		CGprogram CgPixelProgram =
			cgCreateProgram
			(
			m_CgContext,
			CG_SOURCE,  
			PixelCode,  
			m_CgFragmentProfile,
			PixelMain, 
			NULL
			);                    /* No extra compiler options */
		checkForCgError("creating pixel program from string");
		if (!CgPixelProgram)
			return NULL;

		cgGLLoadProgram(CgPixelProgram);
		checkForCgError("loading pixel program");

		Shader *shader = KGE_NEW(ShaderGL)(CgPixelProgram, 0, NULL, NULL);
		return shader;

	}

	bool RendererGL::AddVertexShader( Shader** ppOutShader, u32 handle, const char* fileName, const char* mainName, void* extraParam )
	{
		return false;

	}

	bool RendererGL::AddPixelShader( Shader** ppOutShader, u32 handle, const char* fileName, const char* mainName, void* extraParam )
	{
		return false;

	}

	//------------------------------------------------------------------------------------
	// Sets the vertex declaration
	//------------------------------------------------------------------------------------
	void RendererGL::SetVertexDec( VertexDec* pVD )
	{
		m_pCurrentVD = pVD;

	} // SetVertexDeclaration

	void RendererGL::SetRenderingWindow( void* hwnd )
	{

	}

	void RendererGL::OnResize( int width, int height )
	{

	}

	void RendererGL::OnResize(unsigned int width, unsigned int height, bool fullscreen, bool vsync)
	{

	}

	//------------------------------------------------------------------------------------
	// Sets the culling mode
	//------------------------------------------------------------------------------------
	void RendererGL::SetCullingMode( CullMode cm )
	{

	} // SetCullingMode

	//------------------------------------------------------------------------------------
	// Creates a texture from an Image
	//------------------------------------------------------------------------------------
	Texture* RendererGL::CreateTexture( ImageData* pImg )
	{
		Texture* pTex = KGE_NEW(TextureGL)(pImg);

		return pTex;

	} // CreateTexture

	//------------------------------------------------------------------------------------
	// Sets the depth bias
	//------------------------------------------------------------------------------------
	void RendererGL::SetDepthBias( float fBias )
	{

	} // SetDepthBias

	//------------------------------------------------------------------------------------
	// Sets the depth bias slope
	//------------------------------------------------------------------------------------
	void RendererGL::SetDepthBiasSlope( float fSlope )
	{

	} // SetDepthBiasSlope

	//------------------------------------------------------------------------------------
	// Sets the depth test mode.
	//------------------------------------------------------------------------------------
	void RendererGL::SetDepthMode( DepthTestMode e )
	{

	} // SetDepthMode

	//------------------------------------------------------------------------------------
	// Sets the scissor properties
	//------------------------------------------------------------------------------------
	void RendererGL::SetScissorRegion( int x, int y, int width, int height )
	{
		glScissor(x, m_Params.Height - (y + height), width, height);

	} // SetScissorRegion

} // gfx

} // kge
