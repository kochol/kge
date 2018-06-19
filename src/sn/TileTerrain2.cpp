// File name: TileTerrain2.cpp
// Des: This is a tile based terrain class good for strategic games
// Date: 26/10/1390
// Programmers: Ali Akbar Mohammadi (Kochol)

#include "../../include/kge/sn/TileTerrain2.h"
#include "../../include/kge/gui/GuiManager.h"
#include "../../include/kge/io/File.h"
#include "../../include/kge/sn/Light.h"

namespace kge
{
	namespace sn
	{

		//------------------------------------------------------------------------------------
		// Constructor
		//------------------------------------------------------------------------------------
		TileTerrain2::TileTerrain2(): 
			  m_ppBlendMaps(NULL)
			, m_ppTextureIDMaps(NULL)
			, m_iMapsCount(0)
			, m_iMapsSize(8)
			, m_iMapsCX(0)
			, m_iMapsCY(0)
			, m_pVsBrush(NULL)
			, m_pPsBrush(NULL)
			, m_pBrush(NULL)
			, m_fBrushSize(10.0)
			, m_pImgBrush(NULL)
			, m_shDirLightColor(NULL)
		{
			m_eNodeType	   = ENT_TileTerrain2;
			m_bTileTerrain2= true;

			m_pImgScaledBrush = KGE_NEW(gfx::Image)("tempbrush");

			// Calculate the 2D camera matrix.
			m_m2D._11 = 2.0/512.0;
			m_m2D._22 = -2.0/512.0;
			m_m2D._12 = m_m2D._13 = m_m2D._14 = m_m2D._21 = m_m2D._23 = m_m2D._24 =
				m_m2D._31 = m_m2D._32 = m_m2D._34 = m_m2D._43 = 0.0f;
			m_m2D._41 = -1.0f;
			m_m2D._44 = m_m2D._33 = m_m2D._42 = 1.0f;

		} // Constructor

		//------------------------------------------------------------------------------------
		// Destructor
		//------------------------------------------------------------------------------------
		TileTerrain2::~TileTerrain2()
		{
			if (m_ppBlendMaps && m_ppTextureIDMaps)
			{
				for (int i = 0; i < m_iMapsCount; i++)
				{
					m_ppBlendMaps[i]->DecRef();
					m_ppTextureIDMaps[i]->DecRef();
				}
				KGE_DELETE_ARRAY(m_ppBlendMaps);
				KGE_DELETE_ARRAY(m_ppTextureIDMaps);
			}

		} // Destructor

		//------------------------------------------------------------------------------------
		// Create the terrain if already created then recreate it.
		//------------------------------------------------------------------------------------
		void TileTerrain2::ReCreate( int numCols, int numRows, float MinHeight /*= 0.0f*/, float MaxHeight /*= 0.0f*/ )
		{
			// Create blend map textures
			if (m_ppBlendMaps && m_ppTextureIDMaps)
			{
				for (int i = 0; i < m_iMapsCount; i++)
				{
					m_ppBlendMaps[i]->DecRef();
					m_ppTextureIDMaps[i]->DecRef();
				}
				KGE_DELETE_ARRAY(m_ppBlendMaps);
				KGE_DELETE_ARRAY(m_ppTextureIDMaps);
			}

			// I. Find out how many blend texture is needed for this terrain
			int tx, ty, 
				cx = 1, cy = 1, 
				texsize;
			tx = numCols * m_iMapsSize;
			ty = numRows * m_iMapsSize;
			texsize = 64 * m_iMapsSize;
			while (tx > texsize)
			{
				tx -= texsize;
				cx++;
			}
			while (ty > texsize)
			{
				ty -= texsize;
				cy++;
			}
			m_iMapsCount		= cx * cy;
			m_iMapsCX			= cx;
			m_iMapsCY			= cy;

			// II. Create the textures
			m_ppBlendMaps		= KGE_NEW_ARRAY(kge::gfx::Texture*, m_iMapsCount);
			m_ppTextureIDMaps	= KGE_NEW_ARRAY(kge::gfx::Texture*, m_iMapsCount);
			kge::core::String str;
			str = "TexID";
			u8* alphadata = KGE_NEW_ARRAY(u8, texsize * texsize);
			memset(alphadata, 0, texsize * texsize);
			for (int i = 0; i < m_iMapsCount; i++)
			{
				str += i;
				int handle				= Device::GetSingletonPtr()->GetTextureManager()->Add(NULL, NULL, str.ToCharPointer());
				m_ppBlendMaps[i]		= Device::GetSingletonPtr()->GetTextureManager()->GetResource(handle);
				m_ppBlendMaps[i]->CreateTexture(texsize, texsize, gfx::ETF_A8, 0);
				m_ppBlendMaps[i]->SetData(alphadata, texsize * texsize);
				str += i;
				handle					= Device::GetSingletonPtr()->GetTextureManager()->Add(NULL, NULL, str.ToCharPointer());
				m_ppTextureIDMaps[i]	= Device::GetSingletonPtr()->GetTextureManager()->GetResource(handle);
				m_ppTextureIDMaps[i]->CreateTexture(texsize, texsize, gfx::ETF_A8L8, 1);
			}
			KGE_DELETE_ARRAY(alphadata);
			
			TileTerrain::ReCreate(numCols, numRows, MinHeight, MaxHeight);
			m_pMaterial->shader->m_pVertexShader = m_pVshader;
			m_pMaterial->shader->m_pPixelShader = m_pPshader;
			m_shDirLightColor = m_pPshader->GetConstatnt("LightColor");

		} // ReCreate

		//------------------------------------------------------------------------------------
		// Creates the terrain shader code
		//------------------------------------------------------------------------------------
		void TileTerrain2::CreateShaderCode( core::String &VertexCode, core::String &PixelCode )
		{
			if (m_pRenderer->GetRendererPipeline() == gfx::ERP_Deferred)
			{
				CreateDeferredShaderCode(VertexCode, PixelCode);
				return;
			}

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
 			if (m_pMaterial[0].ppTexture[1])
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
 				VertexCode += "   float4	ProjTex: TEXCOORD1;\n";
 			}
 			if (m_pMaterial[0].ppTexture[1])
 			{
 				VertexCode += "float3 ViewDirection :   TEXCOORD3;\n"\
 					"   float3 LightDirection:   TEXCOORD4;\n";
 			}
 			else
 			{
 				VertexCode += "float    Lit: TEXCOORD3;\n";
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
 			if (m_pMaterial[0].ppTexture[1])
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
				"sampler2D TextureBlend: register(s2);\n"\
 				"sampler2D TextureID: register(s3);\n"\
 				"sampler2D TextureUnder: register(s4);\n"\
				"float4 LightColor;\n";

 			if (m_bReceiveShadow)
 			{
 				PixelCode += m_pShadowShaderCode;
 			}
 			if (m_pMaterial[0].ppTexture[1])
 			{
 				PixelCode += "sampler2D bumpMap: register(s1);\n";
 			}

			/// This function evaluates the mipmap LOD level for a 2D texture using the given texture coordinates
			/// and texture size (in pixels)
			PixelCode +=
				"float mipmapLevel(float2 uv, float textureSize)\n"\
				"{\n"\
				"    float2 dx = ddx(uv * textureSize);\n"\
				"    float2 dy = ddy(uv * textureSize);\n"\
				"    float d = max(dot(dx, dx), dot(dy, dy));\n"\
				"    return 0.5 * log2(d);\n"\
				"}\n";
			/// This function samples a texture with tiling and mipmapping from within a texture pack of the given
			/// attributes
			/// - tex is the texture pack from which to sample a tile
			/// - uv are the texture coordinates of the pixel *inside the tile*
			/// - tile are the coordinates of the tile within the pack (ex.: 2, 1)
			/// - packTexFactors are some constants to perform the mipmapping and tiling
			/// Texture pack factors:
			/// - inverse of the number of horizontal tiles (ex.: 4 tiles -> 0.25)
			/// - inverse of the number of vertical tiles (ex.: 2 tiles -> 0.5)
			/// - size of a tile in pixels (ex.: 1024)
			/// - amount of bits representing the power-of-2 of the size of a tile (ex.: a 1024 tile is 10 bits).
			PixelCode +=
				"float4 sampleTexturePackMipWrapped(in float2 uv, const in float2 tile,\n"\
				"	const in float4 packTexFactors)\n"\
				"{\n"\
				" 	/// estimate mipmap/LOD level\n"\
				"	float lod = mipmapLevel(uv, packTexFactors.z);\n"\
				"	lod = clamp(lod, 0.0, packTexFactors.w);\n"\
				"\n"\
				"	/// get width/height of the whole pack texture for the current lod level\n"\
				"	float size = pow(2.0, packTexFactors.w - lod);\n"\
				"	float sizex = size / packTexFactors.x; // width in pixels\n"\
				"	float sizey = size / packTexFactors.y; // height in pixels\n"\
				"\n"\
				"	/// perform tiling\n"\
				"	uv = frac(uv);\n"\
				"\n"\
				"	/// tweak pixels for correct bilinear filtering, and add offset for the wanted tile\n"\
				"	uv.x = uv.x * ((sizex * packTexFactors.x - 1.0) / sizex) + 0.5 / sizex + packTexFactors.x * tile.x;\n"\
				"	uv.y = uv.y * ((sizey * packTexFactors.y - 1.0) / sizey) + 0.5 / sizey + packTexFactors.y * tile.y;\n"\
				"\n"\
				"    return float4(uv, lod, 0);\n"\
				"}";

 			PixelCode += "struct PS_INPUT \n"\
 				"{\n"\
 				"	float2 Texcoord : TEXCOORD0;\n"\
 				"	float	Height: TEXCOORD2;\n"\
 				"   float4 color : COLOR0;\n";
 			if (m_bReceiveShadow)
 			{
 				PixelCode += "   float4	ProjTex: TEXCOORD1;\n";
 			}
 			if (m_pMaterial[0].ppTexture[1])
 			{
 				PixelCode += "float3 ViewDirection :   TEXCOORD3;\n"\
 					"   float3 LightDirection:   TEXCOORD4;\n";
 			}
 			else
 			{
 				PixelCode += "float    Lit: TEXCOORD3;\n";
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

			//------------------------------------------------------------------------------------
			// Testing start
			//------------------------------------------------------------------------------------
			// finding the tileid
			PixelCode += "	float4 id = tex2D(TextureID, ((int2)Input.Texcoord % 64) / 64.0);//return tex2D(baseMap, Input.Texcoord).rbaa;\n"\
						 "	int2 tileid; tileid = id.ra * 64.0;\n"\
						 "	float4 f4TileID; f4TileID.xz = tileid % ";
			PixelCode += m_iTileH / m_iTileSizeInPixel;
			PixelCode += "; f4TileID.yw = tileid / ";
			PixelCode += m_iTileW / m_iTileSizeInPixel;
			PixelCode += ";\n";
			// Calculate the UVs.
			PixelCode += "	float4 c = sampleTexturePackMipWrapped(Input.Texcoord *";
			float fUV  = 1.0 / (m_iTileSizeInPixel / 64.0);
			PixelCode += fUV;
			PixelCode += "\n, f4TileID.xy, float4(";
			PixelCode += m_fHTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_fVTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_iTileSizeInPixel;
			PixelCode += ", ";
			PixelCode += m_iBitCount;
			PixelCode += "));\n"\
						 "	float4 d = sampleTexturePackMipWrapped(Input.Texcoord *";
			PixelCode += fUV;
			PixelCode += "\n, f4TileID.zw, float4(";
			PixelCode += m_fHTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_fVTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_iTileSizeInPixel;
			PixelCode += ", ";
			PixelCode += m_iBitCount;
			PixelCode += "));\n"\
						 "	float4 a = tex2D(TextureBlend, frac(Input.Texcoord * 0.015625));//return a.a;\n";
 			// Normal map
 			if (m_pMaterial[0].ppTexture[1])
 			{
 				PixelCode += "float4 fvSpecular = {0.5188, 0.5065, 0.3195, 1};\n"\
 					"float3 fvLightDirection = normalize( Input.LightDirection );\n"\
 					"float3 fvNormal         = tex2Dlod( bumpMap, c ).wyz;\n"\
					"fvNormal = (1 - a.a) * fvNormal + a.a * tex2Dlod( bumpMap, d ).wyz;\n";
 			}
			PixelCode += "	float4 b1 = tex2Dlod(baseMap, c);\n"\
						 "	float4 b2 = tex2Dlod(baseMap, d);\n"\
						 "	float4 r = b2 * a.a + (1 - a.a) * b1;\n";
			// Normal map
			if (m_pMaterial[0].ppTexture[1])
			{
				PixelCode += "fvNormal = normalize( ( fvNormal * 2.0f ) - 1.0f );\n"\
							 "float  fNDotL           = max(0.25, dot( fvNormal, fvLightDirection )) + 0.2; \n"\
							 "float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); \n"\
							 "float3 fvViewDirection  = normalize( Input.ViewDirection );\n"\
							 "float  fRDotV           = max( 0.0f, dot( fvReflection, fvViewDirection ) );\n";
			}
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
//  			if (m_bReceiveShadow)
//  			{
//  				PixelCode += "Input.ProjTex.xy /=  Input.ProjTex.w;\n"\
//  					"Input.ProjTex.x   =  0.5f * Input.ProjTex.x + 0.5f;\n"\
//  					"Input.ProjTex.y   = -0.5f * Input.ProjTex.y + 0.5f;\n";
//  			}
 			if (m_bReceiveShadow && m_pMaterial[0].ppTexture[1])
 			{
 				PixelCode += "float shad = GetShadow(Input.ProjTex);\n"\
 					"float4 spec = float4(0,0,0,0);\n"\
 					"if (shad > 0.8f)\n"\
 					"spec = LightColor * r.a * fvSpecular * pow( fRDotV, 10 );\n";
 			}
 			PixelCode += "	r = ";
 			if (m_bWater || m_bCaustics)
 			{
 				PixelCode += "col * ";
 			}
 			if (m_bReceiveShadow && !m_pMaterial[0].ppTexture[1])
 			{
 				PixelCode += "GetShadow(Input.ProjTex) * ";
 			}
 			if (m_pMaterial[0].ppTexture[1] && !m_bReceiveShadow)
 			{
 				PixelCode += "LightColor * r * fNDotL * Input.color + LightColor * r.a * fvSpecular * pow( fRDotV, 10 );	\n";
 			}
 			else if (m_pMaterial[0].ppTexture[1] && m_bReceiveShadow)
 			{
 				PixelCode += "r * LightColor * fNDotL * Input.color /** float4(0.9863, 0.9850, 0.9850, 1.0)*/ * shad + spec ;	\n";
 			}
 			else
 			{
 				PixelCode += "LightColor * r * Input.Lit * Input.color;	\n";
 			}

			PixelCode += "	r.a = Input.Height * 0.05;\n"\
						 "	return r;\n"\
						 "}\n";

			io::Logger::Log(VertexCode.ToCharPointer());
			io::Logger::Log(PixelCode.ToCharPointer());
// 			io::File f;
// 			f.Open("e:/ter2ps.txt", true);
// 			f.write((void*)PixelCode.ToCharPointer(), PixelCode.GetLenght());
// 			f.Open("e:/ter2vs.txt", true);
// 			f.write((void*)VertexCode.ToCharPointer(), VertexCode.GetLenght());

		} // CreateShaderCode

		//------------------------------------------------------------------------------------
		// Render the terrain
		//------------------------------------------------------------------------------------
		void TileTerrain2::Render()
		{
			TileTerrain::Render();
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 2);
			m_pRenderer->SetTextureParams(gfx::ETP_Point, 3);
			m_pRenderer->SetTextureParams(gfx::ETP_Clamp, 2);
			m_pRenderer->SetTextureParams(gfx::ETP_Clamp, 3);
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 1);
			const size_t m_vVisiblePartsSize = m_vVisibleParts.size();
			int u;
			int v;
			for (size_t i = 0; i < m_vVisiblePartsSize; ++i)
			{
				int id = m_vVisibleParts[i];
				u = id % m_iNumUparts / 4;
				v = id / m_iNumUparts / 4;
				int t = m_iNumUparts / 4;
				if (m_iNumUparts % 4 != 0)
					t++;
				id = v * t + u;
				m_pRenderer->SetTexture(m_ppBlendMaps[id], 2);
				m_pRenderer->SetTexture(m_ppTextureIDMaps[id], 3);
				m_pRenderer->DrawTriangleList(m_iNumVerts, 1536, m_pMesh->m_pVertexDec, 0, m_vVisibleParts[i] * 1536);
			}
			//m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 2);
			m_pRenderer->SetTextureParams(gfx::ETP_Anisotropic, 3);

		} // Render

		//------------------------------------------------------------------------------------
		// Brush the tiles
		//------------------------------------------------------------------------------------
		void TileTerrain2::BrushTile( float x, float y, int tileid, float BlendFactor, bool ChangeBackground, bool Mode2 )
		{
			if (!m_pImgBrush)
				return;

			int mx = x;
			int my = y;
			int id = 0;
			int b = 1;
			if (ChangeBackground)
				b = 0;
			MapIDData mid;

			x = mx % 64 * 8;
			y = my % 64 * 8;

			std::vector<MapIDData> vID = GetBlendMapIDs(mx, my);

			//================================
			// Render the image to the texture
			//================================
			for (int ids = 0; ids < vID.size(); ids++)
			{
				id = vID[ids].id;
				x = vID[ids].x % 512;
				y = vID[ids].y % 512;
				if (vID[ids].sy < 0)
				{
					y -= m_fBrushSize;
				}
				if (vID[ids].sx < 0)
				{
					x -= m_fBrushSize;
				}
				// Get texture and image data
				u8* pTexData	= m_ppBlendMaps[id]->GetCopyData(0);
				u8* pTexIDData	= m_ppTextureIDMaps[id]->GetCopyData(0);
				u8* pImgData	= m_pImgScaledBrush->GetData();

				// for v  and h
				int ix = 0, iy = -1, 
					is = m_fBrushSize * m_pImgScaledBrush->GetBytePerPixel(),
					iBpp = m_pImgScaledBrush->GetBytePerPixel();
				for (int v = y; v <  y + m_fBrushSize ; v++)
				{
					iy++;
					ix = -1;
					if (v < 0 || v >= 512)
						continue;
					for (int h = x; h <  x + m_fBrushSize; h++)
					{
						ix++;
						if (h < 0 || h >= 512)
							continue;

						// Read data and set them
						u8 i = pImgData[iy * is + ix * iBpp];
						bool bc = true;
						if (i > 0 && !ChangeBackground)
						{
							int t = pTexData[v * 512 + h];
							int t1 = pTexIDData[v * 1024 + h * 2];
							int t2 = pTexIDData[v * 1024 + h * 2 + 1];
							if (!Mode2 && t > i * BlendFactor && i > 0 && t2 != tileid * 4 + 2)
							{
								t -= i * BlendFactor;
								bc = false;
							}
							else if (Mode2 && t > i && i > 0 && t2 != tileid * 4 + 2)
							{
								bc = false;
							}
							else
								t += i * BlendFactor;
							if (t >= 255)
							{
								t = 255;
								//pTexIDData[v * 1024 + h * 2] = 4 * tileid + 2;
								//t = 0;
							}
							if (t < 0)
								t = 0;
							pTexData[v * 512 + h] = t;
						}
						if (bc && i > 0)
						{
							pTexIDData[v * 1024 + h * 2 + b] = u8(4 * tileid + 2);
						}
					}
				}

				// Set the texture
				m_ppBlendMaps[id]->SetData(pTexData, 512*512);
				m_ppTextureIDMaps[id]->SetData(pTexIDData, 512*512*2);
			}

		} // BrushTile

		//------------------------------------------------------------------------------------
		// Sets the brush texture and shape
		//------------------------------------------------------------------------------------
		void TileTerrain2::SetBrush( char* FileName )
		{
			if (m_pImgBrush)
			{
				m_pImgBrush->DecRef();
				m_pImgBrush = NULL;
			}

			int h = Device::GetSingletonPtr()->GetImageManager()->Add(FileName, NULL, NULL);
			m_pImgBrush = Device::GetSingletonPtr()->GetImageManager()->GetResource(h);

			SetBrushSize(m_fBrushSize);

		} // SetBrush

		void TileTerrain2::SetBrushSize( float fSize )
		{
			m_fBrushSize = fSize;

			// Create temp image
			m_pImgScaledBrush->CreateImage
				(
				m_pImgBrush->GetWidth(),
				m_pImgBrush->GetHeight(),
				1,
				m_pImgBrush->GetBytePerPixel(),
				m_pImgBrush->GetFormat(),
				m_pImgBrush->GetData()
				);

#if defined(DEBUG) || defined(_DEBUG)

			// Scale the image
			m_pImgScaledBrush->Scale(m_fBrushSize, m_fBrushSize, 1);

#endif // debug

		}

		//------------------------------------------------------------------------------------
		// Check the collisions between blend maps and brush
		//------------------------------------------------------------------------------------
		std::vector<TileTerrain2::MapIDData> TileTerrain2::GetBlendMapIDs( float x, float y )
		{
			std::vector<TileTerrain2::MapIDData> r;
			TileTerrain2::MapIDData midd;
			// Check the Top-Left
			float fx = x - m_fBrushSize / 16.0;
			float fy = y - m_fBrushSize / 16.0;

			int tx = fx / 64;
			int ty = fy / 64;
			int id = ty * (m_iNumUparts / 4 + 1) + tx;

			if (id >= 0 && id < m_iMapsCount)
			{
				midd.id = id;
				midd.x  = fx * 8.0;
				midd.y  = fy * 8.0;
				midd.sx = 1;
				midd.sy = 1;
				r.push_back(midd);
			}

			// Check the Top-Right
			fx = x + m_fBrushSize / 16.0;
			fy = y - m_fBrushSize / 16.0;

			tx = fx / 64;
			ty = fy / 64;
			int id2 = ty * (m_iNumUparts / 4 + 1) + tx;

			if (id2 >= 0 && id2 < m_iMapsCount && id2 != id)
			{
				midd.id = id2;
				midd.x  = fx * 8.0;
				midd.y  = fy * 8.0;
				midd.sx = -1;
				midd.sy = 1;
				r.push_back(midd);
			}

			// Check the Bottom-Left
			fx = x - m_fBrushSize / 16.0;
			fy = y + m_fBrushSize / 16.0;

			tx = fx / 64;
			ty = fy / 64;
			int id3 = ty * (m_iNumUparts / 4 + 1) + tx;

			if (id3 >= 0 && id3 < m_iMapsCount && id3 != id && id3 != id2)
			{
				midd.id = id3;
				midd.x  = fx * 8.0;
				midd.y  = fy * 8.0;
				midd.sx = 1;
				midd.sy = -1;
				r.push_back(midd);
			}

			// Check the Bottom-Right
			fx = x + m_fBrushSize / 16.0;
			fy = y + m_fBrushSize / 16.0;

			tx = fx / 64;
			ty = fy / 64;
			int id4 = ty * (m_iNumUparts / 4 + 1) + tx;

			if (id4 >= 0 && id4 < m_iMapsCount && id4 != id && id4 != id2 && id4 != id3)
			{
				midd.id = id4;
				midd.x  = fx * 8.0;
				midd.y  = fy * 8.0;
				midd.sx = -1;
				midd.sy = -1;
				r.push_back(midd);
			}

			return r;

		} // GetBlendMapIDs

		//------------------------------------------------------------------------------------
		// Sets the tile texture
		//------------------------------------------------------------------------------------
		void TileTerrain2::SetTileTexture
			( 
				gfx::Texture* pTileTex, int Width, int Height,
				int HorizantalTilesCount,
				int VerticalTilesCount,
				int TileSize
			)
		{
			m_pMaterial[0].ppTexture[0] = pTileTex;
			m_iTileW = Width;
			m_iTileH = Height;
			m_fHTilesCountInverse = 1.0 / HorizantalTilesCount;
			m_fVTilesCountInverse = 1.0 / VerticalTilesCount;
			m_iTileSizeInPixel = TileSize;
			m_iBitCount = 0;
			int i = TileSize;
			while (i > 1)
			{
				m_iBitCount++;
				i /= 2;
			}
		} // SetTileTexture


		const u8* TileTerrain2::GetBlendMapsData(int n) const
		{
			return m_ppBlendMaps[n]->GetCopyData(0);
		}

		const u8* TileTerrain2::GetTextureIDMapsData(int n) const
		{
			return m_ppTextureIDMaps[n]->GetCopyData(0);
		}

		void TileTerrain2::SetBlendMapsData(int n, u8* data, int size)
		{
			m_ppBlendMaps[n]->SetData(data, size, 0);
		}

		void TileTerrain2::SetTextureIDMapsData(int n, u8* data, int size)
		{
			m_ppTextureIDMaps[n]->SetData(data, size, 0);
		}

		//------------------------------------------------------------------------------------
		// Sets the tile in the terrain.
		//------------------------------------------------------------------------------------
		void TileTerrain2::SetTile( int Col, int Row, u32 TileID )
		{
			int mx = Col;
			int my = Row;
			int tx = mx / 64;
			int ty = my / 64;
			int id = ty * (m_iNumUparts / 4 + 1) + tx;
			tx = (mx % 64) * 8;
			ty = (my % 64) * 8;

				// Get texture and image data
				u8* pTexData	= m_ppBlendMaps[id]->GetCopyData(0);
				u8* pTexIDData	= m_ppTextureIDMaps[id]->GetCopyData(0);

				// for v and h
				int ix = 0, iy = -1, 
					is = 8 * m_pImgScaledBrush->GetBytePerPixel();
				for (int v = ty; v <  ty + 8 ; v++)
				{
					iy++;
					ix = -1;
					if (v < 0 || v >= 512)
						continue;
					for (int h = tx; h <  tx + 8; h++)
					{
						ix++;
						if (h < 0 || h >= 512)
							continue;

						// Read data and set them
						pTexData[v * 512 + h] = 255;
						pTexIDData[v * 1024 + h * 2 + 1] = u8(4 * TileID + 2);
					}
				}

				// Set the texture
				m_ppBlendMaps[id]->SetData(pTexData, 512*512);
				m_ppTextureIDMaps[id]->SetData(pTexIDData, 512*512*2);
			
		} // SetTile

		void TileTerrain2::userSetShaderParams()
		{
			if (m_shDirLightColor)
			{
				Light* pLit = m_pSnMan->GetNearestLight(math::Vector());
				m_pPshader->SetConstant(m_shDirLightColor, pLit->GetLightData()->Diffuse.c, 4);
			}
		}

		//------------------------------------------------------------------------------------
		// Creates the terrain shader code for deferred shading
		//------------------------------------------------------------------------------------
		void TileTerrain2::CreateDeferredShaderCode( core::String &VertexCode, core::String &PixelCode )
		{
			// Create vertex shader
			VertexCode = "float4x4 matViewProjection;\n"\
				"float4x4 matView;\n";
			VertexCode += "struct VS_INPUT\n"\
				"{\n"\
				"float4 Position : POSITION0;\n"\
				"float3 Normal : NORMAL;\n"\
				"float2 Texcoord : TEXCOORD0;\n"\
				"float4 color : COLOR0;\n";
			if (m_pMaterial[0].ppTexture[1])
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
				"float3	Normal: TEXCOORD3;\n"\
				"float4 depth: TEXCOORD4;\n"\
				"float4 color : COLOR0;\n";
			if (m_pMaterial[0].ppTexture[1])
			{
				VertexCode +=    
					"float3	BiNormal: TEXCOORD5;\n"\
					"float3	Tangent: TEXCOORD6;\n";
			}

			VertexCode += "};\n"\
				"VS_OUTPUT main( VS_INPUT Input )\n"\
				"{\n"\
				"VS_OUTPUT Output;\n"\
				"Output.Position = mul( Input.Position, matViewProjection );\n"\
				"Output.depth = mul( Input.Position, matView );\n"\
				"Output.Texcoord = Input.Texcoord;\n"\
				"Output.Height = Input.Position.y / 25.0;\n"\
				"Output.color = Input.color;\n";
			if (m_pMaterial[0].ppTexture[1])
			{
 				VertexCode +=    
 					"Output.Normal         = Input.Normal;\n"\
 					"Output.BiNormal       = Input.Binormal;\n"\
 					"Output.Tangent        = Input.Tangent;\n";
			}
			else
			{
				VertexCode +=    
					"Output.Normal         = Input.Normal;\n";

			}

			VertexCode += 	"return( Output );\n"\
				"}";



			// Create pixel shader
			PixelCode  = "sampler2D baseMap;\n"\
				"sampler2D BlendMap;\n"\
				"sampler2D TextureBlend: register(s2);\n"\
				"sampler2D TextureID: register(s3);\n"\
				"sampler2D TextureUnder: register(s4);\n";

			if (m_pMaterial[0].ppTexture[1])
			{
				PixelCode += "sampler2D bumpMap: register(s1);\n";
			}

			/// This function evaluates the mipmap LOD level for a 2D texture using the given texture coordinates
			/// and texture size (in pixels)
			PixelCode +=
				"float mipmapLevel(float2 uv, float textureSize)\n"\
				"{\n"\
				"    float2 dx = ddx(uv * textureSize);\n"\
				"    float2 dy = ddy(uv * textureSize);\n"\
				"    float d = max(dot(dx, dx), dot(dy, dy));\n"\
				"    return 0.5 * log2(d);\n"\
				"}\n";
			/// This function samples a texture with tiling and mipmapping from within a texture pack of the given
			/// attributes
			/// - tex is the texture pack from which to sample a tile
			/// - uv are the texture coordinates of the pixel *inside the tile*
			/// - tile are the coordinates of the tile within the pack (ex.: 2, 1)
			/// - packTexFactors are some constants to perform the mipmapping and tiling
			/// Texture pack factors:
			/// - inverse of the number of horizontal tiles (ex.: 4 tiles -> 0.25)
			/// - inverse of the number of vertical tiles (ex.: 2 tiles -> 0.5)
			/// - size of a tile in pixels (ex.: 1024)
			/// - amount of bits representing the power-of-2 of the size of a tile (ex.: a 1024 tile is 10 bits).
			PixelCode +=
				"float4 sampleTexturePackMipWrapped(in float2 uv, const in float2 tile,\n"\
				"	const in float4 packTexFactors)\n"\
				"{\n"\
				" 	/// estimate mipmap/LOD level\n"\
				"	float lod = mipmapLevel(uv, packTexFactors.z);\n"\
				"	lod = clamp(lod, 0.0, packTexFactors.w);\n"\
				"\n"\
				"	/// get width/height of the whole pack texture for the current lod level\n"\
				"	float size = pow(2.0, packTexFactors.w - lod);\n"\
				"	float sizex = size / packTexFactors.x; // width in pixels\n"\
				"	float sizey = size / packTexFactors.y; // height in pixels\n"\
				"\n"\
				"	/// perform tiling\n"\
				"	uv = frac(uv);\n"\
				"\n"\
				"	/// tweak pixels for correct bilinear filtering, and add offset for the wanted tile\n"\
				"	uv.x = uv.x * ((sizex * packTexFactors.x - 1.0) / sizex) + 0.5 / sizex + packTexFactors.x * tile.x;\n"\
				"	uv.y = uv.y * ((sizey * packTexFactors.y - 1.0) / sizey) + 0.5 / sizey + packTexFactors.y * tile.y;\n"\
				"\n"\
				"    return float4(uv, lod, 0);\n"\
				"}\n";

			PixelCode += 
				"struct PS_OUTPUT \n"\
				"{\n"\
				"	float4 outpos: COLOR0;\n"\
				"	float4 outdif: COLOR1;\n"\
				"	float4 outnor: COLOR2;\n"\
				"};\n"\
				"struct PS_INPUT \n"\
				"{\n"\
				"	float2 Texcoord : TEXCOORD0;\n"\
				"	float	Height: TEXCOORD2;\n"\
				"	float3	Normal: TEXCOORD3;\n"\
				"	float4 depth: TEXCOORD4;\n"\
				"   float4 color : COLOR0;\n";
			if (m_pMaterial[0].ppTexture[1])
			{
				PixelCode += 
					"float3	BiNormal: TEXCOORD5;\n"\
					"float3	Tangent: TEXCOORD6;\n";
			}
			PixelCode += "};\n"\
				"half4 encode (half3 n)\n"\
				"{\n"\
				"	half p = sqrt(n.z*8+8);\n"\
				"	return half4(n.xy/p + 0.5,0,0);\n"\
				"}\n"\
				"PS_OUTPUT main( PS_INPUT Input )\n"\
				"{\n"\
				"	PS_OUTPUT outps;\n";
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

			//------------------------------------------------------------------------------------
			// Testing start
			//------------------------------------------------------------------------------------
			// finding the tileid
			PixelCode += "	float4 id = tex2D(TextureID, (Input.Texcoord % 64.0) / 64.0);//return id.ragb;\n"\
				"	int2 tileid; tileid = id.ra * 64.0;\n"\
				"	float4 f4TileID; f4TileID.xz = tileid % ";
			PixelCode += m_iTileH / m_iTileSizeInPixel;
			PixelCode += "; f4TileID.yw = tileid / ";
			PixelCode += m_iTileW / m_iTileSizeInPixel;
			PixelCode += ";\n";
			// Calculate the UVs.
			PixelCode += "	float4 c = sampleTexturePackMipWrapped(Input.Texcoord *";
			float fUV  = 1.0 / (m_iTileSizeInPixel / 64.0);
			PixelCode += fUV;
			PixelCode += "\n, f4TileID.xy, float4(";
			PixelCode += m_fHTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_fVTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_iTileSizeInPixel;
			PixelCode += ", ";
			PixelCode += m_iBitCount;
			PixelCode += "));\n"\
				"	float4 d = sampleTexturePackMipWrapped(Input.Texcoord *";
			PixelCode += fUV;
			PixelCode += "\n, f4TileID.zw, float4(";
			PixelCode += m_fHTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_fVTilesCountInverse;
			PixelCode += ", ";
			PixelCode += m_iTileSizeInPixel;
			PixelCode += ", ";
			PixelCode += m_iBitCount;
			PixelCode += "));\n"\
				"	float4 a = tex2D(TextureBlend, frac(Input.Texcoord * 0.015625));//return a.a;\n";
			// Normal map
			if (m_pMaterial[0].ppTexture[1])
			{
				PixelCode += 
					"float3 fvNormal         = tex2Dlod( bumpMap, c ).wyz;\n"\
					"fvNormal = (1 - a.a) * fvNormal + a.a * tex2Dlod( bumpMap, d ).wyz;\n"\
					"fvNormal = fvNormal - float3(0.5,0.5,0.5);\n"\
					"float3 Nn = normalize(Input.Normal);\n"\
					"float3 Tn = normalize(Input.Tangent);\n"\
					"float3 Bn = normalize(Input.BiNormal);\n"\
					"Nn = Nn + fvNormal.x*Tn + fvNormal.y*Bn;\n"\
					"Nn = normalize(Nn);\n"\
					"//fvNormal = normalize(fvNormal.x * Input.Tangent + fvNormal.y * Input.BiNormal + fvNormal.z * Input.Normal);\n"\
					"outps.outpos = float4(length(Input.depth), Nn.z, 0.0, 0.0);\n"\
					"outps.outnor = Nn.xyxy;\n";
			}
			PixelCode += "	float4 b1 = tex2Dlod(baseMap, c);\n"\
				"	float4 b2 = tex2Dlod(baseMap, d);\n"\
				"	float4 r = b2 * a.a + (1 - a.a) * b1;\n";
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
			//  			if (m_bReceiveShadow)
			//  			{
			//  				PixelCode += "Input.ProjTex.xy /=  Input.ProjTex.w;\n"\
			//  					"Input.ProjTex.x   =  0.5f * Input.ProjTex.x + 0.5f;\n"\
			//  					"Input.ProjTex.y   = -0.5f * Input.ProjTex.y + 0.5f;\n";
			//  			}
			PixelCode += "	outps.outdif = r * Input.color ";
			if (m_bWater || m_bCaustics)
			{
				PixelCode += " * col";
			}
			PixelCode += ";\n"\
				"	return outps;\n"\
				"}\n";

			io::Logger::Log(VertexCode.ToCharPointer());
			io::Logger::Log(PixelCode.ToCharPointer());
			// 			io::File f;
			// 			f.Open("e:/ter2ps.txt", true);
			// 			f.write((void*)PixelCode.ToCharPointer(), PixelCode.GetLenght());
			// 			f.Open("e:/ter2vs.txt", true);
			// 			f.write((void*)VertexCode.ToCharPointer(), VertexCode.GetLenght());

		} // CreateDeferredShaderCode

	} // sn

} // kge
