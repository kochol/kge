#ifndef KGE_LOADER_H
#define KGE_LOADER_H

#include "kgedef.h"
#include "String.h"
#include <vector>

namespace kge
{
	//   F O R W A R D   D E C L A R A T I O N
	class Resource;

	namespace io
	{
		class Stream;

	} // io

	//! This class is an interface for resource loaders in KGE
	class KGE_API Loader
	{
	public:

		//! Constructor
		Loader(): m_bSwapEndian(false)
		{}

		//! Destructor
		virtual ~Loader() {}

		//! returns true if the file maybe is able to be loaded by this Loader
		//! based on the file extension (e.g. ".mesh")
		virtual bool IsALoadableFileExtension(const core::stringc& strFilename);

		//! Loads a resource from a FileSystem and return its pointer.
		/*!
			\param strFilename The resource file name to load.
			\return Returns the created resource pointer. Note resource may not loaded yet.
			\sa kge::io::FileSystemManager, Resource::IsLoaded
		 */
		virtual Resource* LoadResource(io::Stream* pStream) = 0;

	protected:

		std::vector<core::stringc>	        m_aFileExtension;		//!< The file extension list that this loader is capable to load
		bool								m_bSwapEndian;			//!< Swap the loaded data or not

	}; // Loader

} // kge

#endif // KGE_LOADER_H
