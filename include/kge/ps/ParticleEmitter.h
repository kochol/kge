// File name: ParticleEmitter.h
// Des: 
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "../math/Vector.h"
#include "../math/AABB.h"

namespace kge
{

namespace ps
{

struct Particles;

//
class ParticleEmitter
{
public:
    //!
    ParticleEmitter(unsigned int maxCount,
                    unsigned int emission,
                    float speed,
                    bool hasColor,
                    float minWeight,
                    float maxWeight,
                    float minAgeLB,
                    float minAgeUB,
                    float respawnRate,
                    unsigned int imageCount);

    //!
    virtual ~ParticleEmitter() {}

    //!
    virtual unsigned int        Emit(Particles& particles, const math::Vector& pos, unsigned int num) = 0;

    //!
    void			            Emit(Particles& particles, const math::Vector& pos, float elapsedTime, bool& flush);

    //!
    void			            Destroy(Particles& particles, const math::AABB& boundingVolume, const math::Vector& origin);

    //!
    void			            Clear();

    //! 
    void                        Reset();
    
    //!
    const unsigned int          ActiveCount() const { return m_activeCount; }

    //!
    void                        SetMaxParticles(unsigned int num);

    //!
    void                        SetRanges(const math::Vector& ranges);

    //!
    void                        SetEmission(unsigned int num);

    //!
    void                        SetRespawnRate(float respawnRate);

    //!
    void                        SetSpeed(float speed) { m_speed = speed; }

    //!
    void                        SetWeight(float minWeight, float maxWeight) { m_minWeight = minWeight; m_maxWeight = maxWeight; }

    //!
    void                        SetMinAge(float lowerBound, float upperBound) { m_minAgeLB = lowerBound; m_minAgeUB = upperBound; }

    //!
    virtual void                SetDirection(const math::Vector& direction) {}

    //!
    virtual void                SetAngle(float angle) {}

    //!
    virtual void                SetRadius(float radius) {}

    //!
    virtual void                SetSize(const math::Vector& dimensions) {}

    //!
    virtual void                SetTargetSize(const math::Vector& dimensions) {}

    //!
    virtual void                SetTargetDistance(float distance) {}

    //!
    void                        EnableColor(bool hasColor) { m_hasColor = hasColor; }
    
    //!
    bool                        HasColor() const { return m_hasColor; }

    //!
    unsigned int                GetMaxParticles() const { return m_maxCount; }

    //!
    unsigned int                GetEmission() const { return m_emission0; }

    //!
    unsigned int                GetCurrentEmission() const { return m_emission1; }

    //!
    float                       GetSpeed() const { return m_speed; }

    //!
    float                       GetMinWeight() const { return m_minWeight; }

    //!
    float                       GetMaxWeight() const { return m_maxWeight; }

    //!
    float                       GetMinAgeLB() const { return m_minAgeLB; }

    //!
    float                       GetMinAgeUB() const { return m_minAgeUB; }

    //!
    float                       GetRespawnRate() const { return m_respawnRate; }

    //!
    virtual float               GetAngle() const { return 0.0f; }

    //!
    virtual float               GetRadius() const { return 0.0f; }

    //!
    virtual math::Vector        GetSize() const { return math::Vector(0.0f, 0.0f, 0.0f); }

    //!
    virtual math::Vector        GetTargetSize() const { return math::Vector(0.0f, 0.0f, 0.0f); }

    //!
    virtual float               GetTargetDistance() const { return 0.0f; }

    //!
    virtual math::Vector        GetDirection() const { return math::Vector(); }

protected:
    //!
    void			            Destroy(Particles& particles, unsigned int index);

    //!
    void                        SetEmissionInternal(unsigned int num);

    unsigned int                m_maxCount;
    unsigned int                m_emission0;
    unsigned int                m_emission1;
    float			            m_speed;
    bool                        m_hasColor;
    float                       m_minWeight;
    float                       m_maxWeight;
    float                       m_minAgeLB;
    float                       m_minAgeUB;
    unsigned int                m_activeCount;
    float                       m_interval;
    unsigned int                m_ppi;
    float                       m_delta;
    float                       m_respawnRate;
    unsigned int                m_ep;
    unsigned int                m_imageCount;
};

class PointEmitter : public ParticleEmitter
{
public:
    //!
    PointEmitter(   unsigned int maxCount,
                    unsigned int emission,
                    float speed,
                    bool hasColor,
                    float minWeight,
                    float maxWeight,
                    float minAgeLB,
                    float minAgeUB,
                    float respawnRate,
                    unsigned int imageCount,
                    const math::Vector& direction);

    //!
    unsigned int        Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void                SetDirection(const math::Vector& direction) { m_direction = direction; m_direction.Normalize(); }

    //!
    math::Vector        GetDirection() const { return m_direction; }

private:
    math::Vector        m_direction;
};

//
class SphereEmitter : public ParticleEmitter
{
public:
    //!
    SphereEmitter(  unsigned int maxCount,
                    unsigned int emission,
                    float speed,
                    bool hasColor,
                    float minWeight,
                    float maxWeight,
                    float minAgeLB,
                    float minAgeUB,
                    float respawnRate,
                    unsigned int imageCount,
                    float radius);

    //!
    unsigned int    Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void            SetRadius(float radius) { m_radius = radius; }

    //!
    float           GetRadius() const { return m_radius; }

private:
    float           m_radius;
};

//
class BoxEmitter : public ParticleEmitter
{
public:
    //!
    BoxEmitter( unsigned int maxCount,
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
                const math::Vector& dimensions);

    //!
    unsigned int        Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void		        SetDirection(const math::Vector& direction) { m_direction = direction; m_direction.Normalize(); }

    //!
    void                SetSize(const math::Vector& dimensions) { m_dimensions = dimensions; }

    //!
    math::Vector        GetDirection() const { return m_direction; }

    //!
    math::Vector        GetSize() const { return m_dimensions; }

private:
    math::Vector	    m_direction;
    math::Vector        m_dimensions;
};

//
class ConeEmitter : public ParticleEmitter
{
public:
    //!
    ConeEmitter(unsigned int maxCount,
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
                float angle);

    //!
    unsigned int        Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void		        SetDirection(const math::Vector& direction) { m_direction = direction; m_direction.Normalize(); }

    //!
    void                SetAngle(float angle) { m_angle = angle * 0.5f; }

    //!
    math::Vector        GetDirection() const { return m_direction; }

    //!
    float               GetAngle() const { return m_angle * 2.0f; }

private:
    math::Vector        m_direction;
    float               m_angle;
};

//
class TwoBoxEmitter : public ParticleEmitter
{
public:
    //!
    TwoBoxEmitter(
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
        float targetDistance);

    //!
    unsigned int        Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void		        SetDirection(const math::Vector& direction) { m_direction = direction; m_direction.Normalize(); }

    //!
    void                SetSize(const math::Vector& dimensions) { m_srcDimensions = dimensions; }

    //!
    void                SetTargetSize(const math::Vector& dimensions) { m_destDimensions = dimensions; }

    //!
    void                SetTargetDistance(float distance) { m_distance = distance; }

    //!
    math::Vector        GetDirection() const { return m_direction; }

    //!
    math::Vector        GetSize() const { return m_srcDimensions; }

    //!
    math::Vector        GetTargetSize() const { return m_destDimensions; }

    //!
    float               GetTargetDistance() const { return m_distance; }

private:
    math::Vector	    m_direction;
    math::Vector        m_srcDimensions;
    math::Vector        m_destDimensions;
    float               m_distance;
};

//
class CircleEmitter : public ParticleEmitter
{
public:
    //!
    CircleEmitter(  unsigned int maxCount,
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
        float radius);

    //!
    unsigned int    Emit(Particles& particles, const math::Vector& pos, unsigned int num);

    //!
    void            SetRadius(float radius) { m_radius = radius; }

    //!
    void		    SetDirection(const math::Vector& direction) { m_direction = direction; m_direction.Normalize(); }

    //!
    float           GetRadius() const { return m_radius; }

    //!
    math::Vector    GetDirection() const { return m_direction; }

private:
    math::Vector    m_direction;
    float           m_radius;
};

} // ps

} // kge

#endif // PARTICLEEMITTER_H
