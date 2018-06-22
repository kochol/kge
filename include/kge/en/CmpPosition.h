#pragma once

#include "Component.h"
#include "../kgedef.h"

namespace kge
{
namespace en
{
class KGELIB_API CmpPosition : public Component
{
public:

	//! Default constructor
	CmpPosition(): x(0.0f), y(0.0f), z(0.0f) {}

	//! Constructor
	CmpPosition(float X, float Y, float Z): x(X), y(Y), z(Z) {}

	//! Every component must declare this function for returning their class ID
	int GetClassID();

	//! Every component must declare this function for returning their class name
	std::string GetClassName();

	//! Clone a component
	virtual Component* Clone();


	float					x;			//!< X position
	float					y;			//!< Y position
	float					z;			//!< Z position
	
}; // CmpPosition

} // en

} // kge
