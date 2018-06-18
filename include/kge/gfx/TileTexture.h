// File name: TileTexture.h
// Des: This class is for storing tile data in a texture
// Date: 22/05/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_TILETEXTURE_H
#define KGE_TILETEXTURE_H

#include "../KgeUnknown.h"
#include "../kgedef.h"
#include "Texture.h"
#include "Tile.h"
#include <vector>

#ifdef KGE_USE_BOOST
#include <boost/serialization/vector.hpp>
#include <boost/serialization/binary_object.hpp>
#endif // KGE_USE_BOOST

namespace kge
{
namespace gfx
{

class KGE_API TileTexture: public KgeUnknown
{
#ifdef KGE_USE_BOOST
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int file_version) const
	{
		ar &	m_iNumCols &
				m_iNumRows &
				m_iWidth &
				m_iHeight &
				m_iMaxMipMapLevel &
				m_fU &
				m_fV &
				m_fUO &
				m_fVO &
				m_dU &
				m_dV;

		// Save Tiles
		ar &	m_vTiles;
		
		// Save compressed texture
		int w , h, t, s;
		for (int i = 0; i < m_iMaxMipMapLevel; i++)
		{
			t = math::pow(2, i);
			w = m_iWidth / t;
			h = m_iHeight / t;
			s = w * h;
			ar & s;
			u8* pData = m_pCompressedTexture->GetCopyData(i);
			ar & boost::serialization::make_binary_object(pData, s); 
		}

	} // save

	template<class Archive>
	void load(Archive & ar, const unsigned int file_version) 
	{
		ar &	m_iNumCols &
				m_iNumRows &
				m_iWidth &
				m_iHeight &
				m_iMaxMipMapLevel &
				m_fU &
				m_fV &
				m_fUO &
				m_fVO &
				m_dU &
				m_dV;
		if (m_pCompressedTexture)
		{
			m_pCompressedTexture->DecRef();
		}

		// Load Tiles
		ar &	m_vTiles;

		// Create Compressed texure
		static int loadercount = -1;
		loadercount++;
		core::String str;
		str = "LoadedTileMap";
		str += loadercount;
		u32 iHandle = Device::GetSingletonPtr()->GetTextureManager()->Add
			(NULL, NULL, str.ToCharPointer());
		m_pCompressedTexture = Device::GetSingletonPtr()->GetTextureManager()->
			GetResource(iHandle);
		m_pCompressedTexture->CreateTexture(m_iWidth, m_iHeight, ETF_DXT5, m_iMaxMipMapLevel);

		// Load compressed texture
		int w , s;// h, t;
		for (int i = 0; i < m_iMaxMipMapLevel; i++)
		{
			ar & s;
			u8* pData = KGE_NEW_ARRAY(u8, s);
			ar & boost::serialization::make_binary_object(pData, s); 
			m_pCompressedTexture->SetData(pData, s, i);
			KGE_DELETE_ARRAY(pData);
		}
		m_pTexture = m_pCompressedTexture;

	} // load

#endif // KGE_USE_BOOST

public:

	//! Constructor
	TileTexture();

	//! Destructor
	~TileTexture();

	//! Load a created tile texture from file
	void CreateTileTexture(const char* pTextureFileName, int numCols, int numRows);

	//! Create the tile texture class with the needed data
	void CreateTileTexture( int Width, int Height, int numCols, int numRows);

	//! Add a tile to the tile texture and returns its ID. Returns -1 on fail
	int AddTile(Tile* pTile);

	//! Returns the tile by ID.
	Tile* GetTile(u32 TileID) { return m_vTiles[TileID]; }

	//! Returns tile count
	int GetTileCount() {return (int)m_vTiles.size();}

	float GetU() { return m_fU; }

	float GetV() { return m_fV; }

	double GetdU() { return m_dU; }

	double GetdV() { return m_dV; }

	Texture* GetTexture() { return m_pTexture; }

	//! Save the current tile texture to the file
	void Save(char* FileName);

	//! Load the current tile texture to the file
	void Load(char* FileName);

	void TestTextureCompress() {CreateCompressedTexture(); m_pTexture=m_pCompressedTexture;}

protected:

	// Texture
	Texture			*	m_pTexture,				//!< Tile texture
					*	m_pCompressedTexture;	//!< Compressed texture

	// Count every thing
	int					m_iNumCols,
						m_iNumRows,
						m_iWidth,
						m_iHeight,
						m_iMaxMipMapLevel;

	float				m_fU,
						m_fV,
						m_fUO,
						m_fVO;
	double				m_dU,
						m_dV;

	// Tiles
	std::vector<Tile*>	m_vTiles;

	// Check for tile collision
	bool CheckTileCollision( int X, int Y, int XplusWidth, int YplusHeight );

	//! Create a compressed texture
	void CreateCompressedTexture();

}; // TileTexture

} // gfx

} // kge

#endif // KGE_TILETEXTURE_H
