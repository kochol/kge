// File name: FmodListener.h
// Des: FMOD
// Date: 7/12/1389 (jalali), Feb-27-2011
// Programmers: Pooya Shahinfar (Pswin)


#ifndef KGE_FMOD_LISTENER_H
#define KGE_FMOD_LISTENER_H

#include "../../../include/sn/Listener.h"

#if KGE_USE_FMOD == 1
#include <fmod.hpp>

namespace kge
{
	namespace sn
	{
		class FmodListener : public Listener
		{
		public:

			//! Constructor
			FmodListener(FMOD::System* system, u32 _index);

			//! Destructor
			virtual ~FmodListener();

			//! Set the listener velocity.
			virtual void SetVelocity(const math::Vector& vel);

			//! Set the listener orientation
			virtual void SetOrientation(const math::Vector& up, const math::Vector& at);

			//! Set the listener gain (default 1)
			virtual void SetGain(float value);

		// virtual methods from SceneNode
		public:
			//! SetPosition
			virtual void SetPosition(const math::Vector& v);

		private:
			FMOD::System*	m_system;
			u32				m_uIndex;
		}; // FmodListener
	} // sn
} // Kochol Game Engine

#endif // KGE_USE_FMOD
#endif // KGE_FMOD_LISTENER_H