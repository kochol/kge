#pragma once

#include "kgedef.h"

namespace kge
{
	class KGE_API Settings
	{
		//! Constructor
		Settings();

	public:
		
		//! Destructor
		~Settings();

		//! Returns the singleton pointer
		static Settings* GetPointer();

		uint MaxTextureSize;			//!< Maximum size of the loaded textures. Textures larger than this size will be minimized to this size on load. Default value 4096
		
	}; // Settings

} // kge
