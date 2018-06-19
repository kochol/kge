// File name: Particle.h
// Des: Particles data
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#ifndef PARTICLE_H
#define PARTICLE_H

#include "../math/Vector.h"
#include "../gfx/Color.h"

namespace kge
{

namespace ps
{

struct Particles
{
    math::Vector*   positions;              //
    //math::Vector*   oldPositions;           //
    math::Vector*   velocities;             //
    math::Vector*   accelerations;          //
    gfx::Colorf*    colors;                 //
    math::Vector*   extents;                //
    float*          rotations;              //
    float*          weights;                //
    float*          ages;                   //
    unsigned int*   imageFrames;            //

    // Shared data
    math::Vector    initialParticleSize;    //
    math::Vector    finalParticleSize;      //
    gfx::Colorf     initialColor;           //
    gfx::Colorf     middleColor;            //
    gfx::Colorf     finalColor;             //
    float           maxAge;                 //
    float           angularVelocity;        //
};

} // ps

} // kge

#endif // PARTICLE_H
