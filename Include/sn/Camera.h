// File name: Camera.h
// Des: In class Camera Node hast.
// Date: 3/6/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef CAMERA_H
#define CAMERA_H

#include "../math/Vector.h"
#include "../math/Matrix.h"
#include "../core/Structs.h"
#include "../math/Frustum.h"
#include "SceneNode.h"

namespace kge
{
namespace sn
{
// PreDefine
class SceneManager;

//!
class KGE_API Camera : public SceneNode
{
friend class SceneManager;

public:

	//! Constructor
	Camera(SceneManager* pSnMan,
            const math::Vector &Positon, 
		    const math::Vector &LookAt,
		    const math::Vector &UpDir);

	//! Destructor
	virtual ~Camera();

	//! Karhayee ke Ghabl az render bayad anjam shvad.
	virtual void PreRender(float elapsedTime = 0.0f);

	//! Render kardane Object.
	virtual void Render();

	//! Karhaee ke bad az render bayad anjam beshe.
	virtual void PostRender();

	//! Harekate dorbin be jolo va aghab.
	//  \param Speed Sorate harekat age mosbat bashe jolo mire va age manfi bashe aghab mire.
	virtual void MoveBF(float Speed);

	//! Harekate dorbin be Rast va Chap.
	//  \param Speed Sorate harekat age mosbat bashe Rast mire va age manfi bashe Chap mire.
	virtual void MoveRL(float Speed);

	//! Charkhesh dorbin dor yek bordar.
    //  \param Angle Darajeye charkhesh dorbin bar hasbe radian.
    //  \param Axis Mehvari ke dorbin be dor an micharkhad be tore pishfarz dor mehvare y.
	virtual void Rotate(float Angle, const math::Vector& Axis = math::Vector(0.0f, 1.0f, 0.0f));

	//! Be sorate Automatic dorbin tavasote mouse micharkhad.
	//  \param Enable Agar true ro pass konid ba charkheshe mouse dorbin micharkhad va 
	//        hamishe mouse vasate safhe mimonad.
	//  \param Speed Sorate charkhesh dorbin.
	virtual void AutoRotateByMouse(bool Enable=true, float Speed=1);

    //! Charkhondan dorbin be dore yek noghte ke bishtar be darde bazi haye sevom shakhs mikhore.
    //  \param Angle Zavie ee ke mikhahid becharkhad bar hasbe radian.
    //  \param Center Noghte ee ke dorbin bayad be dore an becharkhad ke agar NULL bashad
    //      Automatic be dore noghte ee ke dorbin darad be an nega mikonad micharkhad.
    //  \param Axis Mehvari ke dorbin bayad be dore an becharkhad.
	virtual void RotateAroundPoint(float Angle, const math::Vector *Center = NULL, 
								   const math::Vector& Axis = math::Vector(0.0f, 1.0f, 0.0f));

	//! Returns camera right direction
	virtual const math::Vector& GetRightDir() { return m_vRight; }

	//! Returns camera up direction
	virtual const math::Vector& GetUpDir() { return m_vUp; }

	//! Sets camera up direction
	virtual void SetUpDir(const math::Vector& v);

	//! Ezafe kardane effect be in dorbin ke felan hich meghdari ro ghabol nemikone.
	//	\param p Effecti ke mikhahid ezafe konid (felan hichi ghabul nemikone).
	//	\param ppOut Class Effect sakhte shode.
	//	\return Felan hamishe false barmigardone.
	virtual bool AddEffect( efx::EffectType p , efx::Effect** ppOut ) { return false; }

	virtual void Draw( bool WithMaterial , bool WithTransform , bool bPosition , bool bNormalTexcoord , bool bTangentBinormal) { }

	//! Set the camera position.
	virtual void SetPosition(const math::Vector& v);

	//! Returns the camera position.
	virtual const math::Vector& GetPosition() const;

	//! Sets the camera target point.
	virtual void SetTarget(const math::Vector& v);

	//! Returns the camera target.
	virtual const math::Vector& GetTarget() const;

	//! Sets the Projection matrix
	virtual void SetProjectionMatrix(const math::Matrix* mat) { m_mProj = *mat; m_bProjChanged = true; }

    //! Returns the view matrix.
    const math::Matrix& GetViewMatrix() const { return m_mView; }

    //! Returns the projection matrix.
    const math::Matrix& GetProjectionMatrix() const { return m_mProj; }

    //! Returns the view frustum.
    const math::Frustum* GetFrustum() const { return &m_frustum; }

    //! 
    int GetViewportWidth() const { return m_vpWidth; }

    //! 
    int GetViewportHeight() const { return m_vpHeight; }

    //! Returns the node type.
    NodeType GetType() const { return ENT_Camera; }

    //! Sets field of view
	void SetFov(float fov);

    //!
	float GetFov() const
	{
		return m_fFov;
	}

    //!
	float GetFarValue() const
	{
		return m_fFar;
	}

    //!
	float GetNearValue() const
	{
		return m_fNear;
	}

	void SetFarValue(float f) 
	{
		m_fFar = f;
	}

	//!
	void SetNearValue(float f) 
	{
		m_fNear = f;
	}

	//!
    bool IsChanged() const { return m_bChanged; }

protected:
    math::Matrix    m_mView;        // View Matrix
    math::Matrix    m_mProj;        // Projection Matrix
	math::Vector	m_vPos,			// Camera position.
					m_vLook,		// Look at point.
					m_vUp,			// Up direction.
					m_vRight;		// Right direction.
	bool			m_bChanged,		// Aya dorbin az frame ghabl taghir karde ast.
					m_bProjChanged,
					m_bARBM;		// AutoRotateByMouse
	int				m_iMiddleX,		// Screen Width/2
					m_iMiddleY;		// Screen Height/2 
	float			m_fCurRotX,		// Current Rotation X
					m_fLastRotX,	// Last Rotation X
					m_fSpeed,		// Sorate charkheshe dorbin.
					m_fFov,
					m_fNear,
					m_fFar;
    math::Frustum   m_frustum;      // View Frustum
    int             m_vpWidth;      // Viewport Width
    int             m_vpHeight;     // Viewport Height

	// Mohasebeye matrixe view
	virtual void CalcViewMatrix();

	// Rotate with mouse.
	virtual void RotViewByMouse();

}; // Camera

} // sn

} // kge

#endif // CAMERA_H
