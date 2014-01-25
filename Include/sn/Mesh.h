// File name: Mesh.h
// Des: In class ye mesh standard hast ke tavasote engine namayesh dade mishe.
// Date: 13/7/1385
// Programmer: Ali Akbar Mohammadi(Kochol)

#ifndef MESH_H
#define MESH_H

#include "../kgedef.h"
#include "../gfx/Structs.h"
#include "SceneManager.h"
#include "../gfx/Renderer.h"
#include "../efx/Effect.h"
#include "../efx/MirrorPlane.h"
#include "../efx/ShadowPlane.h"

namespace kge
{
namespace sn
{
class KGE_API Mesh : public SceneNode
{
	friend class MeshMS3D;
public:

	/// Constructor.
	Mesh();

	/// Constructor.
	/// \param sn A pointer to SceneManager.
	/// \param rn A pointer to Renderer.
	Mesh(SceneManager *sn, gfx::Renderer *rn);

	// Destructor
	~Mesh();

	//! Karhayee ke Ghabl az render bayad anjam shvad. Mesle colision detection.
	void PreRender(float elapsedTime = 0.0f);

	/// Render the mesh.
	void Render();

	//! Render with options
	void Render(bool bPosition, bool bNormalTexcoord, bool bTangentBinormal);

	//! Karhaee ke bad az render bayad anjam beshe.
	virtual void PostRender();

	/*! Ezafe kardane effect be  hamin gereh.
		\param p Effecti ke mikhahid ezafe konid.
		\param ppOut Class Effect sakhte shode.
		\return agar ke in gereh betavanad in effect ra bepazirad true bar migardanad.
	 */
	virtual bool AddEffect ( efx::EffectType p, efx::Effect** ppOut );

	/*! Draw the mesh with/without its Materials and Transforms.
		\param WithMaterial Draw it with its own materials or not?
		\param WithMaterial Draw it with its own Transformations or not?
	 */
	virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal );

	/*! Prepare the buffers for multi stream
	 */
	virtual void PrepareBuffers(bool CreateTangents = false, bool Dynamic = false);

	//! Refills vertex and index buffers
	virtual void RefillBuffers();

	core::String	  m_sName;			// Name of the mesh.

	gfx::Vertex3TN	* m_pVerts;			// Vertices.
	gfx::Vertex3	* m_pPos;			// Positions.
	gfx::Vertex3T	* m_pNorTex;		// Normal & Texcoords
	gfx::VertexNTC	* m_pNorTexColor;	// Normal + Texcoords + Color
	gfx::Vertex3	* m_pTangentBinormal;// The Tangent and binormal
	u16				* m_pIndices;		// Indices.
	u32				* m_pIndices32;

	u32				  m_NumVerts,		// vertices count.
					  m_NumIndices;		// Indices count.

	gfx::HardwareBuffer *m_pIndexBuffer,		// The index buffer
						*m_pvbPosition,			// The position vertex buffer
						*m_pvbNormalTexcoord,	// The Normal & Texcoord vertex buffer
						*m_pvbTangentBinormal;	// The Tangent and binormal vertex buffer

	s16				* m_Bone;			// Bone(Joint) index.

	bool				m_bHasMat;		///< Has a material?
	bool				m_bInstanced;	///< Instanced mesh

	gfx::Material	  m_Mat;			// THE Material.

	gfx::Renderer	* m_pRenderer;		// Renderer pointer.

	gfx::VertexDec	*	m_pVertexDec;

protected:
	//-----------------------
	//   F U N C T I O N S   
	//-----------------------

	// Create Mirror Plane.
	void CreateMirrorPlane(efx::Effect** ppCreatedMirror);

	// Create Shadow Plane.
	void CreateShadowPlane(efx::Effect** ppCreatedShadow , math::Vector &vLightDir);

	//! Create tangent and binormal buffers
	void CreateTangentBinormalBuffers(bool Dynamic);

	//! Prepare the buffers for multi stream
	void PrepareBuffers2(bool CreateTangents, bool Dynamic);

	//! Create tangent and binormal buffers
	void CreateTangentBinormalBuffers2(bool Dynamic);

}; // Mesh

} // sn

} // kge

#endif // MESH_H
