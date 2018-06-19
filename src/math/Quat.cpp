// File name: Quat.h
// Des: In class baraye quaternion hast ke quaternion ha baraye charkhesh estefade
//		mishan va ghabeliat haye besiar ziyadi nesbat be matrix darad.
// Date: 24/7/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#include "../../Include/math/Quat.h"
#include <math.h>
#include <memory.h>



namespace kge
{
namespace math
{
	//----------------------------------------------------------
	// Sazande kahli ke W = 1 va baghie = 0 meghdar dehi mishan.
	//----------------------------------------------------------
	Quat::Quat()
	{
		memset(m_fQuat, 0 , sizeof(float[4]));
		W = 1.0f;
	} // Sazande

	//------------------------------------
	// Sazande ke 4 ta parametr ra migire.
	//------------------------------------
	Quat::Quat(float fX, float fY, float fZ, float fW)
	{
		X = fX;
		Y = fY;
		Z = fZ;
		W = fW;
	} // Sazande(float fX, fY, fZ, fW)

	//------------------------------------------
	// Ye chizi to maye haye Lenght vector hast.
	//------------------------------------------
	//float Quat::Magnitude() const
	//{
	//	return sqrtf((X*X)+(Y*Y)+(Z*Z)+(W*W));
	//} // Magnitude

	//--------------------------
	// Normalize the quaternion.
	//--------------------------
	//void Quat::Normalize()
	//{
 //       float fInvMag = 1.0f / Magnitude();
 //       if(fInvMag != 1.0f)
 //       {
 //           m_fQuat[0] *= fInvMag;
 //           m_fQuat[1] *= fInvMag;
 //           m_fQuat[2] *= fInvMag;
 //           m_fQuat[3] *= fInvMag;
 //       }
	//} // Normalize

	//------------------------
	// Jame do ta Quat ba ham.
	//------------------------
	Quat Quat::operator+(const Quat& rQuat) const
	{
		return Quat(m_fQuat[0] + rQuat.m_fQuat[0], m_fQuat[1] + rQuat.m_fQuat[1],
					m_fQuat[2] + rQuat.m_fQuat[2], m_fQuat[3] + rQuat.m_fQuat[3]);
	} // operator +

	//---------------------------------
	// Menha kardane do ta quat az ham.
	//---------------------------------
	Quat Quat::operator-(const Quat& rQuat) const
	{
		return Quat(m_fQuat[0] - rQuat.m_fQuat[0], m_fQuat[1] - rQuat.m_fQuat[1],
					m_fQuat[2] - rQuat.m_fQuat[2], m_fQuat[3] - rQuat.m_fQuat[3]);
	} // operator - Quat

	//---------------------------
	// Zarbe ye Quat ba ye float.
	//---------------------------
	Quat Quat::operator*(const float fScalar) const
	{
		return Quat(m_fQuat[0] * fScalar, m_fQuat[1] * fScalar, 
					m_fQuat[2] * fScalar, m_fQuat[3] * fScalar);  
	} // operator * float

	//------------------------------
	// Taghsime ye Quat ba ye float.
	//------------------------------
	Quat Quat::operator/(const float fScalar) const
	{
		return Quat(m_fQuat[0] / fScalar, m_fQuat[1] / fScalar, 
					m_fQuat[2] / fScalar, m_fQuat[3] / fScalar);  
	} // operator / float

	//--------------------
	// Manfi kardane Quat.
	//--------------------
	Quat Quat::operator-(void) const
	{
		return Quat(-m_fQuat[0], -m_fQuat[1], -m_fQuat[2], -m_fQuat[3]);
	} // operator - void

	//-------------------------------------------------------------
	// Az zavie charkheshe dore 3 mehvare X, Y va Z quat ro misaze.
	//-------------------------------------------------------------
	void Quat::FromEulers(const float * fpAngles)
	{
		FromEulers(fpAngles[0], fpAngles[1], fpAngles[2]);

	} // FromEulers(float*)

	//-------------------------------------------------------------
	//! Az zavie charkheshe dore 3 mehvare X, Y va Z quat ro misaze.
	//-------------------------------------------------------------
	//void Quat::FromEulers(float fX, float fY, float fZ)
	//{
	//	float fSY = sinf(fZ * 0.5f);
	//	float fSP = sinf(fY * 0.5f);
	//	float fSR = sinf(fX * 0.5f);
	//	float fCY = cosf(fZ * 0.5f);
	//	float fCP = cosf(fY * 0.5f);
	//	float fCR = cosf(fX * 0.5f);

	//	m_fQuat[0] = fSR * fCP * fCY - fCR * fSP * fSY;
	//	m_fQuat[1] = fCR * fSP * fCY + fSR * fCP * fSY;
	//	m_fQuat[2] = fCR * fCP * fSY - fSR * fSP * fCY;
	//	m_fQuat[3] = fCR * fCP * fCY + fSR * fSP * fSY;

	//} // FromEulers(float fX, fY, fZ)

	//-----------------------------
	// Tabdile quat be matrixe 4x4.
	//-----------------------------
	//Matrix Quat::ToMatrix()
	//{
	//	Matrix Mat;

	//	Mat.m_fMat[0] = 1.0f - 2.0f * ((m_fQuat[1]*m_fQuat[1]) + (m_fQuat[2]*m_fQuat[2])); 
	//	Mat.m_fMat[1] = 2.0f * m_fQuat[0] * m_fQuat[1] + 2.0f * m_fQuat[2] * m_fQuat[3];
	//	Mat.m_fMat[2] = 2.0f * m_fQuat[0] * m_fQuat[2] - 2.0f * m_fQuat[1] * m_fQuat[3];

	//	Mat.m_fMat[4] = 2.0f * m_fQuat[0] * m_fQuat[1] - 2.0f * m_fQuat[2] * m_fQuat[3];
	//	Mat.m_fMat[5] = 1.0f - 2.0f * ((m_fQuat[0]*m_fQuat[0]) + (m_fQuat[2]*m_fQuat[2]));
	//	Mat.m_fMat[6] = 2.0f * m_fQuat[1] * m_fQuat[2] + 2.0f * m_fQuat[0] * m_fQuat[3];
	//
	//	Mat.m_fMat[8] = 2.0f * m_fQuat[2] * m_fQuat[0] + 2.0f * m_fQuat[1] * m_fQuat[3];
	//	Mat.m_fMat[9] = 2.0f * m_fQuat[1] * m_fQuat[2] - 2.0f * m_fQuat[0] * m_fQuat[3];
	//	Mat.m_fMat[10] = 1.0f - 2.0f * ((m_fQuat[0]*m_fQuat[0]) + (m_fQuat[1]*m_fQuat[1]));
	//
	//	Mat.m_fMat[15] = 1.0f;

	//	return Mat;
	//} // ToMatrix(4x4)

	//----------------------------------------
	// Bedast avardane quati beyne do ta quat.
	//----------------------------------------
	Quat Quat::LERP(const Quat& rQuat0, const Quat& rQuat1, float fInterp)
	{
		Quat ret = ((rQuat1 - rQuat0) * fInterp) + rQuat0;
		ret.Normalize();
		return ret;
	} // LERP

	//-----------------------------------------------------------------
	// Bedast avardane quati beyne do ta quat ke roye ye kaman bashand.
	//-----------------------------------------------------------------
	Quat Quat::SLERP(const Quat& rQuat0, const Quat& rQuat1, float fInterp)
	{
		//Calculate the dot product
		float fDot = rQuat0.X * rQuat1.X + rQuat0.Y * rQuat1.Y + rQuat0.Z * rQuat1.Z + rQuat0.W * rQuat1.W;

		if (fDot < 1.00001f && fDot > 0.99999f)
		{
			return LERP(rQuat0, rQuat1, fInterp);
		}

		if (fDot < 0.0f)
		{
			float fTheta = acosf(-fDot);
			return (rQuat0 * sinf(fTheta * (1 - fInterp)) + (-rQuat1) * sinf(fTheta * fInterp)) / sinf(fTheta);
		}

		//calculate the angle between the quaternions 
		float fTheta = acosf(fDot);

		return (rQuat0 * sinf(fTheta * (1 - fInterp)) + rQuat1 * sinf(fTheta * fInterp)) / sinf(fTheta);
	} // SLERP

} // math

} // kge