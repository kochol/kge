#pragma once

#include "../../Include/Loader.h"

namespace kge
{
	class LoaderStbImage: public Loader
	{
	public:

		//! Constructor
		LoaderStbImage();

		//! Destructor
		~LoaderStbImage();

		//! Loads a resource from a FileSystem and return its pointer.
		Resource* LoadResource(io::Stream* pStream);

	}; // LoaderStbImage

} // kge
