// File name: Color.h
// Des: in file dar bar girande Rangha hast va daraye tavabe ee baraye asan tar shodan
//      kar ba rangha bashad. dar sakht in class az irrlicht komak gerefte shode ast.
// Date: 22/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef COLOR_H
#define COLOR_H

#include "../kgedef.h"

namespace kge
{
namespace gfx
{
//! Color class
class Color
{
public:

	//! Sazande khali ke hich kari anjam nemide.
	Color() : c(0xffffffff) {}

	//! Sazande ba se rang Ghermez va Sabz va Abi.
	/*!
	  \param Red Set kardane range ghermez 0 ta 255.
	  \param Green Set kardane range sabz 0 ta 255.
	  \param Blue Set kardane range Abi 0 ta 255.
	 */
	Color(int Red, int Green, int Blue)
		: c(((255 & 0xff)<<24) | ((Red & 0xff)<<16) | ((Green & 0xff)<<8) 
			| (Blue & 0xff))	{} // Color(r,g,b)

	void Set(int Red, int Green, int Blue, int Alpha)
	{
		c = ((Alpha & 0xff)<<24) | ((Red & 0xff)<<16) | ((Green & 0xff)<<8) 
			| (Blue & 0xff);
	}
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

//! In class rang ba 4 float hast.
class Colorf
{
public:

	//! Sazande (Hich kari Anjam nemidahad.)
	Colorf():r(1.0f),g(1.0f),b(1.0f),a(1.0f) {}

	Colorf(float R, float G, float B, float A) : r(R), g(G), b(B), a(A) {}

	//! Sazande ba 3 ta int (RGB) ke bayad 0 ta 255 bashand.
	/*!
	  \param Red Set kardane range ghermez 0 ta 255.
	  \param Green Set kardane range sabz 0 ta 255.
	  \param Blue Set kardane range Abi 0 ta 255.
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

#endif // COLOR_H