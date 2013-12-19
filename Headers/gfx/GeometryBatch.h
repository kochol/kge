#ifndef KGE_GEOMETRYBATCH_H
#define KGE_GEOMETRYBATCH_H

#include <vector>
#include "../../Include/gfx/VertexDec.h"
#include "../../Include/math/Matrix.h"

namespace kge
{
namespace sn
{
	//------------------------------------------------------------------------------------
	// Forward deceleration
	//------------------------------------------------------------------------------------
	class StaticMesh;
	class AnimatedMesh;
}
namespace gfx
{

	class DynamicHardwareBuffer;
	class MeshBuffer;
	class Material;

/// Class for rendering the instanced meshes
class GeometryBatch
{
public:

	//! Constructor
	GeometryBatch(MeshBuffer* mb);

	//! Destructor
	~GeometryBatch();

	/// Adds a static mesh as instance.
	/// \return Returns false if there is not space for adding the new instance
	bool AddInstance(sn::StaticMesh* pSmesh);

	/// Adds an animated mesh as instance.
	/// \return Returns false if there is not space for adding the new instance
	bool AddInstance(sn::AnimatedMesh* pAmesh);

	/// Update the instance buffer and Render the instances
	void Render();

	/// For rendering shadows
	void RenderShadow(Material* pMat1, Material* pMat2);

protected:

	/// Static mesh instances
	std::vector<sn::StaticMesh*>			m_vStaticInstances;

	/// Static mesh instances
	std::vector<sn::AnimatedMesh*>			m_vAnimatedInstances;

	/// Vertex buffer for storing instanced data
	DynamicHardwareBuffer				*	m_pInstancedVB;

	VertexDec							*	m_pVDec;		///< For Normal rendering

	MeshBuffer							*	m_pMeshBuffer;

	/// Max Animated mesh instance to accept
	int										m_iMaxAnimatedInstance;

	/// Pixel count for bones
	int										m_iPixelNeedCount;

	float									m_fX,
											m_fY;

	/// Render static instances
	void RenderStaticInstances(Material* pMat1 = NULL, Material* pMat2 = NULL);

	/// Render animated instances
	void RenderAnimatedInstances();

private:

}; // GeometryBatch

} // gfx

} // kge

#endif // KGE_GEOMETRYBATCH_H
