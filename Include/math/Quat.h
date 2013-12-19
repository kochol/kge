// File name: Quat.h
// Des: In class baraye quaternion hast ke quaternion ha baraye charkhesh estefade
//		mishan va ghabeliat haye besiar ziyadi nesbat be matrix darad.
// Date: 24/7/1385
// Programmer: Ali Akbar Mohamadi(Kochol)

#ifndef QUAT_H
#define QUAT_H

#include "Matrix.h"

namespace kge
{
    namespace math
    {
        class KGE_API Quat
        {
        public:

            union
            {
                //! X Y Z W
                float m_fQuat[4];
                struct
                {
                    float X;
                    float Y;
                    float Z;
                    float W;
                }; // struct
            }; // union

            //! Sazande kahli ke W = 1 va baghie = 0 meghdar dehi mishan.
            Quat();

            //! Sazande ke 4 ta parametr ra migire.
            Quat(float fX, float fY, float fZ, float fW);

            //! Ye chizi to maye haye Lenght vector hast.
            float Magnitude() const
            {
                return sqrtf((X*X)+(Y*Y)+(Z*Z)+(W*W));
            }

            //! Normalize the quaternion.
            void Normalize()
            {
                const float lengthSq = X * X + Y * Y + Z * Z + W * W;

                if (lengthSq > 1.001f || lengthSq < 0.999f)
                {
                    const float invLength = 1.0f / sqrtf(lengthSq);
                    m_fQuat[0] *= invLength;
                    m_fQuat[1] *= invLength;
                    m_fQuat[2] *= invLength;
                    m_fQuat[3] *= invLength;
                }
            }

            //! Jame do ta Quat ba ham.
            Quat operator+(const Quat& rQuat) const;

            //! Menha kardane do ta quat az ham.
            Quat operator-(const Quat& rQuat) const;

            //! Zarbe ye Quat ba ye float.
            Quat operator*(const float fScalar) const;

            //! Taghsime ye Quat ba ye float.
            Quat operator/(const float fScalar) const;

            //! Manfi kardane Quat.
            Quat operator-(void) const;

            //! Az zavie charkheshe dore 3 mehvare X, Y va Z quat ro misaze.
            //! \param fpAngles ye arraye 3 khone e ke betartib zavie charkhes dor mehvare X, Y, Z hastand.
            void FromEulers(const float * fpAngles);

            //! Az zavie charkheshe dore 3 mehvare X, Y va Z quat ro misaze.
            //! \param fX zavie chrkhesh dore mevare X ha.
            //! \param fY zavie chrkhesh dore mevare Y ha.
            //! \param fZ zavie chrkhesh dore mevare Z ha.
            void FromEulers(float fX, float fY, float fZ)
            {
                float fSY = sinf(fZ * 0.5f);
                float fSP = sinf(fY * 0.5f);
                float fSR = sinf(fX * 0.5f);
                float fCY = cosf(fZ * 0.5f);
                float fCP = cosf(fY * 0.5f);
                float fCR = cosf(fX * 0.5f);

                m_fQuat[0] = fSR * fCP * fCY - fCR * fSP * fSY;
                m_fQuat[1] = fCR * fSP * fCY + fSR * fCP * fSY;
                m_fQuat[2] = fCR * fCP * fSY - fSR * fSP * fCY;
                m_fQuat[3] = fCR * fCP * fCY + fSR * fSP * fSY;
            }

            //! Tabdile quat be matrixe 4x4.
            Matrix ToMatrix()
            {
                Matrix Mat;

                Mat.m_fMat[0] = 1.0f - 2.0f * ((m_fQuat[1]*m_fQuat[1]) + (m_fQuat[2]*m_fQuat[2])); 
                Mat.m_fMat[1] = 2.0f * m_fQuat[0] * m_fQuat[1] + 2.0f * m_fQuat[2] * m_fQuat[3];
                Mat.m_fMat[2] = 2.0f * m_fQuat[0] * m_fQuat[2] - 2.0f * m_fQuat[1] * m_fQuat[3];

                Mat.m_fMat[4] = 2.0f * m_fQuat[0] * m_fQuat[1] - 2.0f * m_fQuat[2] * m_fQuat[3];
                Mat.m_fMat[5] = 1.0f - 2.0f * ((m_fQuat[0]*m_fQuat[0]) + (m_fQuat[2]*m_fQuat[2]));
                Mat.m_fMat[6] = 2.0f * m_fQuat[1] * m_fQuat[2] + 2.0f * m_fQuat[0] * m_fQuat[3];

                Mat.m_fMat[8] = 2.0f * m_fQuat[2] * m_fQuat[0] + 2.0f * m_fQuat[1] * m_fQuat[3];
                Mat.m_fMat[9] = 2.0f * m_fQuat[1] * m_fQuat[2] - 2.0f * m_fQuat[0] * m_fQuat[3];
                Mat.m_fMat[10] = 1.0f - 2.0f * ((m_fQuat[0]*m_fQuat[0]) + (m_fQuat[1]*m_fQuat[1]));

                Mat.m_fMat[15] = 1.0f;

                return Mat;
            }

            //! Bedast avardane quati beyne do ta quat.
            //! \param rQuat0 Avalin quat.
            //! \param rQuat1 Dovomin quat.
            //! \param fInterp in meghdari hast ke moshakhas mikonad ke quat hasel be kodom quat nazdik tar bashad va meghdare hasele bayad beyne 0.0f va 1.0f bashad.
            //! \return Quati beyne do ta quat barmigardone.
            static Quat LERP(const Quat& rQuat0, const Quat& rQuat1, float fInterp);

            //! Bedast avardane quati beyne do ta quat ke roye ye kaman bashand. In bishtar baraye rotate ha estefade mishe.
            //! \param rQuat0 Avalin quat.
            //! \param rQuat1 Dovomin quat.
            //! \param fInterp in meghdari hast ke moshakhas mikonad ke quat hasel be kodom quat nazdik tar bashad va meghdare hasele bayad beyne 0.0f va 1.0f bashad.
            //! \return Quati beyne do ta quat barmigardone.
            static Quat SLERP(const Quat& rQuat0, const Quat& rQuat1, float fInterp);

        }; // Quat

    } // math

} // kge

#endif // QUAT_H
