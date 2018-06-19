// File name: ParticleSystem.h
// Des: Particle system class, for producing effects like fire, smoke and sparks
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "SceneNode.h"
#include "../ps/Particle.h"

namespace kge
{

namespace ps
{
	class ParticleEmitter;
	class ParticleBehavior;
	class ParticleRenderer;
}

namespace gfx
{
	class HardwareBuffer;
}

namespace sn
{

struct RBuffer;

enum ParticleEmitterType
{
	EPET_POINT,
	EPET_SPHERE,
	EPET_BOX,
	EPET_CONE,
	EPET_2BOX,
	EPET_CIRCLE,
};

enum ParticleBehaviorType
{
	EPBT_SIMPLE,
	EPBT_FADING,
	EPBT_SCALING,
	EPBT_ROTATING,
	EPBT_FADING_SCALING,
	EPBT_FADING_ROTATING,
	EPBT_SACLING_ROTATING,
	EPBT_FADING_SCALING_ROTATING,
	EPBT_CHAOTIC,
};

enum ParticleRendererType
{
	EPRT_QUAD,
	EPRT_MESH,
};

struct ParticleSystemParameters
{
	ParticleEmitterType         emitterType;            //
	ParticleBehaviorType        behaviorType;           //
	ParticleRendererType        rendererType;           //
	const char*                 meshName;               //
	const char*                 textureName;            //
	const char*                 shaderName;             //
	bool                        additiveBlending;       //
	math::Vector                textureRect;            //
	unsigned int                horizontalImages;       //
	unsigned int                verticalImages;         //
	unsigned int                maxParticles;           //
	unsigned int                emission;               //
	float                       respawnRate;            //
	math::Vector                ranges;                 //
	math::Vector                direction;              //
	math::Vector                initialParticleSize;    //
	math::Vector                finalParticleSize;      //
	float                       speed;                  //
	float                       angularVelocity;        //
	bool                        hasColor;               //
	gfx::Colorf                 initialColor;           //
	gfx::Colorf                 finalColor;             //
	float                       minWeight;              //
	float                       maxWeight;              //
	float                       minAgeLowerBound;       //
	float                       minAgeUpperBound;       //
	float                       maxAge;                 //
	float                       emitterAngle;           //
	float                       emitterRadius;          //
	math::Vector                emitterDimensions;      //
	bool                        hasGravity;             //
	math::Vector                gravity;                //
	unsigned int                numForces;              //
	const math::Vector*         forces;                 //
	math::Vector                force;                  // We use one force until the editor can handle multiple forces properly.
	math::Vector                targetDimensions;       //
	float                       fTargetDistance;        //
	bool                        bRotateDirection;       //
	bool                        bRemoveOnInactive;      //
	gfx::Colorf                 middleColor;            //
	bool                        flush;                  //


	ParticleSystemParameters() :
		emitterType(EPET_SPHERE),
		behaviorType(EPBT_SIMPLE),
		rendererType(EPRT_QUAD),
		meshName(0),
		textureName(0),
		shaderName(0),
		additiveBlending(true),
		textureRect(0.0f, 0.0f, 1.0f, 1.0f),
		horizontalImages(1),
		verticalImages(1),
		maxParticles(100),
		emission(10),
		respawnRate(1.0f),
		ranges(10.0f, 10.0f, 10.0f),
		direction(0.0f, 1.0f, 0.0f),
		initialParticleSize(0.2f, 0.2f, 0.2f),
		finalParticleSize(0.2f, 0.2f, 0.2f),
		speed(1.0f),
		angularVelocity(0.0f),
		hasColor(true),
		initialColor(1.0f, 1.0f, 1.0f, 1.0f),
		finalColor(0.0f, 0.0f, 0.0f, 0.0f),
		minWeight(0.1f),
		maxWeight(0.2f),
		minAgeLowerBound(0.0f),
		minAgeUpperBound(5.0f),
		maxAge(10.0f),
		emitterAngle(math::PI / 6.0f),
		emitterRadius(0.5f),
		emitterDimensions(1.0f, 1.0f, 1.0f),
		hasGravity(false),
		gravity(0.0f, 0.0f, 0.0f),
		numForces(0),
		forces(0),
		force(0.0f, 0.0f, 0.0f),
		targetDimensions(1.0f, 1.0f, 1.0f),
		fTargetDistance(1.0f),
		bRotateDirection(true),
		bRemoveOnInactive(false),
		middleColor(0.5f, 0.5f, 0.5f, 0.5f),
		flush(false)
	{}
};

//!
class KGE_API ParticleSystem : public SceneNode
{
private:
	//! Copy constructor
	ParticleSystem(const ParticleSystem& other); // non copyable

	//! Assignment operator
	ParticleSystem& operator =(const ParticleSystem& other); // non copyable

public:
	//! Constructor
	ParticleSystem(const ParticleSystemParameters& p);

	//! Destructor
	~ParticleSystem();

	//! The jobs before rendering like culling and calculate position
	//! and update state of the particle system through time.
	void                PreRender(float elapsedTime);

	//! Render the objects
	void                Render();

	//! The jobs after rendering
	void                PostRender();

	/*! Draw the node with/without its Materials and Transforms.
	\param WithMaterial Draw it with its own materials or not?
	\param WithMaterial Draw it with its own Transformations or not?
	*/
	void                Draw( bool WithMaterial , bool WithTransform, bool bPosition /* = true */, bool bNormalTexcoord /* = true */, bool bTangentBinormal /* = true */ );

	//! Activate or deactivate system
	void                Activate(bool active);

	//!
	void                Emit(unsigned int num);

	//! Toggle system
	void                Toggle();

	//!
	bool                IsActive() const { return m_active; }

	//! Reset particle system
	void                Reset();

	//! 
	ParticleSystem&     SetTexture(const char* filename);

	//! 
	ParticleSystem&     SetShader(const char* filename);

	//! 
	//! rect.x = left, rect.y = top, rect.z = right, rect.w = bottom
	ParticleSystem&     SetTextureRect(const math::Vector& rect, unsigned int horizontalFrames, unsigned int verticalFrames);

	//!
	ParticleSystem&     SetMesh(const char* filename);

	//! maximum particles
	ParticleSystem&     SetMaxParticles(unsigned int num);

	//!
	void                SetSharedBuffer(gfx::HardwareBuffer* ib);

	//! emission rate per second
	ParticleSystem&     SetEmission(unsigned int num);

	//!
	ParticleSystem&     SetRespawnRate(float respawnRate);

	//! Ranges of particle system
	ParticleSystem&     SetRanges(const math::Vector& ranges);

	//! direction of emission
	ParticleSystem&     SetEmitterDirection(const math::Vector& direction);

	//! direction variation in radians
	//ParticleSystem&   SetDirectionChaos(float angle);

	//! initial speed of particles
	ParticleSystem&     SetSpeed(float speed);

	//! angular velocity of particles (radians in seconds)
	ParticleSystem&     SetRotationSpeed(float speed);

	//! size of particles
	ParticleSystem&     SetInitialParticleSize(const math::Vector& size);

	//! size of particles
	ParticleSystem&     SetFinalParticleSize(const math::Vector& size);

	//!
	ParticleSystem&     SetInitialColor(const gfx::Colorf& color);

	//!
	ParticleSystem&     SetMiddleColor(const gfx::Colorf& color);

	//!
	ParticleSystem&     SetFinalColor(const gfx::Colorf& color);

	//! minimum and maximum weight of particles
	ParticleSystem&     SetWeight(float minWeight, float maxWeight);

	//! range for minimum age of particles in seconds
	ParticleSystem&     SetMinAge(float lowerBound, float upperBound);

	//! maximum age of particles in seconds
	ParticleSystem&     SetMaxAge(float maxAge);

	//! angle of cone
	ParticleSystem&     SetEmitterAngle(float angle);

	//! radius of sphere
	ParticleSystem&     SetEmitterRadius(float radius);

	//! dimensions of box
	ParticleSystem&     SetEmitterDimensions(const math::Vector& size);

	//! dimensions of target box
	ParticleSystem&     SetTargetDimensions(const math::Vector& size);

	//! 
	ParticleSystem&     SetTargetDistance(float distance);

	//! 
	ParticleSystem&     AddForce(const math::Vector& force);

	//! 
	ParticleSystem&     SetForce(const math::Vector& force);

	//!
	//! Standard gravity = Vector(0.0f, -9.80665f, 0.0f)
	ParticleSystem&     SetGravity(const math::Vector& gravity);

	//!
	ParticleSystem&     SetRotatable(bool rotatable) { m_rotateDirection = rotatable; return *this; }

	//!
	ParticleSystem&     SetRemoveOnInactive(bool rem) { m_remove = rem;  return *this; }

	//!
	void                SetPos(float* x, float* y, float* z);

	//!
	void                SetFlush(bool flush) { m_flush = flush; }

	//!
	void                RemoveOnInactive(bool rem) { m_remove = rem; }

	//!
	void                EnableColor(bool hasColor);

	//!
	void                EnableAdditiveBlending(bool additiveBlending);

	//!
	void                EnableGravity(bool hasGravity) { m_hasGravity = hasGravity; }

	//!
	bool                HasColor() const;

	//!
	bool                HasAdditiveBlending() const;

	//!
	bool                HasGravity() const { return m_hasGravity; }

	//!
	ParticleEmitterType GetEmitterType() const { return m_emitterType; }

	//!
	ParticleBehaviorType GetBehaviorType() const { return m_behaviorType; }

	//!
	ParticleRendererType GetRendererType() const { return m_rendererType; }

	//!
	const char*         GetTextureName() const;

	//!
	const char*         GetShaderName() const;

	//!
	const char*         GetMeshName() const;

	//!
	math::Vector        GetTextureRect() const;

	//!
	unsigned int        GetHorizontalImages() const;

	//!
	unsigned int        GetVerticalImages() const;

	//!
	unsigned int        GetMaxParticles() const;

	//!
	unsigned int        GetEmission() const;

	//!
	float               GetRespawnRate() const;

	//!
	const math::Vector& GetRanges() const { return m_ranges; }

	//!
	math::Vector        GetEmitterDirection() const;

	//!
	float               GetSpeed() const;

	//!
	float               GetRotationSpeed() const { return m_particles.angularVelocity; }

	//!
	math::Vector        GetInitialParticleSize() const;

	//!
	math::Vector        GetFinalParticleSize() const;

	//!
	const gfx::Colorf&  GetInitialColor() const { return m_particles.initialColor; }

	//!
	const gfx::Colorf&  GetMiddleColor() const { return m_particles.middleColor; }

	//!
	const gfx::Colorf&  GetFinalColor() const { return m_particles.finalColor; }

	//!
	float               GetMinWeight() const;

	//!
	float               GetMaxWeight() const;

	//!
	float               GetMinAgeLB() const;

	//!
	float               GetMinAgeUB() const;

	//!
	float               GetMaxAge() const { return m_particles.maxAge; }

	//!
	float               GetEmitterAngle() const;

	//!
	float               GetEmitterRadius() const;

	//!
	math::Vector        GetEmitterDimensions() const;

	//!
	math::Vector        GetTargetDimensions() const;

	//!
	float               GetTargetDistance() const;

	//!
	size_t              GetForces(const math::Vector** forces) const;

	//!
	const math::Vector& GetForce() const { return m_forces[0]; }

	//!
	const math::Vector& GetGravity() const { return m_gravity; }

	//!
	bool                GetRotatable() const { return m_rotateDirection; }

	//!
	bool                GetRemoveOnInactive() const { return m_remove; }

	//!
	bool                GetFlush() const { return m_flush; }

private:
	ParticleEmitterType         m_emitterType;
	ParticleBehaviorType        m_behaviorType;
	ParticleRendererType        m_rendererType;

	std::string                 m_shaderName;
	std::string                 m_textureName;
	std::string                 m_meshName;

	ps::ParticleEmitter*        m_emitter;
	ps::ParticleBehavior*       m_behavior;
	ps::ParticleRenderer*       m_particleRenderer;
	math::Vector                m_ranges;
	std::vector<math::Vector>   m_forces;
	math::Vector                m_gravity;
	bool                        m_active;
	bool                        m_hasGravity;
	bool                        m_flush;
	bool                        m_rotateDirection;
	bool                        m_remove;
	unsigned int                m_immNum;
	bool                        m_flushFlag;

	// Particles Data
	ps::Particles	            m_particles;

	RBuffer*                    m_rb;
};

} // sn

} // kge

#endif // PARTICLESYSTEM_H
