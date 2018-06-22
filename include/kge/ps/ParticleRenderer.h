// File name: ParticleRenderer.h
// Des:  
// Date: January 11, 2011 (21/10/1389)
// Programmer: Nader Golbaz

#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#include <vector>
#include "../../include/kge/gfx/Renderer.h"

namespace kge
{

namespace math
{
class Vector;
}

namespace gfx
{
    class Renderer;
    class Texture;
    class HardwareBuffer;
    class Shader;
    class Material;
}

namespace sn
{
class StaticMesh;
}

namespace ps
{

struct Particles;

//!
class ParticleRenderer
{
public:
    //!
    ParticleRenderer(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending);

    //!
    virtual ~ParticleRenderer();

    //!
    virtual void Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer) = 0;

    //! 
    void    SetTextureRect(const math::Vector& rect, unsigned int horizontalFrames, unsigned int verticalFrames);

    //!
    void	SetBuffer(gfx::HardwareBuffer* vb);

    //!
    void    SetSharedBuffer(gfx::HardwareBuffer* ib);

    //!
    void    SetTexture(gfx::Texture* texture);

    //!
    void    SetShader(gfx::Shader* vshader, gfx::Shader* pshader);

    //!
    void    EnableAdditiveBlending(bool additiveBlending);

    //!
    bool    HasAdditiveBlending() const { return m_blendingType == gfx::ERF_AdditiveBlending;; }

    //!
    const math::Vector& GetTextureRect() const { return m_rect; }

    //!
    unsigned int    GetHorizontalFrames() const { return m_horizontalImages; }

    //!
    unsigned int    GetVerticalFrames() const { return m_verticalImages; }

protected:
    math::Vector            m_rect;
    unsigned int            m_horizontalImages;
    unsigned int            m_verticalImages;
    gfx::Material*          m_material;
    gfx::HardwareBuffer*    m_vbuffer;
    gfx::HardwareBuffer*    m_ibuffer;
    gfx::RenderFlags        m_blendingType;
    unsigned int            m_bufferOffset;
};

//!
class QuadRenderer : public ParticleRenderer
{
public:
    //!
    QuadRenderer(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending);

    //!
    void    Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer);
};

//!
class RotatingQuadRenderer : public ParticleRenderer
{
public:
    //!
    RotatingQuadRenderer(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending);

    //!
    void    Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer);
};

//!
class QuadRenderer2 : public ParticleRenderer
{
public:
    //!
    QuadRenderer2(const math::Vector& rect, unsigned int horizontalImages, unsigned int verticalImages, bool additiveBlending);

    //!
    void    Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer);
};





//!
//class MeshRenderer
//{
//public:
//    //!
//    MeshRenderer();
//
//    //!
//    ~MeshRenderer();
//
//    //!
//    void        Render(const Particles& particles, unsigned int activeCount, gfx::Renderer* renderer);
//
//    //!
//    void        SetMesh(sn::StaticMesh* mesh);
//
//private:
//    sn::StaticMesh* m_mesh;
//};

} // ps

} // kge

#endif // PARTICLERENDERER_H
