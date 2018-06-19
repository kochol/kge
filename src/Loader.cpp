#include "../include/kge/Loader.h"

namespace kge
{

	//------------------------------------------------------------------------------------
	// returns true if the file maybe is able to be loaded by this Loader
	//------------------------------------------------------------------------------------
	bool Loader::IsALoadableFileExtension( core::String& strFilename )
	{
		for (u32 i = 0; i < m_aFileExtension.size(); i++)
		{
			if (strFilename.FindLast(m_aFileExtension[i].c_str()) >= 0)
				return true;
		}

		return false;

	} // IsALoadableFileExtension

} // kge
