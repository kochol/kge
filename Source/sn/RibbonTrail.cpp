// File name: RibbonTrail.cpp
// Des:
// Date: March 26, 2011
// Programmer: Nader Golbaz

#include "../../Include/sn/RibbonTrail.h"
#include "../../Include/sn/SceneManager.h"
#include "../../Include/gfx/Renderer.h"
#include "../../Include/gfx/HardwareBuffer.h"
#include "../../Include/math/Utility.h"

namespace kge
{

	namespace sn
	{
		//--------------------------
		//
		//--------------------------
		RibbonTrail::RibbonTrail(
			SceneManager* sceneMan,
			const char* shaderName,
			const char* textureName,
			const math::Vector& textureRect,
			const gfx::Colorf& initialColor,
			const gfx::Colorf& finalColor,
			float width,
			float speed,
			float age,
			float chaosRatio)
			: m_shaderName(shaderName),
			m_textureName(textureName),
			m_pMaterial(0),
			m_pRB(0),
			m_sizeOfData(0),
			m_bufferOffset(0),
			m_rect(textureRect),
			m_initialColor(initialColor),
			m_finalColor(finalColor),
			m_width(width),
			m_speed(speed),
			m_totalAge(age),
			m_currentAge(0.0f),
			m_chaosRatio(chaosRatio),
			m_elapsed(0.0f),
			m_running(false),
			m_maxCount(100),
			m_activeCount(0),
			m_positions(0),
			m_colors(0),
			m_index(0),
			m_bolt(false)
		{
			m_eNodeType = sn::ENT_RibbonTrail;
			m_pSnMan = sceneMan;
			m_pRenderer = m_pSnMan->GetRenderer();
			m_pMaterial = KGE_NEW(gfx::Material);
			m_pAABB = KGE_NEW(math::AABB);

			m_pMaterial->ppTexture[0] = m_pSnMan->AddTexture(textureName);
			m_pMaterial->shader->m_pVertexShader = m_pSnMan->AddVertexShader(shaderName, "vs", gfx::ESV_VS2);
			m_pMaterial->shader->m_pPixelShader = m_pSnMan->AddPixelShader(shaderName, "ps", gfx::ESV_PS2);
			m_pRB = m_pSnMan->GetRBuffer(m_maxCount * 6);

			m_positions = new math::Vector[m_maxCount];
			m_colors = new gfx::Color[m_maxCount];

			//for (unsigned int i = 0; i < m_maxCount; ++i)
			//{
			//	const float t = static_cast<float>(i) / (m_maxCount - 1);
			//	m_colors[i].r = m_initialColor.r + t * (m_finalColor.r - m_initialColor.r);
			//	m_colors[i].g = m_initialColor.g + t * (m_finalColor.g - m_initialColor.g);
			//	m_colors[i].b = m_initialColor.b + t * (m_finalColor.b - m_initialColor.b);
			//	m_colors[i].a = m_initialColor.a + t * (m_finalColor.a - m_initialColor.a);
			//}

			m_pMaterial->Alpha = true;
			m_pMaterial->Culling = false;
		}

		//--------------------------
		//
		//--------------------------
		RibbonTrail::~RibbonTrail()
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

			m_pSnMan->FreeRBuffer(m_pRB);

			delete[] m_positions;
			delete[] m_colors;

			delete m_pAABB;
			delete m_pMaterial;
		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::PreRender(float elapsedTime)
		{
			if (!m_running) { return; }

			if (m_pParent)
			{
				//*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
				*m_pFinalMat = *m_pAbsMat;
				m_pFinalMat->_41 += m_pParent->GetFinalMatrix()->_41;
				m_pFinalMat->_42 += m_pParent->GetFinalMatrix()->_42;
				m_pFinalMat->_43 += m_pParent->GetFinalMatrix()->_43;
			}
			else
			{
				*m_pFinalMat = *m_pAbsMat;
			}

			if (!m_bolt)
			{
				//////////////////////////////////////////////////////////////////////////
				m_currentAge += elapsedTime;

				math::Vector curPos(m_pFinalMat->_41, m_pFinalMat->_42, m_pFinalMat->_43);
				math::Vector dir = curPos - m_prevPos;

				if (dir.GetSqrLength() > 0.1f/*0.0025f*/)
				{
					bool removeLast = false;

					m_elapsed += elapsedTime;

					if (m_elapsed > m_totalAge)
					{
						removeLast = true;
						m_elapsed = 0.0f;
					}

					if (m_activeCount < m_maxCount)
					{
						m_positions[m_activeCount] = m_prevPos + (dir * 0.5f);
						++m_activeCount;
					}
					else
					{
						m_positions[m_activeCount] = m_prevPos + (dir * 0.5f);
						removeLast = true;
					}

					if (removeLast)
					{
						for (unsigned int i = 1; i < m_activeCount; ++i)
						{
							m_positions[i - 1] = m_positions[i];
							m_colors[i - 1] = m_colors[i];
						}
					}

					m_prevPos = curPos;
				}
				else if (m_activeCount)
				{
					m_elapsed += elapsedTime;

					if (m_elapsed > m_totalAge)
					{
						for (unsigned int i = 1; i < m_activeCount; ++i)
						{
							m_positions[i - 1] = m_positions[i];
							m_colors[i - 1] = m_colors[i];
						}

						--m_activeCount;
						m_elapsed = 0.0f;
					}
				}

				for (unsigned int i = 0; i < m_activeCount; ++i)
				{
					const float t = static_cast<float>((m_activeCount - 1) - i) / (m_activeCount - 1);
					int r = (m_initialColor.r + t * (m_finalColor.r - m_initialColor.r)) * 255.0f;
					int g = (m_initialColor.g + t * (m_finalColor.g - m_initialColor.g)) * 255.0f;
					int b = (m_initialColor.b + t * (m_finalColor.b - m_initialColor.b)) * 255.0f;
					int a = (m_initialColor.a + t * (m_finalColor.a - m_initialColor.a)) * 255.0f;
					m_colors[i].Set(r, g, b, a);
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				if (m_currentAge >= m_totalAge)
				{
					m_running = false;
					return;
				}

				m_elapsed += elapsedTime;

				float t = m_elapsed * m_speed;

				if (t >= 1.0f)
				{
					t = 1.0f;
					m_currentAge += elapsedTime;
					if (m_currentAge > m_totalAge) { m_currentAge = m_totalAge; }
				}

				m_activeCount = (m_maxCount - 1) * t;

				for (unsigned int i = 0; i < m_activeCount; ++i)
				{
					const float t = (static_cast<float>((m_activeCount - 1) - i) / (m_activeCount - 1)) *
						(m_currentAge / m_totalAge);
					int r = (m_initialColor.r + t * (m_finalColor.r - m_initialColor.r)) * 255.0f;
					int g = (m_initialColor.g + t * (m_finalColor.g - m_initialColor.g)) * 255.0f;
					int b = (m_initialColor.b + t * (m_finalColor.b - m_initialColor.b)) * 255.0f;
					int a = (m_initialColor.a + t * (m_finalColor.a - m_initialColor.a)) * 255.0f;
					m_colors[i].Set(r, g, b, a);
				}
			}

			if (!m_bVis || (m_bAutoCulling && (m_pSnMan->IsVisible(this) != math::ECT_In)))
			{
				return;
			}

			const math::Matrix view = m_pRenderer->GetTransForm(gfx::ETM_View);
			//math::Vector rightVec(view._11, view._21, view._31);
			math::Vector upVec(view._12, view._22, view._32);

			//rightVec.Normalize();
			upVec.Normalize();
			upVec = upVec * m_width * 0.5f;
			math::Vector up(upVec);

			gfx::Vertex3CT* v = 0;
			kge::ul32 flag = 0x00001000L;
			m_sizeOfData = m_activeCount * 6 * sizeof(gfx::Vertex3CT);

			if (m_bufferOffset + m_sizeOfData > m_pRB->size * sizeof(gfx::Vertex3CT))
			{
				m_bufferOffset = 0;
				flag = 0x00002000L;
			}

			if (!(m_pRB->hb->Lock(m_bufferOffset, m_sizeOfData, reinterpret_cast<void**>(&v), flag)))
			{
				return;
			}

			for (unsigned int i = 0; i < m_activeCount; ++i)
			{
				math::Vector vec3(m_positions[i] - up);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i];
				v->tex.X = m_rect.x;
				v->tex.Y = m_rect.w;

				++v;

				vec3 = math::Vector(m_positions[i] + up);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i];
				v->tex.X = m_rect.x;
				v->tex.Y = m_rect.y;

				++v;

				const math::Vector tempUp = upVec * (static_cast<float>(m_activeCount - (i + 1)) / m_activeCount);

				vec3 = math::Vector(m_positions[i + 1] + tempUp);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i + 1];
				v->tex.X = m_rect.z;
				v->tex.Y = m_rect.y;

				++v;

				vec3 = math::Vector(m_positions[i + 1] - tempUp);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i + 1];
				v->tex.X = m_rect.z;
				v->tex.Y = m_rect.w;

				++v;

				vec3 = math::Vector(m_positions[i] - up);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i];
				v->tex.X = m_rect.x;
				v->tex.Y = m_rect.w;

				++v;

				vec3 = math::Vector(m_positions[i + 1] + tempUp);

				v->pos.X = vec3.x;
				v->pos.Y = vec3.y;
				v->pos.Z = vec3.z;
				v->col = m_colors[i + 1];
				v->tex.X = m_rect.z;
				v->tex.Y = m_rect.y;

				++v;

				up = tempUp;
			}

			m_pRB->hb->Unlock();

			m_pSnMan->RegNode(this, ERO_Last);
		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::Render()
		{
			math::Matrix mat = m_pRenderer->GetTransForm(gfx::ETM_Projection);
			mat *= m_pRenderer->GetTransForm(gfx::ETM_View);
			math::Transpose(mat);
			m_pMaterial->shader->m_pVertexShader->SetConstant(&mat._11, 0, 4);
			m_pRenderer->SetMaterial(m_pMaterial);
			m_pRenderer->SetVertexBuffer(m_pRB->hb);
			m_pRenderer->DrawTriangleList(m_activeCount * 6, 0, m_pRenderer->GetVertexDec(gfx::EVT_V3CT),
				m_bufferOffset / sizeof(gfx::Vertex3CT), 0);
			m_bufferOffset += m_sizeOfData;
		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::PostRender()
		{

		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::Draw(bool WithMaterial , bool WithTransform , bool bPosition , bool bNormalTexcoord , bool bTangentBinormal)
		{

		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::Start(const math::Vector& target)
		{
			if (m_running) { return; }

			m_bolt = true;
			m_activeCount = 0;
			m_elapsed = 0.0f;
			m_currentAge = 0.0f;
			m_running = true;

			if (m_pParent)
			{
				*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
			}
			else
			{
				*m_pFinalMat = *m_pAbsMat;
			}

			const math::Vector startPoint(m_pFinalMat->_41, m_pFinalMat->_42, m_pFinalMat->_43);

			m_pAABB->Reset();
			//m_pAABB->AddInternalPoint(math::Vector(0.0f, 0.0f, 0.0f));
			m_pAABB->AddInternalPoint(target - startPoint);
			m_pAABB->AddInternalPoint(startPoint - target);

			m_positions[0] = startPoint;

			const int numControlPoints = 5;
			math::Vector controlPoints[numControlPoints];
			math::Vector dir = target - startPoint;
			const float length = dir.GetLength();
			dir.Normalize();
			float delta = 1.0f / numControlPoints;
			float progress = 0.0f;

			controlPoints[0] = startPoint;
			for (unsigned int i = 1; i < numControlPoints - 1; ++i)
			{
				progress += delta;
				controlPoints[i] = startPoint + dir * (length * progress) + math::RandomVector() * m_chaosRatio;
			}
			controlPoints[numControlPoints - 1] = target;

			delta = 1.0f / m_maxCount;
			progress = 0.0f;

			for (unsigned int i = 1; i < m_maxCount - 1; ++i)
			{
				progress += delta;
				BezierCurve(m_positions[i], controlPoints, numControlPoints, progress);
			}

			m_positions[m_maxCount - 1] = target;
		}

		//--------------------------
		//
		//--------------------------
		void RibbonTrail::Start()
		{
			m_bolt = false;
			m_elapsed = 0.0f;
			m_currentAge = 0.0f;
			m_activeCount = 0;

			if (m_pParent)
			{
				*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
			}
			else
			{
				*m_pFinalMat = *m_pAbsMat;
			}

			m_prevPos.x = m_pFinalMat->_41;
			m_prevPos.y = m_pFinalMat->_42;
			m_prevPos.z = m_pFinalMat->_43;

			m_pAABB->Reset();
			m_pAABB->AddInternalPoint(math::Vector(5.0f, 5.0f, 5.0f));
			m_pAABB->AddInternalPoint(math::Vector(-5.0f, -5.0f, -5.0f));

			m_positions[0] = m_prevPos;

			m_running = true;
		}

	} // sn

} // kge
