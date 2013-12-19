// File name: VideoManager.h
// Des: This class add videos and manage them
// Date: 01/09/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/plugin/VideoManager.h"
#include <dshow.h>

#ifdef WIN32
#ifdef _DEBUG
#pragma comment(lib, "../Debug/kge.lib")

// end of #ifdef _DEBUG

#else
#pragma comment(lib, "../Release/kge.lib")

#endif // not _DEBUG

#endif // WIN32

namespace kge
{
namespace plugin
{
	// Constructor
	VideoManager::VideoManager(Device* pDev, const u32 Handle, char* FileName, char* Name):
		Plugin(pDev, Handle, FileName, Name)
	{
		io::Logger::Log("*** VideoManager Constructor.");

	} // Constructor

	// Destructor
	VideoManager::~VideoManager()
	{
		CoUninitialize();

	} // Destructor

	Resource* VideoManager::Add()
	{
		return new av::Video(m_pDev, 1, NULL, NULL);
	}

} // plugin

} // kge

//------------------------------------------------------------------------------------
// Returns the plugin type
//------------------------------------------------------------------------------------
KGE_API kge::PluginType KGE_GetPluginType()
{
	return kge::EPT_AV;

} // KGE_GetPluginType

//------------------------------------------------------------------------------------
// Init the flash control plugin
//------------------------------------------------------------------------------------
KGE_API bool KGE_InitPlugin (kge::Resource** ppOut, kge::Device* pDev, const kge::u32 Handle)
{
    CoInitialize(0);
	*ppOut = new kge::plugin::VideoManager(pDev, Handle, "Video", NULL);

	return true;

} // KGE_InitPlugin
