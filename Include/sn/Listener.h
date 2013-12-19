// File name: Listener.h
// Des: This class is for working with sound listener in 3D space.
// Date: 16/4/1387, rewrite: 8/12/1389
// Programmer: Pooya Shahinfar (Pswin)

#ifndef KGE_LISTENER_H
#define KGE_LISTENER_H

#include "../kgedef.h"
#include "Camera.h"
#include "../av/Structs.h"


namespace kge
{
	namespace sn
	{
		class Listener : public SceneNode
		{
		public:

			//! Constructor
			Listener()
				:m_fGain(1.0f)
			{
				m_vVelocity.set(0.0f,0.0f,0.0f);
				m_vUp.set(0.0f,1.0f,0.0f);
				m_vAt.set(0.0f,0.0f,1.0f);
			};

			//! Destructor
			virtual ~Listener() {}

			//! Set the listener velocity.
			virtual void SetVelocity(const math::Vector& vel) = 0;

			//! Get the listener velocity.
			virtual math::Vector GetVelocity() const { return m_vVelocity; }

			//! Set the listener orientation
			virtual void SetOrientation(const math::Vector& up, const math::Vector& at) = 0;

			//! Set the listener gain (default 1)
			virtual void SetGain(float value) = 0;

			//! Get the listener gain
			virtual float GetGain() const { return m_fGain; }

		// virtual methods from SceneNode
		public:
			//! PreRender
			virtual void PreRender(float elapsedTime = 0.0f) {}

			//! Render
			virtual void Render() {}

			//! PostRender
			virtual void PostRender() {}

			//! Draw
			virtual void Draw( bool WithMaterial , bool WithTransform, bool bPosition = true, bool bNormalTexcoord = true, bool bTangentBinormal = true ) {}

			//! SetPosition
			virtual void SetPosition(const math::Vector& v) = 0;

		protected:
			float m_fGain;
			math::Vector m_vVelocity;
			math::Vector m_vUp;
			math::Vector m_vAt;
		}; // Listener
	} // sn
} // Kochol Game Engine

#endif // KGE_LISTENER_H
