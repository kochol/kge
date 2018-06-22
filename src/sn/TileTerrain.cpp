// File name: TileTerrain.cpp
// Des: This is a tile based terrain class good for strategic games
// Date: 22/11/1387
// Programmers: Ali Akbar Mohammadi (Kochol)

#include <stdio.h>
#include "../../include/kge/sn/TileTerrain.h"
#include "../../include/kge/gfx/Renderer.h"
#include "../../include/kge/gfx/MeshManager.h"
#include "../../include/kge/core/mem_fun.h"
#include "../../include/kge/sn/Light.h"
//#include "../../include/kge/io/File.h"
#include <queue>



typedef std::map<kge::u32, kge::sn::DecalData*>	MapDecalData;

namespace kge
{
	namespace sn
	{
		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		TileTerrain::TileTerrain() :
	m_pIndices(NULL), m_fWidth(1.0f), m_fHeight(1.0f),
		m_iNumVerts(0), m_iNumIndices(0), m_iNumCols(0), m_iNumRows(0),
		m_fXOffset(-0.5f), m_fYOffset(-0.5f), m_pTileTexture(NULL),
		m_pTextureID(NULL), m_pDTM(NULL), m_pTiles(NULL), m_pBlendTiles(NULL),
		m_iNumColspp(0), m_iNumRowspp(0), m_pTilesTemp(NULL), m_pTextureBlend(NULL),
		m_pTiles2(NULL), m_bWater(false), m_bCaustics(false), m_ppCausticsTextures(NULL),
		m_iCurrentCaustic(0), m_pCausticsTimer(NULL), m_pBlendMap(NULL), m_pVshader(NULL),
		m_pPshader(NULL), m_pMesh(NULL), m_pPos(NULL), m_pNorTexColor(NULL), m_bPosChanged(false),
		m_bNorTexChanged(false), m_bReceiveShadow(false), m_pShadowShaderCode(NULL),
		m_pShadowMatrix(NULL), m_pNormalMapTileTexture(NULL), m_pNormalVerts(NULL),
		m_shDecalWVP(0), m_shDecalAlpha(0), m_bTileTerrain2(false)
	{
		m_eNodeType	   = ENT_TileTerrain;
		m_pAABB		   = NULL;
		m_cNormal[0].c = 0x00ff0000;
		m_cNormal[1].c = 0x0000ff00;
		m_cNormal[2].c = 0x000000ff;
		m_cNormal[3].c = 0x00ffffff;
		m_pMaterial	   = KGE_NEW(gfx::Material)();

		const char chv[] = "float4x4 matViewProjection;\n"\
			"struct VS_INPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Texcoord : TEXCOORD0;\n"\
			"};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"	float4 Position : POSITION0;\n"\
			"	float2 Texcoord : TEXCOORD0;  \n"\
			"};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"	VS_OUTPUT Output;\n"\
			"	Output.Position = mul( Input.Position, matViewProjection );\n"\
			"	Output.Texcoord = Input.Texcoord;\n"\
			"	return( Output );\n"\
			"}";

		const char chp[] = "float fAlpha;"\
			"sampler2D baseMap;\n"\
			"struct PS_INPUT \n"\
			"{\n"\
			"	float2 Texcoord : TEXCOORD0;\n"\
			"};\n"\
			"float4 main( PS_INPUT Input ) : COLOR0\n"\
			"{\n"\
			"	float4 c = tex2D( baseMap, Input.Texcoord );\n"\
			"	c.a *= fAlpha;\n"\
			"	return c;\n"\
			"}";

		m_pVSdecal   = gfx::Renderer::GetSingletonPtr()->
			CreateVertexShaderFromString(chv, "main", gfx::ESV_VS2);
		m_shDecalWVP = m_pVSdecal->GetConstatnt("matViewProjection");
		m_pPSdecal   = gfx::Renderer::GetSingletonPtr()->
			CreatePixelShaderFromString(chp, "main", gfx::ESV_PS2);
		m_shDecalAlpha = m_pPSdecal->GetConstatnt("fAlpha");
		m_pDecalFun  = core::mem_fun(this, &TileTerrain::SetDecalShaderParams);

	} // Constructor

	//------------------------------------------------------------------------------------
	// Destructor
	//------------------------------------------------------------------------------------
	TileTerrain::~TileTerrain()
	{
		m_vTerrainData.clear();

		KGE_DELETE_ARRAY(m_pTiles);
		KGE_DELETE_ARRAY(m_pTiles2);
		KGE_DELETE_ARRAY(m_pTilesTemp);
		KGE_DELETE_ARRAY(m_pBlendTiles);
		KGE_DELETE_ARRAY(m_pNormalVerts);
		KGE_DELETE(m_pMaterial, Material);
		KGE_DELETE(m_pCausticsTimer, Timer);
		KGE_DELETE(m_pMesh, Mesh);
		KGE_DELETE_CLASS_ARRAY(m_pAABB, AABB, GetAABBcount());

	} // Destructor

	//------------------------------------------------------------------------------------
	// Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	//------------------------------------------------------------------------------------
	void TileTerrain::PreRender(float elapsedTime)
	{
		if (m_pParent)
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
		else
			*m_pFinalMat = *m_pAbsMat;

		if (m_bVis)
		{
			if (m_bPosChanged)
			{
				m_pMesh->m_pvbPosition->SetData(m_pPos, 0, m_iNumVerts);
				m_bPosChanged = false;
			}
			if (m_bNorTexChanged)
			{
				m_pMesh->m_pvbNormalTexcoord->SetData(m_pNorTexColor, 0, m_iNumVerts);
				if (m_pNormalMapTileTexture || m_pMaterial[0].ppTexture[1])
					m_pMesh->m_pvbTangentBinormal->SetData(m_pMesh->m_pTangentBinormal, 0, m_iNumVerts);
				m_bNorTexChanged = false;
			}
			if ( !m_bAutoCulling)
			{
				for (int i = 0; i < GetAABBcount(); i++)
				{
					m_vVisibleParts.push_back(i);
				}
			}
			else
			{
				m_vVisibleParts.clear();
				const math::Frustum* pFrus = m_pSnMan->GetActiveCamera()->GetFrustum();
				for (int i = 0; i < GetAABBcount(); i++)
				{
					if (pFrus->Collision(&m_pAABB[i]) != math::ECT_Out )
					{
						m_vVisibleParts.push_back(i);
						///m_pSnMan->m_pSceneAABB->AddAABB(&m_pAABB[i]);
					}
				}
			}
			// Check for caustics
			if (m_bCaustics)
			{
				if (m_pCausticsTimer->NextFrame())
				{
					m_iCurrentCaustic++;
					if (m_iCurrentCaustic >= m_iNumCausticsTextures)
						m_iCurrentCaustic = 0;
				}
			}

			m_pSnMan->RegNode(this);
		}
	} // PreRender

	//------------------------------------------------------------------------------------
	// Render the terrain
	//------------------------------------------------------------------------------------
	void TileTerrain::Render()
	{

		// Set decals data
		MapDecalData::iterator it;
		for (it = m_mDecalBufferData.begin(); it != m_mDecalBufferData.end(); it++)
		{
			it->second->VertexBuffer->SetData(it->second->Vertices, 0, it->second->vbOffset);
		}

		m_pRenderer->SetTransForm(m_pFinalMat);
		m_pRenderer->SetMaterial(m_pMaterial);
		if (!m_bTileTerrain2)
		{
			m_pVshader->PreRender();
			m_pVshader->Render();
			m_pPshader->Render();

			// Set textures
			m_pRenderer->SetTexture(m_pTileTexture->GetTexture());
			m_pRenderer->SetTexture(m_pBlendMap, 1);
			m_pRenderer->SetTexture(m_pTextureID, 2);
			m_pRenderer->SetTexture(m_pTextureBlend, 3);
			m_pRenderer->SetTextureParams(gfx::ETP_Point, 3);
		}
		if (m_bCaustics)
			m_pRenderer->SetTexture(m_ppCausticsTextures[m_iCurrentCaustic], 4);
		if(m_pNormalMapTileTexture)
		{
			m_pRenderer->SetTexture(m_pNormalMapTileTexture->GetTexture(), 5);
		}
		if (m_pMaterial[0].ppTexture[1])
			m_pRenderer->SetTexture(m_pMaterial[0].ppTexture[1], 5);

		m_pRenderer->SetVertexDec(m_pMesh->m_pVertexDec);
		m_pRenderer->SetVertexBuffer(m_pMesh->m_pvbPosition);
		m_pRenderer->SetVertexBuffer(m_pMesh->m_pvbNormalTexcoord, 1);
		m_pRenderer->SetVertexBuffer(m_pMesh->m_pvbTangentBinormal, 2);
		m_pRenderer->SetIndexBuffer(m_pMesh->m_pIndexBuffer);
		if (!m_bTileTerrain2)
		{
			const size_t m_vVisiblePartsSize = m_vVisibleParts.size();
			for (size_t i = 0; i < m_vVisiblePartsSize; ++i)
			{
				m_pRenderer->DrawTriangleList(m_iNumVerts, 1536, m_pMesh->m_pVertexDec, 0, m_vVisibleParts[i] * 1536);
			}
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 3);
		}
#ifdef DEBUG
		//  		for (int i = 0; i < m_iNumUparts * m_iNumVparts; i++)
		//  		{
		//  			m_pAABB[i].DebugRender();
		//  		}
		//m_pRenderer->Disable(gfx::ERF_Lighting);
		//m_pRenderer->SetTexture(MAXID);
		//m_pRenderer->DrawLineList(m_pNormalVerts, m_iNumVerts * 2, 0, 0);
		//m_pRenderer->Enable(gfx::ERF_Lighting);
#endif // DEBUG

	} // Render

	//------------------------------------------------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//------------------------------------------------------------------------------------
	void TileTerrain::PostRender()
	{
	} // PostRender

	//------------------------------------------------------------------------------------
	// Ezafe kardane effect be  hamin gereh.
	//------------------------------------------------------------------------------------
	bool TileTerrain::AddEffect ( efx::EffectType p, efx::Effect** ppOut )
	{
		return false;

	} // AddEffect

	//------------------------------------------------------------------------------------
	// Draw the node with/without its Materials and Transforms.
	//------------------------------------------------------------------------------------
	void TileTerrain::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{
	} // Draw

	//------------------------------------------------------------------------------------
	// Creates a tile based terrain with the given size
	//------------------------------------------------------------------------------------
	void TileTerrain::ReCreate(int numCols, int numRows, float MinHeight, float MaxHeight)
	{
		if (!m_bTileTerrain2)
		{
			if (!m_pTileTexture)
			{
				io::Logger::Log("You have to assign a TileTexture to init this terrain.", io::ELM_Error);
				return;
			}

			// Create the TextureID
			int tx, ty, mip;
			tx = gfx::Texture::GetPowerOf2Size(numCols, mip);
			ty = gfx::Texture::GetPowerOf2Size(numRows, mip);
			if (m_pTextureID)
				m_pTextureID->DecRef();
			int handle   = Device::GetSingletonPtr()->GetTextureManager()->Add(NULL, NULL, "TextureID");
			m_pTextureID = Device::GetSingletonPtr()->GetTextureManager()->GetResource(handle);
			m_pTextureID->CreateTexture(tx, ty, gfx::ETF_A32B32G32R32F, 1);
			m_iTextureIDSizeX = tx;
			m_iTextureIDSizeY = ty;
			float* f = KGE_NEW_ARRAY(float, tx * ty * 4);
			int cfcf = tx * 4;
			float cc = 0.0f, ccy = 0.0f;
			for (int r = 0; r < numRows; r++)
			{
				cc = 0.0f;
				for (int c = 0; c < numCols; c++)
				{
					f[r * cfcf + c * 4] = cc;
					f[r * cfcf + c * 4 + 1] = ccy;
					f[r * cfcf + c * 4 + 2] = 0;
					f[r * cfcf + c * 4 + 3] = 0;
					cc += m_pTileTexture->GetU() - 4.0f * m_pTileTexture->GetdU();
				}
				ccy += m_pTileTexture->GetV() - 4.0f * m_pTileTexture->GetdV();
			}
			io::Logger::Log(io::ELM_Warning, "TextureID %f %f", m_pTileTexture->GetU(), m_pTileTexture->GetV());
			m_pTextureID->SetData((u8*)f, tx * ty * 4 * 4);
			KGE_DELETE_ARRAY(f);

			// Create texture blend
			if (m_pTextureBlend)
				m_pTextureBlend->DecRef();
			handle			= Device::GetSingletonPtr()->GetTextureManager()->Add(NULL, NULL, "TextureBlend");
			m_pTextureBlend = Device::GetSingletonPtr()->GetTextureManager()->GetResource(handle);
			m_pTextureBlend->CreateTexture(tx, ty, gfx::ETF_A8, 1);
			u8* temp = KGE_NEW_ARRAY(u8, tx * ty);
			memset(temp, 255, tx * ty);
			m_pTextureBlend->SetData(temp, tx * ty);
			KGE_DELETE_ARRAY(temp);

			// Create tile data holders.
			KGE_DELETE_ARRAY(m_pTiles);
			KGE_DELETE_ARRAY(m_pTiles2);
			KGE_DELETE_ARRAY(m_pTilesTemp);
			KGE_DELETE_ARRAY(m_pBlendTiles);

			m_pTiles	  = KGE_NEW_ARRAY(int, numCols * numRows);
			m_pTiles2	  = KGE_NEW_ARRAY(int, numCols * numRows);
			m_pTilesTemp  = KGE_NEW_ARRAY(int, numCols * numRows);
			m_pBlendTiles = KGE_NEW_ARRAY(u8 , numCols * numRows);
		}

		// Clear the memory
		m_vTerrainData.clear();
		KGE_DELETE_ARRAY(m_pPos);
		KGE_DELETE_ARRAY(m_pNorTexColor);
		KGE_DELETE_ARRAY(m_pIndices);
		KGE_DELETE(m_pMesh, Mesh);
		KGE_DELETE_CLASS_ARRAY(m_pAABB, AABB, GetAABBcount());

		m_iNumCols		=  numCols;
		m_iNumRows		=  numRows;
		m_iNumColspp    =  numCols + 1;
		m_iNumRowspp	=  numRows + 1;
		m_iNumVerts		= (numRows + 1) * (numCols + 1);
		m_iNumIndices	=  numRows      *  numCols      * 6;

		bool bRand = false;
		float DHeight = MaxHeight - MinHeight;
		printf("%f\t%f\t%f\n", DHeight, MaxHeight, MinHeight);
		if (DHeight > 0.0f)
		{
			bRand = true;
		}

		int k	 = 0;
		int index= 0;

		m_pPos			= KGE_NEW_ARRAY(gfx::Vertex3, m_iNumVerts);
		m_pNorTexColor	= KGE_NEW_ARRAY(gfx::VertexNTC, m_iNumVerts);
		m_pMesh     = KGE_NEW(sn::Mesh)();

		for (int i = 0; i <= numRows; i++)
		{
			for (int j = 0; j <= numCols; j++)
			{
				// Create terrain data
				TileTerrainData t;
				if (bRand)
				{
					t.Height  = (float)(rand() % 101) / 100.0f * DHeight + MinHeight;
				}
				else
					t.Height  = MinHeight;
				t.TileID      = 0;
				m_vTerrainData.push_back(t);

				// Create vectors
				m_pPos[index].X			 = j * m_fWidth + m_fXOffset ;
				m_pPos[index].Y			 = t.Height;
				m_pPos[index].Z			 = i * m_fHeight + m_fYOffset;
				m_pNorTexColor[index].tex.X   = (float)j;
				m_pNorTexColor[index].tex.Y   = (float)i;
				m_pNorTexColor[index].Color   = 0xffffffff;
				index++;

			} // for j

		} // for i

		// Create Indexes and split the terrain.
		int iu, iv;
		iu			  = ceil((float)numCols / 16.0f);
		iv			  = ceil((float)numRows / 16.0f);
		m_iNumUparts  = iu;
		m_iNumVparts  = iv;
		m_pAABB		  = KGE_NEW_CLASS_ARRAY(math::AABB, iu * iv);
		m_pIndices	  = KGE_NEW_ARRAY(u32, m_iNumIndices);
		k		      = 0;
		int numColspp = numCols + 1;

		for (int v = 0; v < iv; v++)
		{
			for (int u = 0; u < iu; u++)
			{
				for (int i = v * 16; i < (v + 1) * 16; i++)
				{
					if (i >= numRows)
						break;
					for (int j = u * 16; j < (u + 1) * 16; j++)
					{
						if (j >= numCols)
							break;
						m_pIndices[k    ] = (i * numColspp + j) + 1;
						m_pIndices[k + 1] = (i * numColspp + j);
						m_pIndices[k + 2] = ((i + 1) * numColspp + j);

						m_pIndices[k + 3] = (i * numColspp + j) + 1;
						m_pIndices[k + 4] = ((i + 1) * numColspp + j);
						m_pIndices[k + 5] = ((i + 1) * numColspp + j) + 1;

						m_pAABB[v * iu + u].AddInternalPoint(&m_pPos[m_pIndices[k    ]]);
						m_pAABB[v * iu + u].AddInternalPoint(&m_pPos[m_pIndices[k + 1]]);
						m_pAABB[v * iu + u].AddInternalPoint(&m_pPos[m_pIndices[k + 2]]);
						m_pAABB[v * iu + u].AddInternalPoint(&m_pPos[m_pIndices[k + 5]]);

						k += 6;

					} // for j

				} // for i

			} // for u

		} // for v

		// Create shader
		if (m_pVshader)
		{
			m_pVshader->DecRef();
			m_pVshader = NULL;
		}
		if (m_pPshader)
		{
			m_pPshader->DecRef();
			m_pPshader = NULL;
		}

		core::String VertexShaderCode,
			PixelShaderCode;
		CreateShaderCode(VertexShaderCode, PixelShaderCode);

		m_pVshader = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString
			(VertexShaderCode.ToCharPointer(), "main", gfx::ESV_VS3);
		m_pPshader = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString
			(PixelShaderCode.ToCharPointer(), "main", gfx::ESV_PS3);
		m_pVshader->ConnectOnPreRender(core::mem_fun(this, &TileTerrain::SetShaderParams));
		m_shLit	   = m_pVshader->GetConstatnt("DirLit");
		m_shWVP	   = m_pVshader->GetConstatnt("matViewProjection");
		m_shView   = m_pVshader->GetConstatnt("matView");
		m_shfvEyePosition = m_pVshader->GetConstatnt("fvEyePosition");
		if (m_bReceiveShadow)
			m_shShadowMat = m_pVshader->GetConstatnt("matLight");

		// Calculate Normals
#ifdef DEBUG
		KGE_DELETE_ARRAY(m_pNormalVerts);
		m_pNormalVerts     = KGE_NEW_ARRAY(gfx::Vertex3C, m_iNumVerts * 2);
#endif // DEBUG

		gfx::RecalculateNormals_Smooth32<gfx::VertexNTC>(m_pPos, m_pNorTexColor, m_iNumVerts, m_pIndices, m_iNumIndices);

		// Create Mesh
		m_pMesh->m_pPos			= m_pPos;
		m_pMesh->m_pNorTexColor	= m_pNorTexColor;
		m_pMesh->m_sName		= "Tile Terrain Mesh";
		m_pMesh->m_pIndices32   = m_pIndices;
		m_pMesh->m_NumVerts     = m_iNumVerts;
		m_pMesh->m_NumIndices   = m_iNumIndices;
		m_pMesh->m_pRenderer	= m_pRenderer;
		m_pMesh->m_pVertexDec   = m_pRenderer->GetVertexDec(gfx::EVT_P0NTC1BT2);
		if (m_pNormalMapTileTexture || m_pMaterial[0].ppTexture[1])
			m_pMesh->PrepareBuffers(true, true);
		else
			m_pMesh->PrepareBuffers(false, true);

		m_pRenderer->SetTextureParams(gfx::ETP_Point, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Mirror, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 4);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 5);

		// Create Normal lines for debug.
		for (int i = 0; i <= numRows; i++)
		{
			for (int j = 0; j <= numCols; j++)
			{
				//m_vTerrainData[i * numCols + j].LeftNormal->set( m_pVertices[(i * numColspp + j)].Nor.X
				//												,m_pVertices[(i * numColspp + j)].Nor.Y
				//												,m_pVertices[(i * numColspp + j)].Nor.Z);
				//m_vTerrainData[i * numCols + j].RightNormal->set(m_pVertices[((i + 1) * numCols + j) + 1].Nor.X
				//												,m_pVertices[((i + 1) * numCols + j) + 1].Nor.Y
				//												,m_pVertices[((i + 1) * numCols + j) + 1].Nor.Z);
#ifdef DEBUG
				m_pNormalVerts[(i * numColspp + j) * 2 ].c   = m_cNormal[0];
				m_pNormalVerts[(i * numColspp + j) * 2 ].pos = m_pPos[(i * numColspp + j)];
				m_pNormalVerts[(i * numColspp + j) * 2 + 1].c   = m_cNormal[0];
				m_pNormalVerts[(i * numColspp + j) * 2 + 1].pos = m_pPos[(i * numColspp + j)] + m_pNorTexColor[(i * numColspp + j)].Nor;
#endif // DEBUG
			}
		}

		if (m_pDTM)
			m_pDTM->Init(this);

	} // ReCreate

	//------------------------------------------------------------------------------------
	// Sets the tile texture
	//------------------------------------------------------------------------------------
	void TileTerrain::SetTileTexture(gfx::TileTexture * pTileTex)
	{
		pTileTex->AddRef();
		m_pTileTexture = pTileTex;

	} // SetTileTexture

	//------------------------------------------------------------------------------------
	// Sets the tile in the terrain.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetTile(int Col, int Row, u32 TileID)
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		if (TileID < 0 || TileID >= m_pTileTexture->GetTileCount())
			return;

		math::Vector v = m_pTileTexture->GetTile(TileID)->GetTile(Col, Row);
		float f[4];
		f[0] = (float)(Col * m_pTileTexture->GetU()) - v.x;
		f[1] = (float)(Row * m_pTileTexture->GetV()) - v.y;
		f[2] = 0;
		f[3] = 0;

		m_pTextureID->SetData(Col, Row, 1, 1, (u8*)f, 16);

		m_pTiles[Row * m_iNumCols + Col]	  = TileID;
		m_pTiles2[Row * m_iNumCols + Col]	  = -1;
		m_pBlendTiles[Row * m_iNumCols + Col] = 255;
		m_pTextureBlend->SetData(Col, Row, 1, 1 , &m_pBlendTiles[Row * m_iNumCols + Col], 1);

	} // SetTile

	void TileTerrain::SetTile( int Col, int Row, int TileID, int TileID2 )
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		if (TileID < 0 || TileID >= m_pTileTexture->GetTileCount())
			return;

		if (TileID == TileID2)
			return;

		math::Vector v  = m_pTileTexture->GetTile(TileID)->GetTile(Col, Row);
		math::Vector v2;
		float f[4];
		f[0] = (float)(Col * m_pTileTexture->GetU()) - v.x;
		f[1] = (float)(Row * m_pTileTexture->GetV()) - v.y;
		if (TileID2 >= 0)
		{
			v2 = m_pTileTexture->GetTile(TileID2)->GetTile(Col, Row);
			f[2] = (float)(Col * m_pTileTexture->GetU()) - v2.x;
			f[3] = (float)(Row * m_pTileTexture->GetV()) - v2.y;
		}
		else
		{
			f[2] = 0;
			f[3] = 0;
		}

		m_pTextureID->SetData(Col, Row, 1, 1, (u8*)f, 16);

		m_pTiles[Row * m_iNumCols + Col]	  = TileID;
		m_pTiles2[Row * m_iNumCols + Col]	  = TileID2;
	}

	//------------------------------------------------------------------------------------
	// Sets the tiles in the terrain.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetTiles(u32 tileID)
	{
		if (tileID < 0 || tileID >= m_pTileTexture->GetTileCount())
			return;

		float* data = KGE_NEW_ARRAY(float, 4 * m_iNumRows * m_iNumCols);
		int c = 0;

		for (int i = 0; i < m_iNumRows; ++i)
		{
			for (int j = 0; j < m_iNumCols; ++j)
			{
				math::Vector v = m_pTileTexture->GetTile(tileID)->GetTile(j, i);
				data[c] = (j * m_pTileTexture->GetU()) - v.x;
				data[c + 1] = (i * m_pTileTexture->GetV()) - v.y;
				data[c + 2] = 0;
				data[c + 3] = 0;
				m_pTiles[i * m_iNumCols + j] = tileID;
				m_pTiles2[i * m_iNumCols + j] = -1;
				m_pBlendTiles[i * m_iNumCols + j] = 255;
				c += 4;
			}
		}

		m_pTextureID->SetData(0, 0, m_iNumRows, m_iNumCols, (u8*)data, 16 * m_iNumRows * m_iNumCols);
		m_pTextureBlend->SetData(0, 0, m_iNumRows, m_iNumCols , m_pBlendTiles, m_iNumRows * m_iNumCols);

		KGE_DELETE_ARRAY(data);
	}

	//------------------------------------------------------------------------------------
	// Blend the tile stored in the given position and their same tiles attached to it.
	//------------------------------------------------------------------------------------
	void TileTerrain::Blend( int Col, int Row )
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		int tile = m_pTiles[Row * m_iNumCols + Col];
		memset(m_pTilesTemp, 0, m_iNumCols * m_iNumRows * 4);

		Flood_Fill(Col, Row, tile);

		for (int r = 1; r < m_iNumRows - 1; r++)
		{
			for (int c = 1; c < m_iNumCols - 1; c++)
			{
				if (m_pTilesTemp[r * m_iNumCols + c] == 0)
				{
					int i = 0;
					if (m_pTilesTemp[r * m_iNumCols + c + 1] == 2)
						i++;
					if (m_pTilesTemp[r * m_iNumCols + c - 1] == 2)
						i++;
					if (m_pTilesTemp[(r + 1) * m_iNumCols + c] == 2)
						i++;
					if (m_pTilesTemp[(r - 1) * m_iNumCols + c] == 2)
						i++;
					if (i > 1)
					{
						m_pTilesTemp[r * m_iNumCols + c] = 3;
					}
				}
			}
		}

		for (int r = 1; r < m_iNumRows - 1; r++)
		{
			for (int c = 1; c < m_iNumCols - 1; c++)
			{
				u32 t = 0;
				if (m_pTilesTemp[r * m_iNumCols + c] == 2 ||
					m_pTilesTemp[r * m_iNumCols + c] == 3)
				{
					if (m_pBlendTiles[r * m_iNumCols + c] == 255)
					{
						if (m_pTiles[(r - 1) * m_iNumCols + c - 1] == tile)
							t |= 1;
						if (m_pTiles[(r - 1) * m_iNumCols + c] == tile)
							t |= 2;
						if (m_pTiles[(r - 1) * m_iNumCols + c + 1] == tile)
							t |= 4;
						if (m_pTiles[r * m_iNumCols + c - 1] == tile)
							t |= 8;
						if (m_pTiles[r * m_iNumCols + c + 1] == tile)
							t |= 16;
						if (m_pTiles[(r + 1) * m_iNumCols + c - 1] == tile)
							t |= 32;
						if (m_pTiles[(r + 1) * m_iNumCols + c] == tile)
							t |= 64;
						if (m_pTiles[(r + 1) * m_iNumCols + c + 1] == tile)
							t |= 128;

						printf("%d\n", t);
						if (t == 128)
							m_pBlendTiles[r * m_iNumCols + c] = 0; // 0
						else if (t == 224 || t == 96 || t == 192 || t == 64)
							m_pBlendTiles[r * m_iNumCols + c] = 13; // 1
						else if (t == 32)
							m_pBlendTiles[r * m_iNumCols + c] = 26; // 2
						else if (t == 10 || t == 14 || t == 11 || t == 15 || t == 42 || t == 46 || t == 43 || t == 47)
							m_pBlendTiles[r * m_iNumCols + c] = 39; // 3
						else if (t == 20 || t == 16 || t == 144 || t == 148)
							m_pBlendTiles[r * m_iNumCols + c] = 53; // 4
						else if (t == 255 || t == 254 || t == 251 || t == 223 || t == 127 || t == 250 || t == 222 || t == 126 || t == 219 || t == 123 || t == 95 || t == 90 || t == 91 || t == 94 || t == 122 || t == 218)
							m_pBlendTiles[r * m_iNumCols + c] = 64; // 5 (12,13,14)
						else if (t == 9 || t == 8 || t == 40 || t == 41)
							m_pBlendTiles[r * m_iNumCols + c] = 77; // 6
						else if (t == 72 || t == 73 || t == 104 || t == 200 || t == 105 || t == 201 || t == 232 || t == 233)
							m_pBlendTiles[r * m_iNumCols + c] = 90; // 7
						else if (t == 4)
							m_pBlendTiles[r * m_iNumCols + c] = 104; // 8
						else if (t == 3 || t == 6 || t == 2 || t == 7)
							m_pBlendTiles[r * m_iNumCols + c] = 115; // 9
						else if (t == 1)
							m_pBlendTiles[r * m_iNumCols + c] = 128; // 10
						else if (t == 18 || t == 19 || t == 22 || t == 146 || t == 23 || t == 150 || t == 147 || t == 151)
							m_pBlendTiles[r * m_iNumCols + c] = 141; // 11
						else if (t == 80 || t == 208 || t == 112 || t == 240 || t == 84 || t == 212 || t == 116 || t == 244)
							m_pBlendTiles[r * m_iNumCols + c] = 192; // 15

						m_pTextureBlend->SetData(c, r, 1, 1 , &m_pBlendTiles[r * m_iNumCols + c], 1);
						SetTile(c, r, m_pTiles[r*m_iNumCols+c], tile);
					}
					else if (m_pTilesTemp[r * m_iNumCols + c] == 2)
					{
						SetTile(c, r, tile, m_pTiles2[r*m_iNumCols+c]);
					}
				}

			}
		}

		//		m_pTextureBlend->SetData(0, 0, m_iNumCols, m_iNumRows ,m_pBlendTiles, m_iNumCols * m_iNumRows);

	} // Blend

	//------------------------------------------------------------------------------------
	// Fill the terrain with the given tile ID
	//------------------------------------------------------------------------------------
	void TileTerrain::Fill( int Col, int Row, int TileID )
	{
		if (TileID < 0 || TileID >= m_pTileTexture->GetTileCount())
			return;
		int tile = m_pTiles[Row * m_iNumCols + Col];
		Flood_Fill(Col, Row, tile, TileID);

	} // Fill

	//------------------------------------------------------------------------------------
	// This function is for blending
	//------------------------------------------------------------------------------------
	void TileTerrain::Flood_Fill( int Col, int Row, int TileID )
	{
		if (Col < 0 || Col >= m_iNumCols || Row < 0 || Row >= m_iNumRows)
			return;
		if (m_pTiles[Row * m_iNumCols + Col] != TileID)
			return;
		if (m_pTilesTemp[Row * m_iNumCols + Col] != 0)
			return;

		std::queue<int> qc, qr;
		qc.push(Col);
		qr.push(Row);

		m_pTilesTemp[Row * m_iNumCols + Col] = 1;

		int c,r;
		while (!qc.empty())
		{
			c = qc.front();
			r = qr.front();
			qc.pop();
			qr.pop();
			if (c > 0)
			{
				if (m_pTiles[r * m_iNumCols + c - 1] == TileID &&
					m_pBlendTiles[r * m_iNumCols + c - 1] == 255 &&
					m_pTilesTemp[r * m_iNumCols + c - 1] == 0)
				{
					m_pTilesTemp[r * m_iNumCols + c - 1] = 1;
					qc.push(c - 1);
					qr.push(r);
				}
				else if (m_pTiles[r * m_iNumCols + c - 1] != TileID &&
					m_pTilesTemp[r * m_iNumCols + c - 1] == 0)
				{
					m_pTilesTemp[r * m_iNumCols + c - 1] = 2;
				}

			}
			if (c < m_iNumCols - 1)
			{
				if (m_pTiles[r * m_iNumCols + c + 1] == TileID &&
					m_pBlendTiles[r * m_iNumCols + c + 1] == 255 &&
					m_pTilesTemp[r * m_iNumCols + c + 1] == 0)
				{
					m_pTilesTemp[r * m_iNumCols + c + 1] = 1;
					qc.push(c + 1);
					qr.push(r);
				}
				else if (m_pTiles[r * m_iNumCols + c + 1] != TileID &&
					m_pTilesTemp[r * m_iNumCols + c + 1] == 0)
				{
					m_pTilesTemp[r * m_iNumCols + c + 1] = 2;
				}
			}
			if (r > 0)
			{
				if (m_pTiles[(r - 1) * m_iNumCols + c] == TileID &&
					m_pBlendTiles[(r - 1) * m_iNumCols + c] == 255 &&
					m_pTilesTemp[(r - 1) * m_iNumCols + c] == 0)
				{
					m_pTilesTemp[(r - 1) * m_iNumCols + c] = 1;
					qc.push(c);
					qr.push(r - 1);
				}
				else if (m_pTiles[(r - 1) * m_iNumCols + c] != TileID &&
					m_pTilesTemp[(r - 1) * m_iNumCols + c] == 0)
				{
					m_pTilesTemp[(r - 1) * m_iNumCols + c] = 2;
				}
			}
			if (r < m_iNumRows - 1)
			{
				if (m_pTiles[(r + 1) * m_iNumCols + c] == TileID &&
					m_pBlendTiles[(r + 1) * m_iNumCols + c] == 255 &&
					m_pTilesTemp[(r + 1) * m_iNumCols + c] == 0)
				{
					m_pTilesTemp[(r + 1) * m_iNumCols + c] = 1;
					qc.push(c);
					qr.push(r + 1);
				}
				else if (m_pTiles[(r + 1) * m_iNumCols + c] != TileID &&
					m_pTilesTemp[(r + 1) * m_iNumCols + c] == 0)
				{
					m_pTilesTemp[(r + 1) * m_iNumCols + c] = 2;
				}
			}
		}

	} // Flood_Fill ( BLEND )

	//------------------------------------------------------------------------------------
	// This function is for filling
	//------------------------------------------------------------------------------------
	void TileTerrain::Flood_Fill( int Col, int Row, int ReplaceID, int TileID )
	{
		if (Col < 0 || Col >= m_iNumCols || Row < 0 || Row >= m_iNumRows)
			return;
		if (m_pTiles[Row * m_iNumCols + Col] != ReplaceID)
			return;
		if (m_pTiles[Row * m_iNumCols + Col] == TileID)
			return;

		std::queue<int> qc, qr;
		qc.push(Col);
		qr.push(Row);

		SetTile(Col, Row, TileID);

		int c,r;
		while (!qc.empty())
		{
			c = qc.front();
			r = qr.front();
			qc.pop();
			qr.pop();
			if (c > 0)
			{
				if (m_pTiles[r * m_iNumCols + c - 1] == ReplaceID)
				{
					SetTile(c - 1, r, TileID);
					qc.push(c - 1);
					qr.push(r);
				}
			}
			if (c < m_iNumCols - 1)
			{
				if (m_pTiles[r * m_iNumCols + c + 1] == ReplaceID)
				{
					SetTile(c + 1, r, TileID);
					qc.push(c + 1);
					qr.push(r);
				}
			}
			if (r > 0)
			{
				if (m_pTiles[(r - 1) * m_iNumCols + c] == ReplaceID)
				{
					SetTile(c, r - 1, TileID);
					qc.push(c);
					qr.push(r - 1);
				}
			}
			if (r < m_iNumRows - 1)
			{
				if (m_pTiles[(r + 1) * m_iNumCols + c] == ReplaceID)
				{
					SetTile(c, r + 1, TileID);
					qc.push(c);
					qr.push(r + 1);
				}
			}
		}

	} // Flood_Fill ( FILL )

	//------------------------------------------------------------------------------------
	// Returns the terrain height at the given grid position
	//------------------------------------------------------------------------------------
	float TileTerrain::GetHeight( float x, float z )
	{
		float c = (x - m_fXOffset) / m_fWidth;
		float d = (z - m_fYOffset) / m_fHeight;
		int col = (int)c;
		int row = (int)d;

		float A = m_pPos[row * m_iNumColspp + col].Y;
		float B = m_pPos[row * m_iNumColspp + col + 1].Y;
		float C = m_pPos[(row + 1) * m_iNumColspp + col].Y;
		float D = m_pPos[(row + 1) * m_iNumColspp + col + 1].Y;

		float s = c - (float)col;
		float t = d - (float)row;

		if (t < 1.0f - s)
		{
			float uy = B - A;
			float vy = C - A;
			return A + s * uy + t * vy;
		}
		else
		{
			float uy = C - D;
			float vy = B - D;
			return D + (1.0f - s) * uy + (1.0f - t) * vy;
		}

	} // GetHeight

	//------------------------------------------------------------------------------------
	// Sets the terrain height by the index id.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetHeight( int IndexID, float h )
	{
		m_bPosChanged = true;
		m_pPos[m_pIndices[IndexID]].Y = h;

		math::Vector2I v = GetTileByFaceID(IndexID / 3);

		// Update AABBs
		int tu = v.x / 16;
		int tv = v.y / 16;

		m_pAABB[tv * m_iNumUparts + tu].AddY(h);

	} // SetHeight( index ID )

	//------------------------------------------------------------------------------------
	// Sets the terrain height by the tile position.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetHeight( int Col, int Row, float h )
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		m_bPosChanged = true;

		m_pPos[Row * m_iNumColspp + Col].Y = h;
		m_pPos[Row * m_iNumColspp + Col + 1].Y = h;
		m_pPos[(Row + 1) * m_iNumColspp + Col].Y = h;
		m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y = h;

		// Update AABBs
		int tu = Col / 16;
		int tv = Row / 16;
		int mu = Col % 16;
		int mv = Row % 16;

		m_pAABB[tv * m_iNumUparts + tu].AddY(h);

		if ((mu == 15 || mu == 0))
		{
			if (mu == 0  && tu != 0)
				mu = -1;
			else if (mu == 15 && tu != m_iNumUparts - 1)
				mu = 1;
			else
				mu = -2;
		}
		else
			mu = -2;

		if ((mv == 15 || mv == 0))
		{
			if (mv == 0  && tv != 0)
				mv = -1;
			else if (mv == 15 && tv != m_iNumVparts - 1)
				mv = 1;
			else
				mv = -2;
		}
		else
			mv = -2;

		if (mu != -2)
		{
			m_pAABB[tv * m_iNumUparts + tu + mu].AddY(h);
		}

		if (mv != -2)
		{
			m_pAABB[(tv + mv) * m_iNumUparts + tu].AddY(h);
		}

	} // SetHeight ( Col, Row )

	//------------------------------------------------------------------------------------
	// Sets the terrain color by the index id.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetVertexColor( int IndexID, gfx::Color c )
	{
		m_bNorTexChanged = true;
		m_pNorTexColor[m_pIndices[IndexID]].Color = c.c;

	} // SetVertexColor

	//------------------------------------------------------------------------------------
	// Sets the terrain color by the tile position.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetVertexColor( int Col, int Row, gfx::Color c )
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		m_bNorTexChanged = true;

		m_pNorTexColor[Row * m_iNumColspp + Col].Color           = c.c;
		m_pNorTexColor[Row * m_iNumColspp + Col + 1].Color		= c.c;
		m_pNorTexColor[(Row + 1) * m_iNumColspp + Col].Color		= c.c;
		m_pNorTexColor[(Row + 1) * m_iNumColspp + Col + 1].Color = c.c;

	} // SetVertexColor

	//------------------------------------------------------------------------------------
	// Adds the terrain height by the index id.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetHeightAdd( int IndexID, float h )
	{
		m_bPosChanged = true;
		m_pPos[m_pIndices[IndexID]].Y += h;

		math::Vector2I v = GetTileByFaceID(IndexID / 3);

		// Update AABBs
		int tu = v.x / 16;
		int tv = v.y / 16;

		m_pAABB[tv * m_iNumUparts + tu].AddY(m_pPos[m_pIndices[IndexID]].Y);

	} // SetHeightAdd

	//------------------------------------------------------------------------------------
	// Adds the terrain height by the tile position.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetHeightAdd( int Col, int Row, float h )
	{
		if (Col < 0 || Col >= m_iNumCols ||
			Row < 0 || Row >= m_iNumRows)
			return;

		m_bPosChanged = true;

		m_pPos[Row * m_iNumColspp + Col].Y				+= h;
		m_pPos[Row * m_iNumColspp + Col + 1].Y			+= h;
		m_pPos[(Row + 1) * m_iNumColspp + Col].Y		+= h;
		m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y	+= h;

		// Update AABBs
		int tu = Col / 16;
		int tv = Row / 16;
		int mu = Col % 16;
		int mv = Row % 16;

		m_pAABB[tv * m_iNumUparts + tu].AddY(m_pPos[Row * m_iNumColspp + Col].Y);
		m_pAABB[tv * m_iNumUparts + tu].AddY(m_pPos[Row * m_iNumColspp + Col + 1].Y);
		m_pAABB[tv * m_iNumUparts + tu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col].Y);
		m_pAABB[tv * m_iNumUparts + tu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y);

		if ((mu == 15 || mu == 0))
		{
			if (mu == 0  && tu != 0)
				mu = -1;
			else if (mu == 15 && tu != m_iNumUparts - 1)
				mu = 1;
			else
				mu = -2;
		}
		else
			mu = -2;

		if ((mv == 15 || mv == 0))
		{
			if (mv == 0  && tv != 0)
				mv = -1;
			else if (mv == 15 && tv != m_iNumVparts - 1)
				mv = 1;
			else
				mv = -2;
		}
		else
			mv = -2;

		if (mu != -2)
		{
			if (mu == -1)
			{
				m_pAABB[tv * m_iNumUparts + tu + mu].AddY(m_pPos[Row * m_iNumColspp + Col].Y);
				m_pAABB[tv * m_iNumUparts + tu + mu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col].Y);
			}
			else
			{
				m_pAABB[tv * m_iNumUparts + tu + mu].AddY(m_pPos[Row * m_iNumColspp + Col + 1].Y);
				m_pAABB[tv * m_iNumUparts + tu + mu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y);
			}
		}

		if (mv != -2)
		{
			if (mv == -1)
			{
				m_pAABB[(tv + mv) * m_iNumUparts + tu].AddY(m_pPos[Row * m_iNumColspp + Col].Y);
				m_pAABB[(tv + mv) * m_iNumUparts + tu].AddY(m_pPos[Row * m_iNumColspp + Col + 1].Y);
			}
			else
			{
				m_pAABB[(tv + mv) * m_iNumUparts + tu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col].Y);
				m_pAABB[(tv + mv) * m_iNumUparts + tu].AddY(m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y);
			}
		}

	} // SetHeightAdd ( Col, Row )

	//------------------------------------------------------------------------------------
	// Gets the column number and row number by Face index.
	//------------------------------------------------------------------------------------
	math::Vector2I TileTerrain::GetTileByFaceID( int FaceID )
	{
		math::Vector2I v;
		v.y = m_pNorTexColor[m_pIndices[FaceID * 3]].tex.Y;
		v.x = m_pNorTexColor[m_pIndices[FaceID * 3]].tex.X - 1;

		return v;

	} // GetTileByFaceID

	//------------------------------------------------------------------------------------
	// Returns the DynamicTriangleMesh pointer and send updates to it if needed.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetDynamicTriangleMesh( ph::DynamicTriangleMesh* pDTM )
	{
		if (m_pDTM)
			m_pDTM->DecRef();
		m_pDTM = pDTM;
		m_pDTM->AddRef();

	} // SetDynamicTriangleMesh

	//------------------------------------------------------------------------------------
	// Recalculate the terrain normals
	//------------------------------------------------------------------------------------
	void TileTerrain::UpdateNormals()
	{
		m_bNorTexChanged = true;
		gfx::RecalculateNormals_Smooth32<gfx::VertexNTC>(m_pPos, m_pNorTexColor, m_iNumVerts, m_pIndices, m_iNumIndices);
		if (m_pNormalMapTileTexture || m_pMaterial[0].ppTexture[1])
		{
			gfx::CalcBinormalsTangents32(m_pPos, m_pNorTexColor, m_pMesh->m_pTangentBinormal, m_iNumVerts, m_pIndices, m_iNumIndices);
		}

	} // UpdateNormals

	//------------------------------------------------------------------------------------
	// Create a decal and return its pointer.
	//------------------------------------------------------------------------------------
	Decal* TileTerrain::CreateDecal(int sizeX, int sizeY, gfx::Texture* pTex)
	{
		if (sizeX >= m_iNumRows || sizeY >= m_iNumCols)
		{
			const int newSizeX = sizeX >= m_iNumRows ? m_iNumRows - 1 : sizeX;
			const int newSizeY = sizeY >= m_iNumCols ? m_iNumCols - 1 : sizeY;
			io::Logger::Log(io::ELM_Warning, "Decal size (%d, %d) is bigger than terrain, size reduced to (%d, %d).",
				sizeX, sizeY, newSizeX, newSizeY);
			return this->CreateDecal(newSizeX, newSizeY, pTex);
		}

		// Find a place in vertex buffer for this decal texture
		MapDecalData::iterator it = m_mDecalBufferData.find(pTex->GetHandle());
		DecalData *	pDecalData	  = NULL;

		// not found
		if (it == m_mDecalBufferData.end())
		{
			pDecalData					= KGE_NEW(DecalData);
			pDecalData->vbOffset		= 0;
			pDecalData->ibOffset		= 0;
			pDecalData->vbSizeTotal		= 1600;
			pDecalData->ibSizeTotal		= 6000;
			pDecalData->VertexBuffer	= m_pRenderer->CreateVertexBuffer(NULL, 1600, gfx::EVT_V3T, true);
			pDecalData->IndexBuffer		= m_pRenderer->CreateIndexBuffer(NULL, 5400, gfx::EIBT_16Bit, true);
			pDecalData->Vertices		= KGE_NEW_ARRAY(gfx::Vertex3T, 1600);
			m_mDecalBufferData.insert(MapDecalData::value_type(pTex->GetHandle(), pDecalData));
		}
		// found
		else
		{
			pDecalData = it->second;

		}

		// Check for empty space
		bool bEmptyFound = false;
		RemovedDecal rd = {0};
		rd.VertexSize = (sizeX + 2) * (sizeY + 2);
		for (size_t i = 0; i < pDecalData->Space.size(); i++)
		{
			if (pDecalData->Space[i].VertexSize == rd.VertexSize)
			{
				rd.ibOffset = pDecalData->Space[i].ibOffset;
				rd.vbOffset = pDecalData->Space[i].vbOffset;
				pDecalData->Space.erase(pDecalData->Space.begin() + i);
				bEmptyFound = true;
				break;
			}
		}
		if (!bEmptyFound)
		{
			// check if vertex buffer is full
			if (rd.VertexSize + pDecalData->vbOffset >= pDecalData->vbSizeTotal)
			{
				// we need to expand our vertex buffer
				while (rd.VertexSize + pDecalData->vbOffset >= pDecalData->vbSizeTotal)
				{
					pDecalData->vbSizeTotal	+= 1600;
				}

				KGE_DELETE(pDecalData->VertexBuffer, HardwareBuffer);
				pDecalData->VertexBuffer	= m_pRenderer->CreateVertexBuffer(NULL, pDecalData->vbSizeTotal, gfx::EVT_V3T, true);
				gfx::Vertex3T* pNewVertices = KGE_NEW_ARRAY(gfx::Vertex3T, pDecalData->vbSizeTotal);
				memcpy(pNewVertices, pDecalData->Vertices, pDecalData->vbOffset * sizeof(gfx::Vertex3T));
				KGE_DELETE_ARRAY(pDecalData->Vertices);
				pDecalData->Vertices = pNewVertices;

				// SetDecals
				for (size_t i = 0; i < pDecalData->Decals.size(); i++)
				{
					pDecalData->Decals[i]->m_pVertices = &pNewVertices[pDecalData->Decals[i]->m_iVbOffset];
				}

			}

			// check if index buffer is full
			if ((sizeX + 1) * (sizeY + 1) * 6 + pDecalData->ibOffset >= pDecalData->ibSizeTotal)
			{
				while ((sizeX + 1) * (sizeY + 1) * 6 + pDecalData->ibOffset >= pDecalData->ibSizeTotal)
				{
					pDecalData->ibSizeTotal	+= 6000;
				}

				KGE_DELETE(pDecalData->IndexBuffer, HardwareBuffer);
				pDecalData->IndexBuffer		= m_pRenderer->CreateIndexBuffer(NULL, pDecalData->ibSizeTotal, gfx::EIBT_16Bit, true);
				u16* pTemp					= KGE_NEW_ARRAY(u16, pDecalData->ibSizeTotal);

				// SetIndex
				for (size_t i = 0; i < pDecalData->Decals.size(); i++)
				{
					memcpy(&pTemp[pDecalData->Decals[i]->m_iIbOffset], pDecalData->Decals[i]->m_pIndices, pDecalData->Decals[i]->m_iICount * 2);
				}
				pDecalData->IndexBuffer->SetData(pTemp, 0, pDecalData->ibOffset);
				KGE_DELETE_ARRAY(pTemp);
			}

			rd.ibOffset = pDecalData->ibOffset;
			rd.vbOffset = pDecalData->vbOffset;

		}

		Decal*			pDecal		= KGE_NEW(Decal)();
		gfx::Vertex3T*	pVerts		= &pDecalData->Vertices[rd.vbOffset];
		pDecal->m_ppVertexBuffer	= &pDecalData->VertexBuffer;
		pDecal->m_ppIndexBuffer		= &pDecalData->IndexBuffer;
		pDecal->m_iIbOffset			= rd.ibOffset;
		pDecal->m_iVbOffset			= rd.vbOffset;
		pDecal->m_pTerrain			= this;
		pDecal->m_iIndex			= pTex->GetHandle();
		int iOffset					= rd.vbOffset;
		if (!bEmptyFound)
			pDecalData->vbOffset   += rd.VertexSize;
		u16*			pIndices	= KGE_NEW_ARRAY(u16, (sizeX + 1) * (sizeY + 1) * 6);
		float			fU			= 1.0f / (float)sizeX;
		float			fV			= 1.0f / (float)sizeY;
		int				sizeXpp		= sizeX + 1;
		int				sizeXp2		= sizeX + 2;
		int				sizeYpp		= sizeY + 1;

		for (int y = 0; y < sizeY + 2; y++)
		{
			for (int x = 0; x < sizeXp2; x++)
			{
				pVerts[y * sizeXp2 + x].pos.X = x * m_fWidth;
				pVerts[y * sizeXp2 + x].pos.Z = y * m_fHeight;
				pVerts[y * sizeXp2 + x].pos.Y = 0.0f;

				pVerts[y * sizeXp2 + x].tex.X = x * fU;
				pVerts[y * sizeXp2 + x].tex.Y = y * fV;

			} // for x

		} // for y

		int k = 0;

		for (int y = 0; y < sizeYpp; y++)
		{
			for (int x = 0; x < sizeXpp; x++)
			{
				pIndices[k    ] = (y * sizeXp2 + x) + 1 + iOffset;
				pIndices[k + 1] = (y * sizeXp2 + x) + iOffset;
				pIndices[k + 2] = ((y + 1) * sizeXp2 + x) + iOffset;

				pIndices[k + 3] = (y * sizeXp2 + x) + 1 + iOffset;
				pIndices[k + 4] = ((y + 1) * sizeXp2 + x) + iOffset;
				pIndices[k + 5] = ((y + 1) * sizeXp2 + x) + 1 + iOffset;

				k += 6;

			} // for x

		} // for y

		pDecal->m_eVertexType	= gfx::EVT_V3T;
		pDecal->m_pIndices		= pIndices;
		pDecal->m_pMaterial
			->ppTexture[0]		= pTex;
		pDecal->m_pMaterial
			->Alpha				= true;
		pDecal->m_pVertices		= pVerts;
		pDecal->m_iVCount		= sizeXp2 * (sizeY + 2);
		pDecal->m_iICount		= sizeXpp * sizeYpp * 6;
		pDecal->m_iSizeX		= sizeX;
		pDecal->m_iSizeY		= sizeY;
		pDecal->m_fU			= fU;
		pDecal->m_fV			= fV;
		gfx::ShaderInstance* si = KGE_NEW(gfx::ShaderInstance)();
		si->m_pVertexShader		= m_pVSdecal;
		si->m_pPixelShader		= m_pPSdecal;
		si->m_pFun				= m_pDecalFun;
		si->m_pUserData			= (void*)pDecal;
		pDecal->m_pMaterial
			->shader			= si;

		// Set indexes to index buffer
		pDecalData->IndexBuffer->SetData(pIndices,
			rd.ibOffset, pDecal->m_iICount);
		if (!bEmptyFound)
			pDecalData->ibOffset   += pDecal->m_iICount;

		m_pVSdecal->AddRef();
		m_pPSdecal->AddRef();
		m_pSnMan->AddSceneNode(pDecal, ENT_Decal);

		pDecalData->Decals.push_back(pDecal);

		return pDecal;

	} // CreateDecal

	//------------------------------------------------------------------------------------
	// Sets the decal on position
	//------------------------------------------------------------------------------------
	void TileTerrain::SetDecal( float fx, float fy, Decal* pDecal )
	{
		int ix = (int)(fx / m_fWidth);
		int iy = (int)(fy / m_fHeight);

		if (ix < 0 || ix > m_iNumCols - pDecal->m_iSizeX - 1 || iy < 0 || iy > m_iNumRows - pDecal->m_iSizeY - 1)
			return;

		float fu = (fx / m_fHeight) - ix;
		float fv = (fy / m_fHeight) - iy;
		fu /= pDecal->m_iSizeX;
		fv /= pDecal->m_iSizeY;

		pDecal->GetAbsoluteMatrix()->_41 = ix * m_fWidth + m_fXOffset;
		pDecal->GetAbsoluteMatrix()->_43 = iy * m_fHeight + m_fYOffset;

		for (int y = 0; y < pDecal->m_iSizeY + 2; y++)
		{
			for (int x = 0; x < pDecal->m_iSizeX + 2; x++)
			{
				pDecal->m_pVertices[y * (pDecal->m_iSizeX + 2) + x].pos.Y
					= m_pPos[(iy + y) * m_iNumColspp + ix + x].Y + 0.05f;

				pDecal->m_pVertices[y * (pDecal->m_iSizeX + 2) + x].tex.X = x * pDecal->m_fU - fu;
				pDecal->m_pVertices[y * (pDecal->m_iSizeX + 2) + x].tex.Y = y * pDecal->m_fV - fv;

			} // for x

		} // for y
		pDecal->m_bPosChanged = true;

	} // SetDecal

	//------------------------------------------------------------------------------------
	// Creates the terrain shader code
	//------------------------------------------------------------------------------------
	void TileTerrain::CreateShaderCode(core::String &VertexCode, core::String &PixelCode)
	{
		// Create vertex shader
		VertexCode = "float4x4 matViewProjection;\n"\
			"float4x4 matView;\n"\
			"float3 DirLit;\n"\
			"float3 fvEyePosition;\n";
		if (m_bReceiveShadow)
		{
			VertexCode += "float4x4 matLight;\n";
		}
		VertexCode += "struct VS_INPUT\n"\
			"{\n"\
			"float4 Position : POSITION0;\n"\
			"float3 Normal : NORMAL;\n"\
			"float2 Texcoord : TEXCOORD0;\n"\
			"float4 color : COLOR0;\n";
		if (m_pNormalMapTileTexture)
		{
			VertexCode += "   float3 Binormal : BINORMAL0;\n"\
				"   float3 Tangent :  TANGENT0;\n";
		}
		VertexCode += "};\n"\
			"struct VS_OUTPUT \n"\
			"{\n"\
			"float4 Position : POSITION0;\n"\
			"float2 Texcoord : TEXCOORD0;\n"\
			"float	Height: TEXCOORD2;\n"\
			"float4 color : COLOR0;\n";
		if (m_bReceiveShadow)
		{
			VertexCode += "   float4	ProjTex: TEXCOORD3;\n";
		}
		if (m_pNormalMapTileTexture)
		{
			VertexCode += "float3 ViewDirection :   TEXCOORD4;\n"\
				"   float3 LightDirection:   TEXCOORD5;\n";
		}
		else
		{
			VertexCode += "float    Lit: TEXCOORD1;\n";
		}

		VertexCode += "};\n"\
			"VS_OUTPUT main( VS_INPUT Input )\n"\
			"{\n"\
			"VS_OUTPUT Output;\n"\
			"Output.Position = mul( Input.Position, matViewProjection );\n"\
			"Output.Texcoord = Input.Texcoord;\n"\
			"Output.Height = Input.Position.y / 25.0;\n"\
			"Output.color = Input.color;\n";
		if (m_bReceiveShadow)
		{
			VertexCode += "Output.ProjTex = mul( Input.Position, matLight );\n";
		}
		if (m_pNormalMapTileTexture)
		{
			VertexCode +=    "float3 fvObjectPosition = mul( Input.Position, matView ).xyz;\n"\
				"float3 fvNormal         = mul( float4(Input.Normal, 1.0),   matView ).xyz;\n"\
				"float3 fvBinormal       = mul( float4(Input.Binormal, 1.0), matView ).xyz;\n"\
				"float3 fvTangent        = mul( float4(Input.Tangent, 1.0),  matView ).xyz;\n"\
				"float3 fvViewDirection  = fvEyePosition - fvObjectPosition;\n"\
				"Output.ViewDirection.x  = dot( fvTangent,  fvViewDirection );\n"\
				"Output.ViewDirection.y  = dot( fvBinormal, fvViewDirection );\n"\
				"Output.ViewDirection.z  = dot( fvNormal,   fvViewDirection );\n"\
				"Output.LightDirection.x  = dot( fvTangent,  DirLit );\n"\
				"Output.LightDirection.y  = dot( fvBinormal, DirLit );\n"\
				"Output.LightDirection.z  = dot( fvNormal,   DirLit );\n";
		}
		else
		{
			VertexCode += "Output.Lit = 0.5 * dot(DirLit, Input.Normal) + 0.5;\n";
		}

		VertexCode += 	"return( Output );\n"\
			"}";

		// Create pixel shader
		PixelCode  = "sampler2D baseMap;\n"\
			"sampler2D BlendMap;\n"\
			"sampler2D TextureID;\n"\
			"sampler2D TextureBlend;\n"\
			"sampler2D TextureUnder;\n";
		if (m_bReceiveShadow)
		{
			PixelCode += m_pShadowShaderCode;
		}
		if (m_pNormalMapTileTexture)
		{
			PixelCode += "sampler2D bumpMap;\n";
		}
		PixelCode += "struct PS_INPUT \n"\
			"{\n"\
			"	float2 Texcoord : TEXCOORD0;\n"\
			"	float	Height: TEXCOORD2;\n"\
			"   float4 color : COLOR0;\n";
		if (m_bReceiveShadow)
		{
			PixelCode += "   float4	ProjTex: TEXCOORD3;\n";
		}
		if (m_pNormalMapTileTexture)
		{
			PixelCode += "float3 ViewDirection :   TEXCOORD4;\n"\
				"   float3 LightDirection:   TEXCOORD5;\n";
		}
		else
		{
			PixelCode += "float    Lit: TEXCOORD1;\n";
		}
		PixelCode += "};\n"\
			"float4 main( PS_INPUT Input ) : COLOR0\n"\
			"{\n";
		// if want under water coloring
		if (m_bWater)
		{
			PixelCode += "	float4 c1 = float4(";
			PixelCode += m_cStart.c[0];
			PixelCode += ", ";
			PixelCode += m_cStart.c[1];
			PixelCode += ", ";
			PixelCode += m_cStart.c[2];
			PixelCode += ", 1);\n"\
				"	float4 c2 = float4(";
			PixelCode += m_cEnd.c[0];
			PixelCode += ", ";
			PixelCode += m_cEnd.c[1];
			PixelCode += ", ";
			PixelCode += m_cEnd.c[2];
			PixelCode += ", 1);\n";
		}
		PixelCode += "	float2 m[16] = \n"\
			"	{\n"\
			"		float2(0.0, 0.0),\n"\
			"		float2(0.25, 0.0),\n"\
			"		float2(0.5, 0.0),\n"\
			"		float2(0.75, 0.0),\n"\
			"		float2(0.0, 0.25),\n"\
			"		float2(0.25, 0.25),\n"\
			"		float2(0.5, 0.25),\n"\
			"		float2(0.75, 0.25),\n"\
			"		float2(0.0, 0.5),\n"\
			"		float2(0.25, 0.5),\n"\
			"		float2(0.5, 0.5),\n"\
			"		float2(0.75, 0.5),\n"\
			"		float2(0.0, 0.75),\n"\
			"		float2(0.25, 0.75),\n"\
			"		float2(0.5, 0.75),\n"\
			"		float2(0.75, 0.75)\n"\
			"	};\n"\
			"	float4 d = tex2D( TextureID, Input.Texcoord * float2( ";
		// Set the texture ID size
		PixelCode += (float)(1.0f / (float)m_iTextureIDSizeX);
		PixelCode += " , ";
		PixelCode += (float)(1.0f / (float)m_iTextureIDSizeY);
		PixelCode += 	" ) );\n"\
			"	float4 b = tex2D( TextureBlend, Input.Texcoord * float2( ";
		PixelCode += (float)(1.0f / (float)m_iTextureIDSizeX);
		PixelCode += " , ";
		PixelCode += (float)(1.0f / (float)m_iTextureIDSizeY);
		PixelCode += 	" ) );\n"\
			"	float2 z = Input.Texcoord * float2( ";
		// Sets the tile texture size
		PixelCode += m_pTileTexture->GetU();
		PixelCode += " , ";
		PixelCode += m_pTileTexture->GetV();
		PixelCode += " ) ;\n"\
			"	float2 tex;\n"\
			"	tex.x = z.x - d.r;\n"\
			"	tex.y = z.y - d.g;\n";
		// Normal map
		if (m_pNormalMapTileTexture)
		{
			PixelCode += "float4 fvSpecular = {0.5188, 0.5065, 0.3195, 1};\n"\
				"float3 fvLightDirection = normalize( Input.LightDirection );\n"\
				"float3 fvNormal         = tex2D( bumpMap, tex ).wyz;\n";
		}

		PixelCode += "	float4 c;\n"\
			"	if (b.a < 0.9)\n"\
			"	{\n"\
			"		float2 t = m[b.a * 20];\n"\
			"		float2 s = (int2)(Input.Texcoord) * 0.25;\n"\
			"		float4 a = tex2D( BlendMap, Input.Texcoord * 0.25 - s + t );\n"\
			"		t.x = z.x - d.b;\n"\
			"		t.y = z.y - d.a;\n"\
			"		c = (1 - a.r) * tex2D( baseMap, tex) + a.r * tex2D( baseMap, t);\n";
		if (m_pNormalMapTileTexture)
		{
			// blend normals
			PixelCode += "fvNormal.xy = (1 - a.r) * fvNormal.xy + a.r * tex2D( bumpMap, t ).wy;\n";
		}
		PixelCode += "	}\n"\
			"	else\n"\
			"		c = tex2D( baseMap, tex);\n";
		// Calc normal map
		if (m_pNormalMapTileTexture)
		{
			PixelCode += "fvNormal = normalize( ( fvNormal * 2.0f ) - 1.0f );\n"\
				"float  fNDotL           = max(0.25, dot( fvNormal, fvLightDirection )) + 0.2; \n"\
				"float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); \n"\
				"float3 fvViewDirection  = normalize( Input.ViewDirection );\n"\
				"float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );\n";
		}
		io::Logger::Log(io::ELM_Warning, "%d b", m_bCaustics);
		if (m_bWater || m_bCaustics)
		{
			PixelCode += "	float4 col = float4(1, 1, 1, 1);\n"\
				"	if (Input.Height < 0.0)\n"\
				"	{\n"\
				"		Input.Height *= -1.0;\n";
			if (m_bWater)
			{
				PixelCode += "	col = lerp(c1, c2, Input.Height);\n";
			}
			if (m_bCaustics)
			{
				PixelCode += "	float4 under = tex2D(TextureUnder, Input.Texcoord * 0.1);\n";
				if (m_bWater)
					PixelCode += "	col += under / ((Input.Height * 0.2 + 1.5) );\n";
				else
					PixelCode += "	col = under / ((Input.Height * 0.2 + 1.5) * 0.5);\n";
			}
			PixelCode += "	}\n";
		}
		if (m_bReceiveShadow)
		{
			PixelCode += "Input.ProjTex.xy /=  Input.ProjTex.w;\n"\
				"Input.ProjTex.x   =  0.5f * Input.ProjTex.x + 0.5f;\n"\
				"Input.ProjTex.y   = -0.5f * Input.ProjTex.y + 0.5f;\n";
		}
		if (m_bReceiveShadow && m_pNormalMapTileTexture)
		{
			PixelCode += "float shad = GetShadow(Input.ProjTex.xy);\n"\
				"float4 spec = float4(0,0,0,0);\n"\
				"if (shad > 0.8f)\n"\
				"spec = c.a * fvSpecular * pow( fRDotV, 10 /*fSpecularPower*/ );\n";
		}
		PixelCode += "	float4 dd = ";
		if (m_bWater || m_bCaustics)
		{
			PixelCode += "col * ";
		}
		if (m_bReceiveShadow && !m_pNormalMapTileTexture)
		{
			PixelCode += "GetShadow(Input.ProjTex.xy) * ";
		}
		if (m_pNormalMapTileTexture && !m_bReceiveShadow)
		{
			PixelCode += "c * fNDotL * Input.color * float4(0.9863, 0.9850, 0.9850, 1.0) + fvSpecular * pow( fRDotV, 25 /*fSpecularPower*/ );	\n";
		}
		else if (m_pNormalMapTileTexture && m_bReceiveShadow)
		{
			PixelCode += "c * fNDotL * Input.color * float4(0.9863, 0.9850, 0.9850, 1.0) * shad + spec ;	\n";
		}
		else
		{
			PixelCode += "c * Input.Lit * Input.color;	\n";
		}
		PixelCode += "	dd.a = Input.Height * 0.05;\n"\
			"	return dd;\n"\
			"}\n";

		io::Logger::Log(VertexCode.ToCharPointer());
		io::Logger::Log(PixelCode.ToCharPointer());
		//io::File f;
		//f.Open("e:/ter1ps.txt", true);
		//f.write((void*)PixelCode.ToCharPointer(), PixelCode.GetLenght() / 2);
		//f.write((void*)(PixelCode.ToCharPointer() + PixelCode.GetLenght() / 2), PixelCode.GetLenght() / 2);
		//f.Open("e:/ter1vs.txt", true);
		//f.write((void*)VertexCode.ToCharPointer(), VertexCode.GetLenght());

	} // CreateShaderCode

	//------------------------------------------------------------------------------------
	// Enables the under water vertex coloring effect.
	//------------------------------------------------------------------------------------
	void TileTerrain::EnableWater( bool Enable,
		const gfx::Colorf& cStart /*= gfx::Colorf(255, 255, 255)*/,
		const gfx::Colorf& cEnd /*= gfx::Colorf(0, 229, 255)*/ )
	{
		m_bWater	= Enable;
		m_cStart	= cStart;
		m_cEnd		= cEnd;

	} // EnableWater

	//------------------------------------------------------------------------------------
	// Enable and sets the caustics textures.
	//------------------------------------------------------------------------------------
	void TileTerrain::SetCaustics( gfx::Texture** ppTextures, int TexturesCount, int speed /*= 100*/ )
	{
		if (TexturesCount <= 0)
		{
			m_bCaustics = false;
			KGE_DELETE(m_pCausticsTimer, Timer);
			return;
		}
		if (!m_pCausticsTimer)
			m_pCausticsTimer		= KGE_NEW(core::Timer)();
		m_bCaustics					= true;
		m_iCurrentCaustic			= 0;
		m_ppCausticsTextures		= ppTextures;
		m_iNumCausticsTextures		= TexturesCount;
		m_pCausticsTimer->Interval	= speed;

	} // SetCaustics

	//------------------------------------------------------------------------------------
	// Sets the shader params
	//------------------------------------------------------------------------------------
	void TileTerrain::SetShaderParams()
	{
		// Get lights
		Light* pLit = m_pSnMan->GetNearestLight(math::Vector());
		math::Vector dir = pLit->GetLightData()->Direction;
		//dir.Negate();
		//dir.Normalize();
		//io::Logger::Log(io::ELM_Warning, "dir=%f %f %f", dir.x, dir.y, dir.z);
		m_pVshader->SetConstant(m_shLit, &dir.x, 3);

		// GET WVP
		math::Matrix mat = m_pRenderer->GetTransForm(kge::gfx::ETM_Projection) *
			m_pRenderer->GetTransForm(kge::gfx::ETM_View)  * (*m_pFinalMat);

		m_pVshader->SetConstant(m_shWVP, mat.m_fMat, 16);

		if (m_bReceiveShadow)
			m_pVshader->SetConstant(m_shShadowMat, m_pShadowMatrix->m_fMat, 16);

		if (m_shView)
			m_pVshader->SetConstant(m_shView, m_pSnMan->GetActiveCamera()->GetViewMatrix().m_fMat, 16);

		if (m_pNormalMapTileTexture || m_pMaterial[0].ppTexture[1] )
		{
			kge::sn::SceneNode* pCam = m_pSnMan->GetActiveCamera();
			math::Vector vp = pCam->GetPosition();
			mat.Identity();
			mat.Inverse();
			mat.TransFormVec2(dir);
			mat.TransFormVec2(vp);
			dir.Normalize();
			m_pVshader->SetConstant(m_shLit, &dir.x, 3);
			mat.Identity();
			//m_pVshader->SetConstant(m_shView, mat.m_fMat, 16);
			//vp.Normalize();
			//vp = vp * 100.0f;
			//(0, 0, 0);//(-mat._14, -mat._24, -mat._34);//
			m_pVshader->SetConstant(m_shfvEyePosition, &vp.x, 3);
		}

		userSetShaderParams();

	} // SetShaderParams

	//------------------------------------------------------------------------------------
	// Create an array of Y position vertices and return
	//------------------------------------------------------------------------------------
	void TileTerrain::GetYs(float* h)
	{
		for( int i = 0; i < m_iNumVerts; i++ )
			h[i] = m_pPos[i].Y;


	} // GetYs

	//------------------------------------------------------------------------------------
	// Sets the heights
	//------------------------------------------------------------------------------------
	void TileTerrain::SetYs( float* pHeights )
	{
		m_bPosChanged = true;
		for (int i = 0; i < m_iNumUparts * m_iNumVparts; i++)
		{
			m_pAABB[i].ResetY();
		}
		int u, v;
		for( int i = 0; i < m_iNumVerts; i++ )
		{
			m_pPos[i].Y = pHeights[i];
			u = (m_pNorTexColor[i].tex.X ) / 16;
			v = (m_pNorTexColor[i].tex.Y ) / 16;
			if (u >= m_iNumUparts)
				u--;
			if (v >= m_iNumVparts)
				v--;
			m_pAABB[v * m_iNumUparts + u].AddY(pHeights[i]);
		}

	} // SetYs

	//------------------------------------------------------------------------------------
	// Returns the tile count if you pass the NULL and set them otherwise
	//------------------------------------------------------------------------------------
	int TileTerrain::GetTiles(int *pTiles1, int* pTiles2)
	{
		int tilescount =  m_iNumCols * m_iNumRows;
		if (!pTiles1 || !pTiles2)
			return tilescount;

		memcpy(pTiles1, m_pTiles, sizeof(int) * tilescount);
		memcpy(pTiles2, m_pTiles2, sizeof(int) * tilescount);

		return tilescount;

	} // GetTiles

	//------------------------------------------------------------------------------------
	// Sets the tiles
	//------------------------------------------------------------------------------------
	void TileTerrain::SetTiles(int *pTiles1, int* pTiles2)
	{
		for (int j = 0; j < m_iNumRows; j++)
		{
			for (int i = 0; i < m_iNumCols; i++)
			{
				SetTile(i, j, pTiles1[j * m_iNumCols + i], pTiles2[j * m_iNumCols + i]);

			} // for i

		} // for j

	} // SetTiles

	//------------------------------------------------------------------------------------
	// Returns blend map size if you pass NULL and set the pointer otherwise
	//------------------------------------------------------------------------------------
	int TileTerrain::GetBlendMap(u8* pBlendMap)
	{
		int tilescount =  m_iNumCols * m_iNumRows;
		if (!pBlendMap)
			return tilescount;

		memcpy(pBlendMap, m_pBlendTiles, tilescount);

		return tilescount;

	} // GetBlendMap

	//------------------------------------------------------------------------------------
	// Sets the blend map
	//------------------------------------------------------------------------------------
	void TileTerrain::SetBlendMap(u8* pBlendMap)
	{
		int tilescount =  m_iNumCols * m_iNumRows;
		memcpy(m_pBlendTiles, pBlendMap, tilescount);
		m_pTextureBlend->SetData(0, 0, m_iNumCols, m_iNumRows ,m_pBlendTiles, tilescount);

	} // SetBlendMap

	//------------------------------------------------------------------------------------
	// Copy the vertex colors to the pointer
	//------------------------------------------------------------------------------------
	void TileTerrain::GetVertexColors(ul32* pVertexColor)
	{
		for (int i = 0; i < m_iNumVerts; i++)
		{
			pVertexColor[i] = m_pNorTexColor[i].Color;
		}

	} // GetVertexColors

	//------------------------------------------------------------------------------------
	// Copy the vertex colors from the pointer
	//------------------------------------------------------------------------------------
	void TileTerrain::SetVertexColors(ul32* pVertexColor)
	{
		m_bNorTexChanged = true;
		for (int i = 0; i < m_iNumVerts; i++)
		{
			m_pNorTexColor[i].Color = pVertexColor[i];
		}

	} // SetVertexColors

	void TileTerrain::ReceiveShadow( const char* Code, math::Matrix* shadowmat )
	{
		if ( Code && shadowmat)
		{
			m_bReceiveShadow	= true;
			m_pShadowShaderCode = Code;
			m_pShadowMatrix		= shadowmat;
		}
		else
		{
			m_bReceiveShadow	= false;
			m_pShadowShaderCode = 0;
			m_pShadowMatrix		= 0;
		}

	} // ReceiveShadow

	void TileTerrain::SetDecalShaderParams( gfx::ShaderInstance* pSI )
	{
		math::Matrix mat = m_pRenderer->GetTransForm(gfx::ETM_Projection)
			* m_pRenderer->GetTransForm(gfx::ETM_View)
			* *((Decal*)(pSI->m_pUserData))->GetAbsoluteMatrix();

		pSI->m_pVertexShader->SetConstant(m_shDecalWVP, mat.m_fMat, 16);
		float f = ((Decal*)(pSI->m_pUserData))->m_alpha;
		pSI->m_pPixelShader->SetConstant(m_shDecalAlpha, &f, 1);

	} // SetDecalShaderParams

	void TileTerrain::SetNormalMapTileTexture( gfx::TileTexture * pTileTex )
	{
		m_pNormalMapTileTexture = pTileTex;

	}

	//------------------------------------------------------------------------------------
	// Smooths the tile height
	//------------------------------------------------------------------------------------
	void TileTerrain::Smooth( int Col, int Row )
	{
		SmoothOneNode(Col, Row);
		SmoothOneNode(Col + 1, Row);
		SmoothOneNode(Col, Row + 1);
		SmoothOneNode(Col + 1, Row + 1);

	} // Smooth

	//------------------------------------------------------------------------------------
	// Smooths one node height
	//------------------------------------------------------------------------------------
	void TileTerrain::SmoothOneNode( int Col, int Row )
	{
		if (Col < 1 || Col > m_iNumCols - 1 ||
			Row < 1 || Row > m_iNumRows - 1)
			return;

		m_bPosChanged = true;

		float h =
			m_pPos[Row * m_iNumColspp + Col].Y +
			m_pPos[Row * m_iNumColspp + Col + 1].Y +
			m_pPos[(Row + 1) * m_iNumColspp + Col].Y +
			m_pPos[(Row + 1) * m_iNumColspp + Col + 1].Y +
			m_pPos[Row * m_iNumColspp + Col - 1].Y +
			m_pPos[(Row - 1) * m_iNumColspp + Col - 1].Y +
			m_pPos[(Row + 1) * m_iNumColspp + Col - 1].Y +
			m_pPos[(Row - 1) * m_iNumColspp + Col + 1].Y +
			m_pPos[(Row - 1) * m_iNumColspp + Col].Y ;

		m_pPos[Row * m_iNumColspp + Col].Y = h / 9.0f;

	} // SmoothOneNode

	//------------------------------------------------------------------------------------
	// Empty space in vertex buffer
	//------------------------------------------------------------------------------------
	void TileTerrain::RemoveDecal( Decal* pDecal )
	{
		// Find a place in vertex buffer for this decal texture
		MapDecalData::iterator it = m_mDecalBufferData.find(pDecal->m_iIndex);

		// found
		if (it != m_mDecalBufferData.end())
		{
			RemovedDecal rd;
			rd.VertexSize = pDecal->m_iVCount;
			rd.vbOffset	  = pDecal->m_iVbOffset;
			rd.ibOffset	  = pDecal->m_iIbOffset;
			it->second->Space.push_back(rd);
			for (size_t i = 0; i < it->second->Decals.size(); i++)
			{
				if (it->second->Decals[i] == pDecal)
				{
					it->second->Decals.erase(it->second->Decals.begin() + i);
					break;
				}
			}
		}

	} // RemoveDecal

	//------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------
	void TileTerrain::OnLost()
	{

	} // OnLost

	//------------------------------------------------------------------------------------
	// Refills vertex and index buffers after reseting device
	//------------------------------------------------------------------------------------
	void TileTerrain::OnReset()
	{
		if (m_pMesh)
			m_pMesh->RefillBuffers();

		UpdateNormals();

		// Refill decals index buffer
		size_t arraySize = 6000;
		u16* pTemp = KGE_NEW_ARRAY(u16, arraySize);

		for (MapDecalData::iterator it = m_mDecalBufferData.begin(), it_end = m_mDecalBufferData.end();
			it != it_end; ++it)
		{
			DecalData* pDecalData = it->second;

			if (pDecalData->ibSizeTotal > arraySize)
			{
				KGE_DELETE_ARRAY(pTemp);
				arraySize = pDecalData->ibSizeTotal;
				pTemp = KGE_NEW_ARRAY(u16, arraySize);
			}

			for (size_t i = 0, size = pDecalData->Decals.size(); i < size; ++i)
			{
				memcpy(
					&pTemp[pDecalData->Decals[i]->m_iIbOffset],
					pDecalData->Decals[i]->m_pIndices,
					pDecalData->Decals[i]->m_iICount * 2);
			}

			pDecalData->IndexBuffer->SetData(pTemp, 0, pDecalData->ibOffset);
		}

		KGE_DELETE_ARRAY(pTemp);
		//

		m_pRenderer->SetTextureParams(gfx::ETP_Point, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Mirror, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 4);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 5);

	} // OnReset

	//------------------------------------------------------------------------------------
	//
	//------------------------------------------------------------------------------------
	void TileTerrain::RecreateShaders()
	{
		// Create shader
		if (m_pVshader)
		{
			m_pVshader->DecRef();
			m_pVshader = NULL;
		}
		if (m_pPshader)
		{
			m_pPshader->DecRef();
			m_pPshader = NULL;
		}

		core::String VertexShaderCode;
		core::String PixelShaderCode;
		CreateShaderCode(VertexShaderCode, PixelShaderCode);

		m_pVshader = gfx::Renderer::GetSingletonPtr()->CreateVertexShaderFromString
			(VertexShaderCode.ToCharPointer(), "main", gfx::ESV_VS3);
		m_pPshader = gfx::Renderer::GetSingletonPtr()->CreatePixelShaderFromString
			(PixelShaderCode.ToCharPointer(), "main", gfx::ESV_PS3);
		m_pVshader->ConnectOnPreRender(core::mem_fun(this, &TileTerrain::SetShaderParams));
		m_shLit	   = m_pVshader->GetConstatnt("DirLit");
		m_shWVP	   = m_pVshader->GetConstatnt("matViewProjection");
		m_shView   = m_pVshader->GetConstatnt("matView");
		m_shfvEyePosition = m_pVshader->GetConstatnt("fvEyePosition");

		if (m_bReceiveShadow)
			m_shShadowMat = m_pVshader->GetConstatnt("matLight");

		m_pRenderer->SetTextureParams(gfx::ETP_Point, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Mirror, 1);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 4);
		m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 5);

	} // RecreateShaders

	} // sn

} // kge
