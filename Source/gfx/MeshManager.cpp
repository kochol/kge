// File name: MeshManager.cpp
// Des: This class is for working with meshes and changing their vertex.
// Date: 15/5/1388
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/gfx/MeshManager.h"



namespace kge
{
	template<> gfx::MeshManager* Singleton<gfx::MeshManager>::m_pSingleton = 0;

namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	MeshManager::MeshManager()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	MeshManager::~MeshManager()
	{

	} // Destructor

} // gfx

} // kge
