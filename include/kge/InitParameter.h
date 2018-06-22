// File name: InitParameter.h
// Des: This struct contains the parameter that device need to know to init Kochol Game Engine
// Date: 01/08/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_INITPARAMETER_H
#define KGE_INITPARAMETER_H

#include "kgedef.h"
#include "enums.h"
#include <string>

#ifndef NULL
#define NULL 0
#endif 

namespace kge
{

	//! The parameters that is used for initialize the KGE
	struct InitParameters
	{
		//! Constructor for setting default values
		InitParameters():
			Width(800),
			Height(600),
			FullScreen(false),
			VSync(false),
			UseStencil(true),
			Bits(32),
			AAMode(gfx::EAAM_MSAA),
			AAQuality(2),
			RendererName("d3d9"),
			RendererType(gfx::ERA_DirectX9),
			hwnd(NULL),
//			Logmode(io::ELM_Console),
			Loglevel(io::ELL_EveryThing),
			MaxTextureSize(4096)
		{
		} // Constructor

		u32							Width,			//!< Screen width (default is 800)
									Height,			//!< Screen height (default is 600)
									MaxTextureSize;	//!< The maximum texture size that engine can allow to load. The bigger texture sizes will be scaled down to this size automatically. (default is 4096)
		bool						FullScreen,		//!< FullScreen mode or windowed mode (default is false)
									VSync,			//!< Vertical sync On or Off (default is Off == false)
									UseStencil;		//!< Create and use stencil buffer (default is true)
		u32							Bits;			//!< color bits 16, 24, 32 (default is 32)
		gfx::AntiAliasingMode		AAMode;			//!< Anti aliasing technique (default is EAAM_MSAA)
		u32							AAQuality;		//!< Anti aliasing technique 2,4,6,8 (default is 2)
		gfx::RendererAPI			RendererType;	//!< RendererType (default is ERA_DirectX9)
		std::string					RendererName,	//!< Renderer plugin name (default is "d3d9")
									InputMgrName;	//!< InputManager plugin name (ie. OIS)
		void*						hwnd;			//!< External window handle pointer (default is NULL)
//		io::LogMode					Logmode;		//!< LogMode (default is ELM_Console)
		io::LogLevel				Loglevel;		//!< LogLevel (default is ELL_EveryThing)

	}; // InitParameters

} // kge

#endif // KGE_INITPARAMETER_H

