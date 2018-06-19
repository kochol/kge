// File name: Font.cpp
// Des: Interface class for static text.
// Date: 16/01/1387
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/gui/Text.h"
#include <map>

namespace kge
{
namespace gui
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Text::Text( core::RectI rect, gfx::Renderer* pRenderer, const wchar_t* text, int FontSize,
			    Font* pFont)
	{
		m_Rect		   = rect;
		m_pRenderer    = pRenderer;
		m_FontSize	   = FontSize;
		m_pText		   = new wchar_t[wcslen(text)];
		wcscpy(m_pText, text);
		m_Color.c	   = 0xffffffff;
		m_bMouseOver   = false;
		m_Rect.Height += 100;
		m_pFont		   = pFont;

		CreateChars();
		
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Text::~Text()
	{
		if (m_pText)
			delete[] m_pText;
		
		m_pFont->DecRef();

	} // Destructor

	//------------------------------------------------------------------------------------
	// Renders the static text.
	//------------------------------------------------------------------------------------
	void Text::Render()
	{
		m_pRenderer->SetTexture(m_pFont->m_pTexture);
		m_pRenderer->DrawTriangleList(m_pVertexBuffer, m_pIndexBuffer, m_iVcount, m_iIcount, gfx::EVT_V3T);

	} // Render

	//------------------------------------------------------------------------------------
	// Updates the control properties.
	//------------------------------------------------------------------------------------
	void Text::Update()
	{
		m_Rect.Height += 100;

	} // Update

	//------------------------------------------------------------------------------------
	// Create text vertex buffer
	//------------------------------------------------------------------------------------
	void Text::CreateChars()
	{
		int tLen = (int)wcslen(m_pText);
		if (tLen < 1)
			return;

		// Check for farsi language
		bool bFarsi = false;
		if (m_pText[0] > 1000)
		{
			bFarsi = true;
		}

		gfx::Vertex3T* verts   = new gfx::Vertex3T[tLen * 4];
		u16			 * indices = new u16[tLen * 6];
		Char		 * ch;
		float x = (float)m_Rect.X;
		float y = (float)(m_Rect.Y);// + m_pFont->m_iLineHeight);
		float z = 0.0f;
		float u, v, u2, v2;
		std::map<int, Char*>::const_iterator cIt;
		wchar_t pch = 32, nch = 32;
		//int ich = 0;
		bool bp,bn;

		if (m_pText[0] > 1000)
		{
			bFarsi = true;
			//x = (float)(m_Rect.X + m_Rect.Width);

			// Parse farsi script
			wchar_t* wtch = new wchar_t[tLen];
			
			for (int i = 0; i < tLen; i++)
				wtch[i] = m_pText[i];

			for (int i = 0; i < tLen; i++)
			{
				bp = bn = true;
				if (i - 1 >= 0)
					pch = wtch[i - 1];
				else
					pch = 32;
				if (i + 1 < tLen)
					nch = wtch[i + 1];
				else
					nch = 32;

				if (pch == 32 || pch == 1575 || pch == 1608 || pch == 1688)
					bp = false;
				else if (pch > 1582 && pch < 1587)
					bp = false;
				if (nch == 32)
					bn = false;

				// start farsi script
				switch (m_pText[i])
				{
					// Hamze
				case 1574:
					m_pText[i] = CalcChar(65164, bp, bn);
					break;

					// Alef
				case 1575:
					if (bp)
						m_pText[i] = 65166;
					else
						m_pText[i] = 65165;
					break;

					// Be
				case 1576:
					m_pText[i] = CalcChar(65170, bp, bn);
					break;

					// Te
				case 1578:
					m_pText[i] = CalcChar(65176, bp, bn);
					break;

					// Se
				case 1579:
					m_pText[i] = CalcChar(65180, bp, bn);
					break;

					// Jim
				case 1580:
					m_pText[i] = CalcChar(65184, bp, bn);
					break;

					// He jimi
				case 1581:
					m_pText[i] = CalcChar(65188, bp, bn);
					break;

					// Khe
				case 1582:
					m_pText[i] = CalcChar(65192, bp, bn);
					break;

					// Dal
				case 1583:
					if (bp)
						m_pText[i] = 65194;
					else
						m_pText[i] = 65193;
					break;

					// Zal
				case 1584:
					if (bp)
						m_pText[i] = 65196;
					else
						m_pText[i] = 65195;
					break;

					// Re
				case 1585:
					if (bp)
						m_pText[i] = 65198;
					else
						m_pText[i] = 65197;
					break;

					// Ze
				case 1586:
					if (bp)
						m_pText[i] = 65200;
					else
						m_pText[i] = 65199;
					break;

					// Sin
				case 1587:
					m_pText[i] = CalcChar(65204, bp, bn);
					break;

					// Shin
				case 1588:
					m_pText[i] = CalcChar(65208, bp, bn);
					break;

					// Sad
				case 1589:
					m_pText[i] = CalcChar(65212, bp, bn);
					break;

					// Zad
				case 1590:
					m_pText[i] = CalcChar(65216, bp, bn);
					break;

					// Ta daste dar
				case 1591:
					if (bp)
						m_pText[i] = 65220;
					else
						m_pText[i] = 65219;
					break;

					// Za daste dar
				case 1592:
					if (bp)
						m_pText[i] = 65224;
					else
						m_pText[i] = 65223;
					break;

					// Eyn
				case 1593:
					m_pText[i] = CalcChar(65228, bp, bn);
					break;

					// Gheyn
				case 1594:
					m_pText[i] = CalcChar(65232, bp, bn);
					break;

					// Fe
				case 1601:
					m_pText[i] = CalcChar(65236, bp, bn);
					break;

					// Ghaf
				case 1602:
					m_pText[i] = CalcChar(65240, bp, bn);
					break;

					// Lam
				case 1604:
					m_pText[i] = CalcChar(65248, bp, bn);
					break;

					// Mim
				case 1605:
					m_pText[i] = CalcChar(65252, bp, bn);
					break;

					// Noon
				case 1606:
					m_pText[i] = CalcChar(65256, bp, bn);
					break;

					// He
				case 1607:
					m_pText[i] = CalcChar(65260, bp, bn);
					break;

					// Vav
				case 1608:
					if (bp)
						m_pText[i] = 65262;
					else
						m_pText[i] = 65261;
					break;

					// Pe
				case 1662:
					m_pText[i] = CalcChar(64345, bp, bn);
					break;

					// Che
				case 1670:
					m_pText[i] = CalcChar(64381, bp, bn);
					break;

					// Zhe
				case 1688:
					if (bp)
						m_pText[i] = 64395;
					else
						m_pText[i] = 64394;
					break;

					// Ke
				case 1705:
					m_pText[i] = CalcChar(64401, bp, bn);
					break;

					// Gaf
				case 1711:
					m_pText[i] = CalcChar(64405, bp, bn);
					break;

					// Ye
				case 1740:
				case 1610:
					if (bp && bn)
						m_pText[i] = 65268;
					else if (bp)
						m_pText[i] = 65264;
					else if (bn)
						m_pText[i] = 65267;
					else
						m_pText[i] = 65263;
					break;

				}
			}

			for (int i = 0; i < tLen; i++)
				wtch[i] = m_pText[tLen - i - 1];
			for (int i = 0; i < tLen; i++)
				m_pText[i] = wtch[i];
			delete[] wtch;
		}

		for (int i = 0; i < tLen; i++)
		{
			cIt = m_pFont->m_mChars.find((int)m_pText[i]);
			ch = cIt->second;

			// Map the center of the texel to the corners
			// in order to get pixel perfect mapping
			u = (float(ch->x)+0.5f) / (float)m_pFont->m_iWidth;
			v = (float(ch->y)+0.5f) / (float)m_pFont->m_iHeight;
			u2 = u + float(ch->width) / (float)m_pFont->m_iWidth;
			v2 = v + float(ch->height) / (float)m_pFont->m_iHeight; 

			verts[i*4].pos.X   = x + ch->xOffset;
			verts[i*4+1].pos.X = x + ch->xOffset + ch->width;
			verts[i*4+2].pos.X = x + ch->xOffset;
			verts[i*4+3].pos.X = x + ch->xOffset + ch->width;
			verts[i*4].tex.X = u;
			verts[i*4+1].tex.X = u2;
			verts[i*4+2].tex.X = u;
			verts[i*4+3].tex.X = u2;
			x += ch->xAdvance;
			indices[i*6]   = i*4;
			indices[i*6+1] = i*4+1;
			indices[i*6+2] = i*4+2;
			indices[i*6+3] = i*4+1;
			indices[i*6+4] = i*4+3;
			indices[i*6+5] = i*4+2;
			verts[i*4].pos.Y = y + ch->yOffset;
			verts[i*4].pos.Z = z;
			verts[i*4].tex.Y = v;
			verts[i*4+1].pos.Y = y + ch->yOffset;
			verts[i*4+1].pos.Z = z;
			verts[i*4+1].tex.Y = v;
			verts[i*4+2].pos.Y = y + ch->yOffset + ch->height;
			verts[i*4+2].pos.Z = z;
			verts[i*4+2].tex.Y = v2;
			verts[i*4+3].pos.Y = y + ch->yOffset + ch->height;
			verts[i*4+3].pos.Z = z;
			verts[i*4+3].tex.Y = v2;
		}
		m_iVcount = tLen * 4;
		m_iIcount = tLen * 6;
		m_pVertexBuffer = m_pRenderer->CreateVertexBuffer(verts, m_iVcount, gfx::EVT_V3T);
		m_pIndexBuffer  = m_pRenderer->CreateIndexBuffer(indices, m_iIcount);

		delete[] verts;
		verts = NULL;
		delete[] indices;
		indices = NULL;

	} // CreateChars

	//------------------------------------------------------------------------------------
	// Calc farsi char
	//------------------------------------------------------------------------------------
	wchar_t Text::CalcChar(int id, bool prev, bool next)
	{
		if (prev && next)
			return id;
		else if (next)
			return id - 1;
		else if (prev)
			return id - 2;
		else
			return id - 3;

	} // CalcChar

} // gui

} // kge
