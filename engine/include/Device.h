#ifndef KGE_DEVICE_H
#define KGE_DEVICE_H

#include <string>

#include "kgedef.h"
#include "structs.h"

//! Everything about KGE is here
namespace kge
{
	namespace gfx
	{
		class Renderer;

	} // gfx

	namespace io
	{
		class KgeWindow;
		class Logger;
		class InputManager;

	} // io

	class PluginManager;

	//! This is the main class that user must create and Device will create other parts of engine.
	class KGE_API Device
	{
	public:

		//! Constructor
		Device();

		//! Destructor
		~Device();

		//! Returns the Device object pointer.
		static Device* GetSingletonPtr();

		//! Initialize KGE with given parameters.
		/*! 
			\param params: Initialize parameters
			\return Returns true on success
		*/
		bool Init(InitParameters& params);

		//! Run the KGE
		bool Run();

		//! Get the renderer
		gfx::Renderer* GetRenderer();

	protected:

		PluginManager		*	m_pPluginMgr;	//!< Plugin manager
		io::Logger			*	m_pLogger;		//!< Logger pointer
		io::KgeWindow		*	m_pWindow;		//!< Rendering window
		io::InputManager	*	m_pInputMgr;	//!< InputManager pointer

	}; // Device

} // kge

#endif // KGE_DEVICE_H
