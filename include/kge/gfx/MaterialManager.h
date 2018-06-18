// File name: MaterialManager.h
// Des: The Material Manager class.
// Date: 28/4/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include "../Singleton.h"
#include "MaterialParams.h"
#include <vector>

namespace kge
{
namespace gfx
{
class KGE_API MaterialManager : public Singleton<MaterialManager>
{
public:

	//! Constructor
	MaterialManager();

	//! Destructor
	~MaterialManager();

	//! Search for the MaterialParams you passed.
	MaterialParams* GetMaterialParam(MaterialParams* other);

	//! returns the MaterialManager
	static MaterialManager GetSingleton();

	//! returns the MaterialManager pointer
	static MaterialManager* GetSingletonPtr();

protected:

	//! The array of created materials
	std::vector<MaterialParams*>		m_aMatParams;

}; // MaterialManager

} // gfx

} // kge

#endif // MATERIALMANAGER_H
