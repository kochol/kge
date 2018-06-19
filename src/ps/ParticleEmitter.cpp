// File name: ParticleEmitter.cpp
// Des: 
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#include "../../Headers/ps/Particle.h"
#include "../../Headers/ps/ParticleEmitter.h"
#include "../../include/kge/math/Utility.h"
#include <cstdlib>

namespace kge
{

namespace ps
{

    //----------------------------------------
    //
    //----------------------------------------
    ParticleEmitter::ParticleEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount)

        : m_maxCount(maxCount), m_emission0(emission), m_emission1(emission),
        m_speed(speed), m_hasColor(hasColor),
        m_minWeight(minWeight), m_maxWeight(maxWeight),
        m_minAgeLB(minAgeLB), m_minAgeUB(minAgeUB),
        m_activeCount(0), m_interval(0.0f), m_ppi(1), m_delta(0.0f),
        m_respawnRate(respawnRate), m_ep(0), m_imageCount(imageCount)
    {
        SetEmission(emission);
    }

    //----------------------------------------
    //
    //----------------------------------------
    void ParticleEmitter::Emit(Particles& particles, const math::Vector& pos, float elapsedTime, bool& flush)
    {
        if (!flush)
        {
            m_delta += elapsedTime;

            if (m_delta >= m_interval)
            {
                m_ep += this->Emit(particles, pos, m_ppi);

                if (m_ep == m_maxCount)
                {
                    m_ep = 0;
                    unsigned int num = static_cast<float>(m_emission1) * m_respawnRate;
                    SetEmissionInternal(num);
                }

                m_delta = 0.0f;
            }
        }
        else
        {
            this->Emit(particles, pos, m_maxCount);
            flush = false;
        }
    }

    //----------------------------------------
    // Destroy dead particles
    //----------------------------------------
    void ParticleEmitter::Destroy(Particles& particles, const math::AABB& boundingVolume, const math::Vector& origin)
    {
        for (unsigned int i = 0; i < m_activeCount; ++i)
        {
            if (particles.ages[i] >= particles.maxAge || (!boundingVolume.Inside(particles.positions[i] - origin)))
            {
                this->Destroy(particles, i);
            }
        }
    }

    //----------------------------------------
    // Clears all active particles
    //----------------------------------------
    void ParticleEmitter::Clear()
    {
        m_activeCount = 0;
    }

    //----------------------------------------
    // 
    //----------------------------------------
    void ParticleEmitter::Reset()
    {
        SetEmission(m_emission0);
    }
    
    //----------------------------------------------------------------
    // Removes particle at index by swapping with last active particle
    //----------------------------------------------------------------
    void ParticleEmitter::Destroy(Particles& particles, unsigned int index)
    {
        --m_activeCount;

        particles.positions[index] = particles.positions[m_activeCount];
        //particles.oldPositions[index] = particles.oldPositions[m_activeCount];
        particles.velocities[index] = particles.velocities[m_activeCount];
        particles.accelerations[index] = particles.accelerations[m_activeCount];
        particles.colors[index] = particles.colors[m_activeCount];
        particles.extents[index] = particles.extents[m_activeCount];
        particles.rotations[index] = particles.rotations[m_activeCount];
        particles.weights[index] = particles.weights[m_activeCount];
        particles.ages[index] = particles.ages[m_activeCount];
        particles.imageFrames[index] = particles.imageFrames[m_activeCount];
    }

    //----------------------------------------------
    //
    //----------------------------------------------
    void ParticleEmitter::SetMaxParticles(unsigned int num)
    {
        m_maxCount = num;

        if (m_activeCount > num)
        {
            m_activeCount = num;
        }
    }

    //----------------------------------------------
    //
    //----------------------------------------------
    void ParticleEmitter::SetEmission(unsigned int num)
    {
        m_emission0 = num;
        SetEmissionInternal(num);
    }

    //----------------------------------------------
    //
    //----------------------------------------------
    void ParticleEmitter::SetEmissionInternal(unsigned int num)
    {
        m_emission1 = num;
        m_ppi = std::ceil(static_cast<float>(num) / 30.0f);
        m_interval = num ? (1.0f / num) * m_ppi : 0.0f;
    }

    //----------------------------------------------
    //
    //----------------------------------------------
    void ParticleEmitter::SetRespawnRate(float respawnRate)
    {
        m_respawnRate = respawnRate;
    }

    //----------------------------------------
    //
    //----------------------------------------
    PointEmitter::PointEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        const math::Vector& direction)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_direction(direction)
    {
        m_direction.Normalize();
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int PointEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        math::Vector vel = m_direction * m_speed;

        for (unsigned int i = 0; i < num; ++i)
        {
            particles.positions[m_activeCount] = pos;//math::Vector(0.0f, 0.0f, 0.0f);
            particles.velocities[m_activeCount] = vel;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

    //----------------------------------------
    //
    //----------------------------------------
    SphereEmitter::SphereEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        float radius)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_radius(radius)
    {
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int SphereEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        for (unsigned int i = 0; i < num; ++i)
        {
            math::Vector dir(math::RandomVector());
            particles.positions[m_activeCount] = pos + (dir * m_radius);
            particles.velocities[m_activeCount] = dir * m_speed;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

    //----------------------------------------
    //
    //----------------------------------------
    BoxEmitter::BoxEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        const math::Vector& direction,
        const math::Vector& dimensions)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_direction(direction), m_dimensions(dimensions)
    {
        m_direction.Normalize();
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int BoxEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        math::Vector vel = m_direction * m_speed;

        for (unsigned int i = 0; i < num; ++i)
        {
            particles.positions[m_activeCount].x = pos.x + math::RandomFloat(-m_dimensions.x, m_dimensions.x);
            particles.positions[m_activeCount].y = pos.y + math::RandomFloat(-m_dimensions.y, m_dimensions.y);
            particles.positions[m_activeCount].z = pos.z + math::RandomFloat(-m_dimensions.z, m_dimensions.z);
            particles.velocities[m_activeCount] = vel;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

    //----------------------------------------
    //
    //----------------------------------------
    ConeEmitter::ConeEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        const math::Vector& direction,
        float angle)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_direction(direction), m_angle(angle * 0.5f)
    {
        m_direction.Normalize();
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int ConeEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        for (size_t i = 0; i < num; ++i)
        {
            particles.positions[m_activeCount] = pos;//math::Vector(0.0f, 0.0f, 0.0f);

            math::Vector dir(
                math::RandomFloat(-m_angle, m_angle),
                math::RandomFloat(-m_angle, m_angle),
                math::RandomFloat(-m_angle, m_angle));

            math::Matrix m;
            m.SetRotation(&dir.x);
            dir = m_direction;
            m.RotateVec(dir);
            dir.Normalize();

            particles.velocities[m_activeCount] = dir * m_speed;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

    //----------------------------------------
    //
    //----------------------------------------
    TwoBoxEmitter::TwoBoxEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        const math::Vector& direction,
        const math::Vector& srcDimensions,
        const math::Vector& destDimensions,
        float targetDistance)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_direction(direction), m_srcDimensions(srcDimensions), m_destDimensions(destDimensions), m_distance(targetDistance)
    {
        m_direction.Normalize();
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int TwoBoxEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        for (unsigned int i = 0; i < num; ++i)
        {
            particles.positions[m_activeCount].x = pos.x + math::RandomFloat(-m_srcDimensions.x, m_srcDimensions.x);
            particles.positions[m_activeCount].y = pos.y + math::RandomFloat(-m_srcDimensions.y, m_srcDimensions.y);
            particles.positions[m_activeCount].z = pos.z + math::RandomFloat(-m_srcDimensions.z, m_srcDimensions.z);

            math::Vector direction(
                m_direction.x * m_distance + (pos.x + math::RandomFloat(-m_destDimensions.x, m_destDimensions.x)) - particles.positions[m_activeCount].x,
                m_direction.y * m_distance + (pos.y + math::RandomFloat(-m_destDimensions.y, m_destDimensions.y)) - particles.positions[m_activeCount].y,
                m_direction.z * m_distance + (pos.z + math::RandomFloat(-m_destDimensions.z, m_destDimensions.z)) - particles.positions[m_activeCount].z);

            direction.Normalize();

            particles.velocities[m_activeCount] = direction * m_speed;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

    //----------------------------------------
    //
    //----------------------------------------
    CircleEmitter::CircleEmitter(
        unsigned int maxCount,
        unsigned int emission,
        float speed,
        bool hasColor,
        float minWeight,
        float maxWeight,
        float minAgeLB,
        float minAgeUB,
        float respawnRate,
        unsigned int imageCount,
        const math::Vector& direction,
        float radius)

        : ParticleEmitter(maxCount, emission, speed, hasColor, minWeight, maxWeight, minAgeLB, minAgeUB, respawnRate, imageCount),
        m_direction(direction), m_radius(radius)
    {
        m_direction.Normalize();
    }

    //----------------------------------------
    //
    //----------------------------------------
    unsigned int CircleEmitter::Emit(Particles& particles, const math::Vector& pos, unsigned int num)
    {
        if (m_activeCount + num > m_maxCount)
        {
            num = m_maxCount - m_activeCount;
        }

        if (!num) return 0;

        for (unsigned int i = 0; i < num; ++i)
        {
            math::Vector dir(math::RandomVector());
            dir = dir - (m_direction * (dir * m_direction));

            particles.positions[m_activeCount] = pos + (dir * m_radius);
            particles.velocities[m_activeCount] = dir * m_speed;
            particles.extents[m_activeCount] = particles.initialParticleSize;
            particles.weights[m_activeCount] = math::RandomFloat(m_minWeight, m_maxWeight);
            particles.ages[m_activeCount] = math::RandomFloat(m_minAgeLB, m_minAgeUB);
            particles.imageFrames[m_activeCount] = rand() % m_imageCount;
            particles.rotations[m_activeCount] = 0.0f;

            if (m_hasColor)
            {
                particles.colors[m_activeCount] = particles.initialColor;
            }
            else
            {
                particles.colors[m_activeCount].r = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].g = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].b = math::RandomFloat(0.0f, 1.0f);
                particles.colors[m_activeCount].a = 1.0f;
            }

            ++m_activeCount;
        }

        return num;
    }

} // ps

} // kge

