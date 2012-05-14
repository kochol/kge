#ifndef KGE_PLUGINMANAGER_H
#define KGE_PLUGINMANAGER_H

#include <vector>

#include "kgedef.h"
#include "Plugin.h"

namespace kge
{
	class Device;

	namespace gfx
	{
		class Renderer;

	} // gfx

	namespace io
	{
		class InputManager;

	} // io

	typedef Plugin<gfx::Renderer>		RendererPlugin;
	typedef Plugin<io::InputManager>	InputMgrPlugin;

	//! Loads plugins and manage them
	class KGE_API PluginManager
	{
	public:
	
		//! Destructor
		virtual ~PluginManager();

		/*! Loads a plugin from a .dll or .so file.
		 */
		int LoadPlugin(const char* name);

		//! Returns the PluginManager singleton pointer
		static PluginManager*	GetSingletonPtr();

		//! Release all loaded plugin from memory
		void Release();

		//------------------------------------------------------------------------------------
		//  R E N D E R E R    P L U G I N S 
		//------------------------------------------------------------------------------------

		//! Returns the count of Renderer plugins that loaded in engine
		/*!
			\return Returns the count of Renderer plugins that loaded in engine
			\sa GetRendererPlugin()
		 */
		int GetRendererPluginsCount() { return m_vRendererPlugins.size(); }

		//! Get renderer plugin with its index
		/*!
			\param index The index of registered renderer plugins.
			\return Returns the requested renderer plugin
			\sa GetRendererPluginsCount()
		 */
		RendererPlugin* GetRendererPlugin(int index) { return m_vRendererPlugins[index]; }

		/*! With this interface a Renderer Plugin will register himself with PluginManager.
		 */
		int RegisterRenderer(RendererPlugin* pRenPlug);

		//------------------------------------------------------------------------------------
		//   I N P U T    M A N A G E R     P L U G I N S
		//------------------------------------------------------------------------------------

		//! Returns the count of InputManager plugins that loaded in engine
		/*!
			\return Returns the count of InputManager plugins that loaded in engine
			\sa GetInputManagerPlugin()
		 */
		int GetInputManagerPluginCount() { return m_vInputMgrPlugins.size(); }

		//! Get InputManager plugin with its index
		/*!
			\param index The index of registered InputManager plugin.
			\return Returns the requested InputManager plugin
			\sa GetInputManagerPluginCount()
		 */
		InputMgrPlugin* GetInputManagerPlugin(int index) { return m_vInputMgrPlugins[index]; }

		/*! With this interface an InputManager Plugin will register himself with PluginManager.
		 */
		int RegisterInputManager(InputMgrPlugin* pInputMgrPlug);

	protected:

		//! Protected Constructor
		PluginManager();

		std::vector<RendererPlugin*>	m_vRendererPlugins;	//!< Renderer plugins list
		std::vector<InputMgrPlugin*>	m_vInputMgrPlugins;	//!< Input manager plugins list

	}; // PluginManager

} // kge

#endif // KGE_PLUGINMANAGER_H
