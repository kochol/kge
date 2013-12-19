// File name: ParticleBehavior.h
// Des: 
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#ifndef PARTICLEBEHAVIOR_H
#define PARTICLEBEHAVIOR_H

#include <vector>

namespace kge
{

namespace ps
{

struct Particles;

//!
class ParticleBehavior
{
public:
    void ApplyForces(Particles& particles, unsigned int activeCount,
        const std::vector<math::Vector>& forces);

    void ApplyGravity(Particles& particles, unsigned int activeCount,
        const math::Vector& gravity);

    virtual void Update(Particles& particles, unsigned int activeCount, float elapsedTime) = 0;
};

//!
class SimpleBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class FadingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class ScalingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class RotatingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class FadingScalingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class FadingRotatingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class ScalingRotatingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class FadingScalingRotatingBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

//!
class ChaoticBehavior : public ParticleBehavior
{
public:
    void Update(Particles& particles, unsigned int activeCount, float elapsedTime);
};

} // ps

} // kge

#endif // PARTICLEBEHAVIOR_H
