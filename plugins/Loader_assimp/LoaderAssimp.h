#pragma once

#include "../../include/kge/Loader.h"

namespace kge
{
	class LoaderAssimp: public Loader
	{
	public:

		//! Constructor
		LoaderAssimp();

		//! Destructor
		~LoaderAssimp();

		//! Loads a resource from a FileSystem and return its pointer.
		Resource* LoadResource(io::Stream* pStream);

	}; // LoaderAssimp

} // kge
