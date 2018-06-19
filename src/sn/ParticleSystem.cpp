// File name: ParticleSystem.cpp
// Des: Particle system class, for producing effects like fire, smoke and sparks
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#include "../../Headers/ps/Particle.h"
#include "../../Headers/ps/ParticleEmitter.h"
#include "../../Headers/ps/ParticleBehavior.h"
#include "../../Headers/ps/ParticleRenderer.h"
#include "../../include/kge/sn/SceneManager.h"
#include "../../include/kge/sn/ParticleSystem.h"
#include "../../include/kge/sn/StaticMesh.h"
#include "../../include/kge/gfx/Renderer.h"

namespace kge
{

namespace sn
{

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem::ParticleSystem(const ParticleSystemParameters& p)
		: m_emitterType(p.emitterType), m_behaviorType(p.behaviorType),
		m_rendererType(p.rendererType), m_emitter(NULL), m_behavior(NULL),
		m_particleRenderer(NULL), m_active(false), m_ranges(p.ranges),
		m_hasGravity(p.hasGravity), m_gravity(p.gravity), m_flush(p.flush),
		m_rotateDirection(p.bRotateDirection), m_remove(p.bRemoveOnInactive),
		m_immNum(0), m_flushFlag(false), m_rb(0)
	{
		m_eNodeType = ENT_ParticleSystem;

		//
		m_forces.push_back(p.force);
		//

		for (unsigned int i = 0; i < p.numForces; ++i)
		{
			m_forces.push_back(p.forces[i]);
		}

		m_particles.positions = 0;
		//m_particles.oldPositions = 0;
		m_particles.velocities = 0;
		m_particles.accelerations = 0;
		m_particles.colors = 0;
		m_particles.extents = 0;
		m_particles.rotations = 0;
		m_particles.weights = 0;
		m_particles.ages = 0;
		m_particles.imageFrames = 0;
		m_particles.initialParticleSize = p.initialParticleSize * 0.5f;
		m_particles.finalParticleSize = p.finalParticleSize * 0.5f;
		m_particles.initialColor = p.initialColor;
		m_particles.middleColor = p.middleColor;
		m_particles.finalColor = p.finalColor;
		m_particles.maxAge = p.maxAge;
		m_particles.angularVelocity = p.angularVelocity;

		// ParticleEmitter

		if (EPET_POINT == m_emitterType)
		{
			m_emitter = new ps::PointEmitter(p.maxParticles, p.emission, p.speed, p.hasColor,
				p.minWeight, p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound,
				p.respawnRate, p.horizontalImages * p.verticalImages, p.direction);
		}
		else if (EPET_SPHERE == m_emitterType)
		{
			m_emitter = new ps::SphereEmitter(p.maxParticles, p.emission, p.speed, p.hasColor,
				p.minWeight, p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound,
				p.respawnRate, p.horizontalImages * p.verticalImages, p.emitterRadius);
		}
		else if (EPET_BOX == m_emitterType)
		{
			m_emitter = new ps::BoxEmitter(p.maxParticles, p.emission, p.speed, p.hasColor,
				p.minWeight, p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound, p.respawnRate,
				p.horizontalImages * p.verticalImages, p.direction, p.emitterDimensions * 0.5f);
		}
		else if (EPET_CONE == m_emitterType)
		{
			m_emitter = new ps::ConeEmitter(p.maxParticles, p.emission, p.speed, p.hasColor, p.minWeight,
				p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound, p.respawnRate,
				p.horizontalImages * p.verticalImages, p.direction, p.emitterAngle);
		}
		else if (EPET_2BOX == m_emitterType)
		{
			m_emitter = new ps::TwoBoxEmitter(p.maxParticles, p.emission, p.speed, p.hasColor,
				p.minWeight, p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound, p.respawnRate,
				p.horizontalImages * p.verticalImages, p.direction, p.emitterDimensions * 0.5f, p.targetDimensions * 0.5f, p.fTargetDistance);
		}
		else if (EPET_CIRCLE == m_emitterType)
		{
			m_emitter = new ps::CircleEmitter(p.maxParticles, p.emission, p.speed, p.hasColor,
				p.minWeight, p.maxWeight, p.minAgeLowerBound, p.minAgeUpperBound,
				p.respawnRate, p.horizontalImages * p.verticalImages, p.direction, p.emitterRadius);
		}

		// ParticleBehavbior

		if (EPBT_SIMPLE == m_behaviorType)
		{
			m_behavior = new ps::SimpleBehavior;
		}
		else if (EPBT_FADING == m_behaviorType)
		{
			m_behavior = new ps::FadingBehavior;
		}
		else if (EPBT_SCALING == m_behaviorType)
		{
			m_behavior = new ps::ScalingBehavior;
		}
		else if (EPBT_ROTATING == m_behaviorType)
		{
			m_behavior = new ps::RotatingBehavior;
		}
		else if (EPBT_FADING_SCALING == m_behaviorType)
		{
			m_behavior = new ps::FadingScalingBehavior;
		}
		else if (EPBT_FADING_ROTATING == m_behaviorType)
		{
			m_behavior = new ps::FadingRotatingBehavior;
		}
		else if (EPBT_SACLING_ROTATING == m_behaviorType)
		{
			m_behavior = new ps::ScalingRotatingBehavior;
		}
		else if (EPBT_FADING_SCALING_ROTATING == m_behaviorType)
		{
			m_behavior = new ps::FadingScalingRotatingBehavior;
		}
		else if (EPBT_CHAOTIC == m_behaviorType)
		{
			m_behavior = new ps::ChaoticBehavior;
		}

		// ParticleRenderer

		if (EPRT_QUAD == m_rendererType)
		{
			if (EPBT_ROTATING == m_behaviorType || EPBT_FADING_ROTATING == m_behaviorType ||
				EPBT_SACLING_ROTATING == m_behaviorType || EPBT_FADING_SCALING_ROTATING == m_behaviorType)
			{
				m_particleRenderer = new ps::RotatingQuadRenderer(p.textureRect, p.horizontalImages, p.verticalImages, p.additiveBlending);
			}
			else
			{
				if (EPET_CIRCLE == m_emitterType)
				{
					m_particleRenderer = new ps::QuadRenderer2(p.textureRect, p.horizontalImages, p.verticalImages, p.additiveBlending);
				}
				else
				{
					m_particleRenderer = new ps::QuadRenderer(p.textureRect, p.horizontalImages, p.verticalImages, p.additiveBlending);
				}
			}
		}
		else if (EPRT_MESH == m_rendererType)
		{
			//m_particleRenderer = new ps::MeshRenderer;
		}

		m_pAABB = KGE_NEW(math::AABB);

		m_pAABB->AddInternalPoint(m_ranges);
		math::Vector r = m_ranges;
		r.Negate();
		m_pAABB->AddInternalPoint(r);
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem::~ParticleSystem()
	{
		delete m_pAABB;
		delete m_emitter;
		delete m_behavior;
		delete m_particleRenderer;

		m_pSnMan->FreeRBuffer(m_rb);

		delete[] m_particles.positions;
		//delete[] m_particles.oldPositions;
		delete[] m_particles.velocities;
		delete[] m_particles.accelerations;
		delete[] m_particles.colors;
		delete[] m_particles.extents;
		delete[] m_particles.rotations;
		delete[] m_particles.weights;
		delete[] m_particles.ages;
		delete[] m_particles.imageFrames;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::PreRender(float elapsedTime)
	{
		if (!m_active)
		{
			if (m_remove) { m_pSnMan->RemoveSceneNode(this); }
			return;
		}

		if (m_pParent)
		{
			*m_pFinalMat = *m_pParent->GetFinalMatrix() * (*m_pAbsMat);
		}
		else
		{
			*m_pFinalMat = *m_pAbsMat;
		}

		const math::Vector location(m_pFinalMat->_41, m_pFinalMat->_42, m_pFinalMat->_43);

		if (m_rotateDirection)
		{
			math::Vector dir = m_emitter->GetDirection();
			math::Vector temp = dir;
			m_pFinalMat->RotateVec(dir);
			m_emitter->SetDirection(dir);
			m_emitter->Emit(m_particles, location, elapsedTime, m_flushFlag);
			if (m_immNum)
			{
				m_emitter->Emit(m_particles, location, m_immNum);
				m_immNum = 0;
			}
			m_emitter->SetDirection(temp);
		}
		else
		{
			m_emitter->Emit(m_particles, location, elapsedTime, m_flushFlag);

			if (m_immNum)
			{
				m_emitter->Emit(m_particles, location, m_immNum);
				m_immNum = 0;
			}
		}

		if (!m_forces.empty())
		{
			m_behavior->ApplyForces(m_particles, m_emitter->ActiveCount(), m_forces);
		}

		if (m_hasGravity)
		{
			m_behavior->ApplyGravity(m_particles, m_emitter->ActiveCount(), m_gravity);
		}

		m_behavior->Update(m_particles, m_emitter->ActiveCount(), elapsedTime);

		m_emitter->Destroy(m_particles, *m_pAABB, location);

		m_active = m_emitter->ActiveCount() || m_emitter->GetCurrentEmission();

		if (m_bVis)
		{
			if (!m_bAutoCulling || (m_pSnMan->IsVisible(this) == math::ECT_In))
			{
				m_pSnMan->RegNode(this, ERO_Last);
			}
		}
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Render()
	{
		//m_pRenderer->SetTransForm(m_pFinalMat);
		m_particleRenderer->Render(m_particles, m_emitter->ActiveCount(), m_pRenderer);
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::PostRender()
	{

	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Draw( bool WithMaterial , bool WithTransform, bool bPosition , bool bNormalTexcoord , bool bTangentBinormal )
	{

	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Activate(bool active)
	{
		m_active = active;
		m_emitter->Reset();
		if (m_flush) { m_flushFlag = true; }
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Emit(unsigned int num)
	{
		m_active = true;
		m_immNum = num;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Toggle()
	{
		m_active = !m_active;
		m_emitter->Reset();
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::Reset()
	{
		m_emitter->Clear();
		m_emitter->Reset();
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetTexture(const char* filename)
	{
		m_textureName = filename;

		m_particleRenderer->SetTexture(m_pSnMan->AddTexture(filename));

		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetShader(const char* filename)
	{
		m_shaderName = filename;

		//m_particleRenderer->SetShader(
		//    m_pRenderer->CreateVertexShaderFromFile(filename, "vs", gfx::ESV_VS2),
		//    m_pRenderer->CreatePixelShaderFromFile(filename, "ps", gfx::ESV_PS2));

		m_particleRenderer->SetShader(
			m_pSnMan->AddVertexShader(filename, "vs", gfx::ESV_VS2),
			m_pSnMan->AddPixelShader(filename, "ps", gfx::ESV_PS2));

		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetTextureRect(const math::Vector &rect, unsigned int horizontalFrames, unsigned int verticalFrames)
	{
		m_particleRenderer->SetTextureRect(rect, horizontalFrames, verticalFrames);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetMesh(const char* filename)
	{
		m_meshName = filename;

		//StaticMesh* mesh = m_pSnMan->AddStaticMeshNode(filename);

		//if (mesh)
		//{
		//    mesh->SetParent(this);
		//    m_particleRenderer->SetMesh(mesh);
		//}

		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetMaxParticles(unsigned int num)
	{
		if (m_particles.positions && m_particles.ages)
		{
			if (num == m_emitter->GetMaxParticles())
			{
				return *this;
			}
			else if (num < m_emitter->GetMaxParticles())
			{
				m_emitter->SetMaxParticles(num);
				return *this;
			}

			const unsigned int count = m_emitter->ActiveCount();

			math::Vector* tempPositions = new math::Vector[num];
			std::copy(m_particles.positions, m_particles.positions + count, tempPositions);
			delete[] m_particles.positions;
			m_particles.positions = tempPositions;

			//math::Vector* tempOldPositions = new math::Vector[num];
			//std::copy(m_particles.oldPositions, m_particles.oldPositions + count, tempOldPositions);
			//delete[] m_particles.oldPositions;
			//m_particles.oldPositions = tempOldPositions;

			math::Vector* tempVelocities = new math::Vector[num];
			std::copy(m_particles.velocities, m_particles.velocities + count, tempVelocities);
			delete[] m_particles.velocities;
			m_particles.velocities = tempVelocities;

			math::Vector* tempAccelerations = new math::Vector[num];
			std::copy(m_particles.accelerations, m_particles.accelerations + count, tempAccelerations);
			delete[] m_particles.accelerations;
			m_particles.accelerations = tempAccelerations;

			math::Vector* tempExtents = new math::Vector[num];
			std::copy(m_particles.extents, m_particles.extents + count, tempExtents);
			delete[] m_particles.extents;
			m_particles.extents = tempExtents;

			gfx::Colorf* tempColors = new gfx::Colorf[num];
			std::copy(m_particles.colors, m_particles.colors + count, tempColors);
			delete[] m_particles.colors;
			m_particles.colors = tempColors;

			float* tempWeights = new float[num];
			std::copy(m_particles.weights, m_particles.weights + count, tempWeights);
			delete[] m_particles.weights;
			m_particles.weights = tempWeights;

			float* tempAges = new float[num];
			std::copy(m_particles.ages, m_particles.ages + count, tempAges);
			delete[] m_particles.ages;
			m_particles.ages = tempAges;

			float* tempRotations = new float[num];
			std::copy(m_particles.rotations, m_particles.rotations + count, tempRotations);
			delete[] m_particles.rotations;
			m_particles.rotations = tempRotations;

			if (EPRT_QUAD == m_rendererType)
			{
				unsigned int* tempImageFrames = new unsigned int[num];
				std::copy(m_particles.imageFrames, m_particles.imageFrames + count, tempImageFrames);
				delete[] m_particles.imageFrames;
				m_particles.imageFrames = tempImageFrames;
			}
		}
		else
		{
			delete[] m_particles.positions;
			//delete[] m_particles.oldPositions;
			delete[] m_particles.velocities;
			delete[] m_particles.accelerations;
			delete[] m_particles.colors;
			delete[] m_particles.extents;
			delete[] m_particles.rotations;
			delete[] m_particles.weights;
			delete[] m_particles.ages;
			delete[] m_particles.imageFrames;

			m_particles.positions = new math::Vector[num];
			//m_particles.oldPositions = new math::Vector[num];
			m_particles.velocities = new math::Vector[num];
			m_particles.accelerations = new math::Vector[num];
			m_particles.extents = new math::Vector[num];
			m_particles.colors = new gfx::Colorf[num];
			m_particles.weights = new float[num];
			m_particles.ages = new float[num];
			m_particles.rotations = new float[num];

			if (EPRT_QUAD == m_rendererType)
			{
				m_particles.imageFrames = new unsigned int[num];
			}
		}

		m_emitter->SetMaxParticles(num);

		if (EPRT_QUAD == m_rendererType)
		{
			//m_particleRenderer->SetBuffer(m_pRenderer->CreateVertexBuffer(NULL, num * 4, gfx::EVT_V3CT, true));

			m_pSnMan->FreeRBuffer(m_rb);
			m_rb = m_pSnMan->GetRBuffer(num * 4);
			m_particleRenderer->SetBuffer(m_rb->hb);

			gfx::HardwareBuffer* ib = m_pSnMan->GetParticlesSharedBuffer();

			if (ib->GetDataCount() < num * 6)
			{
				delete ib;

				u16* indices = new u16[num * 6];
				u16 c = 0;

				for (unsigned int j = 0; j < num; ++j)
				{
					const unsigned int k = j * 6;
					indices[k] = c;
					indices[k+1] = c + 1;
					indices[k+2] = c + 2;
					indices[k+3] = c + 3;
					indices[k+4] = c;
					indices[k+5] = c + 2;
					c += 4;
				}

				ib = m_pRenderer->CreateIndexBuffer(indices, num * 6, gfx::EIBT_16Bit, false);

				delete[] indices;

				m_pSnMan->SetParticlesSharedBuffer(ib);
			}
			else
			{
				m_particleRenderer->SetSharedBuffer(ib);
			}
		}

		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::SetSharedBuffer(gfx::HardwareBuffer* ib)
	{
		m_particleRenderer->SetSharedBuffer(ib);
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetEmission(unsigned int num)
	{
		m_emitter->SetEmission(num);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetRespawnRate(float respawnRate)
	{
		if (respawnRate > 1.0f)
		{
			respawnRate = 1.0f;
		}

		m_emitter->SetRespawnRate(respawnRate);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetRanges(const math::Vector& ranges)
	{
		m_ranges = ranges;
		m_pAABB->Reset();
		m_pAABB->AddInternalPoint(m_ranges);
		math::Vector r = m_ranges;
		r.Negate();
		m_pAABB->AddInternalPoint(r);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetEmitterDirection(const math::Vector& direction)
	{
		m_emitter->SetDirection(direction);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetSpeed(float speed)
	{
		m_emitter->SetSpeed(speed);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetRotationSpeed(float speed)
	{
		m_particles.angularVelocity = speed;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetInitialParticleSize(const math::Vector& size)
	{
		m_particles.initialParticleSize = size * 0.5f;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetFinalParticleSize(const math::Vector& size)
	{
		m_particles.finalParticleSize = size * 0.5f;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetInitialColor(const gfx::Colorf& color)
	{
		m_particles.initialColor = color;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetMiddleColor(const gfx::Colorf& color)
	{
		m_particles.middleColor = color;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetFinalColor(const gfx::Colorf& color)
	{
		m_particles.finalColor = color;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetWeight(float minWeight, float maxWeight)
	{
		m_emitter->SetWeight(minWeight, maxWeight);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetMinAge(float lowerBound, float upperBound)
	{
		m_emitter->SetMinAge(lowerBound, upperBound);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetMaxAge(float maxAge)
	{
		m_particles.maxAge = maxAge;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetEmitterAngle(float angle)
	{
		m_emitter->SetAngle(angle);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetEmitterRadius(float radius)
	{
		m_emitter->SetRadius(radius);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetEmitterDimensions(const math::Vector& size)
	{
		m_emitter->SetSize(size * 0.5f);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetTargetDimensions(const math::Vector& size)
	{
		m_emitter->SetTargetSize(size * 0.5f);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetTargetDistance(float distance)
	{
		m_emitter->SetTargetDistance(distance);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::SetPos(float* x, float* y, float* z)
	{
		SceneNode::SetPosition(kge::math::Vector(*x, *y, *z));
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::AddForce(const math::Vector& force)
	{
		m_forces.push_back(force);
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetForce(const math::Vector& force)
	{
		m_forces[0] = force;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	ParticleSystem& ParticleSystem::SetGravity(const math::Vector& gravity)
	{
		m_gravity = gravity;
		return *this;
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::EnableColor(bool hasColor)
	{
		m_emitter->EnableColor(hasColor);
	}

	//----------------------------------------
	//
	//----------------------------------------
	void ParticleSystem::EnableAdditiveBlending(bool additiveBlending)
	{
		m_particleRenderer->EnableAdditiveBlending(additiveBlending);
	}

	//----------------------------------------
	//
	//----------------------------------------
	bool ParticleSystem::HasColor() const
	{
		return m_emitter->HasColor();
	}

	//----------------------------------------
	//
	//----------------------------------------
	bool ParticleSystem::HasAdditiveBlending() const
	{
		return m_particleRenderer->HasAdditiveBlending();
	}

	//----------------------------------------
	//
	//----------------------------------------
	const char* ParticleSystem::GetTextureName() const
	{
		return m_textureName.c_str();
	}

	//----------------------------------------
	//
	//----------------------------------------
	const char* ParticleSystem::GetShaderName() const
	{
		return m_shaderName.c_str();
	}

	//----------------------------------------
	//
	//----------------------------------------
	const char* ParticleSystem::GetMeshName() const
	{
		return m_meshName.c_str();
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetTextureRect() const
	{
		return m_particleRenderer->GetTextureRect();
	}

	//----------------------------------------
	//
	//----------------------------------------
	unsigned int ParticleSystem::GetHorizontalImages() const
	{
		return m_particleRenderer->GetHorizontalFrames();
	}

	//----------------------------------------
	//
	//----------------------------------------
	unsigned int ParticleSystem::GetVerticalImages() const
	{
		return m_particleRenderer->GetVerticalFrames();
	}

	//----------------------------------------
	//
	//----------------------------------------
	unsigned int ParticleSystem::GetMaxParticles() const
	{
		return m_emitter->GetMaxParticles();
	}

	//----------------------------------------
	//
	//----------------------------------------
	unsigned int ParticleSystem::GetEmission() const
	{
		return m_emitter->GetEmission();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetRespawnRate() const
	{
		return m_emitter->GetRespawnRate();
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetEmitterDirection() const
	{
		return m_emitter->GetDirection();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetSpeed() const
	{
		return m_emitter->GetSpeed();
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetInitialParticleSize() const
	{
		return m_particles.initialParticleSize * 2.0f;
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetFinalParticleSize() const
	{
		return m_particles.finalParticleSize * 2.0f;
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetMinWeight() const
	{
		return m_emitter->GetMinWeight();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetMaxWeight() const
	{
		return m_emitter->GetMaxWeight();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetMinAgeLB() const
	{
		return m_emitter->GetMinAgeLB();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetMinAgeUB() const
	{
		return m_emitter->GetMinAgeUB();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetEmitterAngle() const
	{
		return m_emitter->GetAngle();
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetEmitterRadius() const
	{
		return m_emitter->GetRadius();
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetEmitterDimensions() const
	{
		return m_emitter->GetSize() * 2.0f;
	}

	//----------------------------------------
	//
	//----------------------------------------
	math::Vector ParticleSystem::GetTargetDimensions() const
	{
		return m_emitter->GetTargetSize() * 2.0f;
	}

	//----------------------------------------
	//
	//----------------------------------------
	float ParticleSystem::GetTargetDistance() const
	{
		return m_emitter->GetTargetDistance();
	}

	//----------------------------------------
	//
	//----------------------------------------
	size_t ParticleSystem::GetForces(const math::Vector** forces) const
	{
		*forces = &m_forces[0];
		return m_forces.size();
	}

} // sn

} // kge

