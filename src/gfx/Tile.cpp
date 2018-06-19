// File name: Tile.h
// Des: This class is for storing tile data for TileTexture class
// Date: 08/06/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#include "../../Include/gfx/Tile.h"
#include "../../Include/Device.h"



namespace kge
{
namespace gfx
{

	//------------------------------------------------------------------------------------
	// Constructor
	//------------------------------------------------------------------------------------
	Tile::Tile(): m_iWidth(0), m_iHeight(0), m_eTileType(ETT_BackTile), m_pImage(0),
		m_iNumCols(0), m_iNumRows(0), m_fU(0.0f), m_fV(0.0f), m_fUOffset(0.0f),
		m_fVOffset(0.0f), m_iX(0), m_iY(0)
	{

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	Tile::~Tile()
	{

	} // Destructor

	//------------------------------------------------------------------------------------
	// Create the tile data and load its image tile.
	//------------------------------------------------------------------------------------
	bool Tile::CreateTile( const char* pFileName, TileType eTileType, int NumCols, int NumRows, const char* name )
	{
		
		u32 ih = 
			Device::GetSingletonPtr()->GetImageManager()->Add
			(pFileName, NULL, name);

		m_pImage = Device::GetSingletonPtr()->GetImageManager()->GetResource(ih);

		Image* pTemp = new Image("temp");
		u8* data = new u8[m_pImage->GetWidth()*m_pImage->GetHeight()*4];
		pTemp->CreateImage(m_pImage->GetWidth() + 64, m_pImage->GetHeight() + 64, 
			1, 4, EIF_RGBA);

		u8* dataOrig = new u8[m_pImage->GetWidth()*m_pImage->GetHeight()*4];//m_pImage->GetData();
		u8* dataTemp = new u8[pTemp->GetWidth()*pTemp->GetHeight()*4];//pTemp->GetData();
		m_pImage->GetPixels(0,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),1,EIF_RGBA,dataOrig);
		pTemp->GetPixels(0,0,0,pTemp->GetWidth(),pTemp->GetHeight(),1,EIF_RGBA,dataTemp);

  		for (int y = 0; y < m_pImage->GetHeight(); y++)
  		{
			// center
  			memcpy(&dataTemp[(y + 32) * pTemp->GetWidth() * 4 + 128], 
  				&dataOrig[y * m_pImage->GetWidth() * 4], 
  				m_pImage->GetWidth() * 4);
			// left to right
			memcpy(&dataTemp[(y + 32) * pTemp->GetWidth() * 4 + (m_pImage->GetWidth() + 32) * 4], 
				&dataOrig[y * m_pImage->GetWidth() * 4], 
				128);
			// right to left
			memcpy(&dataTemp[(y + 32) * pTemp->GetWidth() * 4], 
				&dataOrig[y * m_pImage->GetWidth() * 4 + (m_pImage->GetWidth() - 32) * 4], 
				128);
  		}

		for (int y = 0; y < 32; y++)
		{
			// top to down
			memcpy(&dataTemp[(y + m_pImage->GetHeight() + 32) * pTemp->GetWidth() * 4 + 128], 
				&dataOrig[y * m_pImage->GetWidth() * 4], 
				m_pImage->GetWidth() * 4);
			// down to top
			memcpy(&dataTemp[y * pTemp->GetWidth() * 4 + 128], 
				&dataOrig[(y + m_pImage->GetHeight() - 32) * m_pImage->GetWidth() * 4], 
				m_pImage->GetWidth() * 4);
			// north-east to south-west
			memcpy(&dataTemp[(y + m_pImage->GetHeight() + 32) * pTemp->GetWidth() * 4 + (m_pImage->GetWidth() + 32) * 4], 
				&dataOrig[y * m_pImage->GetWidth() * 4], 
				128);
			// north-west to south-east
			memcpy(&dataTemp[(y + m_pImage->GetHeight() + 32) * pTemp->GetWidth() * 4], 
				&dataOrig[y * m_pImage->GetWidth() * 4 + (m_pImage->GetWidth() - 32) * 4], 
				128);
			// south-east to north-west
			memcpy(&dataTemp[y * pTemp->GetWidth() * 4 + (m_pImage->GetWidth() + 32) * 4], 
				&dataOrig[(y + m_pImage->GetHeight() - 32) * m_pImage->GetWidth() * 4], 
				128);
			// south-west to north-east
			memcpy(&dataTemp[y * pTemp->GetWidth() * 4], 
				&dataOrig[(y + m_pImage->GetHeight() - 32) * m_pImage->GetWidth() * 4 + (m_pImage->GetWidth() - 32) * 4], 
				128);
		}

		m_pImage->SetPixels(0,0,0,m_pImage->GetWidth(),m_pImage->GetHeight(),1,EIF_RGBA,dataOrig);
		pTemp->SetPixels(0,0,0,pTemp->GetWidth(),pTemp->GetHeight(),1,EIF_RGBA,dataTemp);

 		//// Center
 		//m_pImage->GetPixels(0, 0, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), 1, EIF_RGBA, data);
 		//pTemp->SetPixels( 16, 16, 0, m_pImage->GetWidth(), m_pImage->GetHeight(), 1, EIF_RGBA, data);
// 	
// 
//  		m_pImage->GetPixels(0, 0, 0, 16, m_pImage->GetHeight(), 1, EIF_RGBA, data);
//  		pTemp->SetPixels(m_pImage->GetWidth() + 16, 16, 0, 16, m_pImage->GetHeight(), 1, EIF_RGBA, data);

//  		m_pImage->GetPixels(m_pImage->GetWidth() - 16, 0, 0, 16, m_pImage->GetHeight(), 1, EIF_BGRA, data);
//  		pTemp->SetPixels(0, 16, 0, 16, m_pImage->GetHeight(), 1, EIF_BGRA, data);
// 
//  		m_pImage->GetPixels(0, 0, 0, m_pImage->GetWidth(), 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(16, m_pImage->GetHeight() + 16, 0, m_pImage->GetWidth(), 16, 1, EIF_BGRA, data);
// 
//  		m_pImage->GetPixels(0, m_pImage->GetHeight() - 16, 0, m_pImage->GetWidth(), 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(16, 0, 0, m_pImage->GetWidth(), 16, 1, EIF_BGRA, data);
//  
//  		m_pImage->GetPixels(0, 0, 0, 16, 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(m_pImage->GetWidth() + 16, m_pImage->GetHeight() + 16, 0, 16, 16, 1, EIF_BGRA, data);
// 
//  		m_pImage->GetPixels(m_pImage->GetWidth() - 16, m_pImage->GetHeight() - 16, 0, 16, 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(0, 0, 0, 16, 16, 1, EIF_BGRA, data);
// 
//  		m_pImage->GetPixels(0, m_pImage->GetHeight() - 16, 0, 16, 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(m_pImage->GetWidth() + 16, 0, 0, 16, 16, 1, EIF_BGRA, data);
// 
//  		m_pImage->GetPixels(m_pImage->GetWidth() - 16, 0, 0, 16, 16, 1, EIF_BGRA, data);
//  		pTemp->SetPixels(0, m_pImage->GetHeight() + 16, 0, 16, 16, 1, EIF_BGRA, data);

	//	m_pImage->DecRef();
		m_pImage = pTemp;

		delete[] data;

		//m_pImage->BuildTileMipMap(NumCols, NumRows);
		m_pImage->BuildMipMaps();

		CreateTile(m_pImage->GetWidth(), m_pImage->GetHeight(), eTileType, NumCols
			, NumRows);

		return true;

	} // CreateTile(FileName)

	//------------------------------------------------------------------------------------
	// Create the tile data. It's good for precreated tile textures.
	//------------------------------------------------------------------------------------
	void Tile::CreateTile(int Width, int Height, TileType eTileType, int NumCols, int NumRows)
	{
		m_eTileType = eTileType;
		m_iNumCols  = NumCols;
		m_iNumRows  = NumRows;
		m_iWidth    = Width;
		m_iHeight   = Height;
		m_fUOffset	= 2.0f / (float)Width;
		m_fVOffset  = 2.0f / (float)Height;
		m_fU		= 1.0f / (float)NumCols - m_fUOffset / (float)NumCols;
		m_fV		= 1.0f / (float)NumRows - m_fVOffset / (float)NumRows;

	} // CreateTile

	//------------------------------------------------------------------------------------
	// Returns the tile texcoords.
	//------------------------------------------------------------------------------------
	math::Vector Tile::GetTile( int Col, int Row )
	{
		Col = Col % m_iNumCols;
		Row = Row % m_iNumRows;
		Vertex2 v;
		v.X = Col * m_fU + m_fUOffset;
		v.Y = Row * m_fV + m_fVOffset;

		return math::Vector(v.X, v.Y, v.X + m_fU, v.Y + m_fV);

	} // GetTile

} // gfx

} // kge
