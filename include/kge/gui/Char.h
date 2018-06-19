// File name: Char.h
// Des: Class for bitmap fonts.
// Date: 28/07/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef KGE_CHAR_H
#define KGE_CHAR_H

#include "../../Include/kgedef.h"

namespace kge
{
namespace gui
{
class Char
{
public:

	//! Constructor
	Char() : ID(0), x(0), y(0), width(0), height(0), xOffset(0), yOffset(0), xAdvance(0)
	{
	}

	//! Destructor
	~Char() { }

	int ID,
		x,
		y,
		width,
		height,
		xOffset,
		yOffset,
		xAdvance;

}; // Char

} // gui

} // kge

#endif // KGE_CHAR_H
