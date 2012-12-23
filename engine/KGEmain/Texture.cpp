#include "../include/Texture.h"
#include "../include/Image.h"

namespace kge
{
	namespace gfx
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		Texture::Texture( Image* pImg ): m_iWidth(pImg->m_iWidth), m_iHeight(pImg->m_iHeight),
			m_iBpp(pImg->m_iBpp), m_ibpp(pImg->m_ibpp), m_iMipmapsCount(pImg->m_iMipmapsCount)
			, m_Format(pImg->m_Format)
		{

		} // Constructor

	} // gfx

} // kge
