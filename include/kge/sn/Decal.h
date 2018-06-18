// File name: Decal.h
// Des: This class create decals on terrain
// Date: 3/8/1388
// Programmers: Ali Akbar Mohammadi (Kochol)

#ifndef KGE_DECAL_H
#define KGE_DECAL_H

#include "SceneNode.h"

namespace kge
{
namespace gfx
{
	class HardwareBuffer;

} // gfx

namespace sn
{
	class TileTerrain;

class KGE_API Decal: public SceneNode
{
	friend class TileTerrain;

public:

	//! Constructor
	Decal();

	//! Destructor
	~Decal();

	//! The jobs before rendering like culling and calculate position
	virtual void PreRender(float elapsedTime = 0.0f);

	//! Render the objects
	virtual void Render();

	//! The jobs after rendering
	virtual void PostRender();

	/*! Draw the node with/without its Materials and Transforms.
	\param WithMaterial Draw it with its own materials or not?
	\param WithMaterial Draw it with its own Transformations or not?
	*/
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	// returns the decal material
	virtual gfx::Material* GetMaterial(u32 Index)
	{
		return m_pMaterial;
	}

	//! Rotates the decal
	void SetRotate(float Radian);

	//! Scales the decal the value must be between 0.0 and 100.0
	void SetScale(float Scale);

    //!
    void SetFadeoutTime(float fadeoutTime);

protected:

	gfx::Vertex3T*      m_pVertices;

	gfx::VertexType		m_eVertexType;

	u16*				m_pIndices;
	gfx::HardwareBuffer
					**	m_ppVertexBuffer,
					**	m_ppIndexBuffer;
	int					m_iIbOffset,
						m_iVbOffset;
	u32					m_iIndex;
	TileTerrain		*	m_pTerrain;

	gfx::Material*		m_pMaterial;

	math::Matrix*		m_pMatTexture;

	int					m_iVCount,
						m_iICount,
						m_iSizeX,
						m_iSizeY;
	float				m_fU,
						m_fV,
						m_fRot,
						m_fScale;
	bool				m_bPosChanged;

    float               m_totalTime;
    float               m_elapsedTime;
    float               m_alpha;

}; // Decal

} // sn

} // kge

#endif // KGE_DECAL_H
