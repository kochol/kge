// File name: soundsystem.h
// Des: interface for the sound systems
// Date: 15/12/1389 (jalali), Mar-06-2011
// Programmer: Pooya Shahinfar(Pswin)

#ifndef FMOD_SOUDN3D_H
#define FMOD_SOUND3D_H

#include "../../../include/kge/sn/Sound3D.h"

#if KGE_USE_FMOD == 1
#include <fmod.hpp>

#include "../../av/fmod/FmodSoundBuffer.h"

// class prototypes
namespace FMOD
{
	class Channel;
	class Sound;
}

namespace kge
{
	namespace sn
	{
		class FmodSound3D : public sn::Sound3D
		{
		public:
			//! Constructor
			FmodSound3D(FMOD::System* system, FMOD::ChannelGroup* channelGroup,
				FMOD::Sound* pSound);

			//! Destructor
			~FmodSound3D();

			//! Set the position of the sound.
			void SetPosition(const math::Vector& v);

			//! Set sound source velocity
			void SetVelocity(const math::Vector& velocity);

			//! Set the sound source volume 
			void SetVolume (float value);

			//! Sets looping of sound
			void SetLoopingState(bool looping);

			//! sound source state
			av::SourceStates GetState() const;

			//! sound source type (static,streaming...)
			av::SourceType GetSoundType() const;

			//! play source (sound)
			void Play();

			//! pause playing source (sound)
			void Pause();

			//! stop playing source (sound)
			void Stop();

			//! set max and min distance
			void SetMinMaxDistance(float _min, float _max = 10000.0f);

			//!
			void SetFading(float value, unsigned int time);

		private:
			FMOD::System*		m_system;
			FMOD::ChannelGroup* m_group;
			FMOD::Sound*		m_pSound;
			FMOD::Channel*		m_pChannel;
			float				m_fMinDistance;
			float				m_fMaxDistance;

		}; // FmodSound3D
} // sn
} // Kochol Game Engine

#endif // KGE_USE_FMOD
#endif // FMOD_SOUND3D_H

