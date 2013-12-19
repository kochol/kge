// File name: VideoManager.h
// Des: This class add videos and manage them
// Date: 01/09/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_VIDEOMANAGER_H
#define KGE_VIDEOMANAGER_H

#include "../Plugin.h"
#include "../av/video.h"

namespace kge
{
namespace plugin
{
class KGE_API VideoManager : public Plugin
{
public:

	//! Constructor
	VideoManager(Device* pDev, const u32 Handle, char* FileName, char* Name);

	//! Destructor
	~VideoManager();

	Resource* Add();

}; // Video

} // plugin

} // kge

#endif // KGE_VIDEOMANAGER_H

extern "C"
{
	//! Returns the plugin type
	KGE_API kge::PluginType KGE_GetPluginType();

	//! Init the flash control plugin
	KGE_API bool KGE_InitPlugin (kge::Resource** ppOut, kge::Device* pDev, const kge::u32 Handle);

} // extern "C"
