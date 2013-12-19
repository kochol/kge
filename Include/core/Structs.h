// File name: structs.h
// Des: in file tamame struct hayee ke dar namespace core hastand ra dar khod negah 
//		midarad.
// Date: 27/5/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef CORE_STRUCTS_H
#define CORE_STRUCTS_H

#include "../kgedef.h"

namespace kge
{
namespace core
{
//! RECT
/** A rect struct for using in viewports */
template <typename T>
struct Rect
{
	Rect() {}
	Rect(T iX, T iY, T iW, T iH)
	{
		X = iX;
		Y = iY;
		Width = iW;
		Height = iH;
	}

	T X;		 /**< X */
	T Y;		 /**< Y */
	T Width;  /**< Width */
	T Height; /**< Height */

}; // Rect

typedef Rect<int>  RectI;
typedef Rect<ul32> RectUL;

} // core

} // kge

#endif // CORE_STRUCTS_H