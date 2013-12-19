// File name: Image.h
// Des: This file is the image control.
// Date: 7/1/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef KGE_IMAGE_H
#define KGE_IMAGE_H

#include "Control.h"

namespace kge
{
namespace gui
{
	//! Image control
	class KGE_API Image: public Control
	{

	public:

		//! Constructor
		Image(u16* pIndices,
			  core::RectI rect,
			  gfx::Renderer* pRenderer);

		//! Destructor
		~Image();

		//! Render the control
		virtual void Render();

		// Set custom UV
		virtual void SetUV(float u1, float v1, float u2, float v2);

		kge::gfx::Vertex3T* m_pVerts;

	protected:

		// Updates the control properties.
		virtual void Update();

	};// Image

} // gui

} // kge

#endif // KGE_IMAGE_H
