#ifndef T10_IMAGE_H
#define T10_IMAGE_H

#include "../../engine/include/Renderer.h"
#include "../../engine/include/math.h"
#include "../../engine/include/VertexElement.h"

class kge::gfx::HardwareBuffer;
class kge::gfx::VertexDec;

struct Vertex3 
{
	float	x,
			y,
			z;

}; // Vertex3

class ImageT10
{
public:

	kge::gfx::HardwareBuffer	*	VB,
								*	IB;
	kge::gfx::VertexDec			*	VD;

	//! Constructor
	ImageT10();

protected:

private:

};

#endif // T10_IMAGE_H
