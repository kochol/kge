#ifndef KGE_ENUMS_H
#define KGE_ENUMS_H

namespace kge
{
	namespace gfx
	{
		//! Renderer API type
		enum RendererAPI
		{
			ERA_DirectX9,
			ERA_OpenGL

		}; // RendererAPI

		enum AntiAliasingMode
		{
			EAAM_Off,
			EAAM_MSAA

		}; // AntiAliasingMode

		//! Texture formats
		enum TextureFormat
		{
			ETF_NONE = 0,
			ETF_A32B32G32R32F,
			ETF_R32F,
			ETF_R16G16F,
			ETF_A8R8G8B8,
			ETF_A8B8G8R8,
			ETF_X8R8G8B8,
			ETF_X8B8G8R8,
			ETF_A8L8,
			ETF_A8,
			ETF_DXT1,				/**< Use DXT1 compress format*/
			ETF_DXT2,				/**< Use DXT2 compress format*/
			ETF_DXT3,				/**< Use DXT3 compress format*/
			ETF_DXT4,				/**< Use DXT4 compress format*/
			ETF_DXT5,				/**< Use DXT5 compress format*/
			ETF_NULL				//!< The NULL texture format is useful when you only want depth buffer

		}; // TextureFormat

		//! Texture formats name
		static const char* TextureFormatName[] = 
		{
			"ETF_NONE",
			"ETF_A32B32G32R32F",
			"ETF_R32F",
			"ETF_R16G16F",
			"ETF_A8R8G8B8",
			"ETF_A8B8G8R8",
			"ETF_X8R8G8B8",
			"ETF_X8B8G8R8",
			"ETF_A8L8",
			"ETF_A8",
			"ETF_DXT1",
			"ETF_DXT2",
			"ETF_DXT3",
			"ETF_DXT4",
			"ETF_DXT5",
			"ETF_NULL"

		}; // TextureFormatName

	} // gfx

	namespace io
	{
		//! Where to print logs
		//enum LogMode
		//{
		//	ELM_Console //!< print logs to console

		//}; // LogMode

		//! What to log
		enum LogLevel
		{
			ELL_EveryThing,		//!< Logs everything
			ELL_Information,	//!< Logs informations, warnings and errors
			ELL_Warning,		//!< Logs warnings and errors
			ELL_Error,			//!< Logs errors
			ELL_Nothing			//!< No logs

		}; // LogLevel

	} // io

} // kge

#endif // KGE_ENUMS_H
