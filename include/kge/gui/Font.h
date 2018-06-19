// File name: Font.h
// Des: Class for bitmap fonts.
// Date: 17-Apr-2007
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef FONT_H
#define FONT_H

#include <map>
#include "../kgedef.h"
#include "../Resource.h"
#include "../sn/SceneManager.h"
#include "../core/Structs.h"
#include "Char.h"

namespace kge
{
namespace gfx
{
	class Color;

} // gfx

namespace gui
{

class KGE_API Font : public Resource
{
	friend class Text;

public:

	//! Constructor.
	Font(const u32 Handle, const char* FileName, const char* Name, sn::SceneManager* pSnmgr);

	//! Destructor
	virtual ~Font();

	//! Loads and create font characters.
	virtual bool Init();

protected:

	Char*				 m_pChars;
	std::map<int, Char*> m_mChars;
	int					 m_iLineHeight,
						 m_iBase,
						 m_iWidth,
						 m_iHeight,
						 m_iPages,
						 m_iCharsCount;
	char*				 m_pTextureFileName;
	gfx::Texture*		 m_pTexture;
	sn::SceneManager*	 m_pSnmgr;

}; // Font

}  // gui

}  // kge

#endif // FONT_H