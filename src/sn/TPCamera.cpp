// File name: TPCamera.cpp
// Des: Third person camera with spherical coordinate system
// Date: February 24, 2011 (05/12/1389)
// Programmer: Nader Golbaz

#include "../../include/kge/sn/TPCamera.h"
#include "../../include/kge/sn/SceneManager.h"
#include "../../include/kge/gfx/Renderer.h"
#include "../../include/kge/math/Utility.h"
#include <cmath>

namespace kge
{

namespace sn
{
    //-------------------------
    // 
    //-------------------------
    TPCamera::TPCamera(
        SceneManager* pSnMan, float distance, float theta, float phi, const math::Vector& target, const math::Vector& up)
        : m_center(target), m_radius(distance), m_inclination(theta), m_azimuth(phi - math::PiOver2),
        m_minTheta(math::PiOver2 / 90.0f), m_maxTheta(math::PiOver2 - math::PiOver2 / 90.0f),
        m_up(up), m_update(false), m_fov(math::PI / 4.0f), m_nearZ(0.1f), m_farZ(1000.0f),
        m_lerp(false), m_viewChanged(false), m_projChanged(false),
        m_delay(0.5f), m_elapsed(0.0f), m_fallowing(0),
        m_shakeMagnitude(0.0f), m_shakeDuration(0.0f), m_shakeTime(0.0f), m_shaking(false)
    {
        m_eNodeType = ENT_Camera;
        m_pSnMan = pSnMan;
        m_pRenderer = m_pSnMan->GetRenderer();
        m_vpWidth = m_pRenderer->GetWindowWidth();
        m_vpHeight = m_pRenderer->GetWindowHeight();

        if (m_inclination > m_maxTheta) m_inclination = m_maxTheta;
        else if (m_inclination < m_minTheta) m_inclination = m_minTheta;

        const float rs = m_radius * sinf(m_inclination);
        m_position.x = rs * cosf(m_azimuth) + m_center.x;
        m_position.y = m_radius * cosf(m_inclination) + m_center.y;
        m_position.z = rs * sinf(m_azimuth) + m_center.z;

        m_pAbsMat->SetTranslation(&m_position.v[0]);
        m_frustum.m_pCameraPosition = &m_position;

        ComputeProjectionMatrix();
        ComputeViewMatrix();
        m_frustum.Init(&m_view, &m_projection);

        m_target0 = m_target1 = m_center;
    }

    //-------------------------
    // 
    //-------------------------
    TPCamera::TPCamera(SceneManager* pSnMan, const math::Vector& position, const math::Vector& target, const math::Vector& up)
        : m_center(target),
        m_minTheta(math::PiOver2 / 90.0f), m_maxTheta(math::PiOver2 - math::PiOver2 / 90.0f),
        m_position(position), m_up(up), m_fov(math::PI / 4.0f), m_nearZ(0.1f), m_farZ(1000.0f),
        m_update(false), m_lerp(false), m_viewChanged(false), m_projChanged(false),
        m_delay(0.5f), m_elapsed(0.0f), m_fallowing(0),
        m_shakeMagnitude(0.0f), m_shakeDuration(0.0f), m_shakeTime(0.0f), m_shaking(false)
    {
        m_eNodeType = ENT_Camera;
        m_pSnMan = pSnMan;
        m_pRenderer = m_pSnMan->GetRenderer();
        m_vpWidth = m_pRenderer->GetWindowWidth();
        m_vpHeight = m_pRenderer->GetWindowHeight();

        m_radius = (m_center - m_position).GetLength();
        m_inclination = acosf((m_position.y - m_center.y) / m_radius);
        m_azimuth = atanf((m_position.z - m_center.z) / (m_position.x - m_center.x));

        m_pAbsMat->SetTranslation(&m_position.v[0]);
        m_frustum.m_pCameraPosition = &m_position;
        
        ComputeProjectionMatrix();
        ComputeViewMatrix();
        m_frustum.Init(&m_view, &m_projection);

        m_target0 = m_target1 = m_center;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::PreRender(float elapsedTime)
    {
        if (!m_shaking)
        {
            if (m_fallowing)
            {
                SetTarget(m_fallowing->GetPosition());
            }

            if (m_lerp)
            {
                m_elapsed += elapsedTime;
                const float age = m_elapsed / m_delay;
                m_center = m_target0 + (m_target1 - m_target0) * age;
                const float rs = m_radius * sinf(m_inclination);
                m_position.x = rs * cosf(m_azimuth) + m_center.x;
                m_position.y = m_radius * cosf(m_inclination) + m_center.y;
                m_position.z = rs * sinf(m_azimuth) + m_center.z;

                if (m_elapsed >= m_delay) m_lerp = false;

                m_update = true;
            }
        }
        else
        {
            m_shakeTime += elapsedTime;

            if (m_shakeTime < m_shakeDuration)
            {
                const float progress = m_shakeTime / m_shakeDuration;
                const float magnitude = m_shakeMagnitude * (1.0f - (progress * progress));
                const math::Vector shakingOffset = math::RandomVector() * magnitude;
                m_position = m_tempEye + shakingOffset;
                m_center = m_tempTarget + shakingOffset;
                m_update = true;
            }
            else
            {
                m_shaking = false;
                m_position = m_tempEye;
                m_center = m_tempTarget;
                m_update = true;
            }
        }

        if (m_update)
        {
            ComputeViewMatrix();
            m_update = false;
        }

        //if (m_pParent)
        //{
        //    *m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
        //}
        //else
        //{
        //    *m_pFinalMat = *m_pAbsMat;
        //}

        if (m_bVis && (m_viewChanged || m_projChanged))
        {
            m_pSnMan->RegNode(this, ERO_First);
            m_frustum.Init(&m_view, &m_projection);
        }
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::Render()
    {
        if (m_projChanged)
        {
            m_pRenderer->SetTransForm(&m_projection, gfx::ETM_Projection);
            m_projChanged = false;
        }

        if (m_viewChanged)
        {
            m_pRenderer->SetTransForm(&m_view, gfx::ETM_View);
            m_viewChanged = false;
        }
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::PostRender()
    {

    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::Draw(bool WithMaterial , bool WithTransform , bool bPosition , bool bNormalTexcoord , bool bTangentBinormal)
    {
        
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetPosition(const math::Vector& position)
    {
        m_pAbsMat->SetTranslation(&position.v[0]);
        m_position = position;
        m_radius = (m_center - m_position).GetLength();
        m_inclination = acosf((m_position.y - m_center.y) / m_radius);
        m_azimuth = atanf((m_position.z - m_center.z) / (m_position.x - m_center.x));
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetUpDir(const math::Vector& up)
    {
        m_up = up;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetTarget(const math::Vector& target)
    {
        m_target0 = m_center;
        m_target1 = target;
        m_elapsed = 0.0f;
        m_lerp = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetDistance(float distance)
    {
        m_position = (m_position / m_radius) * distance;
        m_radius = distance;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetTheta(float theta)
    {
        if (theta > m_maxTheta) theta = m_maxTheta;
        else if (theta < m_minTheta) theta = m_minTheta;

        const float rs = m_radius * sinf(theta);
        m_position.x = rs * cosf(m_azimuth) + m_center.x;
        m_position.y = m_radius * cosf(theta) + m_center.y;
        m_position.z = rs * sinf(m_azimuth) + m_center.z;
        m_inclination = theta;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetThetaD(float theta)
    {
        theta *= math::fDegToRad;

        if (theta > m_maxTheta) theta = m_maxTheta;
        else if (theta < m_minTheta) theta = m_minTheta;

        const float rs = m_radius * sinf(theta);
        m_position.x = rs * cosf(m_azimuth) + m_center.x;
        m_position.y = m_radius * cosf(theta) + m_center.y;
        m_position.z = rs * sinf(m_azimuth) + m_center.z;
        m_inclination = theta;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetPhi(float phi)
    {
        const float rs = m_radius * sinf(m_inclination);
        m_position.x = rs * cosf(phi) + m_center.x;
        m_position.z = rs * sinf(phi) + m_center.z;
        m_azimuth = phi;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetPhiD(float phi)
    {
        phi *= math::fDegToRad;
        const float rs = m_radius * sinf(m_inclination);
        m_position.x = rs * cosf(phi) + m_center.x;
        m_position.z = rs * sinf(phi) + m_center.z;
        m_azimuth = phi;
        m_update = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetFov(float fov)
    {
        m_fov = fov;
        ComputeProjectionMatrix();
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetFovD(float fov)
    {
        m_fov = fov * math::fDegToRad;
        ComputeProjectionMatrix();
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetNearValue(float nearZ)
    {
        m_nearZ = nearZ;
        ComputeProjectionMatrix();
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::SetFarValue(float farZ)
    {
        m_farZ = farZ;
        ComputeProjectionMatrix();
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::OnResize()
    {
        m_vpWidth = static_cast<float>(m_pRenderer->GetWindowWidth());
        m_vpHeight = static_cast<float>(m_pRenderer->GetWindowHeight());
        ComputeProjectionMatrix();
    }

    //-------------------------
    // 
    //-------------------------
    //void TPCamera::MoveFB(float distance)
    //{
    //    math::Vector dir = m_center - m_position;
    //    dir.Normalize();

    //    m_position += dir * distance;
    //    m_center += dir * distance;

    //    m_fallowing = 0;
    //    m_lerp = false;
    //    m_update = true;
    //}

    //-------------------------
    // 
    //-------------------------
    //void TPCamera::MoveRL(float distance)
    //{
    //    math::Vector dir = m_center - m_position;
    //    dir.Normalize();

    //    math::Vector right;
    //    right.Cross(m_up, dir);
    //    right.Normalize();

    //    m_position += right * distance;
    //    m_center += right * distance;

    //    m_fallowing = 0;
    //    m_lerp = false;
    //    m_update = true;
    //}

    //-------------------------
    // 
    //-------------------------
    void TPCamera::RotateVer(float angle)
    {
        SetTheta(m_inclination + angle);
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::RotateVerD(float angle)
    {
        RotateVer(angle * math::fDegToRad);
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::RotateHor(float angle)
    {
        SetPhi(m_azimuth + angle);
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::RotateHorD(float angle)
    {
        RotateHor(angle * math::fDegToRad);
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::Zoom(float scale)
    {
        SetDistance(m_radius + scale);
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::Shake(float magnitude, float duration)
    {
        m_shakeMagnitude = magnitude;
        m_shakeDuration = duration;
        m_shakeTime = 0.0f;

        if (!m_shaking)
        {
            m_tempEye = m_position;
            m_tempTarget = m_center;
            m_shaking = true;
        }
    }

    //-------------------------
    // 
    //-------------------------
    math::Vector TPCamera::GetRightDir() const
    {
        math::Vector dir = m_center - m_position;
        dir.Normalize();

        math::Vector right;
        right.Cross(m_up, dir);
        right.Normalize();

        return right;
    }

    //-------------------------
    // 
    //-------------------------
    math::Vector TPCamera::GetDirection() const
    {
        math::Vector dir = m_center - m_position;
        dir.Normalize();
        return dir;
    }

    //-------------------------
    // 
    //-------------------------
    float TPCamera::GetThetaD() const
    {
        return m_inclination * math::fRadToDeg;
    }

    //-------------------------
    // 
    //-------------------------
    float TPCamera::GetPhiD() const
    {
        return m_azimuth * math::fRadToDeg + math::PiOver2;
    }

    //-------------------------
    // 
    //-------------------------
    float TPCamera::GetFovD() const
    {
        return m_fov * math::fRadToDeg;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::ComputeProjectionMatrix()
    {
        const float yScale = 1.0f / tanf(m_fov * 0.5f);
        const float xScale = yScale / (static_cast<float>(m_vpWidth) / static_cast<float>(m_vpHeight));

        m_projection._11 = xScale; m_projection._12 = 0.0f;   m_projection._13 = 0.0f;                        m_projection._14 = 0.0f;
        m_projection._21 = 0.0f;   m_projection._22 = yScale; m_projection._23 = 0.0f;                        m_projection._24 = 0.0f;
        m_projection._31 = 0.0f;   m_projection._32 = 0.0f;   m_projection._33 = m_farZ / (m_farZ - m_nearZ); m_projection._34 = 1.0f;
        m_projection._41 = 0.0f;   m_projection._42 = 0.0f;   m_projection._43 = -m_nearZ * m_projection._33; m_projection._44 = 0.0f;

        m_projChanged = true;
    }

    //-------------------------
    // 
    //-------------------------
    void TPCamera::ComputeViewMatrix()
    {
        math::Vector zAxis = m_center - m_position;
        zAxis.Normalize();

        math::Vector xAxis;
        xAxis.Cross(m_up, zAxis);
        xAxis.Normalize();

        math::Vector yAxis;
        yAxis.Cross(zAxis, xAxis);

        m_view._11 = xAxis.x;               m_view._12 = yAxis.x;               m_view._13 = zAxis.x;               m_view._14 = 0.0f;
        m_view._21 = xAxis.y;               m_view._22 = yAxis.y;               m_view._23 = zAxis.y;               m_view._24 = 0.0f;
        m_view._31 = xAxis.z;               m_view._32 = yAxis.z;               m_view._33 = zAxis.z;               m_view._34 = 0.0f;
        m_view._41 = -(xAxis * m_position); m_view._42 = -(yAxis * m_position); m_view._43 = -(zAxis * m_position); m_view._44 = 1.0f;

        m_viewChanged = true;
    }

} // sn

} // kge
