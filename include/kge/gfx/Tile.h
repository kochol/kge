// File name: Tile.h
// Des: This class is for storing tile data for TileTexture class
// Date: 08/06/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_TILE_H
#define KGE_TILE_H

#include "../KgeUnknown.h"
#include "Image.h"
#include "Structs.h"
#include "../math/Vector.h"

#ifdef KGE_USE_BOOST
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>

#endif // KGE_USE_BOOST

namespace kge
{
namespace gfx
{
//! The tile type. currently we have two tile type one for back tiles and one for front tile
enum TileType
{
	ETT_BackTile = 0,
	ETT_FrontTile

}; // TileType

//! This class store tiles data for TileTexture class
class KGE_API Tile: public KgeUnknown
{
#ifdef KGE_USE_BOOST
	friend class boost::serialization::access;
	// Serialize
	template<class Archive>
	void serialize(Archive & ar, const unsigned int file_version)
	{
		ar & m_iWidth &
			m_iHeight &
			m_iX &
			m_iY &
			m_iNumCols &
			m_iNumRows &
			m_iTotalCols &
			m_iTotalRows &
			m_fU &
			m_fV &
			m_fUOffset &
			m_fVOffset ;
			//m_eTileType;

	}
#endif // KGE_USE_BOOST

public:

	//! Constructor
	Tile();

	//! Destructor
	~Tile();

	//! Create the tile data and load its image tile.
	bool CreateTile(const char* pFileName, TileType eTileType, int NumCols, int NumRows, const char* name = "TileClass");

	//! Create the tile data. It's good for created tile textures.
	void CreateTile(int Width, int Height, TileType eTileType, int NumCols, int NumRows);

	//! Returns the tile texcoords.
	math::Vector GetTile(int Col, int Row);

	//! Sets the U & V offset. this function is used by TileTexture Class
	void SetOffsets( float UOffset, float VOffset) 
	{ 
		m_fUOffset = UOffset;
		m_fVOffset = VOffset;
	}

	void SetUV(float U, float V)
	{
		m_fU = U;
		m_fV = V;

	} // SetUV

	//! Sets the tile position in the tile texture class
	void SetPosition(int X, int Y)
	{
		m_iX	   = X;
		m_iY	   = Y;

	}

	//! returns the image x position in the tile texture
	int GetX() { return m_iX; }

	//! Returns the image y position in the tile texture
	int GetY() { return m_iY; }

	//! Returns the image Width
	int GetWidth() { return m_iWidth; }

	//! Returns the image Height
	int GetHeight() { return m_iHeight; }

	//! Returns the tile Image
	Image* GetImage() { return m_pImage; }

	//! 
	void SetTotalCellsNumber(int ColCount, int RowCount)
	{
		m_iTotalCols = ColCount;
		m_iTotalRows = RowCount;
	}

protected:
	int			m_iWidth,
				m_iHeight,
				m_iX,
				m_iY,
				m_iNumCols,
				m_iNumRows,
				m_iTotalCols,
				m_iTotalRows;
	float		m_fU,
				m_fV,
				m_fUOffset,
				m_fVOffset;
	TileType	m_eTileType;
	Image*		m_pImage;

}; // Tile

} // gfx

} // kge

#endif // KGE_TILE_H
