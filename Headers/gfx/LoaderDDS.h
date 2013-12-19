#ifndef KGE_LOADERDDS_H
#define KGE_LOADERDDS_H

#include "../../Include/Loader.h"

namespace kge
{
	class LoaderDDS: public Loader
	{
	public:

		//! Constructor
		LoaderDDS();

		//! Destructor
		~LoaderDDS();

		//! Loads a resource from a FileSystem and return its pointer.
		Resource* LoadResource(io::Stream* pStream);

	};

}

#endif // KGE_LOADERDDS_H
