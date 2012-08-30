#include "../../engine/include/Device.h"
#include "../../engine/include/Renderer.h"
#include "../../engine/include/math.h"
#include "../../engine/include/VertexElement.h"

#include <gl/GL.h>
#pragma comment(lib, "opengl32.lib")

#if KGE_PLATFORM == KGE_PLATFORM_WINDOWS
	#ifdef _DEBUG
		#pragma comment(lib, "../../bin/debug/kge.lib")
	#else
		#pragma comment(lib, "../../bin/release/kge.lib")
	#endif
#endif

struct Vertex3
{
	float	x,
			y,
			z;

}; // Vertex3

int main()
{
	// Init kge
	kge::Device dev;

	kge::InitParameters params;
	params.RendererName = "ogl";
	dev.Init(params);

	kge::gfx::Renderer*	pRen = dev.GetRenderer();
	if (!pRen)
        return -1;

	// create Vertex Buffer and index buffer
	kge::gfx::HardwareBuffer	*	VB,
								*	IB;
	kge::gfx::VertexDec			*	VD;
	Vertex3 v[] =
	{
		{-1,-1,0},
		{1,-1,0},
		{0,1,0}
	};
	kge::u16 i[] = {0,1,2};
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
	kge::core::stringc vdName("V3");
	VD = pRen->CreateVertexDeclaration(cve, vdName);

	// Create matrices
	kge::math::Matrix mProj;
	mProj.SetPerspectiveLH(params.Width, params.Height, 0.8f, 0.1, 100.0);
	pRen->SetTransForm(&mProj, kge::gfx::ETM_Projection);
	kge::math::Matrix mView;
	kge::math::Vector vPos(0,0,5), vTarget, vUp(0,1,0);
	mView.SetViewLookatLH(vPos, vTarget, vUp);
	pRen->SetTransForm(&mView, kge::gfx::ETM_View);
	pRen->SetTransForm(NULL);

	pRen->SetClearColor(kge::gfx::Color(100,0,100));

	while (dev.Run())
	{
		pRen->BeginRendering(true, true, true);
// 		glBegin(GL_QUADS);
// 		//glColor3f(1.0f, 0.5f, 0.0f);
// 		glVertex2f(0.0f, 0.0f);
// 		glVertex2f(1.0f, 0.0f);
// 		glVertex2f(1.0f, 1.0f);
// 		glVertex2f(0.0f, 1.0f);
// 		glEnd();
		pRen->SetVertexDeclaration(VD);
		pRen->SetVertexBuffer(VB);
		pRen->SetIndexBuffer(IB);
		pRen->DrawTriangleList(3, 3);
		pRen->EndRendering();
	}

	return 0;

} // main
