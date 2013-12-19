// File name: Plugin.h
// Des: This class is a base class for plugin which will be used in KGE
// Date: 04/08/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_PLUGIN_H
#define KGE_PLUGIN_H

namespace kge
{
	class Device;

	enum PluginType
	{
		EPT_Renderer = 0,		/**< Render system plugin */
		EPT_InputManager,		//!< InputManager plugin
		EPT_TextureLoader,		//!< Texture loader plugin

		EPT_Unknown				//!< Unknown plugin type

	}; // PluginType

	//! This class is a base class for plugin which will be used in KGE
	template <typename T>
	class Plugin
	{
	public:

		//! Constructor
		Plugin(): m_ePluginType(EPT_Unknown) {}

		//! Destructor
		virtual ~Plugin() {}

		//! Returns plugin type
		PluginType GetPluginType() {return m_ePluginType;}

		//! Returns the engine major version that plugin compiled with
		int GetEngineMajorVersion() {return KGE_VERSION_MAJOR;}

		//! Returns the engine minor version that plugin compiled with
		int GetEngineMinorVersion() {return KGE_VERSION_MINOR;}

		//! Returns the engine release version that plugin compiled with
		int GetEnginePatchVersion() {return KGE_VERSION_RELEASE;}

		//! Create the plugin
		virtual T* Create() = 0;

	protected:
		PluginType		m_ePluginType;		//!< Plugin type

	}; // Plugin

} // kge

#endif // KGE_PLUGIN_H
