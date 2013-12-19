// File name: ParticleBehavior.cpp
// Des: 
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#include "../../Headers/ps/Particle.h"
#include "../../Headers/ps/ParticleBehavior.h"
#include "../../Include/math/Utility.h"

namespace kge
{

namespace ps
{
    //----------------------------------------
    // Applies forces to particles
    //----------------------------------------
    void ParticleBehavior::ApplyForces(Particles& particles, unsigned int activeCount, const std::vector<math::Vector>& forces)
    {
        const size_t numForces = forces.size();

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            for (size_t j = 0; j < numForces; ++j)
            {
                particles.accelerations[i] += forces[j] / particles.weights[i];
            }   
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void ParticleBehavior::ApplyGravity(Particles& particles, unsigned int activeCount, const math::Vector& gravity)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.accelerations[i] += gravity;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void SimpleBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            // velocity = previous velocity + acceleration * time
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            // position = previous position + velocity * time
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.ages[i] += elapsedTime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void FadingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;

            particles.ages[i] += elapsedTime;

            const float a = (1.0f - lifetime) * (1.0f - lifetime);
            const float b = 2.0f * (1.0f - lifetime) * lifetime;
            const float c = lifetime * lifetime;

            particles.colors[i].r = a * particles.initialColor.r + b * particles.middleColor.r + c * particles.finalColor.r;
            particles.colors[i].g = a * particles.initialColor.g + b * particles.middleColor.g + c * particles.finalColor.g;
            particles.colors[i].b = a * particles.initialColor.b + b * particles.middleColor.b + c * particles.finalColor.b;
            particles.colors[i].a = a * particles.initialColor.a + b * particles.middleColor.a + c * particles.finalColor.a;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void ScalingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;
            particles.ages[i] += elapsedTime;
            particles.extents[i] = particles.initialParticleSize + (particles.finalParticleSize - particles.initialParticleSize) * lifetime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void RotatingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.ages[i] += elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.rotations[i] += particles.angularVelocity * elapsedTime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void FadingScalingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;

            particles.ages[i] += elapsedTime;

            const float a = (1.0f - lifetime) * (1.0f - lifetime);
            const float b = 2.0f * (1.0f - lifetime) * lifetime;
            const float c = lifetime * lifetime;

            particles.colors[i].r = a * particles.initialColor.r + b * particles.middleColor.r + c * particles.finalColor.r;
            particles.colors[i].g = a * particles.initialColor.g + b * particles.middleColor.g + c * particles.finalColor.g;
            particles.colors[i].b = a * particles.initialColor.b + b * particles.middleColor.b + c * particles.finalColor.b;
            particles.colors[i].a = a * particles.initialColor.a + b * particles.middleColor.a + c * particles.finalColor.a;

            particles.extents[i] = particles.initialParticleSize + (particles.finalParticleSize - particles.initialParticleSize) * lifetime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void FadingRotatingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;

            particles.ages[i] += elapsedTime;

            const float a = (1.0f - lifetime) * (1.0f - lifetime);
            const float b = 2.0f * (1.0f - lifetime) * lifetime;
            const float c = lifetime * lifetime;

            particles.colors[i].r = a * particles.initialColor.r + b * particles.middleColor.r + c * particles.finalColor.r;
            particles.colors[i].g = a * particles.initialColor.g + b * particles.middleColor.g + c * particles.finalColor.g;
            particles.colors[i].b = a * particles.initialColor.b + b * particles.middleColor.b + c * particles.finalColor.b;
            particles.colors[i].a = a * particles.initialColor.a + b * particles.middleColor.a + c * particles.finalColor.a;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.rotations[i] += particles.angularVelocity * elapsedTime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void ScalingRotatingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;
            particles.ages[i] += elapsedTime;
            particles.extents[i] = particles.initialParticleSize + (particles.finalParticleSize - particles.initialParticleSize) * lifetime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.rotations[i] += particles.angularVelocity * elapsedTime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void FadingScalingRotatingBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = particles.ages[i] / particles.maxAge;

            particles.ages[i] += elapsedTime;

            const float a = (1.0f - lifetime) * (1.0f - lifetime);
            const float b = 2.0f * (1.0f - lifetime) * lifetime;
            const float c = lifetime * lifetime;

            particles.colors[i].r = a * particles.initialColor.r + b * particles.middleColor.r + c * particles.finalColor.r;
            particles.colors[i].g = a * particles.initialColor.g + b * particles.middleColor.g + c * particles.finalColor.g;
            particles.colors[i].b = a * particles.initialColor.b + b * particles.middleColor.b + c * particles.finalColor.b;
            particles.colors[i].a = a * particles.initialColor.a + b * particles.middleColor.a + c * particles.finalColor.a;

            particles.extents[i] = particles.initialParticleSize + (particles.finalParticleSize - particles.initialParticleSize) * lifetime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.rotations[i] += particles.angularVelocity * elapsedTime;
        }
    }

    //----------------------------------------
    //
    //----------------------------------------
    void ChaoticBehavior::Update(Particles& particles, unsigned int activeCount, float elapsedTime)
    {
        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.velocities[i] += math::RandomVector();
            particles.velocities[i] += particles.accelerations[i] * elapsedTime;
            particles.accelerations[i].Reset();
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            particles.positions[i] += particles.velocities[i] * elapsedTime;
        }

        for (unsigned int i = 0; i < activeCount; ++i)
        {
            const float lifetime = (particles.maxAge - particles.ages[i]) / particles.maxAge;
            particles.ages[i] += elapsedTime;
            particles.colors[i].a = particles.initialColor.a * lifetime;
        }
    }

} // ps

} // kge

