#ifndef KGE_COLOR_H
#define KGE_COLOR_H

#include "kgedef.h"

namespace kge
{
namespace gfx
{
//! Color class
class Color
{
public:

	//! Constructor
	Color() : c(0xff000000) {}

	//! Constructor
	/*!
	  \param Red Sets the red color 0 to 255.
	  \param Green Sets the green color 0 to 255.
	  \param Blue Sets the blue color 0 to 255.
	 */
	Color(int Red, int Green, int Blue)
		: c(((255 & 0xff)<<24) | ((Red & 0xff)<<16) | ((Green & 0xff)<<8) 
			| (Blue & 0xff))	{} // Color(r,g,b)

	//! Converts color to OpenGL color format.
	//! \return Returns the 32 bit OpenGL color value (RGBA)
	inline s32 toOpenGLColor() const 
	{
		return (((c>>24) & 0xff)<<24) |
				(((c)& 0xff)<<16) |
				((c>>8 & 0xff)<<8) |
				((c>>16) & 0xff);
	};

	//! Returns the alpha component of the color. The alpha component
	//! defines how transparent a color should be.
	//! 0 means not transparent, 255 means fully transparent.
	inline s32 getAlpha() const	{	return (c>>24) & 0xff;	}

	//! Returns the red component of the color. 
	//! \return Returns a value between 0 and 255, specifying how red the color is.
	//! 0 means dark, 255 means full red.
	inline s32 getRed() const	{	return c & 0xff;	}

	//! Returns the green component of the color. 
	//! \return Returns a value between 0 and 255, specifying how green the color is.
	//! 0 means dark, 255 means full green.
	inline s32 getGreen() const	{	return (c>>16) & 0xff;	}

	//! Returns the blue component of the color. 
	//! \return Returns a value between 0 and 255, specifying how blue the color is.
	//! 0 means dark, 255 means full blue.
	inline s32 getBlue() const	{	return (c>>8) & 0xff;	}

	//! A8R8G8B8 color format
	s32 c;

}; // Color

//! Color class that contain 4 float for storing RGBA
class Colorf
{
public:

	//! Constructor
	Colorf():r(1.0f),g(1.0f),b(1.0f),a(1.0f) {}

	//! Constructor
	Colorf(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}

	//! Constructor
	/*!
		\param Red Sets the red color 0 to 255.
		\param Green Sets the green color 0 to 255.
		\param Blue Sets the blue color 0 to 255.
	 */
	Colorf(int Red, int Green, int Blue)
	{
		r = (float)Red / 255.0f;
		g = (float)Green / 255.0f;
		b = (float)Blue / 255.0f;
		a = 1.0f;

	} // Colorf(int r,g,b)

	inline Colorf operator*(float f) const
	{
		Colorf c;
		c.r = r * f;
		c.g = g * f;
		c.b = b * f;
		c.a = a;
		return c;

	} // Colorf*float

	inline void Set(float R, float G, float B)
	{
		r = R;
		g = G;
		b = B;
	}

	void Set(float R, float G, float B, float A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	union
	{
		float c[4];
		struct
		{
			float r,	//! Red
				  g,	//! Green
				  b,	//! Blue
				  a;	//! Alpha
		};
	}; 

}; // Colorf

} // gfx

} // kge

#endif // KGE_COLOR_H