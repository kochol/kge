// File name: Font.cpp
// Des: Class for bitmap fonts.
// Date: 28/07/1387
// Programmer: Ali Akbar Mohammadi(Kochol)

#include "../../Include/gui/Font.h"
#include "../../Include/io/Logger.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace gui
{
	//------------------------------------------------------------------------------------
	// Constructor.
	//------------------------------------------------------------------------------------
	Font::Font(const u32 Handle, const char* FileName, const char* Name, sn::SceneManager* pSnmgr) : 
		Resource(Handle, FileName, Name, NULL), m_pChars(NULL), m_pSnmgr(pSnmgr)
	{
	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Font::~Font()
	{
	} // Destructor

	//------------------------------------------------------------------------------------
	// Loads and create font characters.
	//------------------------------------------------------------------------------------
	bool Font::Init()
	{
		if (m_pChars)
			delete[] m_pChars;

		// Parse fnt file
		std::fstream file(m_pFileName);

		if (!file.is_open())
		{
			io::Logger::Log(io::ELM_Error, "Can't find font file %s", m_pFileName);
			return false;
		}

		std::string Line;
		std::string Read, Key, Value;
		std::size_t i;
		//this is data for a specific char
		int CharID = 0;
		typedef std::pair<int, Char*> cPair;

		while( std::getline( file, Line ) )
		{
			std::stringstream LineStream;
			LineStream << Line;
			
			//read the line's type
			LineStream >> Read;
			if( Read == "common" )
			{
				//this holds common data
				while( LineStream >> Read )
				{
					std::stringstream Converter;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "lineHeight" )
						Converter >> m_iLineHeight;
					else if( Key == "base" )
						Converter >> m_iBase;
					else if( Key == "scaleW" )
						Converter >> m_iWidth;
					else if( Key == "scaleH" )
						Converter >> m_iHeight;
					else if( Key == "pages" )
						Converter >> m_iPages;
				}
			}
			else if( Read == "page" )
			{
				//this holds page data
				while( LineStream >> Read )
				{
					std::stringstream Converter;
					std::string str;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "file" )
					{
						Converter >> str;
						str = str.substr(1, str.length() - 2);
						m_pTextureFileName = new char[str.length()];
						strcpy(m_pTextureFileName, str.c_str());
					}
				}
			}
			else if( Read == "chars" )
			{
				//this holds chars data
				while( LineStream >> Read )
				{
					std::stringstream Converter;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "count" )
					{
						Converter >> m_iCharsCount;
						m_pChars = new Char[m_iCharsCount];
					}
				}
			}
			else if( Read == "char" )
			{

				while( LineStream >> Read )
				{
					std::stringstream Converter;
					i = Read.find( '=' );
					Key = Read.substr( 0, i );
					Value = Read.substr( i + 1 );

					//assign the correct value
					Converter << Value;
					if( Key == "id" )
						Converter >> m_pChars[CharID].ID;
					else if( Key == "x" )
						Converter >> m_pChars[CharID].x;
					else if( Key == "y" )
						Converter >> m_pChars[CharID].y;
					else if( Key == "width" )
						Converter >> m_pChars[CharID].width;
					else if( Key == "height" )
						Converter >> m_pChars[CharID].height;
					else if( Key == "xoffset" )
						Converter >> m_pChars[CharID].xOffset;
					else if( Key == "yoffset" )
						Converter >> m_pChars[CharID].yOffset;
					else if( Key == "xadvance" )
					{
						Converter >> m_pChars[CharID].xAdvance;
						m_mChars.insert(cPair(m_pChars[CharID].ID, &m_pChars[CharID]));
						CharID++;
					}
				}
			} 
		}

		m_pTexture = m_pSnmgr->AddTexture(m_pTextureFileName);//, m_pFileName, gfx::ECT_None);

		return true;

	} // Init

} // gui

} // kge
