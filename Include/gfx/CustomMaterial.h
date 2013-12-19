// File name: CustomMaterial.h
// Des: The custom Material part class.
// Date: 28/4/1389
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_CUSTOMMATERIAL_H
#define KGE_CUSTOMMATERIAL_H

#include "../Singleton.h"
#include "../KgeUnknown.h"

namespace kge
{
namespace gfx
{
class MaterialParams;
class ShaderInstance;

enum CustomMaterialType
{
	ECMT_Unknown,
	ECMT_Lighting,
	ECMT_Tree,
	ECMT_PhongLighting

};

enum VertexShaderEntry
{
	EVSE_Unknown = 0,
	EVSE_GetVertexShaderHandles,
	EVSE_ChangePosition,
	EVSE_Lighting
};

enum PixelShaderEntry
{
	EPSE_Start = 0,
	EPSE_Lighting,
	EPSE_GetPixelShaderHandles
};

//! Custom material base class
class CustomMaterial : public KgeUnknown
{
public:

	//! Constructor
	CustomMaterial();

	//! Destructor
	virtual ~CustomMaterial();

	//! Changes the material param shader codes.
	virtual bool CreateVertexShaderCode(MaterialParams* pMP, VertexShaderEntry ese = EVSE_Unknown) = 0;

	virtual bool CreatePixelShaderCode(MaterialParams* pMP, PixelShaderEntry ese) = 0;

	virtual void SetShaderConstants( ShaderInstance* pSI ) { }

	virtual CustomMaterial* GetCopy() {	AddRef(); return this; }

	CustomMaterialType GetType() {return m_eType;}

protected:

	CustomMaterialType			m_eType;

}; // CustomMaterial

//! Simple lighting
class SimpleLightingCM : public CustomMaterial
{
public:
	
	SimpleLightingCM() 
	{m_eType = ECMT_Lighting;}

	static SimpleLightingCM* GetSingletonPtr();

	//! Adds simple lighting vertex shader code
	virtual bool CreateVertexShaderCode(MaterialParams* pMP, VertexShaderEntry ese );

	//! Adds simple lighting pixel shader code
	virtual bool CreatePixelShaderCode(MaterialParams* pMP, PixelShaderEntry ese);

}; // SimpleLightingCM

} // gfx

} // kge

#endif // KGE_CUSTOMMATERIAL_H
