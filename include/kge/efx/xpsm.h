#ifndef KGE_XPSM_H
#define KGE_XPSM_H

#include "ShadowMapProjection.h"

namespace kge
{
	namespace efx
	{
		class KGE_API XPSM: public ShadowMapProjection
		{
		public:
			//! Constructor
			XPSM(int ShadowMapSize, sn::SceneManager* pSmgr);

			//! Destructor
			~XPSM();

			//! Inits the XPSM
			virtual bool Init();

			//! The works must to do before rendering the object.
			void PreRender(float elapsedTime = 0.0f);

			//! The works must to do after rendering the object.
			void Render();

			//! The works must to do after rendering all of the scene nodes.
			void PostRenderEverything();

			//various bias values
			float m_XPSM_Coef;
			float m_XPSM_Bias;
			float m_XPSM_ZBias;
			float m_XPSM_EpsilonW;

		protected:

			float m_fAspect, m_zNear, m_zFar, m_ppNear, m_ppFar, m_fSlideBack, m_fLSPSM_Nopt, m_fCosGamma;

			math::Matrix m_LightViewProj;

			//! Builds XPSM Projection Matrix
			void BuildXPSMProjectionMatrix();

			void TransformWithWClip(
				const std::vector<gfx::Vertex3>& inVectors, 
				std::vector<gfx::Vertex3>&       outVectors,
				const math::Matrix&               transform,
				float wThreshold);

			bool IntersectAABB2D(
				math::AABB& c, 
				const math::AABB& a, 
				const math::AABB& b);


		}; // XPSM

	} // efx

} // kge

#endif //  KGE_XPSM_H
