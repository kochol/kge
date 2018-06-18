// File name: TPCamera.h
// Des: Third person camera with spherical coordinate system
// Date: February 24, 2011 (05/12/1389)
// Programmer: Nader Golbaz

#ifndef TPCAMERA_H
#define TPCAMERA_H

#include "../core/Structs.h"
#include "../math/Vector.h"
#include "../math/Frustum.h"
#include "SceneNode.h"

namespace kge
{

namespace sn
{

class TileTerrain;

class KGE_API TPCamera : public SceneNode
{
    //! Copy constructor
    TPCamera(const TPCamera& other); // non copyable

    //! Assignment operator
    TPCamera& operator =(const TPCamera& other); // non copyable

public:
    //! Constructor
    TPCamera(SceneManager* pSnMan, float distance, float theta, float phi, const math::Vector& target, const math::Vector& up);

    //! Constructor
    TPCamera(SceneManager* pSnMan, const math::Vector& position, const math::Vector& target, const math::Vector& up);

    //! 
    void                    PreRender(float elapsedTime = 0.0f);

    //! 
    void                    Render();

    //! 
    void                    PostRender();

    //! 
    void                    Draw(bool WithMaterial , bool WithTransform , bool bPosition , bool bNormalTexcoord , bool bTangentBinormal);
    
    //! 
    void                    Fallow(const sn::SceneNode* node) { m_fallowing = node; }

    //! Camera's position
    void                    SetPosition(const math::Vector& position);

    //! World's up
    void                    SetUpDir(const math::Vector& up);

    //! Sphere's center
    void                    SetTarget(const math::Vector& target);

    //! Sphere's radius
    void                    SetDistance(float distance);

    //! Polar angle in radians
    void                    SetTheta(float theta);

    //! Polar angle in degrees
    void                    SetThetaD(float theta);

    //! Azimuth angle in radians
    void                    SetPhi(float phi);

    //! Azimuth angle in degrees
    void                    SetPhiD(float phi);

    //! Field of view in radians
    void                    SetFov(float fov);

    //! Field of view in degrees
    void                    SetFovD(float fov);

    //! Z-value of the near view-plane.
    void                    SetNearValue(float nearZ);

    //! Z-value of the far view-plane
    void                    SetFarValue(float farZ);

    //! 
    void                    SetProjectionMatrix(const math::Matrix& projection) { m_projection = projection; m_projChanged = true; }

    //! 
    void                    SetDelay(float delay) { m_delay = delay; }

    //! 
    void                    OnResize();

    //! 
    //void                    MoveFB(float distance);

    //! 
    //void                    MoveRL(float distance);

    //! 
    void                    RotateVer(float angle);

    //! 
    void                    RotateVerD(float angle);

    //! 
    void                    RotateHor(float angle);

    //! 
    void                    RotateHorD(float angle);

    //! 
    void                    Zoom(float scale);

    //! 
    void                    Shake(float magnitude, float duration);

    //! Returns the view matrix.
    const math::Matrix&     GetViewMatrix() const { return m_view; }

    //! Returns the projection matrix.
    const math::Matrix&     GetProjectionMatrix() const { return m_projection; }

    //! 
    float                   GetFov() const { return m_fov; }

    //! 
    float                   GetFovD() const;

    //! 
    float                   GetNearValue() const { return m_nearZ; }

    //! 
    float                   GetFarValue() const { return m_farZ; }

    //! 
    const math::Vector&     GetPosition() const { return m_position; }

    //! 
    const math::Vector&     GetUpDir() const { return m_up; }

    //! 
    const math::Vector&     GetTarget() const { return m_center; }

    //! 
    math::Vector            GetRightDir() const;

    //! 
    math::Vector            GetDirection() const;

    //! 
    float                   GetDistance() const { return m_radius; }

    //! 
    float                   GetTheta() const { return m_inclination; }

    //! 
    float                   GetThetaD() const;

    //! 
    float                   GetPhi() const { return m_azimuth + math::PiOver2; }

    //! 
    float                   GetPhiD() const;

    //! 
    float                   GetDelay() const { return m_delay; }

    //! 
    bool                    IsChanged() const { return m_viewChanged; }

    //! Returns the view frustum.
    const math::Frustum*    GetFrustum() const { return &m_frustum; }

    //! 
    int                     GetViewportWidth() const { return m_vpWidth; }

    //! 
    int                     GetViewportHeight() const { return m_vpHeight; }

    //! Returns the node type.
    NodeType                GetType() const { return ENT_Camera; }

private:
    //! 
    void                    ComputeProjectionMatrix();

    //! 
    void                    ComputeViewMatrix();

private:
    // spherical coordinates
    math::Vector            m_center;
    float                   m_radius;
    float                   m_inclination;
    float                   m_azimuth;
    const float             m_minTheta;
    const float             m_maxTheta;


    //
    math::Vector            m_position;     // Camera Position
    math::Vector            m_up;           // Up Direction
    math::Matrix            m_view;         // View Matrix
    math::Matrix            m_projection;   // Projection Matrix
    float                   m_fov;          // Field of View
    float                   m_nearZ;        // Near View Plane
    float                   m_farZ;         // Far View Plane
    math::Frustum           m_frustum;      // View Frustum
    int                     m_vpWidth;      // Viewport Width
    int                     m_vpHeight;     // Viewport Height

    //
    bool                    m_update;
    bool                    m_lerp;
    bool                    m_projChanged;
    bool                    m_viewChanged;
    math::Vector            m_target0;
    math::Vector            m_target1;
    float                   m_delay;
    float                   m_elapsed;
    const sn::SceneNode*    m_fallowing;

    // shaking variables
    float                   m_shakeMagnitude;
    float                   m_shakeDuration;
    float                   m_shakeTime;
    float                   m_shaking;
    math::Vector            m_tempEye;
    math::Vector            m_tempTarget;
};

} // sn

} // kge

#endif // TPCAMERA_H

