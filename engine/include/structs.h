#ifndef KGE_STRUCTS_H
#define KGE_STRUCTS_H

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
			bits(32),
			AAMode(gfx::EAAM_MSAA),
			AAQuality(2),
			RendererName("d3d9"),
			hwnd(NULL),
			Logmode(io::ELM_Console),
			Loglevel(io::ELL_EveryThing)
		{
		} // Constructor

		u32							Width,			//!< Screen width (default is 800)
									Height;			//!< Screen height (default is 600)
		bool						FullScreen,		//!< FullScreen mode or windowed mode (default is false)
									VSync,			//!< Vertical sync On or Off (default is Off == false)
									UseStencil;		//!< Create and use stencil buffer (default is true)
		u32							bits;			//!< color bits 16, 24, 32 (default is 32)
		gfx::AntiAliasingMode		AAMode;			//!< Anti aliasing technique (default is EAAM_MSAA)
		u32							AAQuality;		//!< Anti aliasing technique 2,4,6,8 (default is 2)
		std::string					RendererName,	//!< Renderer plugin name (default is "d3d9")
									InputMgrName;	//!< InputManager plugin name (ie. OIS)
		void*						hwnd;			//!< External window handle pointer (default is NULL)
		io::LogMode					Logmode;		//!< LogMode (default is ELM_Console)
		io::LogLevel				Loglevel;		//!< LogLevel (default is ELL_EveryThing)

	}; // InitParameters

} // kge

#endif // KGE_STRUCTS_H
