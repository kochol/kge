#include "../include/Loader.h"

namespace kge
{

	//------------------------------------------------------------------------------------
	// returns true if the file maybe is able to be loaded by this Loader
	//------------------------------------------------------------------------------------
	bool Loader::IsALoadableFileExtension( const core::stringc& strFilename )
	{
		core::stringc::size_type se, st = strFilename.size();
		for (u32 i = 0; i < m_aFileExtension.size(); i++)
		{
			se = m_aFileExtension[i].size();
			if (strFilename.compare(st - se, se, m_aFileExtension[i]) == 0)
				return true;
		}

		return false;

	} // IsALoadableFileExtension

} // kge
