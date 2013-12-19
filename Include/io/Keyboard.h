// File name: Keyboard.h
// Des: In class Keyboard hast.
// Date: 3/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol), Pooya Shahinfar(PSWIN) , Hadi Robati(hadirobati)

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../KgeUnknown.h"
#include "../kgedef.h"
#include "../math/Vector.h"
#include "../core/Functor.h"
#include "../gui/Events.h"
#include <vector>

namespace kge
{
namespace io
{
enum KEYS
{
	/* The keyboard syms have been cleverly chosen to map to ASCII */
	EK_UNKNOWN		= 0,
	EK_FIRST		= 0,
	EK_BACKSPACE		= 8,
	EK_TAB		= 9,
	EK_CLEAR		= 12,
	EK_RETURN		= 13,
	EK_PAUSE		= 19,
	EK_ESCAPE		= 27,
	EK_SPACE		= 32,
	EK_EXCLAIM		= 33,
	EK_QUOTEDBL		= 34,
	EK_HASH		= 35,
	EK_DOLLAR		= 36,
	EK_AMPERSAND		= 38,
	EK_QUOTE		= 39,
	EK_LEFTPAREN		= 40,
	EK_RIGHTPAREN		= 41,
	EK_ASTERISK		= 42,
	EK_PLUS		= 43,
	EK_COMMA		= 44,
	EK_MINUS		= 45,
	EK_PERIOD		= 46,
	EK_SLASH		= 47,
	EK_0			= 48,
	EK_1			= 49,
	EK_2			= 50,
	EK_3			= 51,
	EK_4			= 52,
	EK_5			= 53,
	EK_6			= 54,
	EK_7			= 55,
	EK_8			= 56,
	EK_9			= 57,
	EK_COLON		= 58,
	EK_SEMICOLON		= 59,
	EK_LESS		= 60,
	EK_EQUALS		= 61,
	EK_GREATER		= 62,
	EK_QUESTION		= 63,
	EK_AT			= 64,
	/* 
	   Skip uppercase letters
	 */
	EK_LEFTBRACKET	= 91,
	EK_BACKSLASH		= 92,
	EK_RIGHTBRACKET	= 93,
	EK_CARET		= 94,
	EK_UNDERSCORE		= 95,
	EK_BACKQUOTE		= 96,
	EK_a			= 97,
	EK_b			= 98,
	EK_c			= 99,
	EK_d			= 100,
	EK_e			= 101,
	EK_f			= 102,
	EK_g			= 103,
	EK_h			= 104,
	EK_i			= 105,
	EK_j			= 106,
	EK_k			= 107,
	EK_l			= 108,
	EK_m			= 109,
	EK_n			= 110,
	EK_o			= 111,
	EK_p			= 112,
	EK_q			= 113,
	EK_r			= 114,
	EK_s			= 115,
	EK_t			= 116,
	EK_u			= 117,
	EK_v			= 118,
	EK_w			= 119,
	EK_x			= 120,
	EK_y			= 121,
	EK_z			= 122,
	EK_DELETE		= 127,
	/* End of ASCII mapped keysyms */

	/* International keyboard syms */
	EK_WORLD_0		= 160,		/* 0xA0 */
	EK_WORLD_1		= 161,
	EK_WORLD_2		= 162,
	EK_WORLD_3		= 163,
	EK_WORLD_4		= 164,
	EK_WORLD_5		= 165,
	EK_WORLD_6		= 166,
	EK_WORLD_7		= 167,
	EK_WORLD_8		= 168,
	EK_WORLD_9		= 169,
	EK_WORLD_10		= 170,
	EK_WORLD_11		= 171,
	EK_WORLD_12		= 172,
	EK_WORLD_13		= 173,
	EK_WORLD_14		= 174,
	EK_WORLD_15		= 175,
	EK_WORLD_16		= 176,
	EK_WORLD_17		= 177,
	EK_WORLD_18		= 178,
	EK_WORLD_19		= 179,
	EK_WORLD_20		= 180,
	EK_WORLD_21		= 181,
	EK_WORLD_22		= 182,
	EK_WORLD_23		= 183,
	EK_WORLD_24		= 184,
	EK_WORLD_25		= 185,
	EK_WORLD_26		= 186,
	EK_WORLD_27		= 187,
	EK_WORLD_28		= 188,
	EK_WORLD_29		= 189,
	EK_WORLD_30		= 190,
	EK_WORLD_31		= 191,
	EK_WORLD_32		= 192,
	EK_WORLD_33		= 193,
	EK_WORLD_34		= 194,
	EK_WORLD_35		= 195,
	EK_WORLD_36		= 196,
	EK_WORLD_37		= 197,
	EK_WORLD_38		= 198,
	EK_WORLD_39		= 199,
	EK_WORLD_40		= 200,
	EK_WORLD_41		= 201,
	EK_WORLD_42		= 202,
	EK_WORLD_43		= 203,
	EK_WORLD_44		= 204,
	EK_WORLD_45		= 205,
	EK_WORLD_46		= 206,
	EK_WORLD_47		= 207,
	EK_WORLD_48		= 208,
	EK_WORLD_49		= 209,
	EK_WORLD_50		= 210,
	EK_WORLD_51		= 211,
	EK_WORLD_52		= 212,
	EK_WORLD_53		= 213,
	EK_WORLD_54		= 214,
	EK_WORLD_55		= 215,
	EK_WORLD_56		= 216,
	EK_WORLD_57		= 217,
	EK_WORLD_58		= 218,
	EK_WORLD_59		= 219,
	EK_WORLD_60		= 220,
	EK_WORLD_61		= 221,
	EK_WORLD_62		= 222,
	EK_WORLD_63		= 223,
	EK_WORLD_64		= 224,
	EK_WORLD_65		= 225,
	EK_WORLD_66		= 226,
	EK_WORLD_67		= 227,
	EK_WORLD_68		= 228,
	EK_WORLD_69		= 229,
	EK_WORLD_70		= 230,
	EK_WORLD_71		= 231,
	EK_WORLD_72		= 232,
	EK_WORLD_73		= 233,
	EK_WORLD_74		= 234,
	EK_WORLD_75		= 235,
	EK_WORLD_76		= 236,
	EK_WORLD_77		= 237,
	EK_WORLD_78		= 238,
	EK_WORLD_79		= 239,
	EK_WORLD_80		= 240,
	EK_WORLD_81		= 241,
	EK_WORLD_82		= 242,
	EK_WORLD_83		= 243,
	EK_WORLD_84		= 244,
	EK_WORLD_85		= 245,
	EK_WORLD_86		= 246,
	EK_WORLD_87		= 247,
	EK_WORLD_88		= 248,
	EK_WORLD_89		= 249,
	EK_WORLD_90		= 250,
	EK_WORLD_91		= 251,
	EK_WORLD_92		= 252,
	EK_WORLD_93		= 253,
	EK_WORLD_94		= 254,
	EK_WORLD_95		= 255,		/* 0xFF */

	/* Numeric keypad */
	EK_KP0		= 256,
	EK_KP1		= 257,
	EK_KP2		= 258,
	EK_KP3		= 259,
	EK_KP4		= 260,
	EK_KP5		= 261,
	EK_KP6		= 262,
	EK_KP7		= 263,
	EK_KP8		= 264,
	EK_KP9		= 265,
	EK_KP_PERIOD		= 266,
	EK_KP_DIVIDE		= 267,
	EK_KP_MULTIPLY	= 268,
	EK_KP_MINUS		= 269,
	EK_KP_PLUS		= 270,
	EK_KP_ENTER		= 271,
	EK_KP_EQUALS		= 272,

	/* Arrows + Home/End pad */
	EK_UP			= 273,
	EK_DOWN		= 274,
	EK_RIGHT		= 275,
	EK_LEFT		= 276,
	EK_INSERT		= 277,
	EK_HOME		= 278,
	EK_END		= 279,
	EK_PAGEUP		= 280,
	EK_PAGEDOWN		= 281,

	/* Function keys */
	EK_F1			= 282,
	EK_F2			= 283,
	EK_F3			= 284,
	EK_F4			= 285,
	EK_F5			= 286,
	EK_F6			= 287,
	EK_F7			= 288,
	EK_F8			= 289,
	EK_F9			= 290,
	EK_F10		= 291,
	EK_F11		= 292,
	EK_F12		= 293,
	EK_F13		= 294,
	EK_F14		= 295,
	EK_F15		= 296,

	/* Key state modifier keys */
	EK_NUMLOCK		= 300,
	EK_CAPSLOCK		= 301,
	EK_SCROLLOCK		= 302,
	EK_RSHIFT		= 303,
	EK_LSHIFT		= 304,
	EK_RCTRL		= 305,
	EK_LCTRL		= 306,
	EK_RALT		= 307,
	EK_LALT		= 308,
	EK_RMETA		= 309,
	EK_LMETA		= 310,
	EK_LSUPER		= 311,		/* Left "Windows" key */
	EK_RSUPER		= 312,		/* Right "Windows" key */
	EK_MODE		= 313,		/* "Alt Gr" key */
	EK_COMPOSE		= 314,		/* Multi-key compose key */

	/* Miscellaneous function keys */
	EK_HELP		= 315,
	EK_PRINT		= 316,
	EK_SYSREQ		= 317,
	EK_BREAK		= 318,
	EK_MENU		= 319,
	EK_POWER		= 320,		/* Power Macintosh power key */
	EK_EURO		= 321,		/* Some european keyboards */
	EK_UNDO		= 322,		/* Atari keyboard has Undo */

	/* Add any other keys here */

	EK_LAST
}; // Keys

enum KEY_STATE
{
	EKS_Down,
	EKS_Up
};

//! class keyboard dar dakhele device hast va fagha on mitone in class ro besaze.
//! ba dastor GetKeyboard() ye pointer be in class az device begirid.
class KGE_API Keyboard: public KgeUnknown
{
public:
	//! Sazande.
	Keyboard() {}

	//! Mokhareb.
	~Keyboard() {}

	//! Vaghti ke klide morde nazar feshorde shavad meghdare true ro mide.
	/*
	 \param Keys baraye fahmidane inke kodom kelid feshorde shode ast.
	 \return agr kelid paeen bashad meghdare true mide.
	 */
	bool KeyDown(KEYS key);

	//! Vaghti ke klide morde nazar feshorde shavad meghdare true ro mide.
	/*
	 \param Keys baraye fahmidane inke kodom kelid feshorde shode ast.
	 \return agr kelid paeen bashad meghdare true mide.
	 */
	bool KeyDown(char key);

	//! Connect an event 
	virtual void Connect(gui::EventType eventtype,
		core::Functor1<void, gui::EventArgs*>* Function);

}; // Keyboard

} // io

} // kge

#endif // KEYBOARD_H