// File name: TileTerrain2.h
// Des: This is a tile based terrain class good for strategic games
// Date: 26/10/1390
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_TILETERRAIN2_H
#define KGE_TILETERRAIN2_H

#include "TileTerrain.h"

namespace kge
{
	namespace gfx
	{
		
	} // gfx

	namespace gui
	{
	} // gui

	namespace sn
	{
		class KGE_API TileTerrain2: public TileTerrain
		{
		public:

			//! Constructor
			TileTerrain2();

			//! Destructor
			~TileTerrain2();

			//! Create the terrain if already created then recreate it.
			void ReCreate(int numCols, int numRows, float MinHeight = 0.0f, float MaxHeight = 0.0f);

			//! Creates the terrain shader code
			void CreateShaderCode(core::String &VertexCode, core::String &PixelCode);

			//! Creates the terrain shader code for deferred shading
			void CreateDeferredShaderCode(core::String &VertexCode, core::String &PixelCode);

			//! Render the terrain
			void Render();

			//! Sets the brush texture and shape
			void SetBrush(char* FileName);

			//! Sets the brush size
			void SetBrushSize(float fSize);

			//! Brush the tiles
			void BrushTile(float x, float y, int tileid, float BlendFactor, bool ChangeBackground = false, bool Mode2 = false);

			//! Sets the tile in the terrain.
			void SetTile(int Col, int Row, u32 TileID); 

			//! Sets the tile texture
			void SetTileTexture
				(gfx::Texture* pTileTex, 
				 int Width, 
				 int Height,
				 int HorizantalTilesCount,
				 int VerticalTilesCount,
				 int TileSizeInPixel
				);


			int			GetMapsCount() const { return m_iMapsCount; }
			const u8*	GetBlendMapsData(int n) const;
			const u8*	GetTextureIDMapsData(int n) const;
			void		SetBlendMapsData(int n, u8* data, int size);
			void		SetTextureIDMapsData(int n, u8* data, int size);

		protected:

			//! Blend maps
			gfx::Texture			**	m_ppBlendMaps,			// A8
									**	m_ppTextureIDMaps;		// A8L8

			//! counts
			int							m_iMapsCount,
										m_iMapsSize,
										m_iMapsCX,
										m_iMapsCY,
										m_iTileW,
										m_iTileH,
										m_iTileSizeInPixel,
										m_iBitCount;

			float						m_fVTilesCountInverse,
										m_fHTilesCountInverse;

			// Brush
			gfx::Texture			*	m_pBrush;

			gfx::Image				*	m_pImgBrush,
									*	m_pImgScaledBrush;

			gfx::Shader				*	m_pVsBrush,
									*	m_pPsBrush;

			gfx::ShaderHandle	        m_shBrushBlend,
										m_shDirLightColor;

			float						m_fBrushSize;

			kge::math::Matrix			m_m2D;

			// Check the collisions between blend maps and brush
			struct MapIDData 
			{
				int id;
				int x;
				int y;
				int sx;
				int sy;
			};
			std::vector<TileTerrain2::MapIDData> GetBlendMapIDs(float x, float y);

			// Sets Decal Shader Params
			void SetBrushShaderParams(gfx::ShaderInstance* pSI);

			void userSetShaderParams();

		}; // TileTerrain2

	} // sn

} // kge

#endif // KGE_TILETERRAIN2_H
