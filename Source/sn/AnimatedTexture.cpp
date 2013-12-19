// File name: AnimatedTexture.cpp
// Des: 
// Date: February 08, 2011 (19/11/1389)
// Programmer: Nader Golbaz

#include <d3d9.h>
#include "../../Include/sn/AnimatedTexture.h"
#include "../../Include/gfx/Texture.h"
#include "../../Include/gfx/HardwareBuffer.h"
#include "../../Include/gfx/Material.h"
#include "../../Include/sn/SceneManager.h"
#include "../../Include/gfx/Renderer.h"

namespace kge
{

namespace sn
{
	//----------------------------------------
	//
	//----------------------------------------
	AnimatedTexture::AnimatedTexture(const AnimatedTextureParameters& p) :
		m_pMaterial(0),
		//m_pVB(0),
		m_pRB(0),
		m_bufferOffset(0),
		m_sizeOfData(6 * sizeof(gfx::Vertex3CT)),
		m_rect(p.textureRect),
		m_horizontalImages(p.horizontalImages),
		m_verticalImages(p.verticalImages),
		m_initialColor(p.initialColor),
		m_middleColor(p.middleColor),
		m_finalColor(p.finalColor),
		m_duration(p.fDuration),
		m_angularVelocity(p.fAngularVelocity),
		m_fRot(0.0f),
		m_repeat(false),
		m_elapsed(0.0f),
		m_running(false),
		m_fixedAxis(p.fixedAxis),
		m_positionOnly(p.positionOnly)
	{
		m_size.x = p.size.x * 0.5f;
		m_size.y = p.size.y * 0.5f;
		m_size.z = p.size.z * 0.5f;
		m_size.w = p.size.w * 0.5f;

		m_middleSize.x = p.middleSize.x * 0.5f;
		m_middleSize.y = p.middleSize.y * 0.5f;
		m_middleSize.z = p.middleSize.z * 0.5f;
		m_middleSize.w = p.middleSize.w * 0.5f;

		m_eNodeType = sn::ENT_AnimatedTexture;
		m_pMaterial = KGE_NEW(gfx::Material);
		m_pAABB = KGE_NEW(math::AABB);

		m_pMaterial->Alpha = true;
		//m_pMaterial->Culling = false;

		float fMax = m_size.x > m_size.y ? m_size.x : m_size.y;
		fMax = fMax > m_size.z ? fMax : m_size.z;
		fMax = fMax > m_size.w ? fMax : m_size.w;
		fMax = fMax > m_middleSize.x ? fMax : m_middleSize.x;
		fMax = fMax > m_middleSize.y ? fMax : m_middleSize.y;

		m_pAABB->AddInternalPoint(math::Vector(-fMax, -fMax, -fMax));
		m_pAABB->AddInternalPoint(math::Vector( fMax,  fMax,  fMax));
	}

	//----------------------------------------
	//
	//----------------------------------------
	AnimatedTexture::~AnimatedTexture()
	{
		if (m_pMaterial->shader->m_pVertexShader)
		{
			m_pMaterial->shader->m_pVertexShader->DecRef();
		}

		if (m_pMaterial->shader->m_pPixelShader)
		{
			m_pMaterial->shader->m_pPixelShader->DecRef();
		}

		if (m_pMaterial->ppTexture[0])
		{
			m_pMaterial->ppTexture[0]->DecRef();
		}

		//if (m_pVB)
		//{
		//	delete m_pVB;
		//}

		m_pSnMan->FreeRBuffer(m_pRB);

		delete m_pAABB;
		delete m_pMaterial;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::PreRender(float elapsedTime)
	{
		if (!m_running) { return; }

		if (m_pParent)
		{
			if (m_positionOnly)
			{
				*m_pFinalMat = *m_pAbsMat;
				m_pFinalMat->_41 += m_pParent->GetFinalMatrix()->_41;
				m_pFinalMat->_42 += m_pParent->GetFinalMatrix()->_42;
				m_pFinalMat->_43 += m_pParent->GetFinalMatrix()->_43;
			}
			else
			{
				*m_pFinalMat = *m_pParent->GetFinalMatrix() * (*m_pAbsMat);
			}
		}
		else
		{
			*m_pFinalMat = *m_pAbsMat;
		}

		const float t = m_duration / static_cast<float>(m_horizontalImages * m_verticalImages);
		const unsigned int frame = floorf(m_elapsed / t);
		const float lifetime = m_elapsed / m_duration;
		m_fRot += m_angularVelocity * elapsedTime;

		m_elapsed += elapsedTime;

		const float a = (1.0f - lifetime) * (1.0f - lifetime);
		const float b = 2.0f * (1.0f - lifetime) * lifetime;
		const float c = lifetime * lifetime;

		float width = a * m_size.x + b * m_middleSize.x + c * m_size.z;
		float height = a * m_size.y + b * m_middleSize.y + c * m_size.w;

		gfx::Colorf color(
			a * m_initialColor.r + b * m_middleColor.r + c * m_finalColor.r,
			a * m_initialColor.g + b * m_middleColor.g + c * m_finalColor.g,
			a * m_initialColor.b + b * m_middleColor.b + c * m_finalColor.b,
			a * m_initialColor.a + b * m_middleColor.a + c * m_finalColor.a);

		if (m_elapsed >= m_duration)
		{
			if (m_repeat)
			{
				m_elapsed = 0.0f;
			}
			else
			{
				m_running = false;
				m_fRot = 0.0f;
			}
		}

		if (!m_bVis || (m_bAutoCulling && (m_pSnMan->IsVisible(this) != math::ECT_In)))
		{
			return;
		}

		gfx::Vertex3CT* v = 0;

		math::Matrix billboard;
		
		if (!m_fixedAxis)
		{
			billboard = m_pRenderer->GetTransForm(gfx::ETM_View);
			billboard.Inverse();
		}
		else
		{
			billboard._22 = 0.0f; billboard._23 = 1.0f;
			billboard._32 = 1.0f; billboard._33 = 0.0f;
		}

		billboard._41 = m_pFinalMat->_41;
		billboard._42 = m_pFinalMat->_42;
		billboard._43 = m_pFinalMat->_43;
		billboard._44 = 1.0f;

		const float sinTheta = sinf(m_fRot);
		const float cosTheta = cosf(m_fRot);
		const float w = (m_rect.z - m_rect.x) / m_horizontalImages;
		const float h = (m_rect.w - m_rect.y) / m_verticalImages;
		const unsigned int m = frame % m_horizontalImages;
		const unsigned int n = frame / m_horizontalImages;

		kge::ul32 flag = D3DLOCK_NOOVERWRITE;

		if (m_bufferOffset + m_sizeOfData > m_pRB->size * sizeof(gfx::Vertex3CT))
		{
			m_bufferOffset = 0;
			flag = D3DLOCK_DISCARD;
		}

		if (!(m_pRB->hb->Lock(m_bufferOffset, m_sizeOfData, reinterpret_cast<void**>(&v), flag)))
		{
			return;
		}

		// first triangle

		math::Vector vec3((-width) * cosTheta - (-height) * sinTheta, (-width) * sinTheta + (-height) * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = m * w;
		v->tex.Y = (n + 1) * h;

		++v;

		vec3.set((-width) * cosTheta - height * sinTheta, (-width) * sinTheta + height * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = m * w;
		v->tex.Y = n * h;

		++v;

		vec3.set(width * cosTheta - height * sinTheta, width * sinTheta + height * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = (m + 1) * w;
		v->tex.Y = n * h;

		++v;

		// second triangle

		vec3.set(width * cosTheta - height * sinTheta, width * sinTheta + height * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = (m + 1) * w;
		v->tex.Y = n * h;

		++v;

		vec3.set(width * cosTheta - (-height) * sinTheta, width * sinTheta + (-height) * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = (m + 1) * w;
		v->tex.Y = (n + 1) * h;

		++v;

		vec3.set((-width) * cosTheta - (-height) * sinTheta, (-width) * sinTheta + (-height) * cosTheta, 0.0f);
		billboard.TransFormVec(vec3);

		v->pos.X = vec3.x;
		v->pos.Y = vec3.y;
		v->pos.Z = vec3.z;
		v->col.Set(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		v->tex.X = m * w;
		v->tex.Y = (n + 1) * h;

		m_pRB->hb->Unlock();

		m_pSnMan->RegNode(this, ERO_Last);
	}

	//----------------------------------------
	// 
	//----------------------------------------
	void AnimatedTexture::Render()
	{
		math::Matrix mat = m_pRenderer->GetTransForm(gfx::ETM_Projection);
		mat *= m_pRenderer->GetTransForm(gfx::ETM_View);
		math::Transpose(mat);

		m_pMaterial->shader->m_pVertexShader->SetConstant(&mat._11, 0, 4);

		m_pRenderer->SetMaterial(m_pMaterial);

		m_pRenderer->SetVertexBuffer(m_pRB->hb);
		m_pRenderer->DrawTriangleList(6, 0, m_pRenderer->GetVertexDec(gfx::EVT_V3CT),
			m_bufferOffset / sizeof(gfx::Vertex3CT), 0);

		m_bufferOffset += m_sizeOfData;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::PostRender()
	{

	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::Draw( bool WithMaterial , bool WithTransform, bool bPosition /* = true */, bool bNormalTexcoord /* = true */, bool bTangentBinormal /* = true */ )
	{

	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::Play(bool repeat /* = false */)
	{
		m_elapsed = 0.0f;
		m_running = true;
		m_repeat = repeat;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::Stop()
	{
		m_elapsed = 0.0f;
		m_running = false;
		m_repeat = false;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetTexture(const char* filename)
	{
		m_textureName = filename;

		if (m_pMaterial->ppTexture[0])
		{
			m_pMaterial->ppTexture[0]->DecRef();
		}

		m_pMaterial->ppTexture[0] = m_pSnMan->AddTexture(filename);
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetShader(const char* filename)
	{
		m_shaderName = filename;

		if (m_pMaterial->shader->m_pVertexShader)
		{
			m_pMaterial->shader->m_pVertexShader->DecRef();
		}

		if (m_pMaterial->shader->m_pPixelShader)
		{
			m_pMaterial->shader->m_pPixelShader->DecRef();
		}

		m_pMaterial->shader->m_pVertexShader = m_pSnMan->AddVertexShader(filename, "vs", gfx::ESV_VS2);
		m_pMaterial->shader->m_pPixelShader = m_pSnMan->AddPixelShader(filename, "ps", gfx::ESV_PS2);
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetVertexBuffer()
	{
		//if (m_pVB) m_pVB->DecRef();
		//m_pVB = m_pRenderer->CreateVertexBuffer(NULL, 6 * 3, gfx::EVT_V3CT, true);

		m_pSnMan->FreeRBuffer(m_pRB);
		m_pRB = m_pSnMan->GetRBuffer(6 * 3);

		m_bufferOffset = m_pRB->size * sizeof(gfx::Vertex3CT);
	}
	
	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetTextureRect(const math::Vector& rect, unsigned int horizontalFrames, unsigned int verticalFrames)
	{
		m_rect = rect;
		m_horizontalImages = horizontalFrames;
		m_verticalImages = verticalFrames;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetSize(const math::Vector& size)
	{
		m_size.x = size.x * 0.5f;
		m_size.y = size.y * 0.5f;
		m_size.z = size.z * 0.5f;
		m_size.w = size.w * 0.5f;

		float fMax = m_size.x > m_size.y ? m_size.x : m_size.y;
		fMax = fMax > m_size.z ? fMax : m_size.z;
		fMax = fMax > m_size.w ? fMax : m_size.w;
		fMax = fMax > m_middleSize.x ? fMax : m_middleSize.x;
		fMax = fMax > m_middleSize.y ? fMax : m_middleSize.y;

		m_pAABB->Reset();
		m_pAABB->AddInternalPoint(math::Vector(-fMax, -fMax, -fMax));
		m_pAABB->AddInternalPoint(math::Vector( fMax,  fMax,  fMax));
	}

	//----------------------------------------
	//
	//----------------------------------------
	void AnimatedTexture::SetMiddleSize(const math::Vector& size)
	{
		m_middleSize.x = size.x * 0.5f;
		m_middleSize.y = size.y * 0.5f;
		m_middleSize.z = size.z * 0.5f;
		m_middleSize.w = size.w * 0.5f;

		float fMax = m_size.x > m_size.y ? m_size.x : m_size.y;
		fMax = fMax > m_size.z ? fMax : m_size.z;
		fMax = fMax > m_size.w ? fMax : m_size.w;
		fMax = fMax > m_middleSize.x ? fMax : m_middleSize.x;
		fMax = fMax > m_middleSize.y ? fMax : m_middleSize.y;

		m_pAABB->Reset();
		m_pAABB->AddInternalPoint(math::Vector(-fMax, -fMax, -fMax));
		m_pAABB->AddInternalPoint(math::Vector( fMax,  fMax,  fMax));
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector AnimatedTexture::GetSize() const
	{
		math::Vector size;
		size.x = m_size.x * 2.0f;
		size.y = m_size.y * 2.0f;
		size.z = m_size.z * 2.0f;
		size.w = m_size.w * 2.0f;
		return size;
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector AnimatedTexture::GetMiddleSize() const
	{
		math::Vector size;
		size.x = m_middleSize.x * 2.0f;
		size.y = m_middleSize.y * 2.0f;
		size.z = m_middleSize.z * 2.0f;
		size.w = m_middleSize.w * 2.0f;
		return size;
	}

} // sn

} // kge
