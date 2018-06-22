// File name: Cartoon.h
// Des: This is the cartoon effect class.
// Date: 3/09/1386
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef CARTOON_H
#define CARTOON_H

#include "Effect.h"
#include "../kgedef.h"

namespace kge
{
namespace efx
{
class KGE_API Cartoon : public Effect
{
	//! Constructor
	Cartoon();
	
	//! Destructor
	virtual ~Cartoon();

	//! Do nothing.
	virtual void AddNodes( sn::SceneNode* pNodes )
	{ /* Nothing todo. */ }

	//! The works must to do before rendering the object.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! The works must to do after rendering the object.
	virtual void Render();

	//! The works must to do after rendering all of the scene nodes.
	virtual void PostRenderEverything();

}; // Cartoon

} // efx

} // kge

#endif // CARTOON_H