// File name: Camera.cpp
// Des: In class Camera Node hast.
// Date: 4/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include <math.h>
#include "../../Include/sn/Camera.h"
#include "../../Include/io/Logger.h"
#include "../../Include/math/Utility.h"
#include "../../Include/gfx/Renderer.h"
#include <SDL2/SDL.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#endif



namespace kge
{
namespace sn
{
	// ******* ***********
	// Constructor
	// ******* ***********
	Camera::Camera(SceneManager* pSnMan,
		const math::Vector &vPos, const math::Vector &vPoint, const math::Vector &vUp)
	{
		m_eNodeType = ENT_Camera;
		m_pSnMan = pSnMan;
		m_pRenderer = m_pSnMan->GetRenderer();

		m_vPos      = vPos;
		m_vLook     = vPoint;
		m_vUp       = vUp;
		m_bARBM		= false;
		m_fCurRotX  = 0.0f;
		m_fLastRotX = 0.0f;
		m_frustum.m_pCameraPosition = &m_vPos;
		m_pBB		= KGE_NEW(math::Sphere)();
		m_pAABB		= KGE_NEW(math::AABB)(vPos);
		m_bProjChanged = true;
		m_vpWidth = m_pRenderer->GetWindowWidth();
		m_vpHeight = m_pRenderer->GetWindowHeight();
		m_fFov = math::PI / 4.0f;
		m_fNear = 0.1f;
		m_fFar = 1000.0f;

		m_mProj.SetPerspectiveLH((float)m_vpWidth, 
								 (float)m_vpHeight,
								 m_fFov,
								 m_fNear,
								 m_fFar);

		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);
		m_iMiddleX = current.w;
		m_iMiddleY = current.h;

		m_iMiddleX = m_iMiddleX >> 1; // Taghsim bar 2 fast
		m_iMiddleY = m_iMiddleY >> 1; // Taghsim bar 2 fast

		CalcViewMatrix();
		m_mView._44 = 1.0f;
		m_mView._14 = m_mView._24 = m_mView._34 = 0.0f;

		m_frustum.Init(&m_mView, &m_mProj);

	} // Constructor

	//-----------------------------------------------------------------------------
	// Destructor
	//-----------------------------------------------------------------------------
	Camera::~Camera()
	{
		KGE_DELETE(m_pBB, Sphere);
		KGE_DELETE(m_pAABB, AABB);

	} // Destructor

	// ******** ** ***** ** ****** ***** ***** ******
	// Karhayee ke Ghabl az render bayad anjam shvad.
	// ******** ** ***** ** ****** ***** ***** ******
	void Camera::PreRender(float elapsedTime)
	{
		if (m_pParent)
			*m_pFinalMat = (*m_pParent->GetFinalMatrix()) * (*m_pAbsMat);
		else
			*m_pFinalMat = *m_pAbsMat;

		if (m_bVis)
		{
			if (m_bARBM)
				RotViewByMouse();
			if (m_bChanged || m_bProjChanged)
			{
				m_pSnMan->RegNode(this, ERO_First);

			} // m_bChanged

		} // m_bVis
		m_frustum.Init(&m_mView, &m_mProj);

	} // PreRender
	
	// ****** ******* *******
	// Render kardane Object.
	// ****** ******* *******
	void Camera::Render()
	{
		if (m_bChanged)
		{
			m_pRenderer->SetTransForm(&m_mView, gfx::ETM_View);
			m_bChanged = false;
		}
		if (m_bProjChanged)
		{
			m_pRenderer->SetTransForm(&m_mProj, gfx::ETM_Projection);
			m_bProjChanged = false;
			m_vpWidth	= m_pRenderer->GetWindowWidth();
			m_vpHeight	= m_pRenderer->GetWindowHeight();
		}
		
	} // Render

	//--------------------------------------------
	// Karhaee ke bad az render bayad anjam beshe.
	//--------------------------------------------
	void Camera::PostRender()
	{
	} // Post Render

	// ******** ****** ** **** ** ******
	// Harekate dorbin be jolo va aghab.
	// ******** ****** ** **** ** ******
	void Camera::MoveBF(float Speed)
	{
		math::Vector v;
		v = m_vLook - m_vPos;
		v.Normalize();
		m_vPos.x += v.x * Speed;
		m_vPos.y += v.y * Speed;
		m_vPos.z += v.z * Speed;
		m_vLook.x += v.x * Speed;
		m_vLook.y += v.y * Speed;
		m_vLook.z += v.z * Speed;
		
		// Now rebulid the new view matrix.
		math::Vector Z = m_vLook - m_vPos; // Direction changed
		Z.Normalize();

		m_mView._43 = -(Z * m_vPos);

		m_bChanged = true;
	} // Move Back and Forward

	// ******** ****** ** **** ** *****
	// Harekate dorbin be Rast va Chap.
	// ******** ****** ** **** ** *****
	void Camera::MoveRL(float Speed)
	{
		m_vPos.x += m_vRight.x * Speed;
		m_vPos.y += m_vRight.y * Speed;
		m_vPos.z += m_vRight.z * Speed;

		m_vLook.x += m_vRight.x * Speed;
		m_vLook.y += m_vRight.y * Speed;
		m_vLook.z += m_vRight.z * Speed;

		CalcViewMatrix();

	} // Move Right and Left

	// ********* ****** *** *** *******
	// Charkhesh dorbin dor yek bordar.
	// ********* ****** *** *** *******
	void Camera::Rotate(float Angle, const math::Vector& Axis)
	{
		math::Vector vDir,				// Jahati ke dorbin dare nega mikone.
					 vLook;				// Noghte jadidi ke dorbin dare nega mikone.

		vDir = m_vLook - m_vPos;

		float cost = cosf(Angle);
		float sint = sinf(Angle);

		// Find the new x position for the new rotated point
		vLook.x  = (cost + (1 - cost) * Axis.x * Axis.x)			* vDir.x;
		vLook.x += ((1 - cost) * Axis.x * Axis.y - Axis.z * sint)	* vDir.y;
		vLook.x += ((1 - cost) * Axis.x * Axis.z + Axis.y * sint)	* vDir.z;

		// Find the new y position for the new rotated point
		vLook.y  = ((1 - cost) * Axis.x * Axis.y + Axis.z * sint)	* vDir.x;
		vLook.y += (cost + (1 - cost) * Axis.y * Axis.y)			* vDir.y;
		vLook.y += ((1 - cost) * Axis.y * Axis.z - Axis.x * sint)	* vDir.z;

		// Find the new z position for the new rotated point
		vLook.z  = ((1 - cost) * Axis.x * Axis.z - Axis.y * sint)	* vDir.x;
		vLook.z += ((1 - cost) * Axis.y * Axis.z + Axis.x * sint)	* vDir.y;
		vLook.z += (cost + (1 - cost) * Axis.z * Axis.z)			* vDir.z;


		// Now we just add the newly rotated vector to our position to set
		// our new rotated view of our camera.
		m_vLook.x = m_vPos.x + vLook.x;
		m_vLook.y = m_vPos.y + vLook.y;
		m_vLook.z = m_vPos.z + vLook.z;

		CalcViewMatrix();
	} // Rotate

	// ********** ******* ****
	// Mohasebeye matrixe view
	// ********** ******* ****
	void Camera::CalcViewMatrix()
	{
		math::Vector Z = m_vLook - m_vPos; // Direction vector
		Z.Normalize();
	
		// calculate up vector
		math::Vector vcTemp, vcUp;
		float fDot = m_vUp * Z;
		vcTemp = Z * fDot;
		vcUp = m_vUp - vcTemp;
		float fL = vcUp.GetLength();

		// if too short take y axis 
		if (fL < 1e-6f) 
		{
			math::Vector vcY;
			vcY.set(0.0f, 1.0f, 0.0f);

			vcTemp = Z * Z.y;
			vcUp = vcY - vcTemp;

			fL = vcUp.GetLength();

			// take z axis if still too short
			if (fL < 1e-6f)
			{
				vcY.set(0.0f, 0.0f, 1.0f);
	
				vcTemp = Z * Z.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				// we tried our best
				if (fL < 1e-6f)
				{
					io::Logger::Log("Error: Can't calculate the up vector for camera.", io::ELM_Error);
					return;
				}
			}
		}
		vcUp /= fL;

		m_vRight.Cross(vcUp, Z);

		m_mView._11 = m_vRight.x;		m_mView._12 = vcUp.x;		m_mView._13 = Z.x;
		m_mView._21 = m_vRight.y;		m_mView._22 = vcUp.y;		m_mView._23 = Z.y;
		m_mView._31 = m_vRight.z;		m_mView._32 = vcUp.z;		m_mView._33 = Z.z;
		m_mView._41 = -(m_vRight * m_vPos);		m_mView._42 = -(vcUp * m_vPos);		m_mView._43 = -(Z * m_vPos);

		m_bChanged = true;

	} // CalcViewMatrix

	// ** ****** ********* ****** ******** ***** ***********
	// Be sorate Automatic dorbin tavasote mouse micharkhad.
	// ** ****** ********* ****** ******** ***** ***********
	void Camera::AutoRotateByMouse(bool Enable, float speed)
	{
		if (Enable)
		{
			m_fSpeed = 1001.0f - speed;
			if (m_fSpeed < 1.0f)
				m_fSpeed = 1.0f;
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		m_bARBM = Enable;
	} // AutoRotateByMouse

	// ****** **** ******
	// Rotate with mouse.
	// ****** **** ******
	void Camera::RotViewByMouse()
	{
		int MoX , MoY;	// Mouse position
		float angleY = 0.0f; // In baraye nega kardan be rast va chap hast.
		float angleZ = 0.0f; // In baraye nega kardan be bala va paeen hast.

		SDL_GetRelativeMouseState(&MoX, &MoY);
		printf("w = %d\nh = %d\n", MoX, MoY);

		// Agar mouse tekan nakhorde bod az tabe kharej mishim.
		if ((MoX == 0) && (MoY == 0)) 
			return;

		// Bedast avadane jahati ke mouse harekat karde.
		// Taghsim bar 1000 mikonim ta ziyad dorbin nacharkhad.
		angleY = (float)( (MoX) ) / m_fSpeed;		
		angleZ = (float)( (MoY) ) / m_fSpeed;

		m_fLastRotX = m_fCurRotX;

		m_fCurRotX += angleZ;

		if (m_fCurRotX > 1.0f)
		{
			m_fCurRotX = 1.0f;
			if (m_fLastRotX != 1.0f)
			{
				math::Vector vAxis;
				vAxis.Cross(m_vLook - m_vPos, m_vUp);
				vAxis.Normalize();

				Rotate(1.0f - m_fLastRotX, vAxis);
			} // if (LastRotX != 1.0f)

		} // if (m_fCurRotX > 1.0f)
		else if (m_fCurRotX < -1.0f)
		{
			m_fCurRotX = -1.0f;
			if (m_fLastRotX < -1.0f)
			{
				math::Vector vAxis;
				vAxis.Cross(m_vLook - m_vPos, m_vUp);
				vAxis.Normalize();

				Rotate(-1.0f - m_fLastRotX, vAxis);
			} // if (m_fLastRotX < -1.0f)

		} // if (m_fCurRotX < -1.0f)
		else
		{
			math::Vector vAxis;
			vAxis.Cross(m_vLook - m_vPos, m_vUp);
			vAxis.Normalize();

			Rotate(angleZ, vAxis);
		}
		Rotate(-angleY, math::Vector(0.0f, 1.0f, 0.0f));

	} // RotViewByMouse

	// *********** ****** ** **** *** ******
	// Charkhondan dorbin be dore yek noghte
	// *********** ****** ** **** *** ******
	void Camera::RotateAroundPoint(float angle, const math::Vector *Center, const math::Vector& Axis)
	{
		if (!Center)
			Center = &m_vLook;

		math::Vector vPos = m_vPos - (*Center);
		const float cosTheta = cosf(angle);
		const float sinTheta = sinf(angle);
		math::Vector vNewPos;
		// Find the new x position for the new rotated point
		vNewPos.x  = (cosTheta + (1.0f - cosTheta) * Axis.x * Axis.x)			* vPos.x;
		vNewPos.x += ((1.0f - cosTheta) * Axis.x * Axis.y - Axis.z * sinTheta)	* vPos.y;
		vNewPos.x += ((1.0f - cosTheta) * Axis.x * Axis.z + Axis.y * sinTheta)	* vPos.z;

		// Find the new y position for the new rotated point
		vNewPos.y  = ((1.0f - cosTheta) * Axis.x * Axis.y + Axis.z * sinTheta)	* vPos.x;
		vNewPos.y += (cosTheta + (1.0f - cosTheta) * Axis.y * Axis.y)			* vPos.y;
		vNewPos.y += ((1.0f - cosTheta) * Axis.y * Axis.z - Axis.x * sinTheta)	* vPos.z;

		// Find the new z position for the new rotated point
		vNewPos.z  = ((1.0f - cosTheta) * Axis.x * Axis.z - Axis.y * sinTheta)	* vPos.x;
		vNewPos.z += ((1.0f - cosTheta) * Axis.y * Axis.z + Axis.x * sinTheta)	* vPos.y;
		vNewPos.z += (cosTheta + (1.0f - cosTheta) * Axis.z * Axis.z)			* vPos.z;

		m_vPos = (*Center) + vNewPos;

		CalcViewMatrix();

	} // Rotate Around A Point

	//-------------------------
	// Set the camera position.
	//-------------------------
	void Camera::SetPosition(const math::Vector& v)
	{
		m_pAbsMat->SetTranslation(&v.v[0]);
		m_vPos = v;
		CalcViewMatrix();

	} // SetPosition

	//-----------------------------
	// Returns the camera position.
	//-----------------------------
	const math::Vector& Camera::GetPosition() const
	{
		return m_vPos;

	} // GetPosition

	//------------------------------
	// Sets the camera target point.
	//------------------------------
	void Camera::SetTarget(const math::Vector& v)
	{
		m_vLook = v;
		CalcViewMatrix();

	} // SetTarget

	//---------------------------
	// Returns the camera target.
	//---------------------------
	const math::Vector& Camera::GetTarget() const
	{
		return m_vLook;

	} // GetTarget

	//-----------------------------------------------------------------------------
	// Sets camera up direction
	//-----------------------------------------------------------------------------
	void Camera::SetUpDir( const math::Vector& v )
	{
		m_vUp = v;
		CalcViewMatrix();

	} // SetUpDir

	//-----------------------------------------------------------------------------
	// Sets field of view
	//-----------------------------------------------------------------------------
	void Camera::SetFov(float fov)
	{
		m_fFov = fov;
		m_mProj.SetPerspectiveLH((float)m_vpWidth, 
			(float)m_vpHeight,
			m_fFov,
			m_fNear,
			m_fFar);
		m_bProjChanged = true;
	}

} // sn

} // kge
