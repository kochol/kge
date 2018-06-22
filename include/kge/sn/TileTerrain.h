// File name: TileTerrain.h
// Des: This is a tile based terrain class good for strategic games
// Date: 22/11/1387
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_TILETERRAIN_H
#define KGE_TILETERRAIN_H

#include <map>

#include "SceneNode.h"
#include "Decal.h"
#include "Mesh.h"
#include "../gfx/Structs.h"
#include "../gfx/TileTexture.h"
#include "../gfx/Texture.h"
#include "../ph/DynamicTriangleMesh.h"
#include "../core/Timer.h"

namespace kge
{
namespace gfx
{
	class DynamicVertexBufferDX;
	class DynamicIndexBufferDX;

} // gfx

namespace sn
{
struct TileTerrainData
{
	float			Height;
	math::Vector  * LeftNormal,
				  * RightNormal;
	u32				TileID;

}; // TileTerrainData

struct RemovedDecal
{
	int						vbOffset;
	int						ibOffset;
	int						VertexSize;

}; // RemovedDecal

struct DecalData
{
	int						vbOffset;
	int						ibOffset;
	int						vbSizeTotal;
	int						ibSizeTotal;
	gfx::HardwareBuffer	*	VertexBuffer;
	gfx::HardwareBuffer	*	IndexBuffer;
	gfx::Vertex3T		*	Vertices;
	std::vector<RemovedDecal>
							Space;
	std::vector<Decal*>		Decals;

}; // DecalData

class KGE_API TileTerrain : public SceneNode
{
public:

	//! Constructor
	TileTerrain();

	//! Destructor
	virtual ~TileTerrain();

	//! Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! Render the terrain
	virtual void Render();

	//! Karhaee ke bad az render bayad anjam beshe.
	virtual void PostRender();

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut );

	/*! Draw the node with/without its Materials and Transforms.
		\param WithMaterial Draw it with its own materials or not?
		\param WithMaterial Draw it with its own Transformations or not?
	 */
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	//! Creates a tile based terrain with the given size
	virtual void ReCreate(int numCols, int numRows, float MinHeight = 0.0f, float MaxHeight = 0.0f);

	//! Sets the tile texture
	void SetTileTexture(gfx::TileTexture * pTileTex);

	//! Sets the tile texture
	void SetNormalMapTileTexture(gfx::TileTexture * pTileTex);

	//! Returns the tile texture pointer then you can modify it directly
	gfx::TileTexture* GetTileTexture() { return m_pTileTexture; }

	//! Sets the tile in the terrain.
	virtual void SetTile(int Col, int Row, u32 TileID);

	//! Sets the tiles
	void SetTiles(u32 tileID);

	//! Blend the tile stored in the given position and their same tiles attached to it.
	void Blend(int Col, int Row);

	//! Fill the terrain with the given tile ID
	void Fill(int Col, int Row, int TileID);

	//! Returns the terrain height at the given grid position
	float GetHeight(float x, float z);

	//! Sets the terrain height by the index id. 
	void SetHeight(int IndexID, float h);

	//! Sets the terrain height by the tile position.
	void SetHeight(int Col, int Row, float h);

	//! Sets the terrain color by the index id. 
	void SetVertexColor(int IndexID, gfx::Color c);

	//! Sets the terrain color by the tile position.
	void SetVertexColor(int Col, int Row, gfx::Color c);

	//! Adds the terrain height by the index id. 
	void SetHeightAdd(int IndexID, float h);

	//! Adds the terrain height by the tile position.
	void SetHeightAdd(int Col, int Row, float h);

	//! Smooths the tile height
	void Smooth(int Col, int Row);

	//! Smooths one node height
	void SmoothOneNode(int Col, int Row);

	//! Gets the column number and row number by Face index.
	math::Vector2I GetTileByFaceID(int FaceID);

	//! Returns the terrain position vertices
	gfx::Vertex3* GetPositions() { return m_pPos; }

	//! Create an array of Y position vertices and return 
	void GetYs(float* inArray);

	//! Sets the heights 
	void SetYs(float* pHeights);

	//! Returns the tile count if you pass the NULL and set them otherwise
	int GetTiles(int *pTiles1, int* pTiles2);

	//! Sets the tiles
	void SetTiles(int *pTiles1, int* pTiles2);

	//! Returns blend map size if you pass NULL and set the pointer otherwise
	int GetBlendMap(u8* pBlendMap);

	//! Sets the blend map
	void SetBlendMap(u8* pBlendMap);

	//! Copy the vertex colors to the pointer
	void GetVertexColors(ul32* pVertexColor);

	//! Copy the vertex colors from the pointer
	void SetVertexColors(ul32* pVertexColor);

	//! Returns the count of the vertexes.
	int GetVertexCount() { return m_iNumVerts; }

	//! Returns the terrain indexes
	u32* GetIndices() { return m_pIndices; }

	//! Returns the terrain indexes count
	int GetIndexesCount() { return m_iNumIndices; }

	//! Returns the DynamicTriangleMesh pointer
	ph::DynamicTriangleMesh* GetDynamicTriangleMesh() { return m_pDTM; }

	//! Returns the DynamicTriangleMesh pointer and send updates to it if needed.
	void SetDynamicTriangleMesh(ph::DynamicTriangleMesh* pDTM);

	//! Recalculate the terrain normals
	void UpdateNormals();

	//! Return the number of AABB that splited the terrain.
	int GetAABBcount() { return m_iNumUparts * m_iNumVparts; }

	//! Returns the AABBs array pointer
	math::AABB* GetAABBs() { return m_pAABB; }

	//! Returns the number of ... in U
	int GetUcount() { return m_iNumUparts; }

	//! Returns the number of ... in V
	int GetVcount() { return m_iNumVparts; }

	//! Create a decal and return its pointer.
	Decal* CreateDecal(int sizeX, int sizeY, gfx::Texture* pTex);

	//! Sets the decal on position
	void SetDecal(float fx, float fy, Decal* pDecal);

	//! Get the count of materials this terrain has. always returns 1
	virtual u32 GetMaterialCount() { return 1; }

	//! Get the terrain material.	
	virtual gfx::Material* GetMaterial(u32 Index) { return m_pMaterial; }

	//! Creates the terrain shader code
	virtual void CreateShaderCode(core::String &VertexCode, core::String &PixelCode);

	//! Enables the under water vertex coloring effect.
	void EnableWater(bool Enable, const gfx::Colorf& cStart = gfx::Colorf(255, 255, 255),
					 const gfx::Colorf& cEnd = gfx::Colorf(0, 229, 255));

	//! Enable and sets the caustics textures.
	void SetCaustics(gfx::Texture** ppTextures, int TexturesCount, int speed = 100);

	//! Sets the Blend Map
	void SetBlendMap(gfx::Texture* pTexture) { m_pBlendMap = pTexture; }

	//! Sets the shadow receiver params
	void ReceiveShadow(const char* Code, math::Matrix* shadowmat);

	/// Returns the columns count
	int GetColumnsCount() { return m_iNumCols; }

	/// Returns the rows count
	int GetRowsCount() { return m_iNumRows; }

	//! Render decals
	void RenderDecals();

	//! Empty space in vertex buffer
	void RemoveDecal(Decal* pDecal);

	//! 
	void OnLost();

	//! Refills vertex and index buffers after reseting device
	void OnReset();

	//! 
	void RecreateShaders();

	//! Shaders					
	gfx::Shader					* m_pVshader,
								* m_pPshader;

protected:

	//! Terrain data
	std::vector<TileTerrainData> m_vTerrainData;

	//! Visible parts
	std::vector<int>			  m_vVisibleParts;
	
	//! Vertices
	gfx::Vertex3				* m_pPos;
	gfx::VertexNTC				* m_pNorTexColor;

	sn::Mesh					* m_pMesh;

	//! Normal lines
	gfx::Vertex3C				* m_pNormalVerts;

	//! Normal lines color.
	gfx::Color					  m_cNormal[4];

	//! Indices
	u32							* m_pIndices;

	//! Tile infos
	int							* m_pTiles,
								* m_pTiles2;
	int							* m_pTilesTemp;
	u8							* m_pBlendTiles;

	//! Tile width and height size
	float						  m_fWidth,
								  m_fHeight,
								  m_fXOffset,
								  m_fYOffset;

	//! Count
	int							  m_iNumVerts,
								  m_iNumIndices,
								  m_iNumCols,
								  m_iNumRows,
								  m_iNumColspp,
								  m_iNumRowspp,
								  m_iNumUparts,
								  m_iNumVparts,
								  m_iNumCausticsTextures,
								  m_iCurrentCaustic,
								  m_iTextureIDSizeX,
								  m_iTextureIDSizeY;

	core::Timer					* m_pCausticsTimer;

	// Shader handles
	gfx::ShaderHandle			  m_shLit,
								  m_shWVP,
								  m_shView,
								  m_shShadowMat,
								  m_shfvEyePosition;

	// Water , caustics
	bool						  m_bWater,
								  m_bCaustics,
								  m_bPosChanged,
								  m_bNorTexChanged,
								  m_bReceiveShadow,
								  m_bTileTerrain2;

	//! Shadow code
	const char					* m_pShadowShaderCode;
	math::Matrix				* m_pShadowMatrix;

	// caustics textures array
	gfx::Texture				**m_ppCausticsTextures;
	//! Terrain Texture ID for storing the tile data.
	gfx::Texture				* m_pTextureID;
	gfx::Texture				* m_pTextureBlend;
	gfx::Texture				* m_pBlendMap;

	// Water colors
	gfx::Colorf					  m_cStart,
								  m_cEnd;

	//! Tile texture pointer
	gfx::TileTexture			* m_pTileTexture,
								* m_pNormalMapTileTexture;

	//! Dynamic Triangle Mesh class pointer
	ph::DynamicTriangleMesh		* m_pDTM;

	//! Material
	gfx::Material				* m_pMaterial;

	//! Decal shader
	gfx::Shader					*	m_pVSdecal,
								*	m_pPSdecal;
	kge::core::Functor1
		<void,
		gfx::ShaderInstance*>	*	m_pDecalFun;
	gfx::ShaderHandle				m_shDecalWVP;
	gfx::ShaderHandle	            m_shDecalAlpha;

	// Decal VB & IB data
	std::map<u32, DecalData*>		m_mDecalBufferData;


	void Flood_Fill(int Col, int Row, int TileID);
	void Flood_Fill(int Col, int Row, int ReplaceID, int TileID);

	void SetTile(int Col, int Row, int TileID, int TileID2);

	// Sets the shader params
	virtual void SetShaderParams();

	virtual void userSetShaderParams() {}

	// Sets Decal Shader Params
	void SetDecalShaderParams(gfx::ShaderInstance* pSI);

}; // TileTerrain

} // sn

} // kge

#endif // KGE_TILETERRAIN_H
