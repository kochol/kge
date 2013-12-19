// File name: Device.h
// Des: This class is the main interface for working with engine
// Date: 10/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef DEVICE_H
#define DEVICE_H

#include "KgeUnknown.h"
#include "io/Iwindow.h"
#include "sn/SceneManager.h"
#include "io/Keyboard.h"
#include "io/Mouse.h"
#include "gfx/Image.h"
#include "ph/PhysManager.h"
#include "av/soundsystem.h"
#include "InitParameter.h"

namespace kge
{
	// P R E   D E F I N E   C L A S S E S
	class PluginManager;
	template <class T>
	class ResourceManager;

namespace gfx
{
	// P R E   D E F I N E   C L A S S E S
	class Renderer;
	class Shader;

} // gfx

namespace gui
{
	class GuiManager;
	class Font;

} // gui

namespace efx
{
	class EffectManager;
}

	enum Options
	{
		EO_SHADOW       = 1,
		EO_REFRACTION   = 2,
		EO_VSYNC        = 4,
	};

	//! This is the main class that user must create and Device will create other parts of engine.
	class KGE_API Device : public KgeUnknown
	{
	public:

		//! Constructor
		Device();

		//! Destructor
		~Device();

		//! Returns the Device class pointer.
		static Device* GetSingletonPtr();

		/*! Inits the engine and its submoduoles
			\param params The parameters for initialzing engine
		*/
		bool Init(InitParameters &params);

		// Run the KGE
		bool Run();

		// Get the Renderer from Device
		gfx::Renderer* GetRenderer();

		//! Sets the rendering window caption
		inline void SetWindowCaption(const char* Text) { m_pWindow->SetCaption(Text); }

		//! Change the mouse cursur type
		inline void SetMouseCursor(kge::io::CURSOR_TYPES cur) { m_pWindow->SetCursor(cur); }


		//! Gets the main scene manager pointer
		sn::SceneManager* GetSceneManager() { return m_pSnMan; }

		//! returns the rendering window.
		io::IWindow* GetWindow();

		//! Returns the GUI manager pointer.
		gui::GuiManager* GetGuiManager() { return m_pGuimgr; }

		//! Return the plugin manager pointer then you can load the plugins for KGE
		PluginManager* GetPluginManager() { return m_pPluginmgr; }

		//! Return the Image resource manager
		ResourceManager<gfx::Image>* GetImageManager() { return m_pImageMan; }

		//! Returns the texture manager.
		ResourceManager<gfx::Texture>* GetTextureManager() { return m_pTextureManager; }

		//! Returns the vertex shader manager.
		ResourceManager<gfx::Shader>* GetVertexShaderManager() { return m_pVertexShaderManager; }

		//! Returns the pixel shader manager.
		ResourceManager<gfx::Shader>* GetPixelShaderManager() { return m_pPixelShaderManager; }

		//! Returns the requested physics engine manager. Only one physics engine can be used at the same time.
		ph::PhysManager* GetPhysManager(ph::PhysicsEngineType enumPhysType = ph::EPET_PhysX);

		//! Returns a pointer to the sound system
		av::SoundSystem* GetSoundSystem() const { return m_pSoundSystem; }

		//! Returns the font manager.
		ResourceManager<gui::Font>* GetFontManager() { return m_pFontManager; }

		//! Returns the effect manager.
		efx::EffectManager* GetEffectManager() { return m_pEffectManager; }

		//! Creates a new window and return its pointer
		io::IWindow* CreateNewWindow();

		InitParameters GetParams() {return m_Params;}

	protected:
		io::IWindow				*	m_pWindow;				// Window
		gfx::Renderer			*	m_pRenderer;			// Renderer
		sn::SceneManager		*	m_pSnMan;				// SceneManager
		gui::GuiManager			*	m_pGuimgr;				// GUI Manager pointer
		PluginManager			*	m_pPluginmgr;			// Plugin Manager pointer
		ph::PhysManager			*	m_pPhysManager;			// Physics manager
		av::SoundSystem			*	m_pSoundSystem;			// Sound system
	
		efx::EffectManager		*	m_pEffectManager;		// Effect manager

		gfx::RendererAPI			m_RA;					// Renderer Type
		InitParameters				m_Params;
			
		ResourceManager
			<gfx::Image>		*	m_pImageMan;			//!< Image managers
		ResourceManager
			<gfx::Texture>		*	m_pTextureManager;		/**< Texture Manager pointer*/
		ResourceManager
			<gfx::Shader>		*	m_pVertexShaderManager; /**< Vertex Shader Manager pointer*/
		ResourceManager
			<gfx::Shader>		*	m_pPixelShaderManager;	/**< Pixel Shader Manager pointer*/
		ResourceManager
			<gui::Font>			*	m_pFontManager;			//!< Font manager

	}; // Device

} // kge

#endif // DEVICE_H