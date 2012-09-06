#ifndef KGE_PLUGIN_H
#define KGE_PLUGIN_H

namespace kge
{
	class Device;

	enum PluginType
	{
		EPT_Renderer = 0,		/**< Render system plugin */
		EPT_InputManager,		//!< InputManager plugin
		EPT_Loader,		//!< Texture loader plugin

		EPT_Unknown				//!< Unknown plugin type

	}; // PluginType

	//! This class is a base class for plugin which will be used in KGE
	template <typename T>
	class KGE_API Plugin
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

		//! Create the plugin
		virtual T* Create() = 0;

	protected:
		PluginType		m_ePluginType;		//!< Plugin type

	}; // Plugin

} // kge

#endif // KGE_PLUGIN_H
