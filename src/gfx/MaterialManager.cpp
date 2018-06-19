// File name: MaterialManager.cpp
// Des: The Material Manager class.
// Date: 28/4/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../include/kge/gfx/MaterialManager.h"
#include "../../include/kge/gfx/Renderer.h"



namespace kge
{
	template<> gfx::MaterialManager* Singleton<gfx::MaterialManager>::m_pSingleton = 0;

namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	MaterialManager::MaterialManager()
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	MaterialManager::~MaterialManager()
	{
		for (std::vector<MaterialParams*>::iterator it = m_aMatParams.begin();
			it != m_aMatParams.end(); ++it)
		{
			KGE_DELETE(*it, MaterialParams);
		}

	} // Destructor

	//------------------------------------------------------------------------------------
	// Search for the MaterialParams you passed.
	//------------------------------------------------------------------------------------
	MaterialParams* MaterialManager::GetMaterialParam( MaterialParams* other )
	{
		for (std::vector<MaterialParams*>::iterator it = m_aMatParams.begin();
			it != m_aMatParams.end(); ++it)
		{
			if ((*it)->HasSameVertexShader(other) && (*it)->HasSamePixelShader(other))
			{
				return *it;
			}
		}
		MaterialParams* mp = KGE_NEW(MaterialParams);
		*mp = *other;
		// Check for deferred rendering
		if (Renderer::GetSingletonPtr()->GetRendererPipeline() == ERP_Deferred)
		{
			mp->Deferred = true;
			other->Deferred = true;
		}
		mp->CreateVertexShaderCodeHLSL();
		mp->CreatePixelShaderCodeHLSL();
		m_aMatParams.push_back(mp);

		return mp;

	} // GetMaterialParam

	//------------------------------------------------------------------------------------
	// returns the MaterialManager
	//------------------------------------------------------------------------------------
	kge::gfx::MaterialManager MaterialManager::GetSingleton()
	{
		return *m_pSingleton; 

	} // GetSingleton

	//------------------------------------------------------------------------------------
	// returns the MaterialManager pointer
	//------------------------------------------------------------------------------------
	MaterialManager* MaterialManager::GetSingletonPtr()
	{
		return m_pSingleton;

	} // GetSingletonPtr

} // gfx

} // kge
