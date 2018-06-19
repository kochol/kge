// File name: TileTexture.cpp
// Des: This class is for storing tile data in a texture
// Date: 22/11/1387
// Programmers: Ali Akbar Mohammadi (Kochol)

#include "../../Include/Device.h"
#include "../../Include/gfx/TileTexture.h"
#include "../../Include/gfx/Image.h"
#include "../../Include/io/Logger.h"
#include <time.h>
#include "../../Include/math/math.h"



#ifdef KGE_USE_BOOST
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>
#include <iostream>
#endif // KGE_USE_BOOST

#ifndef NULL
#define NULL 0
#endif

namespace kge
{
namespace gfx
{
	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	TileTexture::TileTexture() : 
		m_iNumCols(0), m_iNumRows(0), 
			m_pTexture(NULL), m_iMaxMipMapLevel(0), m_fU(0), m_fV(0), 
			m_pCompressedTexture(NULL)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TileTexture::~TileTexture()
	{
		m_vTiles.clear();

		if (m_pTexture == m_pCompressedTexture)
		{
			m_pTexture->DecRef();
		}
		else
		{
			if (m_pTexture)
				m_pTexture->DecRef();

			if (m_pCompressedTexture)
				m_pCompressedTexture->DecRef();
		}

	} // Destructor

	//------------------------------------------------------------------------------------
	// Load a created tile texture from file
	//------------------------------------------------------------------------------------
	void TileTexture::CreateTileTexture(const char* pTextureFileName, int numCols, int numRows)
	{
		io::Logger::Log(io::ELM_Warning, "\"CreateTileTexture(char* pTextureFileName, int numCols, int numRows)\" is not implemented yet.");

	} // CreateTileTexture

	//------------------------------------------------------------------------------------
	// Create the tile texture class with the needed data
	//------------------------------------------------------------------------------------
	void TileTexture::CreateTileTexture( int Width, int Height, int numCols, int numRows )
	{
		m_dU = (double)(1.0f / Width);
		m_fU = (float)(1.0f / numCols);
		m_dV = (double)(1.0f / Height);
		m_fV = (float)(1.0f / numRows);

		m_fUO = 32.0f * m_dU;
		m_fVO = 32.0f * m_dV;

		// Create texture
		m_iMaxMipMapLevel = 0;
		int wi = Width;
		while (wi > 256)
		{
			wi /= 2;
			m_iMaxMipMapLevel++;
		}

//		if (m_iMaxMipMapLevel > 3)
	//		m_iMaxMipMapLevel = 3;
		static int idTile = -1;
		idTile++;
		core::String s;
		s = "KGETileTexure";
		s += idTile;
		u32 iHandle = Device::GetSingletonPtr()->GetTextureManager()->Add
			(NULL, NULL, s.ToCharPointer());
		m_pTexture = Device::GetSingletonPtr()->GetTextureManager()->
			GetResource(iHandle);
		m_pTexture->CreateTexture(Width, Height, ETF_A8R8G8B8, m_iMaxMipMapLevel, true);

		m_iNumCols = numCols;
		m_iNumRows = numRows;
		m_iWidth   = Width;
		m_iHeight  = Height;

	} // CreateTileTexture( int Width, int Height, int numCols )

	//------------------------------------------------------------------------------------
	// Add a tile to the tile texture and returns its ID. Returns -1 on fail
	//------------------------------------------------------------------------------------
	int TileTexture::AddTile( Tile* pTile )
	{
		pTile->AddRef();

		// Find a place for tile image in our texture.
		int TileSize = 16;

		int x, y;
		x = y = 0;
		bool bLoop = true;
		for ( y = 0; y < m_iHeight && bLoop; y += TileSize)
		{
			for ( x = 0; x < m_iWidth && bLoop; x += TileSize)
			{
				if (CheckTileCollision(x, y, x + pTile->GetWidth(), y + pTile->GetHeight())
					&& bLoop)
				{
					pTile->SetPosition(x, y);
					pTile->SetOffsets((float)x / (float)m_iWidth + m_fUO, (float)y / (float)m_iHeight + m_fVO);
					bLoop = false;
					io::Logger::Log(io::ELM_Warning, "x=%d \t y=%d", pTile->GetX(), pTile->GetY());
				}
			} // x

		} // y

		if (x == m_iWidth && y == m_iHeight)
		{
			io::Logger::Log("There is no space for adding new tile.");
			return -1;
		}

		// Add tile image data to the texture.
		Image* img = pTile->GetImage();
		pTile->SetTotalCellsNumber(m_iNumCols, m_iNumRows);
		pTile->SetUV(m_fU, m_fV);

		u8* data= NULL;
		int t, w, h;

		for (int i = 0; i < m_iMaxMipMapLevel; i++)
		{
			t = math::pow(2, i);
			w = img->GetWidth() / t;
			h = img->GetHeight() / t;
			io::Logger::Log(io::ELM_Warning, "w=%d \t h=%d", w, h);
			
			if (data)
				delete[] data;
			data = new u8[w * h * 4];
			img->GetPixels(0, 0, 0, w, h, 1, EIF_BGRA, data , i);
			m_pTexture->SetData(pTile->GetX() / t, pTile->GetY() / t, w, h,
				data, w * h * 4, i);
		}

		// Check where we can put the tile.
		m_vTiles.push_back(pTile);

		return (int)m_vTiles.size() - 1;

	} // AddTile

	//------------------------------------------------------------------------------------
	// Check for tile collision
	//------------------------------------------------------------------------------------
	bool TileTexture::CheckTileCollision( int X, int Y, int XplusWidth, int YplusHeight )
	{
		if (X >= m_iWidth || XplusWidth > m_iWidth ||
			Y >= m_iHeight || YplusHeight > m_iHeight)
			return false;

		int i;
		// Searching for resource.
		for(std::vector<Tile*>::iterator it = m_vTiles.begin(); 
			it != m_vTiles.end(); ++it)
		{
			i = 0;
			if (X < (*it)->GetX() + (*it)->GetWidth() &&
				X >= (*it)->GetX())
				i++;
			else if (XplusWidth <= (*it)->GetX() + (*it)->GetWidth() &&
				XplusWidth > (*it)->GetX())
				i++;

			if (Y < (*it)->GetY() + (*it)->GetHeight() &&
				Y >= (*it)->GetY())
				i++;
			else if (YplusHeight <= (*it)->GetY() + (*it)->GetHeight() &&
				YplusHeight > (*it)->GetY())
				i++;

			if (i == 2)
				return false;
		}

		return true;

	} // CheckTileCollision

	//------------------------------------------------------------------------------------
	// Create a compressed texture
	//------------------------------------------------------------------------------------
	void TileTexture::CreateCompressedTexture()
	{
		if (m_pCompressedTexture)
			m_pCompressedTexture->DecRef();

		// Create compressed texture
		core::String s;
		s = m_pTexture->GetName();
		s += "Com";
		u32 iHandle = Device::GetSingletonPtr()->GetTextureManager()->Add
			(NULL, NULL, s.ToCharPointer());
		m_pCompressedTexture = Device::GetSingletonPtr()->GetTextureManager()->
			GetResource(iHandle);
		m_pCompressedTexture->CreateTexture(m_iWidth, m_iHeight, ETF_DXT5, m_pTexture->GetNumberOfMipmapLevels(), true);

		// Create Compressed data
		int w , h, t;
		for (int i = 0; i < m_pTexture->GetNumberOfMipmapLevels(); i++)
		{
			t = math::pow(2, i);
			w = m_iWidth / t;
			h = m_iHeight / t;
			Image* pTemp = new Image("temp");
			pTemp->CreateImage(w, h);
			pTemp->SetPixels(0, 0, 0, w, h, 1, EIF_BGRA,
				m_pTexture->GetCopyData(i));
			u32 size;
			u8* cmpData = pTemp->GetCompressData(ECT_DXT5, size);
			m_pCompressedTexture->SetData(cmpData, size, i);
			KGE_DELETE_ARRAY(cmpData);
			pTemp->DecRef();
		}

	} // CreateCompressedTexture

	// Save the current tile texture to the file
	void TileTexture::Save( char* FileName )
	{
		if (!m_pCompressedTexture)
		{
			CreateCompressedTexture();
		}

#ifdef KGE_USE_BOOST

		// make an archive
		std::ofstream ofs(FileName, std::ios::binary);
		boost::archive::binary_oarchive oa(ofs);
		oa << *this;

#endif // KGE_USE_BOOST

	} // Save

	// Load the current tile texture to the file
	void TileTexture::Load( char* FileName )
	{
#ifdef KGE_USE_BOOST

		// make an archive
		std::ifstream ofs(FileName, std::ios::binary);
		if (!ofs.is_open())
			return;
		boost::archive::binary_iarchive ia(ofs);
		ia >> *this;

#endif // KGE_USE_BOOST

	} // Load

} // gfx

} // kge
