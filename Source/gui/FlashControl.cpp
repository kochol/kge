// File name: FlashControl.cpp
// Des: This file contain flash control plugin setup and flash control class
// Date: 04/08/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/gui/FlashControl.h"

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../Debug/kge.lib")

// end of #ifdef _DEBUG

#else
#pragma comment(lib, "../Release/kge.lib")

#endif // not _DEBUG

#endif // WIN32

extern "C"
{
	//------------------------------------------------------------------------------------
	// Returns the plugin type
	//------------------------------------------------------------------------------------
	KGE_API kge::PluginType KGE_GetPluginType()
	{
		return kge::EPT_GUI;

	} // KGE_GetPluginType

	//------------------------------------------------------------------------------------
	// Init the flash control plugin
	//------------------------------------------------------------------------------------
	KGE_API bool KGE_InitPlugin (kge::Resource** ppOut, kge::Device* pDev, const kge::u32 Handle)
	{
		return false;

	} // KGE_InitPlugin
}
