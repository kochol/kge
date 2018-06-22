// File name: structs.h
// Date: 30/5/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef GFX_STRUCTS_H
#define GFX_STRUCTS_H

#include "../kgedef.h"
#include "../math/Matrix.h"
#include "../math/Vector.h"
#include "Color.h"
#include <math.h>

namespace kge
{
namespace gfx
{

//! The Vertex Type
enum VertexType
{
	EVT_V2 = 0,		/**< 2D Vertex. */
	EVT_V3,			/**< 3D Vertex. */
	EVT_V3C,		/**< 3D Vertex with color. */
	EVT_V3T,		/**< 3D Vertex with texture. */
	EVT_V3CT,       /**< 3D Vertex with color and texture coordinate. */
	EVT_V3TN,		/**< 3D Vertex with texture and normal vector. */
	EVT_V3TNC,		/**< 3D Vertex with texture and normal vector and color. */
	EVT_V3TTN,		/**< 3D Vertex with two texcoord and normal vector. */
	EVT_P0NT1BT2I3,	/**< Vertex3 + Normal vector + Texture coord + Tangent + Binormal in multi stream mode. */
	EVT_P0NTC1BT2,/**< Vertex3 + Normal vector + Texture coord + Color + Tangent + Binormal in multi stream mode. */
	EVT_PJ0NT1BT2I3,/**< Vertex3 + Joint index for hardware skinning + Normal vector + Texture coord + Tangent + Binormal + Instanced data in multi stream mode. */
// 	EVT_P0NT1BT2I3,	/**< Vertex3 + Normal vector + Texture coord + Tangent + Binormal in multi stream mode. */
// 	EVT_P0NTC1BT2I3,/**< Vertex3 + Normal vector + Texture coord + Color + Tangent + Binormal + Instance data in multi stream mode. */
// 	EVT_PJ0NT1BT2I3,/**< Vertex3 + Joint index for hardware skinning + Normal vector + Texture coord + Tangent + Binormal + Instance data in multi stream mode. */
// 	EVT_P0NT1I2,	/**< Vertex3 + Normal vector + Texture coord + Instance data in multi stream mode. */
// 	EVT_P0NTC1I2,	/**< Vertex3 + Normal vector + Texture coord + Color + Instance data in multi stream mode. */
// 	EVT_PJ0NT1I2,	/**< Vertex3 + Joint index for hardware skinning + Normal vector + Texture coord + Instance data in multi stream mode. */

	EVT_Count		/**< For internal use */

}; // VertexType

struct Vertex2
{
	float X;	//! X
	float Y;	//! Y
}; // Vertex2

//! 3D Vertex
/** Az Vertex baraye moshakhas kardane noghat to faza estefade mishe. */
struct Vertex3
{
	float X; /**< X */
	float Y; /**< Y */
	float Z; /**< Z */

	Vertex3() : X(0.0f), Y(0.0f), Z(0.0f) {}

	Vertex3(float x, float y, float z) : X(x), Y(y), Z(z) {}

	/*! Harekat va charkheshe noghte tavasote matrix.
		\param mat Matrixe havi etelaate charkhesh va rotate.
	*/
	inline void Transform4(math::Matrix * mat)
	{
		float xt,yt,zt;
		xt= X * mat->m_fMat[0] + 
			Y * mat->m_fMat[4] +
			Z * mat->m_fMat[8] + 
				mat->m_fMat[12];
		yt= X * mat->m_fMat[1] + 
			Y * mat->m_fMat[5] + 
			Z * mat->m_fMat[9] + 
				mat->m_fMat[13];
		zt= X * mat->m_fMat[2] + 
			Y * mat->m_fMat[6] + 
			Z * mat->m_fMat[10]+
				mat->m_fMat[14];
		X = xt;
		Y = yt;
		Z = zt;
	} // Transform4

	inline void Transform3(math::Matrix * mat)
	{
		float xt,yt,zt;
		xt= X * mat->m_fMat[0] + 
			Y * mat->m_fMat[4] +
			Z * mat->m_fMat[8];
		yt= X * mat->m_fMat[1] + 
			Y * mat->m_fMat[5] + 
			Z * mat->m_fMat[9];
		zt= X * mat->m_fMat[2] + 
			Y * mat->m_fMat[6] + 
			Z * mat->m_fMat[10];
		X = xt;
		Y = yt;
		Z = zt;
	} // Transform3

	inline Vertex3 operator - (const Vertex3 &o) const
	{
		Vertex3 r;
		r.X = X - o.X;
		r.Y = Y - o.Y;
		r.Z = Z - o.Z;

		return r;
	}

	inline Vertex3 operator + (const Vertex3 &o) const
	{
		Vertex3 r;
		r.X = X + o.X;
		r.Y = Y + o.Y;
		r.Z = Z + o.Z;

		return r;
	}

	inline Vertex3 operator * (const float &f) const
	{
		Vertex3 r;
		r.X = X * f;
		r.Y = Y * f;
		r.Z = Z * f;

		return r;
	}

	//inline Vertex3 operator *= (const float &f) const
	//{
	//	Vertex3 r;
	//	r.X = X * f;
	//	r.Y = Y * f;
	//	r.Z = Z * f;

	//	return r;
	//}

	inline void Normalize()
	{
		float f = (float)sqrt(X*X + Y*Y + Z*Z);
		if (f != 0.0f)
		{
			X /= f;
			Y /= f;
			Z /= f;
		}
	} // Normalize

	inline kge::math::Vector ToVector()
	{
		math::Vector v;
		v.x = X;
		v.y = Y;
		v.z = Z;
		return v;

	} // ToVector

	inline void Cross(const Vertex3& v1, const Vertex3& v2)
	{
		X=v1.Y*v2.Z-v1.Z*v2.Y;
		Y=v1.Z*v2.X-v1.X*v2.Z;
		Z=v1.X*v2.Y-v1.Y*v2.X;

	} // Cross

}; // Vertex3

//! 3D Vertex with color
struct Vertex3C
{
	Vertex3 pos;	/**< Makan */
	Color	c;		/**< Rang */
}; // Vertex3C	

//! 3D Vertex with One texture.
struct Vertex3T
{
	Vertex3 pos;	//! Makane vertex.
	Vertex2 tex;	//! Makane texture.
}; // Vertex3T

//! 3D Vertex with color and texture coordinate.
struct Vertex3CT
{
	Vertex3 pos;    //! Vertex position
	Color   col;    //! Vertex color
	Vertex2 tex;    //! Texture coordinate
}; // Vertex3CT

//! 3D Vertex with one texture and normal vector.
struct Vertex3TN
{
	Vertex3 pos;	//! Makane Vertex.
	Vertex3 Nor;	//! Normal Vector.
	Vertex2 tex;	//! Makane Texture.
}; // Vertex3TN

//! 3D Vertex with two texcoord and normal vector.
struct Vertex3TTN
{
	Vertex3 pos;	//! Posision Vertex.
	Vertex3 Nor;	//! Normal Vector.
	Vertex2 tex0;	//! First texcoord.
	Vertex2 tex1;	//! Second texcoord.

}; // Vertex3TTN

//! 3D vertex with texture, normal and color.
struct Vertex3TNC
{
	Vertex3 pos;	//*< Makane Vertex.
	Vertex3 Nor;	//*< Normal Vector.
	Vertex2 tex;	//*< Makane Texture.
	float   Color;  /**< Color */
}; // Vertex3TNC

//! Normal, texture and color.
struct VertexNTC
{
	Vertex3 Nor;	//*< Normal Vector.
	Vertex2 tex;	//*< Texture coordinate.
	ul32    Color;  /**< Color */

}; // Vertex3TNC

//! 3D vertex with Bone ID
struct Vertex3I
{
	Vertex3 pos;
	int		BoneID;
};

//! Etelate marbot be texture haye load shode ke tavasote SceneManager zakhire mishe.
struct TextureInfo
{
	u32 TextureID;			//*< Texture ha dar renderer ba in shenase ha shenakhte mishavand.
	char* Name;				//*< Texture name.

}; // Texture.

//! Light Type
enum LightType
{
	/// Directional Light.
	ELT_Directional,
	/// Point Light.
	ELT_Point,
	/// Spot Light.
	ELT_Spot
};

//! Light structure
struct LightData
{
	/// Ambient color.
	Colorf		 Ambient;		
	/// Diffuse color.
	Colorf		 Diffuse;		
	/// Specular color.
	Colorf		 Specular;		
	/// Light Position. This value dose not needed for directional light.
	math::Vector Position;
	/// Light Direction. This value dose not needed for point light.
	math::Vector Direction;
	/// The distance that light can go. This value dose not needed for directional light.
	float        Range;			
	/// The smaller cone in the spot light.
	float        Theta;
	/// The bigger cone in the spot light.
	float		 Phi;			
	/// The type of the light.
	LightType	 Type;
	/// The light index TODO: this must be deleted after sorting the lights.
	int			 Index;

}; // LightSpot

//! Fog type
enum FogType
{
	EFT_NONE,
	EFT_LINEAR,
	EFT_EXP,
	EFT_EXP2
}; // FogType

// byte-align structures
#ifdef _MSC_VER
#	pragma pack(push, packing)
#	pragma pack(1)
#	define PACK_STRUCT
#elif defined(__GNUC__)
#	pragma pack(1)
#else
#	error compiler not supported
#endif

// Rotation/Translation information for joints
struct KeyFrame
{
	float m_fTime;
	float m_fParam[3];
}; // KeyFrame

// Bone Joints for animation
struct Joint
{
	char m_cName[32]; //Bone name
	Vertex3 m_vRotation; //Starting rotation
	Vertex3 m_vPosition; //Starting position
	u16 m_usNumRotFrames; //Number of rotation frames
	u16 m_usNumTransFrames; //Number of translation frames
	KeyFrame * m_RotKeyFrames; //Rotation keyframes
	KeyFrame * m_TransKeyFrames; //Translation keyframes
	short m_sParent; //Parent joint index
	math::Matrix m_matLocal;
	math::Matrix m_matAbs;
	math::Matrix m_matFinal;
	u16 m_usCurRotFrame;
	u16 m_usCurTransFrame;
}; // Joint

// Default alignment
#ifdef _MSC_VER
#	pragma pack(pop, packing)
#elif defined(__GNUC__)
#	pragma pack()
#endif

} // gfx

} // kge

#endif // GFX_STRUCTS_H
