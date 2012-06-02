// File name: Resource.cpp
// Des: This class is a base class of resources which will be used in KGE
// Date: 28/05/1388
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../include/Resource.h"
#include "../include/String.h"
#include "../include/KgeMemory.h"

namespace kge
{
	//------------------------------------------------------------------------------------
	//! Empty Constructor
	//------------------------------------------------------------------------------------
	Resource::Resource(): 
		m_iHandle(-1),
		m_pFileName(NULL),
		m_pName(NULL),
		m_pExtraParam(NULL),
		m_bIsLoaded(false),
		m_bLoadOnBackground(false)
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Resource::Resource( const u32 Handle, const char* FileName, const char* Name, void* ExtraParam ):
		m_bIsLoaded(false),
		m_bLoadOnBackground(false)
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
