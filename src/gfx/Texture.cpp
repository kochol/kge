#include "../../include/kge/gfx/Texture.h"
#include "../../include/kge/gfx/ImageData.h"

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// ImageData constructor
	//------------------------------------------------------------------------------------
	Texture::Texture( ImageData* pImg ): Resource(0, pImg->m_pFileName, NULL, NULL), 
		m_iWidth(pImg->m_iWidth), m_iHeight(pImg->m_iHeight),
		m_iBpp(pImg->m_iBpp), m_ibpp(pImg->m_ibpp), m_iMipmapsCount(pImg->m_iMipmapsCount)
		, m_Tfmt(pImg->m_Format)
	{

	} // ImageData constructor

} // gfx

} // kge
