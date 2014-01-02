// File name: ParticleRenderer.cpp
// Des: 
// Date: January 11, 2011
// Programmer: Nader Golbaz

#include "../../Headers/ps/Particle.h"
#include "../../Headers/ps/ParticleRenderer.h"
#include "../../Include/sn/StaticMesh.h"
#include "../../Include/gfx/Texture.h"
#include "../../Include/gfx/HardwareBuffer.h"
#include "../../Include/gfx/Material.h"

namespace kge
{

	namespace ps
	{
		//----------------------------------------
		//
		//----------------------------------------
		ParticleRenderer::ParticleRenderer(const math::Vector& rect, unsigned int horizontalImages,
			unsigned int verticalImages, bool additiveBlending)
			: m_rect(rect), m_horizontalImages(horizontalImages), m_verticalImages(verticalImages),
			m_material(0), m_vbuffer(0), m_ibuffer(0), m_bufferOffset(0)
		{
			if (additiveBlending)
			{
				m_blendingType = gfx::ERF_AdditiveBlending;
			}
			else
			{
				m_blendingType = gfx::ERF_AlphaBlending;
			}

			m_material = KGE_NEW(gfx::Material);
			//m_material->Culling = false;
		}

		//----------------------------------------
		//
		//----------------------------------------
		ParticleRenderer::~ParticleRenderer()
		{
			if (m_material->shader->m_pVertexShader)
			{
				m_material->shader->m_pVertexShader->DecRef();
			}

			if (m_material->shader->m_pPixelShader)
			{
				m_material->shader->m_pPixelShader->DecRef();
			}

			if (m_material->ppTexture[0])
			{
				m_material->ppTexture[0]->DecRef();
			}

			delete m_material;
			//delete m_vbuffer;
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::SetTextureRect(const math::Vector& rect, unsigned int horizontalFrames, unsigned int verticalFrames)
		{
			m_rect = rect;
			m_horizontalImages = horizontalFrames;
			m_verticalImages = verticalFrames;
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::SetTexture(gfx::Texture* texture)
		{
			if (m_material->ppTexture[0])
			{
				m_material->ppTexture[0]->DecRef();
			}

			m_material->ppTexture[0] = texture;
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::SetShader(gfx::Shader* vshader, gfx::Shader* pshader)
		{
			if (m_material->shader->m_pVertexShader)
			{
				m_material->shader->m_pVertexShader->DecRef();
			}

			if (m_material->shader->m_pPixelShader)
			{
				m_material->shader->m_pPixelShader->DecRef();
			}

			m_material->shader->m_pVertexShader = vshader;
			m_material->shader->m_pPixelShader = pshader;
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::SetBuffer(gfx::HardwareBuffer* vb)
		{
			//delete m_vbuffer;
			m_vbuffer = vb;

			m_bufferOffset = vb->GetDataCount() * sizeof(gfx::Vertex3CT);
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::SetSharedBuffer(gfx::HardwareBuffer* ib)
		{
			m_ibuffer = ib;
		}

		//----------------------------------------
		//
		//----------------------------------------
		void ParticleRenderer::EnableAdditiveBlending(bool additiveBlending)
		{
			if (additiveBlending)
			{
				m_blendingType = gfx::ERF_AdditiveBlending;
			}
			else
			{
				m_blendingType = gfx::ERF_AlphaBlending;
			}
		}

		//----------------------------------------
		//
		//----------------------------------------
		QuadRenderer::QuadRenderer(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending)
			: ParticleRenderer(rect, horizontalImages, verticalImages, additiveBlending)
		{

		}

		//----------------------------------------
		//
		//----------------------------------------
		void QuadRenderer::Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer)
		{
			if (!activeCount) return;

			gfx::Vertex3CT* v = 0;
			const math::Matrix view = renderer->GetTransForm(gfx::ETM_View);
			math::Vector rightVec(view._11, view._21, view._31);
			math::Vector upVec(view._12, view._22, view._32);

			rightVec.Normalize();
			upVec.Normalize();

			kge::ul32 flag = 0x00001000L;
			const kge::u32 sizeOfData = activeCount * 4 * sizeof(gfx::Vertex3CT);

			if (m_bufferOffset + sizeOfData > m_vbuffer->GetDataCount() * sizeof(gfx::Vertex3CT))
			{
				m_bufferOffset = 0;
				flag = 0x00002000L;
			}

			if (!(m_vbuffer->Lock(m_bufferOffset, sizeOfData, reinterpret_cast<void**>(&v), flag)))
			{
				return;
			}

			for (unsigned int j = 0; j < activeCount; ++j)
			{
				const float w = (m_rect.z - m_rect.x) / m_horizontalImages;
				const float h = (m_rect.w - m_rect.y) / m_verticalImages;
				const unsigned int m = particles.imageFrames[j] % m_horizontalImages;
				const unsigned int n = particles.imageFrames[j] / m_horizontalImages;

				math::Vector right = rightVec * particles.extents[j].x;
				math::Vector up = upVec * particles.extents[j].y;

				math::Vector vec3(particles.positions[j] - up - right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = (n + 1) * h;

				++v;

				vec3 = math::Vector(particles.positions[j] + up - right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = n * h;

				++v;

				vec3 = math::Vector(particles.positions[j] + up + right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = n * h;

				++v;

				vec3 = math::Vector(particles.positions[j] - up + right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = (n + 1) * h;

				++v;
			}

			m_vbuffer->Unlock();

			renderer->SetMaterial(m_material);

			math::Matrix mat = renderer->GetTransForm(gfx::ETM_Projection);
			mat *= view;
			math::Transpose(mat);

			m_material->shader->m_pVertexShader->SetConstant(&mat._11, 0, 4);

			renderer->Enable(m_blendingType); // TODO

			renderer->SetVertexBuffer(m_vbuffer);
			renderer->SetIndexBuffer(m_ibuffer);
			const kge::u32 vertexStart = m_bufferOffset / sizeof(gfx::Vertex3CT);
			renderer->DrawTriangleList(activeCount * 4, activeCount * 6, renderer->GetVertexDec(gfx::EVT_V3CT),
				vertexStart, 0);

			renderer->Disable(m_blendingType);

			m_bufferOffset += sizeOfData;
		}

		//----------------------------------------
		//
		//----------------------------------------
		RotatingQuadRenderer::RotatingQuadRenderer(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending)
			: ParticleRenderer(rect, horizontalImages, verticalImages, additiveBlending)
		{

		}

		//----------------------------------------
		//
		//----------------------------------------
		void RotatingQuadRenderer::Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer)
		{
			if (!activeCount) return;

			gfx::Vertex3CT* v = 0;

			kge::ul32 flag = 0x00001000L;
			const kge::u32 sizeOfData = activeCount * 4 * sizeof(gfx::Vertex3CT);

			if (m_bufferOffset + sizeOfData > m_vbuffer->GetDataCount() * sizeof(gfx::Vertex3CT))
			{
				m_bufferOffset = 0;
				flag = 0x00002000L;
			}

			if (!(m_vbuffer->Lock(m_bufferOffset, sizeOfData, reinterpret_cast<void**>(&v), flag)))
			{
				return;
			}

			const math::Matrix view = renderer->GetTransForm(gfx::ETM_View);
			math::Matrix billboard = view;
			billboard.Inverse();
			billboard._44 = 1.0f;

			for (unsigned int j = 0; j < activeCount; ++j)
			{
				const float sinTheta = sinf(particles.rotations[j]);
				const float cosTheta = cosf(particles.rotations[j]);
				const float left = -particles.extents[j].x;
				const float top = particles.extents[j].y;
				const float right = particles.extents[j].x;
				const float bottom = -particles.extents[j].y;
				const float w = (m_rect.z - m_rect.x) / m_horizontalImages;
				const float h = (m_rect.w - m_rect.y) / m_verticalImages;
				const unsigned int m = particles.imageFrames[j] % m_horizontalImages;
				const unsigned int n = particles.imageFrames[j] / m_horizontalImages;

				billboard._41 = particles.positions[j].x;
				billboard._42 = particles.positions[j].y;
				billboard._43 = particles.positions[j].z;

				math::Vector vec3(left * cosTheta - bottom * sinTheta, left * sinTheta + bottom * cosTheta, 0.0f);
				billboard.TransFormVec(vec3);
				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;

				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = (n + 1) * h;

				++v;

				vec3.set(left * cosTheta - top * sinTheta, left * sinTheta + top * cosTheta, 0.0f);
				billboard.TransFormVec(vec3);
				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;

				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = n * h;

				++v;

				vec3.set(right * cosTheta - top * sinTheta, right * sinTheta + top * cosTheta, 0.0f);
				billboard.TransFormVec(vec3);
				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;

				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = n * h;

				++v;

				vec3.set(right * cosTheta - bottom * sinTheta, right * sinTheta + bottom * cosTheta, 0.0f);
				billboard.TransFormVec(vec3);
				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;

				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = (n + 1) * h;

				++v;
			}

			m_vbuffer->Unlock();

			renderer->SetMaterial(m_material);

			math::Matrix location;

			math::Matrix mat = renderer->GetTransForm(gfx::ETM_Projection);
			mat *= view;
			math::Transpose(mat);

			m_material->shader->m_pVertexShader->SetConstant(&mat._11, 0, 4);

			renderer->Enable(m_blendingType);

			renderer->SetVertexBuffer(m_vbuffer);
			renderer->SetIndexBuffer(m_ibuffer);
			const kge::u32 vertexStart = m_bufferOffset / sizeof(gfx::Vertex3CT);
			renderer->DrawTriangleList(activeCount * 4, activeCount * 6, renderer->GetVertexDec(gfx::EVT_V3CT),
				vertexStart, 0);	

			renderer->Disable(m_blendingType);

			m_bufferOffset += sizeOfData;
		}

		//----------------------------------------
		//
		//----------------------------------------
		QuadRenderer2::QuadRenderer2(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending)
			: ParticleRenderer(rect, horizontalImages, verticalImages, additiveBlending)
		{

		}

		//----------------------------------------
		//
		//----------------------------------------
		void QuadRenderer2::Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer)
		{
			if (!activeCount) return;

			gfx::Vertex3CT* v = 0;
			math::Vector rightVec(1.0f, 0.0f, 0.0f); // TODO
			math::Vector upVec(0.0f, 0.0f, 1.0f); // TODO

			kge::ul32 flag = 0x00001000L;
			const kge::u32 sizeOfData = activeCount * 4 * sizeof(gfx::Vertex3CT);

			if (m_bufferOffset + sizeOfData > m_vbuffer->GetDataCount() * sizeof(gfx::Vertex3CT))
			{
				m_bufferOffset = 0;
				flag = 0x00002000L;
			}

			if (!(m_vbuffer->Lock(m_bufferOffset, sizeOfData, reinterpret_cast<void**>(&v), flag)))
			{
				return;
			}

			for (unsigned int j = 0; j < activeCount; ++j)
			{
				const float w = (m_rect.z - m_rect.x) / m_horizontalImages;
				const float h = (m_rect.w - m_rect.y) / m_verticalImages;
				const unsigned int m = particles.imageFrames[j] % m_horizontalImages;
				const unsigned int n = particles.imageFrames[j] / m_horizontalImages;

				math::Vector right = rightVec * particles.extents[j].x;
				math::Vector up = upVec * particles.extents[j].y;

				math::Vector vec3(particles.positions[j] - up - right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = (n + 1) * h;

				++v;

				vec3 = math::Vector(particles.positions[j] + up - right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = m * w;
				v->tex.Y = n * h;

				++v;

				vec3 = math::Vector(particles.positions[j] + up + right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = n * h;

				++v;

				vec3 = math::Vector(particles.positions[j] - up + right);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col.Set(particles.colors[j].r * 255, particles.colors[j].g * 255, particles.colors[j].b * 255, particles.colors[j].a * 255);
				v->tex.X = (m + 1) * w;
				v->tex.Y = (n + 1) * h;

				++v;
			}

			m_vbuffer->Unlock();

			renderer->SetMaterial(m_material);

			math::Matrix mat = renderer->GetTransForm(gfx::ETM_Projection);
			mat *= renderer->GetTransForm(gfx::ETM_View);
			math::Transpose(mat);

			m_material->shader->m_pVertexShader->SetConstant(&mat._11, 0, 4);

			renderer->Enable(m_blendingType); // TODO

			renderer->SetVertexBuffer(m_vbuffer);
			renderer->SetIndexBuffer(m_ibuffer);
			const kge::u32 vertexStart = m_bufferOffset / sizeof(gfx::Vertex3CT);
			renderer->DrawTriangleList(activeCount * 4, activeCount * 6, renderer->GetVertexDec(gfx::EVT_V3CT),
				vertexStart, 0);	

			renderer->Disable(m_blendingType);

			m_bufferOffset += sizeOfData;
		}











		////----------------------------------------
		////
		////----------------------------------------
		//MeshRenderer::MeshRenderer()
		//    : m_mesh(NULL)
		//{
		//}

		////----------------------------------------
		////
		////----------------------------------------
		//MeshRenderer::~MeshRenderer()
		//{
		//    if (m_mesh)
		//    {
		//        m_mesh->DecRef();
		//    }
		//}

		////----------------------------------------
		////
		////----------------------------------------
		//void MeshRenderer::Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer)
		//{
		//    if (!activeCount) return;

		//    // TODO

		//    for (unsigned int i = 0; i < activeCount; ++i)
		//    {
		//        //m_mesh->SetRotation();
		//        //m_mesh->SetScale(particles.extents[i] * 2.0f);
		//        
		//        m_mesh->Render();
		//    }

		//}

		////----------------------------------------
		////
		////----------------------------------------
		//void MeshRenderer::SetMesh(sn::StaticMesh* mesh)
		//{
		//    m_mesh = mesh;
		//}

	} // ps

} // kge
