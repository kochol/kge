// File name: Resource.cpp
// Des: This class is a base class of resources which will be used in KGE
// Date: 28/05/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../include/kge/Resource.h"
#include "../include/kge/core/String.h"  
#include "../include/kge/core/KgeMemory.h"
#include "../include/kge/io/Logger.h"

namespace kge
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Resource::Resource( const u32 Handle, const char* FileName, const char* Name, void* ExtraParam )
	{
		m_iHandle	  = Handle;
		m_pFileName   = core::String::StringCopy(FileName);
		m_pName		  = core::String::StringCopy(Name);
		m_pExtraParam = ExtraParam;

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Resource::~Resource()
	{
		KGE_DELETE_ARRAY(m_pFileName);
		KGE_DELETE_ARRAY(m_pName);

	} // Destructor

} // kge
