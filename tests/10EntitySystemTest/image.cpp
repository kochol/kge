#include "image.h"

//------------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------------
ImageT10::ImageT10()
{
	// create Vertex Buffer and index buffer
	Vertex3 v[] = 
	{
		{-1,-1,0},
		{0,1,0},
		{1,-1,0}
	};
	kge::u16 i[] = {0,2,1};
	VB = pRen->CreateVertexBuffer(v,3,sizeof(Vertex3));
	IB = pRen->CreateIndexBuffer(i, 3);

	// Create vertex declaration
	kge::gfx::CustomVertexElement cve2[] = 
	{
		{
			0,
				0,
				kge::gfx::EVET_Float3,
				kge::gfx::EVEM_Default,
				kge::gfx::EVEU_Position,
				0
		},
		CusVertexEND()
	};
	kge::core::DynamicArray<kge::gfx::CustomVertexElement> cve;
	cve.push_back(cve2[0]);
	cve.push_back(cve2[1]);
	VD = pRen->CreateVertexDeclaration(cve, kge::core::stringc("V3"));

} // Constructor
